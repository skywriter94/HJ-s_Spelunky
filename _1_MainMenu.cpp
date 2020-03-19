#include "stdafx.h"
#include "_1_MainMenu.h"



_1_MainMenu::_1_MainMenu()
{
}

_1_MainMenu::~_1_MainMenu()
{
}



HRESULT _1_MainMenu::init()
{
	sounds = new Sounds;
	sounds->init();

	isIngame = isEnterIngame = false;

	bgRgb = 255;
	menuRGB = 0;
	recentID = 0;

	init_AddImg();
	SOUNDMANAGER->play("Title");
	return S_OK;
}

void _1_MainMenu::release()
{
	SAFE_DELETE(sounds);
}

void _1_MainMenu::update()
{
	if (!isIngame)
	{
		if (!isEnterIngame)
		{
			if (bgRgb > 0)
			{
				--bgRgb;
			}
			if (bgRgb <= 0 && menuRGB < 255)
			{
				menuRGB += 5;
			}

			if (menuRGB >= 255)
			{
				if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
				{
					SOUNDMANAGER->play("sfx_ui_select_down");
					if (recentID < 1) ++recentID;
					else recentID = 0;
				}
				if (KEYMANAGER->isOnceKeyDown(VK_UP))
				{
					SOUNDMANAGER->play("sfx_ui_select_up");
					if (recentID > 0) --recentID;
					else recentID = 1;
				}

				if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
				{
					SOUNDMANAGER->play("sfx_ui_start");
					switch (recentID)
					{
						case 0:
							isEnterIngame = true;
							break;
						case 1:
							PostQuitMessage(0);
							break;
						default:
							break;
					}
				}
			}
		}
		else
		{
			if (bgRgb < 255)
			{
				bgRgb += 5;
			}
			else
			{
				SOUNDMANAGER->stop("Title");
				isIngame = true;
			}
		}
	}
	else
	{
		SCENEMANAGER->changeScene("2_Ingame");
	}
}

void _1_MainMenu::render()
{
	if (!isIngame)
	{
		IMAGEMANAGER->render("TitleBG", getMemDC(), 0, 0);
		IMAGEMANAGER->findImage("Title")->alphaRender(getMemDC(), 0, 0, menuRGB);
		IMAGEMANAGER->findImage("MenuFrame")->alphaRender(getMemDC(), 20, (138.5f + 70) / 2.0f, menuRGB);
		IMAGEMANAGER->findImage("MenuFrame")->alphaRender(getMemDC(), 20, ((138.5f * 2) + 90) / 2.0f, menuRGB);
		if (menuRGB >= 255)
		{
			if (recentID == 0) IMAGEMANAGER->render("Text_GameStart", getMemDC(), 62.5f, (138.5f + 70 + 50) / 2.0f);
			else IMAGEMANAGER->render("Text_Selected_GameStart", getMemDC(), 62.5f, (138.5f + 70 + 50) / 2.0f);

			if (recentID == 1) IMAGEMANAGER->render("Text_Exit", getMemDC(), 120, ((138.5f * 2) + 90 + 50) / 2.0f);
			else IMAGEMANAGER->render("Text_Selected_Exit", getMemDC(), 120, ((138.5f * 2) + 90 + 50) / 2.0f);
		}
		IMAGEMANAGER->findImage("TitleBG_FadeOut")->alphaRender(getMemDC(), bgRgb);
	}
}



void _1_MainMenu::init_AddImg()
{
	IMAGEMANAGER->addImage("TitleBG_FadeOut", "Image/Title/TitleBG_FadeOut.bmp", WINSIZEX, WINSIZEY, false, RGB(255, 255, 255));
	IMAGEMANAGER->addImage("TitleBG", "Image/Title/TitleBG.bmp", WINSIZEX, WINSIZEY, false, RGB(255, 255, 255));
	IMAGEMANAGER->addImage("Title", "Image/Title/Title.bmp", 334, 93, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("MenuFrame", "Image/Title/MenuFrame.bmp", 277, 78, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Text_GameStart", "Image/Title/Text_GameStart.bmp", 190, 24, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addImage("Text_Option", "Image/Title/Text_Option.bmp", 114, 23, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Text_Exit", "Image/Title/Text_Exit.bmp", 70, 22, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Text_Selected_GameStart", "Image/Title/Text_Selected_GameStart.bmp", 190, 24, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addImage("Text_Selected_Option", "Image/Title/Text_Selected_Option.bmp", 114, 23, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Text_Selected_Exit", "Image/Title/Text_Selected_Exit.bmp", 70, 22, true, RGB(255, 0, 255));
}