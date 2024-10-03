#pragma once
#include "../../Util/Game.h"

#include <vector>
#include <memory>
#include <array>

class SpownEnemy;
class ObjectManager;

class EnemyManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stage">ステージの種類</param>
	/// <param name="pObjectManager">オブジェクトマネージャー</param>
	EnemyManager(Game::Stage stage, ObjectManager* pObjectManager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	ObjectManager* m_pObjectManager;		// オブジェクトマネージャーポインタ
	std::shared_ptr<SpownEnemy> m_pSpown;	// スポーンエネミーポインタ
};
