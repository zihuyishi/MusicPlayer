#include "GacUI.h"
#include "..\PlayCore\PlayCore.h"
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
	GuiButton*			buttonPlay;
	GuiButton*			buttonAddMusic;
	GuiButton*			buttonPause;
	GuiButton*			buttonNext;
	IPlayController*	player;
private:
	//Controller events
	void buttonPlay_OnClick(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		player->PlayNext();
	}
	void buttonAddMusic_OnClick(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		
		player->AddMusic(L"d:\\1.mp3");
	}
	void buttonPause_OnClick(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		player->Pause();
	}
	void buttonNext_OnClick(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{}
private:
	//³õÊ¼»¯
	void initWindows()
	{
		const int iRows = 6;
		const int iColumns = 3;
		int cControls = 0;
		// limit the size that the window should always show the whole content without cliping it
		this->GetContainerComposition()->
			SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		GuiTableComposition* table = new GuiTableComposition;
		table->SetRowsAndColumns(iRows, iColumns);
		for (int i = 0; i < iRows; ++i) {
			table->SetRowOption(i, GuiCellOption::PercentageOption(0.5));
		}
		for (int j = 0; j < iColumns; ++j) {
			table->SetColumnOption(j, GuiCellOption::PercentageOption(0.5));
		}

		table->SetAlignmentToParent(Margin(4, 4, 4, 4));
		table->SetCellPadding(6);

		this->GetContainerComposition()->AddChild(table);

		//add the play button
		{
			GuiCellComposition* cell = new GuiCellComposition;
			table->AddChild(cell);
			//this cell is the top cell
			cell->SetSite(cControls++, 0, 1, 1);

			buttonPlay = g::NewButton();
			buttonPlay->SetText(L"Play");
			buttonPlay->GetBoundsComposition()->
				SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			buttonPlay->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			buttonPlay->Clicked.AttachMethod(this, 
				&APlayerWindow::buttonPlay_OnClick);
			cell->AddChild(buttonPlay->GetBoundsComposition());
		}
		// add the pause button
		{
			GuiCellComposition* cell = new GuiCellComposition;
			table->AddChild(cell);
			//this cell is the top cell
			cell->SetSite(cControls++, 0, 1, 1);

			buttonPause = g::NewButton();
			buttonPause->SetText(L"Pause");
			buttonPause->GetBoundsComposition()->
				SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			buttonPause->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			buttonPause->Clicked.AttachMethod(this,
				&APlayerWindow::buttonPause_OnClick);
			cell->AddChild(buttonPause->GetBoundsComposition());
		}
		// add the addmusic button
		{
			GuiCellComposition* cell = new GuiCellComposition;
			table->AddChild(cell);
			//this cell is the top cell
			cell->SetSite(cControls++, 0, 1, 1);

			buttonAddMusic = g::NewButton();
			buttonAddMusic->SetText(L"Add");
			buttonAddMusic->GetBoundsComposition()->
				SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			buttonAddMusic->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			buttonAddMusic->GetBoundsComposition()->SetBounds(Rect(0, 0, 100, 100));
			buttonAddMusic->Clicked.AttachMethod(this,
				&APlayerWindow::buttonAddMusic_OnClick);
			cell->AddChild(buttonAddMusic->GetBoundsComposition());
		}

		this->ForceCalculateSizeImmediately();
		this->MoveToScreenCenter();
	}

};

#endif
