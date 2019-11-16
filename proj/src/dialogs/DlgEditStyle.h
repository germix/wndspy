#ifndef DLGEDITSTYLE_H
#define DLGEDITSTYLE_H
#include <windows.h>
#include <QDialog>

namespace Ui {
	class DlgEditStyle;
}

class DlgEditStyle;
class QTreeWidgetItem;

class DlgEditStyle : public QDialog
{
	Q_OBJECT
	Ui::DlgEditStyle*			ui;
	QList<QTreeWidgetItem*>		list;
	HWND						m_hwnd;
	DWORD						m_uStyle;
	DWORD						m_uOrgStyle;
	int							m_StyleType;

	DWORD						m_SetMessage;
public:
	DlgEditStyle(QWidget* parent, HWND hwnd, int mode);
	~DlgEditStyle();

private:
public slots:
	void		slotApply();
	void		slotAccept();
	void		slotCancel();

	void		slotTreeWidget_itemSelectionChanged();
};

#endif // DLGEDITSTYLE_H
