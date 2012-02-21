
#include "GimbleRenderWindow.h"
#include "GimbleSysUtils.h"

bool (*Gimble::RenderWindow::mWindowCloseCallback)() = 0;

namespace Gimble
{
	RenderWindow::RenderWindow(string title, uint width, uint height, bool fullscreen)
	{
		mTitle = title;
		mWidth = width;
		mHeight = height;
		mTop = mLeft = 0;
		mFullscreen = fullscreen;
		mActive = false;
		mClosed = true;
	}

	HWND RenderWindow::create()
	{
		HINSTANCE hInst = GetModuleHandle(NULL);
		DWORD dwStyle = mFullscreen ? (WS_VISIBLE | WS_CLIPCHILDREN | WS_POPUP) : (WS_VISIBLE | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW);

		WNDCLASSEX wc =
		{
			sizeof(WNDCLASSEX), 
			CS_CLASSDC, 
			SysUtils::wndProc, 
			0L, 
			0L,
			GetModuleHandle(NULL), 
			NULL, 
			NULL, 
			NULL, 
			NULL,
			"GimbleWnd", 
			NULL
		};
		RegisterClassEx(&wc);

		mInst = hInst;
		mHWnd = CreateWindow("GimbleWnd", mTitle.c_str(), dwStyle, mLeft, mTop, mWidth, mHeight, 0, 0, wc.hInstance, this);
		mActive = true;
		mClosed = false;

		return mHWnd;
	}

	void RenderWindow::set(bool fullscreen, uint width, uint height)
	{
		if (fullscreen != mFullscreen || width != mWidth || height != mHeight)
		{
			DWORD dwStyle = fullscreen ? (WS_VISIBLE | WS_CLIPCHILDREN | WS_POPUP) : (WS_VISIBLE | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW);

			bool oldFullscreen = mFullscreen;
			mFullscreen = fullscreen;

			if (fullscreen)
			{
				mTop = mLeft = 0;
				mWidth = width;
				mHeight = height;

				if (oldFullscreen) SetWindowPos(mHWnd, HWND_TOPMOST, 0, 0, width, height, SWP_NOACTIVATE);
				else
				{
					SetWindowPos(mHWnd, HWND_TOPMOST, 0, 0, width, height, SWP_NOACTIVATE);
					SetWindowLong(mHWnd, GWL_STYLE, dwStyle);
					SetWindowPos(mHWnd, 0, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
				}
			}
			else
			{
				RECT rc;
				SetRect(&rc, 0, 0, width, height);
				AdjustWindowRect(&rc, dwStyle, false);
				unsigned int winWidth = rc.right - rc.left;
				unsigned int winHeight = rc.bottom - rc.top;

				SetWindowLong(mHWnd, GWL_STYLE, dwStyle);
				SetWindowPos(mHWnd, HWND_NOTOPMOST, 0, 0, winWidth, winHeight, SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOACTIVATE);
			}

			//TODO: notify resize
		}
	}

	void RenderWindow::resize(uint width, uint height)
	{
		if (mHWnd && !mFullscreen)
		{
			RECT rc = { 0, 0, width, height };
			AdjustWindowRect(&rc, GetWindowLong(mHWnd, GWL_STYLE), false);
			width = rc.right - rc.left;
			height = rc.bottom - rc.top;
			SetWindowPos(mHWnd, 0, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

			mWidth = width;
			mHeight = height;
		}
	}

	void RenderWindow::activate()
	{
		mActive = true;
		if(mFullscreen)
		{
			ShowWindow(mHWnd, SW_SHOWNORMAL);

			DEVMODE dm;
			dm.dmSize = sizeof(DEVMODE);
			dm.dmBitsPerPel = 32;
			dm.dmPelsWidth = mWidth;
			dm.dmPelsHeight = mHeight;
			dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
		}
	}

	void RenderWindow::deactivate()
	{
		mActive = false;
		if(mFullscreen)
		{
			ChangeDisplaySettings(NULL, 0);
			ShowWindow(mHWnd, SW_SHOWMINNOACTIVE);
		}
	}

	void RenderWindow::windowMovedOrSized()
	{
		if (!mHWnd || IsIconic(mHWnd)) return;

		RECT rc;
		GetWindowRect(mHWnd, &rc);
		mTop = rc.top;
		mLeft = rc.left;
	
		GetClientRect(mHWnd, &rc);
		uint width = rc.right;
		uint height = rc.bottom;
		if (mWidth == width && mHeight == height) return;

		mWidth = width;
		mHeight = height;
	}

	void RenderWindow::destroy()
	{
		DestroyWindow(mHWnd);
		mHWnd = 0;
		mActive = false;
		mClosed = true;
	}

	bool RenderWindow::isFullscreen()
	{
		return mFullscreen;
	}

	bool RenderWindow::isActive()
	{
		return mActive;
	}

	bool RenderWindow::isClosed()
	{
		return mClosed;
	}

	void RenderWindow::setWindowCloseCallback(bool (*callback)())
	{
		RenderWindow::mWindowCloseCallback = callback;
	}
}