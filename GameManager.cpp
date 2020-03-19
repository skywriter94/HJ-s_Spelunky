#include "stdafx.h"
#include "GameManager.h"



GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}



HRESULT GameManager::init()
{
	gameNode::init(true);

	init_AddScene();	// Scene Ãß°¡
	SCENEMANAGER->changeScene("1_MainMenu");

	return S_OK;
}

void GameManager::release()
{
	gameNode::release();
	SCENEMANAGER->releaseSingleton();
}

void GameManager::update()
{
	gameNode::update();
	SCENEMANAGER->update();
}

void GameManager::render()
{
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//===========================================================
	SCENEMANAGER->render();
	//===========================================================
	_backBuffer->render(getHDC(), 0, 0);
}



void GameManager::init_AddScene()
{
	SCENEMANAGER->addScene("1_MainMenu", new _1_MainMenu);
	SCENEMANAGER->addScene("2_Ingame", new _2_Ingame);
}