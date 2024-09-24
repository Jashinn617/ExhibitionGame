#include "Circle.h"

Circle::Circle(const VECTOR& center, float radius, float height):
	m_center(&center),
	m_radius(radius),
	m_height(height)
{
}

Circle::~Circle()
{
}

void Circle::DebugDraw()
{
#ifdef _DEBUG
	// 真ん中の座標
	VECTOR center = VAdd(*m_center, VGet(0.0f, m_height, 0.0f));
	DrawSphere3D(center, m_radius, 32, 0x00ffff, 0x00ffff, false);

#endif // _DEBUG

}

bool Circle::IsCollide(const std::shared_ptr<Circle> that) const
{
	// 真ん中の座標
	VECTOR center = VAdd(*m_center, VGet(0.0f, m_height, 0.0f));
	// 相手の真ん中の座標
	VECTOR targetCenter = VAdd(*that->m_center, VGet(0.0f, that->m_height, 0.0f));

	float distance = static_cast<float>(fabs(VSize(VSub(targetCenter, center))));
	float radius = m_radius + that->m_radius;

	return distance <= radius;
}
