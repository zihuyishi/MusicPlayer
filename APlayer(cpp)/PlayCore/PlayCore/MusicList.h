#pragma once
#include "PlayCoreHead.h"
#include "MusicFile.h"

#include <vector>


PLAYCORE_BEGIN
using std::vector;
class MusicList
{
public:
	MusicList();
	~MusicList();
public:
	const MusicFile& operator[](int index) const { return _list[index]; }
	void Add(const MusicFile &musicFile) { _list.push_back(musicFile); }
	int ListLength() const { return _list.size(); }
	BOOL SaveAs(std::wstring filePath);
	BOOL LoadList(wstring filePath);
private:
	vector<MusicFile>	_list;
	wstring				_listname;
};

PLAYCORE_END