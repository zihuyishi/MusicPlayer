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

}

PLAYCORE_END