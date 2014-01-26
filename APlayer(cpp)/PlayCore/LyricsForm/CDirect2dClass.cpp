#include "stdafx.h"
#include "CDirect2dClass.h"

//SafeRelease
template<class Interface>
inline void SafeRelease(
	Interface **ppInterfaceToRelease
	)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}
/***
Construct and Deconstruct Methods
*/
CDirect2dClass::CDirect2dClass()
{
	CreateFactory();
}
CDirect2dClass::~CDirect2dClass()
{
	SafeRelease(&_pD2DFactory);
	SafeRelease(&_pDWriteFactory);
	DiscardD2DResource();
}
/***
Direct2D Resource Function
*/
HRESULT CDirect2dClass::CreateFactory()
{
	HRESULT hr = S_OK;
	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&_pD2DFactory);
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&_pDWriteFactory)
		);
	return hr;
}
HRESULT CDirect2dClass::CreateD2DResource()
{
	HRESULT hr = S_OK;
	if (_pRT == NULL) {
		RECT _rc;
		GetClientRect(_hWnd, &_rc);
		D2D1_SIZE_U size = D2D1::SizeU(_rc.right, _rc.bottom);

		hr = _pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(_hWnd, size),
			&_pRT);
	}
	return hr;
}
void CDirect2dClass::DiscardD2DResource()
{
	SafeRelease(&_pRT);
}

/****
public methods
*/
void CDirect2dClass::SetTargetHwnd(HWND hWnd)
{
	_hWnd = hWnd;
	DiscardD2DResource();
}
HRESULT CDirect2dClass::DrawRectangle(const RECT &rc, Color color)
{
	HRESULT hr = S_OK;
	hr = CreateD2DResource();
	if (FAILED(hr)) return hr;

	ID2D1SolidColorBrush*	pBrush = NULL;
	ON_SCOPE_EXIT([&]{ SafeRelease(&pBrush); });

	hr = _pRT->CreateSolidColorBrush(
		D2D1::ColorF(ConvertToColor(color)),
		&pBrush
		);
	_pRT->BeginDraw();
	_pRT->Clear(D2D1::ColorF(_backColor));
	_pRT->DrawRectangle(
		D2D1::RectF(
		(float)rc.left, 
		(float)rc.top,
		(float)rc.right,
		(float)rc.bottom),
		pBrush
		);
	hr = _pRT->EndDraw();
	if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
		DiscardD2DResource();
	}
	return hr;
}
//IWriteText
HRESULT CDirect2dClass::WriteText(const std::wstring& inText, const RECT &rc, float fontSize, Color fontcolor)
{
	HRESULT hr = S_OK;

	hr = CreateD2DResource();
	if (FAILED(hr)) return hr;

	ID2D1SolidColorBrush*	pBrush = NULL;
	IDWriteTextFormat*		pTextFormat = NULL;
	ON_SCOPE_EXIT([&] { 
		SafeRelease(&pBrush);
		SafeRelease(&pTextFormat);
	});

	UINT32 cTextLength = (UINT32)inText.length();
	hr = _pDWriteFactory->CreateTextFormat(
		L"Î¢ÈíÑÅºÚ",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"zh-cn",
		&pTextFormat
		);
	hr = pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	hr = pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	D2D1::ColorF brushColor = ConvertToColor(fontcolor);
	hr = _pRT->CreateSolidColorBrush(
		D2D1::ColorF(brushColor),
		&pBrush
		);
	if (FAILED(hr)) return hr;

	D2D1_RECT_F layoutRect = D2D1::RectF(
		static_cast<float>(rc.left),
		static_cast<float>(rc.top),
		static_cast<float>(rc.right - rc.left),
		static_cast<float>(rc.bottom - rc.top)
		);
	_pRT->BeginDraw();
	_pRT->Clear(D2D1::ColorF(_backColor));
	_pRT->DrawTextW(
		inText.c_str(),
		cTextLength,
		pTextFormat,
		layoutRect,
		pBrush
		);
	hr = _pRT->EndDraw();
	if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
		DiscardD2DResource();
	}
	return hr;
}

HRESULT CDirect2dClass::OnFormChange()
{
	DiscardD2DResource();
	return S_OK;
}