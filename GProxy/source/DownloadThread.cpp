#include "DownloadThread.h"

DownloadThread::DownloadThread (MainGUI *p_mainGUI)
{
    mainGUI = p_mainGUI;
    url = QUrl("http://0.static.ghostgraz.com/currentgames.txt");

    QObject::connect(this, SIGNAL(signal_clearGamelist()),
            mainGUI, SLOT(clearGamelist()), Qt::QueuedConnection);

    QObject::connect(this, SIGNAL(signal_addGame(QString, QString, QString)),
            mainGUI, SLOT(addGame(QString, QString, QString)), Qt::QueuedConnection);
}

DownloadThread::~DownloadThread ()
{
    this->exit(0);
}

void DownloadThread::refresh()
{
    QEventLoop loop;
    QNetworkRequest request(url);
    reply = manager.get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}

void DownloadThread::run ()
{
    while (true)
    {
        while (mainGUI->getGproxy()->m_BNET->GetInGame())
        {
            this->sleep(5);
        }

        QEventLoop loop;
        QNetworkRequest request(url);
        reply = manager.get(request);
        connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        this->sleep(5);
    }
}

void DownloadThread::downloadFinished ()
{
    if (reply->error())
    {
        return;
    }

    QStringList lines = QString(reply->readAll()).split(QRegExp("\n"));
    if(lines.count() < 3)
    {
        // Game list is empty
        return;
    }

    emit signal_clearGamelist();

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
            emit signal_addGame(botname, gamename, openSlots);
        }
    }
}