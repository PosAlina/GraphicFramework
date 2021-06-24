#pragma once
#include "inclib.h"

class DisplayWin32
{
public:
	int ClientHeight = 0;
	int ClientWidth = 0;

	DisplayWin32(std::wstring& windowName, int width, int height, WNDPROC WndProc);

#pragma region Window specifications
	HWND hWnd = nullptr;
	WNDCLASSEX wc = {};

	static HMODULE Module;
#pragma endregion Window specifications
};