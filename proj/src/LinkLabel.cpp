#include "LinkLabel.h"
#include <QMouseEvent>

LinkLabel::LinkLabel(QWidget *parent) : QLabel(parent)
{
	setCursor(Qt::PointingHandCursor);
	QFont f = font();
	f.setBold(true);
	setFont(f);
}
LinkLabel::~LinkLabel()
{
}
void LinkLabel::mousePressEvent(QMouseEvent* e)
{
	emit pressed(e->button());
}
