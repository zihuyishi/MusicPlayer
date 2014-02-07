#include "stdafx.h"
#include "LyricFormClass.h"

//public method
BOOL LyricForm::Create(
	PCWSTR lpWindowName,
	DWORD dwStyle,
	DWORD dwExStyle,
	int x,
	int y,
	int nWidth ,
	int nHeight,
	HWND hWndParent ,
	HMENU hMenu)
{
	HINSTANCE hInstance = GetModuleHandleW(NULL);
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
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
HWND LyricForm::Window() const { return m_hWnd; }
BOOL LyricForm::ShowWindow(int nCmdShow) const
{
	if (IsWindow(m_hWnd)) {
		return ::ShowWindow(m_hWnd, nCmdShow);
	}
	return FALSE;
}
BOOL LyricForm::UpdataWindow() const
{
	return UpdateWindow(m_hWnd);
}

//Message Handle
LRESULT	CALLBACK LyricForm::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
LRESULT LyricForm::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		_writeText = LDrawLib::CreateDirect2DDrawText(m_hWnd);
		//_writeText = LDrawLib::CreateGdiplusDrawText(m_hWnd);
		break;
	case WM_PAINT:
		PaintLyric();
		break;
	case WM_DESTROY:
		_writeText->Release();
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		_writeText->OnFormChange();
		break;
	case WM_TIMER:
		OnTimer((UINT)wParam, (LYRICFORM_CALLBACK)lParam);
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
	case CM_CREATETIMER:
		SetTimer(m_hWnd, 1, (UINT)wParam, (TIMERPROC)lParam);
		break;
	default:
		return DefWindowProc(m_hWnd, message, wParam, lParam);
	}
	return 0;
}

//message method
void LyricForm::PaintLyric()
{
	PAINTSTRUCT ps;
	BeginPaint(m_hWnd, &ps);
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	_writeText->WriteText(szLyric, rc, fontsize, fontcolor);
	EndPaint(m_hWnd, &ps);
}

void LyricForm::OnTimer(UINT timerID, LYRICFORM_CALLBACK func)
{
	if (func != NULL) {
		func(m_hWnd, func);
	}
}