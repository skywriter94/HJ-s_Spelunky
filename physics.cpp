#include "stdafx.h"
#include "physics.h"
#include "MapGenerator.h"



void physics::TileCollision(int& _posState, POINTFLOAT& _pos, POINT _id, RECT& _rc, image* _img, bool _dir, float& _gravity, float& _jumpPower, float& _speed)
{
	RECT tempRC;
	if (_posState == 0)
	{
		_gravity += GRAVITY;
		if (_gravity > 15) _gravity = 15;
		_pos.y += _gravity;
	}
	else
	{
		_gravity = 0;
	}

	if (_jumpPower > 0)
	{
		_pos.y -= _jumpPower;
		_jumpPower -= GRAVITY;
	}
	if (_jumpPower > 10)
	{
		_jumpPower = 10;
	}

	if (_pos.y < _img->getFrameHeight() / 2) // 상
	{
		_jumpPower = 0;
		_pos.y = _img->getFrameHeight() / 2;
	}
	if (_pos.y > TILEY * TILESIZE - _img->getFrameHeight() / 2) // 하
	{
		if (_gravity > 0) // 만약 가속도가 0이상이면
		{
			_pos.y -= _gravity;
			_gravity = 0;
		}
		_posState = 1;
		_pos.y = TILEY * TILESIZE - _img->getFrameHeight() / 2;
	}
	if (_pos.x < _img->getFrameWidth() / 2) // 좌
	{
		_pos.x = _img->getFrameWidth() / 2;
	}
	if (_pos.x > TILEX * TILESIZE - _img->getFrameWidth() / 2) // 우
	{
		_pos.x = TILEX * TILESIZE - _img->getFrameWidth() / 2;
	}
	
	if (_mapGene->Get_Tile(_id.y, _id.x)->bgTile)
	{
		// ◆◆◆◆◆ 상 충돌 처리 ◆◆◆◆◆
		if (!_mapGene->Get_Tile(_id.y - 1, _id.x)->bgTile && _id.y > 0)
		{
			if (IntersectRect(&tempRC, &_mapGene->Get_Tile(_id.y - 1, _id.x)->rc, &_rc)) // 충돌 시
			{
				_jumpPower = 0; // 더 이상 타일안으로 들어가지 않도록 점프 가속도 0으로 만듬
				_pos.y = _mapGene->Get_Tile(_id.y - 1, _id.x)->rc.bottom + (_img->getFrameHeight() / 2);
			}
		}

		// ◆◆◆◆◆ 하 충돌 처리 ◆◆◆◆◆
		if (!_mapGene->Get_Tile(_id.y + 1, _id.x)->bgTile && _id.y < TILEY - 1)
		{
			if (IntersectRect(&tempRC, &_mapGene->Get_Tile(_id.y + 1, _id.x)->rc, &_rc)) // 충돌 시
			{
				if (_gravity > 0) // 만약 플레이어 가속도가 0이상이면
				{
					_pos.y -= _gravity;
					_gravity = 0;
				}
				_posState = 1;
				_pos.y = _mapGene->Get_Tile(_id.y + 1, _id.x)->rc.top - (_img->getFrameHeight() / 2);
			}
			if (_posState == 1) // 위치 상태는 '지면'인데
			{
				if (!IntersectRect(&tempRC, &_mapGene->Get_Tile(_id.y + 1, _id.x)->rc, &_rc)) // 만약에 붕떠있는 경우
				{
					_pos.y = _mapGene->Get_Tile(_id.y + 1, _id.x)->rc.top - (_img->getFrameHeight() / 2);
				}
			}
		}
		else
		{
			_posState = 0;
		}
	
		if (!_dir)
		{
			// ◆◆◆◆◆ 좌 충돌 처리 ◆◆◆◆◆
			if (!_mapGene->Get_Tile(_id.y, _id.x - 1)->bgTile)
			{
				if (IntersectRect(&tempRC, &_mapGene->Get_Tile(_id.y, _id.x - 1)->rc, &_rc))
				{
					//_pos.x = _mapGene->Get_Tile(_id.y, _id.x - 1)->rc.right + (_img->getFrameWidth() / 2);
					_pos.x += _speed;
					//_speed = 0;
				}
			}
		}
		else
		{
			// ◆◆◆◆◆ 우 충돌 처리 ◆◆◆◆◆
			if (!_mapGene->Get_Tile(_id.y, _id.x + 1)->bgTile)
			{
				if (IntersectRect(&tempRC, &_mapGene->Get_Tile(_id.y, _id.x + 1)->rc, &_rc))
				{
					//_pos.x = _mapGene->Get_Tile(_id.y, _id.x + 1)->rc.left - (_img->getFrameWidth() / 2);
					_pos.x -= _speed;
					//_speed = 0;
				}
			}
		}
	
		if (_posState == 0)
		{
			if (!_mapGene->Get_Tile(_id.y + 1, _id.x - 1)->bgTile) // 좌 하단
			{
				if (IntersectRect(&tempRC, &_mapGene->Get_Tile(_id.y + 1, _id.x - 1)->rc, &_rc)) // 충돌 시
				{
					if (_mapGene->Get_Tile(_id.y + 1, _id.x - 1)->rc.top < _rc.bottom)
					{
						//_speed = 0;
						_pos.x += _speed;
					}
				}
			}
			if (!_mapGene->Get_Tile(_id.y + 1, _id.x + 1)->bgTile) // 우 하단
			{
				if (IntersectRect(&tempRC, &_mapGene->Get_Tile(_id.y + 1, _id.x + 1)->rc, &_rc)) // 충돌 시
				{
					if (_mapGene->Get_Tile(_id.y + 1, _id.x + 1)->rc.top < _rc.bottom)
					{
						//_speed = 0;
						_pos.x -= _speed;
					}
				}
			}
		}
	}
	else
	{

		if (!_dir) // 방향이 왼쪽인 경우
		{
			if (_mapGene->Get_Tile(_id.y, _id.x)->rc.top < _rc.top)
			{
				_pos.y += (_mapGene->Get_Tile(_id.y, _id.x)->rc.bottom - _rc.top);
			}
			else
			{
				_pos.y -= (_rc.bottom - _mapGene->Get_Tile(_id.y, _id.x)->rc.top);
			}
			_pos.x += (_mapGene->Get_Tile(_id.y, _id.x)->rc.right - _rc.left);
		}
		else // 오른쪽인 경우
		{
			if (_mapGene->Get_Tile(_id.y, _id.x)->rc.top < _rc.top)
			{
				_pos.y += (_mapGene->Get_Tile(_id.y, _id.x)->rc.bottom - _rc.top);
			}
			else
			{
				_pos.y -= (_rc.bottom - _mapGene->Get_Tile(_id.y, _id.x)->rc.top);
			}
			_pos.x -= _rc.right - _mapGene->Get_Tile(_id.y, _id.x)->rc.left;
		}
		_speed = 0;
		_jumpPower = 0;
		_gravity = 0;
	}
}

void physics::ObjTileCollision(int& _posState, POINTFLOAT& _pos, POINT _id, RECT& _rc, image* _img, bool& _dir, float& _gravity, float& _throwHeightPower, float& _speed, float& _angle)
{
	RECT tempRC;

	if (_pos.y < _img->getFrameHeight() / 2) // 상
	{
		//_gravity += _throwHeightPower;
		_throwHeightPower = 0; // 더 이상 타일안으로 들어가지 않도록 점프 가속도 0으로 만듬
		_pos.y = _img->getFrameHeight() / 2;
	}
	if (_pos.y > TILEY* TILESIZE - _img->getFrameHeight() / 2) // 하
	{
		_throwHeightPower += _gravity * 0.5f;
		if (_gravity > 0)
		{
			_pos.y -= _throwHeightPower;
			_gravity = 0;
		}
		_pos.y = TILEY * TILESIZE - _img->getFrameHeight() / 2;
		_posState = 1;
	}
	if (_pos.x < _img->getFrameWidth() / 2) // 좌
	{	
		_pos.x = _img->getFrameWidth() / 2;
		_dir = true;
	}
	if (_pos.x > TILEX* TILESIZE - _img->getFrameWidth() / 2) // 우
	{	
		_pos.x = TILEX * TILESIZE - _img->getFrameWidth() / 2;
		_dir = false;
	}

	if (_mapGene->Get_Tile(_id.y, _id.x)->bgTile)
	{
		// ◆◆◆◆◆ 상 충돌 처리 ◆◆◆◆◆
		if (!_mapGene->Get_Tile(_id.y - 1, _id.x)->bgTile && _id.y > 0)
		{
			if (IntersectRect(&tempRC, &_mapGene->Get_Tile(_id.y - 1, _id.x)->rc, &_rc)) // 충돌 시
			{
				//_gravity += _throwHeightPower;
				_throwHeightPower = 0; // 더 이상 타일안으로 들어가지 않도록 점프 가속도 0으로 만듬
				_pos.y = _mapGene->Get_Tile(_id.y - 1, _id.x)->rc.bottom + (_img->getFrameHeight() / 2);
			}
		}

		// ◆◆◆◆◆ 하 충돌 처리 ◆◆◆◆◆
		if (!_mapGene->Get_Tile(_id.y + 1, _id.x)->bgTile && _id.y < TILEY - 1)
		{
			if (IntersectRect(&tempRC, &_mapGene->Get_Tile(_id.y + 1, _id.x)->rc, &_rc))
			{
				if (_gravity > 0)
				{
					_pos.y -= _throwHeightPower;
					_gravity = 0;
				}
				_pos.y = _mapGene->Get_Tile(_id.y + 1, _id.x)->rc.top - (_img->getFrameHeight() / 2);
				_throwHeightPower += _gravity * 0.5f;
				_posState = 1;
			}
			if (_posState == 1) // 위치 상태는 '지면'인데
			{
				if (!IntersectRect(&tempRC, &_mapGene->Get_Tile(_id.y + 1, _id.x)->rc, &_rc)) // 만약에 붕떠있는 경우
				{
					_pos.y = _mapGene->Get_Tile(_id.y + 1, _id.x)->rc.top - (_img->getFrameHeight() / 2);
				}
			}
		}
		else
		{
			_posState = 0;
		}


		if (!_dir)
		{
			// ◆◆◆◆◆ 좌 충돌 처리 ◆◆◆◆◆
			if (!_mapGene->Get_Tile(_id.y - 1, _id.x - 1)->bgTile)
			{
				if (IntersectRect(&tempRC, &_mapGene->Get_Tile(_id.y - 1, _id.x - 1)->rc, &_rc))
				{
					_pos.x = _mapGene->Get_Tile(_id.y - 1, _id.x - 1)->rc.right + (_img->getFrameWidth() / 2);
					_dir = true;
				}
			}
			if (!_mapGene->Get_Tile(_id.y, _id.x - 1)->bgTile)
			{
				if (IntersectRect(&tempRC, &_mapGene->Get_Tile(_id.y, _id.x - 1)->rc, &_rc))
				{
					_pos.x = _mapGene->Get_Tile(_id.y, _id.x - 1)->rc.right + (_img->getFrameWidth() / 2);
					_dir = true;
				}
			}
			if (!_mapGene->Get_Tile(_id.y + 1, _id.x - 1)->bgTile)
			{
				if (IntersectRect(&tempRC, &_mapGene->Get_Tile(_id.y + 1, _id.x - 1)->rc, &_rc))
				{
					_pos.x = _mapGene->Get_Tile(_id.y + 1, _id.x - 1)->rc.right + (_img->getFrameWidth() / 2);
					_dir = true;
				}
			}
		}
		else
		{
			// ◆◆◆◆◆ 우 충돌 처리 ◆◆◆◆◆
			if (!_mapGene->Get_Tile(_id.y - 1, _id.x + 1)->bgTile)
			{
				if (IntersectRect(&tempRC, &_mapGene->Get_Tile(_id.y - 1, _id.x + 1)->rc, &_rc))
				{
					_pos.x = _mapGene->Get_Tile(_id.y - 1, _id.x + 1)->rc.left - (_img->getFrameWidth() / 2);
					_dir = false;
				}
			}
			if (!_mapGene->Get_Tile(_id.y, _id.x + 1)->bgTile)
			{
				if (IntersectRect(&tempRC, &_mapGene->Get_Tile(_id.y, _id.x + 1)->rc, &_rc))
				{
					_pos.x = _mapGene->Get_Tile(_id.y, _id.x + 1)->rc.left - (_img->getFrameWidth() / 2);
					_dir = false;
				}
			}
			if (!_mapGene->Get_Tile(_id.y + 1, _id.x + 1)->bgTile)
			{
				if (IntersectRect(&tempRC, &_mapGene->Get_Tile(_id.y + 1, _id.x + 1)->rc, &_rc))
				{
					_pos.x = _mapGene->Get_Tile(_id.y + 1, _id.x + 1)->rc.left - (_img->getFrameWidth() / 2);
					_dir = false;
				}
			}
		}
	}
	
	if (_posState == 0)
	{
		_gravity += GRAVITY;
		if (_gravity > 15) _gravity = 15;
		_pos.y += _gravity;
	}
	else
	{
		_gravity *= 0.5f;
		_throwHeightPower = _gravity;
	}
	
	if (_throwHeightPower > 0)
	{
		_throwHeightPower -= GRAVITY;
		_pos.y -= _throwHeightPower;
	}
	
	
	if (!_dir)
	{
		_pos.x -= _speed;
	}
	else
	{
		_pos.x += _speed;
	}
	
	if (_speed > 0)
	{
		_speed -= SPEED_R;
	}
}