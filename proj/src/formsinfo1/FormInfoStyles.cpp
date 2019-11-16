#include <tchar.h>
#include "FormInfoStyles.h"
#include "ui_FormInfoStyles.h"

#include "StylesItemModel.h"

#include "DlgEditStyle.h"

#include <WindowInfo.h>

///////////////////////////////////////////////////////////////////////////////////////////////////

FormInfoStyles::FormInfoStyles(QWidget* parent) :
    QWidget(parent),
	ui(new Ui::FormInfoStyles)
{
    ui->setupUi(this);

	modelStyles1 = new StylesItemModel(this, false);
	modelStyles2 = new StylesItemModel(this, false);
	modelStyles3 = new StylesItemModel(this, false);

	ui->treeView_1->setModel(modelStyles1);
	ui->treeView_2->setModel(modelStyles2);
	ui->treeView_3->setModel(modelStyles3);

	ui->treeView_1->header()->setSectionsMovable(false);
	ui->treeView_2->header()->setSectionsMovable(false);
	ui->treeView_3->header()->setSectionsMovable(false);
}

FormInfoStyles::~FormInfoStyles()
{
    delete ui;
}
void		FormInfoStyles::setupInfo(HWND hwnd, class WindowInfo* info)
{
	Q_UNUSED(info);
	// ...
	if(!hwnd)
	{
		ui->pushButton_1->setEnabled(false);
		ui->pushButton_2->setEnabled(false);
		ui->pushButton_3->setEnabled(false);
		return;
	}
	QString s;
	DWORD fStyle;
	DWORD fExStyle;
	DWORD fCtlStyle;

	// ...
	modelStyles1->setStyle(hwnd, StylesItemModel::MODE_NORMAL, false, &fStyle);
	modelStyles2->setStyle(hwnd, StylesItemModel::MODE_EXTENDED, false, &fExStyle);
	modelStyles3->setStyle(hwnd, StylesItemModel::MODE_CONTROL, false, &fCtlStyle);

	s.sprintf(HEX_FORMAT, (uint)fStyle);
	ui->lineEdit_1->setText(s);
	s.sprintf(HEX_FORMAT, (uint)fExStyle);
	ui->lineEdit_2->setText(s);
	s.sprintf(HEX_FORMAT, (uint)fCtlStyle);
	ui->lineEdit_3->setText(s);

	ui->pushButton_1->setEnabled(fStyle != 0);
	ui->pushButton_2->setEnabled(fExStyle != 0);
	ui->pushButton_3->setEnabled(fCtlStyle != 0);
}
void FormInfoStyles::slotEdit()
{
	QWidget* w = (QWidget*)sender();
	DlgEditStyle* dlg = 0;

	if(w == ui->pushButton_1)
		dlg = new DlgEditStyle(this, GetCurrentWindow(), 0);
	else if(w == ui->pushButton_2)
		dlg = new DlgEditStyle(this, GetCurrentWindow(), 1);
	else if(w == ui->pushButton_3)
		dlg = new DlgEditStyle(this, GetCurrentWindow(), 2);
	if(dlg)
	{
		dlg->exec();
		delete dlg;
	}
}
void FormInfoStyles::changeEvent(QEvent* e)
{
	if(e != NULL)
	{
		if(e->type() == QEvent::LanguageChange)
		{
			ui->retranslateUi(this);
		}
	}
	QWidget::changeEvent(e);
}






