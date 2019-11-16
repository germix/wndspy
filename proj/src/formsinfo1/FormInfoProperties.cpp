#include "FormInfoProperties.h"
#include "ui_FormInfoProperties.h"
#include <WindowInfo.h>

FormInfoProperties::FormInfoProperties(QWidget* parent) : QWidget(parent), ui(new Ui::FormInfoProperties)
{
    ui->setupUi(this);

	modelProperties = new PropertiesItemModel(this);
	ui->treeview->setModel(modelProperties);
}

FormInfoProperties::~FormInfoProperties()
{
    delete ui;
}
void FormInfoProperties::setupInfo(HWND hwnd, class WindowInfo* info)
{
	Q_UNUSED(info);
	modelProperties->setHandle(hwnd);
}
void FormInfoProperties::changeEvent(QEvent* e)
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

