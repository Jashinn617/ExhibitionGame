#include "ItemManager.h"
#include "ItemBase.h"
#include "SpownItem.h"
#include "CountGetItem.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

namespace
{
	// スポーンファイル名
	const char* const kSpownFile[static_cast<int>(Game::Stage::StageNum)] =
	{
		"Data/File/Item/TestItemSpown.csv",
		"Data/File/Item/Stage1ItemSpown.csv",
		"Data/File/Item/Stage2ItemSpown.csv",
		"Data/File/Item/TestItemSpown.csv"
	};
}

ItemManager::ItemManager(Game::Stage stage, ObjectManager* pObjectManager):
	m_isSpown(true),
	m_isClear(false),
	m_pObjectManager(pObjectManager)
{
	// レベルによって出現するアイテムを変える
	m_pSpown = std::make_shared<SpownItem>(kSpownFile[static_cast<int>(stage)], m_pObjectManager);

	// チュートリアル時は後からアイテムを出す
	if (stage == Game::Stage::Tutorial)
	{
		m_isSpown = false;
	}

	m_pCountGetItem = std::make_shared<CountGetItem>(m_pSpown->GetItemNum());
}

ItemManager::~ItemManager()
{
	/*処理無し*/
}

void ItemManager::Init()
{
	m_pSpown->Spown();
}

void ItemManager::Update()
{
	/*処理無し*/
}

void ItemManager::Draw()
{
	m_pCountGetItem->Draw();
}

void ItemManager::AddGetNum()
{
	// 取得数の追加
	m_pCountGetItem->Add();

	// 全て取った場合
	if (m_pCountGetItem->IsCountMax())
	{
		// クリア
		m_isClear = true;
	}

}
