#ifndef FORMINFO2WINDOWS_H
#define FORMINFO2WINDOWS_H

#include <QWidget>

namespace Ui {
	class FormInfo2Windows;
}

class FormInfo2Windows;
class WindowProcessList;

class FormInfo2Windows : public QWidget
{
    Q_OBJECT

	Ui::FormInfo2Windows*	ui;
	WindowProcessList*		m_List;
	bool					m_WrapFind;
	QString					m_ClassName;
public:
	FormInfo2Windows(QWidget *parent = 0);
	~FormInfo2Windows();
private:
public slots:
	void slotRefresh();
	void slotFindPrev();
	void slotFindNext();
	void slotFindWindow();
	void slotFindCurrent();

	void slotOpenTree();
	void slotOpenTreeAll();
	void slotCloseTree();
	void slotCloseTreeAll();

	void slotCustomContextMenu(const QPoint& point);
private:
	bool openFindWindowDialog();
};

#endif // FORMINFO2WINDOWS_H
