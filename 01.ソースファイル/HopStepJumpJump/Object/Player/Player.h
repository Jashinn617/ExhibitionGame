#pragma once
#include "DxLib.h"
#include "../../Util/CharacterData.h"
#include "../CharacterBase.h"

#include <array>
#include <memory>
#include <vector>

class Input;
class Camera;
class StageTest;

class Model;
class Circle;
class PlayerState;
class DamageShader;

/// <summary>
/// プレイヤークラス
/// </summary>
class Player : public CharacterBase
{
public:
	Player();
	~Player();

	void Init();
	void Update() override final;
	virtual void Draw(std::shared_ptr<ToonShader> pToonShader);

	void Draw2D() override final;

	void StageClear() override final;

	void GameEnd() override final;

	/// <summary>
	/// 攻撃を受けた
	/// </summary>
	/// <param name="targetPos"></param>
	void OnDamage(VECTOR targetPos) override final;

	/// <summary>
	/// 攻撃をした
	/// </summary>
	void OnAttack() override final;

	/// <summary>
	/// ジャンプ状態の終了
	/// </summary>
	void EndJump() override final;	

	/// <summary>
	/// ジャンプ力の設定
	/// </summary>
	/// <param name="jumpPower"></param>
	void SetJumpPower(float jumpPower) { m_jumpPower = jumpPower; }

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns></returns>
	const VECTOR& GetPos()const { return m_info.pos; }

	/// <summary>
	/// 当たり判定の種類の取得
	/// </summary>
	/// <returns></returns>
	ColType GetColType()const { return ColType::Player; }

	/// <summary>
	/// カメラの取得
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<Camera> GetCamera() const { return m_pCamera; }

private:	// 関数
	/// <summary>
	/// 角度の更新
	/// </summary>
	void AngleUpdate();

	/// <summary>
	/// 移動方向の更新
	/// </summary>
	void MoveDirectionUpdate();

	/// <summary>
	/// 移動
	/// </summary>
	/// <returns></returns>
	VECTOR MoveUpdate();

private:	// メンバ関数用関数

	/*各状態の初期化*/
	void IdleInit() {};
	void WalkInit() {};
	void DashInit() {};
	void JumpInit();
	void KnockBackInit() {};
	void StageClearInit() {};

	/*各状態の更新*/
	void IdleUpdate();
	void WalkUpdate();
	void DashUpdate();
	void JumpUpdate();
	void KnockBackUpdate();
	void StageClearUpdate();

private:	// 変数
	float m_walkSpeed;	// 移動速度
	float m_dashSpeed;	// ダッシュ時の速度
	float m_acc;	// 加速度

	bool m_isStageClear;	// ステージをクリアしたかどうか

	VECTOR m_moveDirectVec;	// 移動方向ベクトル
	VECTOR m_cameraToPlayerVec;	// プレイヤーからカメラまでの距離


	std::shared_ptr<PlayerState> m_pState;		// 状態のポインタ
	std::shared_ptr<Time> m_pInvincibleTime;	// 攻撃を食らった後の無敵時間
	std::shared_ptr<Camera> m_pCamera;			// カメラポインタの取得
};

