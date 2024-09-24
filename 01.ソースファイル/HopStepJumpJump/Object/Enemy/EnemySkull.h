#pragma once
#include "EnemyBase.h"

#include <memory>

class EnemySkull : public EnemyBase
{
public:
	EnemySkull(VECTOR pos, float speed);
	~EnemySkull();

	void Init() override final;

	void MoveDirectionUpdate() override final;

	VECTOR MoveUpdate() override final;
};

