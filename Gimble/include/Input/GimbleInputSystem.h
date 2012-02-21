
#ifndef _GIMBLE_INPUTSYSTEM_
#define _GIMBLE_INPUTSYSTEM_

#include "GimbleReqs.h"
#include "GimbleSingleton.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace Gimble
{
	namespace Input
	{
		enum Key
		{
			GIKEY_ESCAPE          = 0x01,
			GIKEY_1               = 0x02,
			GIKEY_2               = 0x03,
			GIKEY_3               = 0x04,
			GIKEY_4               = 0x05,
			GIKEY_5               = 0x06,
			GIKEY_6               = 0x07,
			GIKEY_7               = 0x08,
			GIKEY_8               = 0x09,
			GIKEY_9               = 0x0A,
			GIKEY_0               = 0x0B,
			GIKEY_MINUS           = 0x0C,   /* - on main keyboard */
			GIKEY_EQUALS          = 0x0D,
			GIKEY_BACK            = 0x0E,    /* backspace */
			GIKEY_TAB             = 0x0F,
			GIKEY_Q               = 0x10,
			GIKEY_W               = 0x11,
			GIKEY_E               = 0x12,
			GIKEY_R               = 0x13,
			GIKEY_T               = 0x14,
			GIKEY_Y               = 0x15,
			GIKEY_U               = 0x16,
			GIKEY_I               = 0x17,
			GIKEY_O               = 0x18,
			GIKEY_P               = 0x19,
			GIKEY_LBRACKET        = 0x1A,
			GIKEY_RBRACKET        = 0x1B,
			GIKEY_RETURN          = 0x1C,    /* Enter on main keyboard */
			GIKEY_LCONTROL        = 0x1D,
			GIKEY_A               = 0x1E,
			GIKEY_S               = 0x1F,
			GIKEY_D               = 0x20,
			GIKEY_F               = 0x21,
			GIKEY_G               = 0x22,
			GIKEY_H               = 0x23,
			GIKEY_J               = 0x24,
			GIKEY_K               = 0x25,
			GIKEY_L               = 0x26,
			GIKEY_SEMICOLON       = 0x27,
			GIKEY_APOSTROPHE      = 0x28,
			GIKEY_GRAVE           = 0x29,    /* accent grave */
			GIKEY_LSHIFT          = 0x2A,
			GIKEY_BACKSLASH       = 0x2B,
			GIKEY_Z               = 0x2C,
			GIKEY_X               = 0x2D,
			GIKEY_C               = 0x2E,
			GIKEY_V               = 0x2F,
			GIKEY_B               = 0x30,
			GIKEY_N               = 0x31,
			GIKEY_M               = 0x32,
			GIKEY_COMMA           = 0x33,
			GIKEY_PERIOD          = 0x34,    /* . on main keyboard */
			GIKEY_SLASH           = 0x35,    /* / on main keyboard */
			GIKEY_RSHIFT          = 0x36,
			GIKEY_MULTIPLY        = 0x37,    /* * on numeric keypad */
			GIKEY_LMENU           = 0x38,    /* left Alt */
			GIKEY_SPACE           = 0x39,
			GIKEY_CAPITAL         = 0x3A,
			GIKEY_F1              = 0x3B,
			GIKEY_F2              = 0x3C,
			GIKEY_F3              = 0x3D,
			GIKEY_F4              = 0x3E,
			GIKEY_F5              = 0x3F,
			GIKEY_F6              = 0x40,
			GIKEY_F7              = 0x41,
			GIKEY_F8              = 0x42,
			GIKEY_F9              = 0x43,
			GIKEY_F10             = 0x44,
			GIKEY_NUMLOCK         = 0x45,
			GIKEY_SCROLL          = 0x46,    /* Scroll Lock */
			GIKEY_NUMPAD7         = 0x47,
			GIKEY_NUMPAD8         = 0x48,
			GIKEY_NUMPAD9         = 0x49,
			GIKEY_SUBTRACT        = 0x4A,    /* - on numeric keypad */
			GIKEY_NUMPAD4         = 0x4B,
			GIKEY_NUMPAD5         = 0x4C,
			GIKEY_NUMPAD6         = 0x4D,
			GIKEY_ADD             = 0x4E,    /* + on numeric keypad */
			GIKEY_NUMPAD1         = 0x4F,
			GIKEY_NUMPAD2         = 0x50,
			GIKEY_NUMPAD3         = 0x51,
			GIKEY_NUMPAD0         = 0x52,
			GIKEY_DECIMAL         = 0x53,    /* . on numeric keypad */
			GIKEY_OEM_102         = 0x56,    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
			GIKEY_F11             = 0x57,
			GIKEY_F12             = 0x58,
			GIKEY_F13             = 0x64,    /*                     (NEC PC98) */
			GIKEY_F14             = 0x65,    /*                     (NEC PC98) */
			GIKEY_F15             = 0x66,    /*                     (NEC PC98) */
			GIKEY_KANA            = 0x70,    /* (Japanese keyboard)            */
			GIKEY_ABNT_C1         = 0x73,    /* /? on Brazilian keyboard */
			GIKEY_CONVERT         = 0x79,    /* (Japanese keyboard)            */
			GIKEY_NOCONVERT       = 0x7B,    /* (Japanese keyboard)            */
			GIKEY_YEN             = 0x7D,    /* (Japanese keyboard)            */
			GIKEY_ABNT_C2         = 0x7E,    /* Numpad . on Brazilian keyboard */
			GIKEY_NUMPADEQUALS    = 0x8D,    /* = on numeric keypad (NEC PC98) */
			GIKEY_PREVTRACK       = 0x90,    /* Previous Track (GIKEY_CIRCUMFLEX on Japanese keyboard) */
			GIKEY_AT              = 0x91,    /*                     (NEC PC98) */
			GIKEY_COLON           = 0x92,    /*                     (NEC PC98) */
			GIKEY_UNDERLINE       = 0x93,    /*                     (NEC PC98) */
			GIKEY_KANJI           = 0x94,    /* (Japanese keyboard)            */
			GIKEY_STOP            = 0x95,    /*                     (NEC PC98) */
			GIKEY_AX              = 0x96,    /*                     (Japan AX) */
			GIKEY_UNLABELED       = 0x97,    /*                        (J3100) */
			GIKEY_NEXTTRACK       = 0x99,    /* Next Track */
			GIKEY_NUMPADENTER     = 0x9C,    /* Enter on numeric keypad */
			GIKEY_RCONTROL        = 0x9D,
			GIKEY_MUTE            = 0xA0,    /* Mute */
			GIKEY_CALCULATOR      = 0xA1,    /* Calculator */
			GIKEY_PLAYPAUSE       = 0xA2,    /* Play / Pause */
			GIKEY_MEDIASTOP       = 0xA4,    /* Media Stop */
			GIKEY_VOLUMEDOWN      = 0xAE,    /* Volume - */
			GIKEY_VOLUMEUP        = 0xB0,    /* Volume + */
			GIKEY_WEBHOME         = 0xB2,    /* Web home */
			GIKEY_NUMPADCOMMA     = 0xB3,    /* , on numeric keypad (NEC PC98) */
			GIKEY_DIVIDE          = 0xB5,    /* / on numeric keypad */
			GIKEY_SYSRQ           = 0xB7,
			GIKEY_RMENU           = 0xB8,    /* right Alt */
			GIKEY_PAUSE           = 0xC5,    /* Pause */
			GIKEY_HOME            = 0xC7,    /* Home on arrow keypad */
			GIKEY_UP              = 0xC8,    /* UpArrow on arrow keypad */
			GIKEY_PRIOR           = 0xC9,    /* PgUp on arrow keypad */
			GIKEY_LEFT            = 0xCB,    /* LeftArrow on arrow keypad */
			GIKEY_RIGHT           = 0xCD,    /* RightArrow on arrow keypad */
			GIKEY_END             = 0xCF,    /* End on arrow keypad */
			GIKEY_DOWN            = 0xD0,    /* DownArrow on arrow keypad */
			GIKEY_NEXT            = 0xD1,    /* PgDn on arrow keypad */
			GIKEY_INSERT          = 0xD2,    /* Insert on arrow keypad */
			GIKEY_DELETE          = 0xD3,    /* Delete on arrow keypad */
			GIKEY_LWIN            = 0xDB,    /* Left Windows key */
			GIKEY_RWIN            = 0xDC,    /* Right Windows key */
			GIKEY_APPS            = 0xDD,    /* AppMenu key */
			GIKEY_POWER           = 0xDE,    /* System Power */
			GIKEY_SLEEP           = 0xDF,    /* System Sleep */
			GIKEY_WAKE            = 0xE3,    /* System Wake */
			GIKEY_WEBSEARCH       = 0xE5,    /* Web Search */
			GIKEY_WEBFAVORITES    = 0xE6,    /* Web Favorites */
			GIKEY_WEBREFRESH      = 0xE7,    /* Web Refresh */
			GIKEY_WEBSTOP         = 0xE8,    /* Web Stop */
			GIKEY_WEBFORWARD      = 0xE9,    /* Web Forward */
			GIKEY_WEBBACK         = 0xEA,    /* Web Back */
			GIKEY_MYCOMPUTER      = 0xEB,    /* My Computer */
			GIKEY_MAIL            = 0xEC,    /* Mail */
			GIKEY_MEDIASELECT     = 0xED    /* Media Select */
		};

		enum Button
		{
			GIBTN_1,
			GIBTN_2,
			GIBTN_3,
			GIBTN_4
		};

		class GIMBLE_API InputSystem : public Singleton<InputSystem>
		{
		private:
			HWND mHwnd;
			Graphics::RenderSystem *mRenderSys;
			IDirectInput8 *mInput;
			IDirectInputDevice8 *mKeyboard;
			IDirectInputDevice8 *mMouse;

			byte mKeys[256];
			DIMOUSESTATE mMouseState;
			long mX, mY, mPX, mPY, mDX, mDY;

			bool mMouseLocked;

		public:
			InputSystem();
			~InputSystem();

			void init();
			void shutdown();

			void updateDeviceState();

			inline bool haveKeyboard() const { return mKeyboard?true:false; }
			inline bool haveMouse() const { return mMouse?true:false; }
			inline void setMouseLocked(bool locked) { mMouseLocked = locked; }

			inline bool isKeyDown(Key key) const 
			{
				return (mKeys[key] & 0x80)?true:false;
			}

			inline bool isButtonDown(Button button) const 
			{
				if (button > 3) return false;

				return mMouseState.rgbButtons[button]?true:false;
			}

			inline long getMouseX() const { return mX; }
			inline long getMouseY() const { return mY; }
			inline long getMouseDX() const { return mDX; }
			inline long getMouseDY() const { return mDY; }
			inline long getMouseWheelDelta() const { return mMouseState.lZ; }
		};
	}
}

#endif