#include "SpownEnemy.h"

#include "EnemyBase.h"
#include "EnemyBee.h"
#include "EnemySkull.h"

#include "../ObjectBase.h"
#include "../ObjectManager.h"

#include <assert.h>

namespace
{
	constexpr float kHeightPos = 10.0f;	// 高さ調整
}

SpownEnemy::SpownEnemy(const char* fileName, ObjectManager* pObjectManager):
	m_createNum(0),
	m_fileName(fileName),
	m_pObjectManager(pObjectManager)
{
	// アイテムの生成
	Create();
}

SpownEnemy::~SpownEnemy()
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

void SpownEnemy::Init()
{
}

void SpownEnemy::Spown()
{
	Appearance();
}

bool SpownEnemy::IsExist()
{
	// アイテムのデータがなかった場合
	if (m_pData.size() == 0) return false;

	// アイテムの全データを調べる
	for (auto& data : m_pData)
	{
		// アイテムが存在していた場合
		if (data->pEnemy->GetInfo().isExist)
		{
			return true;
		}
	}

	return false;
}

void SpownEnemy::Create()
{
	// ファイル名
	std::ifstream ifs(m_fileName);

	while (getline(ifs, m_line))
	{
		// csvデータの1行を','で複数の文字列に分割する
		std::vector<std::string> strvec = Split(m_line, ',');

		// strvec[0]	: 出現番号				int
		// strvec[1]	: アイテムの種類		string
		// strvec[2]	: アイテムのX座標		float
		// strvec[3]	: アイテムのY座標		float
		// strvec[4]	: アイテムのZ座標		float
		// strvec[5]	: 移動方向ベクトルX		float
		// strvec[6]	: 移動方向ベクトルY		float
		// strvec[7]	: 移動方向ベクトルZ		float
		// strvec[8]	: 反転するまでの時間	int
		// strvec[9]	: 移動速度				float


		// 読み取ったデータを座標に変換する
		VECTOR pos = VGet(stof(strvec[2]), stof(strvec[3]), stof(strvec[4]));
		VECTOR direction = VGet(stof(strvec[5]), stof(strvec[6]), stof(strvec[7]));
		int turnTime = static_cast<int>(stof(strvec[8]));
		float speed = stof(strvec[9]);

		// 高さ調整
		pos.y += kHeightPos;

		// データに合う敵の追加
		Add(stoi(strvec[0]), strvec[1], pos, direction, turnTime, speed);
	}
}

void SpownEnemy::Appearance()
{
	for (auto& data : m_pData)
	{
		m_pObjectManager->AddObject(data->pEnemy);
		data->pEnemy->Init();
	}
}

void SpownEnemy::Add(int createNum, std::string enemyName, VECTOR pos, VECTOR direction, int turnTime, float speed)
{
	std::string enemyModelFile = "Data/Model/Enemy/" + enemyName + ".mv1";
	// モデルファイル名
	const char* const modelFile = enemyModelFile.c_str();
	// モデルハンドルの取得
	int modelH = MV1LoadModel(modelFile);
	assert(modelH != -1);
	// itemNameに合う敵の追加
	if (enemyName == "Bee")
	{
		m_pData.push_back(new Data);
		m_pData.back()->pEnemy = new EnemyBee(pos, direction, turnTime, speed);
	}
	else if (enemyName == "Skull")
	{
		m_pData.push_back(new Data);
		m_pData.back()->pEnemy = new EnemySkull(pos, speed);
	}
	else
	{
		return;
	}

	m_pData.back()->pEnemy->SetModelHandle(modelH);
	m_pData.back()->index = createNum;
	m_pData.back()->name = enemyName;
}

std::vector<std::string> SpownEnemy::Split(std::string& input, char delimiter)
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
