#include <tchar.h>
#include <windows.h>
#include "StylesItemModel.h"
#include "utils.h"
#include "utils2.h"

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <richedit.h>


#define STYLELOOKUPEX(style, cmp_mask, depends, excludes)		{	(UINT)style,	_T(#style),	(UINT)cmp_mask,	(UINT)depends,	(UINT)excludes	},
#define STYLELOOKUPEX_NULL										{	(UINT)(-1),		NULL,		(UINT)(-1),		(UINT)(-1),		(UINT)(-1)		}

#ifndef CCS_VERT
#define CCS_VERT  0x00000080L
#endif
#ifndef UDS_HOTTRACK
#define UDS_HOTTRACK  0x0100
#endif
#ifndef TBS_TOOLTIPS
#define TBS_TOOLTIPS  0x0100
#endif
#ifndef HDS_HOTTRACK
#define HDS_HOTTRACK  0x0004
#endif
#ifndef HDS_DRAGDROP
#define HDS_DRAGDROP  0x0040
#endif
#ifndef HDS_FULLDRAG
#define HDS_FULLDRAG  0x0080
#endif
#ifndef LVS_OWNERDATA
#define LVS_OWNERDATA 4096
#endif
#ifndef TVS_RTLREADING
#define TVS_RTLREADING  0x0040
#endif
#ifndef TVS_NOTOOLTIPS
#define TVS_NOTOOLTIPS  0x0080
#endif
#ifndef TVS_CHECKBOXES
#define TVS_CHECKBOXES  0x0100
#endif
#ifndef TVS_TRACKSELECT
#define TVS_TRACKSELECT  0x0200
#endif
#define RBS_REGISTERDROP  0x1000
#define RBS_AUTOSIZE  0x2000
#define RBS_VERTICALGRIPPER 0x4000
#define RBS_DBLCLKTOGGLE  0x8000
#ifndef TBSTYLE_FLAT
#define TBSTYLE_FLAT  0x0800
#endif
#define TBSTYLE_LIST  0x1000
#define TBSTYLE_CUSTOMERASE  0x2000
#ifndef TCS_SCROLLOPPOSITE
#define TCS_SCROLLOPPOSITE	0x0001
#endif
#ifndef TCS_BOTTOM
#define TCS_BOTTOM	2
#endif
#ifndef TCS_VERTICAL
#define TCS_VERTICAL	128
#endif
#ifndef TCS_RIGHT
#define TCS_RIGHT	2
#endif
#ifndef TCS_MULTISELECT
#define TCS_MULTISELECT	0x0004
#endif
#ifndef TCS_HOTTRACK
#define TCS_HOTTRACK	0x0040
#endif

#ifndef LVS_EX_GRIDLINES
#define LVS_EX_GRIDLINES 1
#endif
#ifndef LVS_EX_SUBITEMIMAGES
#define LVS_EX_SUBITEMIMAGES 2
#endif
#ifndef LVS_EX_CHECKBOXES
#define LVS_EX_CHECKBOXES 4
#endif
#ifndef LVS_EX_TRACKSELECT
#define LVS_EX_TRACKSELECT 8
#endif
#ifndef LVS_EX_HEADERDRAGDROP
#define LVS_EX_HEADERDRAGDROP 16
#endif
#ifndef LVS_EX_FULLROWSELECT
#define LVS_EX_FULLROWSELECT 32
#endif
#ifndef LVS_EX_ONECLICKACTIVATE
#define LVS_EX_ONECLICKACTIVATE 64
#endif
#ifndef LVS_EX_TWOCLICKACTIVATE
#define LVS_EX_TWOCLICKACTIVATE 128
#endif

#define ENM_PARAGRAPHEXPANDED  0x00000020

#define CBEM_GETEXTENDEDSTYLE  (WM_USER+9)
#define CBEM_SETEXTENDEDSTYLE  (WM_USER+14)
#define LVM_GETEXTENDEDLISTVIEWSTYLE (LVM_FIRST+55)
#define LVM_SETEXTENDEDLISTVIEWSTYLE (LVM_FIRST+54)
#define TB_GETEXTENDEDSTYLE  (WM_USER+85)
#define TB_SETEXTENDEDSTYLE  (WM_USER+84)
#define TCM_GETEXTENDEDSTYLE  (TCM_FIRST+53)
#define TCM_SETEXTENDEDSTYLE  (TCM_FIRST+52)

#ifndef TCS_EX_FLATSEPARATORS
#define TCS_EX_FLATSEPARATORS  0x00000001
#endif
#ifndef TCS_EX_REGISTERDROP
#define TCS_EX_REGISTERDROP  0x00000002
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////


StyleLookupEx WindowStyles[] =
{
	STYLELOOKUPEX(WS_OVERLAPPEDWINDOW,	0,				-1,												WS_POPUP|WS_CHILD)
	STYLELOOKUPEX(WS_POPUPWINDOW,		WS_POPUPWINDOW, -1,												0)

	STYLELOOKUPEX(WS_OVERLAPPED,		0,				-1,												WS_POPUP|WS_CHILD)	//0x00000000
	STYLELOOKUPEX(WS_POPUP,				0,				-1,												0)					//0x80000000
	STYLELOOKUPEX(WS_CHILD,				0,				-1,												0)					//0x40000000
	STYLELOOKUPEX(WS_MINIMIZE,			0,				-1,												0)					//0x20000000
	STYLELOOKUPEX(WS_VISIBLE,			0,				-1,												0)					//0x10000000
	STYLELOOKUPEX(WS_DISABLED,			0,				-1,												0)					//0x08000000
	STYLELOOKUPEX(WS_CLIPSIBLINGS,		0,				-1,												0)					//0x04000000
	STYLELOOKUPEX(WS_CLIPCHILDREN,		0,				-1,												0)					//0x02000000
	STYLELOOKUPEX(WS_MAXIMIZE,			0,				-1,												0)					//0x01000000

	STYLELOOKUPEX(WS_CAPTION,			0,				-1,												0)					//0x00C00000

	//(BORDER|CAPTION)
	STYLELOOKUPEX(WS_DLGFRAME,			0,				-1,												0)					//0x00400000

	STYLELOOKUPEX(WS_BORDER,			0,				-1,												0)					//0x00800000

	STYLELOOKUPEX(WS_VSCROLL,			0,				-1,												0)					//0x00200000
	STYLELOOKUPEX(WS_HSCROLL,			0,				-1,												0)					//0x00100000
	STYLELOOKUPEX(WS_SYSMENU,			0,				-1,												0)					//0x00080000
	STYLELOOKUPEX(WS_THICKFRAME,		0,				-1,												0)					//0x00040000
	STYLELOOKUPEX(WS_GROUP,				0,				-1,												0)					//0x00020000
	STYLELOOKUPEX(WS_TABSTOP,			0,				-1,												0)					//0x00010000

	STYLELOOKUPEX(WS_MINIMIZEBOX,		0,				WS_POPUPWINDOW|WS_OVERLAPPEDWINDOW|WS_CAPTION,	0)					//0x00020000
	STYLELOOKUPEX(WS_MAXIMIZEBOX,		0,				WS_POPUPWINDOW|WS_OVERLAPPEDWINDOW|WS_CAPTION,	0)					//0x00010000

	STYLELOOKUPEX_NULL
};

// Dialog box styles (class = #32770)
StyleLookupEx DialogStyles[] =
{
	STYLELOOKUPEX(DS_ABSALIGN,			0,		-1,		0)			//0x00000001
	STYLELOOKUPEX(DS_SYSMODAL,			0,		-1,		0)			//0x00000002
	STYLELOOKUPEX(DS_LOCALEDIT,			0,		-1,		0)			//0x00000020
	STYLELOOKUPEX(DS_SETFONT,			0,		-1,		0)			//0x00000040
	STYLELOOKUPEX(DS_MODALFRAME,		0,		-1,		0)			//0x00000080
	STYLELOOKUPEX(DS_NOIDLEMSG,			0,		-1,		0)			//0x00000100
	STYLELOOKUPEX(DS_SETFOREGROUND,		0,		-1,		0)			//0x00000200

#if(WINVER >= 0x0400)

	STYLELOOKUPEX(DS_3DLOOK,			0,		-1,		0)			//0x00000004
	STYLELOOKUPEX(DS_FIXEDSYS,			0,		-1,		0)			//0x00000008
	STYLELOOKUPEX(DS_NOFAILCREATE,		0,		-1,		0)			//0x00000010
	STYLELOOKUPEX(DS_CONTROL,			0,		-1,		0)			//0x00000400
	STYLELOOKUPEX(DS_CENTER,			0,		-1,		0)			//0x00000800
	STYLELOOKUPEX(DS_CENTERMOUSE,		0,		-1,		0)			//0x00001000
	STYLELOOKUPEX(DS_CONTEXTHELP,		0,		-1,		0)			//0x00002000

#endif

	STYLELOOKUPEX_NULL
};

// Button styles (Button)
StyleLookupEx ButtonStyles[] =
{
	STYLELOOKUPEX(BS_PUSHBUTTON,		0,		-1,		BS_DEFPUSHBUTTON|BS_CHECKBOX|BS_AUTOCHECKBOX|BS_RADIOBUTTON|BS_GROUPBOX|BS_AUTORADIOBUTTON)
	STYLELOOKUPEX(BS_DEFPUSHBUTTON,		0xf,	-1,		0)			//0x0001
	STYLELOOKUPEX(BS_CHECKBOX,			0xf,	-1,		0)			//0x0002
	STYLELOOKUPEX(BS_AUTOCHECKBOX,		0xf,	-1,		0)			//0x0003
	STYLELOOKUPEX(BS_RADIOBUTTON,		0xf,	-1,		0)			//0x0004
	STYLELOOKUPEX(BS_3STATE,			0xf,	-1,		0)			//0x0005
	STYLELOOKUPEX(BS_AUTO3STATE,		0xf,	-1,		0)			//0x0006
	STYLELOOKUPEX(BS_GROUPBOX,			0xf,	-1,		0)			//0x0007
	STYLELOOKUPEX(BS_USERBUTTON,		0xf,	-1,		0)			//0x0008
	STYLELOOKUPEX(BS_AUTORADIOBUTTON,	0xf,	-1,		0)			//0x0009
	STYLELOOKUPEX(BS_OWNERDRAW,			0xf,	-1,		0)			//0x000B
	STYLELOOKUPEX(BS_LEFTTEXT,			0,	 	-1,		0)			//0x0020

	//winver >= 4.0 (index 42)
	STYLELOOKUPEX(BS_TEXT,				0,		-1,		BS_ICON|BS_BITMAP|BS_AUTOCHECKBOX|BS_AUTORADIOBUTTON|BS_CHECKBOX|BS_RADIOBUTTON)	//0x00000000
	STYLELOOKUPEX(BS_ICON,				0,		-1,		0)			//0x0040
	STYLELOOKUPEX(BS_BITMAP,			0,		-1,		0)			//0x0080
	STYLELOOKUPEX(BS_LEFT,				0,		-1,		0)			//0x0100
	STYLELOOKUPEX(BS_RIGHT,				0,		-1,		0)			//0x0200
	STYLELOOKUPEX(BS_CENTER,			0,		-1,		0)			//0x0300
	STYLELOOKUPEX(BS_TOP,				0,		-1,		0)			//0x0400
	STYLELOOKUPEX(BS_BOTTOM,			0,		-1,		0)			//0x0800
	STYLELOOKUPEX(BS_VCENTER,			0,		-1,		0)			//0x0C00
	STYLELOOKUPEX(BS_PUSHLIKE,			0,		-1,		0)			//0x1000
	STYLELOOKUPEX(BS_MULTILINE,			0,		-1,		0)			//0x2000
	STYLELOOKUPEX(BS_NOTIFY,			0,		-1,		0)			//0x4000
	STYLELOOKUPEX(BS_FLAT,				0,		-1,		0)			//0x8000
	STYLELOOKUPEX(BS_RIGHTBUTTON,		0,		-1,		0)			//BS_LEFTTEXT

	STYLELOOKUPEX_NULL
};

// Edit styles (Edit)
StyleLookupEx EditStyles[] =
{
	STYLELOOKUPEX(ES_LEFT,				0,		-1,		ES_CENTER|ES_RIGHT)		//0x0000
	STYLELOOKUPEX(ES_CENTER,			0,		-1,		0)						//0x0001
	STYLELOOKUPEX(ES_RIGHT,				0,		-1,		0)						//0x0002
	STYLELOOKUPEX(ES_MULTILINE,			0,		-1,		0)						//0x0004
	STYLELOOKUPEX(ES_UPPERCASE,			0,		-1,		0)						//0x0008
	STYLELOOKUPEX(ES_LOWERCASE,			0,		-1,		0)						//0x0010
	STYLELOOKUPEX(ES_PASSWORD,			0,		-1,		0)						//0x0020
	STYLELOOKUPEX(ES_AUTOVSCROLL,		0,		-1,		0)						//0x0040
	STYLELOOKUPEX(ES_AUTOHSCROLL,		0,		-1,		0)						//0x0080
	STYLELOOKUPEX(ES_NOHIDESEL,			0,		-1,		0)						//0x0100
	STYLELOOKUPEX(ES_OEMCONVERT,		0,		-1,		0)						//0x0400
	STYLELOOKUPEX(ES_READONLY,			0,		-1,		0)						//0x0800
	STYLELOOKUPEX(ES_WANTRETURN,		0,		-1,		0)						//0x1000
	STYLELOOKUPEX(ES_NUMBER,			0,		-1,		0)						//0x2000

	STYLELOOKUPEX_NULL
};

StyleLookupEx RichedStyles[] =
{
	// Standard edit control styles
	STYLELOOKUPEX(ES_LEFT,				0,		-1,		ES_CENTER|ES_RIGHT)		//0x0000
	STYLELOOKUPEX(ES_CENTER,			0,		-1,		0)						//0x0001
	STYLELOOKUPEX(ES_RIGHT,				0,		-1,		0)						//0x0002
	STYLELOOKUPEX(ES_MULTILINE,			0,		-1,		0)						//0x0004
	//STYLELOOKUPEX(ES_UPPERCASE,		0,		-1,		0)						//0x0008
	//STYLELOOKUPEX(ES_LOWERCASE,		0,		-1,		0)						//0x0010
	STYLELOOKUPEX(ES_PASSWORD,			0,		-1,		0)						//0x0020
	STYLELOOKUPEX(ES_AUTOVSCROLL,		0,		-1,		0)						//0x0040
	STYLELOOKUPEX(ES_AUTOHSCROLL,		0,		-1,		0)						//0x0080
	STYLELOOKUPEX(ES_NOHIDESEL,			0,		-1,		0)						//0x0100
	//STYLELOOKUPEX(ES_OEMCONVERT,		0,		-1,		0)						//0x0400
	STYLELOOKUPEX(ES_READONLY,			0,		-1,		0)						//0x0800
	STYLELOOKUPEX(ES_WANTRETURN,		0,		-1,		0)						//0x1000
	STYLELOOKUPEX(ES_NUMBER,			0,		-1,		0)						//0x2000

	// Addition Rich Edit control styles

	STYLELOOKUPEX(ES_SAVESEL,			0,		-1,		0)						//0x00008000
	STYLELOOKUPEX(ES_SUNKEN,			0,		-1,		0)						//0x00004000
	STYLELOOKUPEX(ES_DISABLENOSCROLL,	0,		-1,		0)						//0x00002000
	STYLELOOKUPEX(ES_SELECTIONBAR,		0,		-1,		0)						//0x01000000
	STYLELOOKUPEX(ES_NOOLEDRAGDROP,		0,		-1,		0)						//0x00000008

	STYLELOOKUPEX_NULL
};

// Combo box styles (combobox)
StyleLookupEx ComboStyles[] =
{
	STYLELOOKUPEX(CBS_SIMPLE,				0x3,	-1,		0)		//0x0001
	STYLELOOKUPEX(CBS_DROPDOWN,				0x3,	-1,		0)		//0x0002
	STYLELOOKUPEX(CBS_DROPDOWNLIST,			0x3,	-1,		0)		//0x0003
	STYLELOOKUPEX(CBS_OWNERDRAWFIXED,		0,		-1,		0)		//0x0010
	STYLELOOKUPEX(CBS_OWNERDRAWVARIABLE,	0,		-1,		0)		//0x0020
	STYLELOOKUPEX(CBS_AUTOHSCROLL,			0,		-1,		0)		//0x0040
	STYLELOOKUPEX(CBS_OEMCONVERT,			0,		-1,		0)		//0x0080
	STYLELOOKUPEX(CBS_SORT,					0,		-1,		0)		//0x0100
	STYLELOOKUPEX(CBS_HASSTRINGS,			0,		-1,		0)		//0x0200
	STYLELOOKUPEX(CBS_NOINTEGRALHEIGHT,		0,		-1,		0)		//0x0400
	STYLELOOKUPEX(CBS_DISABLENOSCROLL,		0,		-1,		0)		//0x0800

#if(WINVER >= 0x0400)
	STYLELOOKUPEX(CBS_UPPERCASE,			0,		-1,		0)		//0x1000
	STYLELOOKUPEX(CBS_LOWERCASE,			0,		-1,		0)		//0x2000
#endif

	STYLELOOKUPEX_NULL
};

// Listbox styles (Listbox)
StyleLookupEx ListBoxStyles[] =
{
	STYLELOOKUPEX(LBS_NOTIFY,				0,		-1,		0)		//0x0001
	STYLELOOKUPEX(LBS_SORT,					0,		-1,		0)		//0x0002
	STYLELOOKUPEX(LBS_NOREDRAW,				0,		-1,		0)		//0x0004
	STYLELOOKUPEX(LBS_MULTIPLESEL,			0,		-1,		0)		//0x0008
	STYLELOOKUPEX(LBS_OWNERDRAWFIXED,		0,		-1,		0)		//0x0010
	STYLELOOKUPEX(LBS_OWNERDRAWVARIABLE,	0,		-1,		0)		//0x0020
	STYLELOOKUPEX(LBS_HASSTRINGS,			0,		-1,		0)		//0x0040
	STYLELOOKUPEX(LBS_USETABSTOPS,			0,		-1,		0)		//0x0080
	STYLELOOKUPEX(LBS_NOINTEGRALHEIGHT,		0,		-1,		0)		//0x0100
	STYLELOOKUPEX(LBS_MULTICOLUMN,			0,		-1,		0)		//0x0200
	STYLELOOKUPEX(LBS_WANTKEYBOARDINPUT,	0,		-1,		0)		//0x0400
	STYLELOOKUPEX(LBS_EXTENDEDSEL,			0,		-1,		0)		//0x0800
	STYLELOOKUPEX(LBS_DISABLENOSCROLL,		0,		-1,		0)		//0x1000
	STYLELOOKUPEX(LBS_NODATA,				0,		-1,		0)		//0x2000
	STYLELOOKUPEX(LBS_NOSEL,				0,		-1,		0)		//0x4000

	STYLELOOKUPEX_NULL
};

// Scrollbar control styles (Scrollbar)
StyleLookupEx ScrollbarStyles[] =
{
	STYLELOOKUPEX(SBS_TOPALIGN,					0,	SBS_HORZ,							0)									//0x0002
	STYLELOOKUPEX(SBS_LEFTALIGN,				0,	SBS_VERT,							0)									//0x0002
	STYLELOOKUPEX(SBS_BOTTOMALIGN,				0,	SBS_HORZ|SBS_SIZEBOX|SBS_SIZEGRIP,	0)									//0x0004
	STYLELOOKUPEX(SBS_RIGHTALIGN,				0,	SBS_VERT|SBS_SIZEBOX|SBS_SIZEGRIP,	0)									//0x0004
	STYLELOOKUPEX(SBS_HORZ,						0,	-1,									SBS_VERT|SBS_SIZEBOX|SBS_SIZEGRIP)	//0x0000
	STYLELOOKUPEX(SBS_VERT,						0,	-1,									SBS_SIZEBOX|SBS_SIZEGRIP)			//0x0001
	STYLELOOKUPEX(SBS_SIZEBOXTOPLEFTALIGN,		0,	SBS_SIZEBOX|SBS_SIZEGRIP,			0)									//0x0002
	STYLELOOKUPEX(SBS_SIZEBOXBOTTOMRIGHTALIGN,	0,	SBS_SIZEBOX|SBS_SIZEGRIP,			0)									//0x0004
	STYLELOOKUPEX(SBS_SIZEBOX,					0,	-1,									0)									//0x0008
	STYLELOOKUPEX(SBS_SIZEGRIP,					0,	-1,									0)									//0x0010

	STYLELOOKUPEX_NULL
};

// Static control styles (Static)
StyleLookupEx StaticStyles[] =
{
	STYLELOOKUPEX(SS_LEFT,				0x1f,	-1,		SS_CENTER|SS_RIGHT)		//0x0000
	STYLELOOKUPEX(SS_CENTER,			0x1f,	-1,		0)						//0x0001
	STYLELOOKUPEX(SS_RIGHT,				0x1f,	-1,		0)						//0x0002
	STYLELOOKUPEX(SS_ICON,				0x1f,	-1,		0)						//0x0003
	STYLELOOKUPEX(SS_BLACKRECT,			0x1f,	-1,		0)						//0x0004
	STYLELOOKUPEX(SS_GRAYRECT,			0x1f,	-1,		0)						//0x0005
	STYLELOOKUPEX(SS_WHITERECT,			0x1f,	-1,		0)						//0x0006
	STYLELOOKUPEX(SS_BLACKFRAME,		0x1f,	-1,		0)						//0x0007
	STYLELOOKUPEX(SS_GRAYFRAME,			0x1f,	-1,		0)						//0x0008
	STYLELOOKUPEX(SS_WHITEFRAME,		0x1f,	-1,		0)						//0x0009
	STYLELOOKUPEX(SS_USERITEM,			0x1f,	-1,		0)						//0x000A
	STYLELOOKUPEX(SS_SIMPLE,			0x1f,	-1,		0)						//0x000B
	STYLELOOKUPEX(SS_LEFTNOWORDWRAP,	0x1f,	-1,		0)						//0x000C

	STYLELOOKUPEX(SS_OWNERDRAW,			0x1f,	-1,		0)						//0x000D
	STYLELOOKUPEX(SS_BITMAP,			0x1f,	-1,		0)						//0x000E
	STYLELOOKUPEX(SS_ENHMETAFILE,		0x1f,	-1,		0)						//0x000F
	STYLELOOKUPEX(SS_ETCHEDHORZ,		0x1f,	-1,		0)						//0x0010
	STYLELOOKUPEX(SS_ETCHEDVERT,		0x1f,	-1,		0)						//0x0011
	STYLELOOKUPEX(SS_ETCHEDFRAME,		0x1f,	-1,		0)						//0x0012
	STYLELOOKUPEX(SS_TYPEMASK,			0x1f,	-1,		0)						//0x001F
	STYLELOOKUPEX(SS_NOPREFIX,			0,		-1,		0)						//0x0080

	STYLELOOKUPEX(SS_NOTIFY,			0,		-1,		0)						//0x0100
	STYLELOOKUPEX(SS_CENTERIMAGE,		0,		-1,		0)						//0x0200
	STYLELOOKUPEX(SS_RIGHTJUST,			0,		-1,		0)						//0x0400
	STYLELOOKUPEX(SS_REALSIZEIMAGE,		0,		-1,		0)						//0x0800
	STYLELOOKUPEX(SS_SUNKEN,			0,		-1,		0)						//0x1000
	STYLELOOKUPEX(SS_ENDELLIPSIS,		0,		-1,		0)						//0x4000
	STYLELOOKUPEX(SS_PATHELLIPSIS,		0,		-1,		0)						//0x8000
	STYLELOOKUPEX(SS_WORDELLIPSIS,		0,		-1,		0)						//0xC000
	STYLELOOKUPEX(SS_ELLIPSISMASK,		0,		-1,		0)						//0xC000

	STYLELOOKUPEX_NULL
};

//	Standard Common controls styles
StyleLookupEx CommCtrlList[] =
{
	STYLELOOKUPEX(CCS_TOP,				0x3,	-1,		0)			//0x0001
	STYLELOOKUPEX(CCS_NOMOVEY,			0x3,	-1,		0)			//0x0002
	STYLELOOKUPEX(CCS_BOTTOM,			0x3,	-1,		0)			//0x0003
	STYLELOOKUPEX(CCS_NORESIZE,			0,		-1,		0)			//0x0004
	STYLELOOKUPEX(CCS_NOPARENTALIGN,	0,		-1,		0)			//0x0008

	STYLELOOKUPEX(CCS_ADJUSTABLE,		0,		-1,		0)			//0x0020
	STYLELOOKUPEX(CCS_NODIVIDER,		0,		-1,		0)			//0x0040

#if (_WIN32_IE >= 0x0300)
	STYLELOOKUPEX(CCS_VERT,				0,		-1,		0)			//0x0080
#endif

	STYLELOOKUPEX_NULL
};

//  DragList - uses same styles as listview

// Header control (SysHeader32)
StyleLookupEx HeaderStyles[] =
{
	STYLELOOKUPEX(HDS_HORZ,				0,		-1,		16)			//0x0000
	STYLELOOKUPEX(HDS_BUTTONS,			0,		-1,		0)			//0x0002

#if (_WIN32_IE >= 0x0300)
	STYLELOOKUPEX(HDS_HOTTRACK,			0,		-1,		0)			//0x0004
	STYLELOOKUPEX(HDS_DRAGDROP,			0,		-1,		0)			//0x0040
	STYLELOOKUPEX(HDS_FULLDRAG,			0,		-1,		0)			//0x0080
#endif

	STYLELOOKUPEX(HDS_HIDDEN,			0,		-1,		0)			//0x0008

#if (_WIN32_IE >= 0x0500)
	STYLELOOKUPEX(HDS_FILTERBAR,		0,		-1,		0)			//0x0100
#endif

	STYLELOOKUPEX_NULL
};

// Listview (SysListView32)
StyleLookupEx ListViewStyles[] =
{
	STYLELOOKUPEX(LVS_ICON,					LVS_TYPEMASK,	-1,		LVS_REPORT|LVS_SMALLICON|LVS_LIST) //0x0000
	STYLELOOKUPEX(LVS_REPORT,				LVS_TYPEMASK,	-1,		0)		//0x0001
	STYLELOOKUPEX(LVS_SMALLICON,			LVS_TYPEMASK,	-1,		0)		//0x0002
	STYLELOOKUPEX(LVS_LIST,					LVS_TYPEMASK,	-1,		0)		//0x0003

	STYLELOOKUPEX(LVS_SINGLESEL,			0,				-1,		0)		//0x0004
	STYLELOOKUPEX(LVS_SHOWSELALWAYS,		0,				-1,		0)		//0x0008
	STYLELOOKUPEX(LVS_SORTASCENDING,		0,				-1,		0)		//0x0010
	STYLELOOKUPEX(LVS_SORTDESCENDING,		0,				-1,		0)		//0x0020
	STYLELOOKUPEX(LVS_SHAREIMAGELISTS,		0,				-1,		0)		//0x0040
	STYLELOOKUPEX(LVS_NOLABELWRAP,			0,				-1,		0)		//0x0080
	STYLELOOKUPEX(LVS_AUTOARRANGE,			0,				-1,		0)		//0x0100
	STYLELOOKUPEX(LVS_EDITLABELS,			0,				-1,		0)		//0x0200

#if (_WIN32_IE >= 0x0300)
	STYLELOOKUPEX(LVS_OWNERDATA,			0,				-1,		0)		//0x1000
#endif

	STYLELOOKUPEX(LVS_NOSCROLL,				0,				-1,		0)		//0x2000

	STYLELOOKUPEX(LVS_ALIGNTOP,				0,				-1,		0)		//0x0000
	STYLELOOKUPEX(LVS_ALIGNLEFT,			LVS_ALIGNMASK,	-1,		0)		//0x0800

//	STYLELOOKUPEX(LVS_ALIGNMASK),			0,				-1,		0)		//0x0c00
//	STYLELOOKUPEX(LVS_TYPESTYLEMASK,		0,				-1,		0)		//0xfc00

	STYLELOOKUPEX(LVS_OWNERDRAWFIXED,		0,				-1,		0)		//0x0400
	STYLELOOKUPEX(LVS_NOCOLUMNHEADER,		0,				-1,		0)		//0x4000
	STYLELOOKUPEX(LVS_NOSORTHEADER,			0,				-1,		0)		//0x8000

	STYLELOOKUPEX_NULL
};

// Toolbar control (ToolbarWindow32)
StyleLookupEx ToolbarStyles[] =
{
	STYLELOOKUPEX(TBSTYLE_TOOLTIPS,			0,		-1,		0)		//0x0100
	STYLELOOKUPEX(TBSTYLE_WRAPABLE,			0,		-1,		0)		//0x0200
	STYLELOOKUPEX(TBSTYLE_ALTDRAG,			0,		-1,		0)		//0x0400

#if (_WIN32_IE >= 0x0300)
	STYLELOOKUPEX(TBSTYLE_FLAT,				0,		-1,		0)		//0x0800
	STYLELOOKUPEX(TBSTYLE_LIST,				0,		-1,		0)		//0x1000
	STYLELOOKUPEX(TBSTYLE_CUSTOMERASE,		0,		-1,		0)		//0x2000
#endif

#if (_WIN32_IE >= 0x0400)
	STYLELOOKUPEX(TBSTYLE_REGISTERDROP,		0,		-1,		0)		//0x4000
	STYLELOOKUPEX(TBSTYLE_TRANSPARENT,		0,		-1,		0)		//0x8000
#endif

	STYLELOOKUPEX_NULL
};

// Rebar control (RebarControl32)
StyleLookupEx RebarStyles[] =
{
#if (_WIN32_IE >= 0x0400)
	STYLELOOKUPEX(RBS_TOOLTIPS,				0,		-1,		0)		//0x0100
	STYLELOOKUPEX(RBS_VARHEIGHT,			0,		-1,		0)		//0x0200
	STYLELOOKUPEX(RBS_BANDBORDERS,			0,		-1,		0)		//0x0400
	STYLELOOKUPEX(RBS_FIXEDORDER,			0,		-1,		0)		//0x0800
	STYLELOOKUPEX(RBS_REGISTERDROP,			0,		-1,		0)		//0x1000
	STYLELOOKUPEX(RBS_AUTOSIZE,				0,		-1,		0)		//0x2000
	STYLELOOKUPEX(RBS_VERTICALGRIPPER,		0,		-1,		0)		//0x4000
	STYLELOOKUPEX(RBS_DBLCLKTOGGLE,			0,		-1,		0)		//0x8000
#endif

	STYLELOOKUPEX_NULL
};

// Track Bar control (msctls_trackbar32)
StyleLookupEx TrackbarStyles[] =
{
	STYLELOOKUPEX(TBS_AUTOTICKS,			0xf,	-1,		0)				//0x0001
	STYLELOOKUPEX(TBS_VERT,					0xf,	-1,		0)				//0x0002
	STYLELOOKUPEX(TBS_HORZ,					0xf,	-1,		TBS_VERT)		//0x0000
	STYLELOOKUPEX(TBS_TOP,					0xf,	-1,		0)				//0x0004
	STYLELOOKUPEX(TBS_BOTTOM,				0xf,	-1,		TBS_TOP)		//0x0000
	STYLELOOKUPEX(TBS_LEFT,					0xf,	-1,		0)				//0x0004
	STYLELOOKUPEX(TBS_RIGHT,				0xf,	-1,		TBS_LEFT)		//0x0000
	STYLELOOKUPEX(TBS_BOTH,					0xf,	-1,		0)				//0x0008

	STYLELOOKUPEX(TBS_NOTICKS,				0,		-1,		0)				//0x0010
	STYLELOOKUPEX(TBS_ENABLESELRANGE,		0,		-1,		0)				//0x0020
	STYLELOOKUPEX(TBS_FIXEDLENGTH,			0,		-1,		0)				//0x0040
	STYLELOOKUPEX(TBS_NOTHUMB,				0,		-1,		0)				//0x0080

#if (_WIN32_IE >= 0x0300)
	STYLELOOKUPEX(TBS_TOOLTIPS,				0,		-1,		0)				//0x0100
#endif

#if (_WIN32_IE >= 0x0500)
	STYLELOOKUPEX(TBS_REVERSED,				0,		-1,		0)				//0x0200
#endif

	STYLELOOKUPEX_NULL
};

// Treeview (SysTreeView32)
StyleLookupEx TreeViewStyles[] =
{
	STYLELOOKUPEX(TVS_HASBUTTONS,			0,		-1,		0)			//0x0001
	STYLELOOKUPEX(TVS_HASLINES,				0,		-1,		0)			//0x0002
	STYLELOOKUPEX(TVS_LINESATROOT,			0,		-1,		0)			//0x0004
	STYLELOOKUPEX(TVS_EDITLABELS,			0,		-1,		0)			//0x0008
	STYLELOOKUPEX(TVS_DISABLEDRAGDROP,		0,		-1,		0)			//0x0010
	STYLELOOKUPEX(TVS_SHOWSELALWAYS,		0,		-1,		0)			//0x0020

#if (_WIN32_IE >= 0x0300)
	STYLELOOKUPEX(TVS_RTLREADING,			0,		-1,		0)			//0x0040
	STYLELOOKUPEX(TVS_NOTOOLTIPS,			0,		-1,		0)			//0x0080
	STYLELOOKUPEX(TVS_CHECKBOXES,			0,		-1,		0)			//0x0100
	STYLELOOKUPEX(TVS_TRACKSELECT,			0,		-1,		0)			//0x0200

#if (_WIN32_IE >= 0x0400)
	STYLELOOKUPEX(TVS_SINGLEEXPAND,			0,		-1,		0)			//0x0400
	STYLELOOKUPEX(TVS_INFOTIP,				0,		-1,		0)			//0x0800
	STYLELOOKUPEX(TVS_FULLROWSELECT,		0,		-1,		0)			//0x1000
	STYLELOOKUPEX(TVS_NOSCROLL,				0,		-1,		0)			//0x2000
	STYLELOOKUPEX(TVS_NONEVENHEIGHT,		0,		-1,		0)			//0x4000

#if (_WIN32_IE >= 0x500)
	STYLELOOKUPEX(TVS_NOHSCROLL,			0,		-1,		0)			//0x8000

#endif
#endif
#endif

	STYLELOOKUPEX_NULL
};

// Tooltips (tooltips_class32)
StyleLookupEx ToolTipStyles[] =
{
	STYLELOOKUPEX(TTS_ALWAYSTIP,			0,		-1,		0)			//0x01
	STYLELOOKUPEX(TTS_NOPREFIX,				0,		-1,		0)			//0x02

#if (_WIN32_IE >= 0x0500)
	STYLELOOKUPEX(TTS_NOANIMATE,			0,		-1,		0)			//0x10
	STYLELOOKUPEX(TTS_NOFADE,				0,		-1,		0)			//0x20
	STYLELOOKUPEX(TTS_BALLOON,				0,		-1,		0)			//0x40
#endif

	STYLELOOKUPEX_NULL
};

// Statusbar (msctls_statusbar32)
StyleLookupEx StatusBarStyles[] =
{
	STYLELOOKUPEX(SBARS_SIZEGRIP,			0,		-1,		0)			//0x0100

#if (_WIN32_IE >= 0x0400)
	STYLELOOKUPEX(SBT_TOOLTIPS,				0,		-1,		0)			//0x0800
#endif

	STYLELOOKUPEX_NULL
};

// Updown control
StyleLookupEx UpDownStyles[] =
{
	STYLELOOKUPEX(UDS_WRAP,					0,		-1,		0)			//0x0001
	STYLELOOKUPEX(UDS_SETBUDDYINT,			0,		-1,		0)			//0x0002
	STYLELOOKUPEX(UDS_ALIGNRIGHT,			0,		-1,		0)			//0x0004
	STYLELOOKUPEX(UDS_ALIGNLEFT,			0,		-1,		0)			//0x0008
	STYLELOOKUPEX(UDS_AUTOBUDDY,			0,		-1,		0)			//0x0010
	STYLELOOKUPEX(UDS_ARROWKEYS,			0,		-1,		0)			//0x0020
	STYLELOOKUPEX(UDS_HORZ,					0,		-1,		0)			//0x0040
	STYLELOOKUPEX(UDS_NOTHOUSANDS,			0,		-1,		0)			//0x0080

#if (_WIN32_IE >= 0x0300)
	STYLELOOKUPEX(UDS_HOTTRACK,				0,		-1,		0)			//0x0100
#endif

	STYLELOOKUPEX_NULL
};

// Progress control (msctls_progress32)
StyleLookupEx ProgressStyles[] =
{
#if (_WIN32_IE >= 0x0300)
	STYLELOOKUPEX(PBS_SMOOTH,				0,		-1,		0)			//0x01
	STYLELOOKUPEX(PBS_VERTICAL,				0,		-1,		0)			//0x04
#endif

	STYLELOOKUPEX_NULL
};

// Tab control (SysTabControl32)
StyleLookupEx TabStyles[] =
{
#if (_WIN32_IE >= 0x0300)
	STYLELOOKUPEX(TCS_SCROLLOPPOSITE,		0,		-1,				0)				//0x0001   // assumes multiline tab
	STYLELOOKUPEX(TCS_BOTTOM,				0,		TCS_VERTICAL,	0)				//0x0002
	STYLELOOKUPEX(TCS_RIGHT,				0,		-1,				0)				//0x0002
	STYLELOOKUPEX(TCS_MULTISELECT,			0,		-1,				0)				//0x0004
#endif
#if (_WIN32_IE >= 0x0400)
	STYLELOOKUPEX(TCS_FLATBUTTONS,			0,		-1,				0)				//0x0008
#endif
	STYLELOOKUPEX(TCS_FORCEICONLEFT,		0,		-1,				0)				//0x0010
	STYLELOOKUPEX(TCS_FORCELABELLEFT,		0,		-1,				0)				//0x0020
#if (_WIN32_IE >= 0x0300)
	STYLELOOKUPEX(TCS_HOTTRACK,				0,		-1,				0)				//0x0040
	STYLELOOKUPEX(TCS_VERTICAL,				0,		-1,				0)				//0x0080
#endif
	STYLELOOKUPEX(TCS_TABS,					0,		-1,				TCS_BUTTONS)	//0x0000
	STYLELOOKUPEX(TCS_BUTTONS,				0,		-1,				0)				//0x0100
	STYLELOOKUPEX(TCS_SINGLELINE,			0,		-1,				TCS_MULTILINE)	//0x0000
	STYLELOOKUPEX(TCS_MULTILINE,			0,		-1,				0)				//0x0200
	STYLELOOKUPEX(TCS_RIGHTJUSTIFY,			0,		-1,				-1)				//0x0000
	STYLELOOKUPEX(TCS_FIXEDWIDTH,			0,		-1,				0)				//0x0400
	STYLELOOKUPEX(TCS_RAGGEDRIGHT,			0,		-1,				0)				//0x0800
	STYLELOOKUPEX(TCS_FOCUSONBUTTONDOWN,	0,		-1,				0)				//0x1000
	STYLELOOKUPEX(TCS_OWNERDRAWFIXED,		0,		-1,				0)				//0x2000
	STYLELOOKUPEX(TCS_TOOLTIPS,				0,		-1,				0)				//0x4000
	STYLELOOKUPEX(TCS_FOCUSNEVER,			0,		-1,				0)				//0x8000

	STYLELOOKUPEX_NULL
};

// Animation control (SysAnimate32)
StyleLookupEx AnimateStyles[] =
{
	STYLELOOKUPEX(ACS_CENTER,				0,		-1,		0)			//0x0001
	STYLELOOKUPEX(ACS_TRANSPARENT,			0,		-1,		0)			//0x0002
	STYLELOOKUPEX(ACS_AUTOPLAY,				0,		-1,		0)			//0x0004

#if (_WIN32_IE >= 0x0300)
	STYLELOOKUPEX(ACS_TIMER,				0,		-1,		0)			//0x0008
#endif

	STYLELOOKUPEX_NULL
};

// Month-calendar control (SysMonthCal32)
StyleLookupEx MonthCalStyles[] =
{
	STYLELOOKUPEX(MCS_DAYSTATE,				0,		-1,		0)			//0x0001
	STYLELOOKUPEX(MCS_MULTISELECT,			0,		-1,		0)			//0x0002
	STYLELOOKUPEX(MCS_WEEKNUMBERS,			0,		-1,		0)			//0x0004

#if (_WIN32_IE >= 0x0400)
	STYLELOOKUPEX(MCS_NOTODAYCIRCLE,		0,		-1,		0)			//0x0008
	STYLELOOKUPEX(MCS_NOTODAY,				0,		-1,		0)			//0x0010
#endif

	STYLELOOKUPEX_NULL
};

// Date-Time picker (SysDateTimePick32)
StyleLookupEx DateTimeStyles[] =
{
	STYLELOOKUPEX(DTS_UPDOWN,					0,		-1,		0)						//0x0001
	STYLELOOKUPEX(DTS_SHOWNONE,					0,		-1,		0)						//0x0002
	STYLELOOKUPEX(DTS_SHORTDATEFORMAT,			0,		-1,		DTS_LONGDATEFORMAT)		//0x0000
	STYLELOOKUPEX(DTS_LONGDATEFORMAT,			0,		-1,		0)						//0x0004

#if (_WIN32_IE >= 0x500)
	STYLELOOKUPEX(DTS_SHORTDATECENTURYFORMAT,	0,		-1,		0)						//0x000C
#endif

	STYLELOOKUPEX(DTS_TIMEFORMAT,				0,		-1,		0)						//0x0009
	STYLELOOKUPEX(DTS_APPCANPARSE,				0,		-1,		0)						//0x0010
	STYLELOOKUPEX(DTS_RIGHTALIGN,				0,		-1,		0)						//0x0020

	STYLELOOKUPEX_NULL
};

// Pager control (SysPager)
StyleLookupEx PagerStyles[] =
{
	//Pager control
	STYLELOOKUPEX(PGS_VERT,						0,		-1,		PGS_HORZ)	//0x0000
	STYLELOOKUPEX(PGS_HORZ,						0,		-1,		0)			//0x0001
	STYLELOOKUPEX(PGS_AUTOSCROLL,				0,		-1,		0)			//0x0002
	STYLELOOKUPEX(PGS_DRAGNDROP,				0,		-1,		0)			//0x0004

	STYLELOOKUPEX_NULL
};

// Extended window styles (for all windows)
StyleLookupEx StyleExList[] =
{
	STYLELOOKUPEX(WS_EX_DLGMODALFRAME,			0,		-1,		0)							//0x00000001L
	STYLELOOKUPEX(WS_EX_NOPARENTNOTIFY,			0,		-1,		0)							//0x00000004L
	STYLELOOKUPEX(WS_EX_TOPMOST,				0,		-1,		0)							//0x00000008L
	STYLELOOKUPEX(WS_EX_ACCEPTFILES,			0,		-1,		0)							//0x00000010L
	STYLELOOKUPEX(WS_EX_TRANSPARENT,			0,		-1,		0)							//0x00000020L

#if(WINVER >= 0x0400)

	STYLELOOKUPEX(WS_EX_MDICHILD,				0,		-1,		0)							//0x00000040L
	STYLELOOKUPEX(WS_EX_TOOLWINDOW,				0,		-1,		0)							//0x00000080L
	STYLELOOKUPEX(WS_EX_WINDOWEDGE,				0,		-1,		0)							//0x00000100L
	STYLELOOKUPEX(WS_EX_CLIENTEDGE,				0,		-1,		0)							//0x00000200L
	STYLELOOKUPEX(WS_EX_CONTEXTHELP,			0,		-1,		0)							//0x00000400L

	STYLELOOKUPEX(WS_EX_LEFT,					0,		-1,		WS_EX_RIGHT)				//0x00000000L
	STYLELOOKUPEX(WS_EX_RIGHT,					0,		-1,		0)							//0x00001000L
	STYLELOOKUPEX(WS_EX_LTRREADING,				0,		-1,		WS_EX_RTLREADING)			//0x00000000L
	STYLELOOKUPEX(WS_EX_RTLREADING,				0,		-1,		0)							//0x00002000L
	STYLELOOKUPEX(WS_EX_LEFTSCROLLBAR,			0,		-1,		0)							//0x00004000L
	STYLELOOKUPEX(WS_EX_RIGHTSCROLLBAR,			0,		-1,		WS_EX_LEFTSCROLLBAR)		//0x00000000L

	STYLELOOKUPEX(WS_EX_CONTROLPARENT,			0,		-1,		0)							//0x00010000L
	STYLELOOKUPEX(WS_EX_STATICEDGE,				0,		-1,		0)							//0x00020000L
	STYLELOOKUPEX(WS_EX_APPWINDOW,				0,		-1,		0)							//0x00040000L

	STYLELOOKUPEX(WS_EX_OVERLAPPEDWINDOW,		0,		-1,		0)							//(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE)
//	STYLELOOKUPEX(WS_EX_PALETTEWINDOW,			0,		-1,		0)							//(WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST)

#endif

	STYLELOOKUPEX_NULL
};

#if 0	//!!!
// ListView extended styles
StyleLookupEx ListViewExStyles[] =
{
	//ListView control styles
	STYLELOOKUPEX(LVS_EX_GRIDLINES,				0,		-1,		0)		//0x00000001
	STYLELOOKUPEX(LVS_EX_SUBITEMIMAGES,			0,		-1,		0)		//0x00000002
	STYLELOOKUPEX(LVS_EX_CHECKBOXES,			0,		-1,		0)		//0x00000004
	STYLELOOKUPEX(LVS_EX_TRACKSELECT,			0,		-1,		0)		//0x00000008
	STYLELOOKUPEX(LVS_EX_HEADERDRAGDROP,		0,		-1,		0)		//0x00000010
	STYLELOOKUPEX(LVS_EX_FULLROWSELECT,			0,		-1,		0)		//0x00000020
	STYLELOOKUPEX(LVS_EX_ONECLICKACTIVATE,		0,		-1,		0)		//0x00000040
	STYLELOOKUPEX(LVS_EX_TWOCLICKACTIVATE,		0,		-1,		0)		//0x00000080
#if (_WIN32_IE >= 0x0400)
	STYLELOOKUPEX(LVS_EX_FLATSB,				0,		-1,		0)		//0x00000100
	STYLELOOKUPEX(LVS_EX_REGIONAL,				0,		-1,		0)		//0x00000200
	STYLELOOKUPEX(LVS_EX_INFOTIP,				0,		-1,		0)		//0x00000400
	STYLELOOKUPEX(LVS_EX_UNDERLINEHOT,			0,		-1,		0)		//0x00000800
	STYLELOOKUPEX(LVS_EX_UNDERLINECOLD,			0,		-1,		0)		//0x00001000
	STYLELOOKUPEX(LVS_EX_MULTIWORKAREAS,		0,		-1,		0)		//0x00002000
#endif
#if (_WIN32_IE >= 0x0500)
	STYLELOOKUPEX(LVS_EX_LABELTIP,				0,		-1,		0)		//0x00004000
#endif

	STYLELOOKUPEX_NULL
};
// ComboBoxEx extended styles
StyleLookupEx ComboBoxExStyles[] =
{
	STYLELOOKUPEX(CBES_EX_NOEDITIMAGE,			0,		-1,		0)		//0x00000001
	STYLELOOKUPEX(CBES_EX_NOEDITIMAGEINDENT,	0,		-1,		0)		//0x00000002
	STYLELOOKUPEX(CBES_EX_PATHWORDBREAKPROC,	0,		-1,		0)		//0x00000004

#if(_WIN32_IE >= 0x0400)
	STYLELOOKUPEX(CBES_EX_NOSIZELIMIT,			0,		-1,		0)		//0x00000008
	STYLELOOKUPEX(CBES_EX_CASESENSITIVE,		0,		-1,		0)		//0x00000010
#endif

	STYLELOOKUPEX_NULL
};

// Tab control extended styles
StyleLookupEx TabCtrlExStyles[] =
{
	STYLELOOKUPEX(TCS_EX_FLATSEPARATORS,		0,		-1,		0)		//0x00000001
	STYLELOOKUPEX(TCS_EX_REGISTERDROP,			0,		-1,		0)		//0x00000002
	
	STYLELOOKUPEX_NULL
};


// Toolbar extended styles
StyleLookupEx ToolBarExStyles[] =
{
#if (_WIN32_IE >= 0x0400)
	STYLELOOKUPEX(TBSTYLE_EX_DRAWDDARROWS,			0,		-1,		0)	//0x0001

#if (_WIN32_IE >= 0x0501)
	STYLELOOKUPEX(TBSTYLE_EX_MIXEDBUTTONS,			0,		-1,		0)	//0x0008
	STYLELOOKUPEX(TBSTYLE_EX_HIDECLIPPEDBUTTONS,	0,		-1,		0)	//0x0010

#endif
#endif

	STYLELOOKUPEX_NULL
};
#endif

// Support RichEdit Event masks!!!
StyleLookupEx RichedEventMask[] =
{
	STYLELOOKUPEX(ENM_NONE,					0,		-1,		-1)		//0x00000000
	STYLELOOKUPEX(ENM_CHANGE,				0,		-1,		0)		//0x00000001
	STYLELOOKUPEX(ENM_UPDATE,				0,		-1,		0)		//0x00000002
	STYLELOOKUPEX(ENM_SCROLL,				0,		-1,		0)		//0x00000004
	STYLELOOKUPEX(ENM_KEYEVENTS,			0,		-1,		0)		//0x00010000
	STYLELOOKUPEX(ENM_MOUSEEVENTS,			0,		-1,		0)		//0x00020000
	STYLELOOKUPEX(ENM_REQUESTRESIZE,		0,		-1,		0)		//0x00040000
	STYLELOOKUPEX(ENM_SELCHANGE,			0,		-1,		0)		//0x00080000
	STYLELOOKUPEX(ENM_DROPFILES,			0,		-1,		0)		//0x00100000
	STYLELOOKUPEX(ENM_PROTECTED,			0,		-1,		0)		//0x00200000
	STYLELOOKUPEX(ENM_CORRECTTEXT,			0,		-1,		0)		//0x00400000		// PenWin specific
	STYLELOOKUPEX(ENM_SCROLLEVENTS,			0,		-1,		0)		//0x00000008
	STYLELOOKUPEX(ENM_DRAGDROPDONE,			0,		-1,		0)		//0x00000010

	// Far East specific notification mask
	STYLELOOKUPEX(ENM_IMECHANGE,			0,		-1,		0)		//0x00800000		// unused by RE2.0
	STYLELOOKUPEX(ENM_LANGCHANGE,			0,		-1,		0)		//0x01000000
	STYLELOOKUPEX(ENM_OBJECTPOSITIONS,		0,		-1,		0)		//0x02000000
	STYLELOOKUPEX(ENM_LINK,					0,		-1,		0)		//0x04000000

	STYLELOOKUPEX_NULL
};

//
//	Lookup table which matches window classnames to style-lists
//
ClassStyleLookup StandardControls[] =
{
	_L("#32770"), 				DialogStyles,		0, 0,
	_L("Button"),				ButtonStyles,		0, 0,
	_L("ComboBox"),				ComboStyles,		0, 0,
	_L("Edit"),					EditStyles,			0, 0,
	_L("ListBox"),				ListBoxStyles,		0, 0,

	_L("RICHEDIT"),				RichedStyles,		0, 0,
	_L("RichEdit20A"),			RichedStyles,		0, 0,
	_L("RichEdit20W"),			RichedStyles,		0, 0,

	_L("Scrollbar"),			ScrollbarStyles,	0, 0,
	_L("Static"),				StaticStyles,		0, 0,

	_L("SysAnimate32"),			AnimateStyles,		0, 0,
	_L("ComboBoxEx"),			ComboStyles,		0, 0,	//(Just a normal combobox)
	_L("SysDateTimePick32"),	DateTimeStyles,		0, 0,
	_L("DragList"),				ListBoxStyles,		0, 0,	//(Just a normal list)
	_L("SysHeader32"),			HeaderStyles,		0, 0,
	//"SysIPAddress32",			IPAddressStyles,	0, 0,	(NO STYLES)
	_L("SysListView32"),		ListViewStyles,		0, 0,
	_L("SysMonthCal32"),		MonthCalStyles,		0, 0,
	_L("SysPager"),				PagerStyles,		0, 0,
	_L("msctls_progress32"),	ProgressStyles,		0, 0,
	_L("RebarWindow32"),		RebarStyles,		0, 0,
	_L("msctls_statusbar32"),	StatusBarStyles,	0, 0,
	//"SysLink",				SysLinkStyles,		0, 0,  (DO IT!)
	_L("SysTabControl32"),		TabStyles,			0, 0,
	_L("ToolbarWindow32"),		ToolbarStyles,		0, 0,
	_L("tooltips_class32"),		ToolTipStyles,		0, 0,
	_L("msctls_trackbar32"),	TrackbarStyles,		0, 0,
	_L("SysTreeView32"),		TreeViewStyles,		0, 0,
	_L("msctls_updown32"),		UpDownStyles,		0, 0,

	_L(""),						0,					0, 0,
};

// Classes which use the CCS_xxx styles
ClassStyleLookup CustomControls[] =
{
	_L("msctls_statusbar32"),	CommCtrlList,		0, 0,
	_L("RebarWindow32"),		CommCtrlList,		0, 0,
	_L("ToolbarWindow32"),		CommCtrlList,		0, 0,
	_L("SysHeader32"),			CommCtrlList,		0, 0,

	_L(""),						0,					0, 0,
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// Toolbar extended styles
StyleLookupEx ToolBarExStyles[] =
{
#if (_WIN32_IE >= 0x0400)
	STYLELOOKUPEX(TBSTYLE_EX_DRAWDDARROWS,			0,		-1,		0)	// 0x00000001
#endif
#if (_WIN32_IE >= 0x0501)
	STYLELOOKUPEX(TBSTYLE_EX_MIXEDBUTTONS,			0,		-1,		0)	// 0x00000008
	STYLELOOKUPEX(TBSTYLE_EX_HIDECLIPPEDBUTTONS,	0,		-1,		0)	// 0x00000010
#endif
#if (_WIN32_WINNT >= 0x501)
	STYLELOOKUPEX(TBSTYLE_EX_DOUBLEBUFFER,			0,		-1,		0)	// 0x00000080
#endif
	// ...
	STYLELOOKUPEX_NULL
};

// Tab control extended styles
StyleLookupEx TabCtrlExStyles[] =
{
#if (_WIN32_IE >= 0x0400)
	STYLELOOKUPEX(TCS_EX_FLATSEPARATORS,			0,		-1,		0)	//0x00000001
	STYLELOOKUPEX(TCS_EX_REGISTERDROP,				0,		-1,		0)	//0x00000002
#endif
	STYLELOOKUPEX_NULL
};

// ComboBoxEx extended styles
StyleLookupEx ComboBoxExStyles[] =
{
	STYLELOOKUPEX(CBES_EX_NOEDITIMAGE,				0,		-1,		0)	// 0x00000001
	STYLELOOKUPEX(CBES_EX_NOEDITIMAGEINDENT,		0,		-1,		0)	// 0x00000002
	STYLELOOKUPEX(CBES_EX_PATHWORDBREAKPROC,		0,		-1,		0)	// 0x00000004
#if (_WIN32_IE >= 0x0400)
	STYLELOOKUPEX(CBES_EX_NOSIZELIMIT,				0,		-1,		0)	// 0x00000008
	STYLELOOKUPEX(CBES_EX_CASESENSITIVE,			0,		-1,		0)	// 0x00000010
#endif /* _WIN32_IE >= 0x0400 */
#if (_WIN32_WINNT >= 0x0600)
	STYLELOOKUPEX(CBES_EX_TEXTENDELLIPSIS,			0,		-1,		0)	// 0x00000020
#endif /* _WIN32_WINNT >= 0x0600 */
	// ...
	STYLELOOKUPEX_NULL
};
// ListView extended styles
StyleLookupEx ListViewExStyles[] =
{
	STYLELOOKUPEX(LVS_EX_GRIDLINES,					0,		-1,		0)		// 0x00000001
	STYLELOOKUPEX(LVS_EX_SUBITEMIMAGES,				0,		-1,		0)		// 0x00000002
	STYLELOOKUPEX(LVS_EX_CHECKBOXES,				0,		-1,		0)		// 0x00000004
	STYLELOOKUPEX(LVS_EX_TRACKSELECT,				0,		-1,		0)		// 0x00000008
	STYLELOOKUPEX(LVS_EX_HEADERDRAGDROP,			0,		-1,		0)		// 0x00000010
	STYLELOOKUPEX(LVS_EX_FULLROWSELECT,				0,		-1,		0)		// 0x00000020
	STYLELOOKUPEX(LVS_EX_ONECLICKACTIVATE,			0,		-1,		0)		// 0x00000040
	STYLELOOKUPEX(LVS_EX_TWOCLICKACTIVATE,			0,		-1,		0)		// 0x00000080
#if (_WIN32_IE >= 0x0400)
	STYLELOOKUPEX(LVS_EX_FLATSB,					0,		-1,		0)		// 0x00000100
	STYLELOOKUPEX(LVS_EX_REGIONAL,					0,		-1,		0)		// 0x00000200
	STYLELOOKUPEX(LVS_EX_INFOTIP,					0,		-1,		0)		// 0x00000400
	STYLELOOKUPEX(LVS_EX_UNDERLINEHOT,				0,		-1,		0)		// 0x00000800
	STYLELOOKUPEX(LVS_EX_UNDERLINECOLD,				0,		-1,		0)		// 0x00001000
	STYLELOOKUPEX(LVS_EX_MULTIWORKAREAS,			0,		-1,		0)		// 0x00002000
#endif
#if (_WIN32_IE >= 0x0500)
	STYLELOOKUPEX(LVS_EX_LABELTIP,					0,		-1,		0)		// 0x00004000
	STYLELOOKUPEX(LVS_EX_BORDERSELECT,				0,		-1,		0)		// 0x00008000
#endif
#if (_WIN32_WINNT >= 0x501)
	STYLELOOKUPEX(LVS_EX_DOUBLEBUFFER,				0,		-1,		0)		// 0x00010000
	STYLELOOKUPEX(LVS_EX_HIDELABELS,				0,		-1,		0)		// 0x00020000
	STYLELOOKUPEX(LVS_EX_SINGLEROW,					0,		-1,		0)		// 0x00040000
	STYLELOOKUPEX(LVS_EX_SNAPTOGRID,				0,		-1,		0)		// 0x00080000
	STYLELOOKUPEX(LVS_EX_SIMPLESELECT,				0,		-1,		0)		// 0x00100000
#endif
#if (_WIN32_WINNT >= 0x0600)
	STYLELOOKUPEX(LVS_EX_JUSTIFYCOLUMNS,			0,		-1,		0)		// 0x00200000
	STYLELOOKUPEX(LVS_EX_TRANSPARENTBKGND,			0,		-1,		0)		// 0x00400000
	STYLELOOKUPEX(LVS_EX_TRANSPARENTSHADOWTEXT,		0,		-1,		0)		// 0x00800000
	STYLELOOKUPEX(LVS_EX_AUTOAUTOARRANGE,			0,		-1,		0)		// 0x01000000
	STYLELOOKUPEX(LVS_EX_HEADERINALLVIEWS,			0,		-1,		0)		// 0x02000000
	STYLELOOKUPEX(LVS_EX_AUTOCHECKSELECT,			0,		-1,		0)		// 0x08000000
	STYLELOOKUPEX(LVS_EX_AUTOSIZECOLUMNS,			0,		-1,		0)		// 0x10000000
	STYLELOOKUPEX(LVS_EX_COLUMNSNAPPOINTS,			0,		-1,		0)		// 0x40000000
	STYLELOOKUPEX(LVS_EX_COLUMNOVERFLOW,			0,		-1,		0)		// 0x80000000
#endif /* _WIN32_WINNT >= 0x0600 */
	// ...
	STYLELOOKUPEX_NULL
};
// TreeView extended styles
StyleLookupEx TreeViewExStyles[] =
{
#if (_WIN32_WINNT >= 0x0600)
	STYLELOOKUPEX(TVS_EX_MULTISELECT,				0,		-1,		0)		// 0x0002
	STYLELOOKUPEX(TVS_EX_DOUBLEBUFFER,				0,		-1,		0)		// 0x0004
	STYLELOOKUPEX(TVS_EX_NOINDENTSTATE,				0,		-1,		0)		// 0x0008
	STYLELOOKUPEX(TVS_EX_RICHTOOLTIP,				0,		-1,		0)		// 0x0010
	STYLELOOKUPEX(TVS_EX_AUTOHSCROLL,				0,		-1,		0)		// 0x0020
	STYLELOOKUPEX(TVS_EX_FADEINOUTEXPANDOS,			0,		-1,		0)		// 0x0040
	STYLELOOKUPEX(TVS_EX_PARTIALCHECKBOXES,			0,		-1,		0)		// 0x0080
	STYLELOOKUPEX(TVS_EX_EXCLUSIONCHECKBOXES,		0,		-1,		0)		// 0x0100
	STYLELOOKUPEX(TVS_EX_DIMMEDCHECKBOXES,			0,		-1,		0)		// 0x0200
	STYLELOOKUPEX(TVS_EX_DRAWIMAGEASYNC,			0,		-1,		0)		// 0x0400
#endif /* _WIN32_WINNT >= 0x0600 */
	
	STYLELOOKUPEX_NULL
};


///////////////////////////////////////////////////////////////////////////////////////////////////


// Classes which have extended window styles
ClassStyleLookup ExtendedControls[] =
{
	CLASSNAME_TOOLBAR,			ToolBarExStyles,	TB_GETEXTENDEDSTYLE,			TB_SETEXTENDEDSTYLE,
	CLASSNAME_TABCONTROL,		TabCtrlExStyles,	TCM_GETEXTENDEDSTYLE,			TCM_SETEXTENDEDSTYLE,
	CLASSNAME_COMBOBOX,			ComboBoxExStyles,	CBEM_GETEXTENDEDSTYLE,			CBEM_SETEXTENDEDSTYLE,
	CLASSNAME_LISTVIEW,			ListViewExStyles,	LVM_GETEXTENDEDLISTVIEWSTYLE,	LVM_SETEXTENDEDLISTVIEWSTYLE,
#if (_WIN32_WINNT >= 0x0501)
	CLASSNAME_TREEVIEW,			TreeViewExStyles,	TVM_GETEXTENDEDSTYLE,			TVM_SETEXTENDEDSTYLE,
#endif /* _WIN32_WINNT >= 0x0501 */
	CLASSNAME_RICHEDIT,			RichedEventMask,	EM_GETEVENTMASK,				EM_SETEVENTMASK,
	CLASSNAME_RICHEDIT20A,		RichedEventMask,	EM_GETEVENTMASK,				EM_SETEVENTMASK,
	CLASSNAME_RICHEDIT20W,		RichedEventMask,	EM_GETEVENTMASK,				EM_SETEVENTMASK,

	_L(""),						0,					0, 0,
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_COLUMNS		2
///////////////////////////////////////////////////////////////////////////////////////////////////

StylesItemModel::StylesItemModel(QObject* parent, bool edition) : QAbstractItemModel(parent)
{
	/*
	MODE				mode;
	HWND				hwnd;
	QList<DATA>			list;
	DWORD				style;
	*/
	this->hwnd = 0;
	this->style = 0;
	this->edition = edition;
}
StylesItemModel::~StylesItemModel()
{
}
QModelIndex		StylesItemModel::index(int row, int column, const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	if(row < 0)
		return QModelIndex();

	if((row >= 0 && row < list.size()) && (column >= 0 && column < MAX_COLUMNS))
	{
		return createIndex(row, column);
	}
	return QModelIndex();
}
QModelIndex		StylesItemModel::parent(const QModelIndex &child) const
{
	Q_UNUSED(child);
	return QModelIndex();
}
int				StylesItemModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return list.size();
}
int				StylesItemModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return MAX_COLUMNS;
}
bool			StylesItemModel::hasChildren(const QModelIndex &parent) const
{
	if(!parent.isValid())
		return true;
	return false;
}
QVariant		StylesItemModel::data(const QModelIndex &index, int role) const
{
	if(index.isValid() && (index.row() < list.size()))
	{
		if(!edition)
		{
			if(role == Qt::DisplayRole)
			{
				const DATA& data = list.at(index.row());

				switch(index.column())
				{
					case 0:
						return QString().sprintf(HEX_FORMAT, (uint)data.style);
					case 1:
						return data.name;
				}
			}
		}
		else
		{
			if(role == Qt::DisplayRole)
			{
				const DATA& data = list.at(index.row());

				switch(index.column())
				{
					case 0:
						return data.name;
//						return QHexHandle((void*)data.style);
					case 1:
						if(style & data.style)
							return tr("Yes");
						return tr("No");
				}
			}
		}
	}
	return QVariant();
}
QVariant		StylesItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	Q_UNUSED(orientation);
	if(!edition)
	{
		if(role == Qt::DisplayRole)
		{
			if(section == 0)
			{
				return QString(tr("Value"));
			}
			else if(section == 1)
			{
				return QString(tr("Name"));
			}
		}
	}
	else
	{
		if(role == Qt::DisplayRole)
		{
			if(section == 0)
			{
				return QString(tr("Styles"));
			}
			else if(section == 1)
			{
				return QString(tr("Activated"));
			}
		}
	}
	return QVariant();
}
Qt::ItemFlags	StylesItemModel::flags(const QModelIndex &index) const
{
	if(edition)
	{
		if(!index.isValid())
			return Qt::ItemIsEnabled;
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	}
	if(!index.isValid())
		return Qt::ItemIsEnabled;
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}


//
// Match the window classname to a
//
// pClassList - a lookup table of classname / matching stylelist
//
StyleLookupEx *FindStyleList(ClassStyleLookup *pClassList, const WCHAR *szClassName, DWORD* pdwGetData, DWORD* pdwSetData)
{
	int i;

	for(i = 0; pClassList[i].stylelist != 0; i++)
	{
		if(wcsicmp(szClassName, pClassList[i].szClassName) == 0)
		{
			if(pdwGetData)
				*pdwGetData = pClassList[i].dwGetData;
			if(pdwSetData)
				*pdwSetData = pClassList[i].dwSetData;
			return pClassList[i].stylelist;
		}
	}

	return 0;
}

#define NUM_CLASS_STYLELISTS	(sizeof(ClassStyleList) / sizeof(ClassStyleList[0]))

//
//	Find all the styles that match from the specified list
//
//	StyleList  - list of styles
//  hwndList   - listbox to add styles to
//  dwStyle    - style for the target window
//  fAllStyles -
//
DWORD EnumStyles(StyleLookupEx* StyleList, QList<StylesItemModel::DATA>& list, DWORD dwStyle, BOOL fAllStyles)
{
	// Remember what the style is before we start modifying it
	DWORD dwOrig = dwStyle;

	int            i;
	BOOL           fAddIt;
	StyleLookupEx *pStyle;

	//
	//	Loop through all of the styles that we know about
	//	Check each style against our window's one, to see
	//  if it is set or not
	//
	for(i = 0; StyleList[i].name != NULL; i++)
	{
		fAddIt = FALSE;

		pStyle = &StyleList[i];

		// This style needs a mask to detect if it is set -
		// i.e. the style doesn't just use one bit to decide if it is on/off.
		if(pStyle->cmp_mask != 0)
		{
			//if((StyleList[i].depends & dwStyle) != dwStyle)
			//	continue;

			// Style is valid if the excludes styles are not set
			if(pStyle->excludes != 0 && (pStyle->excludes & (dwOrig & pStyle->cmp_mask)) == 0)
				fAddIt = TRUE;

			// If the style matches exactly (when masked)
			if(pStyle->style != 0 && (pStyle->cmp_mask & dwStyle) == pStyle->style)
				fAddIt = TRUE;
		}
		else
		{
			// Style is valid when
			if(pStyle->excludes != 0 && (pStyle->excludes & dwOrig) == 0)
				fAddIt = TRUE;

			// If style matches exactly (all bits are set
			if(pStyle->style != 0 && (pStyle->style & dwStyle) == pStyle->style)
				fAddIt = TRUE;

			// If no bits are set, then we have to skip it
			else if(pStyle->style != 0 && (pStyle->style & dwStyle) == 0)
				fAddIt = FALSE;

			// If this style depends on others being set..
			if(dwStyle &&  (pStyle->depends & dwStyle) == 0)
				fAddIt = FALSE;
		}

		// Now add the style..
		if(fAddIt == TRUE || fAllStyles)
		{
			// We've added this style, so remove it to stop it appearing again
			if(fAddIt)
				dwStyle &= ~ (pStyle->style);

			list.append(StylesItemModel::DATA(QString::fromWCharArray(pStyle->name), pStyle->style));
		}
	}

	// return the style. This will be zero if we decoded all the bits
	// that were set, or non-zero if there are still bits left
	return dwStyle;
}


DWORD			StylesItemModel::getStyle() const
{
	return style;
}
void			StylesItemModel::setStyle(HWND hwnd, MODE mode, BOOL fAllStyles, DWORD* pStyle)
{
	beginResetModel();
	// ...
	WCHAR	szClassName[256];
	DWORD	dwStyle = 0;
//	BOOL	fAllStyles;
//	BOOL	fExtControl;
	DWORD	dwGetMessage;
	DWORD	dwSetMessage;
	StyleLookupEx *StyleList;

//	fAllStyles = false;
//	fExtControl = false;

	// window class
	GetClassNameW(hwnd, szClassName, sizeof(szClassName) / sizeof(WCHAR));

	// Empty the list
	list.clear();

	// Normal window styles
	if(mode == MODE_NORMAL)
	{
		dwStyle = GetWindowLong(hwnd, GWL_STYLE);
		this->style = dwStyle;
		if(dwStyle)
		{
			// enumerate the standard window styles, for any window no
			// matter what class it might be
			dwStyle = EnumStyles(WindowStyles, list, dwStyle, fAllStyles);

			// if the window class is one we know about, then see if we
			// can decode any more style bits
			// enumerate the custom control styles
			StyleList = FindStyleList(StandardControls, szClassName, 0, 0);
			if(StyleList != 0)
			{
				dwStyle = EnumStyles(StyleList, list, dwStyle, fAllStyles);
			}
			// does the window support the CCS_xxx styles (custom control styles)
			StyleList = FindStyleList(CustomControls, szClassName, 0, 0);
			if(StyleList != 0)
			{
				dwStyle = EnumStyles(StyleList, list, dwStyle, fAllStyles);
			}
		}
	}
	// Extended window styles
	if(mode == MODE_EXTENDED)
	{
		// Find extended styles
		dwStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
		this->style = dwStyle;
		if(dwStyle)
		{
			dwStyle = EnumStyles(StyleExList, list, dwStyle, fAllStyles);
		}
	}
	if(mode == MODE_CONTROL)
	{
		StyleList = FindStyleList(ExtendedControls, szClassName, &dwGetMessage, &dwSetMessage);

		// Add them if required
		if(StyleList != 0)
		{
			dwStyle = SendMessage(hwnd, dwGetMessage, 0, 0);
			this->style = dwStyle;

			dwStyle = EnumStyles(StyleList, list, dwStyle, fAllStyles);
		}
	}
	// if there are still style bits set in the window style,
	// then there is something that we can't decode. Just display
	// a single HEX entry at the end of the list.
	if(dwStyle != 0)
	{
		QString s;

		s.sprintf(HEX_FORMAT, (unsigned int)dwStyle);
		list.append(StylesItemModel::DATA(s, dwStyle));
	}
	// ...
	this->hwnd = hwnd;
	if(pStyle)
		*pStyle = this->style;

	endResetModel();
}



