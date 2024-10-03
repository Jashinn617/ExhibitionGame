#include "SceneManager.h"
#include "SceneDebug.h"
#include "SceneTitle.h"

#include "../Util/Pad.h"
#include "../Util/Game.h"

#include <assert.h>

namespace
{
	constexpr int kBarPosX = 0;								// 処理バーの座標X
	constexpr int kBarPosY = Game::kScreenHeight - 48;		// 処理バーの座標Y
	constexpr int kUpdateBarColor = 0x0000ff;					// 更新の処理バーの色
	constexpr int kDrawBarColor = 0xff0000;						// 描画の処理バーの色
}

SceneManager::SceneManager() :
	m_pScene(nullptr),
	m_updateTime(0),
	m_drawTime(0)
{
	/*処理無し*/
}

SceneManager::~SceneManager()
{
	/*処理無し*/
}

void SceneManager::Init()
{
#ifdef _DEBUG
	// 最初のシーンのメモリを確保する
	// デバッグ用シーンに飛ぶ
	m_pScene = std::make_shared<SceneDebug>();
#else
	// 最初のシーンのメモリを確保する
	// タイトルシーンに飛ぶ
	m_pScene = std::make_shared<SceneTitle>();
#endif // _DEBUG

	// シーンの初期化
	m_pScene->Init();
}

bool SceneManager::Update()
{
#ifdef _DEBUG
	// 更新前ローディング時間の取得
	LONGLONG start = GetNowHiPerformanceCount();
#endif // _DEBUG

	// シーンがnullだった場合は止まる
	assert(m_pScene);

	// パッド情報の更新
	Pad::Update();

	// 次のシーンの取得
	std::shared_ptr<SceneBase> pNext = m_pScene->Update();

	// 次のシーンが存在していなかった場合
	if (!pNext)
	{
		// 現在処理中のシーンの終了処理
		m_pScene->End();
		// nullで終了
		return false;
	}

	// 次のシーンが現在のシーンでは無かった場合
	if (pNext != m_pScene)
	{
		// 現在処理中のシーンの終了処理
		m_pScene->End();

		// Updateが返した新しいシーンの開始処理を行う
		m_pScene = pNext;
		m_pScene->Init();
	}

#ifdef _DEBUG
	// 更新時の処理速度を測る
	m_updateTime = static_cast<float>(GetNowHiPerformanceCount() - start);
#endif // _DEBUG

	return true;
}

void SceneManager::Draw()
{
#ifdef _DEBUG
	// 描画前ローディング時間の取得	
	LONGLONG start = GetNowHiPerformanceCount();
#endif // _DEBUG

	// シーンがnullだった場合は止まる
	assert(m_pScene);
	// 描画
	m_pScene->Draw();

#ifdef _DEBUG
	// 描画時の処理速度を測る
	m_drawTime = static_cast<float>(GetNowHiPerformanceCount() - start);

	/*処理バーの表示*/
	// 描画処理バー
	float rate = (m_updateTime + m_drawTime) / 16666.6f;
	float width = Game::kScreenWidth * rate;
	DrawBox(kBarPosX, kBarPosY, static_cast<int>(width), Game::kScreenHeight, kDrawBarColor, true);
	// 更新処理バー
	rate = m_updateTime / 16666.6f;
	width = Game::kScreenWidth * rate;
	DrawBox(kBarPosX, kBarPosY, static_cast<int>(width), Game::kScreenHeight, kUpdateBarColor, true);

#endif // _DEBUG
}

void SceneManager::End()
{
	// シーンがnullだった場合は止まる
	assert(m_pScene);
	// 終了処理
	m_pScene->End();
}
