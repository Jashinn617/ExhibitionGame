#include "Field.h"
#include "Model.h"
#include "ObjectManager.h"
#include "Player/Player.h"
#include "../Shader/ToonShader.h"
#include "../Shader/ShadowMapShader.h"

namespace
{
	// フィールドのファイル名
	// ステージごとに変える
	const char* const kFileName[static_cast<int>(Game::Stage::StageNum)]
	{
		"Data/Model/Stage/TestStage.mv1",
		"Data/Model/Stage/Stage1.mv1",
		"Data/Model/Stage/Stage2.mv1",
	};
}

Field::Field(Game::Stage stage)
{
	// モデルの生成
	m_pModel = std::make_shared<Model>(kFileName[static_cast<int>(stage)]);

	m_pModel->SetScale(VGet(0.2f, 0.2f, 0.2f));
	m_pModel->SetPos(VGet(0.0f, -200.0f, 0.0f));
}

Field::~Field()
{
	/*処理無し*/
}

void Field::Init()
{
	/*処理無し*/
}

void Field::Update()
{
	m_pModel->Update();
}

void Field::Draw(std::shared_ptr<ToonShader> pToonShader)
{
	/*処理無し*/
	m_pModel->Draw();
}

void Field::ShadowMapDraw(std::shared_ptr<ShadowMapShader> pShadoeMapShader)
{
	// 影の描画
	m_pModel->Draw();
}

void Field::Draw2D()
{
	/*処理無し*/
}

void Field::StageClear()
{
	/*処理無し*/
}

void Field::GameEnd()
{
	m_isGameEnd = true;
}
