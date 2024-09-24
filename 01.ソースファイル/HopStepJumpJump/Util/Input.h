#pragma once
#include <array>
#include <map>
#include <unordered_map>
#include <string>

using namespace std;

/// <summary>
/// 入力デバイス
/// </summary>
enum class InputType
{
	kwybd,		// キーボード
	pad			// ゲームパッド
};

// コマンド名,入力<入力デバイス,入力>
using InputTable_t = unordered_map<string, map<InputType, int>>;

/// <summary>
/// 入力の管理をするクラス
/// </summary>
class Input
{
public:
	/// <summary>
	/// 押されたコマンドの対応表
	/// </summary>
	Input();

	/// <summary>
	/// 入力情報の更新
	/// </summary>
	void Update();

	/// <summary>
	/// 指定のコマンドが押された瞬間なのかを返す
	/// </summary>
	/// <param name="command">コマンド名</param>
	/// <returns></returns>
	bool IsTriggered(const char* command)const;

	/// <summary>
	/// 指定のコマンドが今押されているかを返す
	/// </summary>
	/// <param name="command">コマンド名</param>
	/// <returns></returns>
	bool IsPressing(const char* command)const;

	/// <summary>
	/// 指定のコマンドが離された瞬間なのかを返す
	/// </summary>
	/// <param name="command">コマンド名</param>
	/// <returns></returns>
	bool IsReleased(const char* command)const;


private:
	InputTable_t m_commandTable;	// コマンド名と入力をペアにしたテーブル

	// コマンドの入力を覚えておく
	map<string, bool> m_nowInputData;	// 現在の入力
	map<string, bool>m_lastInputData;	// 直前の入力


};

