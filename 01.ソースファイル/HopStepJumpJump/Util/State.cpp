#include "State.h"

State::State(std::function<void(void)> stateUpdate, Context::StateData state):
	m_stateUpdate(stateUpdate),
	m_stateKind(state)
{
}

void State::Update()
{
	m_stateUpdate();
}
