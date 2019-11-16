#include "FormInfo2Process.h"
#include "ui_FormInfo2Process.h"
#include <windows.h>

#include <list/ProcessList.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

FormInfo2Process::FormInfo2Process(QWidget *parent) :
    QWidget(parent),
	ui(new Ui::FormInfo2Process)
{
	ui->setupUi(this);

	list = new ProcessList(ui->treewidget->invisibleRootItem());
	list->refresh();
}
FormInfo2Process::~FormInfo2Process()
{
	delete list;
	delete ui;
}
void FormInfo2Process::slotRefresh()
{
	list->refresh();
}

