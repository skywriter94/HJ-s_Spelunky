#pragma once
#include <cmath>
#include "gameNode.h"


// ������
#define PI 3.14f
#define PI2 PI * 2.0f

// Ÿ�� ������
#define TILESIZE 64

#define TILEX 64
#define TILEY 64
#define PALETTE_TILE_X 6
#define PALETTE_TILE_Y 4

#define PALETTE_BG_SIZE 64
#define PALETTE_BGTILE_X 2
#define PALETTE_BGTILE_Y 2

// ���� ����Ʈ ���� ������
#define MAX_RALLYPOINT 36
#define MAX_SELECTED_RALLYPOINT 12

// ������ ������ XY ���� ũ��
#define SHUFFLEAREA_X (TILESIZE * TILEX) / ((TILEX / 32) * 10)
#define SHUFFLEAREA_Y (TILESIZE * TILEY) / ((TILEY / 32) * 10)

// ���漱��
class Player;
class EnemyManager;



class MapGenerator : public gameNode
{
private: // ū �б� �帧�� �����ϴ� ���� �Ǵ� �Լ���
	bool isRandomMapInit;

private: // �� ������ �ʿ��� ����ü
	enum RandomMapGeneSequence
	{
		Tiling, Shuffle, MakeSpace, MakingWays, MakeEntrance, SetObj, TileOutlineDeco, Done, InGame
	};
	struct OutsideTile
	{
		bool L, T, R, B;
	};

	struct Tile
	{
		POINTFLOAT pos;
		RECT rc;
		POINT tileFrameID;
		OutsideTile outsideTiling;
		bool bgTile;
	};

	struct RallyPoint
	{
		POINTFLOAT pos;
		POINT selectTileID;
		RECT rc;
		float speed;
		float angle;
		float radius;
		bool isSelected;
	};

	struct RandomMapGeneSupport
	{
		POINTFLOAT minPos;
		POINTFLOAT maxPos;
		POINT entranceTileID;
		POINT exitTileID;
		float minimap_lengthX() { return maxPos.x - minPos.x; }
		float minimap_lengthY() { return maxPos.y - minPos.y; }
		RECT exitRect;
		RECT Get_ExitRect() { return exitRect; }
	};

private: // �� ������ �ʿ��� ������ & ��Ÿ �ʿ��� ����
	RandomMapGeneSequence randMapGeneSequence;
	RandomMapGeneSupport randMapGeneSupport;

	Tile tile[TILEY][TILEX];

	float rallyPointShuffleTime;
	int countSelectedRallyPoint;
	RallyPoint rallyPoint[MAX_RALLYPOINT];
	RallyPoint selectedRallyPoint[MAX_SELECTED_RALLYPOINT];

private:
	Player* _player; // ��Ÿ ����
	EnemyManager* _enemyManager;

private: // ���� �Լ���
	void init_SetObj();
	void init_AddImages();
	void init_SetTiles();
	void init_ResetRallyPointShuffle();

	void update_BuildRandMap();
	void update_RallyPointShuffle();
	void update_MakeSpaceFromRallyPoint();
	void update_ConnectWays();
	void update_MakeEntrance();
	void update_SetObj();
	void update_TileOutlineDeco();

	void render_Debug(HDC getMemDC);

public:
	MapGenerator();
	~MapGenerator();

	HRESULT init();
	void release();
	void update();
	void render();

public: // �ܺη� �������� �Լ���
	int Get_RandomMapGeneSequence() { return randMapGeneSequence; }
	RandomMapGeneSupport Get_RandomMapGeneSupport() { return randMapGeneSupport; }
	Tile* Get_Tile(int y, int x) { return &tile[y][x]; }
	POINTFLOAT Get_Tile_Pos(int y, int x) { return tile[y][x].pos; }
	POINTFLOAT Get_Tile_EntranceTilePos() { return tile[randMapGeneSupport.entranceTileID.y][randMapGeneSupport.entranceTileID.x].pos; }
	POINTFLOAT Get_Tile_ExitTilePos() { return tile[randMapGeneSupport.exitTileID.y][randMapGeneSupport.exitTileID.x].pos; }

	void SetPlayerAddressLink(Player* player) { _player = player; }
	void SetEnemyManagerAddressLink(EnemyManager* enemyManager) { _enemyManager = enemyManager; }
};

/* �� �� ��ȹ - �ʿ��� ������ �Ѵ�!

����� ��Ģ : �Ա��� �ⱸ�� ���� �ϳ��� ����� ������ �� ���� ������ ����� ���Ѵ�.
���� ���� ���۵ǰ� ��� ���� ��ġ�ϰ� ������ �� �� �ϳ��� ������ �� �ְ� �Ѵ�.


1. ����� ���� ��
- �Ϲ� Ÿ�ϸ�ó�� �� �� �ְ� �ý����� �����Ѵ�. ����, ������Ʈ, ���� ����� �÷��̾ ���� ��ġ�� �� �ְ� ���ش�.


2. ���� ��
- ������ ���� �� ������ �� �� �ֵ��� ���ش�.

<���� 1> �ʹ� LTRB ��� �������� ���� �ʰ� �Ѿ� �߻� ��ġ�� �����ְ� �߻� ����, �Ѿ� �߻����� ������ ũ��, �ӵ�, �������� �浹 ����,
		 �ӵ� ���� ����, �� �ùķ��̼� �� �ð��� �������� �ش�. (�Ϸ�)
<���� 2> �� �ùķ��̼��� ���߸� ���� �� �̻� �������� �ʰ� ���� �� ���� �������� �����Ѵ�. (�Ϸ�)
<���� 3> ���� ���� �� �ε��� ��ȣ�������� ���õ� ���������� ���� ���õ� ������ ���� ����� ������ ������ �����ش�. �׿� ���ÿ� x�� y��Ҹ�
		 ������ ����� ������ �������ش�. (�Ϸ�)
<���� 4> ����Ǿ� ������ ���� �κ��� �����ϰ� ���� Ÿ���� �� �ɾ��ش�.(�Ϸ�) ���� ���� ������ ���� �޹����� ��� Ÿ�� �ɱ�(�Ϸ�) -> Ÿ�� �ɱ�(�Ϸ�) -> �ⱸ, �Ա� ��ġ -> ������Ʈ
		 �� ���� �ɱ�, Ÿ���� �ٹҿ�� ��� �ɱ� -> ���� ��� �ɱ� -> Ÿ�� ����ó��(�Ϸ�) -> ���� -> ĳ���ͼ����� �������ش�.
<���� 5> ����Ǿ� �ִ�(���õ�) ��ҵ� �߿� 0,0�� ���� ����� ��ҿ� winsizeXY ��Ҷ� ���� ����� ��Ҹ� ���� �ϳ��� ��Ƽ� �ⱸ�� �Ա��� �����Ѵ�.
*/

/*
����� �� ���� ����(������ ������)

�̸� ����� ���� Ʋ�� �غ��Ѵ�.

�ε����� ���� x, y��� 0�̶� �ִ� �ε�������
*/