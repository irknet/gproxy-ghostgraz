#ifndef CLICKABLELINEEDIT_H
#define	CLICKABLELINEEDIT_H

#include <QLineEdit>

class ClickableLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    ClickableLineEdit(QWidget *parent = 0);
    virtual ~ClickableLineEdit();

protected:
    void mousePressEvent(QMouseEvent *event);
    void focusInEvent(QFocusEvent *event);

signals:
    void clicked();
};

#endif	/* CLICKABLELINEEDIT_H */

