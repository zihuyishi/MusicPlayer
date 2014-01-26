#include "stdafx.h"
#include "TimerThread.h"
inline void SafeCloseHandle(HANDLE &handle)
{
	if (handle != NULL) {
		CloseHandle(handle);
		handle = NULL;
	}
}
DWORD WINAPI CTimerThread::timerThread(LPVOID lpParam)
{
	CTimerThread* pThis = (CTimerThread*)lpParam;
	pThis->_nTimeID = SetTimer(NULL, 0, pThis->_nInterval, NULL);
	MSG msg;
	SetEvent(pThis->_mEvent);

	while (pThis->_bRunning && GetMessage(&msg, NULL, 0, 0))
	{
		switch (msg.message) {
		case WM_TIMER:
			if (msg.wParam == pThis->_nTimeID) {
				pThis->_millisecond += pThis->_nInterval;
			}
			break;
		default:
			break;
		}
	}
	KillTimer(NULL, pThis->_nTimeID);
	return 0;
}
int	CTimerThread::StartTimer()
{
	_mEvent = CreateEvent(
		NULL,
		TRUE,
		FALSE,
		L"timerthread normal event"
		);
	_bRunning = true;
	_millisecond = 0;
	_hThread = CreateThread(
		NULL,
		0,
		timerThread,
		this,
		0,
		&_dwThread);

	WaitForSingleObject(_mEvent, 5000);
	return 0;
}
void CTimerThread::StopTimer()
{
	_bRunning = false;
	WaitForSingleObject(_hThread, 5000);
	Release();
}
void CTimerThread::Release()
{
	SafeCloseHandle(_hThread);
	SafeCloseHandle(_mEvent);
}