#include "delegate/GamelistDelegate.h"

void GamelistDelegate::paint (QPainter * painter,
        const QStyleOptionViewItem & option, const QModelIndex & index)
const
{
    QString bot = "Botname: " + index.data(35).toString();
    QString gamename = index.data(36).toString();
    QString openSlots = "Open slots: " + index.data(37).toString();

    int openSlotsCount = index.data(37).toString().mid(1).toInt();
    QBrush brush = getBrush(openSlotsCount);

    if (gamename.startsWith("in channel") || gamename == "-")
    {
        brush = QBrush(QColor(255, 255, 255));
    }

    QRect rect = option.rect;
    painter->save();

    painter->setPen(QColor(0, 0, 0));
    QFont font = painter->font();
    font.setBold(true);
    painter->setFont(font);

    painter->fillRect(rect, brush);
    painter->drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());

    painter->drawText(rect, Qt::AlignHCenter, gamename);
    rect.adjust(0, 15, 0, 0);
    painter->drawText(rect, Qt::AlignLeft, openSlots);
    painter->drawText(rect, Qt::AlignRight, bot);

    painter->restore();
}

QSize GamelistDelegate::sizeHint (const QStyleOptionViewItem & option,
        const QModelIndex & index)
const
{
    return QSize(200, 35);
}

QBrush GamelistDelegate::getBrush (int openSlots) const
{
    if (openSlots >= 6)
    {
        return QBrush(QColor(102, 255, 153));
    }
    else if (openSlots >= 4)
    {
        return QBrush(QColor(255, 165, 0));
    }
    else if (openSlots >= 1)
    {
        return QBrush(QColor(255, 0, 0));
    }
    else if (openSlots == 0)
    {
        return QBrush(QColor(153, 0, 0));
    }
    else
    {
        return QBrush(QColor(255, 255, 255));
    }
}