#pragma once
#include "gameNode.h"



class Sounds : public gameNode
{
private:
	void init_AddSound();
public:
	HRESULT init();
	void release();
	void update();
};