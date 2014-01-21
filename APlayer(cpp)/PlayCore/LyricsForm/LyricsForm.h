#ifndef APLAYER_LYRICSFORM_HEADER
#define APLAYER_LYRICSFORM_HEADER
#ifdef LYRIC_API
//导出函数
#else
//倒入函数
#define LYRIC_API extern "C" __declspec(dllimport)
#endif

enum LyricFormCommand {
	CM_LYRIC		= 0x8000,
	CM_FONTCOLOR	= 0x8001,
	CM_FONTSIZE		= 0x8002,
};

LYRIC_API void __stdcall LyricForm_Run();
LYRIC_API void __stdcall LyricForm_SetLyric(const wchar_t*);
LYRIC_API void __stdcall LyricForm_SendCommand(LyricFormCommand, WPARAM, LPARAM);
#endif