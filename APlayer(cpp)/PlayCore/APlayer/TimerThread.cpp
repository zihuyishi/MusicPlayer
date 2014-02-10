#include "TimerThread.h"

void CTimerThread::KillTimer()
{
	if (_hTimer != NULL) {
		CancelWaitableTimer(_hTimer);
		CloseHandle(_hTimer);
		_hTimer = NULL;
	}
	_bRunning = false;
}
int CTimerThread::CreateTimer(unsigned int interval, PTIMERAPCROUTINE func, void* lpParam)
{
	if (_bRunning) return TRUE;
	_nInterval = interval;
	_callbackFunc = func;
	_lpParam = lpParam;
	_bRunning = true;
	_hThread = CreateThread(
		NULL,
		0,
		timerThread,
		this,
		0,
		&_dwThread
		);
	if (_hThread == NULL) {
		_bRunning = false;
		return FALSE;
	}
	CloseHandle(_hThread);
	return TRUE;
}

DWORD CTimerThread::timerThread(LPVOID lpParam)
{
	CTimerThread* pThis = (CTimerThread*)lpParam;
	do {
		pThis->_hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
		if (pThis->_hTimer == NULL) break;

		LARGE_INTEGER liDueTime;
		liDueTime.QuadPart = 0;
		BOOL bRet = SetWaitableTimer(
			pThis->_hTimer,
			&liDueTime,
			pThis->_nInterval,
			pThis->_callbackFunc,
			pThis->_lpParam,
			FALSE);
		do {
			SleepEx(30000, TRUE);
		} while (pThis->_bRunning);
		return bRet;
	} while (0);
	return 0;
}
