#include "stdafx.h"
#include "Sounds.h"



HRESULT Sounds::init()
{
	init_AddSound();
	return S_OK;
}

void Sounds::release()
{
}

void Sounds::update()
{

}



void Sounds::init_AddSound()
{
	// 타이틀
	SOUNDMANAGER->addSound("Title", "sound/title/Title.ogg", true, true);

	// 인게임
	SOUNDMANAGER->addSound("Lobby_jingle", "sound/loading/Lobby_jingle.ogg", false, false); // 로딩
	SOUNDMANAGER->addSound("A01_A", "sound/ingame/A01_A.ogg", true, true);
	SOUNDMANAGER->addSound("A01_B", "sound/ingame/A01_B.ogg", true, true);
	SOUNDMANAGER->addSound("A01_C", "sound/ingame/A01_C.ogg", true, true);

	// 플레이어
	SOUNDMANAGER->addSound("YouDied", "sound/ingame/YouDied.mp3", false, false); // 죽을때
	SOUNDMANAGER->addSound("Player_GiveDmg", "sound/player/Player_GiveDmg.wav", false, false); // 플레이어가 데미지를 줄 때
	SOUNDMANAGER->addSound("Player_IntoDoor", "sound/player/Player_IntoDoor.wav", false, false); // 플레이어가 출구로 입장할 때
	SOUNDMANAGER->addSound("Player_Jump", "sound/player/Player_Jump.wav", false, false); // 플레이어가 점프할 때
	SOUNDMANAGER->addSound("Player_TakeDmg", "sound/player/Player_TakeDmg.wav", false, false); // 플레이어가 데미지를 입을 때
	SOUNDMANAGER->addSound("Player_Bounce", "sound/player/Player_Bounce.wav", false, false); // 플레이어가 착지할 때
	SOUNDMANAGER->addSound("Player_Bomb_Kaboom", "sound/player/Player_Bomb_Kaboom.wav", false, false);
	SOUNDMANAGER->addSound("Player_Bomb_Kaboombass", "sound/player/Player_Bomb_Kaboombass.wav", false, false);
	SOUNDMANAGER->addSound("Player_DraggingWall", "sound/player/Player_DraggingWall.wav", false, true);

	// 적
	SOUNDMANAGER->addSound("Jjangshi_Dead", "sound/enemy/Jjangshi_Dead.wav", false, false);
	SOUNDMANAGER->addSound("Snake_Death", "sound/enemy/Snake_Death.wav", false, false);

	// SFX
	SOUNDMANAGER->addSound("sfx_ui_back", "sound/sfx/sfx_ui_back.ogg", false, false);
	SOUNDMANAGER->addSound("sfx_ui_select_down", "sound/sfx/sfx_ui_select_down.ogg", false, false);
	SOUNDMANAGER->addSound("sfx_ui_select_up", "sound/sfx/sfx_ui_select_up.ogg", false, false);
	SOUNDMANAGER->addSound("sfx_ui_start", "sound/sfx/sfx_ui_start.ogg", false, false);

	// etc
	SOUNDMANAGER->addSound("Fragment1", "sound/etc/Fragment1.wav", false, false);
	SOUNDMANAGER->addSound("Fragment2", "sound/etc/Fragment2.wav", false, false);
}