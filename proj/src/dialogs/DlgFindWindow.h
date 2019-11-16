#ifndef DLGFINDWINDOW_H
#define DLGFINDWINDOW_H

#include <QDialog>

namespace Ui {
    class DlgFindWindow;
}

class DlgFindWindow : public QDialog
{
	Q_OBJECT
	Ui::DlgFindWindow *ui;
public:
	DlgFindWindow(QWidget* parent = 0);
	~DlgFindWindow();
public:
	QString			getClassName() const;
};

#endif // DLGFINDWINDOW_H
