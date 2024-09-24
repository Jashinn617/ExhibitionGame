#include "ExpansionImage.h"

namespace
{
	constexpr int kFadeSpeed = 6;
	constexpr double kChangeSizeSpeed = 0.02f;
	constexpr double kMaxChangeSizeSpeed = 0.1f;
	constexpr double kInitSize = 1.4f;	// 初期画像サイズ
	constexpr double kMinSize = 0.8f;	// 最小画像サイズ
	constexpr double kNormalSize = 1.0f;	// 通常画像サイズ
}

ExpansionImage::ExpansionImage():
	m_alpha(0),
	m_size(kInitSize),
	m_changeSizeSpeed(0.0f),
	m_isExpanding(false),
	m_isChangeEnd(false),
	m_updateFunc(&ExpansionImage::FadeInUpdate)
{
}

ExpansionImage::~ExpansionImage()
{
	// 画像のデリート
	DeleteGraph(m_H);
}

void ExpansionImage::Update()
{
	// 状態に合わせて更新処理を行う
	(this->*m_updateFunc)();
}

void ExpansionImage::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	DrawRotaGraph(static_cast<int>(m_center.x), 
		static_cast<int>(m_center.y), 
		m_size,0.0f,m_H, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ExpansionImage::FadeInUpdate()
{
	m_alpha = min(m_alpha + kFadeSpeed, 255);

	//拡大中ではない場合
	if (!m_isExpanding)
	{
		m_changeSizeSpeed = max(m_changeSizeSpeed - kChangeSizeSpeed, -kMaxChangeSizeSpeed);
		m_size = max(m_size + m_changeSizeSpeed, kMinSize);
		if (m_size <= kMinSize)
		{
			m_isExpanding = true;
		}
	}
	//拡大中の場合
	else if (m_isExpanding)
	{
		m_changeSizeSpeed = min(m_changeSizeSpeed + kChangeSizeSpeed, kMaxChangeSizeSpeed);
		m_size = min(m_size + m_changeSizeSpeed, kNormalSize);
	}

	//拡大中かつ、透明度が255より大きいかつ、通常サイズになった場合
	if (m_size >= kNormalSize && m_alpha >= 255 && m_isExpanding)
	{
		m_updateFunc = &ExpansionImage::NormalUpdate;
		m_isChangeEnd = true;
	}
}

void ExpansionImage::NormalUpdate()
{
}
