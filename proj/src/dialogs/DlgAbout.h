#ifndef DLGABOUT_H
#define DLGABOUT_H

#include <QDialog>

namespace Ui {
    class DlgAbout;
}

class DlgAbout : public QDialog
{
    Q_OBJECT
    Ui::DlgAbout *ui;

public:
    explicit DlgAbout(QWidget *parent = 0);
    ~DlgAbout();

private:
};

#endif // DLGABOUT_H
