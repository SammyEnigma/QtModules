#ifndef XPUSHBUTTON_H
#define XPUSHBUTTON_H

#include <QPushButton>

class XPushButton : public QPushButton
{
    Q_OBJECT
public:
    XPushButton(QWidget *parent = nullptr);
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
};

#endif // XPUSHBUTTON_H
