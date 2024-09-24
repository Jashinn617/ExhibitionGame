#pragma once
#include "ImageBase.h"
class FadeImage : public ImageBase
{
public:
	FadeImage();
	virtual ~FadeImage();

	void Update() override;
	void Draw() override;

	void SetMaxAlpha(int maxAlpha) { m_maxAlpha = maxAlpha; }

	/// <summary>
	/// フェードが終了したかどうか
	/// </summary>
	/// <returns></returns>
	bool IsFadeEnd() { return m_alpha == m_maxAlpha; }

private:
	int m_alpha;	// 透明度
	int m_maxAlpha;	// 最大透明度
};

