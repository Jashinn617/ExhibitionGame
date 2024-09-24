#pragma once
#include "SceneBase.h"

#include <array>

class SceneSelect : public SceneBase
{
public:
	enum class NextScene
	{
		My,
		Stage1,
		Stage2,
		Ranking,
		End
	};

public:
	SceneSelect();
	virtual ~SceneSelect();

	virtual void Init();

	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();
	virtual void End();

	virtual SceneKind GetSceneKind() { return SceneKind::StageSelect; }

private:	// 関数
	/// <summary>
	/// セレクトの更新
	/// </summary>
	void  SelectUpdate();

	/// <summary>
	/// セレクトボックスの更新
	/// </summary>
	void SelectBoxUpdate();

	/// <summary>
	/// 背景の描画
	/// </summary>
	void BackDraw();

	void InfoDraw();

	/// <summary>
	/// セレクトボックスの描画
	/// </summary>
	void SelectBoxDraw();

private:	// 変数
	int m_backgroundH;					// 背景画像のハンドル
	int m_backSideH;					// 左右の背景画像ハンドル
	std::array<int, 4> m_SelectBoxH{};	// セレクトボックスのハンドル
	std::array<int, 4> m_textH{};
	int m_scrollX;						// 背景のスクロール値
	int m_scrollY;						// 文字のスクロール値
	int m_textWidth;
	int m_textHeight;
	int m_backWidth;					// 画像の横幅
	int m_backHeight;					// 画像の縦幅
	bool m_isUp;						// 上が選択されているかどうか
	bool m_isLeft;						// 左が選択されているかどうか

	float m_upBoxSize1;
	float m_upBoxSize2;
	float m_downBoxSize1;
	float m_downBoxSize2;

	float m_expansionBoxSize;

	float m_boxSinCount;	// ボックスを拡縮するためのカウント
	float m_boxSinSize;		// ボックスのサイズ計算に使うSin

	NextScene m_next;					// 次のシーン
};

