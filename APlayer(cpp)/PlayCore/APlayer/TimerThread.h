/********************************
author:
	zihuyishi 516378258@qq.com

version 1.0
一个定时器线程，每隔相应时间调用函数。
*********************************/

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