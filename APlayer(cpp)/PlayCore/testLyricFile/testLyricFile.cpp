// testLyricFile.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "..\PlayCore\LyricFile.h"
using namespace PlayCore;
int _tmain(int argc, _TCHAR* argv[])
{
	LyricFile lyric;
	lyric.GetLyric(L"d:\\lyric\\1.lrc");
	LyricFile lyric2;
	lyric2.GetLyric(L"d:\\lyric\\2.lrc");
	return 0;
}

