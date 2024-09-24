#pragma once
#include "SceneBase.h"
class SceneResult : public SceneBase
{
public:
	SceneResult();
	virtual ~SceneResult();


	virtual void Init();

	virtual std::shared_ptr<SceneBase> Update(Input& input);
	virtual void Draw();
	virtual void End();
};

