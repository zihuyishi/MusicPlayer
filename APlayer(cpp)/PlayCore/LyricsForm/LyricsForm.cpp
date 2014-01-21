// LyricsForm.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


//Dll source code file exports functions and variables.
#define APLAY_API extern "C" __declspec(dllexport)
#include "LyricsForm.h"

#include "LyricFormClass.h"
LyricForm wndMain;

DWORD WINAPI LyricFormThread(LPVOID lpParam)
{
	
	MSG msg;
	wndMain.Create();
	wndMain.ShowWindow(SW_SHOW);
	wndMain.UpdataWindow();

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LYRIC_API void __stdcall LyricForm_Run()
{
	HANDLE hThread;
	DWORD dwThreadId;
	hThread = CreateThread(
		NULL,
		0,
		LyricFormThread,
		0,
		0,
		&dwThreadId
		);
	CloseHandle(hThread);
}
LYRIC_API void __stdcall LyricForm_SetLyric(const wchar_t* lyric)
{
	LyricForm_SendCommand(LyricFormCommand::CM_LYRIC, (WPARAM)lyric, NULL);
}
LYRIC_API void __stdcall LyricForm_SendCommand(LyricFormCommand message, WPARAM wParam, LPARAM lParam)
{
	PostMessage(wndMain.m_hWnd, message, wParam, lParam);
}