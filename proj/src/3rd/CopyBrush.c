#include <windows.h>

HBRUSH	CopyBrush(HBRUSH hBrush)
{
	LOGBRUSH lb;
	HBRUSH   hResult = 0;

	if(hBrush)
	{
		if(GetObject(hBrush, sizeof(LOGBRUSH), &lb))
		{
			hResult = CreateBrushIndirect(&lb);
		}
	}
	return hResult;
}
