#pragma once
#include "EnemyBase.h"

#include <memory>

class EnemyBee : public EnemyBase
{
public:
	EnemyBee(VECTOR pos, VECTOR direction, int turnTime, float speed);
	~EnemyBee();

	void Init()override final;

	void MoveDirectionUpdate() override final;

	VECTOR MoveUpdate() override final;
};

