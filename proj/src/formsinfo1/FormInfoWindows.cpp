#include "FormInfoWindows.h"
#include "ui_FormInfoWindows.h"
#include "LinkLabel.h"
#include "MenuWindow.h"

#include <WindowInfo.h>

FormInfoWindows::FormInfoWindows(QWidget* parent)
	:
	QWidget(parent),
	ui(new Ui::FormInfoWindows)
{
    ui->setupUi(this);

	modelChilds = new WindowsItemModel(this, WindowsItemModel::MODE_CHILDS);
	modelSiblings = new WindowsItemModel(this, WindowsItemModel::MODE_SIBLINGS);

	ui->treeviewChilds->setModel(modelChilds);
	ui->treeviewSiblings->setModel(modelSiblings);

	linkOwner = new LinkLabel(this);
	linkParent = new LinkLabel(this);
	ui->layout_link_owner->addWidget(linkOwner);
	ui->layout_link_parent->addWidget(linkParent);
	connect(linkOwner, SIGNAL(pressed(Qt::MouseButton)), this, SLOT(slotLinkPressed(Qt::MouseButton)));
	connect(linkParent, SIGNAL(pressed(Qt::MouseButton)), this, SLOT(slotLinkPressed(Qt::MouseButton)));

	// ...
	linkOwner->setContextMenuPolicy(Qt::CustomContextMenu);
	linkParent->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(linkOwner, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomContextMenu(QPoint)));
	connect(linkParent, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomContextMenu(QPoint)));

	ui->treeviewChilds->setContextMenuPolicy(Qt::CustomContextMenu);
	ui->treeviewSiblings->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->treeviewChilds, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomContextMenu(QPoint)));
	connect(ui->treeviewSiblings, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomContextMenu(QPoint)));
}

FormInfoWindows::~FormInfoWindows()
{
    delete ui;
}
void FormInfoWindows::setupInfo(HWND hwnd, class WindowInfo* info)
{
	if(!hwnd)
	{
		return;
	}
	// Ventanas hijas
	modelChilds->setHandle(hwnd);

	// Ventanas hermanas
	modelSiblings->setHandle(hwnd);

	// Ventana padre
	hwnd = GetParent(info->getHandle());
	linkParent->setText(QHexHandle(hwnd));
	ui->txtParentClassname->setText(QGetClassName(hwnd));
	ui->txtParentWindowtext->setText(QGetWindowText(hwnd));

	// Ventana propietaria
	hwnd = GetWindow(info->getHandle(), GW_OWNER);
	linkOwner->setText(QHexHandle(hwnd));
	ui->txtOwnerClassname->setText(QGetClassName(hwnd));
	ui->txtOwnerWindowtext->setText(QGetWindowText(hwnd));
}
void FormInfoWindows::slotLinkPressed(Qt::MouseButton mb)
{
	if(mb == Qt::LeftButton)
	{
		LinkLabel* label = qobject_cast<LinkLabel*>(sender());
		if(label)
		{
			bool ok;
			HWND hwnd = (HWND)label->text().toInt(&ok, 16);
			if(ok && hwnd)
			{
				SelectCurrentWindow(hwnd);
			}
		}
	}
}
void FormInfoWindows::slotLineEditReturnPressed()
{
	QLineEdit* edit = qobject_cast<QLineEdit*>(sender());

	if(edit == ui->txtOwnerWindowtext)
	{
		bool ok;
		HWND hwnd = (HWND)linkOwner->text().toInt(&ok, 16);
		if(ok && hwnd)
		{
			QSetWindowText(hwnd, ui->txtOwnerWindowtext->text());
		}
	}
	else if(edit == ui->txtParentWindowtext)
	{
		bool ok;
		HWND hwnd = (HWND)linkParent->text().toInt(&ok, 16);
		if(ok && hwnd)
		{
			QSetWindowText(hwnd, ui->txtParentWindowtext->text());
		}
	}
}
void FormInfoWindows::slotCustomContextMenu(const QPoint& point)
{
	Q_UNUSED(point);
	// ...
	HWND hwnd = 0;
	QWidget* widget = qobject_cast<QWidget*>(sender());

	if(widget == ui->treeviewChilds)
	{
		QModelIndex index = ui->treeviewChilds->currentIndex();
		if(index.isValid())
		{
			hwnd = (HWND)modelChilds->getHandle(index.row());
		}
	}
	else if(widget == ui->treeviewSiblings)
	{
		QModelIndex index = ui->treeviewSiblings->currentIndex();
		if(index.isValid())
		{
			hwnd = (HWND)modelSiblings->getHandle(index.row());
		}
	}
	else if(widget == linkOwner)
	{
		bool ok;
		hwnd = (HWND)linkOwner->text().toInt(&ok, 16);
	}
	else if(widget == linkParent)
	{
		bool ok;
		hwnd = (HWND)linkParent->text().toInt(&ok, 16);
	}
	if(hwnd && IsWindow(hwnd))
	{
		MenuWindow(this, hwnd).exec();
	}
}
#if 0	// !!!
void FormInfoWindows::slotTreeViewPressed(const QModelIndex& index)
{
	m_Buttons = QApplication::mouseButtons();
}
#endif
void FormInfoWindows::slotTreeViewDoubleClicked(const QModelIndex& index)
{
//	if(m_Buttons.testFlag(Qt::LeftButton))
	{
		HWND hwnd = 0;
		QTreeView* view = qobject_cast<QTreeView*>(sender());

		if(view == ui->treeviewChilds)
		{
			hwnd = (HWND)modelChilds->getHandle(index.row());
		}
		else if(view == ui->treeviewSiblings)
		{
			hwnd = (HWND)modelSiblings->getHandle(index.row());
		}
		if(hwnd && IsWindow(hwnd))
		{
			SelectCurrentWindow(hwnd);
		}
	}
}
void FormInfoWindows::changeEvent(QEvent* e)
{
	if(e != NULL)
	{
		if(e->type() == QEvent::LanguageChange)
		{
			ui->retranslateUi(this);
		}
	}
	QWidget::changeEvent(e);
}



