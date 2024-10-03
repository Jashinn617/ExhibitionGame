#include "DxLib.h"
#include "SceneRanking.h"
#include "SceneSelect.h"

#include "../Util/Pad.h"
#include "../Util/Ranking.h"
#include "../Util/Game.h"
#include "../Util/SoundManager.h"


#include <assert.h>

namespace
{
	/*画像ファイルパス*/
	const char* const kBackgroundFileName = "Data/Img/Ranking/Background.png";	// 背景
	const char* const kBackButtonFileName = "Data/Img/Ranking/BackButton.png";	// 戻るボタン
	const char* const kArrowFileName = "Data/Img/Ranking/Arrow.png";			// 矢印
	const char* const kTextBoxFileName = "Data/Img/Result/ResultTextBox1.png";	// テキストボックス
	// ランキング画像
	const char* const kRankingFileName[3] =
	{
		"Data/Img/Ranking/Ranking1.png",
		"Data/Img/Ranking/Ranking2.png",
		"Data/Img/Ranking/Ranking3.png"
	};
	// ランキングタイム
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
	// テキスト画像
	const char* const kTextFileName[2] =
	{
		"Data/Img/Ranking/RankingText1.png",
		"Data/Img/Ranking/RankingText2.png",
	};

	/*座標*/
	// ランキングタイム
	constexpr int kRankingMinutesFirstPosX = 775;
	constexpr int kRankingMinutesSecondPosX = 875;
	constexpr int kRankingSecondFirstPosX = 975;
	constexpr int kRankingSecondSecondPosX = 1075;
	constexpr int kRankingTimePosY[3] = { 350,600,850 };
	constexpr int kRankingTimeColonPosX = 925;
	// ランキング座標
	constexpr int kRankingPosX = 584;
	constexpr int kRnakingPosY1 = 340;
	constexpr int kRnakingPosY2 = 590;
	constexpr int kRnakingPosY3 = 840;
	// テキスト座標
	constexpr int kTextPosX = 850;
	constexpr int kTextPosY = 165;
	// ボタン座標
	constexpr int kButtonPosX = 1600;
	constexpr int kButtonPosY = 900;
	// 矢印座標
	constexpr int kArrowPosX = 1500;
	constexpr int kArrowPosY = 500;
	// ボックス座標
	constexpr int kBoxPosX = 835;
	constexpr int kBoxPosY = 505;

	/*サイズ*/
	constexpr float kRankingSize = 0.3f;		// ランキング
	constexpr float krankingTimeSize = 0.4f;	// ランキングタイム
	constexpr float kTextSize = 0.9f;			// テキスト
	constexpr float kButtonSize = 0.9f;			// ボタン
	constexpr float kArrowSize = 0.4f;			// 矢印
	constexpr float kBoxSize = 0.95f;			// ボックス

	constexpr float kArrowAngle = 60.0f * DX_PI_F / 120.0f;	 // 矢印の角度

	constexpr int kColonArrayNum = 10;			// タイムの間に表示するコロンの配列番号
	constexpr int kRankingNum = 3;				// ランキングの数
	constexpr int kBoxAlpha = 200;				// ボックスの透明度
	constexpr float kButtonSinSpeed = 0.07f;	// ボタンの拡縮スピード
	constexpr float kButtonAnimSwing = 0.03f;	// ボタンの拡縮幅
}

SceneRanking::SceneRanking():
	m_backgroundH(-1),
	m_isStage1(true),
	m_isArrow(false),
	m_arrowSize(0),
	m_backButtonSize(0),
	m_expansionButtonSize(0),
	m_buttonSinCount(0),
	m_buttonSinSize(0)
{
	/*画像のロード*/
	LoadImg();

	m_pRanking = std::make_shared <Ranking>();
}

SceneRanking::~SceneRanking()
{
	/*画像のデリート*/
	DeleteImg();
}

void SceneRanking::Init()
{
	// ランキングの取得
	m_stage1Ranking = m_pRanking->GetRanking(Game::Stage::Stage1);
	m_stage2Ranking = m_pRanking->GetRanking(Game::Stage::Stage2);

	// SEを鳴らす
	SoundManager::GetInstance().Play("DoorOpen");
	// BGMを鳴らす
	SoundManager::GetInstance().Play("RankingScene");
}

std::shared_ptr<SceneBase> SceneRanking::Update()
{
	// フェード更新
	UpdateFade();

	// 上下ボタンが押された場合
	if (Pad::isTrigger(PAD_INPUT_UP) || Pad::isTrigger(PAD_INPUT_DOWN))
	{
		// SEを鳴らす
		SoundManager::GetInstance().Play("Select");
		// ボタンを切り替える
		m_isArrow = !m_isArrow;
	}

	// 矢印が選択されている状態でAボタンかBボタンが押されていたら
	if (m_isArrow && (Pad::isTrigger(PAD_INPUT_1) || Pad::isTrigger(PAD_INPUT_2)))
	{
		// SEを鳴らす
		SoundManager::GetInstance().Play("Decide");
		// ランキングを切り替える
		m_isStage1 = !m_isStage1;
	}
	// 矢印が選択されていない状態でAボタンかBボタンが押されていたら
	else if (!m_isArrow && (Pad::isTrigger(PAD_INPUT_1) || Pad::isTrigger(PAD_INPUT_2)))
	{
		// SEを鳴らす
		SoundManager::GetInstance().Play("DoorClose");
		// セレクトシーンに戻る
		return std::make_shared<SceneSelect>();
	}

	return shared_from_this();
}

void SceneRanking::Draw()
{
	// 背景描画
	DrawGraph(0, 0, m_backgroundH, false);

	// ボックス描画
	// 不透明度を少し下げる
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kBoxAlpha);
	DrawRotaGraph(kBoxPosX, kBoxPosY, kBoxSize, 0.0f, m_textBoxH, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ボタン描画
	DrawButton();

	// テキスト描画
	if (m_isStage1)	 // ステージ1
	{
		DrawRotaGraph(kTextPosX, kTextPosY, kTextSize, 0.0f,
			m_textH[0], true);
	}
	else	// ステージ2
	{
		DrawRotaGraph(kTextPosX, kTextPosY, kTextSize, 0.0f,
			m_textH[1], true);
	}

	// ランキング描画
	DrawRanking();


#ifdef _DEBUG
	// シーン名描画
	DrawFormatString(0, 0, 0xffffff, "Ranking");
#endif // _DEBUG   

	// フェード描画
	DrawFade();
}

void SceneRanking::End()
{
	// BGMを止める
	SoundManager::GetInstance().DesignationStopSound("RankingScene");
}

void SceneRanking::DrawRankingTime(int ranking)
{
	int rankingTime = 0;
	/*タイム計算*/
	if (m_isStage1)		// ステージ1
	{
		rankingTime = m_stage1Ranking[ranking] / 60;
	}
	else	// ステージ2
	{
		rankingTime = m_stage2Ranking[ranking] / 60;
	}
	// 分数
	int clearMinutesTime = rankingTime / 60;
	int minutesTimeFirst = clearMinutesTime / 10;	// 一の位
	int minutesTimeSecond = clearMinutesTime % 10;	// 十の位
	// 秒数
	int clearSecondTime = rankingTime % 60;
	int secondTimeFirst = clearSecondTime / 10;		// 一の位
	int secondTimeSecond = clearSecondTime % 10;	// 十の位

	/*タイムの描画*/
	DrawRotaGraph(kRankingMinutesFirstPosX, kRankingTimePosY[ranking],
		krankingTimeSize, 0.0f,
		m_rankingTimeNumH[minutesTimeFirst], true);
	DrawRotaGraph(kRankingMinutesSecondPosX, kRankingTimePosY[ranking],
		krankingTimeSize, 0.0f,
		m_rankingTimeNumH[minutesTimeSecond], true);
	DrawRotaGraph(kRankingTimeColonPosX, kRankingTimePosY[ranking],
		krankingTimeSize, 0.0f,
		m_rankingTimeNumH[kColonArrayNum], true);
	DrawRotaGraph(kRankingSecondFirstPosX, kRankingTimePosY[ranking],
		krankingTimeSize, 0.0f,
		m_rankingTimeNumH[secondTimeFirst], true);
	DrawRotaGraph(kRankingSecondSecondPosX, kRankingTimePosY[ranking],
		krankingTimeSize, 0.0f,
		m_rankingTimeNumH[secondTimeSecond], true);
}

void SceneRanking::DrawRanking()
{
	/*ランキングの描画*/
	DrawRotaGraph(kRankingPosX, kRnakingPosY1,
		kRankingSize, 0.0f,
		m_rankingH[0], true);
	DrawRotaGraph(kRankingPosX, kRnakingPosY2,
		kRankingSize, 0.0f,
		m_rankingH[1], true);
	DrawRotaGraph(kRankingPosX, kRnakingPosY3,
		kRankingSize, 0.0f,
		m_rankingH[2], true);

	// ランキングタイムの描画
	for (int i = 0; i < kRankingNum; i++)
	{
		DrawRankingTime(i);
	}
}

void SceneRanking::DrawButton()
{
	// 拡縮処理
	m_buttonSinCount += kButtonSinSpeed;
	m_expansionButtonSize = sinf(m_buttonSinCount) * kButtonAnimSwing;

	if (m_isArrow)	// 矢印の選択中
	{
		m_arrowSize = m_expansionButtonSize + kArrowSize;
		m_backButtonSize = kButtonSize;
	}
	else
	{
		m_arrowSize = kArrowSize;
		m_backButtonSize = m_expansionButtonSize + kButtonSize;
	}

	// ボタン描画
	DrawRotaGraph(kButtonPosX, kButtonPosY, m_backButtonSize, 0.0f, m_buttonH, true);

	// 矢印描画
	DrawRotaGraph(kArrowPosX, kArrowPosY, m_arrowSize, 0.0f, m_arrowH, true);
}

void SceneRanking::LoadImg()
{
	// 画像ロードが失敗していたら止める
	m_backgroundH = LoadGraph(kBackgroundFileName);
	assert(m_backgroundH != -1);
	m_textBoxH = LoadGraph(kTextBoxFileName);
	assert(m_textBoxH != -1);
	m_buttonH = LoadGraph(kBackButtonFileName);
	assert(m_buttonH != -1);
	m_arrowH = LoadGraph(kArrowFileName);
	assert(m_arrowH != -1);
	for (int i = 0; i < m_rankingTimeNumH.size(); i++)
	{
		m_rankingTimeNumH[i] = LoadGraph(kRankingTimeNumFileName[i]);
		assert(m_rankingTimeNumH[i] != -1);
	}
	for (int i = 0; i < m_rankingH.size(); i++)
	{
		m_rankingH[i] = LoadGraph(kRankingFileName[i]);
		assert(m_rankingH[i] != -1);
	}
	for (int i = 0; i < m_textH.size(); i++)
	{
		m_textH[i] = LoadGraph(kTextFileName[i]);
		assert(m_textH[i] != -1);
	}
}

void SceneRanking::DeleteImg()
{
	DeleteGraph(m_backgroundH);
	DeleteGraph(m_textBoxH);
	DeleteGraph(m_buttonH);
	DeleteGraph(m_arrowH);
	for (int i = 0; i < m_rankingTimeNumH.size(); i++)
	{
		DeleteGraph(m_rankingTimeNumH[i]);
	}
	for (int i = 0; i < m_rankingH.size(); i++)
	{
		DeleteGraph(m_rankingH[i]);
	}
	for (int i = 0; i < m_textH.size(); i++)
	{
		DeleteGraph(m_textH[i]);
	}
}
