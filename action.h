#pragma once
#include "gameNode.h"

typedef void(*CALLBACK_FUNCTION)(void);

class action : public gameNode
{
private:
	image* _image;				//이미지
	float _startX, _startY;		//출발좌표
	float _endX, _endY;			//도착좌표

	float _angle;				//가야하는 각도
	float _travelRange;			//가야하는 거리
	float _worldTimeCount;
	float _time;		

	bool _isMoving;

	CALLBACK_FUNCTION _callbackFunction;
	
public:
	action();
	~action();

	HRESULT init();
	void release();
	void update();
	void render();

	void moveTo(image* image, float endX, float endY, float time);
	void moveTo(image* image, float endX, float endY, float time, CALLBACK_FUNCTION cbFunction);

	void moving();

	bool getIsMoving() { return _isMoving; }
};

