#pragma once
#include "../Util/Game.h"

#include <memory>
#include <vector>
#include <array>
#include <functional>

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
		S,			// Sランク
		A,			// Aランク
		B,			// Bランク
		C,			// Cランク
		RankNum,	// ランクの数
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
	// 次の選択
	enum class NextSelect
	{
		GameScene,	// ゲームシーン
		Select,		// タイトル
		Num,		// セレクト数
	};
	// 次のシーン
	enum class NextScene
	{
		My,			// 自分のシーン
		GameScene,	// ゲームシーン
		Select,		// セレクトシーン
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stageKind">ステージの種類</param>
	StageSceneManager(Game::Stage stageKind);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~StageSceneManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ステージの種類の取得
	/// </summary>
	/// <returns>ステージの種類</returns>
	Game::Stage GetStageKind() const { return m_stageKind; }
	
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
	/// <summary>
	/// クリア時の更新
	/// </summary>
	void ClearUpdate();

	/// <summary>
	/// クリア時の描画
	/// </summary>
	void ClearDraw();

	/// <summary>
	/// テキストの描画
	/// </summary>
	void TextDraw();
	/// <summary>
	/// タイムの描画
	/// </summary>
	void TimeDraw();
	/// <summary>
	/// ランキングの描画
	/// </summary>
	void RankingDraw();
	/// <summary>
	/// ランクの描画
	/// </summary>
	void RankDraw();
	/// <summary>
	/// ランキングタイムの描画
	/// </summary>
	/// <param name="ranking">ランキング</param>
	void RankingTimeDraw(int ranking);
	/// <summary>
	/// ランキング内にある自身のタイムの描画
	/// </summary>
	/// <param name="ranking">ランキング</param>
	void MyRankTimeDraw(int ranking);
	/// <summary>
	/// ボタンの描画
	/// </summary>
	void ButtonDraw();

	/// <summary>
	/// 画像のロード
	/// </summary>
	void LoadImg();
	/// <summary>
	/// 画像のデリート
	/// </summary>
	void DeleteImg();

private:	// 変数
	int m_clearTime;					// クリア時間
	int m_drawClearTime;				// 表示用のクリア時間
	int m_minusLeftPosY;				// 引く左側の画像のY座標
	int m_minusRightPosX;				// 引く右側の画像のX座標
	int m_minusButtonPosY;				// 引くボタンの画像のY座標
	int m_alpha;						// 画面の暗さ
	int m_buttonCount;					// カーソルのカウント
	float m_rankSize;					// ランクのサイズ
	float m_selectChangeTextSize;		// セレクトシーンに戻るボタン用テキストのサイズ
	float m_gameSceneChangeTextSize;	// ゲームシーンに戻るボタン用テキストのサイズ
	bool m_isGameClear;					// ゲームをクリアしたかどうか
	bool m_isMyRank;					// 自分のタイムがランキングに乗っているか
	bool m_isPlayClearSE;				// クリアのSEを流したかどうか
	bool m_isExpasionRnak;				// ランクが拡大中か
	bool m_isButtonFall;				// ボタンが落ちきったかどうか
	bool m_isPlayRankingSE;				// ランキング用SEを流したかどうか

	NextSelect m_nextSelect;			// セレクト
	NextScene m_nextScene;				// 次のシーン
	Rank m_rank;						// ランク
	ClearSceneType m_clearSceneType;	// クリア後のシーンのタイプ
	Game::Stage m_stageKind;			// ステージの種類

	std::array<int, 11> m_myTimeNumH{};			// 自身のクリアタイムの画像ハンドル
	std::array<int, 11> m_rankingTimeNumH{};	// ランキングのタイムの画像ハンドル
	std::array<int, 4> m_rankH{};				// ランク画像ハンドル
	std::array<int, 3> m_textH{};				// テキスト画像ハンドル
	std::array<int, 3> m_rankingH{};			// ランキング画像ハンドル
	std::array<int, 2> m_textBoxH{};			// テキストボックス画像ハンドル
	std::array<int, 3> m_buttonH{};				// ボタンの画像ハンドル
	std::vector<int> m_ranking{};				// ランキング

	int m_clearTextH;									// クリア時に出す画像のハンドル
	int m_operationH;									// 操作説明画像ハンドル
	std::shared_ptr<ObjectManager> m_pObjectManager;	// オブジェクトマネージャークラスのポインタ
	std::shared_ptr<CountTime> m_pCountTime;			// 秒数カウントのポインタ
	std::shared_ptr<Time> m_pImageDrawTime;				// 画像が表示されるまでの時間
	std::shared_ptr<Time> m_pStageClearSETime;			// ステージクリア時のサウンドの開始時間
	std::shared_ptr <Time> m_pRankDrawTime;				// ランクが表示されるまでの時間
	std::shared_ptr<Time> m_pLeftImgDrawTime;			// 左側の画像が表示されるまでの時間
	std::shared_ptr<Time> m_pRightImgDrawTime;			// 右側の画像が表示されるまでの時間
	std::shared_ptr<Time> m_pButtonDrawTime;			// ボタンが表示されるまでの時間
	std::shared_ptr<Ranking> m_pRanking;				// ランキング
};