#include "stdafx.h"
#include "MapGenerator.h"
#include "Player.h"
#include "EnemyManager.h"


MapGenerator::MapGenerator()
{
}

MapGenerator::~MapGenerator()
{
}



HRESULT MapGenerator::init()
{
	init_SetObj();
	init_AddImages();

	if (isRandomMapInit) // ���� �� ����� ���
	{
		init_ResetRallyPointShuffle();
		randMapGeneSequence = Tiling;
		if (randMapGeneSequence == Tiling)
		{
			init_SetTiles();
		}
	}
	else // ���� �� ��尡 �ƴ� ���
	{

	}
	return S_OK;
}

void MapGenerator::release()
{
	SAFE_DELETE(_player);
	SAFE_DELETE(_enemyManager);
}

void MapGenerator::update()
{
	CAMERAMANAGER->update();
	if (isRandomMapInit)
	{
		update_BuildRandMap();
	}
	else
	{
	
	}
}

void MapGenerator::render()
{
	RECT tempRC;

	if ((int)randMapGeneSequence >= (int)Done)
	{
		for (int i = CAMERAMANAGER->get_currentRenderTileID_LT().y; i <= CAMERAMANAGER->get_currentRenderTileID_RB().y; i++) // ��Ʈ ����� ���� Ÿ��
		{
			for (int j = CAMERAMANAGER->get_currentRenderTileID_LT().x; j <= CAMERAMANAGER->get_currentRenderTileID_RB().x; j++)
			{
				if (tile[i][j].bgTile)
				{
					CAMERAMANAGER->render_Camera_ImageRender(tile[i][j].rc, "BG_default", getMemDC(), { (float)tile[i][j].rc.left - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2), (float)tile[i][j].rc.top - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2) });
				}
				else
				{
					CAMERAMANAGER->render_Camera_FrameRender(tile[i][j].rc, "Palette_Tile", getMemDC(), { (float)tile[i][j].rc.left - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2), (float)tile[i][j].rc.top - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2) }, { tile[i][j].tileFrameID.x, tile[i][j].tileFrameID.y });
				}
			}
		}

		CAMERAMANAGER->render_Camera_ImageRender( // �Ա� 
			"Entrance",
			getMemDC(),
			{ (float)tile[randMapGeneSupport.entranceTileID.y][randMapGeneSupport.entranceTileID.x].rc.left - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2),
			(float)tile[randMapGeneSupport.entranceTileID.y][randMapGeneSupport.entranceTileID.x].rc.top - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2) });
		CAMERAMANAGER->render_Camera_ImageRender( // �ⱸ
			"Entrance",
			getMemDC(),
			{ (float)tile[randMapGeneSupport.exitTileID.y][randMapGeneSupport.exitTileID.x].rc.left - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2),
			(float)tile[randMapGeneSupport.exitTileID.y][randMapGeneSupport.exitTileID.x].rc.top - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2) });
		//Rectangle(getMemDC(), randMapGeneSupport.exitRect.left - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2), randMapGeneSupport.exitRect.top - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2), // �ⱸ ��Ʈ ����
		//					randMapGeneSupport.exitRect.right - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2), randMapGeneSupport.exitRect.bottom - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2));

		for (int i = CAMERAMANAGER->get_currentRenderTileID_LT().y; i <= CAMERAMANAGER->get_currentRenderTileID_RB().y; i++) // Ÿ�� ����ó��
		{
			for (int j = CAMERAMANAGER->get_currentRenderTileID_LT().x; j <= CAMERAMANAGER->get_currentRenderTileID_RB().x; j++)
			{
				if (!tile[i][j].bgTile)
				{
					if (i != TILEY - 1)
					{
						if (tile[i][j].outsideTiling.L) // ����
						{
							CAMERAMANAGER->render_Camera_FrameRender(tile[i][j].rc, "Palette_Tile", getMemDC(), { (float)tile[i][j].rc.left - (TILESIZE / 2) + 10 - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2), (float)tile[i][j].rc.top - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2) }, { 2, 2 });
						}
						if (tile[i][j].outsideTiling.R) // ������
						{
							CAMERAMANAGER->render_Camera_FrameRender(tile[i][j].rc, "Palette_Tile", getMemDC(), { (float)tile[i][j].rc.left + (TILESIZE / 2) - 10 - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2), (float)tile[i][j].rc.top - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2) }, { 3, 2 });
						}
						if (tile[i][j].outsideTiling.T) // ����
						{
							CAMERAMANAGER->render_Camera_FrameRender(tile[i][j].rc, "Palette_Tile", getMemDC(), { (float)tile[i][j].rc.left - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2), (float)tile[i][j].rc.top - (TILESIZE / 2) - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2) }, { 0, 3 });
						}
						if (tile[i][j].outsideTiling.B) // �Ʒ���
						{
							CAMERAMANAGER->render_Camera_FrameRender(tile[i][j].rc, "Palette_Tile", getMemDC(), { (float)tile[i][j].rc.left - CAMERAMANAGER->get_CameraPos().x + (WINSIZEX / 2), (float)tile[i][j].rc.top + (TILESIZE / 2) - CAMERAMANAGER->get_CameraPos().y + (WINSIZEY / 2) }, { 1, 2 });
						}
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	
	//for (int i = 0; i < MAX_RALLYPOINT; i++) // �Ϲ� ��������Ʈ ������
	//{
	//	Ellipse(getMemDC(), rallyPoint[i].rc);
	//}
	//if ((int)randMapGeneSequence > (int)Shuffle) // ���õ� ��������Ʈ ������
	//{
	//	for (int i = 0; i < MAX_SELECTED_RALLYPOINT; i++)
	//	{
	//		HBRUSH oldbrush, brush;
	//		brush = CreateSolidBrush(RGB(255, 0, 0));
	//		oldbrush = (HBRUSH)SelectObject(getMemDC(), brush);
	//		Ellipse(getMemDC(), selectedRallyPoint[i].rc);
	//		SelectObject(getMemDC(), oldbrush);
	//		DeleteObject(brush);
	//
	//		if (i < MAX_SELECTED_RALLYPOINT - 1)
	//		{
	//			LineMake(getMemDC(), selectedRallyPoint[i].pos.x, selectedRallyPoint[i].pos.y, selectedRallyPoint[i + 1].pos.x, selectedRallyPoint[i + 1].pos.y);
	//		}
	//		else
	//		{
	//			LineMake(getMemDC(), selectedRallyPoint[i].pos.x, selectedRallyPoint[i].pos.y, selectedRallyPoint[0].pos.x, selectedRallyPoint[0].pos.y);
	//		}
	//	}
	//}
	//render_Debug(getMemDC());
}



void MapGenerator::init_SetObj()
{
	isRandomMapInit = true;
	CAMERAMANAGER->init({0 , 0}, { TILESIZE * TILEX, TILESIZE * TILEY }, { WINSIZEX , WINSIZEY });

	randMapGeneSupport.maxPos = randMapGeneSupport.minPos = { 0, 0 };
	randMapGeneSupport.entranceTileID = randMapGeneSupport.exitTileID = { 0, 0 };
}

void MapGenerator::init_AddImages()
{
	// ����Ʈ �� ��� �̹��� �߰�
	IMAGEMANAGER->addImage("BG_default", "Image/Tile/Palette/BG_default.bmp", 64, 64, false, RGB(255, 255, 255));

	// ���׶��� Ÿ�� �̹��� �߰�
	IMAGEMANAGER->addFrameImage("Palette_BG_Tile", "Image/Tile/Palette/BG_Tile.bmp", 128, 128, PALETTE_BGTILE_X, PALETTE_BGTILE_Y, true, RGB(255, 0, 255));

	// �⺻ Ÿ�� �̹��� �߰�
	IMAGEMANAGER->addFrameImage("Palette_Tile", "Image/Tile/Palette/Tile.bmp", 384, 256, PALETTE_TILE_X, PALETTE_TILE_Y, true, RGB(255, 0, 255));

	// �ⱸ �Ա� �̹��� �߰�
	IMAGEMANAGER->addImage("Entrance", "Image/Tile/Palette/Entrance.bmp", 256, 256, true, RGB(255, 0, 255));
}

void MapGenerator::init_SetTiles()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			tile[i][j].pos = { (float)TILESIZE * j + (TILESIZE / 2) , (float)TILESIZE * i + (TILESIZE / 2) };
			tile[i][j].rc = RectMakeCenter(tile[i][j].pos.x, tile[i][j].pos.y, TILESIZE, TILESIZE);
			if (i != TILEY - 1) tile[i][j].tileFrameID = { RND->getInt(6), 0 };
			else tile[i][j].tileFrameID = { 0, 1 };
			tile[i][j].outsideTiling.L = tile[i][j].outsideTiling.T = tile[i][j].outsideTiling.R = tile[i][j].outsideTiling.B = 0;
			tile[i][j].bgTile = false;
		}
	}
	randMapGeneSequence = Shuffle;
}

void MapGenerator::init_ResetRallyPointShuffle()
{
	for (int i = 0; i < MAX_RALLYPOINT; i++)
	{
		rallyPointShuffleTime = RND->getFromFloatTo(4.0f, 8.0f);
		countSelectedRallyPoint = 0;
		for (int i = 0; i < MAX_RALLYPOINT; i++)
		{
			rallyPoint[i].radius = { RND->getFromFloatTo(4.0f, 10.0f) };
			rallyPoint[i].pos = { RND->getFromFloatTo(rallyPoint[i].radius, SHUFFLEAREA_X), RND->getFromFloatTo( rallyPoint[i].radius, SHUFFLEAREA_Y) };
			rallyPoint[i].speed = RND->getFromFloatTo(0.5f, 4.0f);
			rallyPoint[i].angle = PI2 / RND->getFromFloatTo(1.0f, 4.0f);
			rallyPoint[i].isSelected = false;
			rallyPoint[i].rc = RectMakeCenter(rallyPoint[i].pos.x, rallyPoint[i].pos.y, rallyPoint[i].radius * 2, rallyPoint[i].radius * 2);
		}
	}
}

void MapGenerator::update_BuildRandMap()
{
	switch (randMapGeneSequence)
	{
		case MapGenerator::Shuffle:
			update_RallyPointShuffle();
			break;
		case MapGenerator::MakeSpace:
			update_MakeSpaceFromRallyPoint();
			break;
		case MapGenerator::MakingWays:
			update_ConnectWays();
			break;
		case MapGenerator::MakeEntrance:
			update_MakeEntrance();
			break;
		case MapGenerator::SetObj:
			update_SetObj();
			break;
		case MapGenerator::TileOutlineDeco:
			update_TileOutlineDeco();
			break;
		case MapGenerator::Done:
			randMapGeneSequence = InGame;
			break;
		case MapGenerator::InGame:
		default:
		break;
	}
}

void MapGenerator::update_RallyPointShuffle()
{
	TIMEMANAGER->setLoadingTimeSwitch(true);
	if (TIMEMANAGER->getLoadingTime() < rallyPointShuffleTime)
	{
		for (int i = 0; i < MAX_RALLYPOINT; i++) // �� ���� (�� ��ȣ �ۿ� ���� �̴�� �ݵ�� �����ϱ�)
		{
			for (int j = i + 1; j < MAX_RALLYPOINT; j++) // �� ���� �浹
			{
				if (rallyPoint[i].radius * 2 < getDistance(rallyPoint[i].pos.x, rallyPoint[i].pos.y, rallyPoint[j].pos.x, rallyPoint[j].pos.y)) continue;
				rallyPoint[i].angle = getAngle(rallyPoint[j].pos.x, rallyPoint[j].pos.y, rallyPoint[i].pos.x, rallyPoint[i].pos.y);
				rallyPoint[j].angle = getAngle(rallyPoint[i].pos.x, rallyPoint[i].pos.y, rallyPoint[j].pos.x, rallyPoint[j].pos.y);
			}

			if (rallyPoint[i].pos.x - rallyPoint[i].radius < 0) // ���� ���� ��������
			{
				rallyPoint[i].pos.x = rallyPoint[i].radius;
				rallyPoint[i].angle = PI - rallyPoint[i].angle;
			}
			if (rallyPoint[i].pos.x + rallyPoint[i].radius > SHUFFLEAREA_X) // ������ ���� ��������
			{
				rallyPoint[i].pos.x = SHUFFLEAREA_X - rallyPoint[i].radius;
				rallyPoint[i].angle = PI - rallyPoint[i].angle;
			}
			if (rallyPoint[i].pos.y - rallyPoint[i].radius < 0) // ���� ���� ��������
			{
				rallyPoint[i].pos.y = rallyPoint[i].radius;
				rallyPoint[i].angle = 2 * PI - rallyPoint[i].angle;
			}
			if (rallyPoint[i].pos.y + rallyPoint[i].radius > SHUFFLEAREA_Y) // �Ʒ������� ������
			{
				rallyPoint[i].pos.y = SHUFFLEAREA_Y - rallyPoint[i].radius;
				rallyPoint[i].angle = 2 * PI - rallyPoint[i].angle;
			}
			rallyPoint[i].pos.x += cosf(rallyPoint[i].angle) * rallyPoint[i].speed;
			rallyPoint[i].pos.y += -sinf(rallyPoint[i].angle) * rallyPoint[i].speed;
			rallyPoint[i].rc = RectMakeCenter(rallyPoint[i].pos.x, rallyPoint[i].pos.y, rallyPoint[i].radius * 2, rallyPoint[i].radius * 2);
		}
		randMapGeneSupport.minPos = { rallyPoint[0].pos.x , rallyPoint[0].pos.y }; // ���߿� ��Һ񱳸� �ϱ� ���� �ʱⰪ�� ������ �־���
		randMapGeneSupport.maxPos = { rallyPoint[0].pos.x , rallyPoint[0].pos.y };
	}
	else
	{
		if (countSelectedRallyPoint < MAX_SELECTED_RALLYPOINT)
		{
			for (int i = 0; i < MAX_RALLYPOINT; i++)
			{
				if (rallyPoint[i].isSelected) continue;
				rallyPoint[i].isSelected = RND->getInt(2);

				if (rallyPoint[i].isSelected != 0) // ���õ� ���̶��
				{
					selectedRallyPoint[countSelectedRallyPoint] = rallyPoint[i];
					if (randMapGeneSupport.minPos.x >= selectedRallyPoint[countSelectedRallyPoint].pos.x)
						randMapGeneSupport.minPos.x = selectedRallyPoint[countSelectedRallyPoint].pos.x;
					if (randMapGeneSupport.minPos.y >= selectedRallyPoint[countSelectedRallyPoint].pos.y)
						randMapGeneSupport.minPos.y = selectedRallyPoint[countSelectedRallyPoint].pos.y;
					if (randMapGeneSupport.maxPos.x <= selectedRallyPoint[countSelectedRallyPoint].pos.x)
						randMapGeneSupport.maxPos.x = selectedRallyPoint[countSelectedRallyPoint].pos.x;
					if (randMapGeneSupport.maxPos.y <= selectedRallyPoint[countSelectedRallyPoint].pos.y)
						randMapGeneSupport.maxPos.y = selectedRallyPoint[countSelectedRallyPoint].pos.y;
					countSelectedRallyPoint++;
					break;
				}
			}
		}
		else
		{
			TIMEMANAGER->setLoadingTimeSwitch(false);
			TIMEMANAGER->setLoadingTime(0);
			randMapGeneSequence = MakeSpace;
		}
	}
}

void MapGenerator::update_MakeSpaceFromRallyPoint()
{
	for (int i = 0; i < MAX_SELECTED_RALLYPOINT; i++)
	{	//											�� ���� ���� ��												���ϱ�(*)							������(/)
		// ��� ���� -> ((���õ� ���� x��ǥ - ������� �̴ϸʿ��� ���� ���� x��ǥ) / ������� �̴ϸ��� �� X����))					���� �� Ÿ���� X����					�� Ÿ�� ������		==		�ش� Ÿ���� �ε��� ��
		int new_idX = ((float)(selectedRallyPoint[i].pos.x - randMapGeneSupport.minPos.x) / randMapGeneSupport.minimap_lengthX()) * (float)(TILEX * TILESIZE) / (float)TILESIZE;
		int new_idY = ((float)(selectedRallyPoint[i].pos.y - randMapGeneSupport.minPos.y) / randMapGeneSupport.minimap_lengthY()) * (float)(TILEY * TILESIZE) / (float)TILESIZE;
	
		if (new_idX < 0) new_idX = 0;
		else if (new_idX >= TILEX) new_idX = TILEX - 1; // �迭 �ʰ� ����
	
		if (new_idY < 0) new_idY = 0;
		else if (new_idY >= TILEY)  new_idY = TILEY - 1;
	
		selectedRallyPoint[i].selectTileID = { new_idX, new_idY };
		tile[selectedRallyPoint[i].selectTileID.y][selectedRallyPoint[i].selectTileID.x].bgTile = true;
	}
	randMapGeneSequence = MakingWays;
}

void MapGenerator::update_ConnectWays()
{
	//���� �� ����� (���� ��ġ ����)
	int countConnectRoad = 0;
	POINT currentID;
	bool isSearching = false;
	while (true)
	{
		if (!isSearching) // �˻��ϱ� ����
		{
			currentID = { selectedRallyPoint[countConnectRoad].selectTileID.x,  selectedRallyPoint[countConnectRoad].selectTileID.y }; // ���� ù ��° ���� ����� id�� ��
			isSearching = true;
		}
		else
		{
			if (countConnectRoad < MAX_SELECTED_RALLYPOINT - 1)
			{
				if (currentID.x > selectedRallyPoint[countConnectRoad + 1].selectTileID.x) currentID.x--; // currentID_XY�� �־��� ��ġ���� ��ǥ�������� x ���� Ŭ�� �����ϱ� ���� ����.
				else if (currentID.x < selectedRallyPoint[countConnectRoad + 1].selectTileID.x) currentID.x++;

				if (currentID.y > selectedRallyPoint[countConnectRoad + 1].selectTileID.y) currentID.y--; // currentID_XY�� �־��� ��ġ���� ��ǥ�������� y ���� Ŭ�� �����ϱ� ���� ����.
				else if (currentID.y < selectedRallyPoint[countConnectRoad + 1].selectTileID.y) currentID.y++;

				tile[currentID.y][currentID.x].bgTile = true; // �̵��� ��ŭ �� Ÿ�� �����

				if (currentID.x > 0) // ���� �� ��ĭ �����
					tile[currentID.y][currentID.x - 1].bgTile = true;
				if (currentID.x < TILEX - 1) // ������ �� ��ĭ �����
					tile[currentID.y][currentID.x + 1].bgTile = true;

				if(currentID.y > 0)	// ���� ��ĭ �����
					tile[currentID.y - 1][currentID.x].bgTile = true;
				if (currentID.y < TILEY - 1) // �Ʒ��� ��ĭ �����
					tile[currentID.y + 1][currentID.x].bgTile = true;


				if (currentID.x == selectedRallyPoint[countConnectRoad + 1].selectTileID.x && // �˻� ���� currentID_XY�� ��ǥ������ �����ϸ�
					currentID.y == selectedRallyPoint[countConnectRoad + 1].selectTileID.y)
				{
					countConnectRoad++;
					isSearching = false;
				}
			}
			else if (countConnectRoad == MAX_SELECTED_RALLYPOINT - 1)
			{
				if (currentID.x > selectedRallyPoint[0].selectTileID.x) currentID.x--; // currentID_XY�� �־��� ��ġ���� ��ǥ�������� x ���� Ŭ�� �����ϱ� ���� ����.
				else if (currentID.x < selectedRallyPoint[0].selectTileID.x) currentID.x++;

				if (currentID.y > selectedRallyPoint[0].selectTileID.y) currentID.y--; // currentID_XY�� �־��� ��ġ���� ��ǥ�������� y ���� Ŭ�� �����ϱ� ���� ����.
				else if (currentID.y < selectedRallyPoint[0].selectTileID.y) currentID.y++;

				tile[currentID.y][currentID.x].bgTile = true; // �̵��� ��ŭ �� Ÿ�� �����

				if (currentID.x > 0) // ���� �� ��ĭ �����
					tile[currentID.y][currentID.x - 1].bgTile = true;
				if (currentID.x < TILEX - 1) // ������ �� ��ĭ �����
					tile[currentID.y][currentID.x + 1].bgTile = true;

				if (currentID.y > 0) // ���� ��ĭ �����
					tile[currentID.y - 1][currentID.x].bgTile = true;
				if (currentID.y < TILEY - 1) // �Ʒ��� ��ĭ �����
					tile[currentID.y + 1][currentID.x].bgTile = true;

				if (currentID.x == selectedRallyPoint[0].selectTileID.x && // �˻� ���� currentID_XY�� ��ǥ������ �����ϸ�
					currentID.y == selectedRallyPoint[0].selectTileID.y)
				{
					countConnectRoad++;
					isSearching = false;
				}
			}
			else
			{
				randMapGeneSequence = MakeEntrance;
				break;
			}
		}
	}
}

void MapGenerator::update_MakeEntrance()
{
	float maxDistanceEntrance = getDistance((TILEX * TILESIZE) / 2, (TILEY * TILESIZE) / 2,
		tile[selectedRallyPoint[0].selectTileID.y][selectedRallyPoint[0].selectTileID.x].pos.x,
		tile[selectedRallyPoint[0].selectTileID.y][selectedRallyPoint[0].selectTileID.x].pos.y);

	for (int i = 0; i < MAX_SELECTED_RALLYPOINT; i++) // �Ա�(������) ������ ���� �ݺ���
	{
		if (getDistance((TILEX * TILESIZE) / 2, (TILEY * TILESIZE) / 2, // �� �������κ��� ���� ���� �ְ�, TILEY - 4�� TILEX - 3���� ���� idX, idY�� ������쿡 ��
			tile[selectedRallyPoint[i].selectTileID.y][selectedRallyPoint[i].selectTileID.x].pos.x,
			tile[selectedRallyPoint[i].selectTileID.y][selectedRallyPoint[i].selectTileID.x].pos.y) > maxDistanceEntrance &&
			selectedRallyPoint[i].selectTileID.y < TILEY - 3 && selectedRallyPoint[i].selectTileID.x < TILEX - 3) // �Ա� Ÿ���̹����� ���缭 Y - 4, X - 3�� ���ְ� �ִ�. (�迭 �� ���� ����)
		{
			maxDistanceEntrance = getDistance((TILEX * TILESIZE) / 2, (TILEY * TILESIZE) / 2,
				tile[selectedRallyPoint[i].selectTileID.y][selectedRallyPoint[i].selectTileID.x].pos.x,
				tile[selectedRallyPoint[i].selectTileID.y][selectedRallyPoint[i].selectTileID.x].pos.y);
			randMapGeneSupport.entranceTileID = { selectedRallyPoint[i].selectTileID.x , selectedRallyPoint[i].selectTileID.y }; // �Ա� Ÿ�� idx, idy�� ���� (Ÿ���� �迭 �����ϱ� ���ؼ�)
		}

		if (i == MAX_SELECTED_RALLYPOINT - 1) // i�� ������ �����϶�
		{
			for (int i = 0; i < 4; i++)
			{
				if (i == 3)
				{
					tile[randMapGeneSupport.entranceTileID.y + i][randMapGeneSupport.entranceTileID.x].bgTile = true;
					tile[randMapGeneSupport.entranceTileID.y + i][randMapGeneSupport.entranceTileID.x + 1].bgTile = false;
					tile[randMapGeneSupport.entranceTileID.y + i][randMapGeneSupport.entranceTileID.x + 1].tileFrameID = { 0, 1 };
					tile[randMapGeneSupport.entranceTileID.y + i][randMapGeneSupport.entranceTileID.x + 2].bgTile = false;
					tile[randMapGeneSupport.entranceTileID.y + i][randMapGeneSupport.entranceTileID.x + 2].tileFrameID = { 0, 1 };
					tile[randMapGeneSupport.entranceTileID.y + i][randMapGeneSupport.entranceTileID.x + 3].bgTile = true;
				}
				else
				{
					tile[randMapGeneSupport.entranceTileID.y + i][randMapGeneSupport.entranceTileID.x].bgTile = true;
					tile[randMapGeneSupport.entranceTileID.y + i][randMapGeneSupport.entranceTileID.x + 1].bgTile = true;
					tile[randMapGeneSupport.entranceTileID.y + i][randMapGeneSupport.entranceTileID.x + 2].bgTile = true;
					tile[randMapGeneSupport.entranceTileID.y + i][randMapGeneSupport.entranceTileID.x + 3].bgTile = true;
				}
			}
		}
	}

	float maxDistanceExit = getDistance(tile[randMapGeneSupport.entranceTileID.y][randMapGeneSupport.entranceTileID.x].pos.x,
		tile[randMapGeneSupport.entranceTileID.y][randMapGeneSupport.entranceTileID.x].pos.y,
		tile[selectedRallyPoint[0].selectTileID.y][selectedRallyPoint[0].selectTileID.x].pos.x,
		tile[selectedRallyPoint[0].selectTileID.y][selectedRallyPoint[0].selectTileID.x].pos.y);

	if (randMapGeneSupport.exitTileID.x != randMapGeneSupport.entranceTileID.x &&
		randMapGeneSupport.exitTileID.y != randMapGeneSupport.entranceTileID.y)
	{
		randMapGeneSupport.exitTileID = selectedRallyPoint[0].selectTileID;
	}

	for (int i = 0; i < MAX_SELECTED_RALLYPOINT; i++)
	{
		if (maxDistanceExit < getDistance(tile[randMapGeneSupport.entranceTileID.y][randMapGeneSupport.entranceTileID.x].pos.x,
			tile[randMapGeneSupport.entranceTileID.y][randMapGeneSupport.entranceTileID.x].pos.y,
			tile[selectedRallyPoint[i].selectTileID.y][selectedRallyPoint[i].selectTileID.x].pos.x,
			tile[selectedRallyPoint[i].selectTileID.y][selectedRallyPoint[i].selectTileID.x].pos.y)
			)
		{
			maxDistanceExit = getDistance(tile[randMapGeneSupport.entranceTileID.y][randMapGeneSupport.entranceTileID.x].pos.x,
				tile[randMapGeneSupport.entranceTileID.y][randMapGeneSupport.entranceTileID.x].pos.y,
				tile[selectedRallyPoint[i].selectTileID.y][selectedRallyPoint[i].selectTileID.x].pos.x,
				tile[selectedRallyPoint[i].selectTileID.y][selectedRallyPoint[i].selectTileID.x].pos.y);
			randMapGeneSupport.exitTileID = selectedRallyPoint[i].selectTileID;
		}

		if (i == MAX_SELECTED_RALLYPOINT - 1)
		{
			if (randMapGeneSupport.exitTileID.y <= TILEY - 1 && randMapGeneSupport.exitTileID.y >= TILEY - 4)
			{
				randMapGeneSupport.exitTileID.y -= 4;
			}
			if (randMapGeneSupport.exitTileID.x <= TILEX - 1 && randMapGeneSupport.exitTileID.x >= TILEX - 4)
			{
				randMapGeneSupport.exitTileID.x -= 4;
			}

			for (int i = 0; i < 4; i++)
			{
				if (i == 3)
				{
					tile[randMapGeneSupport.exitTileID.y + i][randMapGeneSupport.exitTileID.x].bgTile = true;
					tile[randMapGeneSupport.exitTileID.y + i][randMapGeneSupport.exitTileID.x + 1].bgTile = false;
					tile[randMapGeneSupport.exitTileID.y + i][randMapGeneSupport.exitTileID.x + 1].tileFrameID = { 0, 1 };
					tile[randMapGeneSupport.exitTileID.y + i][randMapGeneSupport.exitTileID.x + 2].bgTile = false;
					tile[randMapGeneSupport.exitTileID.y + i][randMapGeneSupport.exitTileID.x + 2].tileFrameID = { 0, 1 };
					tile[randMapGeneSupport.exitTileID.y + i][randMapGeneSupport.exitTileID.x + 3].bgTile = true;
					randMapGeneSupport.exitRect = RectMakeCenter(tile[randMapGeneSupport.exitTileID.y + i][randMapGeneSupport.exitTileID.x + 1].pos.x + (TILESIZE / 2),
						tile[randMapGeneSupport.exitTileID.y + 2][randMapGeneSupport.exitTileID.x + 1].pos.y - 12, 68, 83);
				}
				else
				{
					tile[randMapGeneSupport.exitTileID.y + i][randMapGeneSupport.exitTileID.x].bgTile = true;
					tile[randMapGeneSupport.exitTileID.y + i][randMapGeneSupport.exitTileID.x + 1].bgTile = true;
					tile[randMapGeneSupport.exitTileID.y + i][randMapGeneSupport.exitTileID.x + 2].bgTile = true;
					tile[randMapGeneSupport.exitTileID.y + i][randMapGeneSupport.exitTileID.x + 3].bgTile = true;
				}
			}
		}
	}

	POINT currentID; // ���࿡ ������ �Ǵ� ������ ���� ��츦 ����Ͽ�
	currentID = { randMapGeneSupport.entranceTileID.x, randMapGeneSupport.entranceTileID.y };
	while (true)
	{
		if (currentID.x > randMapGeneSupport.exitTileID.x) currentID.x--; // currentID_XY�� �־��� ��ġ���� ��ǥ�������� x ���� Ŭ�� �����ϱ� ���� ����.
		else if (currentID.x < randMapGeneSupport.exitTileID.x) currentID.x++;

		if (currentID.y > randMapGeneSupport.exitTileID.y) currentID.y--; // currentID_XY�� �־��� ��ġ���� ��ǥ�������� y ���� Ŭ�� �����ϱ� ���� ����.
		else if (currentID.y < randMapGeneSupport.exitTileID.y) currentID.y++;

		tile[currentID.y][currentID.x].bgTile = true; // �̵��� ��ŭ �� Ÿ�� �����

		if (currentID.x > 0) // ���� �� ��ĭ �����
			tile[currentID.y][currentID.x - 1].bgTile = true;
		if (currentID.x < TILEX - 1) // ������ �� ��ĭ �����
			tile[currentID.y][currentID.x + 1].bgTile = true;

		if (currentID.y > 0)	// ���� ��ĭ �����
			tile[currentID.y - 1][currentID.x].bgTile = true;
		if (currentID.y < TILEY - 1) // �Ʒ��� ��ĭ �����
			tile[currentID.y + 1][currentID.x].bgTile = true;

		if (currentID.x == randMapGeneSupport.exitTileID.x && // �˻� ���� currentID_XY�� ��ǥ������ �����ϸ�
			currentID.y == randMapGeneSupport.exitTileID.y)
		{
			break;
		}
	}

	// ������ ���� �Ա��� �ⱸ�� ���� �ٽ� �ѹ� Ȯ��
	if (randMapGeneSupport.entranceTileID.y + 3 < TILEY && randMapGeneSupport.entranceTileID.x + 2 < TILEX)
	{
		tile[randMapGeneSupport.entranceTileID.y + 3][randMapGeneSupport.entranceTileID.x + 1].bgTile = false;
		tile[randMapGeneSupport.entranceTileID.y + 3][randMapGeneSupport.entranceTileID.x + 2].bgTile = false;
	}
	if (randMapGeneSupport.exitTileID.y + 3 < TILEY && randMapGeneSupport.exitTileID.x + 2 < TILEX)
	{
		tile[randMapGeneSupport.exitTileID.y + 3][randMapGeneSupport.exitTileID.x + 1].bgTile = false;
		tile[randMapGeneSupport.exitTileID.y + 3][randMapGeneSupport.exitTileID.x + 2].bgTile = false;
	}

	for (int i = 0; i < TILEX; i++)
	{
		tile[TILEY - 1][i].bgTile = false;
	}
	randMapGeneSequence = SetObj;
}

void MapGenerator::update_SetObj()
{
	if (randMapGeneSupport.entranceTileID.x == randMapGeneSupport.exitTileID.x &&
		randMapGeneSupport.entranceTileID.y == randMapGeneSupport.exitTileID.y)
	{
		TIMEMANAGER->setLoadingTimeSwitch(true);
		init();
	}
	else
	{
		randMapGeneSequence = TileOutlineDeco;
	}
}

void MapGenerator::update_TileOutlineDeco()
{
	for (int i = 0; i < TILEY; i++) // Ÿ�� ����ó��
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (j > 0) // ���� �˻�
			{
				if (tile[i][j - 1].bgTile) tile[i][j].outsideTiling.L = true;
			}
			if (j < TILEX - 1) // ������ �˻�
			{
				if (tile[i][j + 1].bgTile) tile[i][j].outsideTiling.R = true;
			}

			if (i > 0) // �� �˻�
			{
				if (tile[i - 1][j].bgTile) tile[i][j].outsideTiling.T = true;
			}
			if (i < TILEY - 1) // �Ʒ� �˻�
			{
				if (tile[i + 1][j].bgTile) tile[i][j].outsideTiling.B = true;
			}
		}
	}
	randMapGeneSequence = Done;
}

void MapGenerator::render_Debug(HDC getMemDC)
{
	char minimapPosID_0[128];
	sprintf_s(minimapPosID_0, sizeof(minimapPosID_0), "minimapPosID_0 : [%d,%d] ", selectedRallyPoint[0].selectTileID.x, selectedRallyPoint[0].selectTileID.y);
	TextOut(getMemDC, 800, 370, minimapPosID_0, strlen(minimapPosID_0));

	char minimapPosID_1[128];
	sprintf_s(minimapPosID_1, sizeof(minimapPosID_1), "minimapPosID_1 : [%d,%d] ", selectedRallyPoint[1].selectTileID.x, selectedRallyPoint[1].selectTileID.y);
	TextOut(getMemDC, 800, 390, minimapPosID_1, strlen(minimapPosID_1));

	char minimapPosID_2[128];
	sprintf_s(minimapPosID_2, sizeof(minimapPosID_2), "minimapPosID_2 : [%d,%d] ", selectedRallyPoint[2].selectTileID.x, selectedRallyPoint[2].selectTileID.y);
	TextOut(getMemDC, 800, 410, minimapPosID_2, strlen(minimapPosID_2));

	char minimapPosID_3[128];
	sprintf_s(minimapPosID_3, sizeof(minimapPosID_3), "minimapPosID_3 : [%d,%d] ", selectedRallyPoint[3].selectTileID.x, selectedRallyPoint[3].selectTileID.y);
	TextOut(getMemDC, 800, 430, minimapPosID_3, strlen(minimapPosID_3));

	char minimapPosID_4[128];
	sprintf_s(minimapPosID_4, sizeof(minimapPosID_4), "minimapPosID_4 : [%d,%d] ", selectedRallyPoint[4].selectTileID.x, selectedRallyPoint[4].selectTileID.y);
	TextOut(getMemDC, 800, 450, minimapPosID_4, strlen(minimapPosID_4));

	char minimapPosID_5[128];
	sprintf_s(minimapPosID_5, sizeof(minimapPosID_5), "minimapPosID_5 : [%d,%d] ", selectedRallyPoint[5].selectTileID.x, selectedRallyPoint[5].selectTileID.y);
	TextOut(getMemDC, 800, 470, minimapPosID_5, strlen(minimapPosID_5));

	char minimapPosID_6[128];
	sprintf_s(minimapPosID_6, sizeof(minimapPosID_6), "minimapPosID_6 : [%d,%d] ", selectedRallyPoint[6].selectTileID.x, selectedRallyPoint[6].selectTileID.y);
	TextOut(getMemDC, 800, 490, minimapPosID_6, strlen(minimapPosID_6));

	char minimapPosID_7[128];
	sprintf_s(minimapPosID_7, sizeof(minimapPosID_7), "minimapPosID_7 : [%d,%d] ", selectedRallyPoint[7].selectTileID.x, selectedRallyPoint[7].selectTileID.y);
	TextOut(getMemDC, 800, 510, minimapPosID_7, strlen(minimapPosID_7));

	char minimapPosID_8[128];
	sprintf_s(minimapPosID_8, sizeof(minimapPosID_8), "minimapPosID_8 : [%d,%d] ", selectedRallyPoint[8].selectTileID.x, selectedRallyPoint[8].selectTileID.y);
	TextOut(getMemDC, 800, 530, minimapPosID_8, strlen(minimapPosID_8));

	char minimapPosID_9[128];
	sprintf_s(minimapPosID_9, sizeof(minimapPosID_9), "minimapPosID_9 : [%d,%d] ", selectedRallyPoint[9].selectTileID.x, selectedRallyPoint[9].selectTileID.y);
	TextOut(getMemDC, 800, 550, minimapPosID_9, strlen(minimapPosID_9));

	char minimapPosID_10[128];
	sprintf_s(minimapPosID_10, sizeof(minimapPosID_10), "minimapPosID_10 : [%d,%d] ", selectedRallyPoint[10].selectTileID.x, selectedRallyPoint[10].selectTileID.y);
	TextOut(getMemDC, 800, 570, minimapPosID_10, strlen(minimapPosID_10));

	char minimapPosID_11[128];
	sprintf_s(minimapPosID_11, sizeof(minimapPosID_11), "minimapPosID_11 : [%d,%d] ", selectedRallyPoint[11].selectTileID.x, selectedRallyPoint[11].selectTileID.y);
	TextOut(getMemDC, 800, 590, minimapPosID_11, strlen(minimapPosID_11));

	char entranceID[128];
	sprintf_s(entranceID, sizeof(entranceID), "entranceID : [%d,%d] ", randMapGeneSupport.entranceTileID.y, randMapGeneSupport.entranceTileID.x);
	TextOut(getMemDC, 800, 610, entranceID, strlen(entranceID));

	char exitID[128];
	sprintf_s(exitID, sizeof(exitID), "exitID : [%d,%d] ", randMapGeneSupport.exitTileID.y, randMapGeneSupport.exitTileID.x);
	TextOut(getMemDC, 800, 630, exitID, strlen(exitID));

	char randMapGeneSequenceCheck[128];
	sprintf_s(randMapGeneSequenceCheck, sizeof(randMapGeneSequenceCheck), "randMapGeneSequence : %d", randMapGeneSequence);
	TextOut(getMemDC, 50, 370, randMapGeneSequenceCheck, strlen(randMapGeneSequenceCheck));

	char miniMapLengthX[128];
	sprintf_s(miniMapLengthX, sizeof(miniMapLengthX), "miniMapLengthX : %f", randMapGeneSupport.minimap_lengthX());
	TextOut(getMemDC, 50, 390, miniMapLengthX, strlen(miniMapLengthX));

	char miniMapLengthY[128];
	sprintf_s(miniMapLengthY, sizeof(miniMapLengthY), "miniMapLengthY : %f", randMapGeneSupport.minimap_lengthY());
	TextOut(getMemDC, 50, 410, miniMapLengthY, strlen(miniMapLengthY));

	char CameraPosX[128];
	sprintf_s(CameraPosX, sizeof(CameraPosX), "camera Pos X: %f", CAMERAMANAGER->get_CameraPos().x);
	TextOut(getMemDC, 50, 430, CameraPosX, strlen(CameraPosX));

	char CameraPosY[128];
	sprintf_s(CameraPosY, sizeof(CameraPosY), "camera Pos Y: %f", CAMERAMANAGER->get_CameraPos().y);
	TextOut(getMemDC, 50, 450, CameraPosY, strlen(CameraPosY));

	char renderTileID_L[128];
	sprintf_s(renderTileID_L, sizeof(renderTileID_L), "renderTileID_L : %d", CAMERAMANAGER->get_currentRenderTileID_LT().x);
	TextOut(getMemDC, 50, 470, renderTileID_L, strlen(renderTileID_L));

	char renderTileID_T[128];
	sprintf_s(renderTileID_T, sizeof(renderTileID_T), "renderTileID_T : %d", CAMERAMANAGER->get_currentRenderTileID_LT().y);
	TextOut(getMemDC, 50, 490, renderTileID_T, strlen(renderTileID_T));

	char renderTileID_R[128];
	sprintf_s(renderTileID_R, sizeof(renderTileID_R), "renderTileID_R : %d", CAMERAMANAGER->get_currentRenderTileID_RB().x);
	TextOut(getMemDC, 50, 510, renderTileID_R, strlen(renderTileID_R));

	char renderTileID_B[128];
	sprintf_s(renderTileID_B, sizeof(renderTileID_B), "renderTileID_B : %d", CAMERAMANAGER->get_currentRenderTileID_RB().y);
	TextOut(getMemDC, 50, 530, renderTileID_B, strlen(renderTileID_B));
}