#include "MButton.h"

MButton::MButton (QWidget* parent) : QPushButton (parent)
{
    connect(this, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
}

MButton::MButton (QString text, QWidget* parent) : QPushButton (text, parent)
{
    connect(this, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
}

MButton::~MButton () { }

void MButton::onButtonClicked()
{
    emit clicked(this);
}