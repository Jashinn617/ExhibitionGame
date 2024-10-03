#include "SceneStage.h"
#include "StageSceneManager.h"
#include "SceneSelect.h"
#include "SceneDebug.h"
#include "SceneTitle.h"

#include "../Object/ObjectBase.h"
#include "../Object/Player/Player.h"
#include "../Object/Camera.h"
#include "../Object/Enemy/EnemyManager.h"
#include "../Object/Enemy/EnemyBase.h"
#include "../Object/Field.h"
#include "../Object/SkyDome.h"

#include "../Util/Collision.h"
#include "../Util/Pad.h"
#include "../Util/SoundManager.h"

#include <cassert>
#include <functional>

namespace
{
	// オプション画像ファイル
	const char* const kOptionImgFileName[4] =
	{
		"Data/Img/Option/OptionText1.png",
		"Data/Img/Option/OptionText2.png",
		"Data/Img/Option/OptionText3.png",
		"Data/Img/Option/OptionText4.png",
	};
	constexpr int kOptionImgPosX = Game::kScreenWidth / 2;	// オプション画像座標X
	constexpr int kOptionImgPosY[4] = { 150,450,650,850 };	// オプション画像座標X
	constexpr float kButtonSize = 0.8f;						// オプション画像サイズ
	constexpr float kButtonSelectSize = 1.0f;				// 選択されているオプション画像サイズ
	constexpr int kOptionSelectNum = 3;						// オプション時のセレクトの数
	constexpr int kOptionBackAlpha = 220;					// オプション時の背景不透明度

	constexpr float kCameraNear = 5.0f;						// カメラ手前クリップ距離
	constexpr float kCameraFar = 5000.0f;					// カメラ奥クリップ距離
}

SceneStage::SceneStage(Game::Stage stage) :
	m_cursorCount(0),
	m_isOption(false),
	m_isSceneEnd(false),
	m_nextScene(nullptr),
	m_stageKind(stage)
{
	// ステージマネージャーポインタの作成
	m_pStageSceneManager = std::make_shared<StageSceneManager>(stage);

	// カメラの設定
	SetCameraNearFar(kCameraNear, kCameraFar);

	// ボタンサイズの初期化
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
	// ステージマネージャーの初期化
	m_pStageSceneManager->Init();
}

std::shared_ptr<SceneBase> SceneStage::Update()
{
	// フェード更新
	UpdateFade();

#ifdef _DEBUG
	// LBが押された場合
	if (Pad::isTrigger(PAD_INPUT_5))
	{
		// デバッグに飛ぶ
		return std::make_shared<SceneDebug>();
	}
#endif // _DEBUG

	// Startボタンが押された場合
	if (Pad::isTrigger(PAD_INPUT_8))
	{
		// オプションを開く
		m_isOption = true;
		// カーソルのカウントを初期化する
		m_cursorCount = 0;
	}

	// 次のシーンに何かしら入っていたら
	if (m_nextScene != nullptr)
	{
		// 次のシーンを変えsう
		return m_nextScene;
	}

	// オプションが開かれていた場合
	if (m_isOption)
	{
		// オプション更新
		UpdateOption();
	}
	else
	{
		// ステージマネージャー更新
		m_pStageSceneManager->Update();

		// 次のシーンの設定
		switch (m_pStageSceneManager->GetNextScene())
		{
		case StageSceneManager::NextScene::GameScene:
			// ゲームシーン
			return std::make_shared<SceneStage>(m_stageKind);
			break;
		case StageSceneManager::NextScene::Select:
			// セレクトシーン
			return std::make_shared<SceneSelect>();
			break;
		case StageSceneManager::NextScene::My:
			// 自分のシーン
			return shared_from_this();
			break;
		default:
			break;
		}
	}
	return shared_from_this();
}

void SceneStage::Draw()
{
	// ステージマネージャー描画
	m_pStageSceneManager->Draw();

	// オプションが開かれていた場合
	if (m_isOption)
	{
		// オプション描画
		DrawOption();
	}

	// フェード描画
	DrawFade();
}

void SceneStage::End()
{
	/*処理無し*/
}

void SceneStage::DrawOption()
{
	// 黒背景
	// 不透明度を少し下げる
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kOptionBackAlpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// テキスト描画
	// サイズは固定
	DrawRotaGraph(kOptionImgPosX, kOptionImgPosY[0],
		kButtonSize, 0.0f, m_optionH[0], true);

	// テキスト描画
	// 選択されているかどうかでサイズが変わる
	DrawRotaGraph(kOptionImgPosX, kOptionImgPosY[1],
		m_buttonSize[0], 0.0f, m_optionH[1], true);
	DrawRotaGraph(kOptionImgPosX, kOptionImgPosY[2],
		m_buttonSize[1], 0.0f, m_optionH[2], true);
	DrawRotaGraph(kOptionImgPosX, kOptionImgPosY[3],
		m_buttonSize[2], 0.0f, m_optionH[3], true);
}

void SceneStage::UpdateOption()
{
	/*カウントの切り替え*/
	if (Pad::isTrigger(PAD_INPUT_UP))
	{
		SoundManager::GetInstance().Play("Select");
		m_cursorCount--;

		// カーソルがループするようにする
		if (m_cursorCount >= kOptionSelectNum) m_cursorCount = 0;
	}
	if (Pad::isTrigger(PAD_INPUT_DOWN))
	{
		SoundManager::GetInstance().Play("Select");
		m_cursorCount++;

		// カーソルがループするようにする
		if (m_cursorCount < 0) m_cursorCount = kOptionSelectNum - 1;
	}

	// AボタンかBボタンが押された場合
	if (Pad::isTrigger(PAD_INPUT_1) || Pad::isTrigger(PAD_INPUT_2))
	{
		if (m_cursorCount == 0)
		{
			// SEを鳴らす
			SoundManager::GetInstance().Play("Decide");
			// ゲームに戻る
			m_isOption = false;
			m_nextScene = nullptr;
		}
		else if (m_cursorCount == 1)
		{
			// BGMを消す
			SoundManager::GetInstance().DesignationStopSound("MainScene");
			// SEを鳴らす
			SoundManager::GetInstance().Play("SceneChange");
			// 最初から
			m_nextScene = std::make_shared<SceneStage>(m_stageKind);

		}
		else if (m_cursorCount == 2)
		{
			// BGMを消す
			SoundManager::GetInstance().DesignationStopSound("MainScene");
			// SEを鳴らす
			SoundManager::GetInstance().Play("SceneChange");
			// セレクトに戻る
			m_nextScene = std::make_shared<SceneSelect>();
		}
	}

	// ボタンサイズの切り替え
	for (int i = 0; i < m_buttonSize.size(); i++)
	{
		if (i == m_cursorCount)	// 選択されていた場合
		{
			m_buttonSize[i] = kButtonSelectSize;
		}
		else	// 選択されていなかった場合
		{
			m_buttonSize[i] = kButtonSize;
		}
	}
}