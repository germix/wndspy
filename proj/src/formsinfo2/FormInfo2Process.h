#ifndef FORMINFO2PROCESS_H
#define FORMINFO2PROCESS_H

#include <QWidget>

namespace Ui {
	class FormInfo2Process;
}
class FormInfo2Process;
class ProcessList;

class FormInfo2Process : public QWidget
{
    Q_OBJECT
	Ui::FormInfo2Process*	ui;
	ProcessList*			list;
public:
	FormInfo2Process(QWidget* parent = 0);
	~FormInfo2Process();
private:
public slots:
	void slotRefresh();
};

#endif // FORMINFO2PROCESS_H
