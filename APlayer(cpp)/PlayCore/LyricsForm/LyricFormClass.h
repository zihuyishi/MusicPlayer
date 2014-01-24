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
	typedef LyricForm ThisType;
public:
	LyricForm()
	{
	}
	~LyricForm()
	{}
public:
	BOOL Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0,
		HMENU hMenu = 0)
	{
		HINSTANCE hInstance = GetModuleHandleW(NULL);
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
		wcex.lpszClassName = ClassName();
		wcex.hIconSm = NULL;
		RegisterClassEx(&wcex);

		m_hWnd = CreateWindowEx(
			dwExStyle, ClassName(), lpWindowName, dwStyle, x,
			y, nWidth, nHeight, hWndParent, hMenu, hInstance, this);
		//DWORD ret = GetLastError();
		return (m_hWnd ? TRUE : FALSE);
	}
	HWND Window() const { return m_hWnd; }
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
		ThisType *pThis = NULL;
		if (message == WM_NCCREATE) {
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (ThisType*)pCreate->lpCreateParams;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);

			pThis->m_hWnd = hWnd;
		}
		else {
			pThis = (ThisType*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		}
		if (pThis) {
			return pThis->HandleMessage(message, wParam, lParam);
		}
		else {
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

	}
	LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CREATE:
			_writeText = CreateDirect2DDrawText(m_hWnd);
			break;
		case WM_PAINT:
			PaintLyric();
			break;
		case WM_DESTROY:
			_writeText->Release();
			PostQuitMessage(0);
			break;
		case CM_LYRIC:
			SetLyric(std::wstring((wchar_t*)wParam));
			PaintLyric();
			break;
		case CM_FONTCOLOR:
			SetFontColor(MakeColor((DWORD)wParam, (DWORD)lParam));
			break;
		case CM_FONTSIZE:
			SetFontSize((float)wParam);
			break;
		default:
			return DefWindowProc(m_hWnd, message, wParam, lParam);
		}
		return 0;
	}
	void PaintLyric()
	{
		PAINTSTRUCT ps;
		BeginPaint(m_hWnd, &ps);
		RECT rc;
		GetClientRect(m_hWnd, &rc);
		_writeText->WriteText(szLyric, rc, fontsize, fontcolor);
		EndPaint(m_hWnd, &ps);
	}
	void SetLyric(const std::wstring &lyric)
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
	LPCWSTR ClassName() const { return L"Lyric Window Class"; }
private:
	IWriteText*		_writeText;

	std::wstring szWindowClass;
	static std::wstring szLyric;
	static float fontsize;
	static Color fontcolor;
};
Color		 LyricForm::fontcolor = { 0, 0, 0, 1.0 };
float		 LyricForm::fontsize = 20.0f;
std::wstring LyricForm::szLyric = L"";
#endif