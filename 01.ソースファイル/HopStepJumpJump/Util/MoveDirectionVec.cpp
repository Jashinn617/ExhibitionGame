#include "MoveDirectionVec.h"
#include "../Util/Pad.h"
#include "CharacterData.h"

namespace
{
	constexpr float kUp = -1.0f;	// 上移動方向ベクトル
	constexpr float kDown = 1.0f;	// 下移動方向ベクトル
	constexpr float kRight = 1.0f;	// 右移動方向ベクトル
	constexpr float kLeft = -1.0f;	// 左移動方向ベクトル
	constexpr float kZero = 0.0f;	// 移動方向ベクトルなし
}

MoveDirectionVec::MoveDirectionVec():
	m_directionVec{0.0f,0.0f,0.0f}
{
	/*処理無し*/
}

MoveDirectionVec::~MoveDirectionVec()
{
	/*処理無し*/
}

void MoveDirectionVec::Update()
{
	//パッドアナログ情報取得
	DINPUT_JOYSTATE input;

	input.X = 0;
	input.Y = 0;

	//移動角度
	float moveAngle = 0.0f;

	// 入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	// 移動ベクトルの初期化
	m_directionVec = VGet(0.0f, 0.0f, 0.0f);

	//スティックでのアナログ入力状態が反応しているときはそっちを優先する
	if (input.X != 0 || input.Y != 0)
	{
		m_directionVec.x = 0.001f * input.X;
		m_directionVec.z = 0.001f * input.Y;
	}
	else
	{
		// 左ボタンを押されたらプレイヤーの向きを左に
		if (Pad::isPress(PAD_INPUT_LEFT))
		{
			m_directionVec.x = kLeft;
		}
		// 右ボタンを押されたらプレイヤーの向き右に
		else if (Pad::isPress(PAD_INPUT_RIGHT))
		{
			m_directionVec.x = kRight;
		}
		else
		{
			m_directionVec.x = kZero;
		}

		// 上ボタンを押されたらプレイヤーの向きを上に
		if (Pad::isPress(PAD_INPUT_UP))
		{
			m_directionVec.z = kUp;
		}
		// 下ボタンを押されたらプレイヤーの向き下に
		else if (Pad::isPress(PAD_INPUT_DOWN))
		{
			m_directionVec.z = kDown;
		}
		else
		{
			m_directionVec.z = kZero;
		}
	}

	//移動方向ベクトルを正規化
	if (VSize(m_directionVec) >= 1.0f)
	{
		m_directionVec = VNorm(m_directionVec);
	}
}

void MoveDirectionVec::SetDirectionVec()
{
	// 移動方向ベクトルを初期化
	m_directionVec = VGet(0.0f, kUp, 0.0f);
}