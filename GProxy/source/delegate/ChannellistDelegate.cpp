#include "delegate/ChannellistDelegate.h"
#include "gproxy.h"
#include "bnet.h"

ChannellistDelegate::ChannellistDelegate (CGProxy* gproxy)
{
    this->gproxy = gproxy;
}

void ChannellistDelegate::paint (QPainter * painter,
        const QStyleOptionViewItem & option, const QModelIndex & index)
const
{
    painter->save();

    QRect rect = option.rect;
    QString user = index.data(ChannellistDelegate::USER).toString();
    QColor color = index.data(ChannellistDelegate::COLOR_USER).value<QColor>();
    if (!color.isValid())
    {
        color = QColor(255, 255, 255);
    }

    painter->setPen(color);

    if (gproxy->m_BNET->GetInGame())
    {
        int slotStatus = index.data(ChannellistDelegate::SLOT_STATUS).toInt();
        int computerStatus = index.data(ChannellistDelegate::SLOT_COMPUTER_STATUS).toInt();
        int computerType = index.data(ChannellistDelegate::SLOT_COMPUTER_TYPE).toInt();

        if (user != "The Sentinel" && user != "The Scourge"
                && !user.startsWith("Team "))
        {
            if (slotStatus == 0)
            {
                user = "Open";
            }
            else if (slotStatus == 1)
            {
                user = "Closed";
            }
            else if (slotStatus == 2)
            {
                if (computerStatus == 1)
                {
                    if (computerType == 0)
                    {
                        user = "Computer(Easy)";
                    }
                    else if (computerType == 1)
                    {
                        user = "Computer(Normal)";
                    }
                    else if (computerType == 2)
                    {
                        user = "Computer(Hard)";
                    }
                }
            }
        }

//        int colorID = index.data(ChannellistDelegate::SLOT_COLOR).toInt();
//        QRect rFill(rect.x()+3, rect.y()-12, 8, 8);
//        painter->fillRect(rFill, this->getBrush(colorID));
        rect.adjust(15, 0, 0, 0);
        painter->drawText(rect, Qt::AlignLeft, user);
    }
    else
    {
        QString clanTag = index.data(ChannellistDelegate::CLAN_TAG).toString();

        painter->drawText(rect, Qt::AlignLeft, user);
        painter->setPen(QColor(255, 255, 0));
        painter->drawText(rect, Qt::AlignRight, clanTag + " ");
    }

    painter->restore();
}

QBrush ChannellistDelegate::getBrush (const int &colorID) const
{
    QColor c;
    QBrush b;
    b.setStyle(Qt::SolidPattern);

    switch (colorID)
    {
        case 0:
        {
            c = QColor::fromRgb(255, 0, 0);
            break;
        }
        case 1:
        {
            c = QColor::fromRgb(0, 0, 255);
            break;
        }
        case 2:
        {
            c = QColor::fromRgb(0, 255, 255);
            break;
        }
        case 3:
        {
            c = QColor::fromRgb(128, 0, 128);
            break;
        }
        case 4:
        {
            c = QColor::fromRgb(255, 255, 0);
            break;
        }
        case 5:
        {
            c = QColor::fromRgb(255, 140, 0);
            break;
        }
        case 6:
        {
            c = QColor::fromRgb(0, 255, 0);
            break;
        }
        case 7:
        {
            c = QColor::fromRgb(255, 0, 255);
            break;
        }
        case 8:
        {
            c = QColor::fromRgb(128, 128, 128);
            break;
        }
        case 9:
        {
            c = QColor::fromRgb(100, 149, 237);
            break;
        }
        case 10:
        {
            c = QColor::fromRgb(34, 139, 34);
            break;
        }
        case 11:
        {
            c = QColor::fromRgb(102, 51, 0);
            break;
        }
        default:
        {
            c = QColor::fromRgb(255, 255, 255);
            break;
        }
    }

    b.setColor(c);
    return b;
}