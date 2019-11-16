#ifndef LINKLABEL_H
#define LINKLABEL_H

#include <QLabel>

class LinkLabel : public QLabel
{
    Q_OBJECT
public:
	LinkLabel(QWidget *parent = 0);
	~LinkLabel();
signals:
	void pressed(Qt::MouseButton mb);
private:
	void mousePressEvent(QMouseEvent* e);
};

#endif // LINKLABEL_H
