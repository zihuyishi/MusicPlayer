#include "stdafx.h"
#include "CDirect2dClass.h"


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
CDirect2dClass::~CDirect2dClass()
{
	SafeRelease(&_pD2DFactory);
	SafeRelease(&_pRT);
}
HRESULT CDirect2dClass::CreateDevice(HWND _hWnd)
{
	HRESULT hr = S_OK;
	SafeRelease(&_pD2DFactory);
	SafeRelease(&_pRT);
	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&_pD2DFactory);
	RECT _rc;
	GetClientRect(_hWnd, &_rc);
	hr = _pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
		_hWnd,
		D2D1::SizeU(
		_rc.right - _rc.left,
		_rc.bottom - _rc.top)),
		&_pRT);
	return hr;
}
HRESULT CDirect2dClass::DrawRectangle(HWND hWnd, const RECT &rc, Color color)
{
	HRESULT hr = S_OK;
	hr = CreateDevice(hWnd);
	if (FAILED(hr)) return hr;

	ID2D1SolidColorBrush* pBrush = NULL;
	ON_SCOPE_EXIT([&]{ SafeRelease(&pBrush); });

	hr = _pRT->CreateSolidColorBrush(
		D2D1::ColorF(ConvertToColor(color)),
		&pBrush
		);
	_pRT->BeginDraw();
	
	_pRT->DrawRectangle(
		D2D1::RectF(
		(float)rc.left, 
		(float)rc.top,
		(float)rc.right,
		(float)rc.bottom),
		pBrush
		);
	hr = _pRT->EndDraw();
	return hr;
}
HRESULT CDirect2dClass::WriteText(HWND hWnd, const std::wstring& inText, const RECT &rc, float fontSize, Color color)
{
	HRESULT hr = S_OK;
	ID2D1SolidColorBrush*	pBrush = NULL;
	IDWriteFactory*			pDWriteFactory = NULL;
	IDWriteTextFormat*		pTextFormat = NULL;
	ON_SCOPE_EXIT([&] { 
		SafeRelease(&pBrush);
		SafeRelease(&pDWriteFactory);
		SafeRelease(&pTextFormat);
	});
	hr = CreateDevice(hWnd);
	if (FAILED(hr)) return hr;

	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&pDWriteFactory)
		);
	if (FAILED(hr)) return hr;

	UINT32 cTextLength = (UINT32)inText.length();
	hr = pDWriteFactory->CreateTextFormat(
		L"Î¢ÈíÑÅºÚ",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"en-us",
		&pTextFormat
		);
	hr = pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	hr = pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	D2D1::ColorF brushColor = ConvertToColor(color);
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
	_pRT->DrawTextW(
		inText.c_str(),
		cTextLength,
		pTextFormat,
		layoutRect,
		pBrush
		);
	_pRT->EndDraw();
	return hr;
}
