/*****************************************
Author	: Li Chong
Main	: myyifan@gmail.com

播放器控制核心
******************************************/
#ifndef APLAYER_PLAYCORE
#define APLAYER_PLAYCORE

#include <iostream>

#ifdef APLAY_API
//导出函数
#else

#define APLAY_API extern "C" __declspec(dllimport)

#endif
enum LoopMode {
	LoopNo = 1,
	LoopAll,
	LoopOnce,
	LoopRandom
};
const wchar_t SupportType[] = L"*.mp3;*.wav;";
class IPlayController
{
public:
	virtual int				__stdcall	AddMusic(const wchar_t* musicPath) = 0;
	virtual int				__stdcall	Play() = 0;
	virtual int				__stdcall	Continue() = 0;
	virtual int				__stdcall	Pause() = 0;
	virtual int				__stdcall	Stop() = 0;
	virtual int				__stdcall	PlayNext() = 0;
	virtual void			__stdcall	SetLoopMode(LoopMode mode) = 0;
	virtual unsigned long	__stdcall	GetPlayMilliseconds() = 0;
	virtual std::wstring	__stdcall	GetLyric() = 0;
	virtual void			__stdcall	Release() = 0;
};

APLAY_API IPlayController* __stdcall CreateController();


#endif