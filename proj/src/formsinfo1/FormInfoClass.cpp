#include "FormInfoClass.h"
#include "ui_FormInfoClass.h"

#include <QPainter>
#include <WindowInfo.h>

extern "C" BOOL GetIconSize(HICON hIcon, int* pnWidth, int* pnHeight);

typedef struct _NATIVE_IMAGE
{
	HDC			hdc;
	HBITMAP		bitmap;
	HBITMAP		null_bitmap;

	UCHAR* bits;
	int	width;
	int	height;
}NATIVE_IMAGE;

typedef struct {
	BITMAPINFOHEADER bmiHeader;
	DWORD redMask;
	DWORD greenMask;
	DWORD blueMask;
} BITMAPINFO_MASK;


VOID NativeImage_Init(NATIVE_IMAGE* image, int width, int height)
{
	BITMAPINFO_MASK bmi;

	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize			 = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth			 = width;
	bmi.bmiHeader.biHeight			 = -height;
	bmi.bmiHeader.biPlanes			 = 1;
	bmi.bmiHeader.biSizeImage		 = 0;
	if(0)		// Format_RGB16
	{
#ifdef OS_WINCE

#else
		{
			bmi.bmiHeader.biCompression	 = BI_BITFIELDS;
			bmi.redMask		 = 0xF800;
			bmi.greenMask	 = 0x07E0;
			bmi.blueMask	 = 0x001F;
		}
#endif
	}
	else
	{
		bmi.bmiHeader.biBitCount	 = 32;
		bmi.bmiHeader.biCompression	 = BI_RGB;
		bmi.redMask		 = 0;
		bmi.greenMask	 = 0;
		bmi.blueMask	 = 0;
	}
	HDC display_dc = GetDC(0);
	image->hdc = CreateCompatibleDC(display_dc);
	ReleaseDC(0, display_dc);

	UCHAR* bits = 0;
	image->bitmap = CreateDIBSection(image->hdc, (BITMAPINFO*)&bmi, DIB_RGB_COLORS, (void**)&bits, 0, 0);

	image->null_bitmap = (HBITMAP)SelectObject(image->hdc, image->bitmap);

	for(int i = 0; i < (width*height*4); i++)
	{
		bits[i] = 0;
	//	bits[i] = 0xff;
	}
	image->bits = bits;
	image->width = width;
	image->height = height;
#ifndef OS_WINCE
	//GdiFlush();
#endif
}
VOID NativeImage_Shutdown(NATIVE_IMAGE* image)
{
	SelectObject(image->hdc, image->null_bitmap);
	DeleteObject(image->bitmap);
	DeleteDC(image->hdc);
}

#define ICON_SIZE	32
#define LABEL_SIZE	(ICON_SIZE+10)
class QWinIconLabel : public QLabel
{
public:
	QWinIconLabel(QWidget* parent) : QLabel(parent)
	{
		m_hicon = NULL;
		setMinimumSize(LABEL_SIZE, LABEL_SIZE);
		setMaximumSize(LABEL_SIZE, LABEL_SIZE);
		setFrameShape(QFrame::Panel);
		setFrameShadow(QFrame::Sunken);
	}
	~QWinIconLabel()
	{
	}
public:
	int		m_cx;
	int		m_cy;
	HICON	m_hicon;
	void setIcon(HICON hicon)
	{
		m_hicon = hicon;
		GetIconSize(m_hicon, &m_cx, &m_cy);

		update();
	}
	void paintEvent(QPaintEvent* e)
	{
		if(m_hicon)
		{
			int width = LABEL_SIZE;
			int height = LABEL_SIZE;
			NATIVE_IMAGE Native;

			NativeImage_Init(&Native, width, height);
//			DrawIcon(Native.hdc, (LABEL_SIZE-ICON_SIZE)/2, (LABEL_SIZE-ICON_SIZE)/2, m_hicon);
			DrawIconEx(Native.hdc, (LABEL_SIZE-m_cx)/2, (LABEL_SIZE-m_cy)/2, m_hicon, m_cx, m_cy, 0, 0, DI_NORMAL);
			{
				QPainter p(this);
				QImage Image(Native.bits, width, height, QImage::Format_ARGB32);

				p.drawImage(0, 0, Image);
			}
			NativeImage_Shutdown(&Native);
		}
		QLabel::paintEvent(e);
	}
};
class QWinCursorLabel : public QLabel
{
	HCURSOR	m_hcursor;
public:
	QWinCursorLabel(QWidget* parent) : QLabel(parent)
	{
		m_hcursor = NULL;
		setMinimumSize(LABEL_SIZE, LABEL_SIZE);
		setMaximumSize(LABEL_SIZE, LABEL_SIZE);
		setFrameShape(QFrame::Panel);
		setFrameShadow(QFrame::Sunken);
	}
	~QWinCursorLabel()
	{
	}
public:
	void setCursor(HCURSOR hcursor)
	{
		m_hcursor = hcursor;
		update();
	}
	void paintEvent(QPaintEvent* e)
	{
		if(m_hcursor)
		{
			int x;
			int y;
			int width = ICON_SIZE;
			int height = ICON_SIZE;
			NATIVE_IMAGE NativeMask;
			NATIVE_IMAGE NativeImage;
			NATIVE_IMAGE NativeResult;

			NativeImage_Init(&NativeMask, width, height);
			NativeImage_Init(&NativeImage, width, height);
			NativeImage_Init(&NativeResult, width, height);

			DrawIconEx(NativeMask.hdc, 0, 0, m_hcursor, 0, 0, 0, NULL, DI_DEFAULTSIZE | DI_MASK);
			DrawIconEx(NativeImage.hdc, 0, 0, m_hcursor, 0, 0, 0, NULL, DI_DEFAULTSIZE | DI_IMAGE);

			for(y = 0; y < height; y++)
			{
				for(x = 0; x < width; x++)
				{
					UINT i = (x + (y * width));
					BYTE* pm = (BYTE*)&((DWORD*)NativeMask.bits)[i];
					BYTE* pi = (BYTE*)&((DWORD*)NativeImage.bits)[i];
					BYTE* pr = (BYTE*)&((DWORD*)NativeResult.bits)[i];

					if(!pm[0] && !pm[1] && !pm[2])
					{
						pr[0] = pi[0];
						pr[1] = pi[1];
						pr[2] = pi[2];
						pr[3] = 255;
					}
				}
			}
			{
				QPainter p(this);
				QImage Image(NativeResult.bits, width, height, QImage::Format_ARGB32);

				x = y = (LABEL_SIZE-ICON_SIZE)/2;
				p.drawImage(x, y, Image);
			}
			NativeImage_Shutdown(&NativeMask);
			NativeImage_Shutdown(&NativeImage);
			NativeImage_Shutdown(&NativeResult);
		}
		QLabel::paintEvent(e);
	}
};
class QWinBrushLabel : public QLabel
{
	HBRUSH		m_hbrush;
public:
	QWinBrushLabel(QWidget* parent) : QLabel(parent)
	{
		m_hbrush = NULL;
		setMinimumSize(LABEL_SIZE, LABEL_SIZE);
		setMaximumSize(LABEL_SIZE, LABEL_SIZE);
		setFrameShape(QFrame::Panel);
		setFrameShadow(QFrame::Sunken);
	}
	~QWinBrushLabel()
	{
	}
public:
	void setBrush(HBRUSH hbrush)
	{
		m_hbrush = hbrush;
		update();
	}
	void paintEvent(QPaintEvent* e)
	{
		QWidget* w = this;
		RECT rc;
		int width = w->width();
		int height = w->height();
		rc.left = 0;
		rc.top = 0;
		rc.right = width;
		rc.bottom = height;

		NATIVE_IMAGE Native;

		NativeImage_Init(&Native, width, height);
		if(!m_hbrush)
		{
			HPEN hOldPen;
			HBRUSH hBrush;

			hBrush = CreateSolidBrush(0);
			FillRect(Native.hdc, &rc, hBrush);
			DeleteObject(hBrush);
			hOldPen = (HPEN)SelectObject(Native.hdc, CreatePen(PS_SOLID, 1, RGB(255, 0, 0)));
			MoveToEx(Native.hdc, 0, 0, NULL);
			LineTo(Native.hdc, rc.right, rc.bottom);
			MoveToEx(Native.hdc, rc.right - 1, 0, NULL);
			LineTo(Native.hdc, -1, rc.bottom);
			DeleteObject(SelectObject(Native.hdc, hOldPen));
		}
		else
		{
			FillRect(Native.hdc, &rc, m_hbrush);
		}
		{
			QPainter p(this);
			QImage Image(Native.bits, width, height, QImage::Format_RGB32);

			p.drawImage(0, 0, Image);
		}
		NativeImage_Shutdown(&Native);

		// ...
		QLabel::paintEvent(e);
	}
};

FormInfoClass::FormInfoClass(QWidget* parent) : QWidget(parent), ui(new Ui::FormInfoClass)
{
    ui->setupUi(this);

	m_LblIcon1 = new QWinIconLabel(ui->groupBoxIcon1);
	m_LblIcon2 = new QWinIconLabel(ui->groupBoxIcon2);
	m_LblCursor = new QWinCursorLabel(ui->groupBoxCursor);
	m_LblBrush = new QWinBrushLabel(ui->groupBoxBrush);

	QGridLayout* l;
	if((l = (QGridLayout*)ui->groupBoxIcon1->layout())) l->addWidget(m_LblIcon1, 0, 1, 2, 1);
	if((l = (QGridLayout*)ui->groupBoxIcon2->layout())) l->addWidget(m_LblIcon2, 0, 1, 2, 1);
	if((l = (QGridLayout*)ui->groupBoxCursor->layout())) l->addWidget(m_LblCursor, 0, 1, 2, 1);
	if((l = (QGridLayout*)ui->groupBoxBrush->layout())) l->addWidget(m_LblBrush, 0, 1, 2, 1);

	QChangePaletteLineEdit_IsReadOnly(this, ui->txtBrush);
}

FormInfoClass::~FormInfoClass()
{
    delete ui;
}
void FormInfoClass::changeEvent(QEvent* e)
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
void FormInfoClass::setupInfo(HWND hwnd, class WindowInfo *info)
{
	if(!hwnd)
		return;
	DWORD fStyle;
	ULONG nNumBytes;
	QString s;
	bool ok;

#if 1	//!!!
	// WARNING!!!
	// Siendo que se crea una copia de los iconos, cursor y brush,
	// la información de los handle, no es correcta, y
	// la llamada a la función AppendFormatHandleLookup no da el resultado adecuado

	// Icono pequeño
	m_LblIcon1->setIcon(info->hIconSm);
	m_LblIcon1->setToolTip(AppendFormatHandleLookup(g_aLookup_Icon, info->hIconSm));
	ui->txtIcon1->setText(QHexHandle(info->hIconSm));

	// Icono grande
	m_LblIcon2->setIcon(info->hIconLg);
	m_LblIcon2->setToolTip(AppendFormatHandleLookup(g_aLookup_Icon, info->hIconLg));
	ui->txtIcon2->setText(QHexHandle(info->hIconLg));

	// Cursor
	m_LblCursor->setCursor(info->hCursor);
	m_LblCursor->setToolTip(AppendFormatHandleLookup(g_aLookup_Cursor, info->hCursor));
	ui->txtCursor->setText(QHexHandle(info->hCursor));

	// Color de fondo
	m_LblBrush->setBrush(info->hBrush);
	m_LblBrush->setToolTip(AppendFormatHandleLookup(g_aLookup_StockBrush, info->hCursor, &ok));
	if(!ok)
		m_LblBrush->setToolTip(AppendFormatHandleLookup(g_aLookup_SystemBrush, info->hCursor, &ok));
	ui->txtBrush->setText(QHexHandle(info->hBrush));

#else
#endif
	// Nombre de la clase
	ui->txtClassName->setText(QGetClassName(hwnd));

	// Estilos de la clase
	fStyle = GetClassLong(hwnd, GCL_STYLE);
	ui->txtClassStyles->setText(QHexHandle((void*)fStyle));

	ui->cboClassStyles->clear();
	if(fStyle == 0)
	{
		ui->cboClassStyles->setEnabled(false);
	}
	else
	{
		ui->cboClassStyles->setEnabled(true);
		for(int i = 0; g_aStyles_Class[i].pszStyleName != NULL; i++)
		{
			if(fStyle & g_aStyles_Class[i].fStyle)
			{
				ui->cboClassStyles->addItem(QString::fromWCharArray(g_aStyles_Class[i].pszStyleName));
			}
		}
		ui->cboClassStyles->setCurrentIndex(0);
	}
	// Bytes extras de la clase
	nNumBytes = GetClassLong(hwnd, GCL_CBCLSEXTRA);

	ui->txtClassBytes->setText(QString().sprintf("%d", (uint)nNumBytes));

	ui->cboClassBytes->clear();
	if(nNumBytes == 0)
	{
		ui->cboClassBytes->setEnabled(false);
	}
	else
	{
		ui->cboClassBytes->setEnabled(true);
		for(int i = 0; nNumBytes != 0; i += 4)
		{
			if(nNumBytes >= 4)
			{
				s.sprintf("+%-8d %08X", i, (uint)GetClassLong(hwnd, i));
				nNumBytes -= 4;
			}
			else
			{
				s.sprintf("+%-8d (Unavailable)", i);
//				s.sprintf("+%-8d %s", i, "(Unavailable)");

				nNumBytes = 0;
			}
			ui->cboClassBytes->addItem(s);
			ui->cboClassBytes->setCurrentIndex(0);
		}
	}
	// Atomo de la clase
	s.sprintf("%04X", (uint)GetClassLong(hwnd, GCW_ATOM));
	ui->txtClassAtom->setText(s);

	// Bytes extra de la ventana
	s.sprintf("%d", (uint)GetClassLong(hwnd, GCL_CBWNDEXTRA));
	ui->txtWindowBytes->setText(s);

#if 0
	// Nombre del menú
	s = info->menuname;
	if(s.isEmpty())
		s = tr("(None)");
	ui->txtMenuHandle->setText(s);
#else
	// Manipulador del menú
	s.sprintf(HEX_FORMAT, (uint)info->hMenu);
	ui->txtMenuHandle->setText(s);
#endif

	// Manipulador de instancia
	s.sprintf(HEX_FORMAT, (uint)GetClassLong(hwnd, GCL_HMODULE));
	ui->txtInstance->setText(s);

	// Procedimiento de clase
	if(info->pfnClassProc == 0)
		s = tr("N/A");
	else
		s.sprintf(HEX_FORMAT, (uint)info->pfnClassProc);
	ui->txtClassProc->setText(s);

	// Procedimiento de ventana
	if(info->pfnWindowProc == 0)
		s = tr("N/A");
	else
	{
		s.sprintf(HEX_FORMAT, (uint)info->pfnWindowProc);
		if(info->pfnClassProc != info->pfnWindowProc)
		{
			s.append(" " + tr("(Subclass)"));
		}
	}
	ui->txtWindowProc->setText(s);
}


