#ifndef FORMINFOWINDOWS_H
#define FORMINFOWINDOWS_H

#include <QtGui>
#include "utils.h"
#include "WindowsItemModel.h"


namespace Ui {
    class FormInfoWindows;
}

class LinkLabel;

class FormInfoWindows : public QWidget
{
    Q_OBJECT
public:
	FormInfoWindows(QWidget* parent = 0);
    ~FormInfoWindows();

private:
	Ui::FormInfoWindows*	ui;

	LinkLabel*				linkOwner;
	LinkLabel*				linkParent;

	WindowsItemModel*		modelChilds;
	WindowsItemModel*		modelSiblings;
public:
	void setupInfo(HWND hwnd, class WindowInfo* info);
private slots:
	void slotLinkPressed(Qt::MouseButton mb);
	void slotLineEditReturnPressed();

	void slotCustomContextMenu(const QPoint& point);

	void slotTreeViewDoubleClicked(const QModelIndex& index);
private:
	void changeEvent(QEvent* e);
};

#endif // FORMINFOWINDOWS_H
