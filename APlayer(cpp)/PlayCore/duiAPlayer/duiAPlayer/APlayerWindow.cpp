#include "stdafx.h"
#include "APlayerWindow.h"

#include "..\..\Common\ScopeGuard.h"

#include <Shlobj.h>
#include <Shobjidl.h>
#include <iostream>
#include <vector>

//打开文件对话框
std::vector<std::wstring> showOpenFile()
{
	HRESULT hr = S_OK;
	std::vector<std::wstring> filePaths;

	IFileOpenDialog *fileDlg = NULL;
	hr = CoCreateInstance(CLSID_FileOpenDialog,
		NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&fileDlg));
	if (FAILED(hr)) return filePaths;
	ON_SCOPE_EXIT([&] { fileDlg->Release(); });

	IKnownFolderManager *pkfm = NULL;
	hr = CoCreateInstance(CLSID_KnownFolderManager,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pkfm));
	if (FAILED(hr)) return filePaths;
	ON_SCOPE_EXIT([&] { pkfm->Release(); });

	IKnownFolder *pKnownFolder = NULL;
	hr = pkfm->GetFolder(FOLDERID_PublicMusic, &pKnownFolder);
	if (FAILED(hr)) return filePaths;
	ON_SCOPE_EXIT([&] { pKnownFolder->Release(); });

	IShellItem *psi = NULL;
	hr = pKnownFolder->GetShellItem(0, IID_PPV_ARGS(&psi));
	if (FAILED(hr)) return filePaths;
	ON_SCOPE_EXIT([&] { psi->Release(); });

	hr = fileDlg->AddPlace(psi, FDAP_BOTTOM);
	COMDLG_FILTERSPEC rgSpec[] = {
		{ L"音乐文件", SupportType }
	};
	fileDlg->SetFileTypes(1, rgSpec);

	DWORD dwOptions;
	fileDlg->GetOptions(&dwOptions);
	fileDlg->SetOptions(dwOptions | FOS_ALLOWMULTISELECT);
	hr = fileDlg->Show(NULL);
	if (SUCCEEDED(hr)) {
		IShellItemArray *pRets;
		hr = fileDlg->GetResults(&pRets);
		if (SUCCEEDED(hr)) {
			DWORD count;
			pRets->GetCount(&count);
			for (DWORD i = 0; i < count; i++) {
				IShellItem *pRet;
				LPWSTR nameBuffer;
				pRets->GetItemAt(i, &pRet);
				pRet->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &nameBuffer);
				filePaths.push_back(std::wstring(nameBuffer));
				pRet->Release();
				CoTaskMemFree(nameBuffer);
			}
			pRets->Release();
		}
	}
	return filePaths;
}





//Handle Message
LRESULT CAPlayerWnd::OnPrepare()
{
	return 0;
}
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
	if (::IsIconic(*this)) bHandled = FALSE;
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
	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
	if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom,
			szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}
	bHandled = FALSE;
	return 0;
}
LRESULT	CAPlayerWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
	lpMMI->ptMaxPosition.x = rcWork.left;
	lpMMI->ptMaxPosition.y = rcWork.top;
	lpMMI->ptMaxSize.x = rcWork.right;
	lpMMI->ptMaxSize.y = rcWork.bottom;

	bHandled = FALSE;
	return 0;
}
LRESULT	CAPlayerWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE) {
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(*this) != bZoomed) {
		if (!bZoomed) {
			m_pMaxBtn->SetVisible(false);
			m_pRestoreBtn->SetVisible(true);
		}
		else {
			m_pMaxBtn->SetVisible(true);
			m_pRestoreBtn->SetVisible(false);
		}
	}
	return lRes;
}
LRESULT	CAPlayerWnd::OnNCHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
		CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
		if (pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 &&
			_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
			_tcscmp(pControl->GetClass(), _T("TextUI")) != 0)
			return HTCAPTION;
	}

	return HTCLIENT;
}
//Button Event
LRESULT CAPlayerWnd::PlayButton_OnClicked(CControlUI* pSender, TNotifyUI& msg)
{
	CButtonUI* button = static_cast<CButtonUI*>(pSender);
	if (!_tcscmp(button->GetText(), L"Play")) {
		if (!m_player->PlayNext()) return -1;
		button->SetText(_T("Pause"));
	}
	else if (!_tcscmp(button->GetText(), L"Pause")) {
		m_player->Pause();
		button->SetText(_T("Continue"));
	}
	else {
		m_player->Continue();
		button->SetText(_T("Pause"));
	}
	return 0;
}
LRESULT CAPlayerWnd::AddButton_OnClicked(CControlUI* pSender, TNotifyUI& msg)
{
	auto files = showOpenFile();
	for (size_t i = 0; i < files.size(); ++i) {
		m_player->AddMusic(files[i].c_str());
	}
	return 0;
}
LRESULT CAPlayerWnd::NextButton_OnClicked(CControlUI* pSender, TNotifyUI& msg)
{
	m_player->PlayNext();
	return 0;
}
LRESULT CAPlayerWnd::LyricButton_OnClicked(CControlUI* pSender, TNotifyUI& msg)
{
	m_lyricform->LyricForm_Run();
	m_lyrictimer.CreateTimer(50, lyricTimerFunc, this);
	return 0;
}
//private method
void CAPlayerWnd::InitControl()
{
	//control button
	m_pPlayBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("playButton")));
	m_pNextBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("nextButton")));
	m_pAddBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("addButton")));
	m_pLyricBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("lyricButton")));

	//caption button
	m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("minButton")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("maxButton")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("restoreButton")));
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("closeButton")));
}
VOID CAPlayerWnd::lyricTimerFunc(LPVOID lpParam, DWORD /*dwTimerLowValue*/, DWORD /*dwTimerHighValue*/)
{
	CAPlayerWnd* pThis = static_cast<CAPlayerWnd*>(lpParam);

	pThis->m_lyricform->LyricForm_SetLyric(
		pThis->m_player->GetLyric().c_str()
		);
}
//handle message
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
	case WM_SYSCOMMAND:		lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:			lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:		lRes = OnNCHitTest(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CAPlayerWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("windowinit")) OnPrepare();
	if (msg.sType == _T("click")) {
		if (msg.pSender == m_pPlayBtn) {
			PlayButton_OnClicked(m_pPlayBtn, msg);
		}
		else if (msg.pSender == m_pNextBtn) {
			NextButton_OnClicked(m_pNextBtn, msg);
		}
		else if (msg.pSender == m_pAddBtn) {
			AddButton_OnClicked(m_pAddBtn, msg);
		}
		else if (msg.pSender == m_pLyricBtn) {
			LyricButton_OnClicked(m_pLyricBtn, msg);
		}
		else if (msg.pSender == m_pCloseBtn) {
			PostQuitMessage(0);
			return;
		}
		else if (msg.pSender == m_pMinBtn) {
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		else if (msg.pSender == m_pMaxBtn) {
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
		else if (msg.pSender = m_pRestoreBtn) {
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		}


	}
}