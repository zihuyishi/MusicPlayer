// PlayCore.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"

//Dll source code file exports functions and variables.
#define APLAY_API extern "C" __declspec(dllexport)

#include "PlayCore.h"
#include "PlayController.h"

IPlayController* __stdcall CreateController()
{
	PlayController *instance = new PlayController();
	return (IPlayController*)instance;
}