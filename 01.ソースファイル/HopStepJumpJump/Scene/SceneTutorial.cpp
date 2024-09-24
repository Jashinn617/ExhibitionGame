#include "SceneTutorial.h"
#include "../Util/Input.h"

SceneTutorial::SceneTutorial()
{
}

SceneTutorial::~SceneTutorial()
{
}

void SceneTutorial::Init()
{
}

std::shared_ptr<SceneBase> SceneTutorial::Update(Input& input)
{
	return shared_from_this();
}

void SceneTutorial::Draw()
{
}

void SceneTutorial::End()
{
}

void SceneTutorial::GameClear()
{
}
