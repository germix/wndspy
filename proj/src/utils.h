#ifndef UTILS_H
#define UTILS_H
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QLineEdit>

typedef unsigned int uint;

#define	__LTEXT(q)	L##q

#define _L(x)	__LTEXT(x)

#define countof(a)		(sizeof(a) / sizeof(a[0]))



#define HEX_FORMAT		"%08X"
#define MAX_BUFFER		256

///////////////////////////////////////////////////////////////////////////////////////////////////
QString QHexUlong(ulong n);
QString QHexHandle(void* p);
QString QGetClassName(HWND hwnd);
QString QGetWindowText(HWND hwnd);
QString QSetWindowText(HWND hwnd, const QString& text);

void QChangePaletteLineEdit_IsReadOnly(QWidget* parent, QLineEdit* edit);

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _HANDLELOOKUP
{
	int			nType;
	HANDLE		hHandle;
	PCWSTR		pHandleName;
}HANDLELOOKUP;

typedef struct tagSTYLELOOKUP
{
	ULONG	fStyle;
	PWSTR	pszStyleName;
}STYLELOOKUP, *PSTYLELOOKUP, *LPSTYLELOOKUP;

typedef struct tagSTYLELOOKUPEXT
{
	ULONG	fStyle;
	PWSTR	pszStyleName;
	ULONG	fCmdMask;
	ULONG	fDepends;
	ULONG	fExcludes;
}STYLELOOKUPEXT, *PSTYLELOOKUPEXT, *LPSTYLELOOKUPEXT;

typedef struct tagCLASSLISTSTYLELOOKUP
{
	ULONG	nGetData;
	ULONG	nSetData;
	PWSTR	pszClassName;
	LPSTYLELOOKUPEXT	lpStyleList;
}CLASSLISTSTYLELOOKUP, *PCLASSLISTSTYLELOOKUP, *LPCLASSLISTSTYLELOOKUP;


#ifndef WC_STATIC
#define WC_STATIC										L"Static"
#endif
#ifndef WC_EDIT
#define WC_EDIT											L"Edit"
#endif
#ifndef WC_BUTTON
#define WC_BUTTON										L"Button"
#endif
#ifndef WC_LISTBOX
#define WC_LISTBOX										L"ListBox"
#endif
#ifndef WC_COMBOBOX
#define WC_COMBOBOX										L"ComboBox"
#endif
#ifndef WC_DRAGLIST
#define WC_DRAGLIST										L"DragList"
#endif
#ifndef WC_COMBOBOXEX
#define WC_COMBOBOXEX									L"ComboBoxEx32"
#endif
#ifndef WC_SCROLLBAR
#define WC_SCROLLBAR									L"ScrollBar"
#endif
#ifndef WC_UPDOWN
#define WC_UPDOWN										L"msctls_updown32"
#endif
#ifndef WC_PAGER
#define WC_PAGER										L"SysPager"
#endif
#ifndef WC_TRACKBAR
#define WC_TRACKBAR										L"msctls_trackbar32"
#endif
#ifndef WC_PROGRESS
#define WC_PROGRESS										L"msctls_progress32"
#endif
#ifndef WC_HEADER
#define WC_HEADER										L"SysHeader32"
#endif
#ifndef WC_LISTVIEW
#define WC_LISTVIEW										L"SysListView32"
#endif
#ifndef WC_TREEVIEW
#define WC_TREEVIEW										L"SysTreeView32"
#endif
#ifndef WC_REBAR
#define WC_REBAR										L"ReBarWindow32"
#endif
#ifndef WC_TOOLBAR
#define WC_TOOLBAR										L"ToolbarWindow32"
#endif
#ifndef WC_STATUSBAR
#define WC_STATUSBAR									L"msctls_statusbar32"
#endif
#ifndef WC_TABCONTROL
#define WC_TABCONTROL									L"SysTabControl32"
#endif
#ifndef WC_ANIMATE
#define WC_ANIMATE										L"SysAnimate32"
#endif
#ifndef WC_DATETIME
#define WC_DATETIME										L"SysDateTimePick32"
#endif
#ifndef WC_MONTHCAL
#define WC_MONTHCAL										L"SysMonthCal32"
#endif
#ifndef WC_LINK
#define WC_LINK											L"SysLink"
#endif
#ifndef WC_IPADDRESS
#define WC_IPADDRESS									L"SysIPAddress32"
#endif
#ifndef WC_HOTKEY
#define WC_HOTKEY										L"msctls_hotkey32"
#endif
#ifndef WC_TOOLTIPS
#define WC_TOOLTIPS										L"tooltips_class32"
#endif
#ifndef WC_RICHEDIT
#define WC_RICHEDIT										L"RICHEDIT"
#endif
#ifndef WC_RICHEDIT20A
#define WC_RICHEDIT20A									L"RichEdit20A"
#endif
#ifndef WC_RICHEDIT20W
#define WC_RICHEDIT20W									L"RichEdit20W"
#endif



extern STYLELOOKUP g_aStyles_Class[];

extern STYLELOOKUPEXT g_aStyles_Window[];
extern STYLELOOKUPEXT g_aStylesEx_Window[];
extern STYLELOOKUPEXT g_aStyles_Dialog[];
extern STYLELOOKUPEXT g_aStyles_Static[];
extern STYLELOOKUPEXT g_aStyles_Edit[];
extern STYLELOOKUPEXT g_aStyles_Button[];
extern STYLELOOKUPEXT g_aStyles_ListBox[];
extern STYLELOOKUPEXT g_aStyles_ComboBox[];
extern STYLELOOKUPEXT g_aStyles_ScrollBar[];
extern STYLELOOKUPEXT g_aStyles_CommCtrl[];
extern STYLELOOKUPEXT g_aStyles_UpDown[];
extern STYLELOOKUPEXT g_aStyles_Pager[];
extern STYLELOOKUPEXT g_aStyles_Trackbar[];
extern STYLELOOKUPEXT g_aStyles_Progress[];
extern STYLELOOKUPEXT g_aStyles_Header[];
extern STYLELOOKUPEXT g_aStyles_ListView[];
extern STYLELOOKUPEXT g_aStyles_TreeView[];
extern STYLELOOKUPEXT g_aStyles_ReBar[];
extern STYLELOOKUPEXT g_aStyles_ToolBar[];
extern STYLELOOKUPEXT g_aStyles_StatusBar[];
extern STYLELOOKUPEXT g_aStyles_Tab[];
extern STYLELOOKUPEXT g_aStyles_Animate[];
extern STYLELOOKUPEXT g_aStyles_DateTime[];
extern STYLELOOKUPEXT g_aStyles_MonthCal[];
extern STYLELOOKUPEXT g_aStyles_ToolTip[];
extern STYLELOOKUPEXT g_aStyles_Riched[];
extern STYLELOOKUPEXT g_aStylesEx_ComboBoxEx[];
extern STYLELOOKUPEXT g_aStylesEx_ListView[];
extern STYLELOOKUPEXT g_aStylesEx_ToolBar[];
extern STYLELOOKUPEXT g_aStylesEx_Tab[];
extern STYLELOOKUPEXT g_aEventMask_Riched[];

extern CLASSLISTSTYLELOOKUP g_aClassList_Standard[];
extern CLASSLISTSTYLELOOKUP g_aClassList_CommCtrl[];
extern CLASSLISTSTYLELOOKUP g_aClassList_Extended[];

extern HANDLELOOKUP g_aLookup_Icon[];
extern HANDLELOOKUP g_aLookup_Cursor[];
extern HANDLELOOKUP g_aLookup_StockBrush[];
extern HANDLELOOKUP g_aLookup_SystemBrush[];
extern HANDLELOOKUP g_aLookup_Font[];


bool AppendFormatStyleLookup(WCHAR* Text, STYLELOOKUP *Lookup, uint Style);
bool AppendFormatHandleLookup(WCHAR* Text, HANDLELOOKUP* Lookup, HANDLE Handle);

QString AppendFormatStyleLookup(STYLELOOKUP *Lookup, uint Style, bool* Ok = NULL);
QString AppendFormatHandleLookup(HANDLELOOKUP* Lookup, HANDLE Handle, bool* Ok = NULL);

///////////////////////////////////////////////////////////////////////////////////////////////////

enum
{
	CMDHWND_CLOSE,
	CMDHWND_SELECT,
	CMDHWND_VISIBLE,
	CMDHWND_ENABLED,
	CMDHWND_TOPMOST,

	CMDHWND_FLASH,
	CMDHWND_CAPTURECLIENT,
	CMDHWND_CAPTUREWINDOW,

	CMDHWND_ADJUST,
	CMDHWND_SENDTOBACK,
	CMDHWND_BRINGTOFRONT,
};

void CommandHWnd(int Command, HWND hwndTarget);

///////////////////////////////////////////////////////////////////////////////////////////////////
extern "C"
{
BOOL FlashHWnd(HWND hWndTarget);
BOOL CaptureHWnd(HWND hWndTarget, BOOL bCaptureClient);
};
void ShowEditWindowSize(QWidget* Parent, HWND hWnd);

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // UTILS_H
