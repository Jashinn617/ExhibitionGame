#include "EnemyManager.h"
#include "EnemyBase.h"
#include "SpownEnemy.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

namespace
{
	// スポーンファイル名
	const char* const kSpownFile[static_cast<int>(Game::Stage::StageNum)] =
	{
		"Data/File/Enemy/TestEnemySpown.csv",
		"Data/File/Enemy/Stage1EnemySpown.csv",
		"Data/File/Enemy/Stage2EnemySpown.csv",
		"Data/File/Enemy/TestEnemySpown.csv"
	};
}


EnemyManager::EnemyManager(Game::Stage stage, ObjectManager* pObjectManager):
	m_isSpown(true),
	m_isClear(false),
	m_pObjectManager(pObjectManager)
{
	// レベルによって出現する敵を変える
	m_pSpown = std::make_shared<SpownEnemy>(kSpownFile[static_cast<int>(stage)], m_pObjectManager);

	// チュートリアル時は後から敵を出す
	if (stage == Game::Stage::Tutorial)
	{
		m_isSpown = false;
	}
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Init()
{
	m_pSpown->Spown();
}

void EnemyManager::Update()
{
	/*処理無し*/
}

void EnemyManager::Draw()
{
	/*処理無し*/
}
