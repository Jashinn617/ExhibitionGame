#pragma once
#include "../Util/Game.h"

#include <memory>
#include <vector>
#include <array>
#include <functional>

class Input;
class ObjectManager;
class CountTime;
class Ranking;
class Time;

class StageSceneManager
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
	// クリア後のシーンのタイプ
	enum class ClearSceneType
	{
		LeftImgDraw,		// 左画像の描画
		ClearTimeMeasure,	// タイム計測
		RankDraw,			// ランク描画
		RankingDraw,		// ランキング描画
		SceneChange,		// シーン遷移
	};

	enum class NextSelect
	{
		GameScene,	// ゲームシーン
		Title,		// タイトル
		Num,
	};

	enum class NextScene
	{
		My,		// 自分のシーン
		GameScene,	// ゲームシーン
		Title,		// タイトルシーン
	};

public:
	StageSceneManager(Game::Stage stageKind);
	virtual ~StageSceneManager();

	void Init();

	void Update(Input& input);
	void Draw();

	void SetGameClear();

	/// <summary>
	/// ステージの取得
	/// </summary>
	/// <returns>ステージの種類</returns>
	Game::Stage GetStageKind() const { return m_stageKind; }
	const std::shared_ptr<ObjectManager> GetObjectManager() { return m_pObjectManager; }
	/// <summary>
	/// ゲームクリア時間の取得
	/// </summary>
	/// <returns></returns>
	int GetGameClearTime() const{return m_clearTime; }

	/// <summary>
	/// ゲームクリアかどうか
	/// </summary>
	/// <returns></returns>
	bool IsGameClear()const { return m_isGameClear; }

	NextScene GetNextScene()const { return m_nextScene; }

private:	// 関数
	void ClearUpdate();
	void ClearSoundUpdate();
	void ClearDraw();

	void ImageLoad();
	void ImageDelete();

	void TextDraw();
	void TimeDraw();
	void RankingDraw();
	void RankDraw();
	void RankTimeDraw(int ranking);
	void MyRankTimeDraw(int ranking);
	void ButtonDraw();

private:	// 変数
	int m_clearTextH;			// クリア時に出す画像のハンドル
	int m_operationH;			// 操作説明画像ハンドル
	int m_clearTime;			// クリア時間
	int m_drawClearTime;		// 表示用のクリア時間
	float m_rankSize;			// ランクのサイズ
	bool m_isGameClear;			// ゲームをクリアしたかどうか

	bool m_isMyRank;			// 自分のタイムが反映されているか

	bool m_isPlayClearSE;		// クリアのSEを流したかどうか

	bool m_isExpasionRnak;	// ランクが拡大中か

	bool m_isButtonDraw;	// ボタンが表示されたかどうか

	int m_minusLeftPosY;		// 引く左側の画像のY座標

	int m_minusRightPosX;		// 引く右側の画像のX座標

	int m_minusButtonPosY;		// 引くボタンの画像のY座標

	int m_alpha;			// 画面の暗さ

	bool m_isButtonFall;		// ボタンが落ちきったかどうか

	bool m_isPlayRankingSE;		// ランキング用SEを流したかどうか

	int m_buttonCount;		// カーソルのカウント

	NextSelect m_nextSelect;	// セレクト
	NextScene m_nextScene;		// 次のシーン

	float m_titleChangeTextSize;
	float m_gameSceneChangeTextSize;


	std::array<int, 11> m_myTimeNumH{};	// 自身のクリアタイムの画像ハンドル
	std::array<int, 11> m_rankingTimeNumH{};	// ランキングのタイムの画像ハンドル
	std::array<int, 4> m_rankH{};			// ランク画像ハンドル
	std::array<int, 3> m_textH{};	// テキスト画像ハンドル
	std::array<int, 3> m_rankingH{};	// ランキング画像ハンドル
	std::array<int, 2> m_textBoxH{};	// テキストボックス画像ハンドル
	std::array<int, 3> m_buttonH{};		// ボタンの画像ハンドル

	Rank m_rank;			// ランク
	ClearSceneType m_clearSceneType;	// クリア後のシーンのタイプ
		
	std::vector<int> m_ranking{};	// ランキング
	Game::Stage m_stageKind;	// ステージの種類

	std::shared_ptr<ObjectManager> m_pObjectManager;	// オブジェクトマネージャークラスのポインタ
	std::shared_ptr<CountTime> m_pCountTime;		// 秒数カウントのポインタ
	std::shared_ptr<Time> m_pImageDrawTime;		// 画像が表示されるまでの時間
	std::shared_ptr<Time> m_pStageClearSETime;	// ステージクリア時のサウンドの開始時間
	std::shared_ptr <Time> m_pRankDrawTime;		// ランクが表示されるまでの時間
	std::shared_ptr<Time> m_pLeftImgDrawTime;	// 左側の画像が表示されるまでの時間
	std::shared_ptr<Time> m_pRightImgDrawTime;	// 右側の画像が表示されるまでの時間
	std::shared_ptr<Time> m_pButtonDrawTime;	// ボタンが表示されるまでの時間
	std::shared_ptr<Ranking> m_pRanking;	// ランキング

};

