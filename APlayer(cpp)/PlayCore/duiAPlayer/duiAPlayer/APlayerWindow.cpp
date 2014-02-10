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

void CAPlayerWnd::Notify(TNotifyUI& msg)
{
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

	return 0;
}
//private method
void CAPlayerWnd::InitControl()
{
	m_pPlayBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("playButton")));
	m_pNextBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("nextButton")));
	m_pAddBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("addButton")));
	m_pLyricBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("lyricButton")));
}