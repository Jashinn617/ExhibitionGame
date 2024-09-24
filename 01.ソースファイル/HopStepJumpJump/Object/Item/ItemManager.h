#pragma once
#include "DxLib.h"
#include "../../Util/Game.h"

#include <vector>
#include <memory>
#include <array>

class ItemBase;
class SpownItem;
class ObjectBase;
class Player;
class ObjectManager;
class CountGetItem;
class Time;

class ItemManager
{
public:
	ItemManager(Game::Stage stage, ObjectManager* pObjectManager);

	~ItemManager();

	void Init();
	void Update();
	void Draw();

	/// <summary>
	/// アイテムの取得数の追加
	/// </summary>
	void AddGetNum();

	// アイテムの取得数用のポインタの取得
	const std::shared_ptr<CountGetItem> GetCountItem()const { return m_pCountGetItem; }

	/// <summary>
	/// クリア状態かどうか
	/// </summary>
	/// <returns></returns>
	bool IsClear()const { return m_isClear; }

private:
	bool m_isSpown;		// アイテムを出現させるかどうか
	bool m_isClear;		// クリアしたかどうか
	ObjectManager* m_pObjectManager;	// オブジェクトマネージャーのポインタ
	std::shared_ptr<CountGetItem> m_pCountGetItem;	// アイテムの取得数
	std::shared_ptr<SpownItem> m_pSpown;	// アイテムの出現

};

