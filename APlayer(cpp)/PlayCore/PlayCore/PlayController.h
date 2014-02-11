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
	int				__stdcall SetVolume(int value);
	unsigned long	__stdcall GetPlayMilliseconds();
	void			__stdcall SetLoopMode(LoopMode mode) { _loopmode = mode; }
	std::wstring	__stdcall GetLyric();
	int				__stdcall LoadList(const wchar_t* listPath);
	int				__stdcall SaveList(const wchar_t* listPath);
	void			__stdcall Release();
private:
	MusicPlayer		_player;
	MusicList		_list;
	LyricFile		_lyric;
	LoopMode		_loopmode;
	int				_currentindex;

private:
	/**
	*自动播放下一首
	*/
	int				autoPlayNext();
	/**
	*@bAuto	是否为自动播放下一首。用来区分用户手动
	*选择播放下一首歌或者上一首播放完自动播放下一首。
	*/
	int				getNextIndex(bool bAuto = false);
private:
	static DWORD WINAPI		playNextThread(LPVOID lpParam);
	static int __stdcall	onPlayEnd(void* instance, void* lpParam);
};

