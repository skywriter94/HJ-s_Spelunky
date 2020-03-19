#pragma once
#include "gameNode.h"
#include "Snake.h"
#include "Viper.h"
#include "Jjangshi.h"
#include <vector>

#define PI 3.14f
#define PI2 PI*2

class MapGenerator;
class Player;



class EnemyManager : public gameNode
{
public:
	EnemyManager();
	~EnemyManager();
	
	HRESULT init();
	void release();
	void update();
	void render();

	void RemoveEnemy(int ID);

	void SetSnake();
	void SetViper();
	void SetJjangshi();

private:
	typedef vector<Enemy*>			 vEnemy;
	typedef vector<Enemy*>::iterator viEnemy;

	vEnemy _vEnemy;
	viEnemy _viEnemy;
	MapGenerator* _mg;
	Player* _player;
	effect* _enemyDeathEffect;

private:
	struct EnemyDeathEffectInfo
	{
		POINT pos;
		float lifeTime;
	};
	vector<EnemyDeathEffectInfo> vEnemyDeathEffectInfo;

private:
	void AddEnemyImg();
	void Collision();

public:
	vector<Enemy*> getV_Enemy() { return _vEnemy; }
	vector<Enemy*>::iterator getVi_Enemy() { return _viEnemy; }

	void SetMapGeneratorAddressLink(MapGenerator* mg) { _mg = mg; }
	void SetPlayerAddressLink(Player* player) { _player = player; }
};