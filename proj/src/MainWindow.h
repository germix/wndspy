#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <windows.h>

#include <QMainWindow>

#include "WindowInfo.h"

#define USE_INFO_GENERAL
#define USE_INFO_RECT
#define USE_INFO_SCROLL
#define USE_INFO_STYLES
#define USE_INFO_WINDOWS
#define USE_INFO_PROPERTIES
//#define USE_INFO_MENU
#define USE_INFO_CLASS
#define USE_INFO_PROCESS

#define USE_MOREINFO_WINDOWS
#define USE_MOREINFO_PROCESS

#ifdef USE_INFO_GENERAL
#include "FormInfoGeneral.h"
#endif
#ifdef USE_INFO_RECT
#include "FormInfoRect.h"
#endif
#ifdef USE_INFO_SCROLL
#include "FormInfoScroll.h"
#endif
#ifdef USE_INFO_STYLES
#include "FormInfoStyles.h"
#endif
#ifdef USE_INFO_WINDOWS
#include "FormInfoWindows.h"
#endif
#ifdef USE_INFO_PROPERTIES
#include "FormInfoProperties.h"
#endif
#ifdef USE_INFO_MENU
#include "FormInfoMenu.h"
#endif
#ifdef USE_INFO_CLASS
#include "FormInfoClass.h"
#endif
#ifdef USE_INFO_PROCESS
#include "FormInfoProcess.h"
#endif

#ifdef USE_MOREINFO_WINDOWS
#include "FormInfo2Windows.h"
#endif
#ifdef USE_MOREINFO_PROCESS
#include "FormInfo2Process.h"
#endif

namespace Ui {
	class MainWindow;
}

#include "Languages.h"

#include "selector/WindowSelector.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
	Ui::MainWindow*			ui;

	QLabel*					m_LblHandle;
	QLabel*					m_LblClass;
	QLabel*					m_LblLabel;

	// Formularios
#ifdef USE_INFO_GENERAL
	FormInfoGeneral*		m_FrmInfoGeneral;
#endif
#ifdef USE_INFO_RECT
	FormInfoRect*			m_FrmInfoRect;
#endif
#ifdef USE_INFO_SCROLL
	FormInfoScroll*			m_FrmInfoScroll;
#endif
#ifdef USE_INFO_STYLES
	FormInfoStyles*			m_FrmInfoStyles;
#endif
#ifdef USE_INFO_WINDOWS
	FormInfoWindows*		m_FrmInfoWindows;
#endif
#ifdef USE_INFO_PROPERTIES
	FormInfoProperties*	m_FrmInfoProperties;
#endif
#ifdef USE_INFO_MENU
	FormInfoMenu*			m_FrmInfoMenu;
#endif
#ifdef USE_INFO_CLASS
	FormInfoClass*			m_FrmInfoClass;
#endif
#ifdef USE_INFO_PROCESS
	FormInfoProcess*		m_FrmInfoProcess;
#endif
#ifdef USE_MOREINFO_WINDOWS
	FormInfo2Windows*		m_FrmMoreInfoWindows;
#endif
#ifdef USE_MOREINFO_PROCESS
	FormInfo2Process*		m_FrmMoreInfoProcess;
#endif
	Languages				languages;

	WindowInfo				windowInfo;

	WindowSelector*			windowSelector;

	bool					visibleTab1;
	bool					visibleTab2;
	QByteArray				previousGeometry;
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	void closeEvent(QCloseEvent* e);
	void changeEvent(QEvent* e);
private:
	void restoreApp();
	void minimizeApp();
	void updateInfo(WId p = 0);
	void updateLabels(WId p = 0);
private slots:
	void slotExit();
	void slotTopMost(bool Checked);
	void slotRefresh();
	void slotFlash();
	void slotCaptureClient();
	void slotCaptureWindow();
	void slotAbout();

	void slotCheckBox_toggled(bool checked);

	void slotWindowSelector_startSelection(WId p);
	void slotWindowSelector_changeSelection(WId p);
	void slotWindowSelector_cancelSelection(WId p);
	void slotWindowSelector_finishSelection(WId p);
public:
	HWND getWindow() const
	{
		return windowInfo.getHandle();
	}
	void setCurrentWindow(HWND hwnd)
	{
		windowInfo.selectWindow(hwnd);

		updateInfo((WId)hwnd);
		updateLabels((WId)hwnd);
	}
	WindowSelector* getSelector() const
	{
		return windowSelector;
	}
	virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);

};

#endif // MAINWINDOW_H
