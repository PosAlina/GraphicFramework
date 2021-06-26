#pragma once
#include <unordered_set>
#include "Keys.h"
#include "Delegates.h"
#include "inclib.h"

class Game; 

class InputDevice
{
	Game *game  = nullptr;
	std::unordered_set<Keys>* keys;

public:

	struct MouseMoveEventArgs
	{
		DirectX::SimpleMath::Vector2 Position;
		DirectX::SimpleMath::Vector2 Offset;
		int WheelDelta = 0;
	};

	DirectX::SimpleMath::Vector2 MousePosition;
	DirectX::SimpleMath::Vector2 MouseOffset;
	int MouseWheelDelta = 0;

	MulticastDelegate<const MouseMoveEventArgs&> MouseMove;

public:

	InputDevice(Game *inGame);
	~InputDevice();


	void AddPressedKey(Keys key);
	void RemovePressedKey(Keys key);
	bool IsKeyDown(Keys key);

protected:
	struct KeyboardInputEventArgs {
		/*
		 * The "make" scan code (key depression).
		 */
		USHORT MakeCode;

		/*
		 * The flags field indicates a "break" (key release) and other
		 * miscellaneous scan code information defined in ntddkbd.h.
		 */
		USHORT Flags;

		USHORT VKey;
		UINT   Message;
	};

	enum class MouseButtonFlags
	{
		/// <unmanaged>RI_MOUSE_LEFT_BUTTON_DOWN</unmanaged>
		LeftButtonDown = 1,
		/// <unmanaged>RI_MOUSE_LEFT_BUTTON_UP</unmanaged>
		LeftButtonUp = 2,
		/// <unmanaged>RI_MOUSE_RIGHT_BUTTON_DOWN</unmanaged>
		RightButtonDown = 4,
		/// <unmanaged>RI_MOUSE_RIGHT_BUTTON_UP</unmanaged>
		RightButtonUp = 8,
		/// <unmanaged>RI_MOUSE_MIDDLE_BUTTON_DOWN</unmanaged>
		MiddleButtonDown = 16, // 0x00000010
		/// <unmanaged>RI_MOUSE_MIDDLE_BUTTON_UP</unmanaged>
		MiddleButtonUp = 32, // 0x00000020
		/// <unmanaged>RI_MOUSE_BUTTON_1_DOWN</unmanaged>
		Button1Down = LeftButtonDown, // 0x00000001
		/// <unmanaged>RI_MOUSE_BUTTON_1_UP</unmanaged>
		Button1Up = LeftButtonUp, // 0x00000002
		/// <unmanaged>RI_MOUSE_BUTTON_2_DOWN</unmanaged>
		Button2Down = RightButtonDown, // 0x00000004
		/// <unmanaged>RI_MOUSE_BUTTON_2_UP</unmanaged>
		Button2Up = RightButtonUp, // 0x00000008
		/// <unmanaged>RI_MOUSE_BUTTON_3_DOWN</unmanaged>
		Button3Down = MiddleButtonDown, // 0x00000010
		/// <unmanaged>RI_MOUSE_BUTTON_3_UP</unmanaged>
		Button3Up = MiddleButtonUp, // 0x00000020
		/// <unmanaged>RI_MOUSE_BUTTON_4_DOWN</unmanaged>
		Button4Down = 64, // 0x00000040
		/// <unmanaged>RI_MOUSE_BUTTON_4_UP</unmanaged>
		Button4Up = 128, // 0x00000080
		/// <unmanaged>RI_MOUSE_BUTTON_5_DOWN</unmanaged>
		Button5Down = 256, // 0x00000100
		/// <unmanaged>RI_MOUSE_BUTTON_5_UP</unmanaged>
		Button5Up = 512, // 0x00000200
		/// <unmanaged>RI_MOUSE_WHEEL</unmanaged>
		MouseWheel = 1024, // 0x00000400
		/// <unmanaged>RI_MOUSE_HWHEEL</unmanaged>
		Hwheel = 2048, // 0x00000800

		None = 0,
	};
	struct RawMouseEventArgs
	{
		/*MOUSE_MOVE_RELATIVE*/
		int Mode;
		int ButtonFlags;
		int ExtraInformation;
		int Buttons;
		int WheelDelta;
		int X;
		int Y;
	};

public:
	void OnKeyDown(KeyboardInputEventArgs args);
	void OnMouseMove(RawMouseEventArgs args);
};


/*
#pragma once
#include "inclib.h"
#include "DisplayWin32.h"
#include <unordered_set>

class InputDevice
{
	struct Key
	{
		USHORT vkey = 0; // virtual code of key
		USHORT scanCode = 0; // extra scan code for difference Left and Right keys

		Key(USHORT vkey, USHORT scandCode)
		{
			this->vkey = vkey;
			this->scanCode = scanCode;
		}
	};
	struct KeyboardStruct
	{
		USHORT code; // Specifies the scan code (from Scan Code Set 1) associated with a key press.
		USHORT flags; // Flags for scan code information. It can be one or more of the following:
		// RI_KEY_MAKE 0 - The key is down.
		// RI_KEY_BREAK 1 - The key is up.
		// RI_KEY_E0 2 - The scan code has the E0 prefix.
		// RI_KEY_E1 4 - The scan code has the E1 prefix.
		USHORT key; // The corresponding legacy virtual-key code.
		UINT message; // The corresponding legacy keyboard window message, for example WM_KEYDOWN, WM_SYSKEYDOWN, and so forth.
	};
	struct MouseStruct
	{
		USHORT flags; // The mouse state. This member can be any reasonable combination of the following.
		// MOUSE_MOVE_RELATIVE 0x00 - Mouse movement data is relative to the last mouse position.For further information about mouse motion, see the following Remarks section.
		// MOUSE_MOVE_ABSOLUTE 0x01 - Mouse movement data is based on absolute position.For further information about mouse motion, see the following Remarks section.
		// MOUSE_VIRTUAL_DESKTOP 0x02 - Mouse coordinates are mapped to the virtual desktop(for a multiple monitor system).For further information about mouse motion, see the following Remarks section.
		// MOUSE_ATTRIBUTES_CHANGED 0x04 - Mouse attributes changed; application needs to query the mouse attributes.
		// MOUSE_MOVE_NOCOALESCE 0x08 - This mouse movement event was not coalesced. Mouse movement events can be coalescened by default.
		int extraInformation; // The device-specific additional information for the event.
		int rawButtons; // The raw state of the mouse buttons. The Win32 subsystem does not use this member.
		int buttonData; // The transition state of the mouse buttons: If mouse wheel is moved, usButtonData contains a signed short value that specifies the distance the wheel is rotated.
		LONG x; // The motion in the X direction. This is signed relative motion or absolute motion, depending on the value of usFlags.
		LONG y; // The motion in the Y direction. This is signed relative motion or absolute motion, depending on the value of usFlags.
	};
	enum KeyName // Or only virtual code?
	{
		UNKNOWN = 0,

		LEFT_MOUSE_BUTTON = 1,
		RIGHT_MOUSE_BUTTON = 2,
		MIDDLE_MOUSE_BUTTON = 3,
		X1_MOUSE_BUTTON = 4,
		X2_MOUSE_BUTTON = 5,
		X3_MOUSE_BUTTON = 6,

		LEFT_CTRL = 7,
		RIGHT_CTRL = 8,
		LEFT_ALT = 9,
		RIGHT_ALT = 10,
		CANCEL = 7,
		BACKSPACE = 8,
		TAB = 9,
		CLEAR = 10,
		ENTER = 11,
	};

	DisplayWin32* game = nullptr;
	std::set<Key> keys;
public:
	std::pair<LONG, LONG> MouseMove;
	std::pair<LONG, LONG> MouseOffset;
	std::pair<LONG, LONG> MousePosition;
	USHORT MouseWheelDelta;

	void OnKeyDown(KeyboardStruct keyboardStruct);
	void OnMouseMove(MouseStruct mouseStruct);

	void AddPressedKey(Key key);
	bool IsKeyDown(Key key);
	void RemovePressed(Key key);
};
*/