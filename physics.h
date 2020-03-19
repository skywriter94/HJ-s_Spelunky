#pragma once
#include "singletonBase.h"

#define GRAVITY 0.2f
#define JUMPPOWER 10.0f
#define SPEED_R 0.1f

class MapGenerator;



class physics : public singletonBase<physics>
{
private:
	MapGenerator* _mapGene;

public:
	void TileCollision(int& _posState, POINTFLOAT& _pos, POINT _id, RECT& _rc, image* _img, bool _dir, float& _gravity, float& _jumpPower, float& _speed);
	void ObjTileCollision(int& _posState, POINTFLOAT& _pos, POINT _id, RECT& _rc, image* _img, bool& _dir, float& _gravity, float& _throwHeightPower, float& _speed, float& _angle);
	void SetMapGeneratorAddressLink(MapGenerator* mapGene) { _mapGene = mapGene; }
};