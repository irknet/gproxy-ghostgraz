#ifndef GAMELISTENTRY_H
#define	GAMELISTENTRY_H

#include <QObject>
#include <QMetaType>
#include <QString>

class GameListEntry : public QObject
{
    Q_OBJECT
    
public:
    GameListEntry();
    GameListEntry(const QString& hostname, const QString& gamename, const int& openSlotCount);
    GameListEntry(const GameListEntry& orig);
    GameListEntry& operator= (const GameListEntry& orig);
    virtual ~GameListEntry();

    // <editor-fold defaultstate="collapsed" desc="Getters and setters">
    QString getGamename() const;
    void setGamename(const QString& gamename);
    QString getHostname() const;
    void setHostname(const QString& hostname);
    int getOpenSlotCount() const;
    void setOpenSlotCount(const int& openSlotCount);
    // </editor-fold>

private:
    QString hostname;
    QString gamename;
    int openSlotCount;
};

Q_DECLARE_METATYPE(GameListEntry)

#endif	/* GAMELISTENTRY_H */

