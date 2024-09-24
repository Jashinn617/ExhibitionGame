#pragma once
#include "DxLib.h"

#include <math.h>
#include <memory>

class Circle
{
public:
	Circle(const VECTOR& center, float radius, float height);

	virtual ~Circle();

	// デバッグ表示
	void DebugDraw();

	// 衝突判定
	bool IsCollide(const std::shared_ptr<Circle> that)const;

	const VECTOR& GetCenter() const { return *m_center; }

	float GetRadius()const { return m_radius; }

private:
	const VECTOR* m_center;	// 中心座標
	const float m_radius;	// 半径
	const float m_height;	// 高さ	
};

