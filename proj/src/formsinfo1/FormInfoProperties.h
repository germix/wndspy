#ifndef FORMINFOPROPERTIES_H
#define FORMINFOPROPERTIES_H

#include <QWidget>
#include "PropertiesItemModel.h"

namespace Ui {
	class FormInfoProperties;
}

class FormInfoProperties : public QWidget
{
    Q_OBJECT
	Ui::FormInfoProperties*	ui;
	PropertiesItemModel*		modelProperties;
public:
	FormInfoProperties(QWidget* parent = 0);
	~FormInfoProperties();
public:
	void setupInfo(HWND hwnd, class WindowInfo* info);
private:
	void changeEvent(QEvent* e);
};

#endif // FORMINFOPROPERTIES_H
