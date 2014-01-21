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
	BOOL Play();
	BOOL Play(const wchar_t* filePath);
	BOOL Play(const MusicFile &musicFile);
	BOOL Play(const wstring filePath);
	BOOL Pause();
	BOOL Stop();
private:
private:
	ZPlay *_player;
};

PLAYCORE_END
