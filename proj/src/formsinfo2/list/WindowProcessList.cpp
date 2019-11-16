#include "WindowProcessList.h"
#include "utils.h"
#include <QtWinExtras/QtWin>

#define MAX_CLASS_LEN   40

#define INSERTMODE_LAST		0
#define INSERTMODE_FIRST	1

#define WINTREE_INCLUDE_HANDLE		0x0001
#define WINTREE_INCLUDE_CLASS		0x0002
#define WINTREE_INCLUDE_CAPTION		0x0004

static uint uTreeInclude = WINTREE_INCLUDE_HANDLE | WINTREE_INCLUDE_CLASS;

extern "C"
{
BOOL GetProcessNameByPid(DWORD dwProcessId, WCHAR szName[], DWORD nNameSize, WCHAR szPath[], DWORD nPathSize);
};

typedef struct _CLASS_LOOKUP CLASS_LOOKUP;
typedef struct _CLASSIMAGE_LOOKUP CLASSIMAGE_LOOKUP;

typedef struct _CLASS_LOOKUP
{
	PCWSTR					ClassName;
	CLASSIMAGE_LOOKUP*		ImageLookup;
}CLASS_LOOKUP;

typedef struct _CLASSIMAGE_LOOKUP
{
	PCWSTR					ImageName;
	DWORD					Mask;
	DWORD					AdjustStyles;
	QImage					Image;
}CLASSIMAGE_LOOKUP;

extern CLASS_LOOKUP g_ClassLookup[];
QPixmap		IconFromClassName(CONST WCHAR* szName, DWORD dwStyle, const QImage& DefaultImage)
{
	DWORD Mask;
	CLASS_LOOKUP* ClassLookup;
	CLASSIMAGE_LOOKUP* ImageLookup;

	ClassLookup = &g_ClassLookup[0];
	for( ; ClassLookup->ClassName != NULL; ClassLookup++)
	{
		if(!ClassLookup->ClassName)
			continue;
		if(0 == wcsicmp(ClassLookup->ClassName, szName))
		{
			ImageLookup = ClassLookup->ImageLookup;
			if(ImageLookup)
			{
				for( ; ImageLookup->ImageName != NULL; ImageLookup++)
				{
					Mask = ImageLookup->Mask;
					if(ImageLookup->AdjustStyles != 0)
					{
						if(Mask == 0)
						{
							if(ImageLookup->AdjustStyles & dwStyle)
							{
								return QPixmap::fromImage(ImageLookup->Image);
							}
						}
						else
						{
							if(ImageLookup->AdjustStyles == (dwStyle & Mask))
							{
								return QPixmap::fromImage(ImageLookup->Image);
							}
						}
					}
					if(ImageLookup->AdjustStyles == 0)
						return QPixmap::fromImage(ImageLookup->Image);
				}
			}
		}
	}
	return QPixmap::fromImage(DefaultImage);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
WindowNode::WindowNode() : QTreeWidgetItem()
{
}
WindowNode::~WindowNode()
{
}
QVariant WindowNode::data(int column, int role) const
{
	if(column == 0)
	{
		if(role == Qt::DisplayRole)
		{
			QString s;

//			if(uTreeInclude & WINTREE_INCLUDE_HANDLE)
			{
				s += QString().sprintf(HEX_FORMAT, (uint)m_Handle);
			}
			if(uTreeInclude & WINTREE_INCLUDE_CLASS)
			{
				s += "  ";
				s += QGetClassName(m_Handle);
			}
			if(uTreeInclude & WINTREE_INCLUDE_CAPTION)
			{
				s += "  ";
				s += QGetWindowText(m_Handle);
			}
			return s;
		}
		else if(role == Qt::DecorationRole)
		{
			return m_Icon;
		}
	}
	else if(column == 1)
	{
	}
	return QVariant();
}
HWND     WindowNode::handle() const
{
	return m_Handle;
}
ProcessNode::ProcessNode() : QTreeWidgetItem()
{
	m_ProcessId = 0;
}
ProcessNode::~ProcessNode()
{
}
QVariant ProcessNode::data(int column, int role) const
{
	if(column == 0)
	{
		if(role == Qt::DisplayRole)
			return m_Name + QString().sprintf(" (%d)", (uint)m_ProcessId);
		else if(role == Qt::DecorationRole)
			return m_Icon;
	}
	return QVariant();
}
///////////////////////////////////////////////////////////////////////////////////////////////////

static BOOL CALLBACK AllWindowProc(HWND hWnd, LPARAM lParam)
{
	WindowProcessList* d = (WindowProcessList*)lParam;
	// ...
	ProcessNode* Process;

	Process = d->getProcess(hWnd);
	if(!Process)
	{
		return false;
	}
	UINT uStyle;
	HWND hWndParent;
	WCHAR szClassName[MAX_CLASS_LEN];
	QImage Image;
	uint InsertMode;

	uStyle = GetWindowLong(hWnd, GWL_STYLE);
	hWndParent = GetParent(hWnd);

	//
	// Seleccionar imagen
	// y modo de inserción, dependiendo del tipo de ventana
	//
	if(uStyle & WS_CHILD)
	{
		// Ventanas hijas (edit boxes, list boxes etc)
		Image = d->m_ImageChild;
		InsertMode = INSERTMODE_LAST;
	}
	else if((uStyle & WS_POPUPWINDOW) == WS_POPUPWINDOW)
	{
		// Ventanas de dialogo
		Image = d->m_ImageDialog;
		InsertMode = INSERTMODE_FIRST;
	}
	else if(uStyle & WS_POPUP)
	{
		// Ventanas popup (tooltips, etc)
		Image = d->m_ImagePopup;
		InsertMode = INSERTMODE_LAST;
	}
	else
	{
		// Ventana top-level
		Image = d->m_ImageWindow;
		InsertMode = INSERTMODE_FIRST;
	}
	WindowNode* Node;
	WindowNode* Parent;

	// Crear nodo
	Node = new WindowNode();
	Node->m_List = d;
	Node->m_Handle = hWnd;
	GetClassNameW(hWnd, szClassName, MAX_CLASS_LEN);
	Node->m_Icon = IconFromClassName(szClassName, uStyle, Image);

	// Buscar nodo padre
	Parent = d->m_Windows[hWndParent];
	if(Parent)
	{
		if(InsertMode == INSERTMODE_FIRST)
			Parent->addChild(Node);
		else
			Parent->insertChild(0, Node);
	}
	else
	{
		if(InsertMode == INSERTMODE_FIRST)
			Process->addChild(Node);
		else
			Process->insertChild(0, Node);
	}
	d->m_Windows.insert(hWnd, Node);
	return true;
}
WindowProcessList::WindowProcessList(QTreeWidgetItem* root)
{
	m_Root = root;
	// ...
	m_ImageChild = QImage(":/images/class/class_default_child.png");
	m_ImagePopup = QImage(":/images/class/class_default_popup.png");
	m_ImageDialog = QImage(":/images/class/class_default_dialog.png");
	m_ImageWindow = QImage(":/images/class/class_default_window.png");
}
WindowProcessList::~WindowProcessList()
{
}
void			WindowProcessList::clear()
{
	QList<QTreeWidgetItem*> children = m_Root->takeChildren();

	for(int i = 0; i < children.size(); i++)
	{
		delete children.at(i);
	}
	children.clear();
	m_Windows.clear();
}
void			WindowProcessList::refresh()
{
	clear();

	// Enumerar ventanas
	EnumChildWindows(GetDesktopWindow(), AllWindowProc, (LPARAM)this);

	m_Windows.clear();
}
WindowNode*	WindowProcessList::getWindow(HWND hwnd)
{
	WindowNode* node;
	for(int i = 0; i < m_Root->childCount(); i++)
	{
		node = searchWindow(m_Root->child(i), hwnd);
		if(node)
			return node;
	}
	return NULL;
}
ProcessNode*	WindowProcessList::getProcess(HWND hwnd)
{
	int i;
	DWORD pid;
	ProcessNode* process;

	GetWindowThreadProcessId(hwnd, &pid);

	// Buscar nodo de proceso de ventana
	for(i = 0; i < m_Root->childCount(); i++)
	{
		process = static_cast<ProcessNode*>(m_Root->child(i));
		if(process && process->m_ProcessId == pid)
		{
			return process;
		}
	}
	// En caso de no encontrar un nodo
	// para el proceso de la ventana, crear un nuevo nodo
	{
		SHFILEINFO		shfi;
		WCHAR			name[100] = L"";
		WCHAR			path[MAX_PATH] = L"";

		process = new ProcessNode();
		process->m_List = this;
		process->m_ProcessId = pid;

		GetProcessNameByPid(pid, name, 100, path, MAX_PATH);
		process->m_Name = QString::fromWCharArray(name);
//		process->m_Path = QString::fromWCharArray(path);

		// Obtener el icono del proceso
		memset(&shfi, 0, sizeof(shfi));
		SHGetFileInfo(path, 0, &shfi, sizeof(shfi), SHGFI_SMALLICON | SHGFI_ICON);
#if (QT_VERSION_MAJOR >= 5)
		process->m_Icon = QtWin::fromHICON(shfi.hIcon);
#else
		process->m_Icon = QPixmap::fromWinHICON(shfi.hIcon);
#endif
		DestroyIcon(shfi.hIcon);

		// Agregar a la lista
#if 0	//!!!
		m_Process.append(process);
#else
		m_Root->addChild(process);
#endif
	}
	return process;
}
WindowNode*	WindowProcessList::searchWindow(QTreeWidgetItem* item, HWND hwnd) const
{
	for(int i = 0; i < item->childCount(); i++)
	{
		WindowNode* node;
		QTreeWidgetItem* child = item->child(i);

		if(child->childCount())
		{
			node = searchWindow(child, hwnd);
			if(node)
				return node;
		}
		if(NULL != (node = dynamic_cast<WindowNode*>(child)))
		{
			if(node->m_Handle == hwnd)
			{
				return node;
			}
		}
	}
	return NULL;
}

