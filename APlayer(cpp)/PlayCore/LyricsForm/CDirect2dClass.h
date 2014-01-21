#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")
#include <dwrite.h>
#pragma comment(lib, "dwrite.lib")
#include "DrawLibrary.h"
#include <iostream>
#include "..\Common\ScopeGuard.h"

#ifndef LLIB_CDIRECT2D_HEADER
#define LLIB_CDIRECT2D_HEADER


class CDirect2dClass :
public IWriteText
{
public:
	CDirect2dClass() :
		_pD2DFactory(NULL), _pRT(NULL)
	{}
	~CDirect2dClass();
public:
	HRESULT DrawRectangle(HWND hWnd, const RECT &rc, 
		Color color = { 0, 0, 0 , 1.0f});
	HRESULT DrawEllipse();
	HRESULT WriteText(HWND hWnd, const std::wstring& inText,
		const RECT &rc, float fontSize = 12.0f,
		Color color = { 0, 0, 0, 1.0f });
	void Release() { delete this; }
//uncopyable
private:
	CDirect2dClass(const CDirect2dClass&){}
	CDirect2dClass& operator=(const CDirect2dClass&) {}
private:
	HRESULT CreateDevice(HWND _hWnd);
	inline D2D1::ColorF ConvertToColor(Color color)
	{
		D2D1::ColorF _color(
			static_cast<float>(color.r) / 255.0f,
			static_cast<float>(color.g) / 255.0f,
			static_cast<float>(color.b) / 255.0f,
			color.a
			);
		return _color;
	}
private:
	ID2D1Factory*			_pD2DFactory;
	ID2D1HwndRenderTarget*	_pRT;
};
IWriteText *CreateDirect2DDrawText()
{
	CDirect2dClass *pD2C = new CDirect2dClass();
	return (IWriteText*)pD2C;
}

#endif