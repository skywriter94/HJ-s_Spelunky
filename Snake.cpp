#include "stdafx.h"
#include "Snake.h"



Snake::Snake()
{
	init();
}

Snake::~Snake()
{
}

HRESULT Snake::init()
{
	moveTime = RND->getFromFloatTo(4.0f, 10.0f);
	idleTime = RND->getFromFloatTo(2.0f, 4.5f);
	stateTemp = (int)getState();
	dirTemp = getDir();
	return S_OK;
}

void Snake::update()
{
	if ((int)getState() != stateTemp || getDir() != dirTemp)
	{
		if ((int)getState() == 0)
		{
			setImage("Snake_Idle");
			if (getDir() == 0)
			{
				setAni("snake_Idle_Left");
			}
			else
			{
				setAni("snake_Idle_Right");
			}
		}
		else
		{
			setImage("Snake_Move");
			if (getDir() == 0)
			{
				setAni("snake_Move_Left");
			}
			else
			{
				setAni("snake_Move_Right");
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



void Snake::update_Idle()
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

void Snake::update_Move()
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

void Snake::update_Attack()
{
}

void Snake::update_Hit()
{
}

void Snake::update_Dead()
{
}