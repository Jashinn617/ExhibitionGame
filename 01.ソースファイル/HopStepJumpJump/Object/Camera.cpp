#include "Camera.h"

#include "Circle.h"
#include "Model.h"
#include "ObjectBase.h"

#include "Player/Player.h"

#include "../Util/Time.h"
#include "../Util/Easing.h"

namespace
{
	/*通常時*/
	constexpr float kNear = 10.0f;	// カメラのnear
	constexpr float kFar = 7000.0f;	// カメラのfar
	constexpr int kMaxInputNum = 1000;	// アナログ入力状態最大値

	constexpr float kCameraAngleSpeedX = 0.1f;	// カメラの旋回スピードX
	constexpr float kCameraAngleSpeedY = 0.05f;	// カメラの旋回スピードY
	
	constexpr float kCameraAngleVMax = DX_PI_F / 2.0f - 0.8f;	// カメラの最大角度
	constexpr float kCameraAngleVMin = -DX_PI_F / 2.0f + 0.6f;	// カメラの最小角度
	
	constexpr float kCameraPlayerTargetHeight = 65.0f;		// カメラの注視点(プレイヤー座標からの高さ)
	
	constexpr float kCameraToPlayerLenghtMax = 175.0f;		// カメラからプレイヤーまでの最大距離
	constexpr float kCameraToPlayerLenghtMin = 0.0f;		// カメラからプレイヤーまでの最小距離
	
	constexpr float kCameraToPlayerLenghtSpeed = 5.0f;		// カメラの距離を変えるスピード
	
	constexpr float kSize = 10.0f;							// カメラのサイズ(当たり判定用)
	
	constexpr int kMaxColHitTryNum = 50;					// カメラの押し出し試行回数
	
	/*カメラのついてくる速度*/
	constexpr float kCameraFollowSpeed = 0.2f;
	constexpr float kPrevCameraFollowSpeed = 1.0f - kCameraFollowSpeed;
	
	/*カメラの注視点を追いかける速度*/
	constexpr float kCameraTargetFollowSpeed = 0.2f;
	constexpr float kPrevCameraTargetFollowSpeed = 1.0f - kCameraTargetFollowSpeed;

	/*ステージクリア時*/
	constexpr float kStageClearStartAngleH = 0.0f;	// ステージクリア時の最初のカメラの角度H
	constexpr float kStageClearStartAngleV = 0.15f;	// ステージクリア時の最初のカメラの角度V
	constexpr float kStageClearEndAngleH = DX_TWO_PI_F + DX_PI_F + DX_PI_F * 2.0f;	// ステージクリア時の最後のカメラの角度H
	constexpr float kStageClearEndAngleV = -0.35f;	// ステージクリア時の最後のカメラの角度V
	constexpr float kEasingTime = 80.0f;	// イージングにかかる時間
	constexpr float kStageClearCameraTargtHeight = kCameraPlayerTargetHeight * 0.5f;	// ステージクリア時の注視点の高さ
	constexpr float kStageClearTargetMoveLength = 43.0f;	// ステージクリア時のターゲットの移動量
	constexpr float kStageClearTargetStartMoveTime = 200.0f;	// ステージをクリアしてからターゲットが動き出すまでの時間
	constexpr float kStageClearTargetLength = 80.0f;		// ステージクリア時のカメラからターゲットまでの距離
	
	/*ステージクリア時のイージングにかかる時間*/
	constexpr float kStageClearEasingTime =  650.0f;
	constexpr float kStageClearTargetMoveTime = 600.0f;
}

Camera::Camera() :
	m_angleH(0.0f),
	m_angleV(0.0f),
	m_cameraToTargetLenght(kCameraToPlayerLenghtMax),
	m_clearCameraToTargetLength(kStageClearTargetLength),
	m_stageClearEasingTime(0.0f),
	m_stageClearTargetEasingTime(0.0f),
	m_isStageClear(false),
	m_pos{0,0,0},
	m_nextPos{0,0,0},
	m_prevPos{0,0,0},
	m_targetPos{0,0,0},
	m_stageClearTargetStartPos{0,0,0},
	m_isMove(false),
	m_isHit(false),
	m_pPoly(nullptr)
{
	m_rotX = MGetRotX(m_angleV);
	m_rotY = MGetRotY(m_angleH);

	/*ポインタの生成*/
	m_pCircle = std::make_shared<Circle>(m_nextPos, kSize, 0.0f);
	m_pClearTargetStartMoveTime = std::make_shared<Time>(kStageClearTargetStartMoveTime);
}

Camera::~Camera()
{
	/*処理無し*/
}

void Camera::Init()
{
	/*処理無し*/
}

void Camera::Update(VECTOR playerPos)
{
	// ステージクリア時は何もしない
	if (m_isStageClear) return;
	// 更新前の座標の設定
	m_prevPos = m_nextPos;

	// ターゲットの座標の設定
	VECTOR targetPos;
	targetPos = playerPos;
	targetPos.y += kCameraPlayerTargetHeight;

	// 角度更新
	UpdateAngle();

	// 座標更新
	NormalUpdate(targetPos);
	
	// 座標の確定
	UpdatePos();
}

void Camera::Draw()
{
#ifdef _DEBUG
	DrawFormatString(0, 0, 0x000000, "カメラ座標：%f,%f,%f", m_prevPos.x, m_prevPos.y, m_prevPos.z);
	DrawFormatString(0, 40, 0x000000, "ターゲット座標：%f,%f,%f", m_targetPos.x, m_targetPos.y, m_targetPos.z);
	DrawFormatString(0, 120, 0x000000, "GetTargetPos:%f,%f,%f", GetCameraTarget().x, GetCameraTarget().y, GetCameraTarget().z);
#endif // _DEBUG

}

void Camera::ResetCamera()
{
	// 表示場所の設定
	SetCameraNearFar(kNear, kFar);
	SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);
}

void Camera::ColUpdate(ObjectBase* pField)
{
	// カメラの周囲にあるステージポリゴンを取得する
	// 検出する範囲は移動距離も考慮する
	m_hitDim = MV1CollCheck_Capsule(pField->GetModel()->GetModelHandle(), 
		-1, m_nextPos, m_prevPos,
		m_pCircle->GetRadius());

	if (m_hitDim.HitNum == 0)
	{
		// 検出したプレイヤーの周囲のポリゴン情報を開放する
		MV1CollResultPolyDimTerminate(m_hitDim);

		m_cameraToTargetLenght = min(m_cameraToTargetLenght + kCameraToPlayerLenghtSpeed,
			kCameraToPlayerLenghtMax);

		// カメラの座標を決定する
		UpdatePos();

		m_hitDim = MV1CollCheck_Capsule(pField->GetModel()->GetModelHandle(),
			-1, m_nextPos, m_prevPos,
			m_pCircle->GetRadius());
	}

	for (int i = 0; i < m_hitDim.HitNum; i++)
	{
		m_pPolyIndex[i] = &m_hitDim.Dim[i];
	}

	// カメラの押し出し処理
	FixPosInternal();

	// 検出したプレイヤーの周囲のポリゴン情報を開放する
	MV1CollResultPolyDimTerminate(m_hitDim);

	// カメラの座標を決定する
	UpdatePos();

	// カメラの情報を反映させる
	m_pos = VAdd(VScale(m_prevPos, kPrevCameraFollowSpeed), VScale(m_nextPos, kCameraFollowSpeed));
	SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);
}

void Camera::StageClearUpdate()
{
	// ステージのクリア時間が一定以上経過したらイージング処理をする
	if (m_pClearTargetStartMoveTime->Update())
	{
		// イージングを利用して計算する
		m_targetPos.x = Easing::EaseOutCubic(m_stageClearTargetEasingTime, m_stageClearTargetStartPos.x,
			m_stageClearTargetEndPos.x, kStageClearTargetMoveTime);
		m_targetPos.y = Easing::EaseOutCubic(m_stageClearTargetEasingTime, m_stageClearTargetStartPos.y,
			m_stageClearTargetEndPos.y, kStageClearTargetMoveTime);
		m_targetPos.z = Easing::EaseOutCubic(m_stageClearTargetEasingTime, m_stageClearTargetStartPos.z,
			m_stageClearTargetEndPos.z, kStageClearTargetMoveTime);
	}
	m_stageClearEasingTime = min(m_stageClearEasingTime + 1.0f, kStageClearEasingTime);

	// ステージクリア時のカメラ角度の更新
	m_angleH = Easing::EaseOutCubic(m_stageClearEasingTime, kStageClearStartAngleH,
		kStageClearEndAngleH, kStageClearEasingTime);
	m_angleV = Easing::EaseOutCubic(m_stageClearEasingTime, kStageClearStartAngleV,
		kStageClearEndAngleV, kStageClearEasingTime);

	// ステージクリア時のカメラからターゲットまでの距離の更新
	m_cameraToTargetLenght = Easing::EaseOutCubic(m_stageClearEasingTime, kCameraToPlayerLenghtMax,
		m_clearCameraToTargetLength, kStageClearEasingTime);

	// カメラ座標の更新
	NormalUpdate(m_targetPos);
	UpdatePos();

	// 更新する前の座標を前フレーム座標に代入
	m_pos = m_nextPos;
}

void Camera::StageClear(float angle, VECTOR targetPos)
{
	// クリアフラグをtrueにする
	m_isStageClear = true;

	/*注視点の設定*/
	m_targetPos = targetPos;
	// ターゲットより少し高い位置に注視点を置く
	m_targetPos.y += kStageClearCameraTargtHeight;

	// 角度の初期化
	m_angleH = kStageClearStartAngleH;
	m_angleV = kStageClearStartAngleV;

	NormalUpdate(m_targetPos);

	// カメラの座標の更新
	UpdatePos();

	// 左方向ベクトルの算出
	m_leftVec = VNorm(VCross(VGet(0.0f, 1.0f, 0.0f),
		VNorm(VSub(VGet(m_targetPos.x, 0.0f, m_targetPos.z),
			VGet(m_nextPos.x, 0.0f, m_nextPos.z)))));

	// ステージクリア時のカメラの座標の設定
	m_stageClearTargetStartPos = m_targetPos;
	// カメラの最終座標の設定
	m_stageClearTargetEndPos = VAdd(m_targetPos, VScale(m_leftVec, kStageClearTargetLength));
	// 向いている方向の傾きに最終座標を合わせる
	MATRIX RotY = MGetRotY(kStageClearEndAngleH);
	m_stageClearTargetEndPos = VTransform(m_stageClearTargetEndPos, RotY);
	// 反転させる
	m_stageClearTargetEndPos.x *= -1.0f;
	m_stageClearTargetEndPos.z *= -1.0f;
}

void Camera::UpdateAngle()
{
	// パッドのアナログ情報の取得
	DINPUT_JOYSTATE input;

	// 入力情報の初期化
	input.Rx = 0;
	input.Ry = 0;

	// 入力情報の取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	// 回転を1～1000から0.001～1の範囲にする
	// 後でカメラ感度も用意する(一旦3にしておく)
	float rotX = 0.00020f * input.Rx * 3;
	float rotY = 0.00017f * input.Ry * 3;

	// カメラの回転スピードをかける
	rotX *= kCameraAngleSpeedX;
	rotY *= kCameraAngleSpeedY;

	m_angleH += rotX;
	if (input.Rx < 0.0f)
	{
		// -180°以下になったら角度地が大きくなりすぎないように360°を足す
		if (m_angleH < -DX_PI_F)
		{
			m_angleH += DX_TWO_PI_F;
		}
	}
	if (input.Rx > 0.0f)
	{
		// 180°以上になったら角度地が大きくなりすぎないように360°を引く
		if (m_angleH > DX_PI_F)
		{
			m_angleH -= DX_TWO_PI_F;
		}
	}

	m_angleV -= rotY;
	// 一定角度以下ににはならないようにする
	if (m_angleV < kCameraAngleVMin)
	{
		m_angleV = kCameraAngleVMin;
	}
	// 一定角度以上にはならないようにする
	if (m_angleV > kCameraAngleVMax)
	{
		m_angleV = kCameraAngleVMax;
	}
}

void Camera::NormalUpdate(VECTOR targetPos)
{
	m_targetPos.x = (m_targetPos.x * kPrevCameraTargetFollowSpeed) + (targetPos.x * kCameraTargetFollowSpeed);
	m_targetPos.y = (m_targetPos.y * kPrevCameraTargetFollowSpeed) + (targetPos.y * kCameraTargetFollowSpeed);
	m_targetPos.z = (m_targetPos.z * kPrevCameraTargetFollowSpeed) + (targetPos.z * kCameraTargetFollowSpeed);
}

void Camera::UpdatePos()
{
	// 垂直方向の回転(X軸)
	MATRIX RotX = MGetRotX(m_angleV);
	// 水平方向の回転(Y軸)
	MATRIX RotY = MGetRotY(m_angleH);

	// カメラの座標の計算
	// (X軸にカメラとプレイヤーとの距離分だけ伸びたベクトルを
	// 垂直方向回転させた後に水平方向回転して
	// それに注視点の座標を足す)
	m_nextPos = VAdd(VTransform(VTransform(VGet(0.0f, 0.0f, m_cameraToTargetLenght), RotX),RotY),m_targetPos);
}

void Camera::FixPos()
{
	// 壁に当たったかどうかのフラグをfalseにしておく
	m_isHit = false;

	// 移動したかどうか
	if (m_isMove)
	{
		// 壁ポリゴンの数だけ繰り返す
		for (int i = 0; i < m_hitDim.HitNum; i++)
		{
			// i番目の壁ポリゴンのアドレスを壁ポチゴンポインタ配列から取得
			m_pPoly = m_pPolyIndex[i];

			// ポリゴンとプレイヤーが当たっていなかったら次のカウントへ
			if (HitCheck_Capsule_Triangle(m_nextPos, m_prevPos, m_pCircle->GetRadius(),
				m_pPoly->Position[0], m_pPoly->Position[1],
				m_pPoly->Position[2]) == false) continue;

			// ここに来たら当たっている
			m_isHit = true;

			// カメラが当たっているとき
			while (m_isHit)
			{
				m_cameraToTargetLenght = max(m_cameraToTargetLenght--, kCameraToPlayerLenghtMin);
				// カメラの座標更新
				UpdatePos();

				// ポリゴンとプレイヤーが当たっていなかったらループから抜ける
				if (HitCheck_Capsule_Triangle(m_nextPos, m_prevPos, m_pCircle->GetRadius(),
					m_pPoly->Position[0], m_pPoly->Position[1],
					m_pPoly->Position[2]) == false)
				{
					m_isHit = false;
				}
			}

			// 新たな移動座標で壁ポリゴンと当たっていないかどうかを判定する
			bool isHitWallPolygom = false;
			for (int j = 0; j < m_hitDim.HitNum; j++)
			{
				// j番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
				m_pPoly = m_pPolyIndex[j];

				// 当たっていたらループから抜ける
				if (HitCheck_Capsule_Triangle(m_nextPos, m_prevPos, m_pCircle->GetRadius(),
					m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2]) == false)
				{
					isHitWallPolygom = true;
					break;
				}
			}

			// すべてのポリゴンと当たっていなかったらループから抜ける
			if (!isHitWallPolygom)
			{
				m_isHit = false;
				break;
			}
		}
	}
	else // 移動していなかった場合
	{
		// 壁ポリゴンの数だけ繰り返す
		for (int i = 0; i < m_hitDim.HitNum; i++)
		{
			// i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
			m_pPoly = m_pPolyIndex[i];

			// ポリゴンに当たっていたあr当たったフラグを立てたうえでループから抜ける
			if (HitCheck_Capsule_Triangle(m_nextPos, m_prevPos, m_pCircle->GetRadius(),
				m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2]) == false)
			{
				m_isHit = true;
				break;
			}
		}
	}
}

void Camera::FixPosInternal()
{
	// 壁からの押し出し処理を試みる最大数だけ繰り返す
	for (int i = 0; i < kMaxColHitTryNum; i++)
	{
		// 当たる可能性のある壁ポリゴンをすべて見る
		bool isHitWall = false;

		// 壁ポリゴンの数だけ繰り返す
		for (int j = 0; j < m_hitDim.HitNum; j++)
		{
			// j番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
			m_pPoly = m_pPolyIndex[j];

			m_cameraToTargetLenght = max(m_cameraToTargetLenght--, kCameraToPlayerLenghtMin);

			// カメラの座標更新
			UpdatePos();

			// 移動したうえで壁ポリゴンと接触しているかどうかを判定
			for (int k = 0; k < m_hitDim.HitNum; k++)
			{
				// 当たったらループから抜ける
				m_pPoly = m_pPolyIndex[k]; if (HitCheck_Capsule_Triangle(m_nextPos, m_prevPos, m_pCircle->GetRadius(),
					m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2]) == false)
				{
					isHitWall = true;
					break;
				}
			}

			// すべてのポリゴンと当たっていなかったらループから抜ける
			if (!isHitWall) break;
		}
		// ループ終了
		if (!isHitWall)break;
	}
}
