#include "DlgEditSize.h"
#include "ui_DlgEditSize.h"
#include <QtGui>

DlgEditSize::DlgEditSize(QWidget* parent, HWND ___hwnd) : QDialog(parent), ui(new Ui::DlgEditSize), hwnd(___hwnd)
{
    ui->setupUi(this);
	// ...
	QSize s = size();
	setMinimumWidth(s.width());
	setMaximumWidth(s.width());

	// ...

	RECT rc;
	DWORD uStyle;

	GetWindowRect(hwnd, &rc);
	uStyle = GetWindowLong(hwnd, GWL_STYLE);
	if(uStyle & WS_CHILD)
	{
		MapWindowPoints(NULL, GetParent(hwnd), (POINT*)&rc, 2);
	}
	originalRect = rc;
	ui->spinboxX->setValue(originalRect.left);
	ui->spinboxY->setValue(originalRect.top);
	ui->spinboxW->setValue(originalRect.right-originalRect.left);
	ui->spinboxH->setValue(originalRect.bottom-originalRect.top);

	ui->spinboxX->installEventFilter(this);
	ui->spinboxY->installEventFilter(this);
	ui->spinboxW->installEventFilter(this);
	ui->spinboxH->installEventFilter(this);
}

DlgEditSize::~DlgEditSize()
{
    delete ui;
}
void		DlgEditSize::slotApply()
{
	SetWindowPos(hwnd, 0,
				 ui->spinboxX->value(),
				 ui->spinboxY->value(),
				 ui->spinboxW->value(),
				 ui->spinboxH->value(),
				 SWP_NOACTIVATE | SWP_NOZORDER);
}
void		DlgEditSize::slotReset()
{
	ui->spinboxX->setValue(originalRect.left);
	ui->spinboxY->setValue(originalRect.top);
	ui->spinboxW->setValue(originalRect.right-originalRect.left);
	ui->spinboxH->setValue(originalRect.bottom-originalRect.top);
}
bool		DlgEditSize::eventFilter(QObject* object, QEvent* event)
{
	if(object == ui->spinboxX || object == ui->spinboxY || object == ui->spinboxW || object == ui->spinboxH)
	{
		if(event->type() == QEvent::KeyPress)
		{
			QKeyEvent* e = static_cast<QKeyEvent*>(event);
			if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
			{
				slotApply();
				return true;
			}
		}
	}
	return false;
}

void ShowEditWindowSize(QWidget* parent, HWND hwnd)
{
	DlgEditSize(parent, hwnd).exec();
}
