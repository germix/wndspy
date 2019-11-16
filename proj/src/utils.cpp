#include "utils.h"

void SelectCurrentWindow(HWND hwnd);

// Cambiar el color de fondo de los QLineEdit
// si entán en modo de sólo lectura
void QChangePaletteLineEdit_IsReadOnly(QWidget* parent, QLineEdit* edit)
{
	QPalette p;

	p = edit->palette();
	p.setColor(QPalette::Base, p.color(QPalette::Button));
	for(int i = 0; i < parent->children().size(); i++)
	{
		edit = qobject_cast<QLineEdit*>(parent->children().at(i));
		if(edit)
		{
			if(edit->isReadOnly())
				edit->setPalette(p);
		}
	}
}
QString QHexUlong(ulong n)
{
	return QString().sprintf(HEX_FORMAT, (uint)n);
}
QString QHexHandle(void* p)
{
	return QString().sprintf(HEX_FORMAT, (uint)p);
}

QString QGetClassName(HWND hwnd)
{
	QString s;
	s.resize(MAX_BUFFER);
	s.truncate(GetClassNameW(hwnd, (WCHAR*)(QChar*)s.data(), MAX_BUFFER));
	return s;
}
QString QGetWindowText(HWND hwnd)
{
	QString s;
	s.resize(MAX_BUFFER);
	s.truncate(GetWindowTextW(hwnd, (WCHAR*)(QChar*)s.data(), MAX_BUFFER));
	return s;
}
QString QSetWindowText(HWND hwnd, const QString& text)
{
	SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)text.data());
	return text;
}
QString AppendFormatStyleLookup(STYLELOOKUP *Lookup, uint Style, bool* Ok)
{
	bool r;
	static WCHAR buf[MAX_BUFFER];

	buf[0] = 0;
	r = AppendFormatStyleLookup(buf, Lookup, Style);
	if(Ok) *Ok = r;
	return QString::fromWCharArray(buf);
}
QString AppendFormatHandleLookup(HANDLELOOKUP* Lookup, HANDLE Handle, bool* Ok)
{
	bool r;
	static WCHAR buf[MAX_BUFFER];

	buf[0] = 0;
	r = AppendFormatHandleLookup(buf, Lookup, Handle);
	if(Ok) *Ok = r;
	return QString::fromWCharArray(buf);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CommandHWnd(int Command, HWND hwndTarget)
{
	HWND  hwndZ;
	DWORD dwStyle;
	DWORD dwStyleEx;
	DWORD dwSWPflags;

	dwStyle   = GetWindowLong(hwndTarget, GWL_STYLE);
	dwStyleEx = GetWindowLong(hwndTarget, GWL_EXSTYLE);
	switch(Command)
	{
		case CMDHWND_CLOSE:
			PostMessage(hwndTarget, WM_CLOSE, 0, 0);
			break;
		case CMDHWND_SELECT:
			SelectCurrentWindow(hwndTarget);
			break;
		// Show / Hide
		case CMDHWND_VISIBLE:
			dwSWPflags = SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER;

			if(dwStyle & WS_VISIBLE)
				dwSWPflags |= SWP_HIDEWINDOW;
			else
				dwSWPflags |= SWP_SHOWWINDOW;

			SetWindowPos(hwndTarget, 0, 0,0,0,0, dwSWPflags);
			break;
		// Enable / Disable
		case CMDHWND_ENABLED:
			EnableWindow(hwndTarget, (dwStyle & WS_DISABLED) ? TRUE : FALSE);
			break;
		// Ontop / Not ontop
		case CMDHWND_TOPMOST:
			dwSWPflags = SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE;

			if(dwStyleEx & WS_EX_TOPMOST)
				hwndZ = HWND_NOTOPMOST;
			else
				hwndZ = HWND_TOPMOST;

			SetWindowPos(hwndTarget, hwndZ,	0,0,0,0, dwSWPflags);
			break;
		case CMDHWND_FLASH:
			FlashHWnd(hwndTarget);
			break;
		case CMDHWND_CAPTURECLIENT:
			CaptureHWnd(hwndTarget, TRUE);
			break;
		case CMDHWND_CAPTUREWINDOW:
			CaptureHWnd(hwndTarget, FALSE);
			break;
		case CMDHWND_ADJUST:
			ShowEditWindowSize(0, hwndTarget);
			break;
		case CMDHWND_SENDTOBACK:
			SetWindowPos(hwndTarget, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			break;
		case CMDHWND_BRINGTOFRONT:
			SetWindowPos(hwndTarget, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			break;
	}
}




