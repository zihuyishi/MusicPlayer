// LyricsForm.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


//Dll source code file exports functions and variables.
#define APLAY_API extern "C" __declspec(dllexport)
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

		HANDLE hThread;
		DWORD dwThreadId;
		hThread = CreateThread(
			NULL,
			0,
			LyricFormThread,
			(LPVOID)&_wndMain,
			0,
			&dwThreadId
			);
		CloseHandle(hThread);
	}

	void __stdcall LyricForm_SetLyric(const wchar_t* lyric)
	{
		SendMessageW(_wndMain.m_hWnd, LyricFormCommand::CM_LYRIC, (WPARAM)lyric, NULL);
	}

	void __stdcall LyricForm_SendCommand(LyricFormCommand message, WPARAM wParam, LPARAM lParam)
	{
		PostMessage(_wndMain.m_hWnd, message, wParam, lParam);
	}
private:
	static DWORD WINAPI LyricFormThread(LPVOID lpParam)
	{
		LyricForm* wndMain = (LyricForm*)lpParam;
		MSG msg;
		wndMain->Create(L"歌词", WS_OVERLAPPEDWINDOW);
		wndMain->ShowWindow(SW_SHOW);
		wndMain->UpdataWindow();

		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return 0;
	}

private:
	LyricForm	_wndMain;
};
ILyricFormController* __stdcall CreateLyricFormController()
{
	LyricFormController* controller = new LyricFormController();
	return (ILyricFormController*)controller;
}
