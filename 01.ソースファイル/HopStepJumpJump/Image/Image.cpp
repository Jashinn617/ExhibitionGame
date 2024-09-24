#include "Image.h"

Image::Image()
{
	/*処理無し*/
}

Image::~Image()
{
	// 画像のデリート
	DeleteGraph(m_H);
}

void Image::Update()
{
	/*処理無し*/
}

void Image::Draw()
{
	// 画像の表示
	DrawRotaGraphF(m_center.x, m_center.y, 1.0f, 0.0f, m_H, true);
}
