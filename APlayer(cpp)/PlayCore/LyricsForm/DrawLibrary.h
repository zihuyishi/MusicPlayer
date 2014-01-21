
#include <windows.h>
#include <iostream>
#ifndef LLIB_DRAWLIBRARY_HEADER
#define LLIB_DRAWLIBRARY_HEADER

typedef struct{
	BYTE r, g, b;
	float a;
}	Color;
struct IWriteText {
	virtual HRESULT WriteText(HWND hWnd, const std::wstring&, const RECT&, float, Color) = 0;
	virtual void Release() = 0;
};

IWriteText *CreateDirect2DDrawText();


#endif