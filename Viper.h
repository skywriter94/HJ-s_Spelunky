#pragma once
#include "Enemy.h"

class Viper : public Enemy
{
private:
	float moveTime;
	float idleTime;
	float attackDelay;
	int stateTemp;
	bool dirTemp;

public:
	Viper();
	~Viper();

	HRESULT init();
	virtual void update() override;
	virtual void update_Idle() override;
	virtual void update_Move() override;
	virtual void update_Attack() override;
	virtual void update_Hit() override;
	virtual void update_Dead() override;
};