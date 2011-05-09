#include "delegate/GamelistDelegate.h"

void GamelistDelegate::paint(QPainter * painter,
        const QStyleOptionViewItem & option, const QModelIndex & index)
const
{
//    if (option.state & QStyle::State_Selected)
//    {
//        painter->fillRect(option.rect, option.palette.color(QPalette::Highlight));
//    }

    QString bot = "Botname: " + index.data(35).toString();
    QString gamename = index.data(36).toString();
    QString openSlots = "Open slots: " + index.data(37).toString();
//    QColor color = getColor(index.data(37).toString().mid(1).toInt());

    QRect rect = option.rect;
    painter->save();

//    painter->fillRect(rect, color);
    painter->drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());
//    painter->setPen(QColor(0, 0, 0));
    painter->drawText(rect, Qt::AlignHCenter, gamename);
    rect.adjust(0, 15, 0, 0);
    painter->drawText(rect, Qt::AlignLeft, openSlots);
    painter->drawText(rect, Qt::AlignRight, bot);

    painter->restore();
}

QSize GamelistDelegate::sizeHint(const QStyleOptionViewItem & option,
        const QModelIndex & index)
const
{
    return QSize(200, 35);
}

QColor GamelistDelegate::getColor(int openSlots) const
{
//    switch(openSlots)
//    {
//        case 0:
//            return QColor(140, 0, 0);
//            break;
//        case 1:
//            return QColor(225, 0, 0);
//            break;
//        case 2:
//            return QColor(255, 0, 70);
//            break;
//        case 3:
//            return QColor(255, 0, 255);
//            break;
//        case 4:
//            return QColor(173, 47, 255);
//            break;
//        case 5:
//            return QColor(152, 152, 251);
//            break;
//        case 6:
//            return QColor(0, 127, 255);
//            break;
//        case 7:
//            return QColor(102, 170, 205);
//            break;
//        case 8:
//            return QColor(0, 255, 255);
//            break;
//        case 9:
//            return QColor(0, 209, 206);
//            break;
//        case 10:
//            return QColor(30, 255, 144);
//            break;
//        default:
//            return QColor(255, 255, 255);
//    }
    return QColor::fromRgb(255, 255, 255);
}