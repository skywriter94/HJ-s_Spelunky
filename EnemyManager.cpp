#include "stdafx.h"
#include "EnemyManager.h"
#include "MapGenerator.h"
#include "Player.h"


EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

HRESULT EnemyManager::init()
{
	_mg = new MapGenerator;
	_player = new Player;
	_vEnemy.clear();
	AddEnemyImg();
	IMAGEMANAGER->addImage("MonsterDead", "Image/Monster/MonsterDead.bmp", 850, 150, true, RGB(255, 0, 255));
	EFFECTMANAGER->addEffect("MonsterDeath", "Image/Monster/MonsterDead.bmp", 850, 150, 170, 150, 10, TIMEMANAGER->getElapsedTime(), 7);
	return S_OK;
}

void EnemyManager::release()
{
	SAFE_DELETE(_mg);
	SAFE_DELETE(_player);
}

void EnemyManager::update()
{
	Collision();
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->setPosID({ (int)_vEnemy[i]->getPos().x / 64, (int)_vEnemy[i]->getPos().y / 64 }); // 몹들의 타일 번호를 실시간으로 받아온다. 

		if ((int)_vEnemy[i]->getKind() == 0) // 뱀
		{
			if ((int)_vEnemy[i]->getState() == 1)
			{
				//if (_mg->Get_Tile(_vEnemy[i]->getPosID().y + 1, _vEnemy[i]->getPosID().x)->outsideTiling.L ||
				//	_mg->Get_Tile(_vEnemy[i]->getPosID().y + 1, _vEnemy[i]->getPosID().x)->outsideTiling.R ||
				//	!_mg->Get_Tile(_vEnemy[i]->getPosID().y, _vEnemy[i]->getPosID().x + 1)->bgTile ||
				//	!_mg->Get_Tile(_vEnemy[i]->getPosID().y, _vEnemy[i]->getPosID().x - 1)->bgTile)
				//{
				//	_vEnemy[i]->setDir(!_vEnemy[i]->getDir());
				//}
			}
		}

		if ((int)_vEnemy[i]->getKind() == 1) // 독사
		{
			if ((int)_vEnemy[i]->getState() == 1)
			{
				//if (_mg->Get_Tile(_vEnemy[i]->getPosID().y + 1, _vEnemy[i]->getPosID().x)->outsideTiling.L ||
				//	_mg->Get_Tile(_vEnemy[i]->getPosID().y + 1, _vEnemy[i]->getPosID().x)->outsideTiling.R ||
				//	!_mg->Get_Tile(_vEnemy[i]->getPosID().y, _vEnemy[i]->getPosID().x + 1)->bgTile ||
				//	!_mg->Get_Tile(_vEnemy[i]->getPosID().y, _vEnemy[i]->getPosID().x - 1)->bgTile)
				//{
				//	_vEnemy[i]->setDir(!_vEnemy[i]->getDir());
				//}
			}
		}

		if ((int)_vEnemy[i]->getKind() == 2) // 강시
		{

		}
		_vEnemy[i]->getAni()->frameUpdate(TIMEMANAGER->getElapsedTime());
		_vEnemy[i]->update();
	}
}

void EnemyManager::render()
{
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		RECT tempRC;
		if (IntersectRect(&tempRC, &CAMERAMANAGER->get_renderArea(), &_vEnemy[i]->getRect())) // 카메라 영역안에 들은 에너미만 화면에서 출력
		{
			//Rectangle(getMemDC(), 
			//	_vEnemy[i]->getRect().left - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2),
			//	_vEnemy[i]->getRect().top - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2),
			//	_vEnemy[i]->getRect().right - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2),
			//	_vEnemy[i]->getRect().bottom - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2));
			_vEnemy[i]->getImage()->aniRender(getMemDC(), _vEnemy[i]->getRect().left - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2),
				_vEnemy[i]->getRect().top - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2), _vEnemy[i]->getAni());
		}
	}

	for (int i = 0; i < vEnemyDeathEffectInfo.size(); i++)
	{
		if (vEnemyDeathEffectInfo[i].lifeTime > 0)
		{
			vEnemyDeathEffectInfo[i].lifeTime -= TIMEMANAGER->getElapsedTime();
			EFFECTMANAGER->render_custom(vEnemyDeathEffectInfo[i].pos.x - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX * 0.5),
				vEnemyDeathEffectInfo[i].pos.y - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY * 0.5));
		}
		else
		{
			vEnemyDeathEffectInfo.erase(vEnemyDeathEffectInfo.begin() + i);
		}
	}
}

void EnemyManager::RemoveEnemy(int ID)
{
	_vEnemy.erase(_vEnemy.begin() + ID);
}

void EnemyManager::SetSnake()
{
	POINT currentID;
	for (currentID.y = 0; currentID.y < TILEY - 1; currentID.y++)
	{
		for (currentID.x = 1; currentID.x < TILEX - 1; currentID.x++)
		{
			if (getDistance(_mg->Get_Tile(currentID.y, currentID.x)->pos.x, _mg->Get_Tile(currentID.y, currentID.x)->pos.y,
				_mg->Get_Tile_EntranceTilePos().x, _mg->Get_Tile_EntranceTilePos().y) < TILESIZE * 3 ||
				_mg->Get_Tile(currentID.y, currentID.x)->pos.y == _mg->Get_Tile_EntranceTilePos().y)
				continue;

			if (_mg->Get_Tile(currentID.y, currentID.x - 1)->bgTile && _mg->Get_Tile(currentID.y, currentID.x)->bgTile && _mg->Get_Tile(currentID.y, currentID.x + 1)->bgTile &&
				!_mg->Get_Tile(currentID.y + 1, currentID.x - 1)->bgTile && !_mg->Get_Tile(currentID.y + 1, currentID.x)->bgTile && !_mg->Get_Tile(currentID.y + 1, currentID.x + 1)->bgTile)
			{
				int selectSpawn = RND->getInt(2);
				if (selectSpawn)
				{
					Enemy* snake;
					snake = new Snake;
					int randImgSelect = RND->getInt(2);
					int randDir = RND->getInt(2);
					if (!randImgSelect) snake->setImage("Snake_Idle");
					else snake->setImage("Snake_Move");
					snake->init({ (float)_mg->Get_Tile(currentID.y, currentID.x)->pos.x, (float)_mg->Get_Tile(currentID.y, currentID.x)->pos.y }, 1.0f, 0, 1, randImgSelect, 1, 1, randDir);
					if (!randImgSelect)
					{
						if (!randDir)
						{
							snake->setAni("snake_Idle_Left");
						}
						else
						{
							snake->setAni("snake_Idle_Right");
						}
					}
					else
					{
						if (!randDir)
						{
							snake->setAni("snake_Move_Left");
						}
						else
						{
							snake->setAni("snake_Move_Right");
						}
					}
					_vEnemy.push_back(snake);
					_vEnemy[(_vEnemy.size() - 1)]->getAni()->start();
					currentID.x += 2;
				}
			}
		}
	}
}

void EnemyManager::SetViper()
{
	POINT currentID;
	for (currentID.y = 0; currentID.y < TILEY - 1; currentID.y++)
	{
		for (currentID.x = 1; currentID.x < TILEX - 1; currentID.x++)
		{
			if (getDistance(_mg->Get_Tile(currentID.y, currentID.x)->pos.x, _mg->Get_Tile(currentID.y, currentID.x)->pos.y,
				_mg->Get_Tile_EntranceTilePos().x, _mg->Get_Tile_EntranceTilePos().y) < TILESIZE * 3 ||
				_mg->Get_Tile(currentID.y, currentID.x)->pos.y == _mg->Get_Tile_EntranceTilePos().y)
				continue;

			if (_mg->Get_Tile(currentID.y, currentID.x - 1)->bgTile && _mg->Get_Tile(currentID.y, currentID.x)->bgTile && _mg->Get_Tile(currentID.y, currentID.x + 1)->bgTile &&
				!_mg->Get_Tile(currentID.y + 1, currentID.x - 1)->bgTile && !_mg->Get_Tile(currentID.y + 1, currentID.x)->bgTile && !_mg->Get_Tile(currentID.y + 1, currentID.x + 1)->bgTile)
			{
				int selectSpawn = RND->getInt(2);
				if (selectSpawn)
				{
					Enemy* viper;
					viper = new Viper;
					int randImgSelect = RND->getInt(2);
					int randDir = RND->getInt(2);
					if (!randImgSelect) viper->setImage("Viper_Idle");
					else viper->setImage("Viper_Move");

					viper->init({ (float)_mg->Get_Tile(currentID.y, currentID.x)->pos.x, (float)_mg->Get_Tile(currentID.y, currentID.x)->pos.y }, 1.0f, 1, 1, randImgSelect, 1, 2, randDir);
					if (!randImgSelect)
					{
						if (!randDir)
						{
							viper->setAni("viper_Idle_Left");
						}
						else
						{
							viper->setAni("viper_Idle_Right");
						}
					}
					else
					{
						if (!randDir)
						{
							viper->setAni("viper_Move_Left");
						}
						else
						{
							viper->setAni("viper_Move_Right");
						}
					}
					_vEnemy.push_back(viper);
					_vEnemy[(_vEnemy.size() - 1)]->getAni()->start();
					currentID.x += 2;
				}
			}
		}
	}
}

void EnemyManager::SetJjangshi()
{
	POINT currentID;
	for (currentID.y = 0; currentID.y < TILEY - 1; currentID.y++)
	{
		for (currentID.x = 1; currentID.x < TILEX - 1; currentID.x++)
		{
			if (getDistance(_mg->Get_Tile(currentID.y, currentID.x)->pos.x, _mg->Get_Tile(currentID.y, currentID.x)->pos.y,
				_mg->Get_Tile_EntranceTilePos().x, _mg->Get_Tile_EntranceTilePos().y) < TILESIZE * 3 || 
				_mg->Get_Tile(currentID.y, currentID.x)->pos.y == _mg->Get_Tile_EntranceTilePos().y)
				continue;

			if (currentID.y > 0)
			{
				if (_mg->Get_Tile(currentID.y - 1, currentID.x - 1)->bgTile && _mg->Get_Tile(currentID.y - 1, currentID.x)->bgTile && _mg->Get_Tile(currentID.y - 1, currentID.x + 1)->bgTile &&
					_mg->Get_Tile(currentID.y, currentID.x - 1)->bgTile && _mg->Get_Tile(currentID.y, currentID.x)->bgTile && _mg->Get_Tile(currentID.y, currentID.x + 1)->bgTile &&
					!_mg->Get_Tile(currentID.y + 1, currentID.x - 1)->bgTile && !_mg->Get_Tile(currentID.y + 1, currentID.x)->bgTile && !_mg->Get_Tile(currentID.y + 1, currentID.x + 1)->bgTile)
				{
					int selectSpawn = RND->getInt(2);
					if (selectSpawn)
					{
						Enemy* jjangshi;
						jjangshi = new Jjangshi;
						int randDir = RND->getInt(2);
						jjangshi->setImage("Jiangshi_Move");
						jjangshi->init({ (float)_mg->Get_Tile(currentID.y - 1, currentID.x)->pos.x, (float)_mg->Get_Tile(currentID.y - 1, currentID.x)->pos.y },
							5.0f, 2, 0, 1, 1, 2, RND->getInt(2));
						if (!randDir)
						{
							jjangshi->setAni("jjangshi_Move_Left");
						}
						else
						{
							jjangshi->setAni("jjangshi_Move_Right");
						}
						_vEnemy.push_back(jjangshi);
						_vEnemy[(_vEnemy.size() - 1)]->getAni()->start();
						currentID.x += 2;
					}
				}
			}
		}
	}
}


void EnemyManager::Collision()
{
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		RECT temp;
		PHYSICS->TileCollision(_vEnemy[i]->getPosStateToIntPt(), _vEnemy[i]->getPosPt(), _vEnemy[i]->getPosID(),
			_vEnemy[i]->getRectPt(), _vEnemy[i]->getImage(), _vEnemy[i]->getDir(), _vEnemy[i]->getGravityPt(), _vEnemy[i]->getJumpPowerPt(), _vEnemy[i]->getSpeedPt());

		if (_vEnemy[i]->getCurrentHP() <= 0)
		{
			if (_vEnemy[i]->getKind() == 0 || _vEnemy[i]->getKind() == 1)
			{
				SOUNDMANAGER->play("Snake_Death");
			}
			else if (_vEnemy[i]->getKind() == 2)
			{
				SOUNDMANAGER->play("Jjangshi_Dead");
			}
			EFFECTMANAGER->play("MonsterDeath", _vEnemy[i]->getRect().left - 85 - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2),
				_vEnemy[i]->getRect().top - 75 - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2));
			EnemyDeathEffectInfo newInfo;
			newInfo.pos = { _vEnemy[i]->getRect().left - 85, _vEnemy[i]->getRect().top - 75 };
			newInfo.lifeTime = 0.25f;
			vEnemyDeathEffectInfo.push_back(newInfo);
			RemoveEnemy(i);
		}
		else
		{
			if (IntersectRect(&temp, &_player->Get_playerRect(), &_vEnemy[i]->getRect()))
			{
				if (((_vEnemy[i]->getRect().top + _vEnemy[i]->getRect().bottom) / 2) - (_vEnemy[i]->getImage()->getFrameHeight() / 4) > _player->Get_playerRect().bottom)
				{
					_player->Set_playerGravity(0);
					_player->Set_playerJumpPower(_player->Get_playerJumpPower() + PLAYERJUMPPOWER);

					if (_vEnemy[i]->get_hitSwitch())
					{
						_vEnemy[i]->setCurrentHP(_vEnemy[i]->getCurrentHP() - 1);
						SOUNDMANAGER->play("Player_GiveDmg");
					}
					_vEnemy[i]->set_hitSwitch(false);
				}
			}
			else if (!IntersectRect(&temp, &_player->Get_playerRect(), &_vEnemy[i]->getRect()))
			{
				_vEnemy[i]->set_hitSwitch(true);
			}
		}
	}
	EFFECTMANAGER->update();
}

void EnemyManager::AddEnemyImg()
{
	IMAGEMANAGER->addFrameImage("Snake_Move", "Image/Monster/Snake/Snake_Move.bmp", 732, 102, 12, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Snake_Idle", "Image/Monster/Snake/Snake_Idle.bmp", 54, 100, 1, 2, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("Viper_Idle", "Image/Monster/Viper/Viper_Idle.bmp", 49, 120, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Viper_Move", "Image/Monster/Viper/Viper_Move.bmp", 780, 120, 12, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Viper_Attack", "Image/Monster/Viper/Viper_Attack.bmp", 492, 124, 6, 2, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("Jiangshi_Move", "Image/Monster/Jiangshi/Jiangshi_Move.bmp", 621, 140, 9, 2, true, RGB(255, 0, 255));

	// 뱀
	int snake_Idle_Left[] = { 0 };
	KEYANIMANAGER->addArrayFrameAnimation("snake_Idle_Left", "Snake_Idle", snake_Idle_Left, 1, 1, true);
	int snake_Idle_Right[] = { 1 };
	KEYANIMANAGER->addArrayFrameAnimation("snake_Idle_Right", "Snake_Idle", snake_Idle_Right, 1, 1, true);

	int snake_Move_Left[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	KEYANIMANAGER->addArrayFrameAnimation("snake_Move_Left", "Snake_Move", snake_Move_Left, 12, 1, true);
	int snake_Move_Right[] = { 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };
	KEYANIMANAGER->addArrayFrameAnimation("snake_Move_Right", "Snake_Move", snake_Move_Right, 12, 1, true);

	// 독사
	int viper_Idle_Left[] = { 0 };
	KEYANIMANAGER->addArrayFrameAnimation("viper_Idle_Left", "Viper_Idle", viper_Idle_Left, 1, 1, true);
	int viper_Idle_Right[] = { 1 };
	KEYANIMANAGER->addArrayFrameAnimation("viper_Idle_Right", "Viper_Idle", viper_Idle_Right, 1, 1, true);
	
	int viper_Move_Left[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	KEYANIMANAGER->addArrayFrameAnimation("viper_Move_Left", "Viper_Move", viper_Move_Left, 12, 1, true);
	int viper_Move_Right[] = { 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };
	KEYANIMANAGER->addArrayFrameAnimation("viper_Move_Right", "Viper_Move", viper_Move_Right, 12, 1, true);
	
	int viper_Attack_Left[] = { 0, 1, 2, 3, 4, 5 };
	KEYANIMANAGER->addArrayFrameAnimation("viper_Attack_Left", "Viper_Attack", viper_Attack_Left, 6, 1, true);
	int viper_Attack_Right[] = { 6, 7, 8, 9, 10, 11 };
	KEYANIMANAGER->addArrayFrameAnimation("viper_Attack_Right", "Viper_Attack", viper_Attack_Right, 6, 1, true);
	
	// 강시
	int jjangshi_Move_Left[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	KEYANIMANAGER->addArrayFrameAnimation("jjangshi_Move_Left", "Jiangshi_Move", jjangshi_Move_Left, 9, 1, false);
	int jjangshi_Move_Right[] = { 9, 10, 11, 12, 13, 14, 15, 16, 17 };
	KEYANIMANAGER->addArrayFrameAnimation("jjangshi_Move_Right", "Jiangshi_Move", jjangshi_Move_Right, 9, 1, false);
}