#include "SceneManager.h"
#include "SceneDebug.h"
#include "SceneTitle.h"

#include "../Util/Input.h"
#include "../Util/Pad.h"
#include "../Util/Game.h"

#include <assert.h>

namespace
{
	constexpr float kBarPosX = 0;								// 処理バーの座標X
	constexpr float kBarPosY = Game::kScreenHeight - 48;		// 処理バーの座標Y
	constexpr int kUpdateBarColor = 0x0000ff;					// 更新の処理バーの色
	constexpr int kDrawBarColor = 0xff0000;						// 描画の処理バーの色
}

SceneManager::SceneManager() :
	m_pScene(nullptr),
	m_updateTime(0),
	m_drawTime(0)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
#ifdef _DEBUG
	// 最初のシーンのメモリを確保する
	m_pScene = make_shared<SceneDebug>();
#else
	// 最初のシーンのメモリを確保する
	m_pScene = make_shared<SceneTitle>();
#endif // _DEBUG

	m_pScene->Init();
}

bool SceneManager::Update(Input& input)
{
	// 更新前のローディング時間の取得
	LONGLONG start = GetNowHiPerformanceCount();

	assert(m_pScene);

	// パッド情報の更新
	Pad::Update();

	shared_ptr<SceneBase> pNext = m_pScene->Update(input);

	if (!pNext)
	{
		m_pScene->End();
		// nullで終了
		return false;
	}

	if (pNext != m_pScene)
	{
		// 現在処理中のシーンの終了処理
		m_pScene->End();

		// Updateが返した新しいシーンの開始処理を行う
		m_pScene = pNext;
		m_pScene->Init();
	}

	m_updateTime = static_cast<float>(GetNowHiPerformanceCount() - start);

	return true;
}

void SceneManager::Draw()
{
	assert(m_pScene);

	LONGLONG start = GetNowHiPerformanceCount();

	m_pScene->Draw();

#ifdef _DEBUG
	// 更新、描画時の処理速度を測る
	m_drawTime = static_cast<float>(GetNowHiPerformanceCount() - start);

	// 処理バーの表示
	float rate = (m_updateTime + m_drawTime) / 16666.6f;
	float width = Game::kScreenWidth * rate;
	DrawBox(kBarPosX, kBarPosY, width, Game::kScreenHeight, kDrawBarColor, true);

	rate = m_updateTime / 16666.6f;
	width = Game::kScreenWidth * rate;
	DrawBox(kBarPosX, kBarPosY, width, Game::kScreenHeight, kUpdateBarColor, true);

#endif // _DEBUG
}

void SceneManager::End()
{
	assert(m_pScene);

	m_pScene->End();
}
