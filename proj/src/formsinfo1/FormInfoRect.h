#ifndef FORMINFORECT_H
#define FORMINFORECT_H

#include <QWidget>

namespace Ui {
	class FormInfoRect;
}

class FormInfoRect : public QWidget
{
    Q_OBJECT
	Ui::FormInfoRect*	ui;
	QString				status[3];
public:
	explicit FormInfoRect(QWidget* parent = 0);
	~FormInfoRect();
public:
	void setupInfo(HWND hwnd, class WindowInfo* info);
public slots:
	void slotAdjust();
	void slotRefresh();
private:
	void updateStatusText();
private:
	void changeEvent(QEvent* e);
};

#endif // FORMINFORECT_H
