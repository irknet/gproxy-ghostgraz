#include "thread/DownloadThread.h"

const static int updateInterval = 5;

DownloadThread::DownloadThread (CGProxy* gproxy)
{
    this->gproxy = gproxy;
    stopped = false;
    url = QUrl("http://0.static.ghostgraz.com/currentgames.txt");
    manager = new QNetworkAccessManager(this);
}

DownloadThread::~DownloadThread ()
{
    manager->deleteLater();
}

void DownloadThread::refresh ()
{
    QNetworkRequest request(url);
    manager->get(request);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(downloadFinished(QNetworkReply*)), Qt::QueuedConnection);
}

void DownloadThread::run ()
{
    while (true)
    {
        while (gproxy->m_GameStarted)
        {
            for (int i = 0; i < updateInterval; i++)
            {
                if(stopped)
                {
                    return;
                }
                else
                {
                    this->sleep(1);
                }
            }
        }

        QEventLoop loop;
        QNetworkRequest request(url);
        manager->get(request);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(downloadFinished(QNetworkReply*)), Qt::QueuedConnection);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                &loop, SLOT(quit()), Qt::QueuedConnection);
        loop.exec();

        for (int i = 0; i < updateInterval; i++)
        {
            if(stopped)
            {
                return;
            }
            else
            {
                this->sleep(1);
            }
        }
    }
}

void DownloadThread::downloadFinished (QNetworkReply *reply)
{
    if (reply->error())
    {
        return;
    }

    QStringList lines = QString(reply->readAll()).split(QRegExp("\n"));

    if (lines.count() < 3)
    {
        // Game list is empty
        return;
    }

    QVector<QStringList> gamelist;

    for (QStringList::Iterator it = lines.begin(); it != lines.end(); ++it)
    {
        if (QString(*it).startsWith("#"))
        {
            // comment line
            continue;
        }

        QStringList splitted = QString(*it).split(QRegExp(","));

        if (splitted.count() == 3)
        {
            QString botname = splitted.at(0);
            QString gamename = splitted.at(1);
            QString openSlots = splitted.at(2);
            gamelist.append((QStringList() << botname << gamename << openSlots));
        }
    }

//    QVector<QStringList> vSortedGamelist = sortGamelist(gamelist);
    emit signal_clearGamelist();

    foreach(QStringList game, gamelist)
    {
        emit signal_addGame(game.at(0), game.at(1), game.at(2));
    }
}

//QVector<QStringList> DownloadThread::sortGamelist (QVector<QStringList> vGamelist)
//{
//    return vGamelist;
//
//    QVector<QStringList> vSortedGamelist;
//
//    foreach(QString bot, vBotorder)
//    {
//        foreach(QStringList game, vGamelist)
//        {
//            if (game.at(0) == bot)
//            {
//                vSortedGamelist.append((QStringList() << game.at(1)
//                        << game.at(1) << game.at(2)));
//                break;
//            }
//        }
//    }
//
//    return vSortedGamelist;
//}

void DownloadThread::stop ()
{
    stopped = true;
    this->wait(updateInterval * 1000);
}