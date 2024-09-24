#include "StageSceneManager.h"

#include "../Object/ObjectManager.h"

#include "../Util/CountTime.h"
#include "../Util/Time.h"
#include "../Util/Ranking.h"
#include "../Util/Input.h"
#include "../Util/Pad.h"
#include "../Util/SoundManager.h"

#include <cassert>

namespace
{
	// ステージごとのクリア時間の基準
	constexpr double kResultDataTable[static_cast<int>(Game::Stage::StageNum)] =
	{
		3600,		// テスト
		7200,		// ステージ1
		14400,		// ステージ2
		0
	};

	// クリア時に出す画像のハンドル
	const char* const kClearTextFileName = "Data/Img/Result/GameClearLogo.png";

	// 操作説明の画像ハンドル
	const char* const kOperationFileName = "Data/Img/Operation.png";

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
	const char* const kRankFileName[static_cast<int>(StageSceneManager::Rank::RankNum)] =
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
	const char* const kButtonFileName[3] =
	{
		"Data/Img/Result/SceneChangeText1.png",
		"Data/Img/Result/SceneChangeText2.png",
		"Data/Img/Result/SceneChangeText3.png",
	};

	/*テキスト座標*/
	constexpr int kTextPosX1 = static_cast<int>(Game::kScreenWidth * 0.28f);
	constexpr int kTextPosX2 = static_cast<int>(Game::kScreenWidth * 0.75f);
	constexpr int kTextPosY1 = static_cast<int>(Game::kScreenHeight * 0.14f);
	constexpr int kTextPosY2 = static_cast<int>(Game::kScreenHeight * 0.43f);
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

	/*ボタン座標*/
	constexpr int kButtonPosX = Game::kScreenWidth / 2;
	constexpr int kButtonPosY1 = 200;
	constexpr int kButtonPosY2 = 500;
	constexpr int kButtonPosY3 = 750;
	constexpr float kButtonSize = 1.0f;
	constexpr float kSelectButtonSize = 1.7f;	// 選ばれているボタンのサイズ
	constexpr int kStartButtonPosY = 2000;		// ボタンの初期位置
	constexpr int kButtonUpSpeed = 40;			// ボタンの上昇速度

	/*操作説明座標*/
	constexpr int kOperationPosX = 1750;
	constexpr int kOperationPosY = 200;
	constexpr float kOperationSize = 0.5f;

	constexpr int kColonArrayNum = 10;			// タイムの間に表示するコロンの配列番号

	constexpr int kRankingNum = 3;			// ランキングの表示数
	constexpr int kClearAnim = 63;			// クリアアニメーション
	constexpr int kStageClearSETime = 20;	// クリアSEが流れるまでの時間
	constexpr int kImageDrawTime = 500;		// 左画像が表示されるまでの時間
	constexpr int kRightImgDrawTime = 100;	// 右画像が表示されるまでの時間
	constexpr int kFallSpeed = 50;			// 画像の落下速度
	constexpr int kStartLeftPosY = 2000;		// 左画像の初期位置Y
	constexpr int kStartRightPosX = -1000;	// 右画像の初期位置X

	constexpr int kTimePlusSpeed = 80;		// 表示用のタイムが増えていくスピード

	constexpr float kRankMaxSize = 1.5f;	// ランク画像の最大サイズ
	constexpr float kRankEndSize = 0.8f;	// 最終ランクサイズ
	constexpr float kRankScaleUpSpeed = 0.07f;	// ランク画像が拡大するスピード
	constexpr int kRankDrawTime = 40;			// ランクが表示されるまでの時間

	constexpr int kSceneChangeAlpha = 200;	// シーン遷移時の画面の暗さ

	constexpr int kClearTextPosX = 1000;
	constexpr int kClearTextPosY = 200;
	constexpr float kClearTextSize = 1.2f;


}

StageSceneManager::StageSceneManager(Game::Stage stageKind):
	m_clearTextH(-1),
	m_clearTime(0),
	m_drawClearTime(0),
	m_rankSize(0.0f),
	m_isGameClear(false),
	m_isMyRank(false),
	m_isPlayClearSE(false),
	m_isExpasionRnak(true),
	m_isButtonDraw(false),
	m_isPlayRankingSE(false),
	m_isButtonFall(false),
	m_stageKind(stageKind),
	m_minusLeftPosY(kStartLeftPosY),
	m_minusRightPosX(kStartRightPosX),
	m_minusButtonPosY(kStartButtonPosY),
	m_clearSceneType(ClearSceneType::LeftImgDraw),
	m_alpha(0),
	m_buttonCount(0),
	m_nextSelect(NextSelect::Title),
	m_nextScene(NextScene::My),
	m_titleChangeTextSize(kButtonSize),
	m_gameSceneChangeTextSize(kButtonSize)
{
	m_pStageClearSETime = std::make_shared<Time>(kStageClearSETime);
	m_pImageDrawTime = std::make_shared<Time>(kImageDrawTime);
	m_pLeftImgDrawTime = std::make_shared<Time>(kImageDrawTime);
	m_pRightImgDrawTime = std::make_shared<Time>(kRightImgDrawTime);
	m_pRankDrawTime = std::make_shared<Time>(kRankDrawTime);

	m_pObjectManager = std::make_shared<ObjectManager>(stageKind);
	m_pCountTime = std::make_shared<CountTime>();
	m_pRanking = std::make_shared<Ranking>();
	ImageLoad();
}

StageSceneManager::~StageSceneManager()
{
	ImageDelete();
}

void StageSceneManager::Init()
{
	// BGMを流す
	SoundManager::GetInstance().Play("MainScene");
}

void StageSceneManager::Update(Input& input)
{
	m_pObjectManager->Update(input);

	if (!m_isGameClear)
	{
		// ゲームクリア時間の更新
		m_clearTime++;

		m_pCountTime->Update(m_clearTime);

		// クリア条件を達成した時
		if (m_pObjectManager->IsGameEnd() && !m_isGameClear)
		{
			m_isGameClear = true;
			// ランキングの取得、更新
			m_pRanking->UpdateRanking(m_stageKind, m_clearTime);
			m_ranking = m_pRanking->GetRanking(m_stageKind);
			// BGMを止める
			SoundManager::GetInstance().DesignationStopSound("MainScene");
		}
	}
	else
	{
		if (m_pObjectManager->IsGameClear())
		{
			ClearUpdate();
		}

	
	}

}

void StageSceneManager::Draw()
{
	m_pObjectManager->Draw();
	if (!m_isGameClear)
	{
		// 現在時間の描画
		m_pCountTime->Draw();

		// 操作説明の描画
		DrawRotaGraph(kOperationPosX, kOperationPosY, kOperationSize,
			0.0f, m_operationH, true);
	}
	else
	{
		if (m_pObjectManager->IsGameClear())
		{
			ClearDraw();
		}
	}
	
}

void StageSceneManager::SetGameClear()
{
	m_pObjectManager->SetGameClear();
	m_isGameClear = true;
}

void StageSceneManager::ClearUpdate()
{
	// ループ防止
	if (!m_isPlayClearSE)
	{
		// SEを流す
		SoundManager::GetInstance().Play("GameClear");
		m_isPlayClearSE = true;
	}
	// クリアSEが流れ終わったらBGMを流す
	if (m_isPlayClearSE &&
		!SoundManager::GetInstance().IsDesignationCheckPlaySound("GameClear"))
	{
		// BGMを流す
		SoundManager::GetInstance().Play("ClearScene");
	}
	
	switch (m_clearSceneType)
	{
	case ClearSceneType::LeftImgDraw:
		// 一定時間たったら左側の画像を落とす
		if (!m_pImageDrawTime->Update()) return;
		m_minusLeftPosY -= kFallSpeed;
		if (m_minusLeftPosY <= 0)
		{
			m_minusLeftPosY = 0;
			m_clearSceneType = ClearSceneType::ClearTimeMeasure;
		}
		break;

	case ClearSceneType::ClearTimeMeasure:
		// 画像が落ちきったらクリアタイムを増やす
		m_drawClearTime += kTimePlusSpeed;

		if (!SoundManager::GetInstance().IsDesignationCheckPlaySound("TimeCount"))
		{
			// カウント用のSEを鳴らす
			SoundManager::GetInstance().Play("TimeCount");
		}
		

		if (m_drawClearTime >= m_clearTime)
		{
			// SEを止める
			SoundManager::GetInstance().DesignationStopSound("TimeCount");

			m_drawClearTime = m_clearTime;
			m_clearSceneType = ClearSceneType::RankDraw;

			
		}
		break;

	case ClearSceneType::RankDraw:
		// クリアタイムが増えきって一定時間たったらランクを表示する
		if (!m_pRankDrawTime->Update()) return;
		// ランクを拡大する
		if (m_isExpasionRnak)
		{
			m_rankSize += kRankScaleUpSpeed;
			// ランクが拡大しきったら
			if (m_rankSize >= kRankMaxSize)
			{
				// ランキング表示用のSEを鳴らす
				SoundManager::GetInstance().Play("RankDraw");

				m_rankSize = kRankMaxSize;
				// 縮小を始める
				m_isExpasionRnak = false;
			}
		}
		else
		{
			m_rankSize -= kRankScaleUpSpeed;
			// ランクが既定の大きさになったらランキングを描画する
			if (m_rankSize <= kRankEndSize)
			{
				m_rankSize = kRankEndSize;
				m_clearSceneType = ClearSceneType::RankingDraw;
			}
		}
		break;

	case ClearSceneType::RankingDraw:
		// ランクを表示してから一定時間たったらランキングを表示する
		if (m_pRightImgDrawTime->Update())
		{
			m_minusRightPosX -= -kFallSpeed;
		}
		if (m_minusRightPosX >= 0)
		{
			// SEを鳴らす
			if (!m_isPlayRankingSE)
			{
				SoundManager::GetInstance().Play("RankingDraw");
				m_isPlayRankingSE = true;
			}
			
			m_minusRightPosX = 0;
			// 何かしらのボタンが押されたら次に行く
			if (CheckHitKeyAll())
			{
				// 画面を暗くする
				m_alpha = kSceneChangeAlpha;

				// 仮
				m_isButtonDraw = true;

				m_clearSceneType = ClearSceneType::SceneChange;
			}
		}
		break;

	case ClearSceneType::SceneChange:
		// 真ん中にボタンを表示する
		// ボタンは下から現れる
		m_minusButtonPosY -= kButtonUpSpeed;
		if (m_minusButtonPosY <= 0)
		{
			m_minusButtonPosY = 0;
			m_isButtonFall = true;
		}

		// ボタンが下から現れてから
		if (m_isButtonFall)
		{			
			// カーソル移動
			if (Pad::isTrigger(PAD_INPUT_UP))
			{
				// SEを鳴らす
				SoundManager::GetInstance().Play("Select");

				m_buttonCount--;
				if (m_buttonCount < 0)
				{
					m_buttonCount = static_cast<int>(NextSelect::Num) - 1;
				}
			}
			if (Pad::isTrigger(PAD_INPUT_DOWN))
			{
				// SEを鳴らす
				SoundManager::GetInstance().Play("Select");

				m_buttonCount++;
				if (m_buttonCount >= static_cast<int>(NextSelect::Num))
				{
					m_buttonCount = 0;
				}
			}

			// AボタンかBボタンが押されたら
			// 押されたボタンによってシーンを遷移する
			if (Pad::isTrigger(PAD_INPUT_1) || Pad::isTrigger(PAD_INPUT_2))
			{
				// BGMを消す
				SoundManager::GetInstance().DesignationStopSound("ClearScene");

				
				if (m_buttonCount == static_cast<int>(NextSelect::GameScene))
				{
					// SEを鳴らす
					SoundManager::GetInstance().Play("SceneChange");

					m_nextScene = NextScene::GameScene;
				}
				else if (m_buttonCount == static_cast<int>(NextSelect::Title))
				{
					// SEを鳴らす
					SoundManager::GetInstance().Play("SceneChange");
					m_nextScene = NextScene::Title;
				}
				else
				{
					m_nextScene = NextScene::My;
				}
			}
		}
		break;

	default:
		break;
	}
}

void StageSceneManager::ClearSoundUpdate()
{
	

}

void StageSceneManager::ClearDraw()
{
	// ゲームクリアの画像の描画
	// ランキングが出たら消す
	if (!m_pImageDrawTime->Update())
	{
		DrawRotaGraph(kClearTextPosX, kClearTextPosY, kClearTextSize,
			0.0f, m_clearTextH, true);
	}

	// テキスト描画
	TextDraw();
	// タイム描画
	TimeDraw();
	// ランキング描画
	RankingDraw();
	// ランク描画
	RankDraw();
	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ボタン描画
	ButtonDraw();

#ifdef _DEBUG
	DrawString(0, 0, "GameClear", 0xffffff);
	DrawFormatString(0, 80, 0xffffff, "1位：%d 2位：%d 3位:%d",
		m_ranking[0] / 60, m_ranking[1] / 60, m_ranking[2] / 60);

	DrawFormatString(0, 140, 0xffffff, "カウント：%d", m_buttonCount);
#endif // _DEBUG
}

void StageSceneManager::ImageLoad()
{
	/*画像ハンドルのロード*/
	m_clearTextH = LoadGraph(kClearTextFileName);
	assert(m_clearTextH != -1);
	m_operationH = LoadGraph(kOperationFileName);
	assert(m_operationH != -1);
	for (int i = 0; i < m_myTimeNumH.size(); i++)
	{
		m_myTimeNumH[i] = LoadGraph(kMyTimeNumFileName[i]);
		assert(m_myTimeNumH[i] != -1);
	}
	for (int i = 0; i < m_rankingTimeNumH.size(); i++)
	{
		m_rankingTimeNumH[i] = LoadGraph(kRankingTimeNumFileName[i]);
		assert(m_rankingTimeNumH[i] != -1);
	}
	for (int i = 0; i < m_rankH.size(); i++)
	{
		m_rankH[i] = LoadGraph(kRankFileName[i]);
		assert(m_rankH[i] != -1);
	}
	for (int i = 0; i < m_textH.size(); i++)
	{
		m_textH[i] = LoadGraph(kTextFileName[i]);
		assert(m_textH[i] != -1);
	}
	for (int i = 0; i < m_rankingH.size(); i++)
	{
		m_rankingH[i] = LoadGraph(kRankingFileName[i]);
		assert(m_rankingH[i] != -1);
	}
	for (int i = 0; i < m_textBoxH.size(); i++)
	{
		m_textBoxH[i] = LoadGraph(kTextBoxFileName[i]);
		assert(m_textBoxH[i] != -1);
	}
	for (int i = 0; i < m_buttonH.size(); i++)
	{
		m_buttonH[i] = LoadGraph(kButtonFileName[i]);
		assert(m_buttonH[i] != -1);
	}
}

void StageSceneManager::ImageDelete()
{
	/*画像のデリート*/
	DeleteGraph(m_clearTextH);
	DeleteGraph(m_operationH);
	for (int i = 0; i < m_myTimeNumH.size(); i++)
	{
		DeleteGraph(m_myTimeNumH[i]);
	}
	for (int i = 0; i < m_rankingTimeNumH.size(); i++)
	{
		DeleteGraph(m_rankingTimeNumH[i]);
	}
	for (int i = 0; i < m_rankH.size(); i++)
	{
		DeleteGraph(m_rankH[i]);
	}
	for (int i = 0; i < m_textH.size(); i++)
	{
		DeleteGraph(m_textH[i]);
	}
	for (int i = 0; i < m_rankingH.size(); i++)
	{
		DeleteGraph(m_rankingH[i]);
	}
	for (int i = 0; i < m_textBoxH.size(); i++)
	{
		DeleteGraph(m_textBoxH[i]);
	}
	for (int i = 0; i < m_buttonH.size(); i++)
	{
		DeleteGraph(m_buttonH[i]);
	}
}

void StageSceneManager::TextDraw()
{
	/*テキストボックスの描画*/
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kTextBoxAlpha);
	DrawRotaGraph(kTextBoxPosX1, kTextBoxPosY - m_minusLeftPosY,
		1.0f, 0.0f,
		m_textBoxH[0], true);
	DrawRotaGraph(kTextBoxPosX2 - m_minusRightPosX, kTextBoxPosY,
		1.0f, 0.0f,
		m_textBoxH[1], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	/*テキストの描画*/
	DrawRotaGraph(kTextPosX1, kTextPosY1 - m_minusLeftPosY,
		1.0f, 0.0f,
		m_textH[0], true);
	DrawRotaGraph(kTextPosX1, kTextPosY2 - m_minusLeftPosY,
		1.0f, 0.0f,
		m_textH[1], true);
	DrawRotaGraph(kTextPosX2 - m_minusRightPosX, kTextPosY1,
		1.0f, 0.0f,
		m_textH[2], true);
}

void StageSceneManager::TimeDraw()
{
	/*タイム計算*/
	int clearTime = m_drawClearTime / 60;
	// 分数
	int clearMinutesTime = clearTime / 60;
	int minutesTimeFirst = clearMinutesTime / 10;	// 一の位
	int minutesTimeSecond = clearMinutesTime % 10;	// 十の位
	// 秒数
	int clearSecondTime = clearTime % 60;
	int secondTimeFirst = clearSecondTime / 10;		// 一の位
	int secondTimeSecond = clearSecondTime % 10;	// 十の位

	/*クリアタイムの描画*/
	DrawRotaGraph(kClearMinutesFirstPosX, kClearTimePosY - m_minusLeftPosY,
		kClearTimeSize, 0.0f,
		m_myTimeNumH[minutesTimeFirst], true);
	DrawRotaGraph(kClearMinutesSecondPosX, kClearTimePosY - m_minusLeftPosY,
		kClearTimeSize, 0.0f,
		m_myTimeNumH[minutesTimeSecond], true);
	DrawRotaGraph(kClearTimeColonPosX, kClearTimePosY - m_minusLeftPosY,
		kClearTimeSize, 0.0f,
		m_myTimeNumH[kColonArrayNum], true);
	DrawRotaGraph(kClearSecondFirstPosX, kClearTimePosY - m_minusLeftPosY,
		kClearTimeSize, 0.0f,
		m_myTimeNumH[secondTimeFirst], true);
	DrawRotaGraph(kClearSecondSecondPosX, kClearTimePosY - m_minusLeftPosY,
		kClearTimeSize, 0.0f,
		m_myTimeNumH[secondTimeSecond], true);
}

void StageSceneManager::RankingDraw()
{
	/*ランキングの描画*/
	DrawRotaGraph(kRankingPosX - m_minusRightPosX, kRnakingPosY1,
		kRankingSize, 0.0f,
		m_rankingH[0], true);
	DrawRotaGraph(kRankingPosX - m_minusRightPosX, kRnakingPosY2,
		kRankingSize, 0.0f,
		m_rankingH[1], true);
	DrawRotaGraph(kRankingPosX - m_minusRightPosX, kRnakingPosY3,
		kRankingSize, 0.0f,
		m_rankingH[2], true);

	m_isMyRank = false;
	/*ランキングタイムの描画*/
	for (int i = 0; i < kRankingNum; i++)
	{
		// ランキングのタイムが自分と同じタイムだった場合は
		// 文字色を変える
		// 同じタイムが二つ以上だった場合は上の順位の色だけ変える
		if (m_ranking[i] == m_clearTime && !m_isMyRank)
		{
			MyRankTimeDraw(i);
			m_isMyRank = true;
		}
		else
		{
			RankTimeDraw(i);
		}
	}
}

void StageSceneManager::RankDraw()
{
	/*ランク計算*/
	// それぞれのタイムの基準
	float BRankTime = static_cast<float>(kResultDataTable[static_cast<int>(m_stageKind)]);
	float ARankTime = BRankTime * 0.8f;
	float SRankTime = ARankTime * 0.8f;

	if (BRankTime <= m_clearTime)
	{
		m_rank = Rank::C;
	}
	else if (BRankTime > m_clearTime && ARankTime <= m_clearTime)
	{
		m_rank = Rank::B;
	}
	else if (ARankTime > m_clearTime && SRankTime <= m_clearTime)
	{
		m_rank = Rank::A;
	}
	else if (SRankTime > m_clearTime)
	{
		m_rank = Rank::S;
	}
	else
	{
		m_rank = Rank::S;
	}
	// 描画
	DrawRotaGraph(kRankPosX, kRankPosY - m_minusLeftPosY,
		m_rankSize, 0.0f,
		m_rankH[static_cast<int>(m_rank)], true);
}

void StageSceneManager::RankTimeDraw(int ranking)
{
	/*タイム計算*/
	int rankingTime = m_ranking[ranking] / 60;
	// 分数
	int clearMinutesTime = rankingTime / 60;
	int minutesTimeFirst = clearMinutesTime / 10;	// 一の位
	int minutesTimeSecond = clearMinutesTime % 10;	// 十の位
	// 秒数
	int clearSecondTime = rankingTime % 60;
	int secondTimeFirst = clearSecondTime / 10;		// 一の位
	int secondTimeSecond = clearSecondTime % 10;	// 十の位

	/*タイムの描画*/
	DrawRotaGraph(kRankingMinutesFirstPosX - m_minusRightPosX, kRankingTimePosY[ranking],
		krankingTimeSize, 0.0f,
		m_rankingTimeNumH[minutesTimeFirst], true);
	DrawRotaGraph(kRankingMinutesSecondPosX - m_minusRightPosX, kRankingTimePosY[ranking],
		krankingTimeSize, 0.0f,
		m_rankingTimeNumH[minutesTimeSecond], true);
	DrawRotaGraph(kRankingTimeColonPosX - m_minusRightPosX, kRankingTimePosY[ranking],
		krankingTimeSize, 0.0f,
		m_rankingTimeNumH[kColonArrayNum], true);
	DrawRotaGraph(kRankingSecondFirstPosX - m_minusRightPosX, kRankingTimePosY[ranking],
		krankingTimeSize, 0.0f,
		m_rankingTimeNumH[secondTimeFirst], true);
	DrawRotaGraph(kRankingSecondSecondPosX - m_minusRightPosX, kRankingTimePosY[ranking],
		krankingTimeSize, 0.0f,
		m_rankingTimeNumH[secondTimeSecond], true);
}

void StageSceneManager::MyRankTimeDraw(int ranking)
{
	/*タイム計算*/
	int rankingTime = m_ranking[ranking] / 60;
	// 分数
	int clearMinutesTime = rankingTime / 60;
	int minutesTimeFirst = clearMinutesTime / 10;	// 一の位
	int minutesTimeSecond = clearMinutesTime % 10;	// 十の位
	// 秒数
	int clearSecondTime = rankingTime % 60;
	int secondTimeFirst = clearSecondTime / 10;		// 一の位
	int secondTimeSecond = clearSecondTime % 10;	// 十の位

	/*タイムの描画*/
	DrawRotaGraph(kRankingMinutesFirstPosX - m_minusRightPosX, kRankingTimePosY[ranking],
		krankingTimeSize, 0.0f,
		m_myTimeNumH[minutesTimeFirst], true);
	DrawRotaGraph(kRankingMinutesSecondPosX - m_minusRightPosX, kRankingTimePosY[ranking],
		krankingTimeSize, 0.0f,
		m_myTimeNumH[minutesTimeSecond], true);
	DrawRotaGraph(kRankingTimeColonPosX - m_minusRightPosX, kRankingTimePosY[ranking],
		krankingTimeSize, 0.0f,
		m_myTimeNumH[kColonArrayNum], true);
	DrawRotaGraph(kRankingSecondFirstPosX - m_minusRightPosX, kRankingTimePosY[ranking],
		krankingTimeSize, 0.0f,
		m_myTimeNumH[secondTimeFirst], true);
	DrawRotaGraph(kRankingSecondSecondPosX - m_minusRightPosX, kRankingTimePosY[ranking],
		krankingTimeSize, 0.0f,
		m_myTimeNumH[secondTimeSecond], true);
}

void StageSceneManager::ButtonDraw()
{
	if (!m_isButtonDraw) return;

	if (m_buttonCount == static_cast<int>(NextSelect::GameScene))
	{
		m_titleChangeTextSize = kSelectButtonSize;
		m_gameSceneChangeTextSize = kButtonSize;
	}
	else if (m_buttonCount == static_cast<int>(NextSelect::Title))
	{
		m_titleChangeTextSize = kButtonSize;
		m_gameSceneChangeTextSize = kSelectButtonSize;
	}


	DrawRotaGraph(kButtonPosX, kButtonPosY1 - m_minusButtonPosY, 
		kButtonSize, 0.0, m_buttonH[0], true);

	DrawRotaGraph(kButtonPosX, kButtonPosY2 - m_minusButtonPosY,
		m_titleChangeTextSize, 0.0, m_buttonH[1], true);
	DrawRotaGraph(kButtonPosX, kButtonPosY3 - m_minusButtonPosY,
		m_gameSceneChangeTextSize, 0.0, m_buttonH[2], true);
}
