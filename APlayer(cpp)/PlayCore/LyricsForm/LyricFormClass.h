#include "..\Common\ScopeGuard.h"
#include "DrawLibrary.h"



#ifndef APLAYER_LYRICFORMCLASS_HEADER
#define APLAYER_LYRICFORMCLASS_HEADER


#include "LyricsForm.h"

#include <Windows.h>
#include "resource.h"
#define MyDllName L"LyricForm.dll"

#include <iostream>

 



class LyricForm 
{
public:
	HWND m_hWnd;
public:
	LyricForm() :
		szTitle(L"¸è´Ê"),
		szWindowClass(L"AplayerLyricFormClass")
	{}
	~LyricForm()
	{}
public:
	HWND Create()
	{
		HINSTANCE hInstance = GetModuleHandleW(MyDllName);
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style	= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = wndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szWindowClass.c_str();
		wcex.hIconSm = NULL;
		RegisterClassEx(&wcex);

		m_hWnd = CreateWindow(szWindowClass.c_str(), szTitle.c_str(), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
		if (!m_hWnd) {
			return NULL;
		}
		return m_hWnd;
	}
	BOOL ShowWindow(int nCmdShow)
	{
		if (IsWindow(m_hWnd)) {
			return ::ShowWindow(m_hWnd, nCmdShow);
		}
		return FALSE;
	}
	BOOL UpdataWindow()
	{
		return UpdateWindow(m_hWnd);
	}
public:
	static LRESULT	CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_PAINT:
			PaintLyric(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case CM_LYRIC:
			SetLyric(std::wstring((wchar_t*)wParam));
			PaintLyric(hWnd);
			break;
		case CM_FONTCOLOR:
			SetFontColor(MakeColor((DWORD)wParam, (DWORD)lParam));
			break;
		case CM_FONTSIZE:
			SetFontSize((float)wParam);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
	static void PaintLyric(HWND hWnd)
	{
		IWriteText *writeText = CreateDirect2DDrawText();
		RECT rc;
		GetClientRect(hWnd, &rc);
		writeText->WriteText(hWnd, szLyric, rc, fontsize, fontcolor);
		writeText->Release();
	}
	static void SetLyric(const std::wstring &lyric)
	{
		szLyric = lyric;
	}
	static void SetFontSize(float inSize)
	{
		fontsize = inSize;
	}
	static void SetFontColor(Color color)
	{
		fontcolor = color;
	}
	static Color MakeColor(DWORD dwColor, DWORD dwAplend = 255)
	{
		Color retcolor;
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
	std::wstring szWindowClass;
	std::wstring szTitle;
	static std::wstring szLyric;
	static float fontsize;
	static Color fontcolor;
};
Color		 LyricForm::fontcolor = { 0, 0, 0, 1.0 };
float		 LyricForm::fontsize = 20.0f;
std::wstring LyricForm::szLyric = L"";
#endif