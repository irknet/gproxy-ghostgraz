#ifndef CLICKABLELISTWIDGET_H
#define	CLICKABLELISTWIDGET_H

#include <QListWidget>

class ClickableListWidget : public QListWidget
{
    Q_OBJECT

public:
    ClickableListWidget(QWidget *parent = 0);
    virtual ~ClickableListWidget();

protected:
    void mousePressEvent(QMouseEvent *mouseEvent);

signals:
    void clicked(QMouseEvent *mouseEvent);

};

#endif	/* CLICKABLELISTWIDGET_H */

