#include "FormInfo2Windows.h"
#include "ui_FormInfo2Windows.h"

#include "utils.h"
#include "MenuWindow.h"
#include "DlgFindWindow.h"
#include "list/WindowProcessList.h"

#include <QMessageBox>

HWND GetCurrentWindow();


void ExpandAllQTreeWidgetItem(QTreeWidgetItem* item, bool expanded)
{
	if(item)
	{
		for(int i = 0; i < item->childCount(); i++)
		{
			ExpandAllQTreeWidgetItem(item->child(i), expanded);
		}
		item->setExpanded(expanded);
	}
}


bool compare_classname(QTreeWidgetItem* item, const QString& ClassName)
{
	WindowNode* node;

	if(NULL != (node = dynamic_cast<WindowNode*>(item)))
	{
		return (QGetClassName(node->m_Handle) == ClassName);
	}
	return false;
}
QTreeWidgetItem* get_below(QTreeWidgetItem* item)
{
	QTreeWidgetItemIterator it(item);
	it++;
	return *it;
}
QTreeWidgetItem* get_above(QTreeWidgetItem* item)
{
	QTreeWidgetItemIterator it(item);

	it--;
	return *it;
}
QTreeWidgetItem* get_last(QTreeWidgetItem* item)
{
	QTreeWidgetItem* last = item;
	QTreeWidgetItemIterator it(item);

	while(*it)
	{
		last = *it;
		it++;
	}
	return last;
}
WindowNode* find_window(QTreeWidgetItem* root, QTreeWidgetItem* start, const QString& className, bool wrap = false, bool backward = false)
{
	QTreeWidgetItem* item;
	if(!root)
		return NULL;

	if(backward)
	{
		if(!start)
			item = get_last(root->child(root->childCount()-1));
		else
			item = get_above(start);

		while(item != NULL)
		{
			if(compare_classname(item, className))
			{
				return (WindowNode*)item;
			}
			item = get_above(item);
		}
	}
	else
	{
		if(!start)
			item = root->child(0);
		else
			item = get_below(start);

		while(item != NULL)
		{
			if(compare_classname(item, className))
			{
				return (WindowNode*)item;
			}
			item = get_below(item);
		}
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////////////////////////

FormInfo2Windows::FormInfo2Windows(QWidget *parent) :
    QWidget(parent),
	ui(new Ui::FormInfo2Windows)
{
	ui->setupUi(this);
	// ...
	m_List = new WindowProcessList(ui->treeview->invisibleRootItem());
	m_WrapFind = true;
	// ...
	ui->treeview->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->treeview, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomContextMenu(QPoint)));
}

FormInfo2Windows::~FormInfo2Windows()
{
	delete m_List;
    delete ui;
}
void FormInfo2Windows::slotRefresh()
{
	m_List->refresh();
}
void FormInfo2Windows::slotFindCurrent()
{
	HWND hwnd;

	if(NULL != (hwnd = GetCurrentWindow()))
	{
		WindowNode* node = m_List->getWindow(hwnd);
		if(node)
		{
			ui->treeview->setCurrentItem(node);
		}
		else
		{
			QMessageBox::information(this, "", tr("The window could not be found. Try updating the list."), QMessageBox::Ok);
		}
	}
}
void FormInfo2Windows::slotFindPrev()
{
	if(m_ClassName.isEmpty())
	{
		if(!openFindWindowDialog())
			return;
	}
	WindowNode* node = find_window(ui->treeview->invisibleRootItem(), ui->treeview->currentItem(), m_ClassName, m_WrapFind, true);
	if(node)
	{
		ui->treeview->setCurrentItem(node);
	}
}
void FormInfo2Windows::slotFindNext()
{
	if(m_ClassName.isEmpty())
	{
		if(!openFindWindowDialog())
			return;
	}
	WindowNode* node = find_window(ui->treeview->invisibleRootItem(), ui->treeview->currentItem(), m_ClassName, m_WrapFind, false);
	if(node)
	{
		ui->treeview->setCurrentItem(node);
	}
}
void FormInfo2Windows::slotFindWindow()
{
	if(openFindWindowDialog())
	{
		WindowNode* node = find_window(ui->treeview->invisibleRootItem(), NULL, m_ClassName, m_WrapFind, false);
		if(node)
		{
			ui->treeview->setCurrentItem(node);
		}
	}
}
void FormInfo2Windows::slotOpenTree()
{
	QTreeWidgetItem* item = ui->treeview->currentItem();
	if(item)
	{
		item->setExpanded(true);
	}
}
void FormInfo2Windows::slotOpenTreeAll()
{
	QTreeWidgetItem* item = ui->treeview->invisibleRootItem();
	if(item)
	{
		ExpandAllQTreeWidgetItem(item, true);
	}
}
void FormInfo2Windows::slotCloseTree()
{
	QTreeWidgetItem* item = ui->treeview->currentItem();
	if(item)
	{
		item->setExpanded(false);
	}
}
void FormInfo2Windows::slotCloseTreeAll()
{
	QTreeWidgetItem* item = ui->treeview->invisibleRootItem();
	if(item)
	{
		ExpandAllQTreeWidgetItem(item, false);
	}
}
void FormInfo2Windows::slotCustomContextMenu(const QPoint& point)
{
	HWND hwnd;
	WindowNode* node;
	QTreeWidgetItem* item = ui->treeview->currentItem();

	if(NULL != (node = dynamic_cast<WindowNode*>(item)))
	{
		if(NULL != (hwnd = node->handle()))
		{
			MenuWindow(this, hwnd).exec();
		}
	}
}
bool FormInfo2Windows::openFindWindowDialog()
{
	bool success;
	DlgFindWindow dialog(this);

	if(dialog.exec() == DlgFindWindow::Accepted)
	{
		m_ClassName = dialog.getClassName();
		success = true;
	}
	else
	{
		success = false;
	}
	return success;
}


