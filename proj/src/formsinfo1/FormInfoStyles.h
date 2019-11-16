#ifndef FORMINFOSTYLES_H
#define FORMINFOSTYLES_H

#include <QWidget>

namespace Ui {
	class FormInfoStyles;
}

class StylesItemModel;

class FormInfoStyles : public QWidget
{
    Q_OBJECT

	Ui::FormInfoStyles*	ui;
	StylesItemModel*	modelStyles1;
	StylesItemModel*	modelStyles2;
	StylesItemModel*	modelStyles3;
public:
	FormInfoStyles(QWidget* parent = 0);
	~FormInfoStyles();
public:
	void setupInfo(HWND hwnd, class WindowInfo* info);
private slots:
	void slotEdit();
private:
	void changeEvent(QEvent* e);
};

#endif // FORMINFOSTYLES_H
