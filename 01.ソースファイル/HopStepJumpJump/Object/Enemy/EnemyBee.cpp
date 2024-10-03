#include "EnemyBee.h"

#include "../Circle.h"
#include "../Model.h"

#include "../../Util/CsvLoad.h"
#include "../../Util/Time.h"

#include "../../Shader/ToonShader.h"

namespace
{
	constexpr float kModelScalse = 0.3f;	// モデルスケール
	constexpr float kEnemySize = 20;		// サイズ
	constexpr float kHeight = 20.0f;		// 高さ
}

EnemyBee::EnemyBee(VECTOR pos, VECTOR direction, int turnTime, float speed)
{
	// アニメーションの初期化
	CsvLoad::GetInstance().AnimLoad(m_animData, "Bee");

	m_info.pos = pos;
	// サイズ
	m_objSize = kEnemySize;
	m_pCircle = std::make_shared<Circle>(m_info.pos, kEnemySize, kHeight);
	m_info.rot = VGet(0.0f, 0.0f, 0.0f);

	// ターンするまでの時間
	m_turnTime = std::make_shared<Time>(turnTime);

	InitMoveSpeed(m_statusData.spd);
	m_moveSpeed = speed;
	m_moveDirectionVec = direction;
	m_moveDirectionVec = VScale(direction, m_moveSpeed);
}

EnemyBee::~EnemyBee()
{
	/*処理無し*/
}

void EnemyBee::Init()
{
	// モデルポインタの作成
	m_pModel = std::make_shared<Model>(m_modelH);
	// アニメーションの設定
	m_pModel->SetAnim(m_animData.idle, true, true);
	// スケールの設定
	m_pModel->SetScale(VGet(kModelScalse, kModelScalse, kModelScalse));
	// 角度の初期化
	m_pModel->SetRot(m_info.rot);
	// 座標の初期化
	m_pModel->SetPos(m_info.pos);
}

void EnemyBee::MoveDirectionUpdate()
{
	// 指定時間経過したら方向を反転する
	if (m_turnTime->Update())
	{
		m_moveDirectionVec = VScale(m_moveDirectionVec, -1);
		m_turnTime->Reset();
	}
	// Y軸を省く
	m_moveDirectionVec.y = 0.0f;
}

VECTOR EnemyBee::MoveUpdate()
{
	// 移動方向の更新
	MoveDirectionUpdate();
	// 角度更新
	AngleUpdate();

	return m_moveDirectionVec;
}