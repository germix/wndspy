#include <windows.h>

BOOL FlashHWnd(HWND hWndTarget)
{
	if(IsWindow(hWndTarget) && IsWindowVisible(hWndTarget))
	{
		HDC hDC;
		HDC hDCMem;
		HBITMAP hBmMem;
		HBITMAP hBmOld;
		LONG nI;
		RECT rcRect;

		GetWindowRect(hWndTarget, &rcRect);
		rcRect.right = rcRect.right - rcRect.left;
		rcRect.bottom = rcRect.bottom - rcRect.top;

		hDC = GetDC(NULL);
		hDCMem = CreateCompatibleDC(hDC);
		hBmMem = CreateCompatibleBitmap(hDC, rcRect.right, rcRect.bottom);
		hBmOld = SelectObject(hDCMem, hBmMem);

		BitBlt(hDCMem, 0, 0, rcRect.right, rcRect.bottom, hDC, rcRect.left, rcRect.top, MERGEPAINT);
		for(nI = 0; nI < 6; nI++)
		{
			BitBlt(hDC, rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, hDCMem, 0, 0, SRCCOPY);
			Sleep(100);
			BitBlt(hDC, rcRect.left, rcRect.top, rcRect.right, rcRect.bottom, hDCMem, 0, 0, NOTSRCCOPY);
			Sleep(100);
		}
		SelectObject(hDCMem, hBmOld);

		DeleteObject(hBmMem);
		DeleteDC(hDCMem);
		ReleaseDC(NULL, hDC);
		return TRUE;
	}
	return FALSE;
}
