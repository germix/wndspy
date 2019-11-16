#include "FormInfoScroll.h"
#include "ui_FormInfoScroll.h"

#include <WindowInfo.h>

FormInfoScroll::FormInfoScroll(QWidget *parent) :
    QWidget(parent),
	ui(new Ui::FormInfoScroll)
{
    ui->setupUi(this);

	ui->cboBar->addItem("SB_HORZ");
	ui->cboBar->addItem("SB_VERT");
//	ui->cboBar->addItem("SB_BOTH");

	ui->cboTypeOfMessage->addItem("SB_LINEUP/SB_LINELEFT");
	ui->cboTypeOfMessage->addItem("SB_LINEDOWN/SB_LINERIGHT");
	ui->cboTypeOfMessage->addItem("SB_PAGEUP/SB_PAGELEFT");
	ui->cboTypeOfMessage->addItem("SB_PAGEDOWN/SB_PAGERIGHT");
	ui->cboTypeOfMessage->addItem("SB_THUMBPOSITION");
	ui->cboTypeOfMessage->addItem("SB_THUMBTRACK");
	ui->cboTypeOfMessage->addItem("SB_TOP/SB_LEFT");
	ui->cboTypeOfMessage->addItem("SB_BOTTOM/SB_RIGHT");
	ui->cboTypeOfMessage->addItem("SB_ENDSCROLL");
}

FormInfoScroll::~FormInfoScroll()
{
    delete ui;
}
void FormInfoScroll::setupInfo(HWND hwnd, class WindowInfo* info)
{
	Q_UNUSED(info);
	// ...
	ULONG fStyle;
	ULONG nBarType;
	SCROLLINFO siInfo;
	WCHAR szClassName[MAX_BUFFER];

	// Si el handle es nulo
	if(!hwnd)
	{
#if 0	//!!!
		for(int i = 0; i < children().size(); i++)
		{
			QWidget* w = (QWidget*)children().at(i);
			QGroupBox* group = qobject_cast<QGroupBox*>(w);

			if(group)
				group->setEnabled(false);
		}
#endif
		ui->btnRefresh->setEnabled(false);
		ui->groupboxSend->setEnabled(false);
		return;
	}
	ui->btnRefresh->setEnabled(true);
	ui->groupboxSend->setEnabled(true);

	// ...
	siInfo.cbSize = sizeof(SCROLLINFO);
	siInfo.fMask = SIF_ALL;

	fStyle = GetWindowLong(hwnd, GWL_STYLE);
	GetClassNameW(hwnd, szClassName, MAX_BUFFER);

	nBarType = SB_HORZ;
	if(wcsicmp(szClassName, WC_SCROLLBAR) == 0)
	{
		if((fStyle & SBS_VERT) == 0)
			nBarType = SB_CTL;
	}
	if(GetScrollInfo(hwnd, nBarType, &siInfo))
	{
		ui->txtHorzMin->setText(QString().number(siInfo.nMin));
		ui->txtHorzMax->setText(QString().number(siInfo.nMax));
		ui->txtHorzPage->setText(QString().number(siInfo.nPage));
		ui->txtHorzPos->setText(QString().number(siInfo.nPos));
		ui->txtHorzTrackpos->setText(QString().number(siInfo.nTrackPos));

		if(nBarType == SB_HORZ)
		{
			if(!(fStyle & WS_HSCROLL))
				ui->txtHorzState->setText(tr("Hidden"));
			else
				ui->txtHorzState->setText(tr("Visible"));
		}
	}
	else
	{
		ui->txtHorzMin->clear();
		ui->txtHorzMax->clear();
		ui->txtHorzPage->clear();
		ui->txtHorzPos->clear();
		ui->txtHorzTrackpos->clear();

		ui->txtHorzState->setText(tr("Disabled"));
	}
	nBarType = SB_VERT;
	if(wcsicmp(szClassName, WC_SCROLLBAR) == 0)
	{
		if((fStyle & SBS_VERT) == SB_VERT)
			nBarType = SB_CTL;
	}
	if(GetScrollInfo(hwnd, nBarType, &siInfo))
	{
		ui->txtVertMin->setText(QString().number(siInfo.nMin));
		ui->txtVertMax->setText(QString().number(siInfo.nMax));
		ui->txtVertPage->setText(QString().number(siInfo.nPage));
		ui->txtVertPos->setText(QString().number(siInfo.nPos));
		ui->txtVertTrackpos->setText(QString().number(siInfo.nTrackPos));
		if(nBarType == SB_VERT)
		{
			if(!(fStyle & WS_VSCROLL))
				ui->txtVertState->setText(tr("Hidden"));
			else
				ui->txtVertState->setText(tr("Visible"));
		}
	}
	else
	{
		ui->txtVertMin->clear();
		ui->txtVertMax->clear();
		ui->txtVertPage->clear();
		ui->txtVertPos->clear();
		ui->txtVertTrackpos->clear();

		ui->txtVertState->setText(tr("Disabled"));
	}
}
void FormInfoScroll::slotRefresh()
{
	HWND hwnd;
	if(NULL != (hwnd = GetCurrentWindow()))
	{
		setupInfo(hwnd, NULL);
	}
}
void FormInfoScroll::slotSendMessage()
{
	HWND hwnd;
	ULONG nTypeBar;
	ULONG nScrollPos;
	ULONG nScrollCode;

	if(NULL != (hwnd = GetCurrentWindow()))
	{
		switch(ui->cboBar->currentIndex())
		{
			case 0:
				nTypeBar = SB_HORZ;
				break;
			case 1:
				nTypeBar = SB_VERT;
				break;
//			case 2:
			default:
				nTypeBar = SB_BOTH;
				break;
		}
		nScrollPos = ui->spinbox_send_pos->value();
		nScrollCode = ui->cboTypeOfMessage->currentIndex();
#if 0	//!!!
		if(nScrollCode == SB_THUMBTRACK || nScrollCode == SB_THUMBPOSITION)
		{
#if 0	//!!!
			SetScrollPos(hwnd, nTypeBar, nScrollPos, true);
#else
			SCROLLINFO sInfo =
			{
				sizeof(SCROLLINFO), 0, 0,
			};
			if(nScrollCode == SB_THUMBTRACK) sInfo.fMask = SIF_TRACKPOS;
			else if(nScrollCode == SB_THUMBPOSITION) sInfo.fMask = SIF_POS;

			sInfo.nPos = nScrollPos;
			sInfo.nTrackPos = nScrollPos;
			SetScrollInfo(hwnd, nTypeBar, &sInfo, true);

			InvalidateRect(hwnd, NULL, TRUE);
#endif
		}
		else
#endif
		{
			switch(nTypeBar)
			{
				case SB_HORZ:
					SendMessage(hwnd, WM_HSCROLL, MAKEWPARAM(nScrollCode, nScrollPos), 0);
					break;
				case SB_VERT:
					SendMessage(hwnd, WM_VSCROLL, MAKEWPARAM(nScrollCode, nScrollPos), 0);
					break;
				case SB_CTL:
					break;
				case SB_BOTH:
					break;
			}
		}
	}
}
void FormInfoScroll::changeEvent(QEvent* e)
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




