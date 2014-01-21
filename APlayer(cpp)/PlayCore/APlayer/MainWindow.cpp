#include "GacUI.h"

#include <Windows.h>
#include "APlayerWindow.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	return SetupWindowsDirect2DRenderer();
}

void PlaceControllers(GuiWindow* window);
void GuiMain()
{
	GuiWindow* window = new APlayerWindow();
	GetApplication()->Run(window);
	delete window;
}
