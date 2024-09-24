#include "DxLib.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include "StageSceneManager.h"
#include "SceneStage.h"

#include "../Util/Game.h"
#include "../Util/Input.h"
#include "../Util/Pad.h"
#include "../Util/SoundManager.h"

#include "../Object/SkyDome.h"
#include "../Object/Camera.h"

#include <assert.h>

namespace
{
	const char* const kTitleLogoImg = "Data/Img/Title/Logo.png";
	const char* const kPressAnyButtonImg = "Data/Img/Title/PressAnyButton.png";
	const char* const kMovieFileName = "Data/Movie/DemoMovie.mp4";

	/*タイトルロゴ画像の初期座標*/
	constexpr int kStartTitleLogoPosY = -1200;
	/*タイトルロゴ画像の座標*/
	constexpr int kTitleLogoPosX = static_cast<int>(Game::kScreenWidth * 0.5f);	// X
	constexpr int kTitleLogoPosY = static_cast<int>(Game::kScreenHeight * 0.5f);	// Y
	/*プレスボタンの画像の座標*/
	constexpr int kPressAnyButtonPosX = static_cast<int>(Game::kScreenWidth * 0.5f);	// X
	constexpr int kPressAnyButtonPosY = static_cast<int>(Game::kScreenHeight * 0.5f);	// Y

	constexpr int kTitleFallSpeed = 30;	// タイトルが落ちてくる速度
}

SceneTitle::SceneTitle():
	m_H1(-1),
	m_H2(-1),
	m_movieH(-1),
	m_TitlePosY(kStartTitleLogoPosY),
	m_IsPlayFallSE(false)
{
	/*画像のロード*/
	m_H1 = LoadGraph(kTitleLogoImg);
	m_H2 = LoadGraph(kPressAnyButtonImg);
	/*動画のロード*/
	m_movieH = LoadGraph(kMovieFileName);
	assert(m_movieH != -1);
}

SceneTitle::~SceneTitle()
{
	/*画像のデリート*/
	DeleteGraph(m_H1);
	DeleteGraph(m_H2);
	/*動画のデリート*/
	DeleteGraph(m_movieH);
}

void SceneTitle::Init()
{
	/*処理無し*/

}

std::shared_ptr<SceneBase> SceneTitle::Update(Input& input)
{
	UpdateFade();

	m_TitlePosY += kTitleFallSpeed;
	m_TitlePosY = min(m_TitlePosY, kTitleLogoPosY);

	/*タイトル落下中*/
	// タイトルの落下音を流す
	if (m_TitlePosY <= kTitleLogoPosY && !m_IsPlayFallSE)
	{
		// 落下音が流れてなかった場合
		if (!SoundManager::GetInstance().IsDesignationCheckPlaySound("Fall"))
		{
			SoundManager::GetInstance().Play("Fall");
		}
	}

	// タイトルが着地したら着地音を流す
	if (m_TitlePosY >= kTitleLogoPosY && !m_IsPlayFallSE)
	{
		// 動画の再生
		PlayMovieToGraph(m_movieH);
		SoundManager::GetInstance().Play("TitleLanding");
		m_IsPlayFallSE = true;
	}
	// SEが流れ終わってからBGMを流す
	if (m_TitlePosY >= kTitleLogoPosY && m_IsPlayFallSE)
	{
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
	if (CheckHitKeyAll() && m_TitlePosY >= kTitleLogoPosY)
	{
		// BGMを消す
		SoundManager::GetInstance().DesignationStopSound("TitleScene");

		// 決定SEを流す
		SoundManager::GetInstance().Play("TitleButtonPush");

		return make_shared<SceneSelect>();
	}

	

	return shared_from_this();
}

void SceneTitle::Draw()
{
#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, "Title");
#endif // _DEBUG   

	// 動画の描画
	DrawGraph(0, 0, m_movieH, true);


	// タイトル画像の描画
	DrawRotaGraph(kTitleLogoPosX, m_TitlePosY,
		1.0f, 0.0f,
		m_H1, true);

	// タイトルがおりきってなかった場合は表示しない
	if (m_TitlePosY < kTitleLogoPosY) return;

	DrawRotaGraph(kPressAnyButtonPosX, kPressAnyButtonPosY,
		1.0f, 0.0f,
		m_H2, true);

	DrawFade();
}

void SceneTitle::End()
{
	/*処理無し*/
}
