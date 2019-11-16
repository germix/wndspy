#ifndef FORMINFOCLASS_H
#define FORMINFOCLASS_H

#include <QWidget>
#include "utils.h"

namespace Ui {
	class FormInfoClass;
}


class QWinIconLabel;
class QWinCursorLabel;
class QWinBrushLabel;

class FormInfoClass : public QWidget
{
    Q_OBJECT
	Ui::FormInfoClass*	ui;
	QWinIconLabel*		m_LblIcon1;
	QWinIconLabel*		m_LblIcon2;
	QWinCursorLabel*	m_LblCursor;
	QWinBrushLabel*		m_LblBrush;
public:
	FormInfoClass(QWidget* parent = 0);
	~FormInfoClass();
public:
	void setupInfo(HWND hwnd, class WindowInfo* info);
private:
	void changeEvent(QEvent* e);
};

#endif // FORMINFOCLASS_H
