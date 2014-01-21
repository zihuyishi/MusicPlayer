#include "GacUI.h"
#include "GacUIWindows.h"
#include <ShlObj.h>
#ifndef APLAYER_VIEWSWITCHINGWINDOW
#define APLAYER_VIEWSWITCHINGWINDOW
using namespace vl;
using namespace vl::collections;
using namespace vl::presentation::windows;
/***********************************************************************
File System Operations
***********************************************************************/

extern WString GetWindowsDirectory();
extern void SearchDirectoriesAndFiles(const WString& path, List<WString>& directories, List<WString>& files);
extern Ptr<GuiImageData> GetFileIcon(const WString& fullPath, UINT uFlags);
extern WString GetFileDisplayName(const WString& fullPath);
extern WString GetFileTypeName(const WString& fullPath);
extern FILETIME GetFileLastWriteTime(const WString& fullPath);
extern LARGE_INTEGER GetFileSize(const WString& fullPath);
extern WString FileTimeToString(const FILETIME& filetime);
extern WString FileSizeToString(LARGE_INTEGER filesize);

/***********************************************************************
FileProperties
***********************************************************************/

class FileProperties
{
private:
	Ptr<GuiImageData> smallIcon;
	Ptr<GuiImageData> bigIcon;
	WString             displayName;
	WString             typeName;
	FILETIME            lastWriteTime;
	LARGE_INTEGER       size;

	bool                loaded;
	WString             fullPath;

	void                Load();
public:
	FileProperties(const WString& _fullPath);

	Ptr<GuiImageData> GetSmallIcon();
	Ptr<GuiImageData> GetBigIcon();
	WString             GetDisplayName();
	WString             GetTypeName();
	FILETIME            GetLastWriteTime();
	LARGE_INTEGER       GetSize();
};

extern void FillData(GuiListView* listView);

/***********************************************************************
ViewSwitchingWindow
***********************************************************************/

class ViewSwitchingWindow : public GuiWindow
{
private:
	GuiListView*                    listView;
	GuiComboBoxListControl*         comboView;

	void comboView_SelectedIndexChanged(GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		switch (comboView->GetSelectedIndex())
		{
		case 0:
			listView->ChangeItemStyle(new list::ListViewBigIconContentProvider);
			break;
		case 1:
			listView->ChangeItemStyle(new list::ListViewSmallIconContentProvider);
			break;
		case 2:
			listView->ChangeItemStyle(new list::ListViewListContentProvider);
			break;
		case 3:
			listView->ChangeItemStyle(new list::ListViewDetailContentProvider);
			break;
		case 4:
			listView->ChangeItemStyle(new list::ListViewTileContentProvider);
			break;
		case 5:
			listView->ChangeItemStyle(new list::ListViewInformationContentProvider);
			break;
		}
	}
public:
	ViewSwitchingWindow()
		:GuiWindow(GetCurrentTheme()->CreateWindowStyle())
	{
		this->SetText(L"Controls.ListView.ViewSwitching");

		GuiTableComposition* table = new GuiTableComposition;
		table->SetCellPadding(4);
		table->SetAlignmentToParent(Margin(0, 0, 0, 0));
		table->SetRowsAndColumns(2, 1);
		table->SetRowOption(0, GuiCellOption::MinSizeOption());
		table->SetRowOption(1, GuiCellOption::PercentageOption(1.0));
		table->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
		{
			GuiCellComposition* cell = new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(0, 0, 1, 1);

			GuiTextList* comboSource = g::NewTextList();
			comboSource->GetItems().Add(new list::TextItem(L"Big Icon"));
			comboSource->GetItems().Add(new list::TextItem(L"Small Icon"));
			comboSource->GetItems().Add(new list::TextItem(L"List"));
			comboSource->GetItems().Add(new list::TextItem(L"Detail"));
			comboSource->GetItems().Add(new list::TextItem(L"Tile"));
			comboSource->GetItems().Add(new list::TextItem(L"Information"));
			comboSource->SetHorizontalAlwaysVisible(false);

			comboView = g::NewComboBox(comboSource);
			comboView->SetSelectedIndex(0);
			comboView->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, -1, 0));
			comboView->GetBoundsComposition()->SetPreferredMinSize(Size(160, 0));
			comboView->SelectedIndexChanged.AttachMethod(this, &ViewSwitchingWindow::comboView_SelectedIndexChanged);
			cell->AddChild(comboView->GetBoundsComposition());
		}
		{
			GuiCellComposition* cell = new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(1, 0, 1, 1);

			listView = g::NewListViewBigIcon();
			listView->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			listView->SetHorizontalAlwaysVisible(false);
			listView->SetVerticalAlwaysVisible(false);
			listView->SetMultiSelect(true);
			cell->AddChild(listView->GetBoundsComposition());
		}
		this->GetBoundsComposition()->AddChild(table);
		FillData(listView);

		// set the preferred minimum client size
		this->GetBoundsComposition()->SetPreferredMinSize(Size(640, 480));
		// call this to calculate the size immediately if any indirect content in the table changes
		// so that the window can calcaulte its correct size before calling the MoveToScreenCenter()
		this->ForceCalculateSizeImmediately();
		// move to the screen center
		this->MoveToScreenCenter();
	}
};


#endif