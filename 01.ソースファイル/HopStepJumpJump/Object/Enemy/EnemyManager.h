#pragma once
#include "../../Util/Game.h"

#include <vector>
#include <memory>
#include <array>

class EnemyBase;
class SpownEnemy;
class ObjectBase;
class Player;
class ObjectManager;
class Time;

class EnemyManager
{
public:
	EnemyManager(Game::Stage stage, ObjectManager* pObjectManager);
	~EnemyManager();

	void Init();
	void Update();
	void Draw();



private:
	bool m_isSpown;
	bool m_isClear;
	ObjectManager* m_pObjectManager;	
	std::shared_ptr<SpownEnemy> m_pSpown;
};
