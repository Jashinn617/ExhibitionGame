#include "DxLib.h"
#include "EffekseerForDXLib.h"

#include "Util/Game.h"
#include "Util//Pad.h"

#include "Scene/SceneManager.h"
#include "Effect/Effekseer3DManager.h"
#include "Util/SoundManager.h"

#include <memory>

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// windowモード設定
	ChangeWindowMode(true);

	// ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	// ゲームバーのタイトルの設定
	SetWindowText(Game::kTitleText);

	// スクリーンサイズの設定
	ChangeWindowMode(Game::kWindowMode);
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);	

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// Effekseerの初期化
	Effekseer_Init(8000);

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ
	SetChangeScreenModeGraphicsSystemResetFlag(false);

	// DXライブラリのデバイスロストした時のコールバックを設定する
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// Zバッファを使用する
	SetUseZBuffer3D(true);
	// Zバッファへの書き込みを行う
	SetWriteZBuffer3D(true);

	// ポリゴンの裏側は描画しない
	SetUseBackCulling(true);

	//// エフェクトのロード
	Effekseer3DManager::GetInstance();
	// サウンドのロード
	SoundManager::GetInstance();

	// ライトの設定
	SetLightPosition(Game::kLightPos);
	SetLightDirection(Game::kLightDir);

	// アスペクト比の設定
	SetCameraDotAspect(Game::kAspect);
	// カメラの視野角の設定
	SetupCamera_Perspective(Game::kFov);

	// シーン管理
	std::shared_ptr<SceneManager> pScene = std::make_shared<SceneManager>();

	// 初期化
	pScene->Init();

	
	while (ProcessMessage() == 0)
	{
		// このフレームの開始時刻を覚えておく
		LONGLONG time = GetNowHiPerformanceCount();

		// 描画を行う前に画面をクリアする
		ClearDrawScreen();

		// ゲームの処理
		// シーンが存在していなかったら強制終了する
		if(!pScene->Update()) break;
		// 描画
		pScene->Draw();

		// エフェクトの更新、描画
		Effekseer3DManager::GetInstance().Update();
		Effekseer3DManager::GetInstance().Draw();

		// 画面が切り替わるのを待つ
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	// シーンの終了処理
	pScene->End();

	// Effekseerの終了処理
	Effkseer_End();	
	// ＤＸライブラリ使用の終了処理
	DxLib_End();

	// ソフトの終了 
	return 0;				
}