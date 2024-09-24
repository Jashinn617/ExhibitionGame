#pragma once
#include "DxLib.h"

/// <summary>
/// ゲーム全体で使用する定数を定義するnamespace
/// </summary>
namespace Game
{
	const char* const kTitleText = "HOP!STEP!JUMP★JUMP!!";		// ウィンドウ名

#ifdef _DEBUG
	constexpr bool kWindowMode = true;
#else
	constexpr bool kWindowMode = false;
#endif // _DEBUG

	/*ウィンドウサイズ*/
	constexpr int kScreenWidth = 1920;	// 画面横サイズ
	constexpr int kScreenHeight = 1080;	// 画面縦サイズ

	constexpr int kColorDepth = 32;		// カラービット数
	
	constexpr VECTOR kLightPos = { -1000.0f,2000.0f,-1000.0f };		// ライトの位置
	constexpr VECTOR kLightDir = { -1.0f, -1.0f, -1.0f };			// ライトの向き

	constexpr float kFov = 60.0f * DX_PI_F / 180.0f;			// カメラの視野角
	constexpr float kAspect = static_cast<float>(kScreenWidth) / static_cast<float>(kScreenHeight);	// アスペクト比


	// ステージ名
	enum class Stage
	{
		Test,
		Stage1,
		Stage2,
		Tutorial,
		StageNum,
	};
}
