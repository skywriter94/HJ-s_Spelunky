#pragma once
#include "gameNode.h"
#include "Bomb.h"

#define GRAVITY 0.2f
#define PLAYERSPEED 5.0f
#define PLAYERJUMPPOWER 10.0f
#define PLAYERKNOCKBACKSPEED 10.0f
#define KNOCKBACK_R 0.1f

class MapGenerator;
class EnemyManager;
class Sounds;



class Player : public gameNode
{
private: // 플레이어 상태 & 구조체
	enum PlayerPosState
	{
		Air,
		Ground
	};
	enum PlayerAniState
	{
		Idle,
		Move,
		Crawl,
		Hit,
		Hanging
	};
	struct KnockBack
	{
		float speed;
		bool dir;
	};

	struct ThePlayer
	{
		PlayerPosState posState;
		PlayerAniState aniState;
		POINTFLOAT pos;
		POINT id;
		float speed;
		float gravity;
		float jumpPower;
		KnockBack knockBack;
		int hp;
		RECT rc;
		bool dir;
		image* img;
		animation* ani;
		float takeHitDelay;
		bool hitSwitch;
		bool isDead;
	};

private: // 내부 변수
	ThePlayer player;
	MapGenerator* _mapGenerator;
	EnemyManager* _enemyManager;
	bool playerDirTemp;
	PlayerPosState playerPosStateTemp;
	PlayerAniState playerAniStateTemp;
	bool isPlayerExit;
	bool isLevelClear;
	int exitAlpha;

	Bomb bomb;
	image* bombEffectImg;
	POINTFLOAT bombEffectPos;
	animation* bombEffectAnim;

private: // 내부 함수
	void init_AddImgAndAni();
	void init_AddItemAndAni();

	void update_PlayerPosState();
	void update_Movement();
	void update_TileCollision();
	void update_EnemyAndObjCollision();
	void update_ChangeAnimation(const char* imgKeyName, const char* aniKeyName);
	void update_ChangeAni();

	void render_Debug(HDC getMemDC);

public:
	HRESULT init(int _posState, int _state, float _x, float _y, int _hp);
	void release();
	void update();
	void render();

public: // 외부에 전달해줄 주요 함수
	ThePlayer Get_Player() { return player; }
	void Set_Player(ThePlayer NewPlayerState) { player = NewPlayerState; }

	POINTFLOAT Get_PlayerPos() { return { player.pos.x, player.pos.y }; }
	void Set_PlayerPos(float posX, float posY) { player.pos = { posX, posY }; }

	RECT Get_playerRect() { return player.rc; }
	void Set_PlayerRect(float posX, float posY, float radius) { player.rc = RectMakeCenter(posX, posY, radius, radius); }

	float Get_playerJumpPower() { return player.jumpPower; }
	void Set_playerJumpPower(float _jumpPower) { player.jumpPower = _jumpPower; }

	float Get_playerGravity() { return player.gravity; }
	void Set_playerGravity(float _gravity) { player.gravity = _gravity; }

	bool Get_PlayerIsDead() { return player.isDead; }
	bool Get_PlayerLevelClear() { return isLevelClear; }

	int Get_PlayerHP() { return player.hp; }

	void SetMapGeneratorAddressLink(MapGenerator* mapGenerator) { _mapGenerator = mapGenerator; }
	void SetEnemyManagerAddressLink(EnemyManager* enemyManager) { _enemyManager = enemyManager; }
};