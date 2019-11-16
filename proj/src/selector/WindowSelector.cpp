#include "WindowSelector.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include "WindowSelectorRectWidget.h"


#include <windows.h>
#include <dwmapi.h>
#include "3rd/WindowFromPointEx.h"

QRect GetRectOfWindowFromMousePosition(HWND* hwnd, bool hiddenWindows)
{
	POINT pt;
	HWND hWndInPoint;
	QRect rectReturn;

	GetCursorPos(&pt);
	hWndInPoint = WindowFromPointEx(pt, hiddenWindows);
	if(hWndInPoint != NULL)
	{
		RECT rc;
		HRESULT hr;
		OSVERSIONINFOW osvi;
		osvi.dwOSVersionInfoSize = sizeof(osvi);
		GetVersionExW(&osvi);
/*
		qDebug("osvi.dwMajorVersion: %d", osvi.dwMajorVersion);
		qDebug("osvi.dwMinorVersion: %d", osvi.dwMinorVersion);
*/
		hr = -1;
		if(osvi.dwMajorVersion > 6 || (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2))
		{
			hr = DwmGetWindowAttribute(hWndInPoint, DWMWA_EXTENDED_FRAME_BOUNDS, &rc, sizeof(RECT));
		}
		if(!SUCCEEDED(hr))
		{
			GetWindowRect(hWndInPoint, &rc);
		}
		rectReturn.setCoords(rc.left, rc.top, rc.right, rc.bottom);
	}
	*hwnd = hWndInPoint;
	return rectReturn;
}


class WindowSelectorPrivate
{
public:
	bool						pressed;
	int							size;
	QImage						image1;
	QImage						image2;
	QRect						selectedRect;
	WindowSelectorRectWidget*	selectedRectWidget;
	QColor						selectionColor;

	HWND						target;
	HWND						previousTarget;
	bool						hiddenWindows;
public:
	WindowSelectorPrivate()
		: pressed(false)
		, selectedRectWidget(0)
		, selectionColor(Qt::red)
		, target(0)
		, previousTarget(0)
	{
	}
	~WindowSelectorPrivate()
	{
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////
WindowSelector::WindowSelector(QWidget* parent, int size, const QImage& image1, const QImage& image2) : QLabel(parent)
{
	d = new WindowSelectorPrivate();
	setCursor(QCursor(Qt::SizeAllCursor));
	setFrameShadow(QFrame::Sunken);
	setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	d->size = size;
	d->image1 = image1;
	d->image2 = image2;

	setPixmap(QPixmap::fromImage(image1));
}
WindowSelector::~WindowSelector()
{
	delete d;
}
void		WindowSelector::setHiddenWindows(bool hw)
{
	d->hiddenWindows = hw;
}
QColor		WindowSelector::getSelectionColor() const
{
	return d->selectionColor;
}
void		WindowSelector::setSelectionColor(const QColor& color)
{
	d->selectionColor = color;
}
QSize		WindowSelector::sizeHint() const
{
	return QSize(d->size, d->size);
}
QSize		WindowSelector::minimumSizeHint() const
{
	return sizeHint();
}
void		WindowSelector::keyPressEvent(QKeyEvent* e)
{
	if(e->key() == Qt::Key_Escape)
	{
		d->pressed = false;
		setPixmap(QPixmap::fromImage(d->image1));
		releaseMouse();
		releaseKeyboard();

		if(d->selectedRectWidget)
		{
			delete d->selectedRectWidget;
			d->selectedRect = QRect();
			d->selectedRectWidget = 0;
		}

		emit onCancelSelection((WId)d->previousTarget);
		d->target = NULL;
	}
}
void		WindowSelector::mouseMoveEvent(QMouseEvent* e)
{
	Q_UNUSED(e);
	if(d->pressed)
	{
		QRect rc = GetRectOfWindowFromMousePosition(&d->target, d->hiddenWindows);
		if(rc != d->selectedRect)
		{
			d->selectedRect = rc;
			if(d->selectedRectWidget)
				delete d->selectedRectWidget;

			d->selectedRectWidget = new WindowSelectorRectWidget(d->selectedRect, d->selectionColor);

			emit onChangeSelection((WId)d->target);
		}
		setCursor(QCursor(Qt::SizeAllCursor));
	}
}
void		WindowSelector::mousePressEvent(QMouseEvent* e)
{
	if(e->button() == Qt::LeftButton)
	{
		d->pressed = true;
		setPixmap(QPixmap::fromImage(d->image2));
		grabMouse();
		grabKeyboard();

		setCursor(Qt::WaitCursor);
		emit onStartSelection(0);
		setCursor(QCursor(Qt::SizeAllCursor));

		mouseMoveEvent(e);
	}
}
void		WindowSelector::mouseReleaseEvent(QMouseEvent* e)
{
	Q_UNUSED(e);
	if(d->pressed)
	{
		d->pressed = false;
		setPixmap(QPixmap::fromImage(d->image1));
		releaseMouse();
		releaseKeyboard();

//		QRect rc = d->selectedRect;
		if(d->selectedRectWidget)
		{
			delete d->selectedRectWidget;
			d->selectedRect = QRect();
			d->selectedRectWidget = 0;
		}
		emit onFinishSelection((WId)d->target);
		d->previousTarget = d->target;
		d->target = NULL;
	}
}


