#pragma once
#include "gameNode.h"
#include "_1_MainMenu.h"
#include "_2_Ingame.h"

class GameManager : public gameNode
{
private:
	void init_AddScene();
	
public:
	HRESULT init();
	void release();
	void update();
	void render();

	GameManager();
	~GameManager();

public:
};