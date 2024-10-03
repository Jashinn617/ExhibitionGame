#include "DxLib.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include "StageSceneManager.h"
#include "SceneStage.h"

#include "../Util/Game.h"
#include "../Util/Pad.h"
#include "../Util/SoundManager.h"

#include "../Object/SkyDome.h"
#include "../Object/Camera.h"

#include <assert.h>

namespace
{
	/*画像ファイルパス*/
	const char* const kTitleLogoImg = "Data/Img/Title/Logo.png";				// タイトルロゴ
	const char* const kPressAnyButtonImg = "Data/Img/Title/PressAnyButton.png";	// テキスト
	const char* const kMovieFileName = "Data/Movie/DemoMovie.mp4";				// 動画ファイルパス

	/*タイトルロゴ画像の初期座標*/
	constexpr int kStartTitleLogoPosY = -1200;
	/*タイトルロゴ画像の座標*/
	constexpr int kTitleLogoPosX = static_cast<int>(Game::kScreenWidth * 0.5f);
	constexpr int kTitleLogoPosY = static_cast<int>(Game::kScreenHeight * 0.5f);
	/*プレスボタンの画像の座標*/
	constexpr int kPressAnyButtonPosX = static_cast<int>(Game::kScreenWidth * 0.5f);
	constexpr int kPressAnyButtonPosY = static_cast<int>(Game::kScreenHeight * 0.5f);

	constexpr int kTitleFallSpeed = 30;	// タイトルが落ちてくる速度
}

SceneTitle::SceneTitle():
	m_titleLogoH(LoadGraph(kTitleLogoImg)),
	m_textH(LoadGraph(kPressAnyButtonImg)),
	m_movieH(LoadGraph(kMovieFileName)),
	m_titlePosY(kStartTitleLogoPosY),
	m_isPlayFallSE(false)
{
	// 画像のロードに失敗していたら止まる
	assert(m_titleLogoH != -1);
	assert(m_textH != -1);
	// 動画のロードに失敗していたら止まる
	assert(m_movieH != -1);
}

SceneTitle::~SceneTitle()
{
	/*画像のデリート*/
	DeleteGraph(m_titleLogoH);
	DeleteGraph(m_textH);
	/*動画のデリート*/
	DeleteGraph(m_movieH);
}

void SceneTitle::Init()
{
	/*処理無し*/
}

std::shared_ptr<SceneBase> SceneTitle::Update()
{
	// フェードの更新
	UpdateFade();

	// タイトルを落とす
	m_titlePosY += kTitleFallSpeed;
	// タイトル位置が既定の位置より下に行かないようにする
	m_titlePosY = min(m_titlePosY, kTitleLogoPosY);

	/*タイトル落下中*/
	// タイトルの落下音を流す
	if (m_titlePosY <= kTitleLogoPosY && !m_isPlayFallSE)
	{
		// 落下音が流れてなかった場合
		if (!SoundManager::GetInstance().IsDesignationCheckPlaySound("Fall"))
		{
			// 落下音
			SoundManager::GetInstance().Play("Fall");
		}
	}
	// タイトルが着地したら着地音を流す
	if (m_titlePosY >= kTitleLogoPosY && !m_isPlayFallSE)
	{
		// 動画の再生
		PlayMovieToGraph(m_movieH);
		// 着地音
		SoundManager::GetInstance().Play("TitleLanding");
		m_isPlayFallSE = true;
	}
	// SEが流れ終わってからBGMを流す
	if (m_titlePosY >= kTitleLogoPosY && m_isPlayFallSE)
	{
		// BGMを流す
		SoundManager::GetInstance().Play("TitleScene");

		// 動画が停止していたら再生位置を最初に戻す
		if (GetMovieStateToGraph(m_movieH) == 0)
		{
			SeekMovieToGraph(m_movieH, 0);
		}

		// 動画の再生
		PlayMovieToGraph(m_movieH);
		
	}

	// タイトルが落ちた後に何かしらのボタンが押されていたら次のシーンに遷移する
	if (CheckHitKeyAll() && m_titlePosY >= kTitleLogoPosY)
	{
		// 決定SEを流す
		SoundManager::GetInstance().Play("TitleButtonPush");

		return std::make_shared<SceneSelect>();
	}

	return shared_from_this();
}

void SceneTitle::Draw()
{
	// 動画の描画
	DrawGraph(0, 0, m_movieH, true);

	// タイトル画像の描画
	DrawRotaGraph(kTitleLogoPosX, m_titlePosY,
		1.0f, 0.0f,
		m_titleLogoH, true);

	// タイトルがおりきってなかった場合は表示しない
	if (m_titlePosY < kTitleLogoPosY) return;
	// テキストの描画
	DrawRotaGraph(kPressAnyButtonPosX, kPressAnyButtonPosY,
		1.0f, 0.0f,
		m_textH, true);

#ifdef _DEBUG
	// シーン名の描画
	DrawFormatString(0, 0, 0xffffff, "Title");
#endif // _DEBUG   

	// フェードの描画
	DrawFade();
}

void SceneTitle::End()
{
	// BGMを消す
	SoundManager::GetInstance().DesignationStopSound("TitleScene");
}
