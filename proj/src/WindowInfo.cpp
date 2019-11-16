#include "3rd/GetRemoteWindowInfo.h"
#include <WindowInfo.h>

extern "C"
{
HBRUSH	CopyBrush(HBRUSH hBrush);
};


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

WindowInfo::WindowInfo()
{
	hWnd = NULL;
	hMenu = NULL;

	hFont = NULL;
	hBrush = NULL;
	hIcon = NULL;
	hCursor = NULL;
	hIconSm = NULL;
	hIconLg = NULL;

	hInstance = NULL;

	pfnClassProc = NULL;
	pfnWindowProc = NULL;

}
WindowInfo::~WindowInfo()
{
	clear();
}
void			WindowInfo::clear()
{
	if(hIcon)
		::DestroyIcon(hIcon);
	if(hIconSm)
		::DestroyIcon(hIconSm);
	if(hIconLg)
		::DestroyIcon(hIconLg);
	if(hCursor)
		::DestroyCursor(hCursor);
	if(hBrush)
		::DeleteObject(hBrush);

	hWnd = NULL;
	hMenu = NULL;

	hFont = NULL;
	hBrush = NULL;
	hIcon = NULL;
	hCursor = NULL;
	hIconSm = NULL;
	hIconLg = NULL;

	hInstance = NULL;

	szClassName.clear();
	szWindowText.clear();

	pfnClassProc = NULL;
	pfnWindowProc = NULL;
}
bool			WindowInfo::isValid() const
{
	return ::IsWindow(hWnd);
}
HWND			WindowInfo::getHandle() const
{
	return hWnd;
}
void			WindowInfo::selectWindow(HWND h)
{
	clear();
	hWnd = h;
	if(!hWnd && IsWindow(hWnd))
	{
		hWnd = NULL;
	}
	else
	{
		WCHAR className[200];
		WCHAR windowName[200];

		hMenu = ::GetMenu(hWnd);

		// Fuente
		hFont = (HFONT)SendMessage(hWnd, WM_GETFONT, 0, 0);

		// Icono
		hIcon = (HICON)GetClassLong(hWnd, GCL_HICON);
		hIconLg = hIcon;
		/*
		if(hIcon)
		{
			hIcon = CopyIcon(hIcon);
			hIconLg = CopyIcon(hIcon);
		}
		*/
		hIconSm = (HICON)GetClassLong(hWnd, GCL_HICONSM);
		/*
		if(hIconSm)
		{
			hIconSm = CopyIcon(hIconSm);
		}
		*/
		// Cursor
		hCursor = (HCURSOR)GetClassLong(hWnd, GCL_HCURSOR);
		/*
		if(hCursor)
		{
			hCursor = CopyCursor(hCursor);
		}
		*/
		GetClassNameW(hWnd, className, 50);
		GetWindowTextW(hWnd, windowName, 50);
		szClassName = QString::fromWCharArray(className);
		szWindowText = QString::fromWCharArray(windowName);

		if(IsWindowUnicode(hWnd))
		{
			pfnClassProc	 = (WNDPROC)GetClassLongW(hWnd, GCL_WNDPROC);
			pfnWindowProc	 = (WNDPROC)GetWindowLongW(hWnd, GWL_WNDPROC);
			hInstance		 = (HINSTANCE)GetWindowLongW(hWnd, GWL_HINSTANCE);
		}
		else
		{
			pfnClassProc	 = (WNDPROC)GetClassLongA(hWnd, GCL_WNDPROC);
			pfnWindowProc	 = (WNDPROC)GetWindowLongA(hWnd, GWL_WNDPROC);
			hInstance		 = (HINSTANCE)GetWindowLongA(hWnd, GWL_HINSTANCE);
		}
		// Remote Threads only available under Windows NT
		if(GetVersion() < 0x80000000)
		{
			REMOTEWINDOWINFO remoteInfo;

			ZeroMemory(&remoteInfo, sizeof(REMOTEWINDOWINFO));
			GetRemoteWindowInfo(hWnd, &remoteInfo);
			if(remoteInfo.Brush)
			{
				hBrush = remoteInfo.Brush;
			}
			pfnWindowProc = remoteInfo.WndProc;
		}
		else
		{
		//	hBrush = CopyBrush((HBRUSH)GetClassLong(hWnd, GCL_HBRBACKGROUND));
			hBrush = (HBRUSH)GetClassLong(hWnd, GCL_HBRBACKGROUND);
		}
	}
}



