#include "DxLib.h"
#include "Player.h"
#include "PlayerState.h"

#include "../ObjectManager.h"
#include "../ObjectBase.h"
#include "../Model.h"
#include "../Camera.h"
#include "../Circle.h"

#include "../../Util/Pad.h"
#include "../../Util/Time.h"
#include "../../Util/MoveDirectionVec.h"
#include "../../Util/CsvLoad.h"
#include "../../Util/SoundManager.h"

#include "../../Shader/ToonShader.h"

#include "../../Effect/Effekseer3DManager.h"

#include <math.h>
#include <cassert>

namespace
{
	const char* const kPlayerFileName = "Data/Model/Player/Character.mv1";	// プレイヤーモデルファイル名
	constexpr float kSize = 15.0f;					// プレイヤーサイズ
	constexpr float kModelSize = 0.11f;				// プレイヤーモデルのサイズ
	constexpr float kFallSEPosY = -300.0f;		// 落下SEが流れるY位置
	constexpr float kFallBackPosY = -800.0f;		// スタート地点に戻される位置Y
	/*スピード関係*/
	constexpr float kMoveSpeedDashRate = 1.2f;		// 走っているときのスピード
	constexpr float kAccelerationRate = 0.5f;		// 加速度
	/*ジャンプ関係*/
	constexpr float kJumpMaxSpeed = 8.0f;			// ジャンプ時の最大速度
	constexpr float kGravity = 0.8f;				// 重力
	constexpr float kFallMaxSpeed = -15.0f;			// 落下最大速度
	constexpr float kAttackHitJumpSpeed = 3.0f;		// 敵を踏んだ際に跳ねる速度
	/*ジャンプ時のアニメーション変更用変数*/
	constexpr float kJumpRise = 1.0f;
	constexpr float kJumpIdleNum = -5.0f;
	constexpr float kJumpDown = -15.0f;

	constexpr float kPlayerAngleSpeed = 0.02f;			// プレイヤーの回転速度
	constexpr int kInvinvibleTime = 60;					// 無敵時間
	constexpr float kKnockBackSpeed = 10.0f;			// ノックバックのスピード
	constexpr float kKnockBackSpeedDecrease = 0.2f;		// ノックバックスピード減少量
	constexpr float kHeight = 35.0f;					// 高さ

	/*移動の補完値*/
	constexpr float kNowVecNum = 0.8f;
	constexpr float kNextVecNum = 0.2f;

	constexpr int kStageClearAnim = 16;					// ステージクリアアニメーション

	constexpr float kDrawDistance = 50.0f;				// 描画可能距離

	constexpr float kEnemyHeadPos = 20;					// 敵の頭の位置

	constexpr VECTOR kStartPos = { 0.0f, -180.0f, 0.0f };

	/*アニメーション速度*/
	enum kAnimSpeed
	{
		Idle = 2,
		Walk = 2,
		Dash = 1,
		Jump = 1,
		KnockBack = 1,
		StageClear = 1,
	};
}

Player::Player() :
	m_walkSpeed(0),
	m_dashSpeed(0),
	m_acc(0),
	m_isStageClear(false),
	m_moveDirectVec{ 0,0,0 },
	m_cameraToPlayerVec{ 0,0,0 }
{
	// アニメーションの初期化
	CsvLoad::GetInstance().AnimLoad(m_animData, "Player");

	// 移動スピードの初期化
	m_walkSpeed = m_statusData.spd;
	m_dashSpeed = m_statusData.spd * kMoveSpeedDashRate;
	m_acc = m_statusData.spd * kAccelerationRate;
	// 情報初期化
	m_info.pos = kStartPos;
	m_info.vec = VGet(0.0f, 0.0f, 0.0f);
	m_info.rot = VGet(0.0f, 0.0f, 0.0f);
	m_info.modelH = -1;
	m_info.isExist = true;
	m_objSize = kSize;
	m_angle = 0.0f;

	/*ポインタの作成*/
	m_pState = std::make_shared<PlayerState>();
	m_pInvincibleTime = std::make_shared<Time>(kInvinvibleTime);
	m_pCamera = std::make_shared<Camera>();
	m_pCircle = std::make_shared<Circle>(m_info.pos, kSize, kHeight * 0.5f);
	//m_pDamageShader = std::make_shared<DamageShader>();

	m_pModel = std::make_shared<Model>(kPlayerFileName);
	m_pModel->SetAnim(m_animData.idle, false, true);
	m_pModel->SetScale(VGet(kModelSize, kModelSize, kModelSize));


	/*状態の追加*/
	// 待機
	m_pState->AddState([=] {IdleInit(); },
		[=] {IdleUpdate(); },
		PlayerState::StateKind::Idle);
	// 歩き
	m_pState->AddState([=] {WalkInit(); },
		[=] {WalkUpdate(); },
		PlayerState::StateKind::Walk);
	// ダッシュ
	m_pState->AddState([=] {DashInit(); },
		[=] {DashUpdate(); },
		PlayerState::StateKind::Dash);
	// ジャンプ
	m_pState->AddState([=] {JumpInit(); },
		[=] {JumpUpdate(); },
		PlayerState::StateKind::Jump);
	// ノックバック
	m_pState->AddState([=] {KnockBackInit(); },
		[=] {KnockBackUpdate(); },
		PlayerState::StateKind::KnockBack);
	// ステージクリア
	m_pState->AddState([=] {StageClearInit(); },
		[=] {StageClearUpdate(); },
		PlayerState::StateKind::StateClear);

	// 初期状態の設定
	// 初期は待機状態から始まる
	m_pState->SetState(PlayerState::StateKind::Idle);
}

Player::~Player()
{
	/*処理無し*/
}

void Player::Init()
{
	/*処理無し*/
}

void Player::Update()
{
	// ステージクリア時
	if (m_isStageClear)
	{
		// カメラをステージクリア用にする
		m_pCamera->StageClearUpdate();
	}

	// 状態更新
	m_pState->Update();

	// 移動更新
	m_info.vec = MoveUpdate();

	// 重力を考慮した更新
	GravityUpdate();

	// 攻撃を受けた時
	if (m_isDamage)
	{
		// 無敵時間を計測して無敵時間が終わったらダメージを受ける状態を解除する
		if (m_pInvincibleTime->Update())
		{
			m_pInvincibleTime->Reset();
			m_isDamage = false;
		}
	}

	// アニメーションの更新
	m_pModel->Update();

	// モデルの座標を設定する
	m_pModel->SetPos(m_info.pos);

	// モデルの角度を設定する
	m_pModel->SetRot(VGet(0.0f, m_angle, 0.0f));

	// カメラの更新
	m_pCamera->Update(m_info.pos);

	// プレイヤーからカメラまでの距離の更新
	m_cameraToPlayerVec = VSub(m_info.pos, m_pCamera->GetPos());

	// 落ちたら初期位置に戻る
	if (m_info.pos.y <= kFallSEPosY)
	{
		if (!m_isStageClear)
		{
			// 落下SEを流す
			if (!SoundManager::GetInstance().IsDesignationCheckPlaySound("Fall"))
			{
				SoundManager::GetInstance().Play("Fall");
			}
		}
		

		if (m_info.pos.y <= kFallBackPosY)
		{
			m_info.pos = VGet(0.0f, 0.0f, 0.0f);
			EndJump();
		}
	}
	
}

void Player::Draw(std::shared_ptr<ToonShader> pToonShader)
{
	 //モデルのフレームごとに描画をする
	for (int i = 0; i < MV1GetTriangleListNum(m_pModel->GetModelHandle()); i++)
	{
		int shaderType = MV1GetTriangleListVertexType(m_pModel->GetModelHandle(), i);

		pToonShader->SetShader(shaderType);

		MV1DrawTriangleList(m_pModel->GetModelHandle(), i);
	}
	pToonShader->ShaderEnd();

	m_pCircle->DebugDraw();
}

void Player::Draw2D()
{
	m_pCamera->Draw();

#ifdef _DEBUG
	DrawFormatString(0, 20, 0x000000, "プレイヤー座標：%f,%f,%f"
		, m_info.pos.x, m_info.pos.y, m_info.pos.z);
	DrawFormatString(0, 100, 0x000000, "ジャンプ力：%f", m_jumpPower);
#endif // _DEBUG
}

void Player::StageClear()
{
	m_angle = 0.0f;

	m_isStageClear = true;
	m_pCamera->StageClear(m_angle, m_info.pos);
	m_pState->StageClear();
}

void Player::GameEnd()
{
	m_isGameEnd = true;

	// 重力を考慮した更新
	GravityUpdate();

	// アニメーションの更新
	m_pModel->Update();

	// モデルの座標を設定する
	m_pModel->SetPos(m_info.pos);

	// モデルの角度を設定する
	m_pModel->SetRot(VGet(0.0f, m_angle, 0.0f));

	// カメラの更新
	m_pCamera->Update(m_info.pos);

	// プレイヤーからカメラまでの距離の更新
	m_cameraToPlayerVec = VSub(m_info.pos, m_pCamera->GetPos());
}

void Player::OnDamage(VECTOR targetPos)
{
	// ゲームクリア状態だったら何もせずに終了する
	if (m_pObjectManager->IsGameClear())return;
	if (m_pObjectManager->IsGameEnd())return;

	// 敵のY位置が自分の位置より低かった場合は何もしない
	if (targetPos.y <= m_info.pos.y - kEnemyHeadPos)return;

	// ダメージ中は何もしない
	if (m_isDamage) return;

	// ノックバックのスピードと方向を計算する
	m_moveSpeed = kKnockBackSpeed;
	m_isDamage = true;

	m_pState->OnDamage();

	m_moveDirectVec = VNorm(VSub(
		VGet(m_info.pos.x, 0.0f, m_info.pos.z),
		VGet(targetPos.x, 0.0f, targetPos.z)));

	// ダメージ音を鳴らす
	SoundManager::GetInstance().Play("Damage");

	// エフェクトを流す
	Effekseer3DManager::GetInstance().Add("Damage", Effekseer3DManager::PlayType::Normal, this, m_info.pos);

}

void Player::OnAttack()
{
	m_jumpPower = kAttackHitJumpSpeed;
	m_pState->SetState(PlayerState::StateKind::Jump);
}

void Player::EndJump()
{
	m_isJump = false;
	m_jumpPower = 0.0f;

	// ステージクリア時は処理を終了する
	if (m_pState->GetState() == PlayerState::StateKind::StateClear) return;

	m_pState->EndState();
}

void Player::AngleUpdate()
{
	// ノックバック中は処理をしない
	if (m_pState->GetState() == PlayerState::StateKind::KnockBack) return;

	float nextAngle = 0;

	nextAngle = atan2(m_moveDirectVec.z, m_moveDirectVec.x)
		+ DX_PI_F * 0.5f + m_pCamera->GetCameraAngleX();

	SmoothAngle(m_angle, nextAngle);
}

void Player::MoveDirectionUpdate()
{
	if (m_isGameEnd) return;

	// ノックバック中は処理をせずに終了する
	if (m_pState->GetState() == PlayerState::StateKind::KnockBack) return;

	// 移動方向ベクトルクラスを生成
	MoveDirectionVec moveVec;

	// 移動方向アップデート
	moveVec.Update();

	// 進みたい方向と今の方向の線形補完
	m_moveDirectVec = VAdd(VScale(m_moveDirectVec, kNowVecNum),
		VScale(moveVec.GetDirectionVec(), kNextVecNum));

	// 進みたい方向のY軸を省く
	m_moveDirectVec.y = 0.0f;
}

VECTOR Player::MoveUpdate()
{
	if (m_isGameEnd) return VGet(0.0f, 0.0f, 0.0f);


	// 移動スピードが0の場合は移動しない
	if (m_moveSpeed == 0.0f)return VGet(0.0f, 0.0f, 0.0f);

	// 移動方向の更新
	MoveDirectionUpdate();
	// 角度の更新
	AngleUpdate();

	// 移動ベクトル生成
	VECTOR move = VNorm(m_moveDirectVec);

	// ノックバック中の処理
	if (m_pState->GetState() == PlayerState::StateKind::KnockBack)
	{
		move.x *= m_moveSpeed;
		move.z *= m_moveSpeed;

		return move;
	}

	// 移動ベクトルにスピードをかける
	// X軸は反転させる
	move.x *= -m_moveSpeed;
	move.z *= m_moveSpeed;

	// カメラの角度によって進む方向を変える
	MATRIX playerRotMtx = MGetRotY(m_pCamera->GetCameraAngleX());

	// 移動ベクトルとカメラ角度行列を乗算する
	move = VTransform(move, playerRotMtx);

	return move;
}

void Player::JumpInit()
{
	if (m_isGameEnd) return;

	m_isJump = true;
	m_jumpPower = kJumpMaxSpeed;
	// ジャンプ音を鳴らす
	SoundManager::GetInstance().Play("Jump");
}

void Player::IdleUpdate()
{
	m_moveSpeed = max(m_moveSpeed - m_acc, 0.0f);

	// アニメーションを待機状態に変更する
	m_pModel->ChangeAnim(m_animData.idle, true, false, kAnimSpeed::Idle);
}

void Player::WalkUpdate()
{
	if (m_isGameEnd) return;

	// X軸方向の移動ベクトルに歩きスピードを代入する
	m_moveSpeed = min(m_moveSpeed + m_acc, m_walkSpeed);

	// アニメーションを歩きアニメーションに変更する
	m_pModel->ChangeAnim(m_animData.walk, true, false, kAnimSpeed::Walk);

	// 歩きサウンドが流れていなかった場合
	if (!SoundManager::GetInstance().IsDesignationCheckPlaySound("Walk"))
	{
		// 足音を鳴らす
		SoundManager::GetInstance().Play("Walk");
	}
	

}

void Player::DashUpdate()
{
	if (m_isGameEnd) return;

	m_moveSpeed = m_dashSpeed;

	// アニメーションをダッシュモーションに変更する
	m_pModel->ChangeAnim(m_animData.run, true, false, kAnimSpeed::Dash);

	// 歩きサウンドが流れていなかった場合
	if (!SoundManager::GetInstance().IsDesignationCheckPlaySound("Dash"))
	{
		// 足音を鳴らす(ダッシュ時)
		SoundManager::GetInstance().Play("Dash");
	}
	
}

void Player::JumpUpdate()
{
	if (m_isGameEnd) return;
	// 上昇中
	if (m_jumpPower > kJumpRise)
	{
		m_pModel->ChangeAnim(m_animData.jumpStart, false, false, kAnimSpeed::Jump);
	}
	// 空中にとどまっている、下降中
	else
	{
		m_pModel->ChangeAnim(m_animData.jumpIdle, false, false, kAnimSpeed::Jump);
	}
}

void Player::KnockBackUpdate()
{
	m_moveSpeed = max(m_moveSpeed - kKnockBackSpeedDecrease, 0.0f);
	m_pModel->ChangeAnim(m_animData.knockBack, false, false, kAnimSpeed::KnockBack);
	// アニメーションが終わったら待機状態に戻る
	if (m_pModel->IsAnimEnd())
	{
		m_pModel->ChangeAnim(m_animData.idle, true, false, kAnimSpeed::Idle);
		m_pState->EndState();
	}
}

void Player::StageClearUpdate()
{
	m_moveSpeed = 0.0f;
	// モデルをステージクリア状態に変更する
	m_pModel->ChangeAnim(kStageClearAnim, true, false, kAnimSpeed::StageClear);
}