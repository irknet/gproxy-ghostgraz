#ifndef STATSPAGE_H
#define	STATSPAGE_H

#include "Player.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

class Statspage : public QObject
{
    Q_OBJECT

public:
    Statspage();
    virtual ~Statspage();

    void login(const QString &username, const QString &password);
    bool isLoggedIn();

    void getPlayerInformation(const QString &playerName);

signals:
    void loginFinished();
    void receivedPlayerInformation(Player *);

private:
    QNetworkAccessManager *manager;
    QString username;
    QString password;
    bool loggedIn;

    QString getInfo(const QString &content, const QString &infoText);

private slots:
    void forumReplyFinished(QNetworkReply *);
    void loginReplyFinished(QNetworkReply *);
    void playerReplyFinished(QNetworkReply *);
};

#endif	/* STATSPAGE_H */

