#include "WindowsItemModel.h"
#include "utils.h"

struct ENUMWINDOWINFO
{
	HWND			hWnd;
	QList<HWND>*	pList;
};

BOOL CALLBACK EnumProcChildWindows(HWND hWnd, LPARAM lParam)
{
	ENUMWINDOWINFO* pInfo;
	if(NULL != (pInfo = (ENUMWINDOWINFO*)lParam))

	if(GetParent(hWnd) == pInfo->hWnd)
	{
		pInfo->pList->append(hWnd);
	}
	return TRUE;
}
BOOL CALLBACK EnumProcSiblingWindows(HWND hWnd, LPARAM lParam)
{
	ENUMWINDOWINFO* pInfo;
	if(NULL != (pInfo = (ENUMWINDOWINFO*)lParam))
	{
		if((hWnd != pInfo->hWnd) && (GetParent(hWnd) == GetParent(pInfo->hWnd)))
		{
			pInfo->pList->append(hWnd);
		}
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_COLUMNS	3

WindowsItemModel::WindowsItemModel(QObject* parent, MODE mode) : QAbstractItemModel(parent)
{
	this->mode = mode;
}
WindowsItemModel::~WindowsItemModel()
{
}
QModelIndex		WindowsItemModel::index(int row, int column, const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	if(row < 0)
		return QModelIndex();

	if((row >= 0 && row < list.size()) && (column >= 0 && column < MAX_COLUMNS))
	{
		return createIndex(row, column, list.at(row));
	}
	return QModelIndex();
}
QModelIndex		WindowsItemModel::parent(const QModelIndex &child) const
{
	Q_UNUSED(child);
	return QModelIndex();
}
int				WindowsItemModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return list.size();
}
int				WindowsItemModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return MAX_COLUMNS;
}
bool			WindowsItemModel::hasChildren(const QModelIndex &parent) const
{
	if(!parent.isValid())
		return true;
	return false;
}
QVariant		WindowsItemModel::data(const QModelIndex &index, int role) const
{
	if(index.isValid() && (index.row() < list.size()))
	{
		if(role == Qt::DisplayRole)
		{
			QString s;
			HWND hwnd = (HWND)list.at(index.row());

			switch(index.column())
			{
				case 0:
					return QHexHandle(hwnd);
				case 1:
					return QGetClassName(hwnd);
				case 2:
					return QGetWindowText(hwnd);
			}
		}
	}
	return QVariant();
}
QVariant		WindowsItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	Q_UNUSED(orientation);
	if(role == Qt::DisplayRole)
	{
		if(section == 0)
		{
			return QString(tr("Handle"));
		}
		else if(section == 1)
		{
			return QString(tr("Class name"));
		}
		else if(section == 2)
		{
			return QString(tr("Window text"));
		}
	}
	return QVariant();
}
Qt::ItemFlags	WindowsItemModel::flags(const QModelIndex &index) const
{
	if(!index.isValid())
		return Qt::ItemIsEnabled;
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
void*			WindowsItemModel::getHandle(int i)
{
	if(i >= 0 && i < list.size())
		return list.at(i);
	return NULL;
}
void			WindowsItemModel::setHandle(void* p)
{
	beginResetModel();
	list.clear();
	if(p)
	{
		ENUMWINDOWINFO Info;

		Info.hWnd = (HWND)p;
		Info.pList = &list;
		if(mode == MODE_CHILDS)
		{
			EnumChildWindows((HWND)p, (WNDENUMPROC)EnumProcChildWindows, (LPARAM)&Info);
		}
		else if(mode == MODE_SIBLINGS)
		{
			EnumChildWindows(GetParent((HWND)p), (WNDENUMPROC)EnumProcSiblingWindows, (LPARAM)&Info);
		}
	}
	endResetModel();
//	reset();
}







