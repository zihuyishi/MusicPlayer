#include "stdafx.h"
#include "PlayCore.h"
#include "PlayController.h"
#include "..\LyricsForm\LyricsForm.h"
#pragma comment(lib, "LyricsForm.lib")
#include <stdlib.h>
#include <time.h>


PlayController::PlayController():
_currentindex(-1), _loopmode(LoopAll)
{
}


PlayController::~PlayController()
{
	
}
//IPlayerController Method
int PlayController::AddMusic(const wchar_t* musicPath)
{
	MusicFile musicFile(musicPath);
	_list.Add(musicFile);
	return true;
}
int PlayController::Play()
{
	if (_list.ListLength() == 0) return FALSE;
	if (_currentindex == -1) {
		_currentindex = 0;
	}
	_player.SetPlayEndCallback(NULL);
	BOOL bRet = _player.Play(_list[_currentindex]);
	if (bRet) {
		_player.SetPlayEndCallback(onPlayEnd, this);
	}
	return bRet;

}
int PlayController::Continue()
{
	return _player.Continue();
}
int PlayController::Pause()
{
	return _player.Pause();
}
int PlayController::Stop()
{
	return _player.Stop();
}
int	PlayController::SetVolume(int value)
{
	return _player.SetVolume(value);
}
int PlayController::PlayNext()
{
	_currentindex = this->getNextIndex();
	return this->Play();
}
unsigned long PlayController::GetPlayMilliseconds()
{
	return _player.GetPlayMilliseconds();
}
std::wstring PlayController::GetLyric()
{
	if (_currentindex < 0) return wstring(L"");
	if (_list.ListLength() == 0) return wstring(L"");
	return _list[_currentindex].GetLyric(
			_player.GetPlayMilliseconds()
			);
}
int PlayController::LoadList(const wchar_t* listPath)
{
	return _list.LoadList(listPath);
}
int PlayController::SaveList(const wchar_t* listPath)
{
	return _list.SaveAs(listPath);
}
void PlayController::Release()
{
	if (this) {
		delete this;
	}
}



//private method
int PlayController::autoPlayNext()
{
	int index = getNextIndex(true);
	if (index == -1) return FALSE;
	_player.Play(_list[index]);
	return TRUE;
}
DWORD PlayController::playNextThread(void *lpParam)
{
	PlayController* playCtrl = (PlayController*)lpParam;
	playCtrl->autoPlayNext();
	return 0;
}
int PlayController::getNextIndex(bool bAuto)
{
	int listLen = _list.ListLength();
	if (listLen == 0) return -1;
	switch (_loopmode) {
	case LoopNo:
		++_currentindex;
		if (!bAuto) break;
		if (_currentindex == listLen) return -1;
		break;
	case LoopAll:
		++_currentindex;
		break;
	case LoopOnce:
		if (!bAuto) ++_currentindex;
		break;
	case LoopRandom:
		srand(static_cast<unsigned int>(time(NULL)));
		_currentindex = rand() % listLen;
		break;
	default:
		++_currentindex;
		break;
	}
	_currentindex %= listLen;
	return _currentindex;
}

int PlayController::onPlayEnd(void *instance, void *lpParam)
{
	PlayController* playctl = (PlayController*)lpParam;
	HANDLE	hThread;
	DWORD	dwThread;
	hThread = CreateThread(
		NULL,
		0,
		playNextThread,
		playctl,
		0,
		&dwThread
		);
	CloseHandle(hThread);
	return 0;
}


