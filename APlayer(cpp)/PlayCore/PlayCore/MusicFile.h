#pragma once
#include "PlayCoreHead.h"
#include "LyricFile.h"

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
	wstring				GetFilePath() const;
	inline wstring		GetLyricPath() const { return _lyric.GetLyricPath(); }
	wstring				GetLyric(unsigned long ms) const;
private:
	LyricFile			_lyric;
	wstring				_filePath;
private:
	void				setInformation();
	wstring				GuessLyricPath(const wstring& filepath) const;
};

PLAYCORE_END