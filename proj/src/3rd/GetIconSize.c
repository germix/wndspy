#include <windows.h>

#if 0	//!!!
BOOL GetIconSize(HICON hIcon, int* pnWidth, int* pnHeight)
{
	ICONINFO ii;

	if(GetIconInfo(hIcon, &ii))
	{
		if(pnWidth)
			*pnWidth = ii.xHotspot * 2;
		if(pnHeight)
			*pnHeight = ii.yHotspot * 2;
		DeleteObject(ii.hbmMask);
		DeleteObject(ii.hbmColor);
		return TRUE;
	}
	return FALSE;
}
#else
BOOL GetIconSize(HICON hIcon, int* pnWidth, int* pnHeight)
{
	BITMAP bm;
	ICONINFO ii;

	if(GetIconInfo(hIcon, &ii))
	{
		GetObject(ii.hbmColor, sizeof(BITMAP), &bm);
		if(pnWidth)
			*pnWidth = bm.bmWidth;
		if(pnHeight)
			*pnHeight = bm.bmHeight;

		DeleteObject(ii.hbmMask);
		DeleteObject(ii.hbmColor);
		return TRUE;
	}
	return FALSE;
}


#endif

