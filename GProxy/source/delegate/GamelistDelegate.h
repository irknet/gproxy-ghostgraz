#ifndef MULTILINEDELEGATE_H
#define	MULTILINEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QColor>
#include <QFontMetrics>

class GamelistDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    enum GameData
    {
        BOTNAME = 35,
        GAMENAME = 36,
        OPEN_SLOTS = 37
    };

protected:
    void paint(QPainter * painter, const QStyleOptionViewItem & option,
            const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem & option,
            const QModelIndex & index) const;

private:
    QBrush getBrush(int openSlots) const;
};

#endif	/* MULTILINEDELEGATE_H */

