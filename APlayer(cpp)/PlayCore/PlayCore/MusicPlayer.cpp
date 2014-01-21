#include "stdafx.h"
#include "MusicPlayer.h"

PLAYCORE_BEGIN

MusicPlayer::MusicPlayer()
{
	_player = CreateZPlay();
}


MusicPlayer::~MusicPlayer()
{
	_player->Release();
}

BOOL MusicPlayer::Play()
{
	return _player->Play();
}
BOOL MusicPlayer::Play(const wchar_t* filePath)
{
	return this->Play(wstring(filePath));
}
BOOL MusicPlayer::Play(const MusicFile &musicFile)
{
	return this->Play(musicFile.GetFilePath());
}
BOOL MusicPlayer::Play(const wstring filePath)
{
	BOOL bret = 0;
	bret = _player->Close();
	bret = _player->OpenFileW(filePath.c_str(), TStreamFormat::sfAutodetect);

	if (!bret) return bret;
	bret = _player->Play();
	return bret;
}

BOOL MusicPlayer::Pause() 
{
	return _player->Pause();
}

BOOL MusicPlayer::Stop()
{
	return _player->Stop();
}
PLAYCORE_END