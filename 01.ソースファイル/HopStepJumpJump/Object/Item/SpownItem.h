#pragma once
#include "DxLib.h"

#include <list>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

class ObjectBase;
class ObjectManager;

class SpownItem
{
public:
	SpownItem(const char* fileName, ObjectManager* pObjectManager);
	virtual ~SpownItem();

	void Init();

	void Spown();

	/// <summary>
	/// 存在しているかどうか
	/// </summary>
	/// <returns></returns>
	bool IsExist();

	/// <summary>
	/// アイテム数の取得
	/// </summary>
	/// <returns></returns>
	int GetItemNum()const { return static_cast<int>(m_pData.size()); }

private:	// 構造体
	struct Data
	{
		ObjectBase* pItem = nullptr;
		int index = 0;
		std::string name{};
	};

private:	// 関数

	/// <summary>
	/// アイテムの生成
	/// </summary>
	void Create();

	/// <summary>
	/// アイテムの出現
	/// </summary>
	void Appearance();

	/// <summary>
	/// アイテムの追加
	/// </summary>
	/// <param name="createNum"></param>
	/// <param name="fileName"></param>
	/// <param name="pos"></param>
	void Add(int createNum, std::string fileName, VECTOR pos);

	/// <summary>
	/// 特定の文字が検出されるまで文字をプッシュバックする
	/// </summary>
	/// <param name="input"></param>
	/// <param name="delimiter"></param>
	/// <returns></returns>
	std::vector<std::string> Split(std::string& input, char delimiter);

private:
	int m_createNum;					// 出現番号
	const char* m_fileName;				// ファイル名
	std::list<Data*> m_pData;			// アイテム情報
	std::string m_line;					// ファイル内の文字列
	ObjectManager* m_pObjectManager;	// オブジェクトマネージャーポインタ
};

