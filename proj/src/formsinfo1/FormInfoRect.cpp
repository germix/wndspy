#include "FormInfoRect.h"
#include "ui_FormInfoRect.h"
#include "utils.h"

#include <WindowInfo.h>

QString RectFormat(const RECT& rc)
{
	return QString().sprintf("(%d, %d)-(%d, %d) %dx%d",
							 (int)rc.left,
							 (int)rc.top,
							 (int)rc.right,
							 (int)rc.bottom,
							 (int)(rc.right - rc.left),
							 (int)(rc.bottom - rc.top));
}

FormInfoRect::FormInfoRect(QWidget *parent) :
    QWidget(parent),
	ui(new Ui::FormInfoRect)
{
    ui->setupUi(this);

	updateStatusText();
	QChangePaletteLineEdit_IsReadOnly(this, ui->txtClientRect);
}

FormInfoRect::~FormInfoRect()
{
    delete ui;
}
void FormInfoRect::setupInfo(HWND hwnd, class WindowInfo *info)
{
	// Si el handle es nulo
	if(!hwnd)
	{
		for(int i = 0; i < children().size(); i++)
		{
			QWidget* w = (QWidget*)children().at(i);
			QLineEdit* edit = qobject_cast<QLineEdit*>(w);

			if(edit) edit->clear();
		}
		ui->btnAdjust->setEnabled(false);
		return;
	}
	RECT rc;
	WINDOWPLACEMENT wp;

	// Area de cliente
	GetClientRect(hwnd, &rc);
	ui->txtClientRect->setText(RectFormat(rc));

	// Area de ventana
	GetWindowRect(hwnd, &rc);
	ui->txtWindowRect->setText(RectFormat(rc));

	// Area de restauración
	GetWindowPlacement(hwnd, &wp);
	ui->txtRestoredRect->setText(RectFormat(wp.rcNormalPosition));

	// Estado del rectángulo
	{
		int idx;
		if(IsIconic(hwnd))
			idx = 0;
		else if(IsZoomed(hwnd))
			idx = 1;
		else
			idx = 2;

		ui->txtRectState->setText(QString(status[idx]));
	}
	ui->btnAdjust->setEnabled(true);
}
void FormInfoRect::slotAdjust()
{
	HWND hwnd;
	if(NULL != (hwnd = GetCurrentWindow()))
	{
		ShowEditWindowSize(0, hwnd);
		slotRefresh();
	}
}
void FormInfoRect::slotRefresh()
{
	HWND hwnd;
	if(NULL != (hwnd = GetCurrentWindow()))
	{
		setupInfo(hwnd, NULL);
	}
}
void FormInfoRect::updateStatusText()
{
	status[0] = tr("Minimized");
	status[1] = tr("Maximized");
	status[2] = tr("Normal");
}
void FormInfoRect::changeEvent(QEvent* e)
{
	if(e != NULL)
	{
		if(e->type() == QEvent::LanguageChange)
		{
			updateStatusText();
			ui->retranslateUi(this);
		}
	}
	QWidget::changeEvent(e);
}




