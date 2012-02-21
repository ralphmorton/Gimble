
#include "GimbleSysUtils.h"
#include "GimbleRenderWindow.h"
#include "Game/GimbleEngine.h"

LRESULT CALLBACK Gimble::SysUtils::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)(((LPCREATESTRUCT)lParam)->lpCreateParams));
		return 0;
	}

	Gimble::RenderWindow* win = (Gimble::RenderWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (!win) return DefWindowProc(hWnd, uMsg, wParam, lParam);

	switch( uMsg )
	{
	case WM_ACTIVATE:
		{
			bool active = (LOWORD(wParam) != WA_INACTIVE);
			if(active) win->activate();
			else win->deactivate();
			break;
		}
	case WM_MOVE:
		win->windowMovedOrSized();
		break;

	case WM_DISPLAYCHANGE:
		win->windowMovedOrSized();
		break;

	case WM_SIZE:
		win->windowMovedOrSized();
		break;

	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
		break;

	case WM_CLOSE:
		if (RenderWindow::mWindowCloseCallback && !RenderWindow::mWindowCloseCallback()) return 0;
		win->destroy();
		return 0;
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}