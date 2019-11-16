#include <windows.h>

BOOL CaptureHWnd(HWND hWndTarget, BOOL bCaptureClient)
{
	if(IsWindow(hWndTarget))
	{
		HDC hDC;
		HDC hDCMem;
		HBITMAP hBmOld;
		HBITMAP hBmMem;
		RECT rcRect;

		if(bCaptureClient)
		{
			hDC = GetDC(hWndTarget);
			GetClientRect(hWndTarget, &rcRect);
		}
		else
		{
			hDC = GetWindowDC(hWndTarget);
			GetWindowRect(hWndTarget, &rcRect);
			rcRect.right -= rcRect.left;
			rcRect.bottom -= rcRect.top;
		}
		hDCMem = CreateCompatibleDC(hDC);
		hBmMem = CreateCompatibleBitmap(hDC, rcRect.right, rcRect.bottom);
		hBmOld = SelectObject(hDCMem, hBmMem);

		BitBlt(hDCMem, 0, 0, rcRect.right, rcRect.bottom, hDC, 0, 0, SRCCOPY);
		SelectObject(hDCMem, hBmOld);

		if(OpenClipboard(0))
		{
			EmptyClipboard();
#ifdef SUPPORT_DIBS
#endif
			SetClipboardData(CF_BITMAP, hBmMem);
			CloseClipboard();
		}
		else
		{
			DeleteObject(hBmMem);
		}
		DeleteDC(hDCMem);
		ReleaseDC(hWndTarget, hDC);
		return TRUE;
	}
	return FALSE;
}








