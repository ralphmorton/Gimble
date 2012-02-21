
#ifndef _GIMBLE_SYSUTILS_
#define _GIMBLE_SYSUTILS_

#include "GimbleReqs.h"

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>

namespace Gimble
{
	class SysUtils
	{
	public:
		static LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}

#endif