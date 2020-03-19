#pragma once
#include "gameNode.h"
#include "Sounds.h"

class _1_MainMenu : public gameNode
{
private:
	Sounds* sounds;
	bool isIngame; // æ¿¿ª πŸ≤„¡÷¥¬ ¡∂∞«
	bool isEnterIngame;
	int bgRgb;
	int menuRGB;
	int recentID;

private:
	void init_AddImg();

public:
	_1_MainMenu();
	~_1_MainMenu();

	HRESULT init();
	void release();
	void update();
	void render();

	void Set_isGameOver(bool ONOFF) { isIngame = ONOFF; }
	bool Get_isGameOVer() { return isIngame; }
};