#pragma once
#include "ImageBase.h"
class ExpansionImage : public ImageBase
{
public:
	ExpansionImage();
	virtual ~ExpansionImage();

	void Update() override final;
	void Draw() override final;

	/// <summary>
	/// 拡大処理が終了したかどうか
	/// </summary>
	/// <returns></returns>
	bool IsExpandionEnd()const { return m_isChangeEnd; }

private:
	void FadeInUpdate();
	void NormalUpdate();

private:	// 変数
	int m_alpha;	// 透明度
	double m_size;	// 画像サイズ
	double m_changeSizeSpeed;	// サイズの変更速度
	bool m_isExpanding;	// 拡大中かどうか
	bool m_isChangeEnd;	// 拡大処理が終了したかどうか

	void (ExpansionImage::* m_updateFunc)();	// メンバ関数ポインタ


};

