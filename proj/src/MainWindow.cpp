#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "utils.h"
#include "DlgAbout.h"

#include "3rd/GetRemoteWindowInfo.h"
#include <selector/WindowSelector.h>

extern "C" BOOL GetRemoteWindowInfoExt(HWND hwnd, WNDCLASSEX *pClass, WNDPROC *pProc, TCHAR *pszText, int nTextLen, HBRUSH* phBrush);


#define SETTINGS_APPLICATION "WndSpy"
#define SETTINGS_ORGANIZATION "Germix"

void addTitledTab(QTabWidget * tabWidget, QWidget * widget)
{
	int index = tabWidget->addTab(widget, widget->windowTitle());
	QObject::connect(widget, &QWidget::windowTitleChanged,
		[index, tabWidget](const QString & text)
	{
		tabWidget->setTabText(index, text);
	});
}

QSize GetWindowFrameSize(QMainWindow* mw)
{
	DWORD style = GetWindowLong((HWND)mw->winId(), GWL_STYLE);
	DWORD exStyle = GetWindowLong((HWND)mw->winId(), GWL_EXSTYLE);
	RECT rect = {0,0,0,0};
	style &= ~(WS_OVERLAPPED); // Not permitted, see docs.
	AdjustWindowRectEx(&rect, style, FALSE, exStyle);
	const QMargins margins(qAbs(rect.left), qAbs(rect.top), qAbs(rect.right), qAbs(rect.bottom));
	// ...
	const int frameWidth = margins.left() + margins.right();
	const int frameHeight = margins.top() + margins.bottom();

	return QSize(frameWidth, frameHeight);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	setWindowFlags(
					Qt::WindowTitleHint |
					Qt::WindowSystemMenuHint |
					Qt::WindowCloseButtonHint |
					Qt::WindowMinimizeButtonHint
				   );
	QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);
	languages.init(ui->menuLanguages, "translations", "wndspy", s.value("Language").toString());

	//
	// Selector
	//
	windowSelector = new WindowSelector(this, 32, QImage(":/images/winsel1.png"), QImage(":/images/winsel2.png"));
	connect(windowSelector, SIGNAL(onStartSelection(WId)), this, SLOT(slotWindowSelector_startSelection(WId)));
	connect(windowSelector, SIGNAL(onChangeSelection(WId)), this, SLOT(slotWindowSelector_changeSelection(WId)));
	connect(windowSelector, SIGNAL(onCancelSelection(WId)), this, SLOT(slotWindowSelector_cancelSelection(WId)));
	connect(windowSelector, SIGNAL(onFinishSelection(WId)), this, SLOT(slotWindowSelector_finishSelection(WId)));

	windowSelector->setMinimumSize(windowSelector->minimumSizeHint());
	windowSelector->setMaximumSize(windowSelector->minimumSizeHint());
	ui->horizontalLayout->insertWidget(0, windowSelector, 0, Qt::AlignLeft);
//	ui->gridLayoutMain->addWidget(windowSelector, 0, 0, 1, 1, Qt::AlignLeft);
	windowSelector->show();

	// Crear etiquetas para la barra de estado
	QStatusBar* sb;

	sb = statusBar();
	m_LblHandle = new QLabel(0);
	m_LblClass = new QLabel(0);
	m_LblLabel = new QLabel(0);
	m_LblHandle->setMinimumWidth(70);
	m_LblHandle->setMaximumWidth(70);
	m_LblClass->setMinimumWidth(250);
	m_LblClass->setMaximumWidth(250);
	m_LblLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
	sb->addWidget(m_LblHandle);
	sb->addWidget(m_LblClass);
	sb->addWidget(m_LblLabel, 1);

	//
	// Crear formularios de información
	//
#ifdef USE_INFO_GENERAL
	addTitledTab(ui->tabWidget_1, m_FrmInfoGeneral = new FormInfoGeneral(this));
#endif
#ifdef USE_INFO_RECT
	addTitledTab(ui->tabWidget_1, m_FrmInfoRect = new FormInfoRect(this));
#endif
#ifdef USE_INFO_SCROLL
	addTitledTab(ui->tabWidget_1, m_FrmInfoScroll = new FormInfoScroll(this));
#endif
#ifdef USE_INFO_STYLES
	addTitledTab(ui->tabWidget_1, m_FrmInfoStyles = new FormInfoStyles(this));
#endif
#ifdef USE_INFO_WINDOWS
	addTitledTab(ui->tabWidget_1, m_FrmInfoWindows = new FormInfoWindows(this));
#endif
#ifdef USE_INFO_PROPERTIES
	addTitledTab(ui->tabWidget_1, m_FrmInfoProperties = new FormInfoProperties(this));
#endif
#ifdef USE_INFO_MENU
	addTitledTab(ui->tabWidget_1, m_FrmInfoMenu = new FormInfoMenu(this));
#endif
#ifdef USE_INFO_CLASS
	addTitledTab(ui->tabWidget_1, m_FrmInfoClass = new FormInfoClass(this));
#endif
#ifdef USE_INFO_PROCESS
	addTitledTab(ui->tabWidget_1, m_FrmInfoProcess = new FormInfoProcess(this));
#endif
	//
	// Crear formularios de información extra
	//
#ifdef USE_MOREINFO_WINDOWS
	addTitledTab(ui->tabWidget_2, m_FrmMoreInfoWindows = new FormInfo2Windows(this));
	m_FrmMoreInfoWindows->slotRefresh();
#endif
#ifdef USE_MOREINFO_PROCESS
	addTitledTab(ui->tabWidget_2, m_FrmMoreInfoProcess = new FormInfo2Process(this));
	m_FrmMoreInfoProcess->slotRefresh();
#endif
	{
		QSize s = ui->tabWidget_1->minimumSizeHint();
		ui->tabWidget_1->setMinimumSize(s);
		ui->tabWidget_1->setMaximumSize(s);
	}

	updateInfo();
	updateLabels();

	// ...
	ui->chkMinimize->setChecked(s.value("Minimize", "false").toBool());
	ui->chkHiddenWindows->setChecked(s.value("HiddenWindows", "false").toBool());
	connect(ui->chkMinimize, SIGNAL(toggled(bool)), this, SLOT(slotCheckBox_toggled(bool)));
	connect(ui->chkHiddenWindows, SIGNAL(toggled(bool)), this, SLOT(slotCheckBox_toggled(bool)));

	//
	// Restaurar estado anterior
	//
	ui->tabWidget_1->setVisible(visibleTab1 = s.value("Tab1Visible", "true").toBool());
	ui->tabWidget_2->setVisible(visibleTab2 = s.value("Tab2Visible", "true").toBool());
	restoreGeometry(s.value("WindowGeometry").toByteArray());
	restoreState(s.value("WindowState").toByteArray());
}

MainWindow::~MainWindow()
{
	QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);
	s.setValue("Language", languages.language());
	s.setValue("WindowState", saveState());
	s.setValue("WindowGeometry", saveGeometry());
    delete ui;
}
void MainWindow::closeEvent(QCloseEvent* e)
{
	Q_UNUSED(e);
	// ...
	QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);
//	s.setValue("Language", languages.language());
//	s.setValue("WindowState", saveState());
//	s.setValue("WindowGeometry", saveGeometry());
	s.setValue("Tab1Visible", ui->tabWidget_1->isVisible());
	s.setValue("Tab2Visible", ui->tabWidget_2->isVisible());
	s.setValue("Minimize", ui->chkMinimize->isChecked());
	s.setValue("HiddenWindows", ui->chkHiddenWindows->isChecked());
}
void MainWindow::changeEvent(QEvent* e)
{
	if(e != NULL)
	{
		switch(e->type())
		{
			case QEvent::LocaleChange:
#if 1
				{
					QString locale = QLocale::system().name();
					locale.truncate(locale.lastIndexOf('_'));
					languages.load(locale);
				}
#endif
				break;
			case QEvent::LanguageChange:
				ui->retranslateUi(this);
				break;
			default:
				break;
		}
	}
	QMainWindow::changeEvent(e);
}

void MainWindow::updateInfo(WId p)
{
#ifdef USE_INFO_GENERAL
	m_FrmInfoGeneral->setupInfo((HWND)p, &windowInfo);
#endif
#ifdef USE_INFO_RECT
	m_FrmInfoRect->setupInfo((HWND)p, &windowInfo);
#endif
#ifdef USE_INFO_SCROLL
	m_FrmInfoScroll->setupInfo((HWND)p, &windowInfo);
#endif
#ifdef USE_INFO_STYLES
	m_FrmInfoStyles->setupInfo((HWND)p, &windowInfo);
#endif
#ifdef USE_INFO_WINDOWS
	m_FrmInfoWindows->setupInfo((HWND)p, &windowInfo);
#endif
#ifdef USE_INFO_PROPERTIES
	m_FrmInfoProperties->setupInfo((HWND)p, &windowInfo);
#endif
#ifdef USE_INFO_MENU
	m_FrmInfoMenu->setupInfo((HWND)p, &windowInfo);
#endif
#ifdef USE_INFO_CLASS
	m_FrmInfoClass->setupInfo((HWND)p, &windowInfo);
#endif
#ifdef USE_INFO_PROCESS
	m_FrmInfoProcess->setupInfo((HWND)p, &windowInfo);
#endif
}
void MainWindow::updateLabels(WId p)
{
	m_LblHandle->setText(QString().sprintf(HEX_FORMAT, (unsigned int)p));
	if(!p)
	{
		m_LblClass->setText(QString());
		m_LblLabel->setText(QString());
	}
	else
	{
		HWND hwnd = (HWND)p;
		WCHAR buf[256];

		GetClassNameW(hwnd, buf, 256);
		m_LblClass->setText(QString::fromUtf16((const ushort*)buf));

		GetWindowTextW(hwnd, buf, 256);
		m_LblLabel->setText(QString::fromUtf16((const ushort*)buf));
	}
}void MainWindow::restoreApp()
{
	restoreGeometry(previousGeometry);
	if(visibleTab1)
		ui->tabWidget_1->setVisible(true);
	if(visibleTab2)
		ui->tabWidget_2->setVisible(true);
}
void MainWindow::minimizeApp()
{
	QSize winFrameSize = GetWindowFrameSize(this);

	int tabRight = ui->tabWidget_1->frameGeometry().width() + 11 + 11;

	int minx = tabRight;
	int miny =
				11
				+ menuBar()->height()
				+ ui->widget->minimumSizeHint().height()
				+ 11
				+ ui->statusBar->height()
				;

	visibleTab1 = ui->tabWidget_1->isVisible();
	visibleTab2 = ui->tabWidget_2->isVisible();

	ui->tabWidget_1->setVisible(false);
	ui->tabWidget_2->setVisible(false);
	resize(minx, miny);

	SetWindowPos(
				(HWND)winId(),
				NULL,
				0,
				0,
				minx + winFrameSize.width(),
				miny + winFrameSize.height(),
				SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}
void MainWindow::slotExit()
{
	close();
}
void MainWindow::slotTopMost(bool Checked)
{
	QPoint p = pos();
	Qt::WindowFlags f = windowFlags();
	if(Checked)
		f |= Qt::WindowStaysOnTopHint;
	else
		f &= ~Qt::WindowStaysOnTopHint;

	setParent(parentWidget(), f);
	move(p);
	show();
}
void MainWindow::slotRefresh()
{
	HWND hwnd;
	if(NULL != (hwnd = GetCurrentWindow()))
	{
		updateInfo((WId)hwnd);
	}
}
void MainWindow::slotFlash()
{
	HWND hwnd;
	if(NULL != (hwnd = GetCurrentWindow()))
	{
		CommandHWnd(CMDHWND_FLASH, hwnd);
	}
}
void MainWindow::slotCaptureClient()
{
	HWND hwnd;
	if(NULL != (hwnd = GetCurrentWindow()))
	{
		CommandHWnd(CMDHWND_CAPTURECLIENT, hwnd);
	}
}
void MainWindow::slotCaptureWindow()
{
	HWND hwnd;
	if(NULL != (hwnd = GetCurrentWindow()))
	{
		CommandHWnd(CMDHWND_CAPTUREWINDOW, hwnd);
	}
}
void MainWindow::slotAbout()
{
	DlgAbout(this).exec();
}
void MainWindow::slotCheckBox_toggled(bool checked)
{
	QCheckBox* chk = qobject_cast<QCheckBox*>(sender());

	if(chk == ui->chkMinimize)
	{
	}
	else if(chk == ui->chkHiddenWindows)
	{
		windowSelector->setHiddenWindows(checked);
	}
}
void MainWindow::slotWindowSelector_startSelection(WId p)
{
	updateLabels(p);

	previousGeometry = saveGeometry();
	if(ui->chkMinimize->isChecked())
	{
		minimizeApp();
	}
	setCursor(QCursor(Qt::SizeAllCursor));
}
void MainWindow::slotWindowSelector_changeSelection(WId p)
{
	updateLabels(p);
}
void MainWindow::slotWindowSelector_cancelSelection(WId p)
{
	updateLabels(p);

	restoreApp();
	setCursor(QCursor(Qt::ArrowCursor));
}
void MainWindow::slotWindowSelector_finishSelection(WId p)
{
	windowInfo.selectWindow((HWND)p);
	updateInfo(p);
	updateLabels(p);

	restoreApp();
	setCursor(QCursor(Qt::ArrowCursor));
}
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	if(eventType == "windows_generic_MSG")
	{
		MSG * msg = static_cast<MSG*>(message);
		POINT pt;
		RECT wrc;

		GetCursorPos(&pt);
		GetWindowRect(msg->hwnd, &wrc);

		if(msg->message == WM_SIZING)
		{
			RECT* prc = (RECT*)msg->lParam;
			UINT nSize = msg->wParam;
			QSize winFrameSize = GetWindowFrameSize(this);

			int tabRight = ui->tabWidget_1->frameGeometry().width() + 11 + 11;
//			int tabRight = ui->tabWidget_1->minimumSizeHint().width() + 11 + 11;

			int minx = tabRight + winFrameSize.width();
			int miny =
						winFrameSize.height()
						+ 11
						+ menuBar()->height()
						+ ui->widget->minimumSizeHint().height()
						+ 11
						+ ui->statusBar->height()
						;

			int maxy = winFrameSize.height()
						+ menuBar()->height()
						+ 11
						+ ui->widget->minimumSizeHint().height()
						+ ui->tabWidget_1->minimumSizeHint().height()
						+ 11
						+ ui->statusBar->height()
						;

			int newWidth   = prc->right - prc->left;
			int newHeight  = prc->bottom - prc->top;
			bool visible1 = true;
			bool visible2 = true;

			if(newWidth < (minx + ui->tabWidget_2->minimumSizeHint().width()))
			{
				newWidth = minx;
				visible2 = false;
			}
			if(newHeight > miny)
			{
				newHeight = maxy;
			}
			if(newHeight <= miny)
			{
				newHeight = miny;
				newWidth  = minx;
				visible1 = false;
				visible2 = false;
			}
			ui->tabWidget_1->setVisible(visible1);
			ui->tabWidget_2->setVisible(visible2);

			switch(nSize)
			{
				case WMSZ_LEFT:
					prc->left   = prc->right  - newWidth;
					break;
				case WMSZ_RIGHT:
					prc->right  = prc->left   + newWidth;
					break;

				case WMSZ_TOP:
					prc->top    = prc->bottom - newHeight;
					prc->right  = prc->left   + newWidth;
					break;
				case WMSZ_TOPLEFT:
					prc->left   = prc->right  - newWidth;
					prc->top    = prc->bottom - newHeight;
					break;
				case WMSZ_TOPRIGHT:
					prc->top    = prc->bottom - newHeight;
					prc->right  = prc->left   + newWidth;
					break;

				case WMSZ_BOTTOM:
					prc->right  = prc->left   + newWidth;
					prc->bottom = prc->top    + newHeight;
					break;
				case WMSZ_BOTTOMLEFT:
					prc->left   = prc->right  - newWidth;
					prc->bottom = prc->top    + newHeight;
					break;
				case WMSZ_BOTTOMRIGHT:
					prc->bottom = prc->top    + newHeight;
					prc->right  = prc->left   + newWidth;
					break;
			}
			return true;
		}
		else if (msg->message == WM_ENTERSIZEMOVE)
		{
			return true;
		}
		else if(msg->message == WM_EXITSIZEMOVE)
		{
			return true;
		}
		else if(msg->message == WM_GETMINMAXINFO)
		{
			return true;
		}
	}
	return QMainWindow::nativeEvent(eventType, message, result);
}
