// LyricsForm.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


//Dll source code file exports functions and variables.
#define LYRIC_API extern "C" __declspec(dllexport)
#include "LyricsForm.h"

#include "LyricFormClass.h"
class LyricFormController :
	public ILyricFormController
{
public:
	typedef LyricFormController MyType;
public:
	//ILyricFormController method
	void __stdcall LyricForm_Run()
	{
		if (IsWindow(_wndMain.Window())) {
			_wndMain.ShowWindow(SW_SHOW);
			return;
		}
		_hCreateEvent = CreateEvent(
			NULL,
			TRUE,
			FALSE,
			L"Create Event"
			);
		ON_SCOPE_EXIT([&] { CloseHandle(_hCreateEvent); });
		HANDLE hThread;
		DWORD dwThreadId;
		hThread = CreateThread(
			NULL,
			0,
			LyricFormThread,
			(LPVOID)this,
			0,
			&dwThreadId
			);
		ON_SCOPE_EXIT([&] { CloseHandle(hThread); });
		WaitForSingleObject(_hCreateEvent, 5000);
	}
	//ILyricFormController methods
	void __stdcall LyricForm_SetLyric(const wchar_t* lyric)
	{
		SendMessageW(_wndMain.m_hWnd, LyricFormCommand::CM_LYRIC, (WPARAM)lyric, NULL);
	}

	void __stdcall LyricForm_SetColor(DWORD rgb, int a)
	{
		SendMessageW(_wndMain.m_hWnd, LyricFormCommand::CM_FONTCOLOR, (WPARAM)rgb, (LPARAM)a);
	}
	void __stdcall LyricForm_SendCommand(LyricFormCommand message, WPARAM wParam, LPARAM lParam)
	{
		PostMessage(_wndMain.m_hWnd, message, wParam, lParam);
	}
	void __stdcall Release()
	{
		PostMessage(_wndMain.m_hWnd, LyricFormCommand::CM_CLOSE, NULL, NULL);
		delete this;
	}

private:
	static DWORD WINAPI LyricFormThread(LPVOID lpParam)
	{
		MyType*		pThis = (MyType*)lpParam;
		LyricForm*	wndMain;
		wndMain = &(pThis->_wndMain);
		MSG msg;
		wndMain->Create(L"歌词", WS_OVERLAPPEDWINDOW);
		wndMain->ShowWindow(SW_SHOW);
		wndMain->UpdataWindow();

		SetEvent(pThis->_hCreateEvent);
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return 0;
	}

private:
	LyricForm	_wndMain;
	HANDLE		_hCreateEvent;

};

ILyricFormController* __stdcall CreateLyricFormController()
{
	LyricFormController* controller = new LyricFormController();
	return (ILyricFormController*)controller;
}
