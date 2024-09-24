#pragma once
#include "DxLib.h"

#include <memory>

class Model;

class SkyDome
{
public:
	SkyDome();
	virtual ~SkyDome();

	void Update(VECTOR pos);
	void Draw();

private:
	float m_rot;	// 回転率
	std::shared_ptr<Model> m_pModel;	// モデルポインタ
};

