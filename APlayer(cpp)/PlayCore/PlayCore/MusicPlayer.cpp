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
	bret = this->Play();
	return bret;
}

BOOL MusicPlayer::Pause() 
{
	return _player->Pause();
}
unsigned long MusicPlayer::GetPlayMilliseconds()
{
	TStreamTime pTime;
	_player->GetPosition(&pTime);
	unsigned long retMS = ((pTime.hms.hour * 60 + pTime.hms.minute) * 60 +
		pTime.hms.second) * 1000 + pTime.hms.millisecond;
	return retMS;
}
BOOL MusicPlayer::Stop()
{
	return _player->Stop();
}
PLAYCORE_END