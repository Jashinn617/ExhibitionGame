#include "SceneStage.h"
#include "StageSceneManager.h"
#include "SceneSelect.h"
#include "SceneDebug.h"
#include "SceneGameClear.h"
#include "SceneOption.h"
#include "SceneTitle.h"

#include "../Object/ObjectBase.h"
#include "../Object/Player/Player.h"
#include "../Object/Camera.h"
#include "../Object/Enemy/EnemyManager.h"
#include "../Object/Enemy/EnemyBase.h"
#include "../Object/Field.h"
#include "../Object/SkyDome.h"

#include "../Util/Collision.h"
#include "../Util/Input.h"
#include "../Util/Pad.h"
#include "../Util/SoundManager.h"

#include <cassert>
#include <functional>

namespace
{
	const char* const kOptionImgFileName[4] =
	{
		"Data/Img/Option/OptionText1.png",
		"Data/Img/Option/OptionText2.png",
		"Data/Img/Option/OptionText3.png",
		"Data/Img/Option/OptionText4.png",
	};

	constexpr int kOptionImgPosX = Game::kScreenWidth / 2;
	constexpr int kOptionImgPosY[4] = {150,450,650,850};
	constexpr float kButtonSize = 0.8f;
	constexpr float kButtonSelectSize = 1.0f;

	constexpr float kCameraNear = 5.0f;
	constexpr float kCameraFar = 5000.0f;

	constexpr int kSelectNum = 3;
	constexpr int kAlpha = 220;
}

SceneStage::SceneStage(Game::Stage stage) :
	m_count(0),
	m_isOption(false),
	m_isSceneEnd(false),
	m_nextScene(nullptr),
	m_stageKind(stage)
{
	m_pStageSceneManager = std::make_shared<StageSceneManager>(stage);

	// カメラの設定
	SetCameraNearFar(kCameraNear, kCameraFar);

	for (int i = 0; i < m_buttonSize.size(); i++)
	{
		m_buttonSize[i] = 0;
	}

	// 画像のロード
	for (int i = 0; i < m_optionH.size(); i++)
	{
		m_optionH[i] = LoadGraph(kOptionImgFileName[i]);
		assert(m_optionH[i] != -1);
	}

}

SceneStage::~SceneStage()
{
	// 画像のデリート
	for (int i = 0; i < m_optionH.size(); i++)
	{
		DeleteGraph(m_optionH[i]);
	}
}

void SceneStage::Init()
{
	m_pStageSceneManager->Init();
}

std::shared_ptr<SceneBase> SceneStage::Update(Input& input)
{
#ifdef _DEBUG
	// デバッグに飛ぶ
	if (Pad::isTrigger(PAD_INPUT_5))
	{
		return make_shared<SceneDebug>();
	}
#endif // _DEBUG

	// オプションを開く
	if (Pad::isTrigger(PAD_INPUT_8))
	{
		m_isOption = true;
		m_count = 0;

	}

	if (m_nextScene != nullptr)
	{
		return m_nextScene;
	}


	if (m_isOption)
	{
		UpdateOption();
	}
	else
	{


		m_pStageSceneManager->Update(input);

		if (m_pStageSceneManager->IsGameClear())
		{
			GameClear();
		}

		UpdateFade();

		if (m_pStageSceneManager->GetNextScene() == StageSceneManager::NextScene::GameScene)
		{
			return make_shared<SceneStage>(m_stageKind);
		}
		else if (m_pStageSceneManager->GetNextScene() == StageSceneManager::NextScene::Title)
		{
			return make_shared<SceneSelect>();
		}
		else if (m_pStageSceneManager->GetNextScene() == StageSceneManager::NextScene::My)
		{
			return shared_from_this();
		}

	}
	return shared_from_this();
}

void SceneStage::Draw()
{
	m_pStageSceneManager->Draw();

	if (m_isOption)
	{
		DrawOption();
	}

	DrawFade();
}

void SceneStage::End()
{
	/*処理無し*/
}

void SceneStage::DrawOption()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAlpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawRotaGraph(kOptionImgPosX, kOptionImgPosY[0],
		kButtonSize, 0.0f, m_optionH[0], true);


	DrawRotaGraph(kOptionImgPosX, kOptionImgPosY[1],
		m_buttonSize[0], 0.0f, m_optionH[1], true);
	DrawRotaGraph(kOptionImgPosX, kOptionImgPosY[2],
		m_buttonSize[1], 0.0f, m_optionH[2], true);
	DrawRotaGraph(kOptionImgPosX, kOptionImgPosY[3],
		m_buttonSize[2], 0.0f, m_optionH[3], true);

#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, "count:%d", m_count);
#endif // _DEBUG


}

void SceneStage::UpdateOption()
{
	// カウントの切り替え
	if (Pad::isTrigger(PAD_INPUT_UP))
	{
		SoundManager::GetInstance().Play("Select");
		m_count--;
	}
	if (Pad::isTrigger(PAD_INPUT_DOWN))
	{
		SoundManager::GetInstance().Play("Select");
		m_count++;
	}
	if (m_count >= kSelectNum) m_count = 0;
	if (m_count < 0) m_count = kSelectNum - 1;

	if (m_count == 0)
	{
		if (Pad::isTrigger(PAD_INPUT_1) || Pad::isTrigger(PAD_INPUT_2))
		{
			// SEを鳴らす
			SoundManager::GetInstance().Play("Decide");
			// ゲームに戻る
			m_isOption = false;
			m_nextScene = nullptr;
		}
	}
	else if (m_count == 1)
	{
		if (Pad::isTrigger(PAD_INPUT_1) || Pad::isTrigger(PAD_INPUT_2))
		{
			// BGMを消す
			SoundManager::GetInstance().DesignationStopSound("MainScene");

			// SEを鳴らす
			SoundManager::GetInstance().Play("SceneChange");
			// 最初から
			m_nextScene = std::make_shared<SceneStage>(m_stageKind);
		}
	}
	else if (m_count == 2)
	{
		if (Pad::isTrigger(PAD_INPUT_1) || Pad::isTrigger(PAD_INPUT_2))
		{
			// BGMを消す
			SoundManager::GetInstance().DesignationStopSound("MainScene");
			// SEを鳴らす
			SoundManager::GetInstance().Play("SceneChange");
			// セレクトに戻る
			m_nextScene = std::make_shared<SceneSelect>();
		}
	}

	for (int i = 0; i < m_buttonSize.size(); i++)
	{
		if (i == m_count)
		{
			m_buttonSize[i] = kButtonSelectSize;
		}
		else
		{
			m_buttonSize[i] = kButtonSize;
		}
	}
}

void SceneStage::GameClear()
{
	/*処理無し*/
}
