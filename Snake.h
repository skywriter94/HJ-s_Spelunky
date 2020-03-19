#pragma once
#include "Enemy.h"

class Snake : public Enemy
{
private:
	float moveTime;
	float idleTime;
	int stateTemp;
	bool dirTemp;

private:

public:
	Snake();
	~Snake();

	HRESULT init();
	virtual void update() override;
	virtual void update_Idle() override;
	virtual void update_Move() override;
	virtual void update_Attack() override;
	virtual void update_Hit() override;
	virtual void update_Dead() override;
};