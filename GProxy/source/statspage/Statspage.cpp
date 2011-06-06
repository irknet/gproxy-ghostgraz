#include "Statspage.h"

Statspage::Statspage ()
{
    manager = new QNetworkAccessManager();
}

Statspage::~Statspage ()
{
    manager->deleteLater();
}

void Statspage::getPlayerInformation (const QString& playerName)
{
    if (loggedIn)
    {
        QNetworkRequest statsRequest(QUrl("http://forum.ghostgraz.com/stats/?u="
                + playerName + "&s=datetime&o=desc&p=user&n=0"));

        manager->get(statsRequest);
    }
}

void Statspage::login (const QString &username, const QString &password)
{
    loggedIn = false;

    this->username = username;
    this->password = password;

    QNetworkRequest forumRequest(QUrl("http://forum.ghostgraz.com/"));
    manager->get(forumRequest);

    connect(manager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(forumReplyFinished(QNetworkReply *)));
}

void Statspage::forumReplyFinished (QNetworkReply *reply)
{
    disconnect(manager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(forumReplyFinished(QNetworkReply *)));

    QByteArray parameters;
    parameters.append("user=" + username + "&");
    parameters.append("passwrd=" + password + "&");
    parameters.append("cookielength=9999&");
    parameters.append("cookieneverexp=on");

    QNetworkRequest loginRequest(QUrl("http://forum.ghostgraz.com/login2/"));
    manager->post(loginRequest, parameters);

    connect(manager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(loginReplyFinished(QNetworkReply *)));

    reply->deleteLater();
}

void Statspage::loginReplyFinished (QNetworkReply *reply)
{
    disconnect(manager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(loginReplyFinished(QNetworkReply *)));

    if (QString(reply->readAll()).isEmpty())
    {
        loggedIn = true;
    }

    connect(manager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(playerReplyFinished(QNetworkReply *)));

    reply->deleteLater();

    emit loginFinished();
}

void Statspage::playerReplyFinished (QNetworkReply *reply)
{
    Player *player = new Player();

    QString content = QString(reply->readAll());

    if (content.contains("List of users matching")
            || content.contains("Welcome to the GhostGraz Stats Page"))
    {
        return;
    }

    QString url = reply->url().toString();
    int userIndex = url.indexOf("u=") + 2;
    int userEndIndex = url.indexOf("&", userIndex);
    player->setName(url.mid(userIndex, userEndIndex - userIndex));
    player->setKills(getInfo(content, "Kills:").toInt());
    player->setDeaths(getInfo(content, "Deaths:").toInt());
    player->setAssits(getInfo(content, "Assists:").toInt());
    player->setKillDeathRatio(getInfo(content, "Kills/Deaths:").toDouble());
    player->setGamesPlayed(getInfo(content, "Games:").toInt());
    QString winsLosses = getInfo(content, "Wins/Losses:");
    player->setWins(winsLosses.left(winsLosses.indexOf("/")).toInt());
    player->setLosses(winsLosses.mid(winsLosses.indexOf("/") + 1).toInt());
    player->setScore(getInfo(content, "Score:").toDouble());
    player->setWinPercent(getInfo(content, "Win Percent:").toDouble());
    player->setCreepKills(getInfo(content, "Creep Kills:").toInt());
    player->setCreepDenies(getInfo(content, "Creep Denies:").toInt());
    player->setTowerKills(getInfo(content, "Tower Kills:").toInt());
    player->setRaxKills(getInfo(content, "Rax Kills:").toInt());
    player->setCourierKills(getInfo(content, "Courier Kills:").toInt());

    reply->deleteLater();

    emit playerInformationReplyFinished(player);
}

QString Statspage::getInfo (const QString &content, const QString &infoText)
{
    int infoTextLabel = content.indexOf(infoText);
    int infoTdIndex = content.indexOf("<td>", infoTextLabel);
    int infoTdEndIndex = content.indexOf("</td>", infoTdIndex);

    return content.mid(infoTdIndex + 4, infoTdEndIndex - infoTdIndex - 4);
}

bool Statspage::isLoggedIn ()
{
    return loggedIn;
}