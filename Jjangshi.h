#pragma once
#include "Enemy.h"

class Jjangshi : public Enemy
{
private:
	float jumpTime;

public:
	Jjangshi();
	~Jjangshi();

	HRESULT init();
	virtual void update() override;
	virtual void update_Idle() override;
	virtual void update_Move() override;
	virtual void update_Attack() override;
	virtual void update_Hit() override;
	virtual void update_Dead() override;
};