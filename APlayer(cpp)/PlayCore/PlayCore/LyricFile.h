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
	inline int operator()() const
	{
		return ToMillisecond();
	}
	inline size_t ToMillisecond() const
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
public:
	//歌词文件类型
	static const wchar_t LyricFormat[];

	void				SetLyric(const std::wstring& filePath);
	std::wstring		NowLyric(size_t millisecond) const;
	inline std::wstring	GetLyricPath() const { return _filePath; }
	inline int			IsEmpty() const { return _lyrics.size(); }
	void Release() { 
		_filePath.clear();
		_lyrics.clear();
		_extrainfo.clear();
		_lyricindex.clear();
	}
private:
	std::wstring _filePath;
	std::vector<LyricLine> _lyrics;
	std::vector<std::wstring> _extrainfo;
	std::vector<int>	_lyricindex;
};
PLAYCORE_END
#endif