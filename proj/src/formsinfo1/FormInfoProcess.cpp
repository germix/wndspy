#include "FormInfoProcess.h"
#include "ui_FormInfoProcess.h"

#include <QMenu>
#include <QMessageBox>

#include <WindowInfo.h>

extern "C"
{
BOOL GetProcessNameByPid(DWORD dwProcessId, WCHAR szName[], DWORD nNameSize, WCHAR szPath[], DWORD nPathSize);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

FormInfoProcess::FormInfoProcess(QWidget* parent) : QWidget(parent), ui(new Ui::FormInfoProcess)
{
    ui->setupUi(this);
}

FormInfoProcess::~FormInfoProcess()
{
    delete ui;
}
void FormInfoProcess::changeEvent(QEvent* e)
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
void FormInfoProcess::setupInfo(HWND hwnd, class WindowInfo* info)
{
	Q_UNUSED(info);
	// ...
	if(!hwnd)
	{
		ui->btnCommands->setEnabled(false);
		ui->txtModuleName->clear();
		ui->txtModulePath->clear();
		ui->txtThreadId->clear();
		ui->txtProcessId->clear();
	}
	else
	{
		QString s;
		DWORD dwThreadId;
		DWORD dwProcessId;

		// Abilitar botón
		ui->btnCommands->setEnabled(true);

		dwThreadId = GetWindowThreadProcessId(hwnd, &dwProcessId);

		// Id del proceso
		s.sprintf("%08X (%u)", (uint)dwProcessId, (uint)dwProcessId);
		ui->txtProcessId->setText(s);

		// Id del hilo
		s.sprintf("%08X (%u)", (uint)dwThreadId, (uint)dwThreadId);
		ui->txtThreadId->setText(s);

		// Nombre y Directorio del proceso
		WCHAR szName[MAX_PATH];
		WCHAR szPath[MAX_PATH];

		if(!GetProcessNameByPid(dwProcessId, szName, MAX_PATH, szPath, MAX_PATH))
		{
			ui->txtModuleName->setText(tr("N/A"));
			ui->txtModulePath->setText(tr("N/A"));
		}
		else
		{
			ui->txtModuleName->setText(QString::fromWCharArray(szName));
			ui->txtModulePath->setText(QString::fromWCharArray(szPath));
		}
	}
}
void FormInfoProcess::slotCommands()
{
	HWND hwnd;
	if(NULL != (hwnd = GetCurrentWindow()))
	{
		if(IsWindow(hwnd))		// Comprobar se es una ventana válida
		{
			QMenu* Menu;

			Menu = new QMenu(this);
			Menu->addAction(tr("Close process (safe)"), this, SLOT(slotCloseProcess()));
			Menu->addAction(tr("Terminate process"), this, SLOT(slotTerminateProcess()));
			Menu->addSeparator();
			Menu->addAction(tr("Duplicate process"), this, SLOT(slotDuplicateProcess()));
			Menu->addSeparator();
			Menu->addAction(tr("Open container folder"), this, SLOT(slotOpenExecutableFolder()));

			Menu->exec(QCursor::pos());

			delete Menu;
		}
	}
}
void FormInfoProcess::slotCloseProcess()
{
	HWND hwnd;
	if(NULL != (hwnd = GetCurrentWindow()))
	{
		if(QMessageBox::warning(this, tr("Warning"), tr("Are you sure you want to close this process?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
		{
			DWORD dwThreadId;
			DWORD dwProcessId;

			dwThreadId = GetWindowThreadProcessId(hwnd, &dwProcessId);
			PostThreadMessage(dwThreadId, WM_QUIT, 0, 0);
		}
	}
}
void FormInfoProcess::slotTerminateProcess()
{
	HWND hwnd;
	if(NULL != (hwnd = GetCurrentWindow()))
	{
		if(QMessageBox::warning(this, tr("Warning"), tr("WARNING: Terminating a process can cause undesired results including loss of data and system instability. The process will not be given the chance to save its state or data before it is terminated.\nAre you sure you want to terminate the process?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
		{
			HANDLE hProcess;
			DWORD  dwThreadId;
			DWORD  dwProcessId;

			dwThreadId = GetWindowThreadProcessId(hwnd, &dwProcessId);

			if(NULL == (hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessId)))
			{
				QMessageBox::information(this, tr("Information"), tr("The process id is invalid"), QMessageBox::Ok);
			}
			else
			{
				TerminateProcess(hProcess, -1);
				CloseHandle(hProcess);
			}
		}
	}
}
void FormInfoProcess::slotDuplicateProcess()
{
	ShellExecuteW(0, L"open", ui->txtModulePath->text().toStdWString().data(), NULL, 0, SW_SHOW);
}
void FormInfoProcess::slotOpenExecutableFolder()
{
	QString s;

	s += "/select,";
	s += "\"";
	s += ui->txtModulePath->text();
	s += "\"";
	ShellExecuteW(0, L"open", L"explorer", s.toStdWString().data(), 0, SW_SHOW);
}







