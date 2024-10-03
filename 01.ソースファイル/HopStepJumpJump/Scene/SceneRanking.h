#pragma once
#include "SceneBase.h"

#include <array>
#include <vector>
#include <memory>

class SceneRanking : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneRanking();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneRanking();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() override final;

	/// <summary>
	/// 更新
	/// </summary>
	/// <returns>次のシーン</returns>
	virtual std::shared_ptr<SceneBase> Update() override final;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() override final;

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void End() override final;

	/// <summary>
	/// シーンの種類の取得
	/// </summary>
	/// <returns>シーンの種類</returns>
	virtual SceneKind GetSceneKind()  override final { return SceneKind::Ranking; }

private:	// 関数
	/// <summary>
	/// ランキングタイムの描画
	/// </summary>
	/// <param name="ranking"></param>
	void DrawRankingTime(int ranking);

	/// <summary>
	/// ランキングの描画
	/// </summary>
	void DrawRanking();

	/// <summary>
	/// ボタンの描画
	/// </summary>
	void DrawButton();

	/// <summary>
	/// 画像のロード処理
	/// </summary>
	void LoadImg();

	/// <summary>
	/// 画像のデリート処理
	/// </summary>
	void DeleteImg();

private:
	int m_backgroundH;	// 背景画像ハンドル
	int m_textBoxH;		// テキストボックス画像ハンドル
	int m_buttonH;		// ボタン画像ハンドル
	int m_arrowH;		// 矢印画像ハンドル
	bool m_isStage1;	// ステージ1のランキングかどうか
	bool m_isArrow;		// 矢印が選ばれているかどうか

	float m_arrowSize;			// 矢印のサイズ
	float m_backButtonSize;		// 戻るボタンのサイズ

	float m_expansionButtonSize;	// 拡縮しているボタンのサイズ
	float m_buttonSinCount;			// 選ばれている場所を拡縮するためのカウント
	float m_buttonSinSize;			// 選ばれている場所のサイズ計算に使うSin



	std::array<int, 11> m_rankingTimeNumH{};	// ランキングタイムの画像ハンドル
	std::array<int, 3> m_rankingH{};			// ランキング画像ハンドル
	std::array<int, 2> m_textH{};				// テキスト画像ハンドル

	std::vector<int> m_stage1Ranking{};			// ステージ1のランキング
	std::vector<int> m_stage2Ranking{};			// ステージ2のランキング
};

