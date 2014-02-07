// testPlayCore.cpp : ����Ӧ�ó������ڵ㡣
//


#include "stdafx.h"
#include "testPlayCore.h"

#include <iostream>

#include "..\PlayCore\PlayCore.h"
#pragma comment( lib, "PlayCore.lib")
#include "..\LyricsForm\LyricsForm.h"
#pragma comment(lib, "LyricsForm.lib")
#include "..\PlayCore\TimerThread.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
ILyricFormController* lyricController;
CTimerThread		g_timeThread;
IPlayController*	player;
// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void				PlayMusic();
VOID	CALLBACK	OnTimer(HWND, UINT, UINT_PTR, DWORD);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TESTPLAYCORE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	lyricController = CreateLyricFormController();
	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTPLAYCORE));

	// ����Ϣѭ��: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTPLAYCORE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TESTPLAYCORE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}
std::wstring lyric[5];
DWORD color[2];
DWORD size[2];
//
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   {
	   lyric[0] = L"ҡ��ҡ ʮ��ҡ������ ����������";
	   lyric[1] = L"�ΰ��� ���ᰢ��������";
	   lyric[2] = L"������ �׻��ǹ���� ���ǳԲ���";
	   lyric[3] = L"������ С�������̰���";
	   lyric[4] = L"...........";

	   color[0] = 0x66ccff;
	   color[1] = 0xff0000;

	   size[0] = 30;
	   size[1] = 30;
   }


   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   PlayMusic();
   lyricController->LyricForm_Run();
   lyricController->LyricForm_SetLyric(L"������������\t��ʣ�������");
   g_timeThread.StartTimer();
   int timeid = SetTimer(hWnd, 1, 2000, NULL);
   return TRUE;
}


VOID CALLBACK OnTimer(HWND, UINT timeId, UINT_PTR, DWORD)
{
	static int i = 0;
	static int nColor = 0;
	static int nSize = 0;
	lyricController->LyricForm_SendCommand(LyricFormCommand::CM_FONTCOLOR, (WPARAM)color[nColor], (LPARAM)255);
	lyricController->LyricForm_SendCommand(LyricFormCommand::CM_FONTSIZE, (WPARAM)size[nSize], NULL);
	unsigned long ms = player->GetPlayMilliseconds();
	std::wstring ly = player->GetLyric();
	if (ly.size() == 0) return;
	wchar_t lpwstr[256];
	wsprintf(lpwstr, L"%s %d", ly.c_str(), ms);
	lyricController->LyricForm_SetLyric(lpwstr);
	i += 1;
	i %= 5;
	nColor += 1;
	nColor %= 2;
	nSize++;
	nSize %= 2;
}
//
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_TIMER:
		OnTimer(hWnd, message, 0, 0);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void PlayMusic()
{
	player = CreateController();
	player->AddMusic(L"D:\\lyric\\����.mp3");
	player->PlayNext();
}