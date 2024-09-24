#pragma once
#include "DxLib.h"
#include "../Util/Vec2.h"

class ImageBase
{
public:

	ImageBase();
	virtual ~ImageBase();

	virtual void Update() abstract;
	virtual void Draw() abstract;

	virtual void SetUp();

	void SetUp(const char* fileName, Vec2 center);

protected:
	int m_H;			// 阿蔵ハンドル
	Vec2 m_center;		// 中心
	bool m_isDisplay;	// 画像を表示させるかどうか
};

