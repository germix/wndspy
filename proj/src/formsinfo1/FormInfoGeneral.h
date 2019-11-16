#ifndef FORMINFOGENERAL_H
#define FORMINFOGENERAL_H

#include <QWidget>

namespace Ui {
	class FormInfoGeneral;
}

class FormInfoGeneral : public QWidget
{
    Q_OBJECT
	Ui::FormInfoGeneral* ui;
public:
	FormInfoGeneral(QWidget* parent = 0);
	~FormInfoGeneral();
public:
	void setupInfo(HWND hwnd, class WindowInfo* info);
private:
	void changeEvent(QEvent* e);
	bool eventFilter(QObject *, QEvent *);
private slots:
	void slotWindowHandle();
};

#endif // FORMINFOGENERAL_H
