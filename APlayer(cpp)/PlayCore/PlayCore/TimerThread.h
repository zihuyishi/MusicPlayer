#ifndef APLAYER_TIMERTHREAD_H
#define APLAYER_TIMERTHREAD_H
#include <Windows.h>
#include <stddef.h>
class CTimerThread
{
public:
	CTimerThread() :
		_hThread(NULL), _mEvent(NULL),
		_bRunning(false), _nInterval(10)
	{}
	~CTimerThread()
	{}
public:
	int						StartTimer();
	void					StopTimer();
	inline UINT64			GetMSecond() { return _millisecond; }
	void					Release();
private:
	static DWORD WINAPI		timerThread(LPVOID lpParam);

private:
	UINT_PTR	_nTimeID;
	UINT64		_millisecond;
	int			_nInterval;

	HANDLE		_mEvent;
	HANDLE		_hThread;
	DWORD		_dwThread;
	bool		_bRunning;
};

#endif