#pragma once
#include "DxLib.h"
#include "../CharacterBase.h"

#include <memory>

class Time;

class EnemyBase :public CharacterBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyBase();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">初期座標</param>
	/// <param name="speed">スピード</param>
	EnemyBase(VECTOR pos, float speed);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">初期座標</param>
	/// <param name="direction">初期方向</param>
	/// <param name="turnTime">反転するまでの時間</param>
	/// <param name="speed">移動スピード</param>
	EnemyBase(VECTOR pos, VECTOR direction, int turnTime, float speed);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~EnemyBase();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="pToonShader"></param>
	virtual void Draw(std::shared_ptr<ToonShader> pToonShader);

	/// <summary>
	/// ステージクリア処理
	/// </summary>
	void StageClear() override final;

	/// <summary>
	/// ゲーム終了時処理
	/// </summary>
	void GameEnd() override final;

	/// <summary>
	/// ダメージ処理
	/// </summary>
	/// <param name="targetPos">攻撃相手の座標</param>
	void OnDamage(VECTOR targetPos) override;

	/// <summary>
	/// 死亡処理
	/// </summary>
	void  OnDead();

	/// <summary>
	/// 存在するかどうか
	/// </summary>
	/// <returns>存在フラグ</returns>
	bool IsExist()const { return m_info.isExist; }

	/// <summary>
	/// 敵からプレイヤーまでのベクトルの取得
	/// </summary>
	/// <returns>プレイヤーまでのベクトル</returns>
	VECTOR GetEnemyToPlayerVec()const { return m_enemyToPlayerVec; }

	/// <summary>
	/// 自身の当たり判定の情報の取得
	/// </summary>
	/// <returns>当たり判定情報</returns>
	virtual ColType GetColType()const { return ColType::Enemy; }

protected:	// 関数
	/// <summary>
	/// 移動速度の初期化
	/// </summary>
	/// <param name="moveSpeed"></param>
	void InitMoveSpeed(float moveSpeed);

	/// <summary>
	/// 角度の更新
	/// </summary>
	void AngleUpdate();

	/// <summary>
	/// 移動方向の更新
	/// </summary>
	virtual void MoveDirectionUpdate();

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <returns></returns>
	virtual VECTOR MoveUpdate();

protected:	// 変数
	VECTOR m_moveDirectionVec;				// 移動方向ベクトル
	VECTOR m_enemyToPlayerVec;				// 敵からプレイヤーまでのベクトル
	std::shared_ptr<Time> m_turnTime;		// ターンするまでの時間
	std::shared_ptr<Time> m_deadTime;		// 死んでからの時間
	std::shared_ptr<Time> m_endTime;		// ゲーム終了してからの時間
	std::shared_ptr<Time> m_endDeadTime;	// ゲーム終了後に消えてからの時間

private:	// 変数
	float m_colHeight;						// 当たり判定の高さをモデルの中心にする
	bool m_isEndDead;						// ゲーム終了後に消えたかどうか
	bool m_isEnd;							// ゲーム終了したかどうか
};
