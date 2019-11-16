#ifndef DLGEDITSIZE_H
#define DLGEDITSIZE_H

#include <QDialog>
#include <windows.h>

namespace Ui {
	class DlgEditSize;
}

class DlgEditSize : public QDialog
{
    Q_OBJECT
	Ui::DlgEditSize*	ui;
	HWND				hwnd;
	RECT				originalRect;
public:
	DlgEditSize(QWidget* parent, HWND hwnd);
	~DlgEditSize();
private:
public slots:
	void		slotApply();
	void		slotReset();
public:
	bool		eventFilter(QObject *, QEvent *);
};

#endif // DLGEDITSIZE_H
