#include "stdafx.h"
#include "Player.h"
#include "MapGenerator.h"
#include "EnemyManager.h"



HRESULT Player::init(int _posState, int _state, float _x, float _y, int _hp)
{
	init_AddImgAndAni();
	init_AddItemAndAni();
	exitAlpha = 0;

	player.posState = (PlayerPosState)_posState;
	player.aniState = (PlayerAniState)_state;
	player.pos = { _x, _y };
	player.id = { 0 , 0 };
	player.speed = 0;
	player.gravity = 0;
	player.jumpPower = 0;
	player.knockBack.speed = 0;
	player.hp = _hp;
	player.dir = true;
	player.knockBack.dir = true;
	player.rc = RectMakeCenter(_x, _y, player.img->getFrameWidth(), player.img->getFrameHeight());
	playerDirTemp = player.dir;
	playerPosStateTemp = player.posState;
	playerAniStateTemp = player.aniState;
	player.isDead = false;
	player.hitSwitch = true;
	player.takeHitDelay = 0;
	isPlayerExit = false;
	isLevelClear = false;
	bombEffectPos = { -1, -1 };
	return S_OK;
}

void Player::release()
{
	SAFE_DELETE(_mapGenerator);
	SAFE_DELETE(_enemyManager);
}

void Player::update()
{
	update_PlayerPosState();
	update_Movement();
	update_TileCollision();
	update_EnemyAndObjCollision();
	update_ChangeAni();
	if (!player.dir) player.pos.x -= player.speed;
	else player.pos.x += player.speed;

	player.rc = RectMakeCenter(player.pos.x, player.pos.y, player.img->getFrameWidth(), player.img->getFrameHeight());
	player.id = { (int)player.pos.x / 64, (int)player.pos.y / 64 };
	CAMERAMANAGER->Set_PlayerCenterPos({ player.pos.x, player.pos.y });
	KEYANIMANAGER->update();

	if (player.isDead)
	{
		if (!player.dir)
		{
			update_ChangeAnimation("Player_Dead", "player_Dead_Left");
		}
		else
		{
			update_ChangeAnimation("Player_Dead", "player_Dead_Right");
		}
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
		{
			SOUNDMANAGER->stop("A01_A");
			SOUNDMANAGER->stop("A01_B");
			SOUNDMANAGER->stop("A01_C");
			SOUNDMANAGER->stop("YouDied");
			SCENEMANAGER->changeScene("1_MainMenu");
		}
	}
}

void Player::render()
{
	player.img->aniRender(getMemDC(), player.rc.left - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2), player.rc.top - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2), player.ani);
	//render_Debug(getMemDC());
	IMAGEMANAGER->findImage("ExitAlphaImg")->alphaRender(getMemDC(),
		_mapGenerator->Get_RandomMapGeneSupport().Get_ExitRect().left - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2),
		_mapGenerator->Get_RandomMapGeneSupport().Get_ExitRect().top - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2),
		exitAlpha);

	if (bomb.Get_isThrow())
	{
		bomb.render();
	}
	if (bombEffectPos.x != -1 && bombEffectPos.y != -1)
	{
		bombEffectImg->aniRender(getMemDC(), bombEffectPos.x - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2),
			bombEffectPos.y - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2), bombEffectAnim);
	}
}



void Player::init_AddImgAndAni()
{
	// 플레이어 HUD 관련
	IMAGEMANAGER->addFrameImage("Player_HUD", "Image/Player/Player_HUD.bmp", 512, 256, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Player_Life", "Image/Player/Player_Life.bmp", 320, 64, 5, 1, true, RGB(255, 0, 255));

	// 플레이어
	IMAGEMANAGER->addFrameImage("Player_Idle", "Image/Player/Player_Idle.bmp", 52, 98, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Player_Move", "Image/Player/Player_Move.bmp", 568, 110, 8, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Player_Air", "Image/Player/Player_Air.bmp", 71, 92, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Player_Crawl", "Image/Player/Player_Crawl.bmp", 60, 60, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Player_Enter", "Image/Player/Player_Enter.bmp", 252, 53, 6, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Player_Knockback", "Image/Player/Player_Knockback.bmp", 300, 124, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Player_Attack", "Image/Player/Player_Attack.bmp", 780, 104, 12, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Player_Dead", "Image/Player/Player_Dead.bmp", 71, 92, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Player_Hanging", "Image/Player/Player_Hanging.bmp", 53, 124, 1, 2, true, RGB(255, 0, 255));

	// 기타
	IMAGEMANAGER->addImage("ExitAlphaImg", "Image/Tile/Palette/ExitAlphaImg.bmp", 68, 83, false, RGB(0, 0, 0));

	//// 플레이어 ////
	// Idle상태
	int player_Idle_Left[] = { 0 };
	KEYANIMANAGER->addArrayFrameAnimation("player_Idle_Left", "Player_Idle", player_Idle_Left, 1, 1, true);
	int player_Idle_Right[] = { 1 };
	KEYANIMANAGER->addArrayFrameAnimation("player_Idle_Right", "Player_Idle", player_Idle_Right, 1, 1, true);

	// Move상태
	int player_Move_Left[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("player_Move_Left", "Player_Move", player_Move_Left, 8, 1, true);
	int player_Move_Right[] = { 8, 9, 10, 11, 12, 13, 14, 15 };
	KEYANIMANAGER->addArrayFrameAnimation("player_Move_Right", "Player_Move", player_Move_Right, 8, 1, true);

	// Air(posState부분)상태
	int player_Air_Left[] = { 0 };
	KEYANIMANAGER->addArrayFrameAnimation("player_Air_Left", "Player_Air", player_Air_Left, 1, 1, true);
	int player_Air_Right[] = { 1 };
	KEYANIMANAGER->addArrayFrameAnimation("player_Air_Right", "Player_Air", player_Air_Right, 1, 1, true);

	// Crawl(포복)상태
	int player_Crawl_Left[] = { 0 };
	KEYANIMANAGER->addArrayFrameAnimation("player_Crawl_Left", "Player_Crawl", player_Crawl_Left, 1, 1, true);
	int player_Crawl_Right[] = { 1 };
	KEYANIMANAGER->addArrayFrameAnimation("player_Crawl_Right", "Player_Crawl", player_Crawl_Right, 1, 1, true);

	// 출구 찾아서 들어갈때 애니메이션 상태
	int player_Enter[] = { 0, 1, 2, 3, 4, 5 };
	KEYANIMANAGER->addArrayFrameAnimation("player_Enter", "Player_Enter", player_Enter, 6, 1, true);

	// 플레이어 맞을 시 피격 애니메이션
	int player_Knockback_Left[] = { 0, 1, 2, 3 };
	KEYANIMANAGER->addArrayFrameAnimation("player_Knockback_Left", "Player_Knockback", player_Knockback_Left, 4, 1, false);
	int player_Knockback_Right[] = { 4, 5, 6, 7 };
	KEYANIMANAGER->addArrayFrameAnimation("player_Knockback_Right", "Player_Knockback", player_Knockback_Right, 4, 1, false);

	// 플레이어 공격 상태
	int player_Attack_Left[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	KEYANIMANAGER->addArrayFrameAnimation("player_Attack_Left", "Player_Attack", player_Attack_Left, 12, 1, false);
	int player_Attack_Right[] = { 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };
	KEYANIMANAGER->addArrayFrameAnimation("player_Attack_Right", "Player_Attack", player_Attack_Right, 12, 1, false);

	int player_Dead_Left[] = { 0 };
	KEYANIMANAGER->addArrayFrameAnimation("player_Dead_Left", "Player_Dead", player_Dead_Left, 1, 1, false);
	int player_Dead_Right[] = { 1 };
	KEYANIMANAGER->addArrayFrameAnimation("player_Dead_Right", "Player_Dead", player_Dead_Right, 1, 1, false);

	int player_Hanging_Left[] = { 0 };
	KEYANIMANAGER->addArrayFrameAnimation("player_Hanging_Left", "Player_Hanging", player_Hanging_Left, 1, 1, false);
	int player_Hanging_Right[] = { 1 };
	KEYANIMANAGER->addArrayFrameAnimation("player_Hanging_Right", "Player_Hanging", player_Hanging_Right, 1, 1, false);

	player.ani = new animation;
	player.dir = true;
	player.aniState = Idle;
	update_ChangeAnimation("Player_Idle", "player_Idle_Right");
}

void Player::init_AddItemAndAni()
{
	IMAGEMANAGER->addFrameImage("Explosion", "Image/Bomb/Explosion.bmp", 1024, 1024, 4, 4, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Bomb_Ani1", "Image/Bomb/Bomb_Ani1.bmp", 76, 42, 2, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("Bomb_Ani2", "Image/Bomb/Bomb_Ani2.bmp", 76, 42, 2, 1, true, RGB(255, 0, 255));

	int explosion[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	KEYANIMANAGER->addArrayFrameAnimation("explosion", "Explosion", explosion, 16, 1, false);
	int bomb_Ani1[] = { 0, 1 };
	KEYANIMANAGER->addArrayFrameAnimation("bomb_Ani1", "Bomb_Ani1", bomb_Ani1, 2, 1, true);
	int bomb_Ani2[] = { 0, 1 };
	KEYANIMANAGER->addArrayFrameAnimation("bomb_Ani2", "Bomb_Ani2", bomb_Ani2, 2, 1, false);

	bombEffectAnim = new animation;
	bombEffectImg = IMAGEMANAGER->findImage("Explosion");
	bombEffectAnim = KEYANIMANAGER->findAnimation("explosion");

	bombEffectAnim->start();
	bombEffectAnim->stop();
}

void Player::update_PlayerPosState()
{
	if (player.posState == Air)
	{
		if (!isPlayerExit)
		{
			player.gravity += GRAVITY;
			if (player.gravity > 15) player.gravity = 15;
			player.pos.y += player.gravity;
		}
	}
	if (player.posState == Ground)
	{
		player.gravity = 0;
	}
}

void Player::update_Movement()
{
	//// 키 입력 부분 ////
	RECT tempRC;
	if (!isPlayerExit && !player.isDead)
	{
		if (player.aniState != Hit)
		{
			if (!KEYMANAGER->isStayKeyDown(VK_DOWN))
			{
				if (KEYMANAGER->isStayKeyDown(VK_LEFT) && KEYMANAGER->isStayKeyDown(VK_RIGHT)) // 왼쪽, 오른쪽 방향키를 동시에 누른 경우 이동 불가
				{
					if (player.posState == Ground)
					{
						player.speed = 0;
					}
				}
				else if (KEYMANAGER->isStayKeyDown(VK_LEFT)) //  ■■■■■■■■■■ 왼쪽 이동 ■■■■■■■■■■
				{
					player.dir = false;
					player.speed = PLAYERSPEED;
				}
				else if (KEYMANAGER->isStayKeyDown(VK_RIGHT)) //  ■■■■■■■■■■ 오른쪽 이동 ■■■■■■■■■■
				{
					player.dir = true;
					player.speed = PLAYERSPEED;
				}
			}

			if (KEYMANAGER->isOnceKeyUp(VK_LEFT))
			{
				player.speed = 0;
			}
			if (KEYMANAGER->isOnceKeyUp(VK_RIGHT))
			{
				player.speed = 0;
			}

			//// 점프 ////
			//// 점프 ////
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				if (player.posState == Ground || player.aniState == Hanging)
				{
					player.posState = Air;
					player.jumpPower = PLAYERJUMPPOWER;
					SOUNDMANAGER->play("Player_Jump");
				}
			}

			//// 포복 ////
			//// 포복 ////
			if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
			{
				player.aniState = Crawl;
			}
			if (KEYMANAGER->isOnceKeyUp(VK_DOWN))
			{
				player.aniState = Idle;
			}

			//// 매달리기 ////
			//// 매달리기 ////
			if (player.posState == Air && 
				player.id.x > 0 && player.id.x < TILEX)
			{
				RECT tempRC;
				if (KEYMANAGER->isOnceKeyDown('E'))
				{
					if (_mapGenerator->Get_Tile(player.id.y, player.id.x)->bgTile && _mapGenerator->Get_Tile(player.id.y + 1, player.id.x)->bgTile)
					{
						if (!player.dir)
						{
							if (!_mapGenerator->Get_Tile(player.id.y, player.id.x - 1)->bgTile && player.rc.left - _mapGenerator->Get_Tile(player.id.y, player.id.x - 1)->rc.right < 10)
							{
								SOUNDMANAGER->play("Player_Jump");
								SOUNDMANAGER->play("Player_DraggingWall", 0.2f);
							}
						}
						else
						{
							if (!_mapGenerator->Get_Tile(player.id.y, player.id.x + 1)->bgTile && _mapGenerator->Get_Tile(player.id.y, player.id.x + 1)->rc.left - player.rc.right < 10)
							{
								SOUNDMANAGER->play("Player_Jump");
								SOUNDMANAGER->play("Player_DraggingWall", 0.2f);
							}
						}
					}
				}

				if (KEYMANAGER->isStayKeyDown('E'))
				{
					if (_mapGenerator->Get_Tile(player.id.y, player.id.x)->bgTile && _mapGenerator->Get_Tile(player.id.y + 1, player.id.x)->bgTile)
					{
						if (!player.dir)
						{
							if (!_mapGenerator->Get_Tile(player.id.y, player.id.x - 1)->bgTile && player.rc.left - _mapGenerator->Get_Tile(player.id.y, player.id.x - 1)->rc.right < 10)
							{
								player.gravity = 0;
								player.jumpPower = 0;
								player.speed = 0;
								player.pos = { (float)_mapGenerator->Get_Tile(player.id.y, player.id.x - 1)->rc.right + (player.img->getFrameWidth() / 2), (float)player.pos.y };
								update_ChangeAnimation("Player_Hanging", "player_Hanging_Left");
								player.aniState = Hanging;
							}
							else
							{
								SOUNDMANAGER->stop("Player_DraggingWall");
								player.aniState = Idle;
								if (!player.dir) update_ChangeAnimation("Player_Air", "player_Air_Left");
								else update_ChangeAnimation("Player_Air", "player_Air_Right");
							}
						}
						else
						{
							if (!_mapGenerator->Get_Tile(player.id.y, player.id.x + 1)->bgTile && _mapGenerator->Get_Tile(player.id.y, player.id.x + 1)->rc.left - player.rc.right < 10)
							{
								player.gravity = 0;
								player.jumpPower = 0;
								player.speed = 0;
								player.pos = { (float)_mapGenerator->Get_Tile(player.id.y, player.id.x + 1)->rc.left - (player.img->getFrameWidth() / 2), (float)player.pos.y };
								update_ChangeAnimation("Player_Hanging", "player_Hanging_Right");
								player.aniState = Hanging;
							}
							else
							{
								SOUNDMANAGER->stop("Player_DraggingWall");
								player.aniState = Idle;
								if (!player.dir) update_ChangeAnimation("Player_Air", "player_Air_Left");
								else update_ChangeAnimation("Player_Air", "player_Air_Right");
							}
						}
					}
				}
				if (KEYMANAGER->isOnceKeyUp('E') && player.aniState == Hanging)
				{
					player.aniState = Idle;
					player.jumpPower = PLAYERJUMPPOWER;
					SOUNDMANAGER->stop("Player_DraggingWall");
					if (!player.dir) update_ChangeAnimation("Player_Air", "player_Air_Left");
					else update_ChangeAnimation("Player_Air", "player_Air_Right");
				}
			}

			if (!bomb.Get_isThrow())
			{
				if (KEYMANAGER->isOnceKeyDown('Z'))
				{
					bomb.init({player.pos.x, player.pos.y}, player.dir, 10.0f, 2.5f);
				}
			}
		}
	}

	//// 기타 부분 ////
	if (player.jumpPower > 10) // 점프 가속도 제한
	{
		player.jumpPower = 10;
	}
	if (player.jumpPower > 0)
	{
		player.pos.y -= player.jumpPower;
		player.jumpPower -= GRAVITY;
	}

	// 이동 애니메이션
	if (!isPlayerExit && !player.isDead)
	{
		if (player.aniState != Crawl && player.aniState != Hit)
		{
			if (player.posState == Ground)
			{
				if (player.speed > 0)
				{
					player.aniState = Move;
				}
				else
				{
					player.aniState = Idle;
				}
			}
		}
		// 맞았을때
		if (player.aniState == Hit)
		{
			if (player.knockBack.speed > 15)
			{
				player.knockBack.speed = 15;
			}
			if (player.knockBack.speed > 0)
			{
				if (!player.knockBack.dir)
				{
					player.pos.x -= player.knockBack.speed;
				}
				else
				{
					player.pos.x += player.knockBack.speed;
				}
				player.knockBack.speed -= KNOCKBACK_R;
			}
			else
			{
				if (player.hp == 1)
				{
					SOUNDMANAGER->play("YouDied");
					player.isDead = true;
				}
				player.hp--;
				RECT tempRC;
				player.aniState = Idle;
			}
		}

		// 맵 영역 벗어나는 것 방지
		if (player.pos.y < player.img->getFrameHeight() / 2)
		{
			player.pos.y = player.img->getFrameHeight() / 2;
			player.jumpPower = 0;
		}
		if (player.pos.y > TILEY * TILESIZE - player.img->getFrameHeight() / 2)
		{
			player.pos.y = TILEY * TILESIZE - player.img->getFrameHeight() / 2;
			player.jumpPower = 0;
		}

		if (player.pos.x < player.img->getFrameWidth() / 2) // 좌
		{
			player.pos.x = player.img->getFrameWidth() / 2;
			player.knockBack.speed = 0;
		}
		if (player.pos.x > TILEX * TILESIZE - player.img->getFrameWidth() / 2) // 우
		{
			player.pos.x = TILEX * TILESIZE - player.img->getFrameWidth() / 2;
			player.knockBack.speed = 0;
		}
	}
}

void Player::update_TileCollision()
{
	RECT tempRC;
	if (_mapGenerator->Get_Tile(player.id.y, player.id.x)->bgTile)
	{
		// ◆◆◆◆◆ 상 충돌 처리 ◆◆◆◆◆
		if (!_mapGenerator->Get_Tile(player.id.y - 1, player.id.x)->bgTile && player.id.y > 0) // 플레이어 위 타일이 벽인 경우
		{
			if (IntersectRect(&tempRC, &_mapGenerator->Get_Tile(player.id.y - 1, player.id.x)->rc, &player.rc)) // 충돌 시
			{
				player.jumpPower = 0; // 더 이상 타일안으로 들어가지 않도록 점프 가속도 0으로 만듬
				player.pos.y = _mapGenerator->Get_Tile(player.id.y - 1, player.id.x)->rc.bottom + (player.img->getFrameHeight() / 2);
				player.knockBack.speed = 0;
			}
		}

		// ◆◆◆◆◆ 하 충돌 처리 ◆◆◆◆◆
		if (!_mapGenerator->Get_Tile(player.id.y + 1, player.id.x)->bgTile && player.id.y < TILEY - 1) // 플레이어 밑 타일이 벽인 경우
		{
			if (IntersectRect(&tempRC, &_mapGenerator->Get_Tile(player.id.y + 1, player.id.x)->rc, &player.rc)) // 충돌 시
			{
				if (player.gravity > 0) // 만약 플레이어 가속도가 0이상이면
				{
					SOUNDMANAGER->play("Player_Bounce");
					player.pos.y -= player.gravity;
					player.gravity = 0;
				}
				player.posState = Ground;
				player.pos.y = _mapGenerator->Get_Tile(player.id.y + 1, player.id.x)->rc.top - (player.img->getFrameHeight() / 2);
			}
			if (player.posState == Ground) // 위치 상태는 '지면'인데
			{
				if (!IntersectRect(&tempRC, &_mapGenerator->Get_Tile(player.id.y + 1, player.id.x)->rc, &player.rc)) // 만약에 붕떠있는 경우
				{
					player.pos.y = _mapGenerator->Get_Tile(player.id.y + 1, player.id.x)->rc.top - (player.img->getFrameHeight() / 2);
				}
			}
		}
		else
		{
			player.posState = Air;
		}

		if (player.knockBack.speed <= 0) // 넉백 스피드가 0 이하면
		{
			if (!player.dir)
			{
				// ◆◆◆◆◆ 좌 충돌 처리 ◆◆◆◆◆
				if (!_mapGenerator->Get_Tile(player.id.y, player.id.x - 1)->bgTile)
				{
					if (IntersectRect(&tempRC, &_mapGenerator->Get_Tile(player.id.y, player.id.x - 1)->rc, &player.rc))
					{
						if (player.posState == Ground) player.pos.x = _mapGenerator->Get_Tile(player.id.y, player.id.x - 1)->rc.right + (player.img->getFrameWidth() / 2);
						else player.pos.x = _mapGenerator->Get_Tile(player.id.y, player.id.x - 1)->rc.right + (player.img->getFrameWidth() / 2);
						player.knockBack.speed = 0;
					}
				}
			}
			else
			{
				// ◆◆◆◆◆ 우 충돌 처리 ◆◆◆◆◆
				if (!_mapGenerator->Get_Tile(player.id.y, player.id.x + 1)->bgTile)
				{
					if (IntersectRect(&tempRC, &_mapGenerator->Get_Tile(player.id.y, player.id.x + 1)->rc, &player.rc))
					{
						if (player.posState == Ground) player.pos.x = _mapGenerator->Get_Tile(player.id.y, player.id.x + 1)->rc.left - (player.img->getFrameWidth() / 2);
						else  player.pos.x = _mapGenerator->Get_Tile(player.id.y, player.id.x + 1)->rc.left - (player.img->getFrameWidth() / 2);
						player.knockBack.speed = 0;
					}
				}
			}
		}
		else // 0보다 크면
		{
			// ◆◆◆◆◆ 좌 충돌 처리 ◆◆◆◆◆
			if (!_mapGenerator->Get_Tile(player.id.y, player.id.x - 1)->bgTile)
			{
				if (IntersectRect(&tempRC, &_mapGenerator->Get_Tile(player.id.y, player.id.x - 1)->rc, &player.rc))
				{
					if (player.posState == Ground) player.pos.x = _mapGenerator->Get_Tile(player.id.y, player.id.x - 1)->rc.right + (player.img->getFrameWidth() / 2);
					else player.pos.x = _mapGenerator->Get_Tile(player.id.y, player.id.x - 1)->rc.right + (player.img->getFrameWidth() / 2);
					player.knockBack.speed = 0;
				}
			}
			// ◆◆◆◆◆ 우 충돌 처리 ◆◆◆◆◆
			if (!_mapGenerator->Get_Tile(player.id.y, player.id.x + 1)->bgTile)
			{
				if (IntersectRect(&tempRC, &_mapGenerator->Get_Tile(player.id.y, player.id.x + 1)->rc, &player.rc))
				{
					if (player.posState == Ground) player.pos.x = _mapGenerator->Get_Tile(player.id.y, player.id.x + 1)->rc.left - (player.img->getFrameWidth() / 2);
					else  player.pos.x = _mapGenerator->Get_Tile(player.id.y, player.id.x + 1)->rc.left - (player.img->getFrameWidth() / 2);
					player.knockBack.speed = 0;
				}
			}
		}

		// ◆◆◆◆◆ 기타 충돌 처리 ◆◆◆◆◆
		if (player.posState == Air)
		{
			if (!_mapGenerator->Get_Tile(player.id.y + 1, player.id.x - 1)->bgTile) // 좌 하단
			{
				if (IntersectRect(&tempRC, &_mapGenerator->Get_Tile(player.id.y + 1, player.id.x - 1)->rc, &player.rc)) // 충돌 시
				{
					if (_mapGenerator->Get_Tile(player.id.y + 1, player.id.x - 1)->rc.top < player.rc.bottom)
					{
						player.speed = 0;
						player.knockBack.speed = 0;
					}
				}
			}
			if (!_mapGenerator->Get_Tile(player.id.y + 1, player.id.x + 1)->bgTile) // 우 하단
			{
				if (IntersectRect(&tempRC, &_mapGenerator->Get_Tile(player.id.y + 1, player.id.x + 1)->rc, &player.rc)) // 충돌 시
				{
					if (_mapGenerator->Get_Tile(player.id.y + 1, player.id.x + 1)->rc.top < player.rc.bottom)
					{
						player.speed = 0;
						player.knockBack.speed = 0;
					}
				}
			}
		}
		if (!_mapGenerator->Get_Tile(player.id.y, player.id.x - 1)->bgTile && !_mapGenerator->Get_Tile(player.id.y, player.id.x + 1)->bgTile) // 좌우 사이에 낑겨있을때
		{
			RECT tempRC2;
			if (IntersectRect(&tempRC, &_mapGenerator->Get_Tile(player.id.y, player.id.x - 1)->rc, &player.rc) &&
				IntersectRect(&tempRC2, &_mapGenerator->Get_Tile(player.id.y, player.id.x + 1)->rc, &player.rc))
			{
				player.speed = 0;
				player.knockBack.speed = 0;
				player.pos.x = _mapGenerator->Get_Tile(player.id.y, player.id.x)->pos.x;
			}
		}
	}
	else
	{	
		//if(!player.dir) // 플레이어 방향이 왼쪽인 경우
		//{
		//	if (_mapGenerator->Get_Tile(player.id.y, player.id.x)->rc.top < player.rc.top)
		//	{
		//		player.pos.y += (_mapGenerator->Get_Tile(player.id.y, player.id.x)->rc.bottom - player.rc.top);
		//	}
		//	else
		//	{
		//		player.pos.y -= (player.rc.bottom - _mapGenerator->Get_Tile(player.id.y, player.id.x)->rc.top);
		//	}
		//	player.pos.x += (_mapGenerator->Get_Tile(player.id.y, player.id.x)->rc.right - player.rc.left);
		//}
		//else // 오른쪽인 경우
		//{
		//	if (_mapGenerator->Get_Tile(player.id.y, player.id.x)->rc.top < player.rc.top)
		//	{
		//		player.pos.y += (_mapGenerator->Get_Tile(player.id.y, player.id.x)->rc.bottom - player.rc.top);
		//	}
		//	else
		//	{
		//		player.pos.y -= (player.rc.bottom - _mapGenerator->Get_Tile(player.id.y, player.id.x)->rc.top);
		//	}
		//	player.pos.x -= player.rc.right - _mapGenerator->Get_Tile(player.id.y, player.id.x)->rc.left;
		//}
		player.speed = 0;
		player.knockBack.speed = 0;
		player.jumpPower = 0;
		player.knockBack.speed = 0;
		player.gravity = 0;
	}
}

void Player::update_EnemyAndObjCollision()
{
	RECT tempRC;
	if (!isPlayerExit && !player.isDead)
	{
		for (int i = 0; i < _enemyManager->getV_Enemy().size(); i++)
		{
			if (player.hitSwitch && player.takeHitDelay <= 0)
			{
				if (IntersectRect(&tempRC, &player.rc, &_enemyManager->getV_Enemy()[i]->getRect())) 	// ◆◆◆◆◆ 적이랑 충돌시 ◆◆◆◆◆
				{
					if (((_enemyManager->getV_Enemy()[i]->getRect().top + _enemyManager->getV_Enemy()[i]->getRect().bottom) / 2) - (_enemyManager->getV_Enemy()[i]->getImage()->getFrameHeight() / 4) <
						player.rc.bottom)
					{
						if (_enemyManager->getV_Enemy()[i]->getPos().x > player.pos.x)
						{
							player.knockBack.dir = false;
						}
						else
						{
							player.knockBack.dir = true;
						}
						player.knockBack.speed = PLAYERKNOCKBACKSPEED;
						player.aniState = Hit;
						player.jumpPower = PLAYERJUMPPOWER;
						player.speed = 0;
						player.takeHitDelay = 100.0f;
						player.hitSwitch = false;
						SOUNDMANAGER->play("Player_TakeDmg");
						SOUNDMANAGER->play("Player_GiveDmg");
					}
				}
			}
			if (!player.hitSwitch)
			{
				if (player.takeHitDelay > 0)
				{
					player.takeHitDelay -= TIMEMANAGER->getElapsedTime();
				}
				else
				{
					player.hitSwitch = true;
				}
			}
		}
	
		// ◆◆◆◆◆ 오브젝트랑 충돌시 ◆◆◆◆◆
		if (IntersectRect(&tempRC, &_mapGenerator->Get_RandomMapGeneSupport().Get_ExitRect(), &player.rc))
		{
			if (KEYMANAGER->isOnceKeyDown('E'))
			{
				isPlayerExit = true;
				SOUNDMANAGER->stop("A01_A");
				SOUNDMANAGER->stop("A01_B");
				SOUNDMANAGER->stop("A01_C");
				SOUNDMANAGER->play("Player_IntoDoor");
				update_ChangeAnimation("Player_Enter", "player_Enter");
			}
		}
	}
	if (isPlayerExit)
	{
		player.pos = { (float)(_mapGenerator->Get_RandomMapGeneSupport().Get_ExitRect().right + _mapGenerator->Get_RandomMapGeneSupport().Get_ExitRect().left) / 2,
			(float)(_mapGenerator->Get_RandomMapGeneSupport().Get_ExitRect().top + _mapGenerator->Get_RandomMapGeneSupport().Get_ExitRect().bottom) / 2 };
		if (exitAlpha < 255)
		{
			exitAlpha += 5;
		}
		else
		{
			isLevelClear = true;
		}
	}

	if (bomb.Get_isThrow())
	{
		if (bomb.Get_BombTimer() <= 0)
		{
			RECT tempRC;
			for (int i = bomb.Get_BombArea_LT().y; i <= bomb.Get_BombArea_RB().y; i++) // 지형
			{
				for (int j = bomb.Get_BombArea_LT().x; j <= bomb.Get_BombArea_RB().x; j++)
				{
					if ( (i >= 0 && i < TILEY) && (j >= 0 && j < TILEX) )
					{
						if (_mapGenerator->Get_Tile(i, j)->tileFrameID.x == 0 && _mapGenerator->Get_Tile(i, j)->tileFrameID.y == 1)
						{
							continue;
						}
						if (!_mapGenerator->Get_Tile(i, j)->bgTile)
						{
							bool randSound = RND->getInt(2);
							string soundKey;
							if (randSound) soundKey = "Fragment1";
							else soundKey = "Fragment2";

							if (getDistance(player.pos.x, player.pos.y, _mapGenerator->Get_Tile(i, j)->pos.x, _mapGenerator->Get_Tile(i, j)->pos.y) < WINSIZEY * 0.5f)
							{
								SOUNDMANAGER->play(soundKey, 0.7f);
							}
							else if (getDistance(player.pos.x, player.pos.y, _mapGenerator->Get_Tile(i, j)->pos.x, _mapGenerator->Get_Tile(i, j)->pos.y) > WINSIZEY * 0.5f &&
								getDistance(player.pos.x, player.pos.y, _mapGenerator->Get_Tile(i, j)->pos.x, _mapGenerator->Get_Tile(i, j)->pos.y) < WINSIZEY)
							{
								SOUNDMANAGER->play(soundKey, 0.4f);
							}
							else
							{
								SOUNDMANAGER->play(soundKey, 0.2f);
							}
							_mapGenerator->Get_Tile(i, j)->bgTile = true;
						}
					}
				}
			}

			for (int i = 0; i < _enemyManager->getV_Enemy().size(); i++) // 적 
			{
				if (IntersectRect(&tempRC, &bomb.Get_BombArea(), &_enemyManager->getV_Enemy()[i]->getRect()))
				{
					_enemyManager->getV_Enemy()[i]->setCurrentHP(_enemyManager->getV_Enemy()[i]->getCurrentHP() - 4);
				}
			}

			if (IntersectRect(&tempRC, &bomb.Get_BombArea(), &player.rc)) // 플레이어
			{
				if (bomb.Get_BombPos().x > player.pos.x)
				{
					player.knockBack.dir = false;
				}
				else
				{
					player.knockBack.dir = true;
				}
				player.knockBack.speed = PLAYERKNOCKBACKSPEED * 2;
				player.aniState = Hit;
				player.jumpPower = PLAYERJUMPPOWER * 2;
				player.speed = 0;
				player.takeHitDelay = 100.0f;
				player.hitSwitch = false;
				SOUNDMANAGER->play("Player_TakeDmg");
				//SOUNDMANAGER->play("Player_GiveDmg");
			}

			if (getDistance(player.pos.x, player.pos.y, bomb.Get_BombPos().x, bomb.Get_BombPos().y) < WINSIZEY * 0.5f)
			{
				SOUNDMANAGER->play("Player_Bomb_Kaboom", 1.5f);
				SOUNDMANAGER->play("Player_Bomb_Kaboombass", 1.5f);
			}
			else if (getDistance(player.pos.x, player.pos.y, bomb.Get_BombPos().x, bomb.Get_BombPos().y) > WINSIZEY * 0.5f &&
				getDistance(player.pos.x, player.pos.y, bomb.Get_BombPos().x, bomb.Get_BombPos().y) < WINSIZEY)
			{
				SOUNDMANAGER->play("Player_Bomb_Kaboom", 0.8f);
				SOUNDMANAGER->play("Player_Bomb_Kaboombass", 0.8f);
			}
			else
			{
				SOUNDMANAGER->play("Player_Bomb_Kaboom", 0.3f);
				SOUNDMANAGER->play("Player_Bomb_Kaboombass", 0.3f);
			}
			bombEffectAnim->start();
			bombEffectPos = { (float)bomb.Get_BombArea().left, (float)bomb.Get_BombArea().top };
			bomb.Set_isThrow(false);
		}
	}
}

void Player::update_ChangeAnimation(const char* imgKeyName, const char* aniKeyName)
{
	player.img = IMAGEMANAGER->findImage(imgKeyName);
	player.ani = KEYANIMANAGER->findAnimation(aniKeyName);
	player.ani->start();
}

void Player::update_ChangeAni()
{
	if (!isPlayerExit && !player.isDead)
	{
		if (player.posState == Ground) // 위치 상태가 땅일때
		{
			if (!player.dir) // 왼쪽
			{
				if (player.aniState != playerAniStateTemp || player.dir != playerDirTemp || player.posState != playerPosStateTemp)
				{
					switch (player.aniState)
					{
					case Idle:
						update_ChangeAnimation("Player_Idle", "player_Idle_Left");
						break;
					case Move:
						update_ChangeAnimation("Player_Move", "player_Move_Left");
						break;
					case Crawl:
						update_ChangeAnimation("Player_Crawl", "player_Crawl_Left");
						break;
					case Hit:
						update_ChangeAnimation("Player_Knockback", "player_Knockback_Left");
						break;
					default:
						break;
					}
					playerAniStateTemp = player.aniState;
					playerDirTemp = player.dir;
					playerPosStateTemp = player.posState;
				}
			}
			else if (player.dir) // 오른쪽
			{
				if (player.aniState != playerAniStateTemp || player.dir != playerDirTemp || player.posState != playerPosStateTemp)
				{
					switch (player.aniState)
					{
					case Idle:
						update_ChangeAnimation("Player_Idle", "player_Idle_Right");
						break;
					case Move:
						update_ChangeAnimation("Player_Move", "player_Move_Right");
						break;
					case Crawl:
						update_ChangeAnimation("Player_Crawl", "player_Crawl_Right");
						break;
					case Hit:
						update_ChangeAnimation("Player_Knockback", "player_Knockback_Right");
						break;
					default:
						break;
					}
					playerAniStateTemp = player.aniState;
					playerDirTemp = player.dir;
					playerPosStateTemp = player.posState;
				}
			}
		}
		else // 위치 상태가 공중일때
		{
			if (player.aniState != Hit && player.aniState != Hanging)
			{
				if (!player.dir) // 왼쪽
				{
					if (player.dir != playerDirTemp)
					{
						update_ChangeAnimation("Player_Air", "player_Air_Left");
						playerDirTemp = player.dir;
					}
				}
				else // 오른쪽
				{
					if (player.dir != playerDirTemp)
					{
						update_ChangeAnimation("Player_Air", "player_Air_Right");
						playerDirTemp = player.dir;
					}
				}
			}
		}
		if (player.posState != playerPosStateTemp) // 위치 상태가 기존 위치 상태와 달라질 경우
		{
			if (player.aniState != Hit)
			{
				switch (player.posState)
				{
				case Ground:
					if (!player.dir)
					{
						update_ChangeAnimation("Player_Idle", "player_Idle_Left");
					}
					else
					{
						update_ChangeAnimation("Player_Idle", "player_Idle_Right");
					}
					break;
				case Air:
					if (!player.dir)
					{
						update_ChangeAnimation("Player_Air", "player_Air_Left");
					}
					else
					{
						update_ChangeAnimation("Player_Air", "player_Air_Right");
					}
					break;
				default:
					break;
				}
				playerPosStateTemp = player.posState;
			}
		}
	}

	player.ani->frameUpdate(TIMEMANAGER->getElapsedTime() * 10);
	if (bombEffectPos.x != -1 && bombEffectPos.y != -1)
	{
		bombEffectAnim->frameUpdate(TIMEMANAGER->getElapsedTime() * 20);
	}
	if (bomb.Get_isThrow())
	{
		bomb.update();
	}
}

void Player::render_Debug(HDC getMemDC)
{
	char playerPosState[128];
	sprintf_s(playerPosState, sizeof(playerPosState), "playerPosState : %d", player.posState);
	TextOut(getMemDC, 50, 530, playerPosState, strlen(playerPosState));

	char playerState[128];
	sprintf_s(playerPosState, sizeof(playerPosState), "playerState : %d", player.aniState);
	TextOut(getMemDC, 50, 550, playerPosState, strlen(playerPosState));

	char playerDir[128];
	sprintf_s(playerDir, sizeof(playerDir), "playerDir : %d", player.dir);
	TextOut(getMemDC, 50, 570, playerDir, strlen(playerDir));

	char playerSpeed[128];
	sprintf_s(playerSpeed, sizeof(playerSpeed), " _boom.get_boomCount : %f", bomb.Get_BombTimer());
	TextOut(getMemDC, 50, 590, playerSpeed, strlen(playerSpeed));

	char playerID_X[128];
	sprintf_s(playerID_X, sizeof(playerID_X), "playerID_X : %d", player.id.x);
	TextOut(getMemDC, 50, 610, playerID_X, strlen(playerID_X));

	char playerID_Y[128];
	sprintf_s(playerID_Y, sizeof(playerID_Y), "playerID_Y : %d", player.id.y);
	TextOut(getMemDC, 50, 630, playerID_Y, strlen(playerID_Y));

	char _testing[128];
	sprintf_s(_testing, sizeof(_testing), "playerHP : %d", player.hp);
	TextOut(getMemDC, 50, 650, _testing, strlen(_testing));
}