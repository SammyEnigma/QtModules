#include "xcheckbox.h"


XCheckBox::XCheckBox(QWidget *parent) : QCheckBox (parent)
{

    this->setCursor(QCursor(Qt::PointingHandCursor));
    this->setMouseTracking(true);
}

void XCheckBox::enterEvent(QEvent *event)
{
    this->setStyleSheet(QString("color:yellow"));
}
void XCheckBox::leaveEvent(QEvent *event)
{
    this->setStyleSheet(QString(""));
}
