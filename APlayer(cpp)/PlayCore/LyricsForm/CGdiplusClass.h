
#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>

#pragma comment(lib, "Gdiplus.lib")
#include "DrawLibrary.h"

#ifndef LLIB_CGDIPLUSCLASS_HEADER
#define LLIB_CGDIPLUSCLASS_HEADER


class CGdiplusClass :
	public LDrawLib::IWriteText
{
public:
	CGdiplusClass()
	{   // Initialize GDI+.
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	}
	~CGdiplusClass()
	{
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}
public:
	// IWriteText
	HRESULT WriteText(const std::wstring&, const RECT&, float, LDrawLib::Color);
	HRESULT OnFormChange() 
	{
		return S_OK;
	}
	void	Release() 
	{
		delete this;
	}
public:
	inline void	SetTargetHwnd(HWND hWnd) { _hWnd = hWnd; }
private:
	inline Gdiplus::Color ConvertToColor(LDrawLib::Color color)
	{
		BYTE a = static_cast<BYTE>(color.a * 255);
		Gdiplus::Color temp(a, color.r, color.g, color.b);
		return temp;
	}
private:
	HWND							_hWnd = NULL;
	Gdiplus::Color					_backColor = Gdiplus::Color(255, 255, 255);
	Gdiplus::GdiplusStartupInput	gdiplusStartupInput;
	ULONG_PTR						gdiplusToken;
};


LDrawLib::IWriteText *LDrawLib::CreateGdiplusDrawText(HWND hWnd)
{
	CGdiplusClass* writetext = new CGdiplusClass();
	writetext->SetTargetHwnd(hWnd);
	return (LDrawLib::IWriteText*)writetext;
}
#endif