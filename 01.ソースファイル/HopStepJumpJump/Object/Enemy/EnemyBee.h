#pragma once
#include "EnemyBase.h"

#include <memory>

class EnemyBee : public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">初期位置</param>
	/// <param name="direction">初期方向</param>
	/// <param name="turnTime">反転するまでの時間</param>
	/// <param name="speed">移動スピード</param>
	EnemyBee(VECTOR pos, VECTOR direction, int turnTime, float speed);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyBee();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override final;

	/// <summary>
	/// 移動方向更新
	/// </summary>
	void MoveDirectionUpdate() override final;

	/// <summary>
	/// 移動更新
	/// </summary>
	/// <returns></returns>
	VECTOR MoveUpdate() override final;
};

