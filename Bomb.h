#pragma once
#include "gameNode.h"

#define PI 3.14f
#define PI2 PI * 2.0f
#define SPEED_R 0.1f
#define THROWHEIGHTPOWER 10.0f



class Bomb : public gameNode
{
private:
	enum BombPosState
	{
		Air,
		Ground
	};

private:
	BombPosState posState;
	POINTFLOAT pos;
	POINT posID;
	RECT rc;
	image* img;
	animation* ani;
	bool dir;
	float gravity;
	float throwHeightPower;
	float speed;
	float angle;
	float timer;
	bool isThrow = false; // 처음에 폭탄을 던지는 조건에서 필요로 하여 false로 지정
	RECT bombArea;

public:
	HRESULT init(POINTFLOAT _pos, bool _dir, float _speed, float _timer);
	void release();
	void update();
	void render();

public: 
	float Get_BombTimer() { return timer; }
	void Set_isThrow(bool _isThrow) { isThrow = _isThrow; }
	bool Get_isThrow() { return isThrow; }
	RECT Get_BombArea() { return bombArea; }
	POINT Get_BombArea_LT() { return { bombArea.left / 64, bombArea.top / 64 }; }
	POINT Get_BombArea_RB() { return { bombArea.right / 64, bombArea.bottom / 64 }; }
	POINTFLOAT Get_BombPos() { return pos; }
};