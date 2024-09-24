#pragma once
#include "Context.h"

#include <functional>

class State
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stateUpdate"></param>
	/// <param name="stateData"></param>
	State(std::function<void(void)> stateUpdate, Context::StateData state);

	void Update();

	Context::StateData GetStateData() const { return m_stateKind; }

private:
	std::function<void(void)> m_stateUpdate;	// 更新処理
	Context::StateData m_stateKind;				// 状態の種類
};

