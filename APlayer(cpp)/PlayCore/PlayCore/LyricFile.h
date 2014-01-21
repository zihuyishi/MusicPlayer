#ifndef APLAYER_LYRICFILE_HEADER
#define APLAYER_LYRICFILE_HEADER
#include "PlayCoreHead.h"

#include <iostream>
#include <vector>
#include <map>
PLAYCORE_BEGIN
typedef struct {
	int minute;
	int second;
	int millisecond;
	inline int operator()()
	{
		return ToMillisecond();
	}
	inline int ToMillisecond()
	{
		return (minute * 60 + second) * 1000 + millisecond;
	}
} LCTime;
typedef struct tagLyricLine{
	LCTime time;
	std::wstring lyric;
} LyricLine;
class LyricFile :
	public UnCopyable
{
public:
	LyricFile()
	{}
	~LyricFile()
	{}
	void GetLyric(std::wstring filePath);
	void Release() { 
		_filePath.clear();
		_lyrics.clear();
		_extrainfo.clear();
	}
private:
	std::wstring _filePath;
	std::vector<LyricLine> _lyrics;
	std::vector<std::wstring> _extrainfo;
};

PLAYCORE_END
#endif