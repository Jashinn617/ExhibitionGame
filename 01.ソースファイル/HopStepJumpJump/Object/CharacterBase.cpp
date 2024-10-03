#include "CharacterBase.h"
#include "Circle.h"

#include "Item/ItemBase.h"


namespace
{
	constexpr int kMaxColHitPolyNum = 2000;	// 最大当たり判定ポリゴン数
	constexpr int kMaxColHitTryNum = 16;	// 壁に押し出す処理の最大試行回数
	constexpr float kColHitSlideLength = 1.0f;	// 壁押し出し時にスライドさせる距離
	constexpr float kGravity = 0.8f;	// 重力
	constexpr float kFallMaxSpeed = -15.0f;	// 最大落下速度
	constexpr float kChangeAngleSpeed = 0.25f;	// 滑らかに角度を移動させる速度
	constexpr float kAngleDiffMax = DX_PI_F + 0.3f;	// 角度の最大差
	constexpr float kAngleDiffMin = -DX_PI_F - 0.3f;	// 角度の最小差
	constexpr float kSlideLength = 10.0f;	// 押し出しの大きさ	
}

CharacterBase::CharacterBase():
	m_moveData{},
	m_animData{},
	m_jumpPower(0.0f),
	m_isJump(false),
	m_isDead(false)
{
	/*処理無し*/
}

CharacterBase::~CharacterBase()
{
	/*処理無し*/
}

void CharacterBase::Init()
{
	/*処理無し*/
}

void CharacterBase::Update()
{
	/*処理無し*/
}

void CharacterBase::Draw(std::shared_ptr<ToonShader> pToonShader)
{
	/*処理無し*/
}

void CharacterBase::OnDamage(VECTOR targetPos)
{
	/*処理無し*/
}

void CharacterBase::MoveCollCharacterUpdate(CharacterBase* pTarget)
{
	// 自分かターゲットが死んでいるときは判定をしない
	if (m_isDead || pTarget->IsDead())
	{
		return;
	}

	// オブジェクト同士の当たり判定
	if (!GetCircle()->IsCollide(pTarget->GetCircle())) return;

	// 移動前の座標の設定
	VECTOR prevPos = VSub(m_info.pos, m_info.vec);

	// 移動後の座標
	VECTOR nextPos;

	// ターゲットのオブジェクトを考慮して移動する
	VECTOR slideVec;

	// 進行方向とターゲットの座標から自分の座標を引いたベクトルに垂直なベクトルを出す
	slideVec = VCross(m_info.vec, VNorm(VSub(m_info.pos, pTarget->GetInfo().pos)));

	// ↑で出したベクトルとターゲットの座標から
	// 自分の座標を引いたベクトルに垂直なベクトルを出して
	// 元の移動成分からターゲットオブジェクトの方向の移動成分を抜いたベクトルを出す
	slideVec = VCross(VNorm(VSub(m_info.pos, pTarget->GetInfo().pos)), slideVec);

	// 壁方向の移動成分を抜いたベクトルを足す
	nextPos = VAdd(prevPos, slideVec);

	// 押し出し処理をする
	for (int i = 0; i < kMaxColHitTryNum; i++)
	{
		// オブジェクトと当たっていなかったら処理を抜ける
		if (!GetCircle()->IsCollide(pTarget->GetCircle())) break;

		VECTOR move = VSub(m_info.pos, pTarget->GetInfo().pos);

		float vecSize = VSize(move);

		move = VScale(move, (1.0f / (vecSize * kSlideLength)));

		// 当たっていたら規定距離文プレイヤーを敵と反対の方向に移動させる
		nextPos = VAdd(nextPos, move);

		m_info.pos = nextPos;
	}
	// 新しい座標を保存する
	m_info.pos = nextPos;
}

void CharacterBase::AttackPlayerCollEnemy(CharacterBase* pEnemy)
{
	// 敵が死んでいたら判定をしない
	if (pEnemy->IsDead()) return;
	// オブジェクト同士の当たり判定
	if (!GetCircle()->IsCollide(pEnemy->GetCircle())) return;

	OnAttack();
	pEnemy->OnDamage(m_info.pos);
}

void CharacterBase::AttackEnemyCollPlayer(CharacterBase* pPlayer)
{
	// プレイヤーが攻撃を食らっている最中だった場合は判定をしない
	if (pPlayer->IsDamage()) return;
	// オブジェクト同士の当たり判定
	if (!GetCircle()->IsCollide(pPlayer->GetCircle())) return;

	pPlayer->OnDamage(m_info.pos);
}

void CharacterBase::PlayerToItem(CharacterBase* pItem)
{
	// オブジェクト同士の当たり判定
	if (!GetCircle()->IsCollide(pItem->GetCircle())) return;

	// アイテムをゲットする
	dynamic_cast<ItemBase*>(pItem)->OnGet();
}

void CharacterBase::EndJump()
{
	m_isJump = false;
	m_jumpPower = 0.0f;
}

void CharacterBase::HitGroundUpdate()
{
	m_jumpPower = 0.0f;
}

void CharacterBase::GravityUpdate()
{
	// 移動量の更新
	// 落ちる速度が最大落下速度を超えないようにする
	m_jumpPower = max(m_jumpPower - (kGravity * 0.5f), kFallMaxSpeed);

	// 落下
	m_info.vec.y = m_jumpPower;
}

void CharacterBase::SmoothAngle(float& nowAngle, float nextAngle)
{
	// -180度以下になったら角度が大きくなりすぎないように360度足す
	if (nextAngle < -DX_PI_F)
	{
		nextAngle += DX_TWO_PI_F;
	}
	// 180度以上になったら角度が小さくなりすぎないように360度引く
	if (nextAngle > DX_PI_F)
	{
		nextAngle -= DX_TWO_PI_F;
	}

	// 角度の差を求める
	float angleDiff = nowAngle - nextAngle;

	// 角度の差が180度以上だった場合
	if (angleDiff > kAngleDiffMax)
	{
		// 角度の差が小さくなりすぎないように360度引く
		nowAngle -= DX_TWO_PI_F;
	}
	// 以下だった場合
	else if (angleDiff < kAngleDiffMin)
	{
		// 角度の差が角度が大きくなりすぎないように360度足す
		nowAngle += DX_TWO_PI_F;
	}

	// 角度の差が少ない場合は何もせずにそのまま次の角度を返す
	if (angleDiff < kChangeAngleSpeed && 
		angleDiff > -kChangeAngleSpeed)
	{
		nowAngle = nextAngle;
		return;
	}
	// それ以外の場合は少しずつ角度を変化させる
	// 角度の差が大きい時はkChangeAngleSpeedを引く
	else if (angleDiff > kChangeAngleSpeed)
	{
		nowAngle -= kChangeAngleSpeed;
		return;
	}
	// 角度の差が小さいときはkChangeAngleSpeedを足す
	else if (angleDiff < kChangeAngleSpeed)
	{
		nowAngle += kChangeAngleSpeed;
		return;
	}
}
