#include "stdafx.h"
#include "MusicPlayer.h"

PLAYCORE_BEGIN

MusicPlayer::MusicPlayer() :
_bPlayEnd(FALSE), _onPlayEnd(NULL),
_onPlayEndPram(NULL)
{
	_player = CreateZPlay();
	_player->SetCallbackFunc(this->TCallbackOnPlayEnd, TCallbackMessage::MsgStop, this);
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
	return this->_play(wstring(filePath));
}
BOOL MusicPlayer::Play(const MusicFile &musicFile)
{
	return this->_play(musicFile.GetFilePath());
}
BOOL MusicPlayer::Play(const wstring& filePath)
{
	return this->_play(filePath);
}
BOOL MusicPlayer::_play(const wstring& filePath)
{
	BOOL bret = 0;
	bret = _player->Close();
	bret = _player->OpenFileW(filePath.c_str(), TStreamFormat::sfAutodetect);
	if (!bret) return bret;
	bret = this->Play();
	_bPlayEnd = FALSE;
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
int MusicPlayer::SetVolume(int value)
{
	if (value > 100) value = 100;
	if (value < 0) value = 0;
	_player->SetPlayerVolume(value, value);
	return value;
}
BOOL MusicPlayer::SetPlayEndCallback(MCallbackFunc pCallbackFunc, void *lpParam)
{
	this->_onPlayEnd = pCallbackFunc;
	this->_onPlayEndPram = lpParam;
	return TRUE;
}

int	MusicPlayer::TCallbackOnPlayEnd(void *instance,
	void *user_data, TCallbackMessage message,
	unsigned int param1, unsigned int param2)
{
	ZPlay *pInstance = (ZPlay*)instance;
	MusicPlayer* musicPlayer = (MusicPlayer*)user_data;

	switch (message) {
	case TCallbackMessage::MsgStop:
		musicPlayer->_bPlayEnd = TRUE;
		if (musicPlayer->_onPlayEnd != NULL) {
			musicPlayer->_onPlayEnd(musicPlayer, musicPlayer->_onPlayEndPram);
		}
		break;
	}

	return 0;
}
PLAYCORE_END