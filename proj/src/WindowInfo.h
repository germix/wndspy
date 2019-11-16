#ifndef WINDOWINFO_H
#define WINDOWINFO_H
#include <windows.h>
#include <QString>

class WindowInfo
{
public:
	HWND			hWnd;
	HMENU			hMenu;

	HFONT			hFont;
	HBRUSH			hBrush;

	HICON			hIcon;
	HCURSOR			hCursor;
	HICON			hIconSm;
	HICON			hIconLg;

	HINSTANCE		hInstance;

	QString			szClassName;
	QString			szWindowText;

	WNDPROC			pfnClassProc;
	WNDPROC			pfnWindowProc;

public:
	WindowInfo();
	~WindowInfo();
public:
	void			clear();

	bool			isValid() const;
	HWND			getHandle() const;
	void			selectWindow(HWND hwnd);

	/*
	HFONT			getFont() const { return m_hFont; }
	HBRUSH			getBrush() const { return m_hBrush; }

	HICON			getIcon() const { return m_hIcon; }
	HCURSOR			getCursor() const { return m_hCursor; }

	HICON			getIconSm() const { return m_hIconSm; }
	HICON			getIconLg() const { return m_hIconLg; }

	HMENU			getMenu() const { return m_hMenu; }
	HINSTANCE		getInstance() const { return m_hInstance; }

	WNDPROC			getClassProc() const { return pfnClassProc; }
	WNDPROC			getWindowProc() const { return pfnWindowProc; }

	QString			getClassName() const { return szClassName; }
	QString			getWindowText() const { return szWindowText; }
*/
//	void			setWindowText(const QString& text);

};

#ifndef MAX_BUFFER
#define MAX_BUFFER		256
#endif
#ifndef HEX_FORMAT
#define HEX_FORMAT		"%08X"
#endif

HWND GetCurrentWindow();
void SelectCurrentWindow(HWND hwnd);

#endif // WINDOWINFO_H
