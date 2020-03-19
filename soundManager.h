#pragma once
#include "singletonBase.h"
#include <map>

#include "inc/fmod.hpp"	//fmod.hpp파일 인클루드
#pragma comment(lib, "lib/fmod64_vc.lib")	//라이브러리 파일 등록

using namespace FMOD;

#define SOUNDBUFFER 100
#define EXTRACHANNELBUFFER 50

#define TOTALSOUNDBUFFER SOUNDBUFFER + EXTRACHANNELBUFFER

class soundManager : public singletonBase<soundManager>
{
private:
	typedef map<string, Sound**>				arrSounds;
	typedef map<string, Sound**>::iterator		arrSoundsIter;
	typedef map<string, Channel**>				arrChannels;
	typedef map<string, Channel**>::iterator	arrChannelsIter;

private:
	System* _system;

	Sound** _sound;
	SoundGroup* _masterSound;
	SoundGroup* _musicSound;

	Channel** _channel;
	ChannelGroup* _musicGroup;  //노래 그룹
	ChannelGroup* _effectGroup; //이펙트 그룹조절용
	ChannelGroup* _masterGroup; //전체적 조절

	arrSounds _mTotalSounds;

public:
	soundManager();
	~soundManager();

	HRESULT init();
	void release();
	void update();

	//사운드 추가 함수
	void addSound(string keyName, string soundName, bool bgm, bool loop);

	// 1.0 maximum 0.0 ~ 1.0 -> 0 ~ 255
	void play(string keyName, float volume = 1.0f, bool isMusic = false);
	void stop(string keyName);
	void pause(string keyName);
	void resume(string keyName);

	unsigned int getSongPosition(string keyName, unsigned int& pos); // 현재 곡의 위치를 받아오는 함수
	float getPitch(string keyName, float& pitch); // 현재 곡의 pitch값을 받아오는 함수

	void setVolume(string keyName, float volume); // Volume 조절 함수
	void setPitch(string keyName, float pitch); // pitch 조절 함수(곡 속도 조절 함수)

	bool isPlaySound(string keyName);
	bool isPauseSound(string keyName);
};