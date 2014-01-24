#include "stdafx.h"

#include "CGdiplusClass.h"
using namespace Gdiplus;
HRESULT CGdiplusClass::WriteText(const std::wstring& lyric,
	const RECT& rc, float fontsize, LDrawLib::Color fontcolor)
{
	HDC				hdc = GetWindowDC(_hWnd);
	Graphics		graphics(hdc);
	SolidBrush		brush(ConvertToColor(fontcolor));
	FontFamily		fontFamily(L"Î¢ÈíÑÅºÚ");
	Font			font(&fontFamily,
						static_cast<int>(fontsize),
						FontStyleRegular,
						UnitPixel);
	StringFormat	strFormat;
	RectF			rectF(
					static_cast<float>(rc.left),
					static_cast<float>(rc.top),
					static_cast<float>(rc.right-rc.left),
					static_cast<float>(rc.bottom-rc.top)
					);

	strFormat.SetLineAlignment(StringAlignmentCenter);
	strFormat.SetAlignment(StringAlignmentCenter);
	graphics.Clear(_backColor);
	graphics.DrawString(lyric.c_str(), lyric.length(), &font, rectF, &strFormat, &brush);
	return S_OK;
}