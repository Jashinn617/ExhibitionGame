#pragma once
#include "SceneBase.h"

#include <array>

class SceneSelect : public SceneBase
{
public:
	// 次のシーン
	enum class NextScene
	{
		My,			// 現在シーン
		Stage1,		// ステージ1
		Stage2,		// ステージ2
		Ranking,	// ランキング
		End			// 終了
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneSelect();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneSelect();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init()  override final;

	/// <summary>
	/// 更新
	/// </summary>
	/// <returns>次シーン</returns>
	virtual std::shared_ptr<SceneBase> Update()  override final;
	
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw()  override final;

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void End()  override final;

	/// <summary>
	/// シーンの種類の取得
	/// </summary>
	/// <returns>シーンの種類</returns>
	virtual SceneKind GetSceneKind()  override final { return SceneKind::StageSelect; }

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

	/// <summary>
	/// 横の説明の描画
	/// </summary>
	void InfoDraw();

	/// <summary>
	/// セレクトボックスの描画
	/// </summary>
	void SelectBoxDraw();

	/// <summary>
	/// 画像のロード
	/// </summary>
	void LoadImg();

	/// <summary>
	/// 画像のデリート
	/// </summary>
	void DeleteImg();

private:	// 変数
	int m_backgroundH;					// 背景画像のハンドル
	int m_backSideH;					// 左右の背景画像ハンドル
	std::array<int, 4> m_SelectBoxH{};	// セレクトボックス画像のハンドル
	std::array<int, 4> m_textH{};		// テキスト画像のハンドル
	int m_scrollX;						// 背景のスクロール値
	int m_scrollY;						// 文字のスクロール値
	int m_textWidth;					// テキストの横幅
	int m_textHeight;					// テキストの縦幅
	int m_backWidth;					// 画像の横幅
	int m_backHeight;					// 画像の縦幅
	float m_upBoxSize1;					// 上のボックスのサイズ1
	float m_upBoxSize2;					// 上のボックスのサイズ2
	float m_downBoxSize1;				// 下のボックスのサイズ1
	float m_downBoxSize2;				// 下のボックスのサイズ2
	float m_expansionBoxSize;			// ボックスの拡縮サイズ
	float m_boxSinCount;	// ボックスを拡縮するためのカウント
	float m_boxSinSize;		// ボックスのサイズ計算に使うSin
	bool m_isUp;						// 上が選択されているかどうか
	bool m_isLeft;						// 左が選択されているかどうか
	NextScene m_next;					// 次のシーン
};

