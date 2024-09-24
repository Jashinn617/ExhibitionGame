#pragma once
#include "SceneBase.h"

class Input;

class SceneTutorial : public SceneBase
{
public:
	enum class UIType
	{

	};

	enum class TextType
	{

	};

public:
	SceneTutorial();
	virtual ~SceneTutorial();

	virtual void Init() override final;
	virtual std::shared_ptr<SceneBase> Update(Input& input)override final;
	virtual void Draw() override final;
	virtual void End() override final;

	virtual SceneKind GetSceneKind() { return SceneKind::Tutorial; }

private:	// ä÷êî
	void GameClear();

private:	// ïœêî

};

