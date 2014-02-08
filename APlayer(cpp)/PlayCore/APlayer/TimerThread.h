#ifndef APLAYER_TIMERTHREAD_H
#define APLAYER_TIMERTHREAD_H
#include <Windows.h>
class CTimerThread
{
public:
	CTimerThread() :
		_hThread(NULL), _hEvent(NULL),
		_bRunning(false), _nInterval(10)
	{}
	~CTimerThread()
	{
		KillTimer();
	}
public:
	int						CreateTimer(unsigned int interval, PTIMERAPCROUTINE func, void* lpParam);
	void					KillTimer();
private:
	static DWORD WINAPI		timerThread(LPVOID lpParam);

private:
	HANDLE				_hTimer;
	int					_nInterval;
	HANDLE				_hEvent;
	void*				_lpParam;

	PTIMERAPCROUTINE	_callbackFunc;
	HANDLE				_hThread;
	DWORD				_dwThread;
	bool				_bRunning;
};

#endif