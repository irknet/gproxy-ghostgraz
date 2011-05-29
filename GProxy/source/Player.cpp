#include "Player.h"

Player::Player () { }

Player::Player(const QString& name)
{
    this->name = name;
}

Player::~Player () { }

double Player::getStayPercent()
{
    if(gamesPlayed == 0)
    {
        return 0;
    }
    else
    {
        return (double) ((wins + losses) * 100) / gamesPlayed;
    }
}

int Player::getAssits ()
{
    return assits;
}

void Player::setAssits (int assits)
{
    this->assits = assits;
}

int Player::getCourierKills ()
{
    return courierKills;
}

void Player::setCourierKills (int courierKills)
{
    this->courierKills = courierKills;
}

int Player::getCreepDenies ()
{
    return creepDenies;
}

void Player::setCreepDenies (int creepDenies)
{
    this->creepDenies = creepDenies;
}

int Player::getCreepKills ()
{
    return creepKills;
}

void Player::setCreepKills (int creepKills)
{
    this->creepKills = creepKills;
}

int Player::getDeaths ()
{
    return deaths;
}

void Player::setDeaths (int deaths)
{
    this->deaths = deaths;
}

int Player::getGamesPlayed ()
{
    return gamesPlayed;
}

void Player::setGamesPlayed (int gamesPlayed)
{
    this->gamesPlayed = gamesPlayed;
}

double Player::getKillDeathRatio ()
{
    return killDeathRatio;
}

void Player::setKillDeathRatio (double killDeathRatio)
{
    this->killDeathRatio = killDeathRatio;
}

int Player::getKills ()
{
    return kills;
}

void Player::setKills (int kills)
{
    this->kills = kills;
}

QString Player::getName ()
{
    return name;
}

void Player::setName (const QString &name)
{
    this->name = name;
}

int Player::getRaxKills ()
{
    return raxKills;
}

void Player::setRaxKills (int raxKills)
{
    this->raxKills = raxKills;
}

double Player::getScore ()
{
    return score;
}

void Player::setScore (double score)
{
    this->score = score;
}

int Player::getTowerKills ()
{
    return towerKills;
}

void Player::setTowerKills (int towerKills)
{
    this->towerKills = towerKills;
}

int Player::getWins ()
{
    return wins;
}

void Player::setWins (int wins)
{
    this->wins = wins;
}

int Player::getLosses ()
{
    return losses;
}

void Player::setLosses (int losses)
{
    this->losses = losses;
}

double Player::getWinPercent ()
{
    return winPercent;
}

void Player::setWinPercent (double winPercent)
{
    this->winPercent = winPercent;
}