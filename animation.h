#pragma once
#include <vector>

typedef void(*CALLBACK_FUNCTION)(void);
typedef void(*CALLBACK_FUNCTION_PARAMETER)(void*);

//void*  뭐든 담을 수 있다 형변환만 하면

class animation
{
public:
	typedef vector<POINT>	vFrameList;	//프레임 인덱스 저장용 벡터
	typedef vector<int>		vPlayList;	//플레이 인덱스 저장용 벡터

private:
	int			_frameNum;			//애니메이션 프레임 갯수

	vFrameList	_frameList;			//프레임 리스트
	vPlayList	_playList;			//플레이 리스트

	int _frameWidth;				//프레임 가로크기
	int _frameHeight;				//프레임 세로크기

	BOOL _loop;						//애니메이션 루프 여부

	float _frameUpdateSec;			//프레임 경과시간
	float _elapsedSec;				//델타 타임

	DWORD _nowPlayIndex;			//현재 플레이 인덱스
	BOOL _play;						//애니메이션 재생여부

	void*							_obj;
	CALLBACK_FUNCTION				_callbackFunction;
	CALLBACK_FUNCTION_PARAMETER		_callbackFunctionParameter;

public:
	animation();
	~animation();

	//애니메이션 init(이미지 전체 가로크기, 세로크기, 프레임 가로크기, 세로크기 )
	HRESULT init(int totalW, int totalH, int frameW, int frameH);
	void release();

	//디폴트 애니메이션
	void setDefPlayFrame(BOOL reverse = FALSE, BOOL loop = FALSE);
	void setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction);
	void setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);

	//배열에 담아 애니메이션 재생
	void setPlayFrame(int* playArr, int arrLen, BOOL loop = FALSE);
	void setPlayFrame(int* playArr, int arrLen, BOOL loop, CALLBACK_FUNCTION cbFunction);
	void setPlayFrame(int* playArr, int arrLen, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);

	//구간 애니메이션 재생
	void setPlayFrame(int start, int end, BOOL reverse = FALSE, BOOL loop = FALSE);
	void setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction);
	void setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);

	//애니메이션 프레임 재생속도 셋팅
	void setFPS(int framePerSec);

	//렌더링
	void frameUpdate(float elapsedTime);

	void start();
	void stop();
	void pause();
	void resume();

	inline BOOL isPlay() { return _play; }
	inline POINT getFramePos() { return _frameList[_playList[_nowPlayIndex]]; }
	inline int getFrameWidth() { return _frameWidth; }
	inline int getFrameHeight() { return _frameHeight; }
};

