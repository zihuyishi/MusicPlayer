#pragma once
#include "PlayCore.h"
#include "PlayCoreHead.h"
#include "MusicPlayer.h"
#include "MusicFile.h"
#include "MusicList.h"
#include "LyricFile.h"

using namespace PlayCore;

class PlayController : public UnCopyable, public IPlayController
{
public:
	PlayController();
	~PlayController();
public:
	int __stdcall AddMusic(wchar_t* musicPath);
	int __stdcall Play();
	int __stdcall Pause();
	int __stdcall Stop();
	int __stdcall PlayNext();
	int __stdcall ShowLyric();
	void __stdcall SetLoopMode(LoopMode mode) { _loopmode = mode; }
	void __stdcall Release();
private:
	MusicPlayer _player;
	MusicList _list;
	LyricFile _lyric;
	LoopMode _loopmode;
	int _currentindex;
private:
	int getNextIndex();
};

