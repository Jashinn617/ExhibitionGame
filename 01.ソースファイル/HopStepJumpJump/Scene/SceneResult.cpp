#include "DxLib.h"
#include "SceneResult.h"

SceneResult::SceneResult()
{
}

SceneResult::~SceneResult()
{
}

void SceneResult::Init()
{
}

std::shared_ptr<SceneBase> SceneResult::Update(Input& input)
{
	return shared_from_this();
}

void SceneResult::Draw()
{
#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, "Result");
#endif // _DEBUG   
}

void SceneResult::End()
{
}
