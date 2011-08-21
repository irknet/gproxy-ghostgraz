#ifndef SLOT_H
#define	SLOT_H

#include <QObject>
#include <QMetaType>
#include <QString>

#include "data/Player.h"

class Slot : public QObject
{
    Q_OBJECT
    Q_ENUMS(SlotsStatus)
    Q_ENUMS(ComputerStatus)
    Q_ENUMS(ComputerType)
    Q_ENUMS(SlotColor)
    Q_ENUMS(Race)

public:
    Slot();
    Slot(const Slot& orig);
    Slot& operator= (const Slot& orig);
    virtual ~Slot();

    // <editor-fold defaultstate="collapsed" desc="Enums">

    enum SlotStatus
    {
        OPEN = 0,
        CLOSED = 1,
        OCCUPIED = 2
    };

    enum ComputerStatus
    {
        NO_COMPUTER = 0,
        COMPUTER = 1
    };

    enum ComputerType
    {
        EASY = 0,
        NORMAL = 1,
        HARD = 2
    };

    enum SlotColor
    {
        RED = 0,
        BLUE = 1,
        TEAL = 2,
        PURPLE = 3,
        YELLOW = 4,
        ORANGE = 5,
        GREEN = 6,
        PINK = 7,
        GRAY = 8,
        LIGHT_BLUE = 9,
        DARK_GREEN = 10,
        BROWN = 11
    };

    enum Race
    {
        HUMAN = 0x01,
        ORC = 0x02,
        NIGHT_ELF = 0x04,
        UNDEAD = 0x08,
        RANDOM = 0x20
    };
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Getters and setters">
    SlotColor getColor() const;
    void setColor(const SlotColor& color);
    ComputerStatus getComputerStatus() const;
    void setComputerStatus(const ComputerStatus& computerStatus);
    ComputerType getComputerType() const;
    void setComputerType(const ComputerType& computerType);
    unsigned char getDownloadProgress() const;
    void setDownloadProgress(const unsigned char& downloadProgress);
    unsigned char getHandicap() const;
    void setHandicap(const unsigned char& handicap);
    Player* getPlayer() const;
    void setPlayer(Player *player);
    unsigned char getPlayerId() const;
    void setPlayerId(const unsigned char& playerId);
    Race getRace() const;
    void setRace(const Race& race);
    SlotStatus getSlotStatus() const;
    void setSlotStatus(const SlotStatus& slotStatus);
    unsigned char getTeam() const;
    void setTeam(const unsigned char& team);
    // </editor-fold>

private:
    unsigned char playerId;
    unsigned char downloadProgress;
    SlotStatus slotStatus;
    ComputerStatus computerStatus;
    unsigned char team;
    SlotColor color;
    Race race;
    ComputerType computerType;
    unsigned char handicap;
    Player *player;
};

Q_DECLARE_METATYPE(Slot)

#endif	/* SLOT_H */

