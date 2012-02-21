
#ifndef _GIMBLE_RENDERWINDOW_
#define _GIMBLE_RENDERWINDOW_

#include "GimbleReqs.h"

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>

namespace Gimble
{
	class RenderWindow
	{
	public:
		static bool (*mWindowCloseCallback)();

	private:
		HINSTANCE mInst;
		HWND mHWnd;

		string mTitle;
		uint mWidth;
		uint mHeight;
		uint mTop;
		uint mLeft;
		bool mFullscreen;
		bool mActive;
		bool mClosed;

	public:
		RenderWindow(string title, uint width, uint height, bool fullscreen);

		HWND create();
		void set(bool fullscreen, uint width, uint height);
		void resize(uint width, uint height);
		void activate();
		void deactivate();
		void windowMovedOrSized();
		void destroy();

		bool isFullscreen();
		bool isActive();
		bool isClosed();

		inline HINSTANCE getInst() const { return mInst; }
		inline HWND getHwnd() const { return mHWnd; }

	public:
		static void setWindowCloseCallback(bool (*callback)());
	};
}

#endif