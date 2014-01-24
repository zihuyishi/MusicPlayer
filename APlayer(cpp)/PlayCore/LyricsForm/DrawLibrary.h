
#include <windows.h>
#include <iostream>
#ifndef LLIB_DRAWLIBRARY_HEADER
#define LLIB_DRAWLIBRARY_HEADER
namespace LDrawLib{
	typedef struct{
		BYTE r, g, b;
		float a;
	}	Color;
	struct IWriteText {
		/*@std::wstring 文字，@RECT& 位置， @float文字大小， @Color 文字颜色*/
		virtual HRESULT WriteText(const std::wstring&, const RECT&, float, Color) = 0;
		virtual HRESULT OnFormChange() = 0;
		virtual void	Release() = 0;
	};

	IWriteText *CreateDirect2DDrawText(HWND hWnd);
	IWriteText *CreateGdiplusDrawText(HWND hWnd);
}
#endif