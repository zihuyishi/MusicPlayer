#include "stdafx.h"
#include "PlayCore.h"
#include "PlayController.h"
#include "..\LyricsForm\LyricsForm.h"
#pragma comment(lib, "LyricsForm.lib")
#include <stdlib.h>
#include <time.h>

PlayController::PlayController():
_currentindex(0), _loopmode(LoopAll)
{
	
}


PlayController::~PlayController()
{
	
	
}

int PlayController::AddMusic(const wchar_t* musicPath)
{
	MusicFile musicFile(musicPath);
	_list.Add(musicFile);
	return true;
}
int PlayController::Play()
{
	return _player.Play();
}
int PlayController::Pause()
{
	return _player.Pause();
}
int PlayController::Stop()
{
	return _player.Stop();
}
int PlayController::PlayNext()
{
	int index = this->getNextIndex();
	if (index == -1) return 0;
	_player.Play(_list[index]);
	return TRUE;
}
void PlayController::Release()
{
	delete this;
}

int PlayController::getNextIndex()
{
	int listLen = _list.ListLength();
	if (listLen == 0) return -1;
	switch (_loopmode) {
	case LoopNo:
		++_currentindex;
		if (_currentindex == listLen) return -1;
		break;
	case LoopAll:
		++_currentindex;
		break;
	case LoopOnce:
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

int __stdcall PlayController::ShowLyric()
{
	LyricForm_Run();
	_lyric.GetLyric(L"d:\\lyric\\1.lrc");
	LyricForm_SetLyric(L"test");
	return 0;
}