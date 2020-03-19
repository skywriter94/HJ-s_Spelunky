#pragma once
#include <cmath>
#include "gameNode.h"


// 원주율
#define PI 3.14f
#define PI2 PI * 2.0f

// 타일 사이즈
#define TILESIZE 64

#define TILEX 64
#define TILEY 64
#define PALETTE_TILE_X 6
#define PALETTE_TILE_Y 4

#define PALETTE_BG_SIZE 64
#define PALETTE_BGTILE_X 2
#define PALETTE_BGTILE_Y 2

// 랠리 포인트 관련 디파인
#define MAX_RALLYPOINT 36
#define MAX_SELECTED_RALLYPOINT 12

// 셔플할 영역의 XY 영역 크기
#define SHUFFLEAREA_X (TILESIZE * TILEX) / ((TILEX / 32) * 10)
#define SHUFFLEAREA_Y (TILESIZE * TILEY) / ((TILEY / 32) * 10)

// 전방선언
class Player;
class EnemyManager;



class MapGenerator : public gameNode
{
private: // 큰 분기 흐름을 제어하는 변수 또는 함수들
	bool isRandomMapInit;

private: // 맵 생성에 필요한 구조체
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

private: // 맵 생성에 필요한 변수들 & 기타 필요한 벡터
	RandomMapGeneSequence randMapGeneSequence;
	RandomMapGeneSupport randMapGeneSupport;

	Tile tile[TILEY][TILEX];

	float rallyPointShuffleTime;
	int countSelectedRallyPoint;
	RallyPoint rallyPoint[MAX_RALLYPOINT];
	RallyPoint selectedRallyPoint[MAX_SELECTED_RALLYPOINT];

private:
	Player* _player; // 기타 변수
	EnemyManager* _enemyManager;

private: // 내부 함수들
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

public: // 외부로 전달해줄 함수들
	int Get_RandomMapGeneSequence() { return randMapGeneSequence; }
	RandomMapGeneSupport Get_RandomMapGeneSupport() { return randMapGeneSupport; }
	Tile* Get_Tile(int y, int x) { return &tile[y][x]; }
	POINTFLOAT Get_Tile_Pos(int y, int x) { return tile[y][x].pos; }
	POINTFLOAT Get_Tile_EntranceTilePos() { return tile[randMapGeneSupport.entranceTileID.y][randMapGeneSupport.entranceTileID.x].pos; }
	POINTFLOAT Get_Tile_ExitTilePos() { return tile[randMapGeneSupport.exitTileID.y][randMapGeneSupport.exitTileID.x].pos; }

	void SetPlayerAddressLink(Player* player) { _player = player; }
	void SetEnemyManagerAddressLink(EnemyManager* enemyManager) { _enemyManager = enemyManager; }
};

/* 맵 툴 계획 - 맵에서 선택을 한다!

공통된 규칙 : 입구와 출구를 각각 하나씩 만들기 전까진 맵 제작 씬에서 벗어나지 못한다.
먼저 씬이 시작되고 어떻게 맵을 배치하고 싶은지 둘 중 하나를 선택할 수 있게 한다.


1. 사용자 정의 맵
- 일반 타일맵처럼 찍어낼 수 있게 시스템을 구성한다. 지형, 오브젝트, 함정 등등을 플레이어가 직접 설치할 수 있게 해준다.


2. 랜덤 맵
- 절차적 랜덤 맵 생성을 할 수 있도록 해준다.

<순서 1> 너무 LTRB 어느 한쪽으로 쏠리지 않게 총알 발사 위치를 정해주고 발사 개수, 총알 발사율과 반지름 크기, 속도, 공끼리의 충돌 여부,
		 속도 감속 여부, 공 시뮬레이션 총 시간을 랜덤으로 준다. (완료)
<순서 2> 공 시뮬레이션이 멈추면 공이 더 이상 움직이지 않게 고정 후 공을 랜덤으로 선택한다. (완료)
<순서 3> 가장 낮은 순 인덱스 번호에서부터 선택된 공에서부터 다음 선택된 공까지 가장 가까운 순으로 연결을 시켜준다. 그와 동시에 x와 y요소를
		 나누어 가까운 순으로 연결해준다. (완료)
<순서 4> 연결되어 공간이 생긴 부분을 제외하고 먼저 타일을 다 심어준다.(완료) 먼저 렌더 순서는 가장 뒷배경부터 배경 타일 심기(완료) -> 타일 심기(완료) -> 출구, 입구 배치 -> 오브젝트
		 및 함정 심기, 타일의 꾸밈요소 등등 심기 -> 무기 등등 심기 -> 타일 마감처리(완료) -> 몬스터 -> 캐릭터순으로 지정해준다.
<순서 5> 연결되어 있는(선택된) 요소들 중에 0,0과 가장 가까운 요소와 winsizeXY 요소랑 가장 가까운 요소를 각각 하나씩 잡아서 출구와 입구로 지정한다.
*/

/*
변경된 맵 제작 절차(아직은 검토중)

미리 만들어 놓은 틀을 준비한다.

인덱스에 각각 x, y요소 0이랑 최대 인덱스에선
*/