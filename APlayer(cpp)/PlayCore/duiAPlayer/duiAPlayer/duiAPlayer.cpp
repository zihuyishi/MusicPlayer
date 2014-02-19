// duiAPlayer.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "APlayerWindow.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	
	CAPlayerWnd *pAplayerWnd = new CAPlayerWnd();
	if (pAplayerWnd == NULL) return 0;
	
	pAplayerWnd->Create(NULL, _T("APlayer"), UI_WNDSTYLE_FRAME, 0L);
	pAplayerWnd->ShowWindow(true);
	CPaintManagerUI::MessageLoop();
	
	delete pAplayerWnd;
	
	return 0;
}

