/*****************************************
Author	: Li Chong
Mail	: myyifan@gmail.com

��ʴ�������࣬�������Ƹ�ʴ������ʾ����ɫ
����Ϣ��
******************************************/
#ifndef APLAYER_LYRICSFORM_HEADER
#define APLAYER_LYRICSFORM_HEADER

#include <Windows.h>
#ifdef LYRIC_API
//��������
#else
//���뺯��
#define LYRIC_API extern "C" __declspec(dllimport)
#endif

enum LyricFormCommand {
	CM_LYRIC		= 0x8000,	//�����¸�ʣ�@WPARAM ����std::wstring*
	CM_FONTCOLOR	= 0x8001,	//���ø��������ɫ��@WPARAM ����RGB 0xRRGGBB,@LPARAM ����͸����0-255
	CM_FONTSIZE		= 0x8002,	//���ø�����ִ�С��@WPARAM �����С
	CM_BACKCOLOR	= 0x8003,	//���ô��ڱ�����ɫ��
	CM_BACKIMAGE	= 0x8004,	//���ô��ڱ���ͼƬ��
	CM_CREATETIMER	= 0x8005,	//����һ����ʱ����@WPARAM ���ݼ����@LPARAM ����callback function
};
typedef void(__stdcall *LYRICFORM_CALLBACK)(HWND hWnd, void *lpParam);
class ILyricFormController {
public:
	virtual	void __stdcall LyricForm_Run() = 0;
	virtual void __stdcall LyricForm_SetLyric(const wchar_t*) = 0;
	virtual void __stdcall LyricForm_SendCommand(LyricFormCommand, WPARAM, LPARAM) = 0;
	virtual void __stdcall LyricForm_CreateTimer(unsigned int, LYRICFORM_CALLBACK, void*) = 0;
};

LYRIC_API ILyricFormController* __stdcall CreateLyricFormController();

#endif