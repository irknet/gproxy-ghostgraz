#ifndef CHANNELLISTDELEGATE_H
#define	CHANNELLISTDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QColor>

#include "MainGUI.h"
#include "data/Player.h"

class ChannellistDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ChannellistDelegate(MainGUI* mainGUI);

    enum ChannelData
    {
        USER = 35,
        CLAN_TAG = 36,
        COLOR_USER = 37,
        SLOT_PID = 40,
        SLOT_DOWNLOAD_STATUS = 41,
        SLOT_STATUS = 42,
        SLOT_COMPUTER_STATUS = 43,
        SLOT_TEAM = 44,
        SLOT_COLOR = 45,
        SLOT_RACE = 46,
        SLOT_COMPUTER_TYPE = 47,
        SLOT_HANDICAP = 48
    };

protected:
    void paint(QPainter * painter, const QStyleOptionViewItem & option,
            const QModelIndex & index) const;
//    QSize sizeHint(const QStyleOptionViewItem & option,
//            const QModelIndex & index) const;

private:
    MainGUI *mainGUI;

    QBrush getSlotColor(const int& colorID) const;
};

#endif	/* CHANNELLISTDELEGATE_H */

