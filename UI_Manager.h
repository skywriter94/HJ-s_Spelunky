#pragma once
#include "gameNode.h"

class Player;



class UI_Manager : public gameNode
{
private:
	Player* _player;
	bool loadingDone;
	int loadingAlpha;
	int youDiedMessageAlpha;

private:
	void AddImgAndInitObj();

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void Set_LoadingImgSwitch(bool _switch) { loadingDone = _switch; }
	bool Get_LoadingImgSwitch() { return loadingDone; }
	void SetPlayerAddressLink(Player* player) { _player = player; }
};