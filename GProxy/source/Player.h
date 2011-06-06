#ifndef PLAYER_H
#define	PLAYER_H

#include <QString>

class Player
{
public:
    Player();
    Player(const QString &name);
    virtual ~Player();

    double getStayPercent();

private:
    int playerId;
    QString name;
    int kills;
    int deaths;
    int assits;
    double killDeathRatio;
    int gamesPlayed;
    int wins;
    int losses;
    double winPercent;
    double score;
    int creepKills;
    int creepDenies;
    int towerKills;
    int raxKills;
    int courierKills;

public:
    // Start getters and setters
    int getPlayerId();
    void setPlayerId(int playerId);
    int getAssits();
    void setAssits(int assits);
    int getCourierKills();
    void setCourierKills(int courierKills);
    int getCreepDenies();
    void setCreepDenies(int creepDenies);
    int getCreepKills();
    void setCreepKills(int creepKills);
    int getDeaths();
    void setDeaths(int deaths);
    int getGamesPlayed();
    void setGamesPlayed(int gamesPlayed);
    double getKillDeathRatio();
    void setKillDeathRatio(double killDeathRatio);
    int getKills();
    void setKills(int kills);
    QString getName();
    void setName(const QString &name);
    int getRaxKills();
    void setRaxKills(int raxKills);
    double getScore();
    void setScore(double score);
    int getTowerKills();
    void setTowerKills(int towerKills);
    int getWins();
    void setWins(int wins);
    int getLosses();
    void setLosses(int losses);
    double getWinPercent();
    void setWinPercent(double winPercent);
    // End getters and setters
};

#endif	/* PLAYER_H */

