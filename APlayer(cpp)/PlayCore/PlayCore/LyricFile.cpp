#include "stdafx.h"
#include "LyricFile.h"

#include <fstream>
#include <windows.h>
#include <Stringapiset.h>
#include <string.h>
using std::ifstream;
using std::wstring;
PLAYCORE_BEGIN

const wchar_t LyricFile::LyricFormat[] = L"lrc";

bool IsLyric(const wstring &src)
{
	size_t lpos = src.find(L'[');
	size_t rpos = src.find(L']');
	if (rpos - lpos != 9)
		return false;
	int testpos[6] = { 1, 2, 4, 5, 7, 8 };
	for (int i = 0; i < 6; ++i)
	{
		if (src[testpos[i]] < L'0' || src[testpos[i]] > L'9')
			return false;
	}
	return true;
}
LCTime lyricTime(const wstring &src)
{
	size_t lpos = src.find(L'[');
	LCTime rettime;
	wstring minute, second, millisecond;
	minute			= src.substr(lpos + 1, 2);
	second			= src.substr(lpos + 4, 2);
	millisecond		= src.substr(lpos + 7, 2);
	rettime.minute = _wtoi(minute.c_str());
	rettime.second = _wtoi(second.c_str());
	rettime.millisecond = _wtoi(millisecond.c_str());
	return rettime;
}
wstring lyricString(const wstring &src)
{
	size_t rpos = src.find(L']');
	wstring retstring = src.substr(rpos + 1, src.length());
	return retstring;
}
void LyricFile::SetLyric(const std::wstring& filePath)
{
	const int linelength = 256;
	char line[linelength];
	wchar_t wline[linelength];
	ifstream inFile;
	inFile.open(filePath.c_str(), std::ifstream::in);
	while (inFile.getline(line, 256)) {
		MultiByteToWideChar(CP_ACP, MB_COMPOSITE, line, -1,
			wline, linelength);
		LyricLine lyricline;
		if (IsLyric(wline)) {
			lyricline.time = lyricTime(wline);
			lyricline.lyric = lyricString(wline);
			_lyrics.push_back(lyricline);
		}
		else {
			_extrainfo.push_back(wline);
		}
	}

}
std::wstring LyricFile::NowLyric(size_t millisecond) const
{
	wstring ret(L"");
	size_t index;
	for (index = 0; index < _lyrics.size(); ++index) {
		if (_lyrics[index].time.ToMillisecond() > millisecond)
		{
			if (index > 0){
				ret = _lyrics[index].lyric;
			}
			break;
		}
	}
	return ret;
}
PLAYCORE_END