#pragma once
#include <vector>
#include <memory>
#include <functional>

class State;

class Context
{
public:
	// ステイトの種類情報
	enum class StateData
	{
		Idle,		// 待機
		Walk,		// 歩き
		Jump,		// ジャンプ
		Dash,		// ダッシュ
		KnockBack,	// ノックバック
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="changeState">ステイト関数</param>
	Context(std::function<void(void)> changeState);

	void Update();

	/// <summary>
	/// ステイトの追加
	/// </summary>
	/// <param name="listener">ステイトの関数</param>
	/// <param name="state">ステイトの種類</param>
	void AddState(std::function<void(void)> listener, Context::StateData stateData);

	/// <summary>
	/// ステイト変更
	/// </summary>
	/// <param name="state">変更先のステイト</param>
	void ChangeState(StateData state);

	/// <summary>
	/// 現在のステイトを返す
	/// </summary>
	/// <returns></returns>
	StateData GetState() const;

private:
	std::vector<std::shared_ptr<State>> m_pState;	// 全てのステイト
	std::shared_ptr<State> m_pNowState;				// 現在のステイト
	std::function<void(void)> m_changeState;		// ステイト変更時の処理関数
};

