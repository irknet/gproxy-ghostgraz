#include <QString>

#include "delegate/ChannellistDelegate.h"
#include "gproxy.h"
#include "bnet.h"

ChannellistDelegate::ChannellistDelegate (MainGUI* mainGUI)
{
    this->mainGUI = mainGUI;
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
        color = mainGUI->getGproxy()->getConfig()->getDefaultColor();
    }
    painter->setPen(color);

    if (mainGUI->getGproxy()->m_BNET->GetInGame())
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
                else
                {
                    if (mainGUI->isAdmin(user))
                    {
                        painter->setPen(QColor(0, 255, 200));
                    }
                    else
                    {
                        QList<Player*> players = mainGUI->getGproxy()->getPlayers();
                        foreach(Player *player, players)
                        {
                            if(player->getName() == user)
                            {
                                if (!mainGUI->getGproxy()->m_GameStarted)
                                {
                                    if (player->getGamesPlayed() == 0 || player->getStayPercent() < 80)
                                    {
                                        painter->setPen(QColor(255, 69, 0));
                                    }
                                }

                                break;
                            }
                        }
                    }
                }
            }

            int colorID = index.data(ChannellistDelegate::SLOT_COLOR).toInt();
            QRect rFill(rect.x()+3, rect.y() + 3, 8, 8);
            painter->fillRect(rFill, this->getSlotColor(colorID));

            rect.adjust(15, 0, 0, 0);
        }

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

QBrush ChannellistDelegate::getSlotColor (const int& colorID) const
{
    switch (colorID)
    {
        case 0:
        {
            return QBrush(QColor(255, 0, 0));
        }
        case 1:
        {
            return QBrush(QColor(0, 0, 255));
        }
        case 2:
        {
            return QBrush(QColor(0, 255, 255));
        }
        case 3:
        {
            return QBrush(QColor(128, 0, 128));
        }
        case 4:
        {
            return QBrush(QColor(255, 255, 0));
        }
        case 5:
        {
            return QBrush(QColor(255, 140, 0));
        }
        case 6:
        {
            return QBrush(QColor(0, 255, 0));
        }
        case 7:
        {
            return QBrush(QColor(255, 0, 255));
        }
        case 8:
        {
            return QBrush(QColor(128, 128, 128));
        }
        case 9:
        {
            return QBrush(QColor(100, 149, 237));
        }
        case 10:
        {
            return QBrush(QColor(34, 139, 34));
        }
        case 11:
        {
            return QBrush(QColor(102, 51, 0));
        }
        default:
        {
            // Should never be possible!
            return mainGUI->getGproxy()->getConfig()->getDefaultColor();
        }
    }
}