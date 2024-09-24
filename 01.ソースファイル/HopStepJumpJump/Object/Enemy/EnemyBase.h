#pragma once
#include "DxLib.h"
#include "../../Util/CharacterData.h"
#include "../CharacterBase.h"

#include <memory>

class Input;

class Player;
class ObjectBase;
class Model;
class Circle;
class Time;

class EnemyBase :public CharacterBase
{
public:
	EnemyBase();
	EnemyBase(VECTOR pos, float speed);
	EnemyBase(VECTOR pos, VECTOR direction, int turnTime, float speed);
	virtual ~EnemyBase();

	virtual void Init();
	virtual void Update(Input& input) override;

	virtual void Draw(std::shared_ptr<ToonShader> pToonShader);

	/// <summary>
	/// ステージクリア処理
	/// </summary>
	void StageClear() override final;

	/// <summary>
	/// ゲーム終了時処理
	/// </summary>
	void GameEnd() override final;

	void OnDamage(VECTOR targetPos) override;

	void  OnDead();

	/// <summary>
	/// 存在するかどうか
	/// </summary>
	/// <returns></returns>
	bool IsExist()const { return m_info.isExist; }

	/// <summary>
	/// 敵からプレイヤーまでのベクトルの取得
	/// </summary>
	/// <returns></returns>
	VECTOR GetEnemyToPlayerVec()const { return m_enemyToPlayerVec; }

	/// <summary>
	/// 自身の当たり判定の情報
	/// </summary>
	/// <returns></returns>
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
	float m_colHeight;			// 当たり判定の高さをモデルの中心にする
	bool m_isEnd;				// ゲーム終了したかどうか
	bool m_isEndDead;			// ゲーム終了後に消えたかどうか
	VECTOR m_moveDirectionVec;	// 移動方向ベクトル
	VECTOR m_enemyToPlayerVec;	// 敵からプレイヤーまでのベクトル
	std::shared_ptr<Time> m_turnTime;	// ターンするまでの時間
	std::shared_ptr<Time> m_deadTime;	// 死んでからの時間
	std::shared_ptr<Time> m_endTime;	// ゲーム終了してからの時間
	std::shared_ptr<Time> m_endDeadTime;	// ゲーム終了後に消えてからの時間
};
