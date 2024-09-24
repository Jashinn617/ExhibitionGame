#pragma once
#include "SceneBase.h"

#include <array>
#include <vector>
#include <memory>

class Ranking;

class SceneRanking : public SceneBase
{
public:
	SceneRanking();
	virtual ~SceneRanking();

	virtual void Init();

	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();
	virtual void End();
	virtual SceneKind GetSceneKind() { return SceneKind::Ranking; }

private:
	void RankTimeDraw(int ranking);
	void RankingDraw();

	void ButtonDraw();

private:
	int m_backgroundH;	// 背景画像ハンドル
	int m_textBoxH;		// テキストボックス画像ハンドル
	int m_buttonH;		// ボタン画像ハンドル
	int m_arrowH;		// 矢印画像ハンドル
	bool m_isStage1;	// ステージ1のランキングかどうか
	bool m_isArrow;		// 矢印が選ばれているかどうか

	float m_arrowSize;
	float m_backButtonSize;

	float m_expansionButtonSize;
	float m_buttonSinCount;		// 選ばれている場所を拡縮するためのカウント
	float m_buttonSinSize;		// 選ばれている場所のサイズ計算に使うSin



	std::array<int, 11> m_rankingTimeNumH{};	// ランキングタイムの画像ハンドル
	std::array<int, 3> m_rankingH{};			// ランキング画像ハンドル
	std::array<int, 2> m_textH{};				// テキスト画像ハンドル

	std::vector<int> m_stage1Ranking{};
	std::vector<int> m_stage2Ranking{};

	std::shared_ptr<Ranking> m_pRanking;

};

