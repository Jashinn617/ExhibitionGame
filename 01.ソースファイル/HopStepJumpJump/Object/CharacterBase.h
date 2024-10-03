#pragma once
#include "ObjectBase.h"

class Input;
class Circle;

class CharacterBase : public ObjectBase
{
public:

	// ステータス情報
	struct Status
	{
		float spd = 5.0f;	// スピード
	};

	// 移動情報
	struct MoveData
	{
		float walkSpeed = 0;	// 歩きスピード
		float runSpeed = 0;		// 走りスピード
		float acceleration = 0;	// 加速度
		float rotSpeed = 0;		// 回転スピード
	};

	// アニメーション情報
	struct AnimData
	{
		int8_t idle = 0;		// 待機
		int8_t walk = 0;		// 歩き
		int8_t jumpStart = 0;	// ジャンプ時
		int8_t jumpIdle = 0;	// ジャンプ中
		int8_t run = 0;			// 走り
		int8_t knockBack = 0;	// ノックバック
	};

public:
	CharacterBase();
	virtual ~CharacterBase();

	virtual void Init();
	virtual void Update();

	virtual void Draw(std::shared_ptr<ToonShader> pToonShader);

	/// <summary>
	/// 攻撃を受けた時の処理
	/// </summary>
	/// <param name="targetPos">攻撃を当てた相手の座標</param>
	virtual void OnDamage(VECTOR targetPos);

	/// <summary>
	/// 角度の取得
	/// </summary>
	/// <returns>角度</returns>
	float GetAngle() const { return m_angle; }

	/// <summary>
	/// ダメージフラグの取得
	/// </summary>
	/// <returns></returns>
	bool IsDamage()const { return m_isDamage; }

	/// <summary>
	/// 攻撃を受けてない状態に戻す
	/// </summary>
	void OffDamageFlag() { m_isDamage = false; }

	/// <summary>
	/// ジャンプ状態かどうか
	/// </summary>
	/// <returns></returns>
	bool IsJump()const { return m_isJump; }

	/// <summary>
	/// ジャンプ力を返す
	/// </summary>
	/// <returns></returns>
	float GetJumpPower()const { return m_jumpPower; }

	/// <summary>
	/// 死んだかどうか
	/// </summary>
	/// <returns></returns>
	bool IsDead()const { return m_isDead; }

	/// <summary>
	/// 円の当たり半径ポインタの取得
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<Circle> GetCircle()const { return m_pCircle; }

	/// <summary>
	/// キャラクター同士の当たり判定を考慮した移動更新
	/// </summary>
	/// <param name="pTarget"></param>
	void MoveCollCharacterUpdate(CharacterBase* pTarget);

	/// <summary>
	/// プレイヤーの攻撃が敵に当たった
	/// </summary>
	/// <param name="pEnemy"></param>
	void AttackPlayerCollEnemy(CharacterBase* pEnemy);

	/// <summary>
	/// 敵の攻撃がプレイヤーに当たった
	/// </summary>
	/// <param name="pPlayer"></param>
	void AttackEnemyCollPlayer(CharacterBase* pPlayer);

	/// <summary>
	/// プレイヤーがアイテムに当たった
	/// </summary>
	/// <param name="pItem"></param>
	void PlayerToItem(CharacterBase* pItem);

	/// <summary>
	/// ジャンプ状態の終了処理
	/// </summary>
	virtual void EndJump();

	/// <summary>
	/// 地面に当たっているときの処理
	/// </summary>
	void HitGroundUpdate();

	/// <summary>
	/// 重力による落下処理
	/// </summary>
	void GravityUpdate();

protected:	// 関数
	/// <summary>
	/// 角度を滑らかに変化させる関数
	/// </summary>
	/// <param name="nowAngle"></param>
	/// <param name="nextAngle"></param>
	void SmoothAngle(float& nowAngle, float nextAngle);

protected:	// 変数
	Status m_statusData;	// ステータス情報
	MoveData m_moveData;	// 移動情報
	AnimData m_animData;	// アニメーション情報

	std::shared_ptr<Circle> m_pCircle;	// 当たり判定の円のポインタ
	
	float m_jumpPower;	// ジャンプ力
	bool m_isJump;	// ジャンプ中かどうか
	bool m_isDead;	// 死んだかどうか
};

