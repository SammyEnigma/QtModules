#ifndef XCHECKBOX_H
#define XCHECKBOX_H


#include <QCheckBox>

class XCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    XCheckBox(QWidget *parent = nullptr);
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
};

#endif // XCHECKBOX_H
