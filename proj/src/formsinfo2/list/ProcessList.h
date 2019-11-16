#ifndef PROCESSLIST_H
#define PROCESSLIST_H
#include <windows.h>
#include <QPixmap>
#include <QTreeWidgetItem>


class ProcessList
{
public:
	class Node : public QTreeWidgetItem
	{
	public:
		QPixmap		m_Icon;
		QString		m_BaseName;
		QString		m_FileName;
		DWORD		m_Index;
		DWORD		m_ProcessId;
	public:
		Node()
		{
		}
		~Node()
		{
		}
	public:
		QVariant data(int column, int role) const
		{
			if(role == Qt::DisplayRole)
			{
				if(column == 0)
				{
					return QString().sprintf("%08X", m_ProcessId);
				}
				else if(column == 1)
				{
					return m_BaseName;
				}
				else if(column == 2)
				{
					return m_FileName;
				}
			}
			else if(role == Qt::DecorationRole)
			{
				if(column == 0)
					return m_Icon;
			}
			return QVariant();
		}
	};
public:
	QTreeWidgetItem*		m_Root;
	QPixmap					m_NullIcon;
public:
	ProcessList(QTreeWidgetItem* root);
	~ProcessList();
public:
	void clear();
	void refresh();
};

#endif // PROCESSLIST_H
