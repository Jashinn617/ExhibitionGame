#pragma once
#include "../Util/Game.h"

#include <memory>
#include <list>
#include <functional>

class EnemyBase;
class ItemBase;
class ObjectBase;
class Player;
class Field;
class Camera;
class EnemyManager;
class ItemManager;
class Collision;
class SkyDome;
class Time;
class ToonShader;
class ShadowMapShader;

class ObjectManager
{
public:
	ObjectManager(Game::Stage stage);
	virtual ~ObjectManager();

	void Update();

	void Draw();

	/// <summary>
	/// オブジェクトの追加
	/// </summary>
	/// <typeparam name="T">ObjectBaseを継承し型名</typeparam>
	/// <param name="pObj">何のオブジェクトを追加するか</param>
	template <class T>
	void AddObject(T* pObj)
	{
		m_pObject.push_back(pObj);
		m_pObject.back()->SetMain(this);
	}

	/// <summary>
	/// プレイヤーが存在するかどうか
	/// </summary>
	/// <returns></returns>
	bool IsPlayerExist();

	/// <summary>
	/// 敵が存在するかどうか
	/// </summary>
	/// <returns></returns>
	bool IsEnemyExist();

	/// <summary>
	/// アイテムが存在するかどうか
	/// </summary>
	/// <returns></returns>
	bool IsItemExist();

	/// <summary>
	/// 敵の数の取得
	/// </summary>
	/// <returns></returns>
	int GetItemNum()const;

	/// <summary>
	/// ゲームをクリアしたかどうか
	/// </summary>
	/// <returns></returns>
	bool IsGameClear()const { return m_isGameClear; }

	bool IsGameEnd()const { return m_isGameEnd; }

	/// <summary>
	/// ゲームが終了したかどうか
	/// </summary>
	/// <returns></returns>
	bool IsObjGameEnd();

	/// <summary>
	/// プレイヤーポインタの取得
	/// </summary>
	/// <returns></returns>
	Player* const GetPlayer();

	/// <summary>
	/// エネミーマネージャーポインタの取得
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<EnemyManager> const GetEnemyManager() { return m_pEnemyManager; }

	/// <summary>
	/// 強制的にゲームクリアにする
	/// </summary>
	void SetGameClear();



private:	// 関数
	/// <summary>
	/// ステージクリア時の更新処理
	/// </summary>
	void GameClearUpdate();

	/// <summary>
	/// ゲーム終了時の更新処理
	/// </summary>
	void GameEndUpdate();

private:	// 変数
	int m_H;
	bool m_isGameClear;		// ゲームをクリアしたか
	bool m_isGameEnd;		// ゲームが終わったかどうか
	bool m_isGoal;			// ゴールしたかどうか
	bool m_isTutorial;		// チュートリアルかどうか

	std::shared_ptr<Collision> m_pCollision;	// 当たり判定
	std::shared_ptr<SkyDome> m_pSkyDome;		// スカイドーム
	std::shared_ptr<EnemyManager> m_pEnemyManager;	// エネミーマネージャー
	std::shared_ptr<ItemManager> m_pItemManager;	// アイテムマネージャー
	std::shared_ptr<ToonShader> m_pToonShader;		// トゥーンシェーダ
	std::shared_ptr<ShadowMapShader> m_pShadowMapShader;	// シャドウマップ
	std::list<ObjectBase*> m_pObject;		// オブジェクト
	std::shared_ptr<Time> m_gameEndTime;	// ゲーム終了してからの時間

};