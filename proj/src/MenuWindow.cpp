#include "MenuWindow.h"
#include "utils.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

MenuWindow::MenuWindow(QWidget* widget, HWND hwnd, bool select)
{
	QAction* action;
	DWORD uStyleEx;

	m_hwnd = hwnd;

	m_Menu = new QMenu(widget);
	m_MenuCapture = new QMenu(tr("Capture"));

	if(select)
	{
		action = m_Menu->addAction(tr("Select"), this, SLOT(slotActionSelect()));

		m_Menu->addSeparator();
	}
	action = m_Menu->addAction(tr("Visible"), this, SLOT(slotActionVisible()));
	if(::IsWindowVisible(hwnd))
	{
		action->setCheckable(true);
		action->setChecked(true);
	}
	action = m_Menu->addAction(tr("Enabled"), this, SLOT(slotActionEnabled()));
	if(::IsWindowEnabled(hwnd))
	{
		action->setCheckable(true);
		action->setChecked(true);
	}
	action = m_Menu->addAction(tr("Always visible"), this, SLOT(slotActionTopMost()));
	uStyleEx = GetWindowLong(hwnd, GWL_EXSTYLE);
	if(uStyleEx & WS_EX_TOPMOST)
	{
		action->setCheckable(true);
		action->setChecked(true);
	}
	m_Menu->addSeparator();
	m_Menu->addAction(tr("Flash"), this, SLOT(slotActionFlash()));
	m_Menu->addMenu(m_MenuCapture);
	m_MenuCapture->addAction(tr("Client area"), this, SLOT(slotActionCaptureClient()));
	m_MenuCapture->addAction(tr("Window area"), this, SLOT(slotActionCaptureWindow()));

	m_Menu->addSeparator();

	m_Menu->addAction(tr("Adjust"), this, SLOT(slotActionAdjust()));
	m_Menu->addAction(tr("Send to back"), this, SLOT(slotActionSendToBack()));
	m_Menu->addAction(tr("Bring to front"), this, SLOT(slotActionBringToFront()));

	m_Menu->addSeparator();

	m_Menu->addAction(tr("Close"), this, SLOT(slotActionClose()));
}
MenuWindow::~MenuWindow()
{
	delete m_Menu;
	delete m_MenuCapture;
}
void		MenuWindow::exec()
{
	m_Menu->exec(QCursor::pos());
}
void		MenuWindow::slotActionClose()
{
	CommandHWnd(CMDHWND_CLOSE, m_hwnd);
}
void		MenuWindow::slotActionSelect()
{
	CommandHWnd(CMDHWND_SELECT, m_hwnd);
}
void		MenuWindow::slotActionVisible()
{
	CommandHWnd(CMDHWND_VISIBLE, m_hwnd);
}
void		MenuWindow::slotActionEnabled()
{
	CommandHWnd(CMDHWND_ENABLED, m_hwnd);
}
void		MenuWindow::slotActionTopMost()
{
	CommandHWnd(CMDHWND_TOPMOST, m_hwnd);
}
void		MenuWindow::slotActionFlash()
{
	CommandHWnd(CMDHWND_FLASH, m_hwnd);
}
void		MenuWindow::slotActionCaptureClient()
{
	CommandHWnd(CMDHWND_CAPTURECLIENT, m_hwnd);
}
void		MenuWindow::slotActionCaptureWindow()
{
	CommandHWnd(CMDHWND_CAPTUREWINDOW, m_hwnd);
}
void		MenuWindow::slotActionAdjust()
{
	CommandHWnd(CMDHWND_ADJUST, m_hwnd);
}
void		MenuWindow::slotActionSendToBack()
{
	CommandHWnd(CMDHWND_SENDTOBACK, m_hwnd);
}
void		MenuWindow::slotActionBringToFront()
{
	CommandHWnd(CMDHWND_BRINGTOFRONT, m_hwnd);
}



