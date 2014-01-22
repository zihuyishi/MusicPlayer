#pragma once
#include "PlayCoreHead.h"

PLAYCORE_BEGIN

class MusicFile
{
public:
	MusicFile(const wchar_t* filePath);
	MusicFile(const wstring &filePath);
	~MusicFile();
	MusicFile& operator=(const MusicFile &musicFile);
	MusicFile(const MusicFile &musicFile) { *this = musicFile; }
public:
	wstring GetFilePath() const;

private:
	wstring _filePath;
	wstring _lyricPath;
private:
	void setInformation();
};

PLAYCORE_END