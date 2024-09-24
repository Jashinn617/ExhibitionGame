#include "SpownItem.h"

#include "ItemBase.h"
#include "ItemStop.h"
#include "ItemRotate.h"

#include "../ObjectBase.h"
#include "../ObjectManager.h"

#include <assert.h>

namespace
{
	constexpr float kHeightPos = 30.0f;	// 高さ調整
	constexpr int kFirstCreateNum = 1;	// 最初の出現番号
}

SpownItem::SpownItem(const char* fileName, ObjectManager* pObjectManager) :
	m_createNum(kFirstCreateNum),
	m_fileName(fileName),
	m_pObjectManager(pObjectManager)
{
	// アイテムの生成
	Create();
}

SpownItem::~SpownItem()
{
	std::list<Data*>::iterator it = m_pData.begin();
	while (it != m_pData.end())
	{
		auto obj = (*it);

		delete obj;
		obj = nullptr;
		it = m_pData.erase(it);
	}
}

void SpownItem::Init()
{
	/*処理無し*/
}

void SpownItem::Spown()
{
	Appearance();
}

bool SpownItem::IsExist()
{
	// アイテムのデータがなかった場合
	if (m_pData.size() == 0) return false;

	// アイテムの全データを調べる
	for (auto& data : m_pData)
	{
		// アイテムが存在していた場合
		if (data->pItem->GetInfo().isExist)
		{
			return true;
		}
	}

	return false;
}

void SpownItem::Create()
{
	// ファイル名
	std::ifstream ifs(m_fileName);

	while (getline(ifs, m_line))
	{
		// csvデータの1行を','で複数の文字列に分割する
		std::vector<std::string> strvec = Split(m_line, ',');

		// strvec[0]	: 出現番号			int
		// strvec[1]	: アイテムの種類	string
		// strvec[2]	: アイテムのX座標	float
		// strvec[3]	: アイテムのY座標	float
		// strvec[4]	: アイテムのZ座標	float

		// 読み取ったデータを座標に変換する
		VECTOR pos = VGet(stof(strvec[2]), stof(strvec[3]), stof(strvec[4]));

		// 高さ調整
		pos.y += kHeightPos;

		// データに合う敵の追加
		Add(stoi(strvec[0]), strvec[1], pos);
	}
}

void SpownItem::Appearance()
{
	for (auto& data : m_pData)
	{
		m_pObjectManager->AddObject(data->pItem);
		data->pItem->Init();
	}
}

void SpownItem::Add(int createNum, std::string itemName, VECTOR pos)
{
	// モデルファイル名
	const char* const modelFile = "Data/Model/Item/Key.mv1";
	// モデルハンドルの取得
	int modelH = MV1LoadModel(modelFile);
	assert(modelH != -1);
	// itemNameに合う敵の追加
	if (itemName == "Stop")
	{
		m_pData.push_back(new Data);
		m_pData.back()->pItem = new ItemStop(pos);
	}
	else
	{
		return;
	}

	m_pData.back()->pItem->SetModelHandle(modelH);
	m_pData.back()->index = createNum;
	m_pData.back()->name = itemName;
}

std::vector<std::string> SpownItem::Split(std::string& input, char delimiter)
{
	std::istringstream stream(input);
	std::string filed;
	std::vector<std::string> result;

	while (getline(stream, filed, delimiter))
	{
		result.push_back(filed);
	}

	return result;
}
