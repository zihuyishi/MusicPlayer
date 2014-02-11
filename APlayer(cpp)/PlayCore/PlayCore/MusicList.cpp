#include "stdafx.h"
#include "MusicList.h"

#include <objbase.h>
#include <MsXml6.h>
#pragma comment(lib, "msxml6.lib")

#include <atlbase.h>
PLAYCORE_BEGIN
MusicList::MusicList()
{
}


MusicList::~MusicList()
{
}

BOOL MusicList::SaveAs(wstring filePath)
{
	HRESULT hr = S_OK;
	CComPtr<IXMLDOMDocument> spXmldoc;
	hr = spXmldoc.CoCreateInstance(__uuidof(DOMDocument60));
	if (FAILED(hr)) return 0;
	CComBSTR bstrXml;
	CComPtr<IXMLDOMElement> spRoot = NULL;
	return FALSE;
}
BOOL MusicList::LoadList(wstring filePath)
{
	HRESULT hr = S_OK;
	do {
		CComPtr<IXMLDOMDocument> spXmldoc;
		hr = spXmldoc.CoCreateInstance(CLSID_DOMDocument60);
		if (FAILED(hr)) break;

		VARIANT_BOOL boolRet;
		hr = spXmldoc->load(CComVariant(filePath.c_str()), &boolRet);
		if (FAILED(hr)) break;

		CComPtr<IXMLDOMNodeList> pRootNodes;
		hr = spXmldoc->get_childNodes(&pRootNodes);
		if (FAILED(hr)) break;

		CComPtr<IXMLDOMNode> pMusicList;
		hr = pRootNodes->get_item(1, &pMusicList);
		if (FAILED(hr)) break;

		//================
		CComBSTR doc;
		pMusicList->get_xml(&doc);
		//================
		CComPtr<IXMLDOMNodeList> pNodeList;
		hr = pMusicList->get_childNodes(&pNodeList);
		if (FAILED(hr)) break;
		long listlen;
		hr = pNodeList->get_length(&listlen);
		for (long i = 0; i < listlen; ++i) {
			CComPtr<IXMLDOMNode> pNode;
			hr = pNodeList->get_item(i, &pNode);
			if (SUCCEEDED(hr)) {
				CComBSTR musicPath;
				CComPtr<IXMLDOMNode> pPath;
				pNode->selectSingleNode(CComBSTR(L"filepath"), &pPath);
				pPath->get_text(&musicPath);
				MusicFile musicFile(musicPath);
				_list.push_back(musicFile);
			}
		}
		return TRUE;
	} while (0);
	return FALSE;
}
PLAYCORE_END