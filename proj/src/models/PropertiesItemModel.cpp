#include "PropertiesItemModel.h"
#include "utils.h"

#define MAX_COLUMNS		2

///////////////////////////////////////////////////////////////////////////////////////////////////

struct ENUMPROPINFO
{
	HWND	hWnd;
	QList<PropertiesItemModel::DATA>*	pList;
};

BOOL CALLBACK EnumProcPropEx(HWND hWnd, LPWSTR pszString, HANDLE hData, DWORD dwUserData)
{
	Q_UNUSED(hWnd);
	ENUMPROPINFO* pInfo;
	if(NULL != (pInfo = (ENUMPROPINFO*)dwUserData))
	{
		PropertiesItemModel::DATA d;

		d.handle = hData;
		if(((DWORD)pszString & 0xFFFF0000) == 0)
		{
			d.text.sprintf("%08X (Atom)", (unsigned int)pszString);
		}
		else
		{
			d.text = QString::fromWCharArray(pszString);
		}
		pInfo->pList->append(d);
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

PropertiesItemModel::PropertiesItemModel(QObject *parent) : QAbstractItemModel(parent)
{
}
PropertiesItemModel::~PropertiesItemModel()
{
}
QModelIndex		PropertiesItemModel::index(int row, int column, const QModelIndex &parent) const
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
QModelIndex		PropertiesItemModel::parent(const QModelIndex &child) const
{
	Q_UNUSED(child);
	return QModelIndex();
}
int				PropertiesItemModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return list.size();
}
int				PropertiesItemModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return MAX_COLUMNS;
}
bool			PropertiesItemModel::hasChildren(const QModelIndex &parent) const
{
	if(!parent.isValid())
		return true;
	return false;
}
QVariant		PropertiesItemModel::data(const QModelIndex &index, int role) const
{
	if(index.isValid() && (index.row() < list.size()))
	{
		if(role == Qt::DisplayRole)
		{
			QString s;
			const PropertiesItemModel::DATA& data = list.at(index.row());

			switch(index.column())
			{
				case 0:
					return QHexHandle(data.handle);
				case 1:
					return data.text;
			}
		}
	}
	return QVariant();
}
QVariant		PropertiesItemModel::headerData(int section, Qt::Orientation orientation, int role) const
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
			return QString(tr("Name of property"));
		}
	}
	return QVariant();
}
Qt::ItemFlags	PropertiesItemModel::flags(const QModelIndex &index) const
{
	if(!index.isValid())
		return Qt::ItemIsEnabled;
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
void			PropertiesItemModel::setHandle(void* p)
{
	beginResetModel();
	list.clear();
	if(p)
	{
		ENUMPROPINFO Info;

		Info.hWnd = (HWND)p;
		Info.pList = &list;
		EnumPropsEx((HWND)p, (PROPENUMPROCEX)EnumProcPropEx, (LPARAM)&Info);

	}
	endResetModel();
}









