#include "SceneOption.h"
#include "../Util/Input.h"

namespace
{

}

SceneOption::SceneOption(std::shared_ptr<SceneBase> prevScene):
	m_isSceneEnd(false),
	m_pPrevScene(prevScene)
{
}

SceneOption::~SceneOption()
{
}

void SceneOption::Init()
{
}

std::shared_ptr<SceneBase> SceneOption::Update(Input& input)
{
	if (m_isSceneEnd)
	{
		return m_pPrevScene;
	}

	return shared_from_this();
}

void SceneOption::Draw()
{
}

void SceneOption::End()
{
}
