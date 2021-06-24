#include "DisplayWin32.h"

HMODULE DisplayWin32::Module = nullptr;

DisplayWin32::DisplayWin32(std::wstring& windowName, int width, int height, WNDPROC WndProc)
{
	ClientWidth = width;
	ClientHeight = height;

	HINSTANCE hInstance = GetModuleHandle(nullptr);

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = windowName.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Register class error!", L"Register class", NULL);
		return;
	}

	int posX = (GetSystemMetrics(SM_CXSCREEN) - ClientWidth) / 2;
	int posY = (GetSystemMetrics(SM_CYSCREEN) - ClientHeight) / 2;

	RECT windowRect = { 0, 0, static_cast<LONG>(ClientWidth), static_cast<LONG>(ClientHeight) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;
	hWnd = CreateWindowEx(WS_EX_APPWINDOW, windowName.c_str(), windowName.c_str(),
		dwStyle,
		posX, posY,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, hInstance, nullptr);
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
	ShowCursor(true);
}
