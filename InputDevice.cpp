#include "InputDevice.h"
#include "Game.h"

InputDevice::InputDevice(Game *inGame)
{
	game = inGame;
	keys = new std::unordered_set<Keys>();
	MouseWheelDelta = 0;

	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x02;
	Rid[0].dwFlags = 0;   // adds HID mouse and also ignores legacy mouse messages
	Rid[0].hwndTarget = game->Display->hWnd;

	Rid[1].usUsagePage = 0x01;
	Rid[1].usUsage = 0x06;
	Rid[1].dwFlags = 0;   // adds HID keyboard and also ignores legacy keyboard messages
	Rid[1].hwndTarget = game->Display->hWnd;

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE)
	{
		auto errorCode = GetLastError();
		std::cout << "ERROR: " << errorCode << std::endl;
	}
}

InputDevice::~InputDevice()
{
	delete keys;
}

void InputDevice::OnKeyDown(KeyboardInputEventArgs args)
{
	bool Break = args.Flags & 0x01;

	auto key = (Keys)args.VKey;

	if (args.MakeCode == 42) key = Keys::LeftShift;
	if (args.MakeCode == 54) key = Keys::RightShift;

	if (Break) {
		if (keys->count(key))	RemovePressedKey(key);
	}
	else {
		if (!keys->count(key))	AddPressedKey(key);
	}
}

void InputDevice::OnMouseMove(RawMouseEventArgs args)
{
	if (args.ButtonFlags & static_cast<int>(MouseButtonFlags::LeftButtonDown))
		AddPressedKey(Keys::LeftButton);
	if (args.ButtonFlags & static_cast<int>(MouseButtonFlags::LeftButtonUp))
		RemovePressedKey(Keys::LeftButton);
	if (args.ButtonFlags & static_cast<int>(MouseButtonFlags::RightButtonDown))
		AddPressedKey(Keys::RightButton);
	if (args.ButtonFlags & static_cast<int>(MouseButtonFlags::RightButtonUp))
		RemovePressedKey(Keys::RightButton);
	if (args.ButtonFlags & static_cast<int>(MouseButtonFlags::MiddleButtonDown))
		AddPressedKey(Keys::MiddleButton);
	if (args.ButtonFlags & static_cast<int>(MouseButtonFlags::MiddleButtonUp))
		RemovePressedKey(Keys::MiddleButton);

	POINT p;
	GetCursorPos(&p);
	ScreenToClient(game->Display->hWnd, &p);

	MousePosition = SimpleMath::Vector2(p.x, p.y);
	MouseOffset = SimpleMath::Vector2(args.X, args.Y);
	MouseWheelDelta = args.WheelDelta;

	const MouseMoveEventArgs moveArgs = { MousePosition, MouseOffset, MouseWheelDelta };

	MouseMove.Broadcast(moveArgs);
}

void InputDevice::AddPressedKey(Keys key)
{
	keys->insert(key);
}

void InputDevice::RemovePressedKey(Keys key)
{
	keys->erase(key);
}

bool InputDevice::IsKeyDown(Keys key)
{
	return keys->count(key);
}
























/*
void InputDevice::OnKeyDown(InputDevice::KeyboardStruct keyboardStruct)
{
	//if ((keyboardStruct.flags & RI_KEY_MAKE) || (keyboardStruct.message & WM_KEYDOWN) || (keyboardStruct.message & WM_SYSKEYDOWN))
	//{
	//	AddPressedKey(Key(keyboardStruct.key, keyboardStruct.code));
	//}
	//else if ((keyboardStruct.flags & RI_KEY_BREAK) || (keyboardStruct.message & WM_KEYUP) || (keyboardStruct.message & WM_SYSKEYUP))
	//{
	//	if (!keys.empty()) RemovePressed(Key(keyboardStruct.key, keyboardStruct.code));
	//}
}

void InputDevice::OnMouseMove(InputDevice::MouseStruct mouseStruct)
{
	//MouseMove.first = mouseStruct.x;
	//MouseMove.second = mouseStruct.y;

	//MouseMove.first = mouseStruct.x - MousePosition.first;
	//MouseMove.second = mouseStruct.y - MousePosition.second;

	//MousePosition.first = mouseStruct.x;
	//MousePosition.second = mouseStruct.y;

	//MouseWheelDelta = mouseStruct.buttonData;
}

void InputDevice::AddPressedKey(InputDevice::Key key)
{
	//keys.insert(key);
}

bool InputDevice::IsKeyDown(Key key)
{
	//keys.find(key);
	//!= keys.end();
	return true;
}

void InputDevice::RemovePressed(Key key)
{
	//if (IsKeyDown(key)) keys.erase(key);
}*/