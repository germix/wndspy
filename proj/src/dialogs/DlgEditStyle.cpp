#include "DlgEditStyle.h"
#include "ui_DlgEditStyle.h"

#include "utils.h"
#include "StylesItemModel.h"

enum STYLETYPE
{
	STYLETYPE_STANDARD,
	STYLETYPE_EXTENDED,
	STYLETYPE_EXTENDED_CONTROL,
};

class StyleNode : public QTreeWidgetItem
{
	DWORD			m_Style;
	StyleLookupEx*	m_Lookup;
	bool			m_Activated;
public:
	StyleNode(DWORD Style, bool Activated)
	{
		m_Lookup = NULL;
		m_Style = Style;
		m_Activated = Activated;
	}
	StyleNode(StyleLookupEx* Lookup, bool Activated)
	{
		m_Lookup = Lookup;
		m_Style = (Lookup ? Lookup->style : 0);
		m_Activated = Activated;
	}
	~StyleNode()
	{
	}
public:
	DWORD		style() const
	{
		return m_Style;
	}
	bool		isActivated() const
	{
		return m_Activated;
	}
	void		setActivated(bool activated)
	{
		m_Activated = activated;
	}
public:
	QVariant data(int column, int role) const
	{
		if(role == Qt::DisplayRole)
		{
			if(column == 0)
			{
				return QString().sprintf(HEX_FORMAT, (uint)m_Style);
			}
			else if(column == 1)
			{
				if(m_Lookup)
					return QString::fromWCharArray(m_Lookup->name);
				return QString().sprintf(HEX_FORMAT, (uint)m_Style);
			}
		}
		return QVariant();
	}
};

extern StyleLookupEx WindowStyles[];
extern ClassStyleLookup StandardControls[];
extern ClassStyleLookup CustomControls[];
extern StyleLookupEx StyleExList[];
extern ClassStyleLookup ExtendedControls[];

StyleLookupEx *FindStyleList(ClassStyleLookup *pClassList, const WCHAR *szClassName, DWORD* pdwGetData, DWORD* pdwSetData);

DWORD		EnumStyles(StyleLookupEx *StyleList, DWORD dwStyle, BOOL fAllStyles, QList<QTreeWidgetItem*>& List)
{
	// Remember what the style is before we start modifying it
	DWORD dwOrig = dwStyle;

	int            i;
	BOOL           fAddIt;
	StyleLookupEx *pStyle;

	//
	//	Loop through all of the styles that we know about
	//	Check each style against our window's one, to see
	//  if it is set or not
	//
	for(i = 0; StyleList[i].style != -1; i++)
	{
		fAddIt = FALSE;

		pStyle = &StyleList[i];

		// This style needs a mask to detect if it is set -
		// i.e. the style doesn't just use one bit to decide if it is on/off.
		if(pStyle->cmp_mask != 0)
		{
			//if((StyleList[i].depends & dwStyle) != dwStyle)
			//	continue;

			// Style is valid if the excludes styles are not set
			if(pStyle->excludes != 0 && (pStyle->excludes & (dwOrig & pStyle->cmp_mask)) == 0)
				fAddIt = TRUE;

			// If the style matches exactly (when masked)
			if(pStyle->style != 0 && (pStyle->cmp_mask & dwStyle) == pStyle->style)
				fAddIt = TRUE;
		}
		else
		{
			// Style is valid when
			if(pStyle->excludes != 0 && (pStyle->excludes & dwOrig) == 0)
				fAddIt = TRUE;

			// If style matches exactly (all bits are set
			if(pStyle->style != 0 && (pStyle->style & dwStyle) == pStyle->style)
				fAddIt = TRUE;

			// If no bits are set, then we have to skip it
			else if(pStyle->style != 0 && (pStyle->style & dwStyle) == 0)
				fAddIt = FALSE;

			// If this style depends on others being set..
			if(dwStyle &&  (pStyle->depends & dwStyle) == 0)
				fAddIt = FALSE;
		}

		// Now add the style..
		if(fAddIt == TRUE || fAllStyles)
		{
			// We've added this style, so remove it to stop it appearing again
			if(fAddIt)
				dwStyle &= ~ (pStyle->style);

			List.append(new StyleNode(pStyle, (fAllStyles ? fAddIt : false)));
		}
	}
	// return the style. This will be zero if we decoded all the bits
	// that were set, or non-zero if there are still bits left
	return dwStyle;
}
DWORD		GetStyleList(HWND hwnd, int Mode, QList<QTreeWidgetItem*>& List, bool fAllStyles, DWORD* pSetMessage)
{
	WCHAR	szClassName[256];
	DWORD	dwStyle;
//	BOOL	fAllStyles;
	BOOL	fExtControl;
	DWORD	dwGetMessage;
	DWORD	dwSetMessage;
	DWORD	dwOriginalStyle;

	StyleLookupEx *StyleList;

//	fAllStyles = false;
	fExtControl = false;

	// window class
	GetClassNameW(hwnd, szClassName, sizeof(szClassName) / sizeof(WCHAR));

	dwOriginalStyle = 0;
	if(Mode == STYLETYPE_STANDARD)
	{
		dwStyle = GetWindowLong(hwnd, GWL_STYLE);
		dwOriginalStyle = dwStyle;
		if(dwStyle)
		{
			// Enumerar los estilos de ventana estándar,
			// de cualquier ventana, sin importar que clase de ventana sea
			dwStyle = EnumStyles(WindowStyles, dwStyle, fAllStyles, List);

			// Si la clase de ventana es conocida,
			// Enumerar los estilos de control estándar
			StyleList = FindStyleList(StandardControls, szClassName, 0, 0);
			if(StyleList != 0)
				dwStyle = EnumStyles(StyleList, dwStyle, fAllStyles, List);

			// Soporte para estilos CCS_xxx (custom control styles)
			StyleList = FindStyleList(CustomControls, szClassName, 0, 0);
			if(StyleList != 0)
				dwStyle = EnumStyles(StyleList, dwStyle, fAllStyles, List);

			if(dwStyle != 0)
			{
				List.append(new StyleNode(dwStyle, (fAllStyles ? true : false)));
			}
		}
	}
	else if(Mode == STYLETYPE_EXTENDED)
	{
		// Find extended styles
		dwStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
		dwOriginalStyle = dwStyle;
		if(dwStyle)
		{
			EnumStyles(StyleExList, dwStyle, fAllStyles, List);

			// Does this window use any custom control extended styles???
			// If it does, then dwMessage will contain the message identifier to send
			// to the window to retrieve them
			if(fExtControl)
			{
				StyleList = FindStyleList(ExtendedControls, szClassName, &dwGetMessage, &dwSetMessage);

				// Add them if required
				if(StyleList != 0)
				{
					dwStyle = SendMessage(hwnd, dwGetMessage, 0, 0);
					EnumStyles(StyleList, dwStyle, fAllStyles, List);
				}
			}
		}
	}
	else if(Mode == STYLETYPE_EXTENDED_CONTROL)
	{
		StyleList = FindStyleList(ExtendedControls, szClassName, &dwGetMessage, &dwSetMessage);
		if(pSetMessage)
			*pSetMessage = dwSetMessage;

		// Add them if required
		if(StyleList != 0)
		{
			dwStyle = SendMessage(hwnd, dwGetMessage, 0, 0);
			dwOriginalStyle = dwStyle;
			EnumStyles(StyleList, dwStyle, fAllStyles, List);
		}

	}
	return dwOriginalStyle;
}
DlgEditStyle::DlgEditStyle(QWidget* parent, HWND hwnd, int mode) : QDialog(parent), ui(new Ui::DlgEditStyle)
{
	ui->setupUi(this);
	// ...
	QSize s = size();
	setMinimumWidth(s.width());
	setMaximumWidth(s.width());
	// ...

	ui->treeview->setHeaderHidden(true);
	ui->treeview->header()->setSectionsMovable(false);
//	ui->treeview->header()->setSectionResizeMode(0, QHeaderView::Stretch);
//	ui->treeview->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);


	m_uStyle = 0;
	m_uOrgStyle = 0;
	m_hwnd = hwnd;
	m_StyleType = mode;
	m_uStyle = GetStyleList(hwnd, mode, list, true, &m_SetMessage);
	m_uOrgStyle = m_uStyle;

	ui->treeview->insertTopLevelItems(0, list);

	for(int i = 0; i < list.size(); i++)
	{
		StyleNode* node = static_cast<StyleNode*>(list.at(i));
		if(node->isActivated())
			node->setSelected(true);
		else
			node->setSelected(false);
	}
	ui->txtStyles->setText(QHexUlong(m_uStyle));
}
DlgEditStyle::~DlgEditStyle()
{
	delete ui;
}
void		DlgEditStyle::slotApply()
{
	switch(m_StyleType)
	{
		case STYLETYPE_STANDARD:
			SetWindowLong(m_hwnd, GWL_STYLE, m_uStyle);
			break;
		case STYLETYPE_EXTENDED:
			SetWindowLong(m_hwnd, GWL_EXSTYLE, m_uStyle);
			break;
		case STYLETYPE_EXTENDED_CONTROL:
			SendMessage(m_hwnd, m_SetMessage, 0, m_uStyle);
			break;
	}
	SetWindowPos(m_hwnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_DRAWFRAME);
	InvalidateRect(m_hwnd, NULL, TRUE);
}
void		DlgEditStyle::slotAccept()
{
	slotApply();
	accept();
}
void		DlgEditStyle::slotCancel()
{
	reject();
}
void		DlgEditStyle::slotTreeWidget_itemSelectionChanged()
{
	StyleNode* node;

	if(NULL != (node = dynamic_cast<StyleNode*>(ui->treeview->currentItem())))
	{
		if(node->isSelected())
		{
			m_uStyle |= node->style();
			node->setActivated(true);
		}
		else
		{
			m_uStyle &= ~node->style();
			node->setActivated(false);
		}
	}
	ui->txtStyles->setText(QHexUlong(m_uStyle));
}




