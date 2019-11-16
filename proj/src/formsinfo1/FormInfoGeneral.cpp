#include "FormInfoGeneral.h"
#include "ui_FormInfoGeneral.h"
#include <windows.h>
#include "utils.h"
#include "MenuWindow.h"

#include <QKeyEvent>

#include <WindowInfo.h>

FormInfoGeneral::FormInfoGeneral(QWidget* parent) : QWidget(parent), ui(new Ui::FormInfoGeneral)
{
    ui->setupUi(this);

	QChangePaletteLineEdit_IsReadOnly(this, ui->txtHandle);
	QChangePaletteLineEdit_IsReadOnly(ui->groupBoxFont, ui->txtHandle);

	ui->cboWindowText->installEventFilter(this);
}

FormInfoGeneral::~FormInfoGeneral()
{
    delete ui;
}
void FormInfoGeneral::setupInfo(HWND hwnd, class WindowInfo* info)
{
	// Si el handle es nulo
	if(!hwnd)
	{
		for(int i = 0; i < children().size(); i++)
		{
			QWidget* w = (QWidget*)children().at(i);
			QLineEdit* edit = qobject_cast<QLineEdit*>(w);
			QComboBox* combo = qobject_cast<QComboBox*>(w);

			if(edit) edit->clear();
			else if(combo) combo->clear();
		}
//		ui->cboWindowText->setEnabled(false);
		ui->cboWindowBytes->setEnabled(false);
		ui->btnHandle->setEnabled(false);
	}
	// Si el handle en válido
	else
	{
		QString s;
		DWORD	dw;
		WCHAR	buf[MAX_BUFFER];

		// Manipulador de ventana
		s = QString().sprintf(HEX_FORMAT, (uint)hwnd);
		ui->txtHandle->setText(s);
		ui->btnHandle->setEnabled(true);

		// Texto de ventana
		SendMessageTimeout(hwnd, WM_GETTEXT, MAX_BUFFER, (LPARAM)buf, SMTO_ABORTIFHUNG, 100, &dw);
		ui->cboWindowText->clear();
		ui->cboWindowText->addItem(QString::fromWCharArray(buf));
		ui->cboWindowText->setCurrentIndex(0);

		// Procedimiento de ventana
		if(info->pfnWindowProc == 0)
			s = QString("N/A");
		else
			s = QString().sprintf(HEX_FORMAT, (uint)info->pfnWindowProc);
		ui->txtProcedure->setText(s);

		// Manipulador de instancia
		s = QString().sprintf(HEX_FORMAT, (uint)info->hInstance);
		ui->txtInstance->setText(s);

		// Manipulador de menú
		s = QString().sprintf(HEX_FORMAT, (uint)info->hMenu);
		ui->txtMenu->setText(s);

		// Control Id
		s = QString().sprintf(HEX_FORMAT, (uint)GetWindowLong(info->hWnd, GWL_ID));
		ui->txtControlId->setText(s);

		// User data
		s = QString().sprintf(HEX_FORMAT, (uint)GetWindowLong(info->hWnd, GWL_USERDATA));
		ui->txtUserData->setText(s);

		// Bytes extras de la ventana
		{
			int i = 0;
			int numBytes = GetClassLong(info->hWnd, GCL_CBWNDEXTRA);

			ui->cboWindowBytes->clear();
			if(numBytes == 0)
			{
				ui->cboWindowBytes->setEnabled(false);
			}
			else
			{
				ui->cboWindowBytes->setEnabled(true);
				for(i = 0; numBytes != 0; i += 4)
				{
					if(numBytes >= 4)
					{
						s = QString().sprintf("+%-8d %08X", i, (uint)GetWindowLong(info->hWnd, i));
						numBytes -= 4;
					}
					else
					{
						s = QString().sprintf("+%-8d (Unavailable)", i);
//						s = QString().sprintf("+%-8d %s", i, "(Unavailable)");
						numBytes = 0;
					}
					ui->cboWindowBytes->addItem(s);
				}
				ui->cboWindowBytes->setCurrentIndex(0);
			}
		}
		// Es unicode
		if(IsWindowUnicode(info->hWnd))
			ui->txtUnicode->setText(tr("Yes"));
		else
			ui->txtUnicode->setText(tr("No"));

		// Fuente
		QString sf;
		LOGFONTW lf;

		s.sprintf(HEX_FORMAT, (uint)info->hFont);
		if(info->hFont == NULL)
		{
			s += " (NULL)";
		}
		else
		{
			sf = AppendFormatHandleLookup(g_aLookup_Font, info->hFont);
			if(!sf.isEmpty())
				s += " (" + sf + ")";
		}

		ui->txtFontHandle->setText(s);

		if(info->hFont)
			GetObjectW(info->hFont, sizeof(LOGFONT), &lf);

		if(info->hFont == NULL)
			s.clear();
		else
			s = QString::fromWCharArray(lf.lfFaceName);
		ui->txtFontName->setText(s);

		if(info->hFont == NULL)
			s = "0";
		else
			s.sprintf("%d", (uint)lf.lfHeight);
		ui->txtFontSize->setText(s);
	}
}
void FormInfoGeneral::changeEvent(QEvent* e)
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
bool FormInfoGeneral::eventFilter(QObject* object, QEvent* event)
{
	if(object == ui->cboWindowText)
	{
		if(event->type() == QEvent::KeyPress)
		{
			QKeyEvent* e = static_cast<QKeyEvent*>(event);
			if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
			{
				HWND hwnd;
				if(NULL != (hwnd = GetCurrentWindow()))
				{
					QSetWindowText(hwnd, ui->cboWindowText->currentText());
				}
				return false;
			}
		}
	}
	return false;
}
void FormInfoGeneral::slotWindowHandle()
{
	HWND hwnd;
	if(NULL != (hwnd = GetCurrentWindow()))
	{
		if(IsWindow(hwnd))
		{
			MenuWindow(this, hwnd, false).exec();
		}
	}
}

