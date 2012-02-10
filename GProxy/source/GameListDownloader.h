#ifndef GAMELISTDOWNLOADER_H
#define	GAMELISTDOWNLOADER_H

#include "data\GameListEntry.h"

#include <QString>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

class GameListDownloader : public QObject
{
    Q_OBJECT
    
public:
    GameListDownloader(const QString& gameListUrl);
    virtual ~GameListDownloader();
    
    void startDownloadInterval(const int& millis);
    
public slots:
    void startDownload();
    
private:
    QUrl gameListUrl;
    QNetworkAccessManager* networkAccessManager;
    QTimer* downloadTimer;
    bool downloadInProgress;
    
private slots:
    void onDownloadFinished(QNetworkReply* reply);

signals:
    void downloadFinished(QList<GameListEntry> gameList);
};

#endif	/* GAMELISTDOWNLOADER_H */

