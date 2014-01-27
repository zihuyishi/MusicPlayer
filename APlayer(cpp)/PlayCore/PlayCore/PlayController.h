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
	//IPlayController method
	int				__stdcall AddMusic(const wchar_t* musicPath);
	int				__stdcall Play();
	int				__stdcall Continue();
	int				__stdcall Pause();
	int				__stdcall Stop();
	int				__stdcall PlayNext();
	unsigned long	__stdcall GetPlayMilliseconds();
	void			__stdcall SetLoopMode(LoopMode mode) { _loopmode = mode; }
	std::wstring	__stdcall GetLyric();
	void			__stdcall Release();
private:
	MusicPlayer		_player;
	MusicList		_list;
	LyricFile		_lyric;
	LoopMode		_loopmode;
	int				_currentindex;

private:
	int				getNextIndex();


};

