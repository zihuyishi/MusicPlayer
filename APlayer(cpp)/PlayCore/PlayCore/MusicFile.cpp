#include "stdafx.h"
#include "MusicFile.h"

PLAYCORE_BEGIN

MusicFile::MusicFile(const wchar_t *filePath) :
_filePath(filePath)
{
	this->setInformation();
}
MusicFile::MusicFile(const wstring &filePath) :
_filePath(filePath)
{
	this->setInformation();
}
MusicFile::~MusicFile()
{

}
MusicFile& MusicFile::operator=(const MusicFile &musicFile)
{
	this->_filePath = musicFile._filePath;
	this->setInformation();
	return *this;
}
wstring MusicFile::GetFilePath() const
{
	return _filePath;
}

void MusicFile::setInformation()
{
	//…Ë÷√∏Ë¥ Œƒº˛
	wstring lyricpath = GuessLyricPath(_filePath);
	_lyric.SetLyric(lyricpath);
}
wstring MusicFile::GetLyric(unsigned long ms) const
{
	return _lyric.NowLyric(ms);
}
wstring MusicFile::GuessLyricPath(const wstring& filepath) const
{
	int lastDotPos = filepath.find_last_of(L'.');
	wstring lyricpath = filepath.substr(0, lastDotPos+1);
	lyricpath.append(LyricFile::LyricFormat);
	return lyricpath;
}
PLAYCORE_END