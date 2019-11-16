#ifndef WINDOWPROCESSLIST_H
#define WINDOWPROCESSLIST_H
#include <QTreeWidget>
#include <windows.h>

class WindowNode;
class ProcessNode;

class WindowProcessList;
class WindowProcessListPrivate;

class WindowNode : public QTreeWidgetItem
{
public:
	WindowProcessList*	m_List;
	QPixmap				m_Icon;
	HWND				m_Handle;
public:
	WindowNode();
	~WindowNode();
public:
	QVariant data(int column, int role) const;
public:
	HWND     handle() const;
};
class ProcessNode : public QTreeWidgetItem
{
public:
	WindowProcessList*	m_List;
	QString				m_Name;
//	QString				m_Path;
	QPixmap				m_Icon;
	DWORD				m_ProcessId;
public:
	ProcessNode();
	~ProcessNode();
public:
	QVariant data(int column, int role) const;
};

class WindowProcessList
{
public:
	QTreeWidgetItem*			m_Root;
//	QList<QTreeWidgetItem*>		m_Process;
	QHash<HWND, WindowNode*>	m_Windows;

	QImage						m_ImageChild;
	QImage						m_ImageDialog;
	QImage						m_ImagePopup;
	QImage						m_ImageWindow;
public:
	WindowProcessList(QTreeWidgetItem* root);
	~WindowProcessList();
public:
	void			clear();
	void			refresh();
	WindowNode*		getWindow(HWND hwnd);
	ProcessNode*	getProcess(HWND hwnd);

public:
	WindowNode*		searchWindow(QTreeWidgetItem* item, HWND hwnd) const;
};

#endif // WINDOWPROCESSLIST_H
