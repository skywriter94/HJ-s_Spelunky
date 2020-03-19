#pragma once
#include "gameNode.h"
#include "MapGenerator.h"
#include "Sounds.h"
#include "Player.h"
#include "EnemyManager.h"
#include "UI_Manager.h"

class _2_Ingame : public gameNode
{
private:
	MapGenerator* _mapGenerator;
	Sounds* _sounds;
	Player* _player;
	EnemyManager* _em;
	UI_Manager* _ui;
	bool setPlayerPos;

public:
	_2_Ingame();
	~_2_Ingame();

	HRESULT init();
	void release();
	void update();
	void render();
};