#include "DxLib.h"
#include "SceneSelect.h"
#include "SceneRanking.h"
#include "SceneStage.h"

#include "../Util/SoundManager.h"
#include "../Util/Pad.h"
#include "../Util/Game.h"

#include <cassert>

namespace
{
	/*画像のファイルパス*/
	const char* const kBackGroundFileName = "Data/Img/Select/Background.png";	// 背景
	const char* const kBackSideFileName = "Data/Img/Select/BackSide.png";		// 画面横の黒ボックス
	// セレクトボックス
	const char* const kSelectBoxFileName[4] =
	{
		"Data/Img/Select/Stage1Box.png",
		"Data/Img/Select/Stage2Box.png",
		"Data/Img/Select/OnlineRankingBox.png",
		"Data/Img/Select/GameEndBox.png"
	};
	// テキスト
	const char* const kTextFileName[4] =
	{
		"Data/Img/Select/Text1.png",
		"Data/Img/Select/Text2.png",
		"Data/Img/Select/Text3.png",
		"Data/Img/Select/Text4.png",
	};

	constexpr int kIndexBackNum = 2;								// スクロールに必要な背景の数
	constexpr int kScrollSpeed = 1;									// スクロールのスピード
	constexpr int kTextScrollSpeed = 2;								// スクロールのスピード
	constexpr int kBackSideRightPosX = Game::kScreenWidth - 220;	// 画面横黒ボックスの座標X
	constexpr int kBackSideLeftPosX = -80;							// 画面横黒ボックスの座標Y

	/*セレクトボックス座標*/
	constexpr int kSelectBoxPosX1 = 600;
	constexpr int kSelectBoxPosX2 = 1300;
	constexpr int kSelectBoxPosX3 = 700;
	constexpr int kSelectBoxPosX4 = 1100;
	constexpr int kUpSelectBoxPosY = 400;
	constexpr int kDownSelectBoxPosY = 820;

	/*ボックスサイズ*/
	constexpr float kUpBoxSize = 0.7f;
	constexpr float kDownBoxSize = 0.6f;

	constexpr float kBoxSinSpeed = 0.07f;	// ボックスが拡縮するスピード
	constexpr float kBoxAnimSwing = 0.03f;	// ボックスの拡縮幅

	constexpr int kInfoBackAlpha = 200;		// 画面横黒ボックスの不透明度
}

SceneSelect::SceneSelect() :
	m_backgroundH(-1),
	m_scrollX(0),
	m_scrollY(0),
	m_textWidth(0),
	m_textHeight(0),
	m_backWidth(0),
	m_backHeight(0),
	m_isUp(true),
	m_isLeft(true),
	m_upBoxSize1(0.0f),
	m_upBoxSize2(0.0f),
	m_downBoxSize1(0.0f),
	m_downBoxSize2(0.0f),
	m_expansionBoxSize(0.0f),
	m_boxSinCount(0.0f),
	m_boxSinSize(0.0f),
	m_next(NextScene::My)
{
	/*画像ハンドルのロード*/
	LoadImg();

	// 背景画像のサイズの取得
	GetGraphSize(m_backgroundH, &m_backWidth, &m_backHeight);
	// テキスト画像サイズの取得
	GetGraphSize(m_textH[0], &m_textWidth, &m_textHeight);
}

SceneSelect::~SceneSelect()
{
	/*画像ハンドルのデリート*/
	DeleteImg();
}

void SceneSelect::Init()
{
	/*BGMを流す*/
	SoundManager::GetInstance().Play("SelectScene");
}

std::shared_ptr<SceneBase> SceneSelect::Update()
{
	// フェード更新
	UpdateFade();

	// 背景のスクロール
	m_scrollX += kScrollSpeed;
	// 文字のスクロール
	m_scrollY += kTextScrollSpeed;

	// 上下ボタンが押された場合
	if (Pad::isTrigger(PAD_INPUT_UP) || Pad::isTrigger(PAD_INPUT_DOWN))
	{
		SoundManager::GetInstance().Play("Select");
		m_scrollY = 0;
		// 上下の選択を入れ替える
		m_isUp = !m_isUp;
	}
	// 左右時間が押された場合
	if (Pad::isTrigger(PAD_INPUT_LEFT) || Pad::isTrigger(PAD_INPUT_RIGHT))
	{
		SoundManager::GetInstance().Play("Select");
		m_scrollY = 0;
		// 左右の選択を入れ替える
		m_isLeft = !m_isLeft;
	}
	// ボックスの更新
	SelectBoxUpdate();
	// セレクトの更新
	SelectUpdate();

	// 次のシーンへの遷移処理
	switch (m_next)
	{
	case SceneSelect::NextScene::My:
		return shared_from_this();
		break;
	case SceneSelect::NextScene::Stage1:
		// ステージ1に遷移する
		return std::make_shared<SceneStage>(Game::Stage::Stage1);
		break;
	case SceneSelect::NextScene::Stage2:
		// ステージ2に遷移する
		return std::make_shared<SceneStage>(Game::Stage::Stage2);
		break;
	case SceneSelect::NextScene::Ranking:
		// ランキングシーンに遷移する
		return std::make_shared<SceneRanking>();
		break;
	case SceneSelect::NextScene::End:
		// ゲーム終了
		DxLib_End();
		break;
	default:
		break;
	}

	return shared_from_this();
}

void SceneSelect::Draw()
{
	// 背景描画
	BackDraw();
	// 説明テキスト描画
	InfoDraw();
	// セレクトボックス描画
	SelectBoxDraw();

#ifdef _DEBUG
	// シーン名描画
	DrawFormatString(0, 0, 0xffffff, "Select");
#endif // _DEBUG   

	// フェード描画
	DrawFade();
}

void SceneSelect::End()
{
	// BGMを止める
	SoundManager::GetInstance().DesignationStopSound("SelectScene");
}

void SceneSelect::SelectUpdate()
{
	// AボタンかBボタンが押されたら次のシーンに遷移する
	if (Pad::isTrigger(PAD_INPUT_1) || Pad::isTrigger(PAD_INPUT_2))
	{
		// SEを鳴らす
		SoundManager::GetInstance().Play("Decide");

		if (m_isUp)	// 上の選択している
		{
			if (m_isLeft)	// 左を選択している
			{
				// ステージ1
				m_next = NextScene::Stage1;
			}
			else
			{
				// ステージ2
				m_next = NextScene::Stage2;
			}

		}
		else	// 下の選択をしている
		{
			if (m_isLeft)	// 左を選択している
			{
				// ランキング
				m_next = NextScene::Ranking;
			}
			else
			{
				// 終了
				m_next = NextScene::End;
			}
		}
	}
	else
	{
		// 現在シーン
		m_next = NextScene::My;
	}
}

void SceneSelect::SelectBoxUpdate()
{
	/*現在選ばれているボックスを拡縮する*/
	// 拡縮処理
	m_boxSinCount += kBoxSinSpeed;
	m_expansionBoxSize = sinf(m_boxSinCount) * kBoxAnimSwing;

	if (m_isUp)	// 上の選択している
	{
		if (m_isLeft)	// 左を選択している
		{
			m_upBoxSize1 = m_expansionBoxSize + kUpBoxSize;
			m_upBoxSize2 = kUpBoxSize;
			m_downBoxSize1 = kDownBoxSize;
			m_downBoxSize2 = kDownBoxSize;
		}
		else
		{
			m_upBoxSize1 = kUpBoxSize;
			m_upBoxSize2 = m_expansionBoxSize + kUpBoxSize;
			m_downBoxSize1 = kDownBoxSize;
			m_downBoxSize2 = kDownBoxSize;
		}

	}
	else	// 下の選択をしている
	{
		if (m_isLeft)	// 左を選択している
		{
			m_upBoxSize1 = kUpBoxSize;
			m_upBoxSize2 = kUpBoxSize;
			m_downBoxSize1 = m_expansionBoxSize + kDownBoxSize;
			m_downBoxSize2 = kDownBoxSize;
		}
		else
		{
			m_upBoxSize1 = kUpBoxSize;
			m_upBoxSize2 = kUpBoxSize;
			m_downBoxSize1 = kDownBoxSize;
			m_downBoxSize2 = m_expansionBoxSize + kDownBoxSize;
		}
	}

}

void SceneSelect::BackDraw()
{
	// 背景をスクロールさせる
	int scrollBack = m_scrollX % m_backWidth;
	for (int index = 0; index < kIndexBackNum; index++)
	{
		DrawGraph(-scrollBack + index * m_backWidth,
			0, m_backgroundH, true);
	}
}

void SceneSelect::InfoDraw()
{
	// 画面横黒ボックスの描画
	// 不透明度を少し下げる
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kInfoBackAlpha);
	DrawGraph(kBackSideRightPosX, 0, m_backSideH, false);
	DrawGraph(kBackSideLeftPosX, 0, m_backSideH, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 画像番号の設定
	// 選んでいる場所によってテキストを変える
	int imgNum = m_isUp ? m_isLeft ? 0 : 1 : m_isLeft ? 2 : 3;

	// 文字をスクロールをさせる
	int scroll = m_scrollY % m_textHeight;
	for (int index = 0; index < kIndexBackNum; index++)
	{
		DrawGraph(kBackSideRightPosX,
			-scroll + index * m_textHeight,
			m_textH[imgNum], true);
		DrawGraph(0,
			-scroll + index * m_textHeight,
			m_textH[imgNum], true);
	}

}

void SceneSelect::SelectBoxDraw()
{
	DrawRotaGraph(kSelectBoxPosX1, kUpSelectBoxPosY,
		m_upBoxSize1, 0.0f, m_SelectBoxH[0], true);
	DrawRotaGraph(kSelectBoxPosX2, kUpSelectBoxPosY,
		m_upBoxSize2, 0.0f, m_SelectBoxH[1], true);

	DrawRotaGraph(kSelectBoxPosX1, kDownSelectBoxPosY,
		m_downBoxSize1, 0.0f, m_SelectBoxH[2], true);
	DrawRotaGraph(kSelectBoxPosX2, kDownSelectBoxPosY,
		m_downBoxSize2, 0.0f, m_SelectBoxH[3], true);
}

void SceneSelect::LoadImg()
{
	// ロードに失敗した場合は止める
	m_backgroundH = LoadGraph(kBackGroundFileName);
	assert(m_backgroundH != -1);
	m_backSideH = LoadGraph(kBackSideFileName);
	assert(m_backSideH != -1);
	for (int i = 0; i < m_SelectBoxH.size(); i++)
	{
		m_SelectBoxH[i] = LoadGraph(kSelectBoxFileName[i]);
		assert(m_SelectBoxH[i] != -1);
	}
	for (int i = 0; i < m_textH.size(); i++)
	{
		m_textH[i] = LoadGraph(kTextFileName[i]);
	}
}

void SceneSelect::DeleteImg()
{
	DeleteGraph(m_backgroundH);
	DeleteGraph(m_backSideH);
	for (int i = 0; i < m_SelectBoxH.size(); i++)
	{
		DeleteGraph(m_SelectBoxH[i]);
	}
	for (int i = 0; i < m_textH.size(); i++)
	{
		DeleteGraph(m_textH[i]);
	}
}
