#pragma once
#include "ObjectBase.h"
#include "../Util/Game.h"

class Model;

class Field : public ObjectBase
{
public:
	Field() {};
	Field(Game::Stage stage);
	~Field();

	void Init();
	void Update() override final;
	
	virtual void Draw(std::shared_ptr<ToonShader> pToonShader) override final;

	virtual void ShadowMapDraw(std::shared_ptr<ShadowMapShader> pShadoeMapShader) override final;

	virtual void Draw2D() override final;

	virtual void StageClear() override final;

	/// <summary>
	/// ƒQ[ƒ€I—¹ˆ—
	/// </summary>
	void GameEnd() override final;

	/// <summary>
	/// ©g‚Ì“–‚½‚è”»’èî•ñ
	/// </summary>
	/// <returns></returns>
	virtual ColType GetColType() const override final { return ColType::Field; }

private:
};

