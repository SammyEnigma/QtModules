#include "xpushbutton.h"
#include <QDebug>

XPushButton::XPushButton(QWidget *parent) : QPushButton (parent)
{
    // set Cursor as a hand when mouse is on the button
    this->setCursor(QCursor(Qt::PointingHandCursor));
    // track mouse, enabling enterEvent and leaveEvent
    this->setMouseTracking(true);
}

void XPushButton::enterEvent(QEvent *event)
{
    // mouse is on the button
    this->setStyleSheet("color:yellow");
}
void XPushButton::leaveEvent(QEvent *event)
{
    // mouse has left the button
    this->setStyleSheet("");
}
