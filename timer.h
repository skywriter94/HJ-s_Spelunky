#pragma once

class timer
{
private:
	float _timeScale;
	float _timeElapsed;	//TickCount

	__int64 _curTime;
	__int64 _lastTime;
	__int64 _periodFrequency;

	unsigned long _frameRate;
	unsigned long _FPSFrameCount;

	float _FPSTimeElapsed;
	float _worldTime;
	float _lodingTime;

	bool _loadingTimeSwitch;

public:
	timer();
	~timer();

	HRESULT init();
	void tick(float lockFPS = 0.0f);
	unsigned long getFrameRate(char* str = NULL) const;

	inline float getElapsedTime() const { return _timeElapsed; }
	inline float getWorldTime() const { return _worldTime; }

	inline float getLodingTime() const { return _lodingTime; }
	inline void setLodingTime(float timeVal) { _lodingTime = timeVal; }
	inline void setLoadingTimeSwitch(bool _switch) { _loadingTimeSwitch = _switch; }
};