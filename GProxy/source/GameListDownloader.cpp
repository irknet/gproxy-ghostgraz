#include "GameListDownloader.h"

#include <QNetworkRequest>
#include <QList>
#include <QStringList>

/** Constructor. */
GameListDownloader::GameListDownloader(const QString& gameListUrl)
{
    this->gameListUrl = QUrl(gameListUrl);
    networkAccessManager = new QNetworkAccessManager();
    downloadTimer = new QTimer();
    downloadInProgress = false;

    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onDownloadFinished(QNetworkReply*)), Qt::QueuedConnection);
    connect(downloadTimer, SIGNAL(timeout()),
            this, SLOT(startDownload()), Qt::QueuedConnection);
}

/** Deconstructor. */
GameListDownloader::~GameListDownloader()
{
    networkAccessManager->deleteLater();
    
    downloadTimer->stop();
    delete downloadTimer;
    downloadTimer = NULL;
}

/**
 * Starts the gamelist download interval.
 * Use the signal downloadFinished(QList&lt;GameListEntry&gt; gameList) 
 * to get the downloaded gamelist.
 * 
 * @param millis The download interval in miiliseconds.
 */
void GameListDownloader::startDownloadInterval(const int& millis)
{
    startDownload();
    downloadTimer->start(millis);
}

/** Starts a gamelist download, if no other download is currently in progress. */
void GameListDownloader::startDownload()
{
    if (downloadInProgress)
    {
        return;
    }
    
    QNetworkRequest request(gameListUrl);
    networkAccessManager->get(request);
    
    downloadInProgress = true;
}

/**
 * Slot: Executed when the gamelist download has finished.
 * 
 * @param reply The QNetworkReply.
 */
void GameListDownloader::onDownloadFinished(QNetworkReply* reply)
{
    downloadInProgress = false;
    
    if (reply->error())
    {
        // TODO: Log error
        reply->deleteLater();
        return;
    }
    
    QString content(reply->readAll());
    if (content.isEmpty()) {
        // TODO: Log error
        reply->deleteLater();
        return;
    }

    QStringList lines = content.split("\n");
    QList<GameListEntry> gameList;
    
    foreach (QString line, lines)
    {
        if (line.startsWith("#"))
        {
            // Comment line
            continue;
        }
        
        QStringList splittedEntryList = line.split(",");
        if (splittedEntryList.count() != 3)
        {
            // TODO: Log error
            continue;
        }
        
        GameListEntry entry;
        entry.setHostname(splittedEntryList.at(0));
        entry.setGamename(splittedEntryList.at(1));
        entry.setOpenSlotCount(splittedEntryList.at(2).toInt());
        
        gameList.append(entry);
    }
    
    emit downloadFinished(gameList);
    
    reply->deleteLater();
}
