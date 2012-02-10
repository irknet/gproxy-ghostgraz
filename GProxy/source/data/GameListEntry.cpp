#include "GameListEntry.h"

GameListEntry::GameListEntry() { }

GameListEntry::GameListEntry(const QString& hostname, const QString& gamename, const int& openSlotCount)
{
    this->setHostname(hostname);
    this->setGamename(gamename);
    this->setOpenSlotCount(openSlotCount);
}

GameListEntry::GameListEntry(const GameListEntry& orig)
{
    this->setHostname(orig.getHostname());
    this->setGamename(orig.getGamename());
    this->setOpenSlotCount(orig.getOpenSlotCount());
}

GameListEntry& GameListEntry::operator= (const GameListEntry& orig)
{
    if (this == &orig)
    {
        return *this;
    }

    this->setHostname(orig.getHostname());
    this->setGamename(orig.getGamename());
    this->setOpenSlotCount(orig.getOpenSlotCount());

    return *this;
}

GameListEntry::~GameListEntry() { }

// <editor-fold defaultstate="collapsed" desc="Getters and setters">
QString GameListEntry::getGamename() const
{
    return gamename;
}

void GameListEntry::setGamename(const QString& gamename)
{
    this->gamename = gamename;
}

QString GameListEntry::getHostname() const
{
    return hostname;
}

void GameListEntry::setHostname(const QString& hostname)
{
    this->hostname = hostname;
}

int GameListEntry::getOpenSlotCount() const
{
    return openSlotCount;
}

void GameListEntry::setOpenSlotCount(const int& openSlotCount)
{
    this->openSlotCount = openSlotCount;
}
// </editor-fold>
