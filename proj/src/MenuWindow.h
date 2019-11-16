#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMenu>

class MenuWindow : public QObject
{
	Q_OBJECT
public:
	HWND	m_hwnd;
	QMenu*	m_Menu;
	QMenu*	m_MenuCapture;
public:
	MenuWindow(QWidget* widget, HWND hwnd, bool select = true);
	~MenuWindow();
public:
	void		exec();
private slots:
	void		slotActionClose();
	void		slotActionSelect();
	void		slotActionVisible();
	void		slotActionEnabled();
	void		slotActionTopMost();
	void		slotActionFlash();
	void		slotActionCaptureClient();
	void		slotActionCaptureWindow();

	void		slotActionAdjust();
	void		slotActionSendToBack();
	void		slotActionBringToFront();
};

#endif // MENUWINDOW_H
