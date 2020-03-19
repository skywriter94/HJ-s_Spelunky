#include "stdafx.h"
#include "Jjangshi.h"



Jjangshi::Jjangshi()
{
}

Jjangshi::~Jjangshi()
{
}



HRESULT Jjangshi::init()
{
	jumpTime = 1.0f;
	return S_OK;
}

void Jjangshi::update()
{
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

void Jjangshi::update_Idle()
{
}

void Jjangshi::update_Move()
{
	if ((int)posState == 1)
	{
		if (jumpTime > 0)
		{
			jumpTime -= TIMEMANAGER->getElapsedTime();
			ani->stop();
		}
		else
		{
			jumpPower = JUMPPOWER;
			speed = RND->getFromFloatTo(5.0f, 10.0f);
			dir = RND->getInt(2);
			posState = (EnemyPosState)0;
			if (!dir)
			{
				setAni("jjangshi_Move_Left");
			}
			else
			{
				setAni("jjangshi_Move_Right");
			}
			ani->start();
		}
	}
	else
	{
		jumpTime = RND->getFromFloatTo(0.5f, 1.0f);
		if (!dir)
		{
			pos.x -= speed;
		}
		else
		{
			pos.x += speed;
		}

		if (speed > 0)
		{
			speed -= SPEED_R;
		}
	}
}

void Jjangshi::update_Attack()
{
}

void Jjangshi::update_Hit()
{
}

void Jjangshi::update_Dead()
{
}