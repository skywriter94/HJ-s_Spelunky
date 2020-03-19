#include "stdafx.h"
#include "Bomb.h"



HRESULT Bomb::init(POINTFLOAT _pos, bool _dir, float _speed, float _timer)
{
	ani = new animation;
	img = IMAGEMANAGER->findImage("Bomb_Ani1");
	ani = KEYANIMANAGER->findAnimation("bomb_Ani1");

	posState = Air;
	pos = _pos;
	posID = { (int)pos.x / 64, (int)pos.y / 64 };
	rc = RectMakeCenter(pos.x, pos.y, img->getFrameWidth(), img->getFrameHeight());
	dir = _dir;
	gravity = 0;
	throwHeightPower = THROWHEIGHTPOWER;
	speed = _speed;
	
	if (!dir) angle = PI;
	else angle = PI2;

	timer = _timer;
	isThrow = true;
	ani->start();
	bombArea = RectMakeCenter((rc.right + rc.left) / 2, (rc.bottom + rc.top) / 2, 
		IMAGEMANAGER->findImage("Explosion")->getFrameWidth(), IMAGEMANAGER->findImage("Explosion")->getFrameHeight());
	return S_OK;
}

void Bomb::release()
{
}

void Bomb::update()
{
	if (isThrow)
	{
		posID = { (int)pos.x / 64, (int)pos.y / 64 };
		rc = RectMakeCenter(pos.x, pos.y, img->getFrameWidth(), img->getFrameHeight());
		bombArea = RectMakeCenter((rc.right + rc.left) / 2, (rc.bottom + rc.top) / 2,
			IMAGEMANAGER->findImage("Explosion")->getFrameWidth(), IMAGEMANAGER->findImage("Explosion")->getFrameHeight());

		PHYSICS->ObjTileCollision((int&)posState, pos, posID, rc, img, dir, gravity, throwHeightPower, speed, angle);
		timer -= TIMEMANAGER->getElapsedTime();
		if (timer < 0.5f)
		{
			ani->stop();
			img = IMAGEMANAGER->findImage("Bomb_Ani2");
			ani = KEYANIMANAGER->findAnimation("bomb_Ani2");
			ani->start();
		}
		ani->frameUpdate(TIMEMANAGER->getElapsedTime());
	}
}

void Bomb::render()
{
	if (isThrow)
	{
		//Rectangle(getMemDC(), bombArea.left - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2), bombArea.top - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2),
		//	bombArea.right - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2), bombArea.bottom - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2));
		//Rectangle(getMemDC(), rc.left - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2), rc.top - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2),
		//	rc.right - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2), rc.bottom - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2));
		img->aniRender(getMemDC(), rc.left - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2), rc.bottom - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2) - img->getFrameHeight(), ani);
	}
}