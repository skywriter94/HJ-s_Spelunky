#pragma once
#include "singletonBase.h"
#include "timer.h"

class timeManager : public singletonBase <timeManager>
{
private:
	timer* _timer;

public:
	timeManager();
	~timeManager();

	HRESULT init();
	void release();
	void update(float lockFPS = 0.0f);
	void render(HDC hdc);

	//DeltaŸ��
	inline float getElapsedTime() const { return _timer->getElapsedTime(); }

	//WorldŸ��
	inline float getWorldTime() const { return _timer->getWorldTime(); }

	//�� �ε� Ÿ�� ����ġ
	inline void setLoadingTimeSwitch(bool _switch) { _timer->setLoadingTimeSwitch(_switch); }

	//�� �ε� Ÿ��
	inline float getLoadingTime() { return _timer->getLodingTime(); }
	inline void setLoadingTime(float timeVal) { _timer->setLodingTime(timeVal); }
};