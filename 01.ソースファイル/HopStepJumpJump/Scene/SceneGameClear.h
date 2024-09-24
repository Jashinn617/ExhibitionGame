#pragma once
#include "DxLib.h"
#include "SceneBase.h"
#include "../Util/Game.h"

#include <memory>
#include <vector>
#include <array>

class Input;
class StageSceneManager;
class Time;

class SceneGameClear : public SceneBase
{
public:
	// ランク
	enum class Rank
	{
		S,
		A,
		B,
		C,
		RankNum,
	};
public:
	SceneGameClear(std::shared_ptr<StageSceneManager> pStageSceneManager, Game::Stage stageKind);
	~SceneGameClear();

	virtual void Init() override final;
	virtual std::shared_ptr<SceneBase> Update(Input& input) override final;
	virtual void Draw() override final;
	virtual void End() override final;

	// シーンの種類の取得
	virtual SceneKind GetSceneKind() { return SceneKind::GameClear; }

private:	// 関数
	void TextDraw();
	void TimeDraw();
	void RankingDraw();
	void RankDraw();
	void RankTimeDraw(int ranking);
	void MyRankTimeDraw(int ranking);
	void ButtonDraw();
	
	void ChangeMainScene();
	void ChangeTitleScene();

private:	// 変数
	int m_clearTime;		// ステージのクリア時間
	bool m_isSceneEnd;	// シーンが終了したかどうか
	bool m_isNextScene;	// 次のシーンに移動可能かどうか
	bool m_isMyRank;	// 自分のタイムが反映されているか
	bool m_isFall;		// 落下中か
	
	int m_plusPosY;		// 引くY座標

	std::array<int, 11> m_myTimeNumH{};	// 自身のクリアタイムの画像ハンドル
	std::array<int, 11> m_rankingTimeNumH{};	// ランキングのタイムの画像ハンドル
	std::array<int, 4> m_rankH{};			// ランク画像ハンドル
	std::array<int, 3> m_textH{};	// テキスト画像ハンドル
	std::array<int, 3> m_rankingH{};	// ランキング画像ハンドル
	std::array<int, 2> m_textBoxH{};	// テキストボックス画像ハンドル
	std::array<int, 2> m_buttonH{};		// ボタンの画像ハンドル

	Rank m_rank;			// ランク

	std::vector<int> m_ranking{};	// ランキング
	Game::Stage m_stageKind;		// ステージの種類

	std::shared_ptr<StageSceneManager> m_pStageSceneManager;
	std::shared_ptr<Time> m_pImageDrawTime;
	std::shared_ptr<Time> m_pStageClearSETime;	// ステージクリア時のサウンドの開始時間
};

