#include "stdafx.h"
#include "UI_Manager.h"
#include "Player.h"



void UI_Manager::AddImgAndInitObj()
{
	loadingDone = false;
	loadingAlpha = 255;
	youDiedMessageAlpha = 0;
	IMAGEMANAGER->addImage("Loading", "Image/Loading/Loading.bmp", 1280, 720, false, RGB(0, 0, 0)); // 로딩 이미지
	IMAGEMANAGER->addImage("YouDied", "Image/InGame/YouDied.bmp", 1280, 720, false, RGB(0, 0, 0)); // 플레이어 죽을때 이미지
}

HRESULT UI_Manager::init()
{
	AddImgAndInitObj();
	return S_OK;
}

void UI_Manager::release()
{
}

void UI_Manager::update()
{
	if (loadingDone)
	{
		if (loadingAlpha > 0)
		{
			loadingAlpha -= 5;
			if (loadingAlpha > 0 && loadingAlpha <= 5)
			{
				int randSelectBGM = RND->getInt(3);
				if (randSelectBGM == 0)
				{
					SOUNDMANAGER->play("A01_A");
				}
				if (randSelectBGM == 1)
				{
					SOUNDMANAGER->play("A01_B");
				}
				if (randSelectBGM == 2)
				{
					SOUNDMANAGER->play("A01_C");
				}
			}
		}
	}
	if (_player->Get_PlayerIsDead())
	{
		if (youDiedMessageAlpha < 255)
		{
			youDiedMessageAlpha += 5;
		}
	}
}

void UI_Manager::render()
{
	IMAGEMANAGER->findImage("Loading")->alphaRender(getMemDC(), loadingAlpha);
	if (loadingAlpha <= 0)
	{
		if (!_player->Get_PlayerIsDead())
		{
			IMAGEMANAGER->findImage("Player_HUD")->setFrameY(0);
			IMAGEMANAGER->findImage("Player_HUD")->frameRender(getMemDC(), 0, 0);
			IMAGEMANAGER->findImage("Player_Life")->setFrameX(_player->Get_PlayerHP());
			IMAGEMANAGER->findImage("Player_Life")->frameRender(getMemDC(), 30, 25);
		}
		else
		{
			IMAGEMANAGER->findImage("Player_HUD")->setFrameY(1);
			IMAGEMANAGER->findImage("Player_HUD")->frameRender(getMemDC(), 0, 0);
			IMAGEMANAGER->findImage("YouDied")->alphaRender(getMemDC(), youDiedMessageAlpha);
		}
	}
}