#ifndef STATSPAGE_H
#define	STATSPAGE_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringList>

#include "data/Player.h"

class Statspage : public QObject
{
    Q_OBJECT

public:
    Statspage();
    virtual ~Statspage();

    void login(const QString &username, const QString &password);
    bool isLoggedIn();
    void getPlayerInformation(const QString &playerName);
    void getAdminlist();

signals:
    void loginFinished();
    void playerInformationReplyFinished(Player *);
    void adminlistReplyFinished(QStringList);

private:
    QNetworkAccessManager *manager;
    QString username;
    QString password;
    bool loggedIn;

    QString getInfo(const QString &content, const QString &infoText);
    void onPlayerReplyFinished(QNetworkReply *);
    void onAdminlistReplyFinished(QNetworkReply *);
    
private slots:
    void forumReplyFinished(QNetworkReply *);
    void loginReplyFinished(QNetworkReply *);
    void replyFinished(QNetworkReply *);
};

#endif	/* STATSPAGE_H */

