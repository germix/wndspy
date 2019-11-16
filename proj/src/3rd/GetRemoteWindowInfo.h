#ifndef ___GETREMOTEWINDOWINFO_H___
#define ___GETREMOTEWINDOWINFO_H___
#include <windows.h>

typedef struct _REMOTEWINDOWINFO
{
	HBRUSH			Brush;

	WNDPROC			WndProc;
	WNDCLASSEXW		WndClassEx;

	WCHAR*			Text;
	int				TextLength;
}REMOTEWINDOWINFO;

#ifdef __cplusplus
extern "C" {
#endif

BOOL GetRemoteWindowInfo(HWND hwnd, REMOTEWINDOWINFO* Info);

#ifdef __cplusplus
};
#endif

#endif
