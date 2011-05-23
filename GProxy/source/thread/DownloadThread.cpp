#include "thread/DownloadThread.h"

DownloadThread::DownloadThread (MainGUI *p_mainGUI)
{
    mainGUI = p_mainGUI;
    url = QUrl("http://0.static.ghostgraz.com/currentgames.txt");

    QObject::connect(this, SIGNAL(signal_clearGamelist()),
            mainGUI, SLOT(clearGamelist()), Qt::QueuedConnection);

    QObject::connect(this, SIGNAL(signal_addGame(QString, QString, QString)),
            mainGUI, SLOT(addGame(QString, QString, QString)), Qt::QueuedConnection);

    QString botorder = mainGUI->getGproxy()->getConfig()->getString("botorder");
    if (botorder.isEmpty())
    {
        return;
    }

    QStringList botorderList = botorder.split(";");

    foreach(QString bot, botorderList)
    {
        vBotorder.append(bot);
    }
}

DownloadThread::~DownloadThread ()
{
    delete reply;
}

void DownloadThread::refresh ()
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
        while (mainGUI->getGproxy()->m_GameStarted)
        {
            this->sleep(5);
        }

        QEventLoop loop;
        QNetworkRequest request(url);
        reply = manager.get(request);
        connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()), Qt::QueuedConnection);
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()), Qt::QueuedConnection);
        loop.exec();

        this->sleep(5);
    }
}

void DownloadThread::downloadFinished ()
{
    if (reply->error())
    {
        reply = NULL;
        return;
    }

    QStringList lines = QString(reply->readAll()).split(QRegExp("\n"));
    reply = NULL;

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

    QVector<QStringList> vSortedGamelist = sortGamelist(gamelist);
    emit signal_clearGamelist();

    foreach(QStringList game, vSortedGamelist)
    {
        emit signal_addGame(game.at(0), game.at(1), game.at(2));
    }
}

QVector<QStringList> DownloadThread::sortGamelist (QVector<QStringList> vGamelist)
{
    if (vBotorder.isEmpty())
    {
        return vGamelist;
    }

    QVector<QStringList> vSortedGamelist;

    foreach(QString bot, vBotorder)
    {

        foreach(QStringList game, vGamelist)
        {
            if (game.at(0) == bot)
            {
                vSortedGamelist.append((QStringList() << game.at(1)
                        << game.at(1) << game.at(2)));
                break;
            }
        }
    }

    return vSortedGamelist;
}

void DownloadThread::stop ()
{
    this->terminate();
    this->wait(5000);
}