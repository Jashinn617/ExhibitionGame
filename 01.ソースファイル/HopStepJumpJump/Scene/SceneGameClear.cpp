#include "DxLib.h"

#include "SceneGameClear.h"
#include "StageSceneManager.h"

#include "../Util/Time.h"
#include "../Util/Ranking.h"
#include "../Util/SoundManager.h"



#include <cassert>

namespace
{
	// ステージごとのクリア時間の基準
	constexpr double kResultDataTable[static_cast<int>(Game::Stage::StageNum)] =
	{
		3600,		// テスト
		7200,		// ステージ1
		0,
		0
	};

	// 自身のクリアタイムの画像ファイル
	const char* const kMyTimeNumFileName[11] =
	{
		 "Data/Img/Result/ClearTimeNum/ClearTimeNum0.png",
		 "Data/Img/Result/ClearTimeNum/ClearTimeNum1.png",
		 "Data/Img/Result/ClearTimeNum/ClearTimeNum2.png",
		 "Data/Img/Result/ClearTimeNum/ClearTimeNum3.png",
		 "Data/Img/Result/ClearTimeNum/ClearTimeNum4.png",
		 "Data/Img/Result/ClearTimeNum/ClearTimeNum5.png",
		 "Data/Img/Result/ClearTimeNum/ClearTimeNum6.png",
		 "Data/Img/Result/ClearTimeNum/ClearTimeNum7.png",
		 "Data/Img/Result/ClearTimeNum/ClearTimeNum8.png",
		 "Data/Img/Result/ClearTimeNum/ClearTimeNum9.png",
		 "Data/Img/Result/ClearTimeNum/ClearTime.png"
	};

	// ランキングタイムの画像ファイル
	const char* const kRankingTimeNumFileName[11] =
	{
		"Data/Img/Ranking/RankingNum/RankingNum0.png",
		"Data/Img/Ranking/RankingNum/RankingNum1.png",
		"Data/Img/Ranking/RankingNum/RankingNum2.png",
		"Data/Img/Ranking/RankingNum/RankingNum3.png",
		"Data/Img/Ranking/RankingNum/RankingNum4.png",
		"Data/Img/Ranking/RankingNum/RankingNum5.png",
		"Data/Img/Ranking/RankingNum/RankingNum6.png",
		"Data/Img/Ranking/RankingNum/RankingNum7.png",
		"Data/Img/Ranking/RankingNum/RankingNum8.png",
		"Data/Img/Ranking/RankingNum/RankingNum9.png",
		"Data/Img/Ranking/RankingNum/RankingTime.png"
	};

	// ランクの画像ハンドル
	const char* const kRankFileName[static_cast<int>(SceneGameClear::Rank::RankNum)] =
	{
		"Data/Img/Result/Rank/S.png",
		"Data/Img/Result/Rank/A.png",
		"Data/Img/Result/Rank/B.png",
		"Data/Img/Result/Rank/C.png"
	};

	// テキスト画像ファイル
	const char* const kTextFileName[3] =
	{
		"Data/Img/Result/ClearTimeText.png",
		"Data/Img/Result/RankText.png",
		"Data/Img/Result/RankingText.png"
	};

	// ランキング画像ファイル
	const char* const kRankingFileName[3] =
	{
		"Data/Img/Ranking/Ranking1.png",
		"Data/Img/Ranking/Ranking2.png",
		"Data/Img/Ranking/Ranking3.png"
	};

	// テキストボックス画像ファイル
	const char* const kTextBoxFileName[2] =
	{
		"Data/Img/Result/ResultTextBox1.png",
		"Data/Img/Result/ResultTextBox2.png"
	};

	// ボタン画像ファイル
	const char* const kButtonFileName[2] =
	{
		"Data/Img/Result/StageBackButton.png",
		"Data/Img/Result/TitleBackButton.png"
	};

	/*テキスト座標*/
	constexpr int kTextPosX1 = static_cast<int>(Game::kScreenWidth * 0.28f);
	constexpr int kTextPosX2 = static_cast<int>(Game::kScreenWidth * 0.75f);
	constexpr int kTextPosY1 = static_cast<int>(Game::kScreenHeight * 0.13f);
	constexpr int kTextPosY2 = static_cast<int>(Game::kScreenHeight * 0.42f);
	/*ランキング座標*/
	constexpr int kRankingPosX = 1209;
	constexpr int kRnakingPosY1 = 350;
	constexpr int kRnakingPosY2 = 600;
	constexpr int kRnakingPosY3 = 850;
	constexpr float kRankingSize = 0.3f;	// ランキングサイズ
	/*テキストボックス座標*/
	constexpr int kTextBoxPosX1 = static_cast<int>(Game::kScreenWidth * 0.28f);
	constexpr int kTextBoxPosX2 = static_cast<int>(Game::kScreenWidth * 0.75f);
	constexpr int kTextBoxPosY = static_cast<int>(Game::kScreenHeight * 0.48f);
	constexpr int kTextBoxAlpha = static_cast<int>(255 * 0.8f);	// テキストボックスの不透明度
	/*ランク座標*/
	constexpr int kRankPosX = 500;
	constexpr int kRankPosY = 750;
	constexpr float kRankSize = 0.8f;	// ランクサイズ
	/*クリアタイム座標*/
	constexpr int kClearMinutesFirstPosX = 200;
	constexpr int kClearMinutesSecondPosX = 350;
	constexpr int kClearSecondFirstPosX = 650;
	constexpr int kClearSecondSecondPosX = 800;
	constexpr int kClearTimeColonPosX = 500;
	constexpr int kClearTimePosY = 300;
	constexpr float kClearTimeSize = 0.5f;

	/*ランキングタイム座標*/
	constexpr int kRankingMinutesFirstPosX = 1400;
	constexpr int kRankingMinutesSecondPosX = 1500;
	constexpr int kRankingSecondFirstPosX = 1600;
	constexpr int kRankingSecondSecondPosX = 1700;
	constexpr int kRankingTimePosY[3] = { 350,600,850 };
	constexpr int kRankingTimeColonPosX = 1550;
	constexpr float krankingTimeSize = 0.4f;

	constexpr int kColonArrayNum = 10;			// タイムの間に表示するコロンの配列番号

	constexpr int kRankingNum = 3;			// ランキングの表示数
	constexpr int kClearAnim = 63;			// クリアアニメーション
	constexpr int kStageClearSETime = 20;	// クリアSEが流れるまでの時間
	constexpr int kImageDrawTime = 120;		// 画像が表示されるまでの時間
	constexpr int kFallSpeed = 20;			// 画像の落下速度
}

SceneGameClear::SceneGameClear(std::shared_ptr<StageSceneManager> pStageSceneManager, Game::Stage stageKind) :
	m_clearTime(0),
	m_isSceneEnd(false),
	m_isNextScene(false),
	m_isMyRank(false),
	m_isFall(true),
	m_stageKind(stageKind),
	m_pStageSceneManager(pStageSceneManager)
{
	m_pImageDrawTime = std::make_shared<Time>(kImageDrawTime);
	m_pStageClearSETime = std::make_shared<Time>(kStageClearSETime);

	// クリアタイム
	m_clearTime = m_pStageSceneManager->GetGameClearTime();

	
}

SceneGameClear::~SceneGameClear()
{
	
}

void SceneGameClear::Init()
{
	// ランキングの更新、取得
	m_pRanking->UpdateRanking(m_stageKind, m_clearTime);
	m_ranking = m_pRanking->GetRanking(m_stageKind);

	// 前のシーンのサウンドを止める
	SoundManager::GetInstance().StopSound();
}

std::shared_ptr<SceneBase> SceneGameClear::Update(Input& input)
{
	// クリアSEを流す
	if (m_pStageClearSETime->Update())
	{
		
	}


	

	// シーン終了
	

	// ステージシーンマネージャー更新
	m_pStageSceneManager->Update(input);




	// BGMを流す
	SoundManager::GetInstance().Play("ClearScene");





	return shared_from_this();
}

void SceneGameClear::Draw()
{
	m_pStageSceneManager->Draw();
}

void SceneGameClear::End()
{
	/*処理無し*/
}

void SceneGameClear::TextDraw()
{
	
}

void SceneGameClear::TimeDraw()
{
	
}

void SceneGameClear::RankingDraw()
{
	
}

void SceneGameClear::RankDraw()
{
	
}

void SceneGameClear::RankTimeDraw(int ranking)
{
	
}

void SceneGameClear::MyRankTimeDraw(int ranking)
{
	
}

void SceneGameClear::ButtonDraw()
{
	
}

void SceneGameClear::ChangeMainScene()
{
}

void SceneGameClear::ChangeTitleScene()
{
}
