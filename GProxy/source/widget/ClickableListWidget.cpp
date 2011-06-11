#include "ClickableListWidget.h"

ClickableListWidget::ClickableListWidget (QWidget *parent) : QListWidget(parent)
{

}

ClickableListWidget::~ClickableListWidget () { }

void ClickableListWidget::mousePressEvent(QMouseEvent* mouseEvent)
{
    emit clicked(mouseEvent);
}