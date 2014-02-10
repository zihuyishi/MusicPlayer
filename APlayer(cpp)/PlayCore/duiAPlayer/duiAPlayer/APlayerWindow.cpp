#include "stdafx.h"
#include "APlayerWindow.h"

void CAPlayerWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		if (msg.pSender == m_pPlayBtn) {
			PlayButton_OnClicked(m_pPlayBtn, msg);
		}
		else if (msg.pSender == m_pStopBtn) {
			StopButton_OnClicked(m_pStopBtn, msg);
		}
		else if (msg.pSender == m_pAddBtn) {
			AddButton_OnClicked(m_pAddBtn, msg);
		}
		else if (msg.pSender == m_pLyricBtn) {
			LyricButton_OnClicked(m_pLyricBtn, msg);
		}
	}
}

LRESULT CAPlayerWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_CREATE:			lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:			lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:		lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:		lRes = OnNCActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:		lRes = OnNCCalcsize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:		lRes = OnNCPaint(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}


//Handle Message
LRESULT CAPlayerWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_PaintManager.Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("WndXml\\APlayerWnd.xml"), (UINT)0, NULL, &m_PaintManager);
	ASSERT(pRoot && "Failed to parse APlayerWnd.xml");
	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);

	InitControl();
	return 0;
}
LRESULT CAPlayerWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}
LRESULT CAPlayerWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0);

	bHandled = FALSE;
	return 0;
}
LRESULT CAPlayerWnd::OnNCActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsIconic(m_hWnd)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}
LRESULT CAPlayerWnd::OnNCCalcsize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}
LRESULT CAPlayerWnd::OnNCPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}
LRESULT CAPlayerWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}


//Button Event
LRESULT CAPlayerWnd::PlayButton_OnClicked(CControlUI* pSender, TNotifyUI& msg)
{
	CButtonUI* button = static_cast<CButtonUI*>(pSender);
	if (_tcscmp(button->GetText(), L"Play")) {
		button->SetText(_T("Play"));
	}
	else {
		button->SetText(_T("Pause"));
	}
	return 0;
}
LRESULT CAPlayerWnd::AddButton_OnClicked(CControlUI* pSender, TNotifyUI& msg)
{
	return 0;
}
LRESULT CAPlayerWnd::StopButton_OnClicked(CControlUI* pSender, TNotifyUI& msg)
{
	return 0;
}
LRESULT CAPlayerWnd::LyricButton_OnClicked(CControlUI* pSender, TNotifyUI& msg)
{
	return 0;
}
//private method
void CAPlayerWnd::InitControl()
{
	m_pPlayBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("playButton")));
	m_pStopBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("stopButton")));
	m_pAddBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("addButton")));
	m_pLyricBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("lyricButton")));
}