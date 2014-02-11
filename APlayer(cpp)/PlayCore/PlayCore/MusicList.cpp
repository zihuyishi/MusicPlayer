#include "stdafx.h"
#include "MusicList.h"

#include <objbase.h>
#include <MsXml6.h>
#pragma comment(lib, "msxml6.lib")

#include <atlbase.h>
PLAYCORE_BEGIN
MusicList::MusicList()
{
	_listname.append(L"Ä¬ÈÏÁÐ±í");
}


MusicList::~MusicList()
{
}

BOOL MusicList::SaveAs(wstring filePath)
{
	HRESULT hr = S_OK;
	CComPtr<IXMLDOMDocument> spXmldoc;
	do {
		hr = spXmldoc.CoCreateInstance(__uuidof(DOMDocument60));
		if (FAILED(hr)) break;
		CComBSTR bstrXml;
		CComBSTR bstrNamespaceURI(L"");
		CComPtr<IXMLDOMElement> pRoot;
		
		hr = spXmldoc->createElement(CComBSTR(_listname.c_str()), &pRoot);
		if (FAILED(hr)) break;

		long listlen = _list.size();
		for (long index = 0; index < listlen; ++index) {
			CComPtr<IXMLDOMElement> pMusicNode;
			CComPtr<IXMLDOMElement> pFilePath;
			MusicFile musicFile = _list[index];
			wstring musicPath = musicFile.GetFilePath();
			spXmldoc->createElement(CComBSTR(L"MusicFile"), &pMusicNode);
			spXmldoc->createElement(CComBSTR(L"filepath"), &pFilePath);
			pFilePath->put_text(CComBSTR(musicPath.c_str()));
			pMusicNode->appendChild(pFilePath, NULL);
			pRoot->appendChild(pMusicNode, NULL);
		}
		
		hr = spXmldoc->putref_documentElement(pRoot);
		if (FAILED(hr)) break;
		hr = spXmldoc->save(CComVariant(filePath.c_str()));
		if (FAILED(hr)) break;

	} while (0);

	

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

		CComPtr<IXMLDOMElement> pRoot;
		hr = spXmldoc->get_documentElement(&pRoot);
		if (FAILED(hr)) break;

		//================
		/*
		CComPtr<IXMLDOMElement> pRoot;
		spXmldoc->get_documentElement(&pRoot);
		CComBSTR test;
		pRoot->get_xml(&test);
		*/
		//================
		CComPtr<IXMLDOMNodeList> pNodeList;
		hr = pRoot->get_childNodes(&pNodeList);
		if (FAILED(hr)) break;
		long listlen;
		hr = pNodeList->get_length(&listlen);
		if (listlen != 0) _list.clear();
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