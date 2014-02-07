#pragma once
#include "PlayCoreHead.h"
#include "PlayCore.h"
#include "MusicFile.h"
PLAYCORE_BEGIN

class MusicPlayer : public UnCopyable
{
public:
	MusicPlayer();
	~MusicPlayer();
public:
	//just call ZPlay.play()
	BOOL			Play();
	//play a new file
	BOOL			Play(const wchar_t* filePath);
	BOOL			Play(const MusicFile &musicFile);
	BOOL			Play(const wstring& filePath);

	unsigned long	GetPlayMilliseconds();
	BOOL			Pause();
	BOOL			Stop();
	int				SetVolume(int value);

	BOOL			HasPlayEnd();
	BOOL			SetPlayEndCallback(MCallbackFunc pCallbackFunc, void* lpParam = 0);
private:
	BOOL			_play(const wstring& filePath);
private:
	static int __stdcall	TCallbackOnPlayEnd(void *instance,
		void *user_data, TCallbackMessage message,
		unsigned int param1, unsigned int param2);
private:
	ZPlay*			_player;
	BOOL			_bPlayEnd;
	MCallbackFunc	_onPlayEnd;
	void*			_onPlayEndPram;
};

PLAYCORE_END
