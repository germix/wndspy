#include "DlgFindWindow.h"
#include "ui_DlgFindWindow.h"

DlgFindWindow::DlgFindWindow(QWidget* parent) : QDialog(parent), ui(new Ui::DlgFindWindow)
{
    ui->setupUi(this);
}

DlgFindWindow::~DlgFindWindow()
{
    delete ui;
}
QString			DlgFindWindow::getClassName() const
{
	return ui->txtClassName->text();
}

