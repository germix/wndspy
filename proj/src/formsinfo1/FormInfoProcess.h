#ifndef FORMINFOPROCESS_H
#define FORMINFOPROCESS_H

#include <QWidget>

namespace Ui {
	class FormInfoProcess;
}

class FormInfoProcess : public QWidget
{
    Q_OBJECT
	Ui::FormInfoProcess* ui;
public:
	explicit FormInfoProcess(QWidget* parent = 0);
	~FormInfoProcess();
public:
	void setupInfo(HWND hwnd, class WindowInfo* info);
private:
	void changeEvent(QEvent* e);
private slots:
	void slotCommands();

	void slotCloseProcess();
	void slotTerminateProcess();
	void slotDuplicateProcess();
	void slotOpenExecutableFolder();
};

#endif // FORMINFOPROCESS_H
