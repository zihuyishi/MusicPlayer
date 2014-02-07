#include "APlayerWindow.h"

std::vector<std::wstring> APlayerWindow::showOpenFile()
{
	HRESULT hr = S_OK;
	std::vector<std::wstring> filePaths;

	IFileOpenDialog *fileDlg = NULL;
	hr = CoCreateInstance(CLSID_FileOpenDialog,
		NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&fileDlg));
	if (FAILED(hr)) return filePaths;
	ON_SCOPE_EXIT([&] { fileDlg->Release(); });

	IKnownFolderManager *pkfm = NULL;
	hr = CoCreateInstance(CLSID_KnownFolderManager,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pkfm));
	if (FAILED(hr)) return filePaths;
	ON_SCOPE_EXIT([&] { pkfm->Release(); });

	IKnownFolder *pKnownFolder = NULL;
	hr = pkfm->GetFolder(FOLDERID_PublicMusic, &pKnownFolder);
	if (FAILED(hr)) return filePaths;
	ON_SCOPE_EXIT([&] { pKnownFolder->Release(); });

	IShellItem *psi = NULL;
	hr = pKnownFolder->GetShellItem(0, IID_PPV_ARGS(&psi));
	if (FAILED(hr)) return filePaths;
	ON_SCOPE_EXIT([&] { psi->Release(); });

	hr = fileDlg->AddPlace(psi, FDAP_BOTTOM);
	COMDLG_FILTERSPEC rgSpec[] = {
		{ L"ÒôÀÖÎÄ¼þ", SupportType }
	};
	fileDlg->SetFileTypes(1, rgSpec);

	DWORD dwOptions;
	fileDlg->GetOptions(&dwOptions);
	fileDlg->SetOptions(dwOptions | FOS_ALLOWMULTISELECT);
	hr = fileDlg->Show(NULL);
	if (SUCCEEDED(hr)) {
		IShellItemArray *pRets;
		hr = fileDlg->GetResults(&pRets);
		if (SUCCEEDED(hr)) {
			DWORD count;
			pRets->GetCount(&count);
			for (DWORD i = 0; i < count; i++) {
				IShellItem *pRet;
				LPWSTR nameBuffer;
				pRets->GetItemAt(i, &pRet);
				pRet->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &nameBuffer);
				filePaths.push_back(std::wstring(nameBuffer));
				pRet->Release();
				CoTaskMemFree(nameBuffer);
			}
			pRets->Release();
		}
	}
	return filePaths;
}

void APlayerWindow::initWindows()
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
		int _row = cControls % iRows;
		int _col = cControls / iRows;
		cControls++;
		cell->SetSite(_row, _col, 1, 1);

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
		int _row = cControls % iRows;
		int _col = cControls / iRows;
		cControls++;
		cell->SetSite(_row, _col, 1, 1);

		buttonPause = g::NewButton();
		buttonPause->SetText(L"Pause");
		buttonPause->GetBoundsComposition()->
			SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		buttonPause->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		buttonPause->Clicked.AttachMethod(this,
			&APlayerWindow::buttonPause_OnClick);
		cell->AddChild(buttonPause->GetBoundsComposition());
	}
	// add the next button
	{
		GuiCellComposition* cell = new GuiCellComposition;
		table->AddChild(cell);
		//this cell is the top cell
		int _row = cControls % iRows;
		int _col = cControls / iRows;
		cControls++;
		cell->SetSite(_row, _col, 1, 1);

		buttonNext = g::NewButton();
		buttonNext->SetText(L"Next");
		buttonNext->GetBoundsComposition()->
			SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		buttonNext->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		buttonNext->GetBoundsComposition()->SetBounds(Rect(0, 0, 100, 100));
		buttonNext->Clicked.AttachMethod(this,
			&APlayerWindow::buttonNext_OnClick);
		cell->AddChild(buttonNext->GetBoundsComposition());
	}
	// add the addmusic button
	{
		GuiCellComposition* cell = new GuiCellComposition;
		table->AddChild(cell);
		//this cell is the top cell
		int _row = cControls % iRows;
		int _col = cControls / iRows;
		cControls++;
		cell->SetSite(_row, _col, 1, 1);

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
	// add the show lyricform button
	{
		GuiCellComposition* cell = new GuiCellComposition;
		table->AddChild(cell);
		//this cell is the top cell
		int _row = cControls % iRows;
		int _col = cControls / iRows;
		cControls++;
		cell->SetSite(_row, _col, 1, 1);

		buttonLyric = g::NewButton();
		buttonLyric->SetText(L"Lyric");
		buttonLyric->GetBoundsComposition()->
			SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		buttonLyric->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		buttonLyric->GetBoundsComposition()->SetBounds(Rect(0, 0, 100, 100));
		buttonLyric->Clicked.AttachMethod(this,
			&APlayerWindow::buttonLyric_OnClick);
		cell->AddChild(buttonLyric->GetBoundsComposition());
	}
	//init lyric form
	{
		lyricform = CreateLyricFormController();
	}
	this->ForceCalculateSizeImmediately();
	this->MoveToScreenCenter();
}

void APlayerWindow::setLyricNow(void *lpParam, DWORD, DWORD)
{
	APlayerWindow* pThis = (APlayerWindow*)lpParam;
	
	std::wstring lyric = pThis->player->GetLyric();
	pThis->lyricform->LyricForm_SetLyric(lyric.c_str());
}