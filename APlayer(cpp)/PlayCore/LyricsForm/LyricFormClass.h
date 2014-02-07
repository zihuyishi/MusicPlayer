/*****************************************
歌词窗口的实现类

******************************************/

#include "..\Common\ScopeGuard.h"
#include "DrawLibrary.h"


#ifndef APLAYER_LYRICFORMCLASS_HEADER
#define APLAYER_LYRICFORMCLASS_HEADER


#include "LyricsForm.h"

#include <Windows.h>
#include "resource.h"
#define MyDllName L"LyricForm.dll"

#include <iostream>

typedef struct{
	unsigned int interval;
	void* lpParam;
} CREATETIMER_PARAM;

class LyricForm 
{
public:
	HWND m_hWnd;
	typedef LyricForm ThisType;
	static const int DefaultWidth = 400;
	static const int DefaultHeight = 400;
public:
	LyricForm()
	{
		fontcolor = { 0, 0, 0, 1.0f };
	}
	~LyricForm()
	{
	}
public:
	BOOL Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = DefaultWidth,
		int nHeight = DefaultHeight,
		HWND hWndParent = 0,
		HMENU hMenu = 0);

	HWND Window() const;
	BOOL ShowWindow(int nCmdShow) const;
	BOOL UpdataWindow() const;


public:
	//Handle Message
	static LRESULT	CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
	//Message method
	void		PaintLyric();
	inline void SetLyric(const std::wstring &lyric) { szLyric = lyric; }
	inline void SetFontSize(float inSize) { fontsize = inSize; }
	inline void SetFontColor(LDrawLib::Color color) { fontcolor = color; }
private:

	LDrawLib::Color MakeColor(DWORD dwColor, DWORD dwAplend = 255)
	{
		LDrawLib::Color retcolor;
		DWORD red		= (dwColor & 0xFF0000) >> 16;
		DWORD green		= (dwColor & 0x00FF00) >> 8;
		DWORD blue		= (dwColor & 0x0000FF) >> 0;

		retcolor.a = static_cast<float>(dwAplend) / 255.0f;
		retcolor.r = static_cast<BYTE>(red);
		retcolor.g = static_cast<BYTE>(green);
		retcolor.b = static_cast<BYTE>(blue);
		return retcolor;
	}
private:
	LPCWSTR ClassName() const { return L"Lyric Window Class"; }
	void	OnTimer(UINT timerID);
private:
	LDrawLib::IWriteText*		_writeText;

	std::wstring			szWindowClass;
	std::wstring			szLyric = L"";
	float					fontsize = 20.0f;
	LDrawLib::Color			fontcolor;
	LYRICFORM_CALLBACK		createtimeFunc;
	void*					createtimeParam;

};
#endif