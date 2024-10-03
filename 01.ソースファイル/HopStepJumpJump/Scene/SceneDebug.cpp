#include "DxLib.h"

#include "SceneDebug.h"
#include "SceneStage.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneRanking.h"

#include "../Object/ObjectManager.h"

#include "../Util/Vec2.h"
#include "../Util/Pad.h"

namespace
{
	constexpr float kCursorPosX = 500.0f;		// 初期カーソル位置X
	constexpr float kCursorPosY = 300.0f;		// 初期カーソル位置Y

	constexpr float kCursorMove = 50.0f;		// カーソルの移動量
}

SceneDebug::SceneDebug():
	m_count(0)
{
	// 移動用文字の設定
	m_sceneString[static_cast<int>(SceneType::Debug)] = "Debug";
	m_sceneString[static_cast<int>(SceneType::Stage1)] = "Stage1";
	m_sceneString[static_cast<int>(SceneType::Stage2)] = "Stage2";
	m_sceneString[static_cast<int>(SceneType::Title)] = "Title";
	m_sceneString[static_cast<int>(SceneType::StageSelect)] = "StageSelect";
	m_sceneString[static_cast<int>(SceneType::Ranking)] = "Ranking";
	m_sceneString[static_cast<int>(SceneType::Test)] = "Test";
}

SceneDebug::~SceneDebug()
{
	/*処理無し*/
}

void SceneDebug::Init()
{
	/*処理無し*/
}

std::shared_ptr<SceneBase> SceneDebug::Update()
{
	// ボタンが押されたら選択されたシーンに遷移する
	if (Pad::isTrigger(PAD_INPUT_1))
	{
		return UpdateNextScene();
	}

	// カーソル更新
	UpdateCursor();

	// フェード更新
	UpdateFade();
	
	return shared_from_this();
}

void SceneDebug::Draw()
{
	// 文字列の描画
	for (int i = 0; i < static_cast<int>(m_sceneString.size()); i++)
	{
		if (m_count == i)
		{
			// 選ばれている文字は赤くする
			DrawString(static_cast<int>(kCursorPosX),
				static_cast<int>(kCursorPosY + (i * kCursorMove)),
				m_sceneString[i].c_str(),
				0xff0000);
		}
		else
		{
			// その他の文字は白
			DrawString(static_cast<int>(kCursorPosX),
				static_cast<int>(kCursorPosY + (i * kCursorMove)),
				m_sceneString[i].c_str(),
				0xffffff);
		}
	}

	DrawFormatString(0, 0, 0x00ffff, "Debug");
	DrawFormatString(60, 0, 0x00ffff, "シーンを選択してください(Aで決定)");

	DrawFade();
}

void SceneDebug::End()
{
	/*処理無し*/
}

void SceneDebug::UpdateCursor()
{
	// カーソルの移動
	if (Pad::isTrigger(PAD_INPUT_UP))
	{
		m_count--;
		// カーソルがループするようにする
		if (m_count < 0) m_count = static_cast<int>(m_sceneString.size() - 1);
	}
	else if (Pad::isTrigger(PAD_INPUT_DOWN))
	{
		m_count++;
		// カーソルがループするようにする
		if (m_count >= static_cast<int>(m_sceneString.size())) m_count = 0;
	}
}

std::shared_ptr<SceneBase> SceneDebug::UpdateNextScene()
{
	// 次のシーンの設定
	std::shared_ptr<SceneBase> nextScene = nullptr;

	// 選択されたシーンに遷移する
	switch (m_count)
	{
	case static_cast<int>(SceneType::Debug):
		nextScene = std::make_shared<SceneDebug>();
		break;
	case static_cast<int>(SceneType::Stage1):
		nextScene = std::make_shared<SceneStage>(Game::Stage::Stage1);
		break;
	case static_cast<int>(SceneType::Stage2):
		nextScene = std::make_shared<SceneStage>(Game::Stage::Stage2);
		break;
	case static_cast<int>(SceneType::Title):
		nextScene = std::make_shared<SceneTitle>();
		break;
	case static_cast<int>(SceneType::StageSelect):
		nextScene = std::make_shared<SceneSelect>();
		break;
	case static_cast<int>(SceneType::Ranking):
		nextScene = std::make_shared<SceneRanking>();
		break;
	case static_cast<int>(SceneType::Test):
		nextScene = std::make_shared<SceneStage>(Game::Stage::Test);
		break;
	default:
		// 何も選択されていなかった場合はシーン遷移を行わないようにする
		return shared_from_this();
		break;
	}
	return nextScene;
}
