#include "ViewSwitchingWindow.h"
#include <math.h>
/***********************************************************************
File System Operations
***********************************************************************/

WString GetWindowsDirectory()
{
	wchar_t folderPath[MAX_PATH] = { 0 };
	HRESULT hr = SHGetFolderPath(NULL, CSIDL_WINDOWS, NULL, 0, folderPath);
	if (FAILED(hr)) return L"";
	return folderPath;
}

void SearchDirectoriesAndFiles(const WString& path, List<WString>& directories, List<WString>& files)
{
	// Use FindFirstFile, FindNextFile and FindClose to enumerate all directories and files
	WIN32_FIND_DATA findData;
	HANDLE findHandle = INVALID_HANDLE_VALUE;

	while (true)
	{
		if (findHandle == INVALID_HANDLE_VALUE)
		{
			WString searchPath = path + L"\\*";
			findHandle = FindFirstFile(searchPath.Buffer(), &findData);
			if (findHandle == INVALID_HANDLE_VALUE)
			{
				break;
			}
		}
		else
		{
			BOOL result = FindNextFile(findHandle, &findData);
			if (result == 0)
			{
				FindClose(findHandle);
				break;
			}
		}

		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (wcscmp(findData.cFileName, L".") != 0 && wcscmp(findData.cFileName, L"..") != 0)
			{
				directories.Add(findData.cFileName);
			}
		}
		else
		{
			files.Add(findData.cFileName);
		}
	}

	Func<vint(WString a, WString b)> comparer = [](WString a, WString b){return _wcsicmp(a.Buffer(), b.Buffer()); };
	CopyFrom(directories, From(directories).OrderBy(comparer));
	CopyFrom(files, From(files).OrderBy(comparer));
}

Ptr<GuiImageData> GetFileIcon(const WString& fullPath, UINT uFlags)
{
	// Use SHGetFileInfo to get the correct icons for the specified directory or file.
	SHFILEINFO info;
	DWORD result = SHGetFileInfo(fullPath.Buffer(), 0, &info, sizeof(SHFILEINFO), uFlags);
	Ptr<GuiImageData> imageData;
	if (result)
	{
		Ptr<INativeImage> image = windows::CreateImageFromHICON(info.hIcon);
		if (image)
		{
			imageData = new GuiImageData(image, 0);
		}
		DestroyIcon(info.hIcon);
	}
	return imageData;
}

WString GetFileDisplayName(const WString& fullPath)
{
	SHFILEINFO info;
	DWORD result = SHGetFileInfo(fullPath.Buffer(), 0, &info, sizeof(SHFILEINFO), SHGFI_DISPLAYNAME);
	return result ? info.szDisplayName : L"";
}

WString GetFileTypeName(const WString& fullPath)
{
	SHFILEINFO info;
	DWORD result = SHGetFileInfo(fullPath.Buffer(), 0, &info, sizeof(SHFILEINFO), SHGFI_TYPENAME);
	return result ? info.szTypeName : L"";
}

FILETIME GetFileLastWriteTime(const WString& fullPath)
{
	// Get file attributes.
	WIN32_FILE_ATTRIBUTE_DATA info;
	BOOL result = GetFileAttributesEx(fullPath.Buffer(), GetFileExInfoStandard, &info);

	// Get the localized string for the file last write date.
	FILETIME localFileTime;
	FileTimeToLocalFileTime(&info.ftLastWriteTime, &localFileTime);

	return localFileTime;
}

LARGE_INTEGER GetFileSize(const WString& fullPath)
{
	// Get file attributes.
	WIN32_FILE_ATTRIBUTE_DATA info;
	BOOL result = GetFileAttributesEx(fullPath.Buffer(), GetFileExInfoStandard, &info);

	// Get the string for file size
	LARGE_INTEGER li;
	li.HighPart = info.nFileSizeHigh;
	li.LowPart = info.nFileSizeLow;

	return li;
}

WString FileTimeToString(const FILETIME& filetime)
{
	SYSTEMTIME localSystemTime;
	FileTimeToSystemTime(&filetime, &localSystemTime);

	// Get the correct locale
	wchar_t localeName[LOCALE_NAME_MAX_LENGTH] = { 0 };
	GetSystemDefaultLocaleName(localeName, sizeof(localeName) / sizeof(*localeName));

	// Get the localized date string
	wchar_t dateString[100] = { 0 };
	GetDateFormatEx(localeName, DATE_SHORTDATE, &localSystemTime, NULL, dateString, sizeof(dateString) / sizeof(*dateString), NULL);

	// Get the localized time string
	wchar_t timeString[100] = { 0 };
	GetTimeFormatEx(localeName, TIME_FORCE24HOURFORMAT | TIME_NOSECONDS, &localSystemTime, NULL, timeString, sizeof(timeString) / sizeof(*timeString));

	return dateString + WString(L" ") + timeString;
}

WString FileSizeToString(LARGE_INTEGER filesize)
{
	WString unit;
	double size = 0;
	if (filesize.QuadPart >= 1024 * 1024 * 1024)
	{
		unit = L" GB";
		size = (double)filesize.QuadPart / (1024 * 1024 * 1024);
	}
	else if (filesize.QuadPart >= 1024 * 1024)
	{
		unit = L" MB";
		size = (double)filesize.QuadPart / (1024 * 1024);
	}
	else if (filesize.QuadPart >= 1024)
	{
		unit = L" KB";
		size = (double)filesize.QuadPart / 1024;
	}
	else
	{
		unit = L" Bytes";
		size = (double)filesize.QuadPart;
	}

	WString sizeString = ftow(size);
	const wchar_t* reading = sizeString.Buffer();
	const wchar_t* point = wcschr(sizeString.Buffer(), L'.');
	if (point)
	{
		const wchar_t* max = reading + sizeString.Length();
		point += 4;
		if (point>max) point = max;
		sizeString = sizeString.Left(point - reading);
	}

	return sizeString + unit;
}

/***********************************************************************
FileProperties
***********************************************************************/

void FileProperties::Load()
{
	if (!loaded)
	{
		loaded = true;
		smallIcon = GetFileIcon(fullPath, SHGFI_SMALLICON | SHGFI_ICON);
		bigIcon = GetFileIcon(fullPath, SHGFI_LARGEICON | SHGFI_ICON);
		displayName = GetFileDisplayName(fullPath);
		typeName = GetFileTypeName(fullPath);
		lastWriteTime = GetFileLastWriteTime(fullPath);
		size = GetFileSize(fullPath);
	}
}

FileProperties::FileProperties(const WString& _fullPath)
:loaded(false)
, fullPath(_fullPath)
{
}

Ptr<GuiImageData> FileProperties::GetSmallIcon()
{
	Load();
	return smallIcon;
}

Ptr<GuiImageData> FileProperties::GetBigIcon()
{
	Load();
	return bigIcon;
}

WString FileProperties::GetDisplayName()
{
	Load();
	return displayName;
}

WString FileProperties::GetTypeName()
{
	Load();
	return typeName;
}

FILETIME FileProperties::GetLastWriteTime()
{
	Load();
	return lastWriteTime;
}

LARGE_INTEGER FileProperties::GetSize()
{
	Load();
	return size;
}
/***********************************************************************
FillData
***********************************************************************/

void FillList(GuiListView* listView, const WString& path, List<WString>& files)
{
	// Fill all information about a directory or a file.
	FOREACH(WString, file, files)
	{
		Ptr<list::ListViewItem> item = new list::ListViewItem;
		WString fullPath = path + L"\\" + file;

		// Get large icon.
		item->largeImage = GetFileIcon(fullPath, SHGFI_LARGEICON | SHGFI_ICON);
		// Get small icon.
		item->smallImage = GetFileIcon(fullPath, SHGFI_SMALLICON | SHGFI_ICON);
		// Get display name
		item->text = GetFileDisplayName(fullPath);
		// Get type name
		item->subItems.Add(GetFileTypeName(fullPath));
		// Get last write time
		item->subItems.Add(FileTimeToString(GetFileLastWriteTime(fullPath)));
		// Get file size
		item->subItems.Add(FileSizeToString(GetFileSize(fullPath)));

		listView->GetItems().Add(item);
	}
}

void FillData(GuiListView* listView)
{
	// Get the Windows directory, normally L"C:\Windows".
	wchar_t folderPath[MAX_PATH] = { 0 };
	HRESULT hr = SHGetFolderPath(NULL, CSIDL_WINDOWS, NULL, 0, folderPath);
	if (FAILED(hr)) return;

	// Enumerate all directories and files in the Windows directory.
	List<WString> directories;
	List<WString> files;
	SearchDirectoriesAndFiles(folderPath, directories, files);

	// Set all columns. The first column is the primary column. All others are sub columns.
	listView->GetItems().GetColumns().Add(new list::ListViewColumn(L"Name", 230));
	listView->GetItems().GetColumns().Add(new list::ListViewColumn(L"Type", 120));
	listView->GetItems().GetColumns().Add(new list::ListViewColumn(L"Date", 120));
	listView->GetItems().GetColumns().Add(new list::ListViewColumn(L"Size", 120));

	// Set all data columns (important sub solumns). The first sub item is 0. The primary column is not counted in.
	listView->GetItems().GetDataColumns().Add(0);    // Type
	listView->GetItems().GetDataColumns().Add(1);    // Data

	// Fill all directories and files into the list view
	FillList(listView, folderPath, directories);
	FillList(listView, folderPath, files);
}