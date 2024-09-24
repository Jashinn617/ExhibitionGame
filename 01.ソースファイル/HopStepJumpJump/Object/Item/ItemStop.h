#pragma once
#include "ItemBase.h"

#include <memory>

class ItemStop : public ItemBase
{
public:
	ItemStop(VECTOR pos);
	~ItemStop();

	void Init()override final;

	//void Draw(std::shared_ptr<ToonShader> pToonShader)override final;

	void MoveDirectionUpdate() override final;

	VECTOR MoveUpdate() override final;

private:
	// 角度の更新
	virtual void AngleUpdate();

private:
	float m_angleX;		// X角度
	float m_moveSinCount;	// 移動カウント
};

