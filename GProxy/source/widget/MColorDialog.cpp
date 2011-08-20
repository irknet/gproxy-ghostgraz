#include "MColorDialog.h"

MColorDialog::MColorDialog (QWidget* parent) : QColorDialog (parent)
{
    connect(this, SIGNAL(currentColorChanged(const QColor&)),
            this, SLOT(onCurrentColorChanged(const QColor&)));
}

MColorDialog::MColorDialog (QColor startColor, QWidget* parent) : QColorDialog (startColor, parent)
{
    connect(this, SIGNAL(currentColorChanged(const QColor&)),
            this, SLOT(onCurrentColorChanged(const QColor&)));
}

MColorDialog::~MColorDialog () { }

void MColorDialog::onCurrentColorChanged(const QColor&)
{
    emit currentColorChanged(this);
}