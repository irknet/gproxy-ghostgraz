#include "DownloadThread.h"

DownloadThread::DownloadThread (MainGUI *p_mainGUI)
{
    mainGUI = p_mainGUI;
    url = QUrl("http://0.static.ghostgraz.com/currentgames.txt");
}

DownloadThread::~DownloadThread () { }

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

    mainGUI->clearGamelist();

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
            mainGUI->addGame(botname, gamename, openSlots);
        }
    }
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