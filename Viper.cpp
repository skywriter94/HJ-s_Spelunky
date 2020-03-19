#include "stdafx.h"
#include "Viper.h"

Viper::Viper()
{
}

Viper::~Viper()
{
}

HRESULT Viper::init()
{
	moveTime = RND->getFromFloatTo(4.0f, 10.0f);
	idleTime = RND->getFromFloatTo(2.0f, 4.5f);
	attackDelay = 1.5f;
	stateTemp = (int)getState();
	dirTemp = getDir();
	return S_OK;
}

void Viper::update()
{
	if ((int)getState() != stateTemp || getDir() != dirTemp)
	{
		if ((int)getState() == 0)
		{
			setImage("Viper_Idle");
			if (!getDir())
			{
				setAni("viper_Idle_Left");
			}
			else
			{
				setAni("viper_Idle_Right");
			}
		}
		else if ((int)getState() == 1)
		{
			setImage("Viper_Move");
			if (!getDir())
			{
				setAni("viper_Move_Left");
			}
			else
			{
				setAni("viper_Move_Right");
			}
		}
		else if ((int)getState() == 2)
		{
			setImage("Viper_Attack");
			if (!getDir())
			{
				setAni("viper_Attack_Left");
			}
			else
			{
				setAni("viper_Attack_Right");
			}
		}
		stateTemp = (int)getState();
		dirTemp = getDir();
	}

	switch (getState())
	{
		case _Idle:
			update_Idle();
			break;
		case _Move:
			update_Move();
			break;
		case _Attack:
			update_Attack();
			break;
		case _Hit:
			update_Hit();
			break;
		case _Dead:
			update_Dead();
			break;
	}
	rc = RectMakeCenter(pos.x, pos.y, img->getFrameWidth(), img->getFrameHeight());
}

void Viper::update_Idle()
{
	if (idleTime > 0)
	{
		idleTime -= TIMEMANAGER->getElapsedTime();
	}
	else
	{
		dir = RND->getInt(2);
		idleTime = RND->getFromFloatTo(2.0f, 4.5f);
		setState(1);
	}
}

void Viper::update_Move()
{
	if (moveTime > 0)
	{
		moveTime -= TIMEMANAGER->getElapsedTime();
		if (!dir)
		{
			pos.x -= speed;
		}
		else
		{
			pos.x += speed;
		}
	}
	else
	{
		moveTime = RND->getFromFloatTo(4.0f, 10.0f);
		setState(0);
	}
}

void Viper::update_Attack()
{

}

void Viper::update_Hit()
{

}

void Viper::update_Dead()
{

}