#include "stdafx.h"
#include "_2_Ingame.h"



_2_Ingame::_2_Ingame()
{
}

_2_Ingame::~_2_Ingame()
{
}



HRESULT _2_Ingame::init()
{
	_mapGenerator = new MapGenerator;
	_mapGenerator->init();
	_sounds = new Sounds;
	_sounds->init();
	_player = new Player;
	_player->init(0, 0, 0, 0, 4);
	_em = new EnemyManager;
	_em->init();
	_ui = new UI_Manager;
	_ui->init();

	_mapGenerator->SetPlayerAddressLink(_player);
	_mapGenerator->SetEnemyManagerAddressLink(_em);
	_player->SetMapGeneratorAddressLink(_mapGenerator);
	_player->SetEnemyManagerAddressLink(_em);
	_em->SetMapGeneratorAddressLink(_mapGenerator);
	_em->SetPlayerAddressLink(_player);
	_ui->SetPlayerAddressLink(_player);
	PHYSICS->SetMapGeneratorAddressLink(_mapGenerator);

	setPlayerPos = false;
	SOUNDMANAGER->play("Lobby_jingle");
	return S_OK;
}

void _2_Ingame::release()
{
	SAFE_DELETE(_mapGenerator);
	SAFE_DELETE(_sounds);
	SAFE_DELETE(_player);
	SAFE_DELETE(_em);
	SAFE_DELETE(_ui);
}

void _2_Ingame::update()
{
	_mapGenerator->update();
	_em->update();
	_ui->update();

	if (_player->Get_PlayerLevelClear())
	{
		init();
	}

	if ((int)_mapGenerator->Get_RandomMapGeneSequence() == 7)
	{
		_ui->Set_LoadingImgSwitch(true);
		setPlayerPos = true;
		_em->SetSnake();
		_em->SetViper();
		_em->SetJjangshi();
	}
	if (setPlayerPos)
	{
		_player->Set_PlayerPos(_mapGenerator->Get_Tile_EntranceTilePos().x + TILESIZE + TILESIZE / 2, _mapGenerator->Get_Tile_EntranceTilePos().y + TILESIZE + TILESIZE / 2);
		_player->Set_PlayerRect(_mapGenerator->Get_Tile_EntranceTilePos().x + TILESIZE + TILESIZE / 2, _mapGenerator->Get_Tile_EntranceTilePos().y + TILESIZE + TILESIZE / 2, 64);
		CAMERAMANAGER->Set_PlayerCenterPos({ _mapGenerator->Get_Tile_EntranceTilePos().x + TILESIZE + TILESIZE / 2, _mapGenerator->Get_Tile_EntranceTilePos().y + TILESIZE + TILESIZE / 2 });
		setPlayerPos = false;
	}

	if ((int)_mapGenerator->Get_RandomMapGeneSequence() == 8)
	{
		_player->update();
	}
}

void _2_Ingame::render()
{
	_mapGenerator->render();
	_em->render();
	_player->render();
	_ui->render();
}