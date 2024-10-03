#include "EnemyBase.h"

#include "../Player/Player.h"

#include "../ObjectBase.h"
#include "../Model.h"
#include "../Circle.h"

#include "../../Util/Time.h"
#include "../../Util/SoundManager.h"

#include "../../Effect/Effekseer3DManager.h"

#include "../../Shader/ToonShader.h"

#include <assert.h>

using namespace CharacterData;

namespace
{
	constexpr int kDeadExistTime = 60;		// 死んでから完全に消えるまでの時間
	constexpr float kColHeight = -10.0f;	// 当たり判定の高さ
	constexpr float kHeadPos = 20;			// 敵の頭の位置
	constexpr float kEndTime = 80;			// ゲーム終了してからの時間
	constexpr float kEndDeadTime = 200;		// ゲーム終了後に消えてからの時間
}

EnemyBase::EnemyBase() :
	m_colHeight(0),
	m_isEnd(false),
	m_isEndDead(false),
	m_moveDirectionVec{ 0,0,0 },
	m_enemyToPlayerVec{ 0,0,0 },
	m_deadTime(std::make_shared<Time>(kDeadExistTime)),
	m_endTime(std::make_shared<Time>(kEndTime)),
	m_endDeadTime(std::make_shared<Time>(kEndDeadTime))
{
	m_info.vec = VGet(0.0f, 0.0f, 0.0f);
	m_info.rot = VGet(0.0f, 0.0f, 0.0f);
	m_info.modelH = -1;
	m_info.isExist = true;

	m_statusData.spd = 0.0f;
	m_objSize = 0.0f;
}

EnemyBase::EnemyBase(VECTOR pos, float speed) :
	m_colHeight(0),
	m_isEnd(false),
	m_isEndDead(false),
	m_moveDirectionVec{ 0,0,0 },
	m_enemyToPlayerVec{ 0,0,0 },
	m_deadTime(std::make_shared<Time>(kDeadExistTime)),
	m_endTime(std::make_shared<Time>(kEndTime)),
	m_endDeadTime(std::make_shared<Time>(kEndDeadTime))
{
	m_info.pos = pos;
	m_info.vec = VGet(0.0f, 0.0f, 0.0f);
	m_info.rot = VGet(0.0f, 0.0f, 0.0f);
	m_info.modelH = -1;
	m_info.isExist = true;

	m_statusData.spd = 0.0f;
	m_objSize = 0.0f;
}

EnemyBase::EnemyBase(VECTOR pos, VECTOR direction, int turnTime, float speed) :
	m_colHeight(0),
	m_isEnd(false),
	m_isEndDead(false),
	m_moveDirectionVec{ 0,0,0 },
	m_enemyToPlayerVec{ 0,0,0 },
	m_deadTime(std::make_shared<Time>(kDeadExistTime)),
	m_endTime(std::make_shared<Time>(kEndTime)),
	m_endDeadTime(std::make_shared<Time>(kEndDeadTime))
{
	m_info.pos = pos;
	m_info.vec = VGet(0.0f, 0.0f, 0.0f);
	m_info.rot = VGet(0.0f, 0.0f, 0.0f);
	m_info.modelH = -1;
	m_info.isExist = true;

	m_statusData.spd = 0.0f;
	m_objSize = 0.0f;
}


EnemyBase::~EnemyBase()
{
	/*処理無し*/
}

void EnemyBase::Init()
{
	/*処理無し*/
}

void EnemyBase::Update()
{
	if (m_isEnd)
	{
		// 移動値を0にする
		m_info.vec = VGet(0.0f, 0.0f, 0.0f);
	}
	else
	{
		// 移動ベクトルの計算
		m_info.vec = MoveUpdate();

		// モデル更新
		m_pModel->Update();

		// 移動ベクトルを現在座標に足す
		m_info.pos = VAdd(m_info.pos, m_info.vec);

		// 座標の設定
		m_pModel->SetPos(m_info.pos);

		// 角度更新
		m_pModel->SetRot(VGet(0.0f, m_angle * DX_PI_F, 0.0f));
	}
}

void EnemyBase::Draw(std::shared_ptr<ToonShader> pToonShader)
{
	// モデルの描画
	for (int i = 0; i < MV1GetTriangleListNum(m_pModel->GetModelHandle()); i++)
	{
		// シェーダの頂点タイプの取得
		int shaderType = MV1GetTriangleListVertexType(m_pModel->GetModelHandle(), i);

		pToonShader->SetShader(shaderType);
		MV1DrawTriangleList(m_pModel->GetModelHandle(), i);
	}
	pToonShader->ShaderEnd();

	// 当たり判定の描画
	m_pCircle->DebugDraw();
}

void EnemyBase::StageClear()
{
	/*処理無し*/
}

void EnemyBase::GameEnd()
{
	// ゲームの終了フラグを立てる
	m_isEnd = true;

	// 一定時間たったら消える
	if (m_endTime->Update())
	{
		// エフェクトを流す
		Effekseer3DManager::GetInstance().Add("GameEndDead", 
			Effekseer3DManager::PlayType::Normal, this, m_info.pos);

		// 死亡処理
		OnDead();

		// 同じSEが鳴ってなかった場合
		if(!SoundManager::GetInstance().IsDesignationCheckPlaySound("EnemyEndDead"))
		{
			// SEを鳴らす
			SoundManager::GetInstance().Play("EnemyEndDead");
			
		}
		
		// ゲーム終了時の処理を終える
		m_isGameEnd = true;
	}	
}

void EnemyBase::OnDamage(VECTOR targetPos)
{
	if (m_isEnd) return;
	// プレイヤーのY位置が自分の位置より低かった場合
	if (targetPos.y <= m_info.pos.y + kHeadPos) return;

	// 攻撃音を鳴らす
	SoundManager::GetInstance().Play("Attack");

	// エフェクトを流す
	Effekseer3DManager::GetInstance().Add("EnemyDead"
		, Effekseer3DManager::PlayType::Normal, this, m_info.pos);
	
	// 死亡処理
	OnDead();
}

void EnemyBase::OnDead()
{
	// 死亡したフラグを立てる
	m_isDead = true;
	// 存在を消す
	m_info.isExist = false;
}

void EnemyBase::InitMoveSpeed(float moveSpeed)
{
	// 移動速度の初期化
	m_moveData.walkSpeed = moveSpeed;
	m_moveData.runSpeed = 0.0f;
	m_moveData.acceleration = 0.0f;
}

void EnemyBase::AngleUpdate()
{
	// atan2で角度の計算をする為Z軸を反転する
	float angleZ = m_moveDirectionVec.z * -1;
	//向きたい方向の設定
	float nextAngle = atan2(m_moveDirectionVec.x, angleZ);

	//滑らかに指定した方向に向く
	SmoothAngle(m_angle, nextAngle);
}

void EnemyBase::MoveDirectionUpdate()
{
	/*処理無し*/
}

VECTOR EnemyBase::MoveUpdate()
{
	/*処理無し*/
	return VGet(0.0f, 0.0f, 0.0f);
}