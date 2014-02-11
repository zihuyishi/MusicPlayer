#ifndef APLAYER_DUIPLAYER_HEADER
#define APLAYER_DUIPLAYER_HEADER

#include "resource.h"


#include <windows.h>
#include <objbase.h>
/*************************************************/
//include duilib
#include "DuiLib\UIlib.h"
using namespace DuiLib;

#include "..\..\PlayCore\PlayCore.h"
#include "..\..\LyricsForm\LyricsForm.h"
#include "..\..\Common\TimerThread.h"

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif
/***************************************************/

class CAPlayerWnd :
	public CWindowWnd, public INotifyUI
{
public:
	CAPlayerWnd()
	{
		m_player = CreateController();
		m_lyricform = CreateLyricFormController();
	};
	~CAPlayerWnd()
	{
		m_player->Release();
	};
	LPCTSTR			GetWindowClassName() const { return _T("APlayerWindow"); }
	UINT			GetClassStyle() const { return CS_DBLCLKS; }
	void			OnFinalMessage(HWND /*hWnd*/) { delete this; }
	void			Notify(TNotifyUI& msg);
	LRESULT			HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

public:
	//Handle Message 
	LRESULT			OnPrepare();
	LRESULT			OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT			OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT			OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT			OnNCActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT			OnNCCalcsize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT			OnNCPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT			OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT			OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT			OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT			OnNCHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	//Button Event
	LRESULT			PlayButton_OnClicked(CControlUI* pSender, TNotifyUI& msg);
	LRESULT			NextButton_OnClicked(CControlUI* pSender, TNotifyUI& msg);
	LRESULT			AddButton_OnClicked(CControlUI* pSender, TNotifyUI& msg);
	LRESULT			LyricButton_OnClicked(CControlUI* pSender, TNotifyUI& msg);
	LRESULT			LoadListButton_OnClicked(CControlUI* pSender, TNotifyUI& msg);
	LRESULT			SaveListButton_OnClicked(CControlUI* pSender, TNotifyUI& msg);
private:
	static VOID APIENTRY lyricTimerFunc(LPVOID lpParam, DWORD /*dwTimerLowValue*/,DWORD /*dwTimerHighValue*/);
private:
	void InitControl();
private:
	CPaintManagerUI			m_PaintManager;
	IPlayController*		m_player;
	ILyricFormController*	m_lyricform;
	CTimerThread			m_lyrictimer;
private:
	//controls
	CButtonUI*			m_pPlayBtn;
	CButtonUI*			m_pNextBtn;
	CButtonUI*			m_pAddBtn;
	CButtonUI*			m_pLyricBtn;
	CButtonUI*			m_pLoadListBtn;
	CButtonUI*			m_pSaveListBtn;
	//min max close button
	CButtonUI*			m_pMinBtn;
	CButtonUI*			m_pMaxBtn;
	CButtonUI*			m_pRestoreBtn;
	CButtonUI*			m_pCloseBtn;

};
#endif