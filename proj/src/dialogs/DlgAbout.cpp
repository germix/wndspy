#include "DlgAbout.h"
#include "ui_DlgAbout.h"
#include <QtGui>


DlgAbout::DlgAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAbout)
{
    ui->setupUi(this);

	// Mostrar información
	{
		QString s;
		QFileInfo info(qApp->applicationFilePath());

		s += tr("Time") + ": ";
		s += info.lastModified().time().toString();
		s += "\n";
		s += tr("Date") + ": ";
		s += info.lastModified().date().toString();
		s += "\n";
		s += tr("Version") + ": ";
		s += QString::number(MAJOR_VERSION) + "." + QString::number(MINOR_VERSION);
		s += "\n";
		s += tr("Developer") + ": Germán Martínez";
		ui->label->setText(s);
	}
	// Ajustar tamaño
	{
		QSize s = sizeHint();
		setMinimumSize(s);
		setMaximumSize(s);
	}
}

DlgAbout::~DlgAbout()
{
    delete ui;
}
