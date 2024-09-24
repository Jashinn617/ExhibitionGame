#include "Context.h"
#include "State.h"

Context::Context(std::function<void(void)> changeState):
	m_pNowState(nullptr),
	m_changeState(changeState)
{
}

void Context::Update()
{
	m_pNowState->Update();
}

void Context::AddState(std::function<void(void)> listener, Context::StateData state)
{
	// ステイトを追加する
	m_pState.push_back(std::make_shared<State>(listener, state));
}

void Context::ChangeState(StateData stateData)
{
	// ステイトがひとつもない場合先頭のステイト状態にし(Idle)処理を終了する
	if (m_pNowState == nullptr)
	{
		m_pNowState = m_pState.front();
		return;
	}
	// 現在の状態と変更する状態が同じ場合処理を終了する
	if (m_pNowState->GetStateData() == stateData) return;

	// ステイトを変更する
	for (auto& state : m_pState)
	{
		// 変更するステイトを見つけたら処理を終了する
		if (state->GetStateData() == stateData)
		{
			m_changeState();
			m_pNowState = state;
			return;
		}
	}
}

Context::StateData Context::GetState() const
{
	// 現在のステイトを返す
	return m_pNowState->GetStateData();
}
