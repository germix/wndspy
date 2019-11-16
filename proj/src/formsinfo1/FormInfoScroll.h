#ifndef FORMINFOSCROLL_H
#define FORMINFOSCROLL_H

#include <QWidget>
#include "utils.h"

namespace Ui {
	class FormInfoScroll;
}

class FormInfoScroll : public QWidget
{
    Q_OBJECT
	Ui::FormInfoScroll* ui;
public:
	explicit FormInfoScroll(QWidget* parent = 0);
	~FormInfoScroll();
public:
	void setupInfo(HWND hwnd, class WindowInfo* info);
private slots:
	void slotRefresh();
	void slotSendMessage();
private:
	void changeEvent(QEvent* e);
};

#endif // FORMINFOSCROLL_H
