/*************************************
播放器窗体实现类

**************************************/

#include "GacUI.h"
#include "..\PlayCore\PlayCore.h"
#include "..\Common\ScopeGuard.h"
#include "..\LyricsForm\LyricsForm.h"
#pragma comment(lib, "LyricsForm.lib")

#include <Shlobj.h>
#include <Shobjidl.h>
#include <iostream>
#include <vector>
#ifndef APLAYER_APLAYERWINDOW
#define APLAYER_APLAYERWINDOW

class APlayerWindow : public GuiWindow
{
public:
	APlayerWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		initWindows();
		player = CreateController();
	}
	~APlayerWindow()
	{}
private:
	GuiButton*				buttonPlay;
	GuiButton*				buttonAddMusic;
	GuiButton*				buttonPause;
	GuiButton*				buttonNext;
	GuiButton*				buttonLyric;
	IPlayController*		player;
	ILyricFormController*	lyricform;
private:
	//打开文件窗口
	std::vector<std::wstring> showOpenFile();
	//Controller events
	void buttonPlay_OnClick(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		player->PlayNext();
	}
	void buttonAddMusic_OnClick(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		std::vector<std::wstring> filePaths = showOpenFile();
		for (size_t i = 0; i < filePaths.size(); ++i) {
			player->AddMusic(filePaths[i].c_str());
		}
	}
	void buttonPause_OnClick(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		player->Pause();
	}
	void buttonNext_OnClick(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		player->PlayNext();
	}
	void buttonLyric_OnClick(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		lyricform->LyricForm_Run();
		lyricform->LyricForm_SetLyric(L"APlayer v1.0");
	}
private:
	//初始化
	void initWindows();
};

#endif
