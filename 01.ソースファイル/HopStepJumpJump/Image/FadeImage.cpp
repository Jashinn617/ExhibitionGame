#include "FadeImage.h"

namespace
{
	constexpr int kFadeSpeed = 6;
}

FadeImage::FadeImage():
	m_alpha(0),
	m_maxAlpha(255)
{
}

FadeImage::~FadeImage()
{
	// âÊëúÇÃÉfÉäÅ[Ég
	DeleteGraph(m_H);
}

void FadeImage::Update()
{
	m_alpha = min(m_alpha + kFadeSpeed, m_maxAlpha);
}

void FadeImage::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	DrawRotaGraphF(m_center.x, m_center.y, 1.0f, 0.0f,m_H, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
