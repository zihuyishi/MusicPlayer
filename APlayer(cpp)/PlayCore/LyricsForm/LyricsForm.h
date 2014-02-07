/*****************************************
Author	: Li Chong
Mail	: myyifan@gmail.com

歌词窗体控制类，用来控制歌词窗体的显示，颜色
等信息。
******************************************/
#ifndef APLAYER_LYRICSFORM_HEADER
#define APLAYER_LYRICSFORM_HEADER

#include <Windows.h>
#ifdef LYRIC_API
//导出函数
#else
//倒入函数
#define LYRIC_API extern "C" __declspec(dllimport)
#endif

enum LyricFormCommand {
	CM_LYRIC		= 0x8000,	//设置新歌词，@WPARAM 传递std::wstring*
	CM_FONTCOLOR	= 0x8001,	//设置歌词文字颜色，@WPARAM 传递RGB 0xRRGGBB,@LPARAM 传递透明度0-255
	CM_FONTSIZE		= 0x8002,	//设置歌词文字大小，@WPARAM 字体大小
	CM_BACKCOLOR	= 0x8003,	//设置窗口背景颜色，
	CM_BACKIMAGE	= 0x8004,	//设置窗口背景图片，
	CM_CREATETIMER	= 0x8005,	//创建一个计时器，@WPARAM 传递间隔，@LPARAM 传递callback function
};
typedef void(__stdcall *LYRICFORM_CALLBACK)(HWND hWnd, void *lpParam);
class ILyricFormController {
public:
	virtual	void __stdcall LyricForm_Run() = 0;
	virtual void __stdcall LyricForm_SetLyric(const wchar_t*) = 0;
	virtual void __stdcall LyricForm_SendCommand(LyricFormCommand, WPARAM, LPARAM) = 0;
	virtual void __stdcall LyricForm_CreateTimer(unsigned int, LYRICFORM_CALLBACK, void*) = 0;
};

LYRIC_API ILyricFormController* __stdcall CreateLyricFormController();

#endif