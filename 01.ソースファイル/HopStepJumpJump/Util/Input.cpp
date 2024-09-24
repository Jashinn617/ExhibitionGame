#include "Input.h"
#include "DxLib.h"

Input::Input()
{
	// 上ボタン
	m_commandTable["up"] = { {InputType::kwybd,KEY_INPUT_UP},
								{InputType::pad,PAD_INPUT_UP} };

	// 下ボタン
	m_commandTable["down"] = { {InputType::kwybd,KEY_INPUT_DOWN},
								{InputType::pad,PAD_INPUT_DOWN} };

	// 右ボタン
	m_commandTable["right"] = { {InputType::kwybd,KEY_INPUT_RIGHT},
								{InputType::pad,PAD_INPUT_RIGHT} };

	// 左ボタン
	m_commandTable["left"] = { {InputType::kwybd,KEY_INPUT_LEFT},
								{InputType::pad,PAD_INPUT_LEFT} };

	// ジャンプボタン
	m_commandTable["jump"] = { {InputType::kwybd,KEY_INPUT_Z},
								{InputType::pad,PAD_INPUT_A} };

	// セレクトボタン
	m_commandTable["select"] = { {InputType::kwybd,KEY_INPUT_Z},
								{InputType::pad,PAD_INPUT_A} };

	// ダッシュボタン
	m_commandTable["dash"] = { {InputType::kwybd,KEY_INPUT_X},
								{InputType::pad,PAD_INPUT_B} };
	// Rボタン
	m_commandTable["R"] = { {InputType::kwybd,KEY_INPUT_S},
								{InputType::pad,PAD_INPUT_5} };

	// Lボタン
	m_commandTable["L"] = { {InputType::kwybd,KEY_INPUT_D},
								{InputType::pad,PAD_INPUT_6} };
}

void Input::Update()
{
	// 直前入力をコピーしておく
	m_lastInputData = m_nowInputData;

	// ハードウェア入力チェック
	char keystate[256];	// キーボード用の配列
	GetHitKeyStateAll(keystate);	// キーボード入力の取得
	int padstate = GetJoypadInputState(DX_INPUT_PAD1);	// パッド情報の取得

	// 登録された情報とハードの情報を照らし合わせる
	// それをもとにinputDataの内容を更新する
	for (const auto& cmd : m_commandTable)
	{
		// コマンドの名前から入力データを作る
		auto& input = m_nowInputData[cmd.first];

		for (const auto& hardIO : cmd.second)
		{

			input = false;
			// キーボードのチェック
			if (hardIO.first == InputType::kwybd)
			{
				// キーボードが押されていた場合
				if (keystate[hardIO.second])
				{
					input = true;
					break;	// 同時押しをできないようにここでbreakする
				}
			}
			// ゲームパッドのチェック
			else if (hardIO.first == InputType::pad)
			{
				// ゲームパッドが押されていた場合
				if (padstate & hardIO.second)
				{
					input = true;
					break;
				}
			}
		}
	}
}

bool Input::IsTriggered(const char* command) const
{
	// 引数で渡されたコマンド名と一致するコマンド名があるか調べる
	auto it = m_nowInputData.find(command);
	// 一致するコマンド名がなかった場合はfalseを返す
	if (it == m_nowInputData.end())
	{
		return false;
	}

	// 現在押されており、直前に押されていなかった場合、trueを返す
	return(m_nowInputData.at(command) && !m_lastInputData.at(command));
}

bool Input::IsPressing(const char* command) const
{
	// 引数で渡されたコマンド名と一致するコマンド名があるか調べる
	auto it = m_nowInputData.find(command);
	// 一致するコマンド名がなかった場合はfalseを返す
	if (it == m_nowInputData.end())
	{
		return false;
	}

	// 現在押され手入ればtrueを返す
	return(m_nowInputData.at(command));
}

bool Input::IsReleased(const char* command) const
{
	// 引数で渡されたコマンド名と一致するコマンド名があるか調べる
	auto it = m_nowInputData.find(command);
	// 一致するコマンド名がなかった場合はfalseを返す
	if (it == m_nowInputData.end())
	{
		return false;
	}

	// 現在押されてなく、直前に押されていたらtrueを返す
	return(!m_nowInputData.at(command) && m_lastInputData.at(command));
}
