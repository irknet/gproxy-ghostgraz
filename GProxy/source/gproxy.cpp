#include <QApplication>
#include <QSound>
#include <QElapsedTimer>
#include <QDateTime>
#include <QTextStream>
#include <QTextCodec>

#include "gproxy.h"
#include "util.h"
#include "socket.h"
#include "commandpacket.h"
#include "bnetprotocol.h"
#include "bnet.h"
#include "gameprotocol.h"
#include "gpsprotocol.h"
#include "MainGUI.h"
#include "Config.h"
#include "ConfigGUI.h"
#include "thread/DownloadThread.h"
#include "thread/GproxyUpdateThread.h"
#include "thread/SleepThread.h"
#include "GhostGrazLogininformationDialog.h"

#include <signal.h>
#include <stdlib.h>
#include <direct.h>
#include <sstream>
#include <windows.h>
#include <winsock.h>
//#include "mysql/include/mysql.h" // has to be after winsock

CGProxy* gproxy;
MainGUI* mainGUI;
DownloadThread* downloadThread;
GproxyUpdateThread* gproxyUpdateThread;
QElapsedTimer timer;
QFile* logFile;

vector<CIncomingSlots*> slotList;
//CPlayer players[12];
bool playerLoadingComplete[12];
int slotListWithPlayer;
int finishedLoadingCounter;
int teamNumber;
int teams;
unsigned int countdownEndTime;
//MYSQL *conn;
//MYSQL_RES *res;
//MYSQL_ROW row;

string parseTextline (string input)
{
    if (gproxy->m_LocalSocket && (input.find("$host$") != string::npos))
    {
        input.replace(input.find("$host$"), 6, gproxy->m_HostName);
    }

    return input;
};

/**
 * Returns the elapsed seconds since GProxy start.
 *
 * @return Elapsed seconds since GProxy start.
 */
unsigned long getElapsedSeconds ()
{
    return timer.elapsed() / 1000;
}

/**
 * Returns the elapsed milliseconds since GProxy start.
 *
 * @return Elapsed milliseconds since GProxy start.
 */
unsigned long getElapsedMilliseconds ()
{
    return timer.elapsed();
}

/**
 * Prints the logMessage to the gproxy log file, if logging is enabled.
 *
 * @param logMessage The log message.
 */
void LOG_Print (const QString& logMessage)
{
    // If logging is disabled the log file is closed.
    if (logFile->isOpen() && logFile->isWritable())
    {
        QTextStream log(logFile);
        QString dateTime = QLocale().toString(QDateTime::currentDateTime(),
                "[dddd dd-MM-yyyy hh:mm:ss] ");
        log << dateTime << logMessage << "\n";
        log.flush();
    }
}

void CheckForGame (string gamename)
{
    if (getautosearch())
    {
        gproxy->m_BNET->SetSearchGameName(gamename); //old one, but it works..
        CONSOLE_Print("[Phyton] Searching for [" + QString::fromStdString(gamename) + "].", true);
        gproxy->autosearch = false;
    }
}

/**
 * Phyton autospoofcheck
 */
void Pspoofcheck ()
{
    if (gproxy->m_BNET->GetLoggedIn())
    {
        gproxy->m_BNET->QueueChatCommand("spoofcheck", gproxy->m_HostName, true);
    }
}

/**
 * Displays the message at the output area of GProxy.
 *
 * @param message The message to be displayed.
 * @param log If the message should be logged. Default: <code>true</code>.
 */
void CONSOLE_Print (QString message, bool log)
{
    gproxy->addMessage(message, log);
}

/**
 * The main function.
 *
 * @param argc Number of command line arguments.
 * @param argv Command line arguments.
 * @return The exit code.
 */
int main (int argc, char** argv)
{
    QApplication app(argc, argv);

    gproxy = new CGProxy();

    mainGUI = new MainGUI(gproxy);
    mainGUI->show();

    qRegisterMetaType<vector<CIncomingSlots*> >("vector<CIncomingSlots*>");

    QObject::connect(gproxy, SIGNAL(signal_addMessage(QString, bool)),
            mainGUI, SLOT(addMessage(QString, bool)), Qt::QueuedConnection);

    QObject::connect(gproxy, SIGNAL(signal_changeChannel(QString)),
            mainGUI, SLOT(changeChannel(QString)), Qt::QueuedConnection);

    QObject::connect(gproxy, SIGNAL(signal_addChannelUser(QString, QString)),
            mainGUI, SLOT(addChannelUser(QString, QString)), Qt::QueuedConnection);

    QObject::connect(gproxy, SIGNAL(signal_removeChannelUser(QString)),
            mainGUI, SLOT(removeChannelUser(QString)), Qt::QueuedConnection);

    QObject::connect(gproxy, SIGNAL(signal_clearFriendlist()),
            mainGUI, SLOT(clearFriendlist()), Qt::QueuedConnection);

    QObject::connect(gproxy, SIGNAL(signal_addFriend(QString, bool, QString)),
            mainGUI, SLOT(addFriend(QString, bool, QString)), Qt::QueuedConnection);

    QObject::connect(gproxy, SIGNAL(signal_setGameslots(vector<CIncomingSlots*>)),
            mainGUI, SLOT(setGameslots(vector<CIncomingSlots*>)), Qt::QueuedConnection);

    QObject::connect(gproxy, SIGNAL(signal_showErrorMessage(QString)),
            mainGUI, SLOT(showErrorMessage(QString)), Qt::QueuedConnection);

    QObject::connect(gproxy, SIGNAL(signal_playerJoined(const QString &)),
            mainGUI, SLOT(playerJoined(const QString &)), Qt::QueuedConnection);

    QLocale::setDefault(QLocale::English);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    timer.start();

    CONSOLE_Print("[GPROXY] starting up");
    CONSOLE_Print("[GPROXY] Trying to loading configuration file");

    Config *config = new Config("gproxy.cfg");
    gproxy->setConfig(config);

    int statusCode = config->loadConfig();
    bool status = gproxy->checkStatus(statusCode);

    mainGUI->init();

    gproxy->applyConfig();

#ifdef WIN32
    // initialize winsock
    CONSOLE_Print("[GPROXY] starting winsock");
    WSADATA wsadata;

    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
    {
        CONSOLE_Print("[GPROXY] error starting winsock");
        return 1;
    }

    // increase process priority
    CONSOLE_Print("[GPROXY] setting process priority to \"above normal\"");
    SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
#endif

    CONSOLE_Print("", false);
    CONSOLE_Print("  Welcome to GProxy++.", false);
    CONSOLE_Print("  Server: " + gproxy->getServer(), false);
    CONSOLE_Print("  Username: " + gproxy->getUsername(), false);
    CONSOLE_Print("  Channel: " + gproxy->getChannel(), false);
    CONSOLE_Print("", false);
    CONSOLE_Print("  Type /help at any time for help.", false);
    CONSOLE_Print("", false);
    CONSOLE_Print("Welcome to <font color=\"red\">GProxy++</font>, "
            "this mod is by <font color=\"darkgreen\">Phyton</font>, "
            "<font color=\"deeppink\">Pr0gm4n</font> and "
            "<font color=\"gold\">Manufactoring</font>.");

    gproxy->init("", "", true, false, status);

    return app.exec();
}

/**
 * Constructor.
 */
CGProxy::CGProxy () { }

/**
 * Initializes all variables.
 *
 * @param cpublic
 * @param cfilter
 * @param temp_displayautocreated
 * @param listing_current_games
 * @param status
 */
void CGProxy::init (string cpublic, string cfilter, bool temp_displayautocreated,
        bool listing_current_games, bool status)
{
    m_Version = mainGUI->windowTitle().remove(0, 7).toStdString();
    m_LocalServer = new CTCPServer();
    m_LocalSocket = NULL;
    m_RemoteSocket = new CTCPClient();
    m_RemoteSocket->SetNoDelay(true);
    m_UDPSocket = new CUDPSocket();
    m_UDPSocket->SetBroadcastTarget("127.0.0.1");
    m_GameProtocol = new CGameProtocol(this);
    m_GPSProtocol = new CGPSProtocol();
    m_TotalPacketsReceivedFromLocal = 0;
    m_TotalPacketsReceivedFromRemote = 0;
    m_Exiting = false;
    m_TFT = !(this->getCDKeyTFT().isEmpty());
    m_War3Path = this->getWar3Path().toStdString();
    m_CDKeyROC = this->getCDKeyROC().toStdString();
    m_CDKeyTFT = this->getCDKeyTFT().toStdString();
    m_LastConnectionAttemptTime = 0;
    m_LastRefreshTime = 0;
    m_RemoteServerPort = 0;
    m_GameIsReliable = false;
    m_GameStarted = false;
    m_LeaveGameSent = false;
    m_ActionReceived = false;
    m_Synchronized = true;
    m_ReconnectPort = 0;
    m_PID = 255;
    m_ChatPID = 255;
    m_ReconnectKey = 0;
    m_NumEmptyActions = 0;
    m_NumEmptyActionsUsed = 0;
    m_LastAckTime = 0;
    m_LastActionTime = 0;
    m_BNET = new CBNET(this, mainGUI, server.toStdString(), string(), 0, 0,
            "USA", "United States", username.toStdString(), password.toStdString(),
            channel.toStdString(), war3version, exeversion, exeversionhash,
            passwordhashtype.toStdString(), 200);
    m_LocalServer->Listen(string(), port);
    cfgfilterfirst = true; // Phyton filter
    cfgfilter = cfilter; // Phyton filter
    cfgpublic = cpublic; // Phyton public
    autosearch = true; //pr0 autosearch
    displayautocreated = temp_displayautocreated;
    m_listing_current_games = listing_current_games;
    testvar = false;
    m_PlaySound = true; // manu
    vShallCreate = false;
    vShallCreateQuiet = false;
    parrot = "";
    dotaMap = false;
    lastLeaver = new Player(username);

    downloadThread = new DownloadThread(mainGUI);
    downloadThread->start(QThread::LowestPriority);

    gproxyUpdateThread = new GproxyUpdateThread(gproxy);
    if (status == true)
    {
        gproxyUpdateThread->start(QThread::HighPriority);
    }

    CONSOLE_Print("[GPROXY] Customized GProxy++ Version " + QString::fromStdString(m_Version));
}

/**
 * Deconstructor
 */
CGProxy::~CGProxy ()
{
    for (vector<CIncomingGameHost* > ::iterator i = m_Games.begin(); i != m_Games.end(); i++)
    {
        m_UDPSocket->Broadcast(6112, m_GameProtocol->SEND_W3GS_DECREATEGAME(((CIncomingGameHost* ) (*i))->GetUniqueGameID()));
    }

    for (QVector<Player*> ::iterator i = players.begin(); i != players.end(); i++)
    {
        delete *i;
    }

    delete config;
    delete m_LocalServer;
    delete m_LocalSocket;
    delete m_RemoteSocket;
    delete m_UDPSocket;
    delete m_BNET;
    delete lastLeaver;

    for (vector<CIncomingGameHost*> ::iterator i = m_Games.begin(); i != m_Games.end(); i++)
    {
        delete *i;
    }

    delete m_GameProtocol;
    delete m_GPSProtocol;


    while (!m_LocalPackets.empty())
    {
        delete m_LocalPackets.front();
        m_LocalPackets.pop();
    }

    while (!m_RemotePackets.empty())
    {
        delete m_RemotePackets.front();
        m_RemotePackets.pop();
    }

    while (!m_PacketBuffer.empty())
    {
        delete m_PacketBuffer.front();
        m_PacketBuffer.pop();
    }
}

/**
 * Cleanup function before exiting.
 */
void CGProxy::cleanup ()
{
#ifdef WIN32
    CONSOLE_Print("[GPROXY] shutting down winsock");
    WSACleanup();
#endif

    CONSOLE_Print("[GPROXY] shutting down");
    downloadThread->stop();
    delete downloadThread;
    gproxyUpdateThread->stop();
    delete gproxyUpdateThread;

    for (vector<CIncomingSlots*> ::iterator it = slotList.begin(); it != slotList.end(); it++)
    {
        delete *it;
    }

    if (logFile->isOpen())
    {
        logFile->close();
    }
    delete logFile;

    delete mainGUI;
    delete gproxy;
}

/**
 * Initializes all variables from the config.
 */
void CGProxy::applyConfig ()
{
    logFile = new QFile("gproxy_log.txt");
    if (config->getBoolean("log"))
    {
        if (!logFile->open(QFile::Append | QFile::Text))
        {
            gproxy->showErrorMessage(logFile->errorString());
        }
    }

    gproxy->setWar3Path(config->getString("war3path"));
    gproxy->setCDKeyROC(config->getString("cdkeyroc"));
    gproxy->setCDKeyTFT(config->getString("cdkeytft"));
    gproxy->setServer(config->getString("server"));

    gproxy->setPrivategamename(config->getString("privategamename"));
    gproxy->setBotprefix(config->getString("botprefix"));
    cautosearch = config->getBoolean("autosearch");
    gproxy->m_PlaySound = config->getBoolean("sound");
    gproxy->setUsername(config->getString("username"));
    gproxy->setPassword(config->getString("password"));
    gproxy->setChannel(config->getString("channel"));
    gproxy->setWar3version(config->getInt("war3version"));
    gproxy->setPort(config->getInt("port"));
    gproxy->setExeversion(UTIL_ExtractNumbers(config->getString("exeversion").toStdString(), 4));
    gproxy->setExeversionhash(UTIL_ExtractNumbers(config->getString("exeversionhash").toStdString(), 4));
    gproxy->setPasswordhashtype(config->getString("passwordhashtype"));
}

/**
 * Emits the signal <code>signal_addMessage</code>.
 * Displays the message at the output area of GProxy.
 *
 * @param msg The message to be displayed.
 * @param log If the message should be logged. Default: <code>true</code>.
 * @see MainGUI#addMessage
 */
void CGProxy::addMessage (QString msg, bool log)
{
    emit signal_addMessage(msg, log);
}

/**
 * Emits the signal <code>signal_changeChannel</code>.
 *
 * @param channel
 * @see MainGUI#changeChannel
 */
void CGProxy::changeChannel (QString channel)
{
    this->m_GameStarted = false;
    emit signal_changeChannel(channel);
}

/**
 * Emits the signal <code>signal_addChannelUser</code>.
 *
 * @param username
 * @param clanTag
 * @see MainGUI#addChannelUser
 */
void CGProxy::addChannelUser (QString username, QString clanTag)
{
    // Phyton waitgame
    if (gproxy->getVShallCreate())
    {
        QString tname = username.toLower();
        QString tpre = gproxy->getBotprefix().toLower();

        if (gproxy->getBotprefix().size() > 0 && gproxy->getBotprefix()[0] != "^"[0])
            if (gproxy->getPrivategamename() != "" && tpre != "" && tname.mid(0, tpre.size()) == tpre)
            {
                gproxy->m_BNET->QueueChatCommand("/w " + username
                        + " !priv " + gproxy->getPrivategamename());
                autosearch = true;
                CheckForGame(gproxy->getPrivategamename().toStdString());
                if (!gproxy->getVShallCreateQuiet())
                {
                    gproxy->m_BNET->QueueChatCommand("gn: " + gproxy->getPrivategamename());
                }
                gproxy->setVShallCreate(false);
            }
            else;
        else
        {
            if (gproxy->getPrivategamename() != "" && tname == tpre.mid(1, tpre.size() - 1))
            {
                autosearch = true;
                gproxy->m_BNET->QueueChatCommand("/w " + username
                        + " !priv " + gproxy->getPrivategamename());
                CheckForGame(gproxy->getPrivategamename().toStdString());
                if (!gproxy->getVShallCreateQuiet())
                {
                    gproxy->m_BNET->QueueChatCommand("gn: " + gproxy->getPrivategamename());
                }
                gproxy->setVShallCreate(false);
            }
        }
    }

    emit signal_addChannelUser(username, clanTag);
}

/**
 * Emits the signal <code>removeChannelUser</code>.
 *
 * @param username
 * @see MainGUI#removeChannelUser
 */
void CGProxy::removeChannelUser (QString username)
{
    emit signal_removeChannelUser(username);
}

/**
 * Calls clearFriendlist() and addFriend(QString username, bool online, QString location) for every friend.
 *
 * @param friendList vector<CIncomingFriendList*>
 */
void CGProxy::friendUpdate (vector<CIncomingFriendList*> friendList)
{
    clearFriendlist();

    for (vector<CIncomingFriendList*> ::iterator it = friendList.begin();
            it != friendList.end(); it++)
    {
        addFriend(QString::fromStdString(((*it))->GetAccount()),
                (((*it))->GetArea() != 0),
                QString::fromStdString((*it)->GetLocation()));
    }
}

/**
 * Emits the signal <code>signal_clearFriendlist</code>.
 *
 * @see MainGUI#clearFriendlist
 */
void CGProxy::clearFriendlist ()
{
    emit signal_clearFriendlist();
}

/**
 * Emits the signal <code>signal_addFriend</code>.
 *
 * @see MainGUI#addFriend
 */
void CGProxy::addFriend (QString username, bool online, QString location)
{
    emit signal_addFriend(username, online, location);
}

/**
 * Emits the signal <code>signal_showErrorMessage</code>.
 *
 * @see MainGUI#showErrorMessage
 */
void CGProxy::showErrorMessage (QString errorMessage)
{
    emit signal_showErrorMessage(errorMessage);
}

bool CGProxy::Update (long usecBlock)
{
    unsigned int NumFDs = 0;

    // take every socket we own and throw it in one giant select statement so we can block on all sockets

    int nfds = 0;
    fd_set fd;
    fd_set send_fd;
    FD_ZERO(&fd);
    FD_ZERO(&send_fd);

    // 1. the battle.net socket

    NumFDs += m_BNET->SetFD(&fd, &send_fd, &nfds);

    // 2. the local server

    m_LocalServer->SetFD(&fd, &send_fd, &nfds);
    NumFDs++;

    // 3. the local socket

    if (m_LocalSocket)
    {
        m_LocalSocket->SetFD(&fd, &send_fd, &nfds);
        NumFDs++;
    }

    // 4. the remote socket

    if (!m_RemoteSocket->HasError() && m_RemoteSocket->GetConnected())
    {
        m_RemoteSocket->SetFD(&fd, &send_fd, &nfds);
        NumFDs++;
    }

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = usecBlock;

    struct timeval send_tv;
    send_tv.tv_sec = 0;
    send_tv.tv_usec = 0;

#ifdef WIN32
    select(1, &fd, NULL, NULL, &tv);
    select(1, NULL, &send_fd, NULL, &send_tv);
#else
    select(nfds + 1, &fd, NULL, NULL, &tv);
    select(nfds + 1, NULL, &send_fd, NULL, &send_tv);
#endif

    if (NumFDs == 0)
    {
        SleepThread::msleep(50);
    }

    if (m_BNET->Update(&fd, &send_fd))
        return true;

    //
    // accept new connections
    //

    CTCPSocket *NewSocket = m_LocalServer->Accept(&fd);

    if (NewSocket)
    {
        if (m_LocalSocket)
        {
            // someone's already connected, reject the new connection
            // we only allow one person to use the proxy at a time

            delete NewSocket;
        }
        else
        {
            CONSOLE_Print("[GPROXY] local player connected");
            m_LocalSocket = NewSocket;
            m_LocalSocket->SetNoDelay(true);
            m_TotalPacketsReceivedFromLocal = 0;
            m_TotalPacketsReceivedFromRemote = 0;
            m_GameIsReliable = false;
            m_GameStarted = false;
            m_LeaveGameSent = false;
            m_ActionReceived = false;
            m_Synchronized = true;
            m_ReconnectPort = 0;
            m_PID = 255;
            m_ChatPID = 255;
            m_ReconnectKey = 0;
            m_NumEmptyActions = 0;
            m_NumEmptyActionsUsed = 0;
            m_LastAckTime = 0;
            m_LastActionTime = 0;
            m_JoinedName.clear();
            m_HostName.clear();

            while (!m_PacketBuffer.empty())
            {
                delete m_PacketBuffer.front();
                m_PacketBuffer.pop();
            }
        }
    }

    if (m_LocalSocket)
    {
        //
        // handle proxying (reconnecting, etc...)
        //

        if (m_LocalSocket->HasError() || !m_LocalSocket->GetConnected())
        {
            CONSOLE_Print("[GPROXY] local player disconnected");

            if (m_BNET->GetInGame())
                m_BNET->QueueEnterChat();

            delete m_LocalSocket;
            m_LocalSocket = NULL;

            // ensure a leavegame message was sent, otherwise the server may wait for our reconnection which will never happen
            // if one hasn't been sent it's because Warcraft III exited abnormally

            if (m_GameIsReliable && !m_LeaveGameSent)
            {
                // note: we're not actually 100% ensuring the leavegame message is sent, we'd need to check that DoSend worked, etc...

                BYTEARRAY LeaveGame;
                LeaveGame.push_back(0xF7);
                LeaveGame.push_back(0x21);
                LeaveGame.push_back(0x08);
                LeaveGame.push_back(0x00);
                UTIL_AppendByteArray(LeaveGame, (uint32_t) PLAYERLEAVE_GPROXY, false);
                m_RemoteSocket->PutBytes(LeaveGame);
                m_RemoteSocket->DoSend(&send_fd);
            }

            m_RemoteSocket->Reset();
            m_RemoteSocket->SetNoDelay(true);
            m_RemoteServerIP.clear();
            m_RemoteServerPort = 0;
        }
        else
        {
            m_LocalSocket->DoRecv(&fd);
            ExtractLocalPackets();
            ProcessLocalPackets();

            if (!m_RemoteServerIP.empty())
            {
                if (m_GameIsReliable && m_ActionReceived
                        && getElapsedSeconds() - m_LastActionTime >= 60)
                {
                    if (m_NumEmptyActionsUsed < m_NumEmptyActions)
                    {
                        SendEmptyAction();
                        m_NumEmptyActionsUsed++;
                    }
                    else
                    {
                        SendLocalChat("GProxy++ ran out of time to reconnect, Warcraft III will disconnect soon.");
                        CONSOLE_Print("[GPROXY] ran out of time to reconnect");
                    }

                    m_LastActionTime = getElapsedSeconds();
                }

                if (m_RemoteSocket->HasError())
                {
                    CONSOLE_Print("[GPROXY] disconnected from remote server due to socket error");

                    if (m_GameIsReliable && m_ActionReceived && m_ReconnectPort > 0)
                    {
                        SendLocalChat("You have been disconnected from the server due to a socket error.");
                        uint32_t TimeRemaining = (m_NumEmptyActions
                                - m_NumEmptyActionsUsed + 1) * 60
                                - (getElapsedSeconds() - m_LastActionTime);

                        if (getElapsedSeconds() - m_LastActionTime
                                > (((unsigned int) m_NumEmptyActions)
                                - ((unsigned int) m_NumEmptyActionsUsed) + 1) * 60)
                        {
                            TimeRemaining = 0;
                        }

                        SendLocalChat("GProxy++ is attempting to reconnect... (" + QString::number(TimeRemaining) + " seconds remain)");
                        CONSOLE_Print("[GPROXY] attempting to reconnect");
                        m_RemoteSocket->Reset();
                        m_RemoteSocket->SetNoDelay(true);
                        m_RemoteSocket->Connect(string(), m_RemoteServerIP, m_ReconnectPort);
                        m_LastConnectionAttemptTime = getElapsedSeconds();
                    }
                    else
                    {
                        if (m_BNET->GetInGame())
                            m_BNET->QueueEnterChat();

                        m_LocalSocket->Disconnect();
                        delete m_LocalSocket;
                        m_LocalSocket = NULL;
                        m_RemoteSocket->Reset();
                        m_RemoteSocket->SetNoDelay(true);
                        m_RemoteServerIP.clear();
                        m_RemoteServerPort = 0;
                        return false;
                    }
                }

                if (!m_RemoteSocket->GetConnecting() && !m_RemoteSocket->GetConnected())
                {
                    CONSOLE_Print("[GPROXY] disconnected from remote server");

                    if (m_GameIsReliable && m_ActionReceived && m_ReconnectPort > 0)
                    {
                        SendLocalChat("You have been disconnected from the server.");
                        uint32_t TimeRemaining = (m_NumEmptyActions
                                - m_NumEmptyActionsUsed + 1) * 60
                                - (getElapsedSeconds() - m_LastActionTime);

                        if (getElapsedSeconds() - m_LastActionTime
                                > (((unsigned int) m_NumEmptyActions)
                                - ((unsigned int) m_NumEmptyActionsUsed) + 1) * 60)
                        {
                            TimeRemaining = 0;
                        }

                        SendLocalChat("GProxy++ is attempting to reconnect... (" + QString::number(TimeRemaining) + " seconds remain)");
                        CONSOLE_Print("[GPROXY] attempting to reconnect");
                        m_RemoteSocket->Reset();
                        m_RemoteSocket->SetNoDelay(true);
                        m_RemoteSocket->Connect(string(), m_RemoteServerIP, m_ReconnectPort);
                        m_LastConnectionAttemptTime = getElapsedSeconds();
                    }
                    else
                    {
                        if (m_BNET->GetInGame())
                            m_BNET->QueueEnterChat();

                        m_LocalSocket->Disconnect();
                        delete m_LocalSocket;
                        m_LocalSocket = NULL;
                        m_RemoteSocket->Reset();
                        m_RemoteSocket->SetNoDelay(true);
                        m_RemoteServerIP.clear();
                        m_RemoteServerPort = 0;
                        return false;
                    }
                }

                if (m_RemoteSocket->GetConnected())
                {
                    if (m_GameIsReliable && m_ActionReceived && m_ReconnectPort > 0
                            && getElapsedSeconds() - m_RemoteSocket->GetLastRecv() >= 20)
                    {
                        CONSOLE_Print("[GPROXY] disconnected from remote server due to 20 second timeout");
                        SendLocalChat("You have been timed out from the server.");
                        uint32_t TimeRemaining = (m_NumEmptyActions
                                - m_NumEmptyActionsUsed + 1) * 60
                                - (getElapsedSeconds() - m_LastActionTime);

                        if (getElapsedSeconds() - m_LastActionTime
                                > ((unsigned int) (m_NumEmptyActions
                                - m_NumEmptyActionsUsed + 1)) * 60)
                        {
                            TimeRemaining = 0;
                        }

                        SendLocalChat("GProxy++ is attempting to reconnect... (" + QString::number(TimeRemaining) + " seconds remain)");
                        CONSOLE_Print("[GPROXY] attempting to reconnect");
                        m_RemoteSocket->Reset();
                        m_RemoteSocket->SetNoDelay(true);
                        m_RemoteSocket->Connect(string(), m_RemoteServerIP, m_ReconnectPort);
                        m_LastConnectionAttemptTime = getElapsedSeconds();
                    }
                    else
                    {
                        m_RemoteSocket->DoRecv(&fd);
                        ExtractRemotePackets();
                        ProcessRemotePackets();

                        if (m_GameIsReliable && m_ActionReceived && m_ReconnectPort > 0
                                && getElapsedSeconds() - m_LastAckTime >= 10)
                        {
                            m_RemoteSocket->PutBytes(m_GPSProtocol->SEND_GPSC_ACK(m_TotalPacketsReceivedFromRemote));
                            m_LastAckTime = getElapsedSeconds();
                        }

                        m_RemoteSocket->DoSend(&send_fd);
                    }
                }

                if (m_RemoteSocket->GetConnecting())
                {
                    // we are currently attempting to connect

                    if (m_RemoteSocket->CheckConnect())
                    {
                        // the connection attempt completed

                        if (m_GameIsReliable && m_ActionReceived)
                        {
                            // this is a reconnection, not a new connection
                            // if the server accepts the reconnect request it will send a GPS_RECONNECT back requesting a certain number of packets

                            SendLocalChat("GProxy++ reconnected to the server!");
                            SendLocalChat("==================================================");
                            CONSOLE_Print("[GPROXY] reconnected to remote server");

                            // note: even though we reset the socket when we were disconnected, we haven't been careful to ensure we never queued any data in the meantime
                            // therefore it's possible the socket could have data in the send buffer
                            // this is bad because the server will expect us to send a GPS_RECONNECT message first
                            // so we must clear the send buffer before we continue
                            // note: we aren't losing data here, any important messages that need to be sent have been put in the packet buffer
                            // they will be requested by the server if required

                            m_RemoteSocket->ClearSendBuffer();
                            m_RemoteSocket->PutBytes(m_GPSProtocol->SEND_GPSC_RECONNECT(m_PID, m_ReconnectKey, m_TotalPacketsReceivedFromRemote));

                            // we cannot permit any forwarding of local packets until the game is synchronized again
                            // this will disable forwarding and will be reset when the synchronization is complete

                            m_Synchronized = false;
                        }
                        else
                            CONSOLE_Print("[GPROXY] connected to remote server");
                    }
                    else if (getElapsedSeconds() - m_LastConnectionAttemptTime >= 10)
                    {
                        // the connection attempt timed out (10 seconds)

                        CONSOLE_Print("[GPROXY] connect to remote server timed out");

                        if (m_GameIsReliable && m_ActionReceived && m_ReconnectPort > 0)
                        {
                            uint32_t TimeRemaining = (m_NumEmptyActions
                                    - m_NumEmptyActionsUsed + 1) * 60
                                    - (getElapsedSeconds() - m_LastActionTime);

                            if (getElapsedSeconds() - m_LastActionTime
                                    > ((unsigned int) (m_NumEmptyActions
                                    - m_NumEmptyActionsUsed + 1)) * 60)
                            {
                                TimeRemaining = 0;
                            }

                            SendLocalChat("GProxy++ is attempting to reconnect... (" + QString::number(TimeRemaining) + " seconds remain)");
                            CONSOLE_Print("[GPROXY] attempting to reconnect");
                            m_RemoteSocket->Reset();
                            m_RemoteSocket->SetNoDelay(true);
                            m_RemoteSocket->Connect(string(), m_RemoteServerIP, m_ReconnectPort);
                            m_LastConnectionAttemptTime = getElapsedSeconds();
                        }
                        else
                        {
                            if (m_BNET->GetInGame())
                                m_BNET->QueueEnterChat();

                            m_LocalSocket->Disconnect();
                            delete m_LocalSocket;
                            m_LocalSocket = NULL;
                            m_RemoteSocket->Reset();
                            m_RemoteSocket->SetNoDelay(true);
                            m_RemoteServerIP.clear();
                            m_RemoteServerPort = 0;
                            return false;
                        }
                    }
                }
            }

            m_LocalSocket->DoSend(&send_fd);
        }
    }
    else
    {
        //
        // handle game listing
        //

        if (getElapsedSeconds() - m_LastRefreshTime >= 2)
        {
            for (vector<CIncomingGameHost*> ::iterator i = m_Games.begin(); i != m_Games.end();)
            {
                // expire games older than 60 seconds *

                if (getElapsedSeconds() - ((CIncomingGameHost*) (*i))->GetReceivedTime() >= 60)
                {
                    // don't forget to remove it from the LAN list first

                    m_UDPSocket->Broadcast(6112, m_GameProtocol->SEND_W3GS_DECREATEGAME(((CIncomingGameHost*) (*i))->GetUniqueGameID()));
                    delete *i;
                    i = m_Games.erase(i);
                    continue;
                }

                BYTEARRAY MapGameType;
                UTIL_AppendByteArray(MapGameType, ((CIncomingGameHost*) (*i))->GetGameType(), false);
                UTIL_AppendByteArray(MapGameType, ((CIncomingGameHost*) (*i))->GetParameter(), false);
                BYTEARRAY MapFlags = UTIL_CreateByteArray(((CIncomingGameHost*) (*i))->GetMapFlags(), false);
                BYTEARRAY MapWidth = UTIL_CreateByteArray(((CIncomingGameHost*) (*i))->GetMapWidth(), false);
                BYTEARRAY MapHeight = UTIL_CreateByteArray(((CIncomingGameHost*) (*i))->GetMapHeight(), false);
                string GameName = ((CIncomingGameHost*) (*i))->GetGameName();

                // colour reliable game names so they're easier to pick out of the list

                if (((CIncomingGameHost*) (*i))->GetMapWidth() == 1984 &&
                        ((CIncomingGameHost*) (*i))->GetMapHeight() == 1984)
                {
                    if (GameName == gproxy->getPrivategamename().toStdString())
                    {
                        GameName = "|cFFFF0000" + GameName;
                    }
                    else if (((CIncomingGameHost*) (*i))->GetHostName().substr(0, 9) == "GhostGraz")
                    {
                        // Phyton ghostgrazgame orange
                        GameName = "|cFFFF9F40" + GameName;
                    }
                    else if (GameName.substr(0, 3) == "thr")
                    {
                        if (GameName.substr(0, 4) == "thr-")
                        {
                            GameName = "|cFF00FF00" + GameName;
                        }
                        else if (GameName.substr(0, 5) == "thr2-")
                        {
                            GameName = "|cFFCCCC00" + GameName;
                        }
                        else if (GameName.substr(0, 5) == "thr3-")
                        {
                            GameName = "|cFFFF0000" + GameName;
                        }
                    }
                    else
                    {
                        GameName = "|cFF4080C0" + GameName;
                    }

                    // unfortunately we have to truncate them
                    // is this acceptable?

                    if (GameName.size() > 31)
                        GameName = GameName.substr(0, 31);
                }

                m_UDPSocket->Broadcast(6112, m_GameProtocol->SEND_W3GS_GAMEINFO(m_TFT, war3version, MapGameType, MapFlags, MapWidth, MapHeight, GameName, (*i)->GetHostName(), (*i)->GetElapsedTime(), (*i)->GetMapPath(), (*i)->GetMapCRC(), 12, 12, port, (*i)->GetUniqueGameID(), (*i)->GetUniqueGameID()));
                i++;
            }

            m_LastRefreshTime = getElapsedSeconds();
        }
    }

    return m_Exiting;
}

void CGProxy::ExtractLocalPackets ()
{
    if (!m_LocalSocket)
        return;

    string *RecvBuffer = m_LocalSocket->GetBytes();
    BYTEARRAY Bytes = UTIL_CreateByteArray((unsigned char *) RecvBuffer->c_str(), RecvBuffer->size());

    // a packet is at least 4 bytes so loop as long as the buffer contains 4 bytes

    while (Bytes.size() >= 4)
    {
        // byte 0 is always 247

        if (Bytes[0] == W3GS_HEADER_CONSTANT)
        {
            // bytes 2 and 3 contain the length of the packet

            uint16_t Length = UTIL_ByteArrayToUInt16(Bytes, false, 2);

            if (Length >= 4)
            {
                if (Bytes.size() >= Length)
                {
                    // we have to do a little bit of packet processing here
                    // this is because we don't want to forward any chat messages that start with a "/" as these may be forwarded to battle.net instead
                    // in fact we want to pretend they were never even received from the proxy's perspective

                    bool Forward = true;
                    BYTEARRAY Data = BYTEARRAY(Bytes.begin(), Bytes.begin() + Length);

                    if (Bytes[1] == CGameProtocol::W3GS_CHAT_TO_HOST)
                    {
                        if (Data.size() >= 5)
                        {
                            unsigned int i = 5;
                            unsigned char Total = Data[4];

                            BYTEARRAY toPIDs = BYTEARRAY(Data.begin() + i, Data.begin() + i + Total);
                            //                            int FromPID = (int) Data[i];

                            if (Total > 0 && Data.size() >= i + Total)
                            {
                                i += Total;
                                unsigned char Flag = Data[i + 1];
                                i += 2;

                                BYTEARRAY ExtraFlags;
                                string MessageString;

                                if (Flag == 16)
                                {
                                    // Chat message
                                    BYTEARRAY Message = UTIL_ExtractCString(Data, i);
                                    MessageString = string(Message.begin(), Message.end());
                                }
                                else if (Flag == 32)
                                {
                                    // Extra flags
                                    ExtraFlags = BYTEARRAY(Data.begin() + i, Data.begin() + i + 4);
                                    BYTEARRAY Message = UTIL_ExtractCString(Data, i + 4);
                                    MessageString = string(Message.begin(), Message.end());
                                }

                                if (Flag == 16 || Flag == 32) // Chat message
                                {
                                    if (Flag == 16)
                                    {
                                        CONSOLE_Print("[LOBBY][" + username + "] " + QString::fromStdString(MessageString));
                                    }
                                    else // GameStarted
                                    {
                                        // ExtraFlags[0] stores the type of chat message as defined by Warcraft III.
                                        if (ExtraFlags[0] == 0) // 0 is an All message
                                        {
                                            CONSOLE_Print("[ALL][" + username + "] " + QString::fromStdString(MessageString));
                                        }
                                        else if (ExtraFlags[0] == 1) // 1 is an Allies message
                                        {
                                            CONSOLE_Print("[ALLY][" + username + "] " + QString::fromStdString(MessageString));
                                        }
                                        else if (ExtraFlags[0] == 2) // 2 is an Observer/Referee message
                                        {
                                            CONSOLE_Print("[OBSERVER][" + username + "] " + QString::fromStdString(MessageString));
                                        }
                                        else if (ExtraFlags[0] >= 3) // 3+ are private messages
                                        {
                                            // Based on limited testing it seems that
                                            // the extra flags' first byte contains 3 plus the recipient's colour to denote a private message.
                                            // Due to the fact that the Datapacket also contains the FromPID, we do not need to extract the color.

                                            CONSOLE_Print("[PRIVATE][TO][" + username + "] " + QString::fromStdString(MessageString));
                                        }
                                    }
                                }

                                Forward = CheckForwarding(QString::fromStdString(MessageString));
                            }
                        }
                    }

                    if (Forward)
                    {
                        m_LocalPackets.push(new CCommandPacket(W3GS_HEADER_CONSTANT, Bytes[1], Data));
                        m_PacketBuffer.push(new CCommandPacket(W3GS_HEADER_CONSTANT, Bytes[1], Data));
                        m_TotalPacketsReceivedFromLocal++;
                    }

                    *RecvBuffer = RecvBuffer->substr(Length);
                    Bytes = BYTEARRAY(Bytes.begin() + Length, Bytes.end());
                }
                else
                    return;
            }
            else
            {
                CONSOLE_Print("[GPROXY] received invalid packet from local player (bad length)");
                m_Exiting = true;
                return;
            }
        }
        else
        {
            CONSOLE_Print("[GPROXY] received invalid packet from local player (bad header constant)");
            m_Exiting = true;
            return;
        }
    }
}

bool CGProxy::CheckForwarding (QString message)
{
    bool forward = true;
    QString command = message.toLower();

    if (command.length() >= 1 && command.startsWith("/"))
    {
        forward = false;

        if (command.startsWith("/p ") || command.startsWith("/phrase "))
        {
            string filePath = "phrase/";

            if (command.startsWith("/p "))
            {
                filePath.append(command.mid(3).toStdString());
            }
            else
            {
                filePath.append(command.mid(8).toStdString());
            }

            if (!QString::fromStdString(filePath).endsWith(".txt"))
            {
                filePath.append(".txt");
            }

            char help[100];
            unsigned int i;
            for (i = 0; i < 100; i++)
                help[i] = NULL;
            for (i = 0; i < filePath.size(); i++)
                help[i] = filePath[i];
            fstream infile(help);
            //
            if (infile) // File exists
            {
                char messageText[224]; // Maximum chat text is 224 bytes.
                do //while( !infile.eof() )
                {
                    infile.getline(messageText, 224);
                    string tmp = parseTextline(messageText);

                    if (gproxy->m_BNET->GetInGame() && (messageText[0] != "#"[0]))
                    {
                        if ((messageText[0] != "%"[0]))
                        {
                            if (messageText[0] != "~"[0])
                            {
                                gproxy->sendGamemessage(QString::fromStdString(tmp));
                            }
                            else
                            {
                                // Sleep(50);
                            }
                        }
                        else
                        {
                            gproxy->m_BNET->QueueChatCommand(QString::fromStdString(tmp.substr(1)));
                        }
                    }
                    else if (messageText[0] != "#"[0])
                        gproxy->m_BNET->QueueChatCommand(QString::fromStdString(tmp));
                }
                while (!infile.eof());

                infile.close();
            }
            else
            {
                CONSOLE_Print("[ERROR] File \"" + QString::fromStdString(help) + "\" does not exist!");
                SendLocalChat("File \"" + QString::fromStdString(help) + "\" does not exist!");
            }
        }
        else if (command.length() >= 5 && command.startsWith("/re "))
        {
            if (m_BNET->GetLoggedIn())
            {
                if (!m_BNET->GetReplyTarget().empty())
                {
                    m_BNET->QueueChatCommand(message.mid(4), m_BNET->GetReplyTarget(), true);
                    SendLocalChat("Whispered to " + QString::fromStdString(m_BNET->GetReplyTarget()) + ": " + message.mid(4));
                }
                else
                {
                    SendLocalChat("Nobody has whispered you yet.");
                }
            }
            else
            {
                SendLocalChat("You are not connected to battle.net.");
            }
        }
        else if (command == "/sc" || command == "/spoof"
                || command == "/spoofcheck" || command == "/spoof check")
        {
            if (m_BNET->GetLoggedIn())
            {
                if (!m_GameStarted)
                {
                    m_BNET->QueueChatCommand("spoofcheck", m_HostName, true);
                    SendLocalChat("Whispered to " + QString::fromStdString(m_HostName) + ": spoofcheck");
                }
                else
                {
                    SendLocalChat("The game has already started.");
                }
            }
            else
            {
                SendLocalChat("You are not connected to battle.net.");
            }
        }
        else if (command.startsWith("/wh "))
        {
            QString msg = command.mid(4, command.length() - 4);
            m_BNET->QueueChatCommand(msg, m_HostName, true);
            SendLocalChat("Whispered to host [" + QString::fromStdString(m_HostName) + "] " + msg);
        }
        else if (command.startsWith("/whs "))
        {
            QString msg = command.mid(5, command.length() - 5);
            m_BNET->QueueChatCommand("!say " + msg, m_HostName, true);
            SendLocalChat("Whispered to host [" + QString::fromStdString(m_HostName) + "] !say " + msg);
        }
        else if (command.startsWith("/cmd "))
        {
            m_BNET->QueueChatCommand(command.mid(5, command.length() - 5));
        }
        else if (command == "/host")
        {
            SendLocalChat("Hosting player/bot is [" + QString::fromStdString(m_HostName) + "]. (use '/wh <message>' to whisper to him)");
        }
        else if (command == "/status")
        {
            if (m_LocalSocket)
            {
                if (m_GameIsReliable && m_ReconnectPort > 0)
                {
                    SendLocalChat("GProxy++ disconnect protection: Enabled");
                }
                else
                {
                    SendLocalChat("GProxy++ disconnect protection: Disabled");
                }

                if (m_BNET->GetLoggedIn())
                {
                    SendLocalChat("battle.net: Connected");
                }
                else
                {
                    SendLocalChat("battle.net: Disconnected");
                }

                if (mainGUI->getStatspage()->isLoggedIn())
                {
                    SendLocalChat("GhostGraz statspage: Available");
                }
                else
                {
                    SendLocalChat("GhostGraz statspage: Not available");
                }
            }
        }
        else if (command.length() >= 4 && command.startsWith("/w "))
        {
            if (m_BNET->GetLoggedIn())
            {
                int messageBeginIndex = command.indexOf(" ", 4);

                if (messageBeginIndex != -1 && messageBeginIndex != 0)
                {
                    m_BNET->QueueChatCommand(message);
                    SendLocalChat("Whisper to " + message.mid(3, messageBeginIndex - 3)
                            + ": " + message.mid(messageBeginIndex));
                }
            }
            else
            {
                SendLocalChat("You are not connected to battle.net.");
            }
        }
        else if (command == "/s" || command.startsWith("/s "))
        {
            if (command == "/s")
            {
                gproxy->sendGamemessage("!stats");
            }
            else
            {
                gproxy->sendGamemessage("!stats "+message.mid(3));
            }
        }
        else if (command == "/sd" || command.startsWith("/sd "))
        {
            if (command == "/sd")
            {
                gproxy->sendGamemessage("!statsdota");
            }
            else
            {
                gproxy->sendGamemessage("!statsdota "+message.mid(4));
            }
        }
        else if (command == "/statslast" || command == "/sl")
        {
            gproxy->sendGamemessage("!stats " + lastLeaver->getName());
        }
    }
    else if (command.length() >= 1 && command.startsWith("!"))
    {
        if (command.startsWith("!stats") || command.startsWith("!statsdota"))
        {
            return true;
        }
        else if (command == "!s" || command.startsWith("!s "))
        {
            if (command == "!s")
            {
                gproxy->sendGamemessage("!stats");
            }
            else
            {
                gproxy->sendGamemessage("!stats "+message.mid(3));
            }
        }
        else if (command == "!sd" || command.startsWith("!sd "))
        {
            if (command == "!sd")
            {
                gproxy->sendGamemessage("!statsdota");
            }
            else if (command.mid(3, 1) == " ")
            {
                gproxy->sendGamemessage("!statsdota "+message.mid(4));
            }
        }
        else if (command == "!statslast" || command == "!sl")
        {
            gproxy->sendGamemessage("!stats " + lastLeaver->getName());
        }
    }

    return forward;
}

void CGProxy::ProcessLocalPackets ()
{
    if (!m_LocalSocket)
        return;

    while (!m_LocalPackets.empty())
    {
        CCommandPacket *Packet = m_LocalPackets.front();
        m_LocalPackets.pop();
        BYTEARRAY Data = Packet->GetData();

        if (Packet->GetPacketType() == W3GS_HEADER_CONSTANT)
        {
            if (Packet->GetID() == CGameProtocol::W3GS_REQJOIN)
            {
                if (Data.size() >= 20)
                {
                    uint32_t EntryKey = UTIL_ByteArrayToUInt32(Data, false, 8);
                    unsigned char Unknown = Data[12];
                    uint16_t ListenPort = UTIL_ByteArrayToUInt16(Data, false, 13);
                    uint32_t PeerKey = UTIL_ByteArrayToUInt32(Data, false, 15);
                    BYTEARRAY Name = UTIL_ExtractCString(Data, 19);
                    string NameString = string(Name.begin(), Name.end());
                    BYTEARRAY Remainder = BYTEARRAY(Data.begin() + Name.size() + 20, Data.end());

                    if (Remainder.size() == 18)
                    {
                        // lookup the game in the main list

                        bool GameFound = false;

                        for (vector<CIncomingGameHost*> ::iterator i = m_Games.begin(); i != m_Games.end(); i++)
                        {
                            if (((CIncomingGameHost*) (*i))->GetUniqueGameID() == EntryKey)
                            {
                                CONSOLE_Print("[GPROXY] local player requested game name [" + QString::fromStdString(((CIncomingGameHost*) (*i))->GetGameName()) + "]");

                                if (NameString != username.toStdString())
                                    CONSOLE_Print("[GPROXY] using battle.net name [" + username + "] instead of requested name [" + QString::fromStdString(NameString) + "]");

                                CONSOLE_Print("[GPROXY] connecting to remote server [" + QString::fromStdString(((CIncomingGameHost*) (*i))->GetIPString()) + "] on port " + QString::number(((CIncomingGameHost*) (*i))->GetPort()));
                                m_RemoteServerIP = ((CIncomingGameHost*) (*i))->GetIPString();
                                m_RemoteServerPort = ((CIncomingGameHost*) (*i))->GetPort();
                                m_RemoteSocket->Reset();
                                m_RemoteSocket->SetNoDelay(true);
                                m_RemoteSocket->Connect(string(), m_RemoteServerIP, m_RemoteServerPort);
                                m_LastConnectionAttemptTime = getElapsedSeconds();
                                m_GameIsReliable = (((CIncomingGameHost*) (*i))->GetMapWidth() == 1984 && ((CIncomingGameHost*) (*i))->GetMapHeight() == 1984);
                                m_GameStarted = false;

                                // rewrite packet

                                BYTEARRAY DataRewritten;
                                DataRewritten.push_back(W3GS_HEADER_CONSTANT);
                                DataRewritten.push_back(Packet->GetID());
                                DataRewritten.push_back(0);
                                DataRewritten.push_back(0);
                                UTIL_AppendByteArray(DataRewritten, ((CIncomingGameHost*) (*i))->GetHostCounter(), false);
                                UTIL_AppendByteArray(DataRewritten, (uint32_t) 0, false);
                                DataRewritten.push_back(Unknown);
                                UTIL_AppendByteArray(DataRewritten, ListenPort, false);
                                UTIL_AppendByteArray(DataRewritten, PeerKey, false);
                                UTIL_AppendByteArray(DataRewritten, username.toStdString());
                                UTIL_AppendByteArrayFast(DataRewritten, Remainder);
                                BYTEARRAY LengthBytes;
                                LengthBytes = UTIL_CreateByteArray((uint16_t) DataRewritten.size(), false);
                                DataRewritten[2] = LengthBytes[0];
                                DataRewritten[3] = LengthBytes[1];
                                Data = DataRewritten;

                                // tell battle.net we're joining a game (for automatic spoof checking)
                                m_BNET->QueueJoinGame(((CIncomingGameHost*) (*i))->GetGameName());

                                // save the hostname for later (for manual spoof checking)
                                m_JoinedName = NameString;
                                m_HostName = ((CIncomingGameHost*) (*i))->GetHostName();
                                GameFound = true;

                                // Manufactorings work
                                m_GameName = ((CIncomingGameHost*) (*i))->GetGameName();
                                gproxy->changeChannel(QString::fromStdString(m_GameName));

                                break;
                            }
                        }

                        if (!GameFound)
                        {
                            CONSOLE_Print("[GPROXY] local player requested unknown game (expired?)");
                            m_LocalSocket->Disconnect();
                        }
                    }
                    else
                        CONSOLE_Print("[GPROXY] received invalid join request from local player (invalid remainder)");
                }
                else
                    CONSOLE_Print("[GPROXY] received invalid join request from local player (too short)");
            }
            else if (Packet->GetID() == CGameProtocol::W3GS_LEAVEGAME)
            {
                m_LeaveGameSent = true;
                m_LocalSocket->Disconnect();
                gproxy->m_BNET->QueueChatCommand("/whois " + QString::fromStdString(gproxy->m_HostName));
            }
            else if (Packet->GetID() == CGameProtocol::W3GS_CHAT_TO_HOST)
            {
                // handled in ExtractLocalPackets (yes, it's ugly)
            }
        }

        // warning: do not forward any data if we are not synchronized (e.g. we are reconnecting and resynchronizing)
        // any data not forwarded here will be cached in the packet buffer and sent later so all is well

        if (m_RemoteSocket && m_Synchronized)
            m_RemoteSocket->PutBytes(Data);

        delete Packet;
    }
}

void CGProxy::ExtractRemotePackets ()
{
    string *RecvBuffer = m_RemoteSocket->GetBytes();
    BYTEARRAY Bytes = UTIL_CreateByteArray((unsigned char *) RecvBuffer->c_str(), RecvBuffer->size());

    // a packet is at least 4 bytes so loop as long as the buffer contains 4 bytes

    while (Bytes.size() >= 4)
    {
        if (Bytes[0] == W3GS_HEADER_CONSTANT || Bytes[0] == GPS_HEADER_CONSTANT)
        {
            // bytes 2 and 3 contain the length of the packet

            uint16_t Length = UTIL_ByteArrayToUInt16(Bytes, false, 2);

            if (Length >= 4)
            {
                if (Bytes.size() >= Length)
                {
                    m_RemotePackets.push(new CCommandPacket(Bytes[0], Bytes[1], BYTEARRAY(Bytes.begin(), Bytes.begin() + Length)));

                    if (Bytes[0] == W3GS_HEADER_CONSTANT)
                        m_TotalPacketsReceivedFromRemote++;

                    *RecvBuffer = RecvBuffer->substr(Length);
                    Bytes = BYTEARRAY(Bytes.begin() + Length, Bytes.end());
                }
                else
                    return;
            }
            else
            {
                CONSOLE_Print("[GPROXY] received invalid packet from remote server (bad length)");
                m_Exiting = true;
                return;
            }
        }
        else
        {
            CONSOLE_Print("[GPROXY] received invalid packet from remote server (bad header constant)");
            m_Exiting = true;
            return;
        }
    }
}

void CGProxy::ProcessRemotePackets ()
{
    if (!m_LocalSocket || !m_RemoteSocket)
        return;

    while (!m_RemotePackets.empty())
    {
        CCommandPacket *Packet = m_RemotePackets.front();
        m_RemotePackets.pop();

        if (Packet->GetPacketType() == W3GS_HEADER_CONSTANT)
        {
            if (Packet->GetID() == CGameProtocol::W3GS_CHAT_FROM_HOST)
            {
                BYTEARRAY Data = Packet->GetData();
                unsigned int i = 5;

                unsigned char toPID_Count = Data[4];
                BYTEARRAY toPIDs = BYTEARRAY(Data.begin() + i, Data.begin() + i + toPID_Count);

                i += toPID_Count;

                int FromPID = (int) Data[i];
                unsigned char Flag = Data[i + 1];
                i += 2;
                BYTEARRAY ExtraFlags;
                string MessageString;

                if (Flag == 16)
                {
                    // Chat message
                    BYTEARRAY Message = UTIL_ExtractCString(Data, i);
                    MessageString = string(Message.begin(), Message.end());
                }
                else if (Flag >= 17 && Flag <= 20)
                {
                    // team/colour/race/handicap change request
                }
                else if (Flag == 32)
                {
                    // Extra flags
                    ExtraFlags = BYTEARRAY(Data.begin() + i, Data.begin() + i + 4);
                    BYTEARRAY Message = UTIL_ExtractCString(Data, i + 4);
                    MessageString = string(Message.begin(), Message.end());
                }

                if (Flag == 16 || Flag == 32) // Chat message
                {
                    string playerName;
                    foreach(Player *player, players)
                    {
                        if(player->getPlayerId() == FromPID)
                        {
                            playerName = player->getName().toStdString();
                            break;
                        }
                    }

                    // If the message is a autogenerated ban message
                    if (MessageString.find("Player [") != string::npos
                            && MessageString.find("] was banned by player [") != string::npos
                            && MessageString.find("] on server [") != string::npos)
                    {
                        QSound::play("sounds/player_banned.wav");

                    }

                    // If the message is a autogenerated "same ip" message
                    if ((MessageString.find("Player [") != string::npos
                            && MessageString.find("] has the same IP address as: ") != string::npos)
                            || MessageString.find("same IPs:") != string::npos)
                    {
                        QSound::play("sounds/same_ip.wav");
                    }

                    if (Flag == 16)
                    {
                        CONSOLE_Print("[LOBBY][" + QString::fromStdString(UTIL_ToColoredText(playerName))
                                + "] " + QString::fromStdString(MessageString), false);
                        LOG_Print("[LOBBY][" + QString::fromStdString(playerName) + "] "
                                + QString::fromStdString(MessageString));
                    }
                    else // GameStarted
                    {
                        // ExtraFlags[0] stores the type of chat message as defined by Warcraft III.
                        if (ExtraFlags[0] == 0) // 0 is an All message
                        {
                            CONSOLE_Print("[ALL][" + QString::fromStdString(playerName) + "] " + QString::fromStdString(MessageString));
                        }
                        else if (ExtraFlags[0] == 1) // 1 is an Allies message
                        {
                            CONSOLE_Print("[ALLY][" + QString::fromStdString(playerName) + "] " + QString::fromStdString(MessageString));
                        }
                        else if (ExtraFlags[0] == 2) // 2 is an Observer/Referee message
                        {
                            CONSOLE_Print("[OBSERVER][" + QString::fromStdString(playerName) + "] " + QString::fromStdString(MessageString));
                        }
                        else if (ExtraFlags[0] >= 3) // 3+ are private messages
                        {
                            // Based on limited testing it seems that
                            // the extra flags' first byte contains 3 plus the recipient's colour to denote a private message.
                            // Due to the fact that the Datapacket also contains the FromPID, we do not need to extract the color.

                            CONSOLE_Print("[PRIVATE][FROM][" + QString::fromStdString(playerName) + "] " + QString::fromStdString(MessageString));
                        }
                    }
                }
            }
            if (Packet->GetID() == CGameProtocol::W3GS_PLAYERLEAVE_OTHERS)
            {
                BYTEARRAY Data = Packet->GetData();

                int playerId = ((int) Data[4]);
                QString playerName;
                for (int i = 0; i < players.count(); i++)
                {
                    if(players.at(i)->getPlayerId() == playerId)
                    {
                        playerName = players.at(i)->getName();
                        delete lastLeaver;
                        lastLeaver = players.at(i);
                        players.remove(i);
                        break;
                    }
                }

                if (gproxy->m_GameStarted)
                {
                    if (gproxy->m_PlaySound)
                    {
                        QSound::play("sounds/leaver_detected.wav");
                    }

                    CONSOLE_Print("[ALL] " + playerName + " has left the game.");
                }
                else
                {
                    CONSOLE_Print("[LOBBY] " + playerName + " has left the game.");
                }
            }
            if (Packet->GetID() == CGameProtocol::W3GS_SLOTINFOJOIN)
            {
                for(int i = 0; i < players.count(); i++)
                {
                    delete players[i];
                }
                players.clear();

                BYTEARRAY Data = Packet->GetData();

                if (Data.size() >= 6)
                {
                    unsigned int SlotInfoSize = ((unsigned int) UTIL_ByteArrayToUInt16(Data, false, 4));

                    if (Data.size() >= 7 + SlotInfoSize)
                    {
                        m_ChatPID = Data[6 + SlotInfoSize];
                    }

                    // send a GPS_INIT packet
                    // if the server doesn't recognize it (e.g. it isn't GHost++) we should be kicked
                    CONSOLE_Print("[GPROXY] join request accepted by remote server");
                    if (m_GameIsReliable)
                    {
                        CONSOLE_Print("[GPROXY] detected reliable game, starting GPS handshake");
                        m_RemoteSocket->PutBytes(m_GPSProtocol->SEND_GPSC_INIT(1));
                    }
                    else
                        CONSOLE_Print("[GPROXY] detected standard game, disconnect protection disabled");

                    unsigned int i = 6;
                    unsigned char totalslotList = Data[i];
                    i += 1;

                    for(unsigned int i = 0; i < slotList.size(); i++)
                    {
                        delete slotList[i];
                    }
                    slotList.clear();

                    teams = 0;
                    vector <int> processedTeams;

                    for (int j = 0; j < totalslotList; j++)
                    {
                        slotList.push_back(new CIncomingSlots(Data[i], Data[i + 1], Data[i + 2], Data[i + 3],
                                Data[i + 4], Data[i + 5], Data[i + 6], Data[i + 7], Data[i + 8]));

                        if (((int) Data[i]) == m_ChatPID)
                            teamNumber = (int) Data[i + 4];

                        if (processedTeams.size() == 0)
                        {
                            processedTeams.push_back((int) Data[i + 4]);
                            teams++;
                        }
                        else
                        {
                            bool teamProcessed = false;
                            for (vector<int> ::iterator it = processedTeams.begin(); it != processedTeams.end(); it++)
                            {
                                if ((*it) == (int) Data[i + 4])
                                {
                                    teamProcessed = true;
                                    break;
                                }
                            }

                            if (!teamProcessed)
                            {
                                processedTeams.push_back((int) Data[i + 4]);
                                teams++;
                            }
                        }

                        i += 9;
                    }

                    Player *player = new Player();
                    player->setPlayerId(((int) m_ChatPID));
                    player->setName(username);
                    players.append(player);

                    emit signal_playerJoined(username);
                    emit signal_setGameslots(slotList);
                }
                else
                {
                    CONSOLE_Print("[SLOTINFOJOIN] Small data recieved!");
                }
            }
            else if (Packet->GetID() == CGameProtocol::W3GS_SLOTINFO)
            {
                BYTEARRAY Data = Packet->GetData();

                uint16_t SlotInfoSize = UTIL_ByteArrayToUInt16(Data, false, 4);

                if (Data.size() >= 7 + ((unsigned int) SlotInfoSize))
                {
                    m_ChatPID = Data[6 + SlotInfoSize];
                }

                unsigned int i = 6;
                unsigned char totalslotList = Data[i];
                i += 1;

                teams = 0;
                vector <int> processedTeams;

                for (int j = 0; j < totalslotList; j++)
                {
                    for (vector<CIncomingSlots *> ::iterator it = slotList.begin(); it != slotList.end(); it++)
                    {
                        if ((((CIncomingSlots *) (*it))->GetColor() == ((int) Data[i + 5])))
                        {
                            ((CIncomingSlots *) (*it))->SetPID(((int) Data[i]));
                            ((CIncomingSlots *) (*it))->SetDownloadStatus(((int) Data[i + 1]));
                            ((CIncomingSlots *) (*it))->SetSlotStatus(((int) Data[i + 2]));
                            ((CIncomingSlots *) (*it))->SetComputerStatus(((int) Data[i + 3]));
                            ((CIncomingSlots *) (*it))->SetTeam(((int) Data[i + 4]));
                            ((CIncomingSlots *) (*it))->SetColor(((int) Data[i + 5]));
                            ((CIncomingSlots *) (*it))->SetRace(((int) Data[i + 6]));
                            ((CIncomingSlots *) (*it))->SetComputerType(((int) Data[i + 7]));
                            ((CIncomingSlots *) (*it))->SetHandicap(((int) Data[i + 8]));
                            foreach(Player *player, players)
                            {
                                if(player->getPlayerId() == ((int) Data[i]))
                                {
                                    ((CIncomingSlots *) (*it))->SetName(player->getName().toStdString());
                                }
                            }

                            if (((int) Data[i]) == m_ChatPID)
                                teamNumber = (int) Data[i + 4];

                            break;
                        }
                    }

                    if (processedTeams.size() == 0)
                    {
                        processedTeams.push_back((int) Data[i + 4]);
                        teams++;
                    }
                    else
                    {
                        bool teamProcessed = false;
                        for (vector<int> ::iterator it = processedTeams.begin(); it != processedTeams.end(); it++)
                        {
                            if ((*it) == (int) Data[i + 4])
                            {
                                teamProcessed = true;
                                break;
                            }
                        }

                        if (!teamProcessed)
                        {
                            processedTeams.push_back((int) Data[i + 4]);
                            teams++;
                        }
                    }
                    i += 9;
                }

                emit signal_setGameslots(slotList);
            }
            else if (Packet->GetID() == CGameProtocol::W3GS_PLAYERINFO)
            {
                BYTEARRAY data = Packet->GetData();

                unsigned int i = 8;
                int playerId = (int) data[i];
                i += 1;
                BYTEARRAY Name = UTIL_ExtractCString(data, i);
                string playersName = UTIL_ToColoredText(string(Name.begin(), Name.end()));

                Player *player = new Player();
                player->setPlayerId(playerId);
                player->setName(QString::fromStdString(playersName));
                players.append(player);

                CONSOLE_Print("[LOBBY] "
                        + QString::fromStdString(playersName) + " has joined the game.");

                emit signal_playerJoined(QString::fromStdString(playersName));
            }
            else if (Packet->GetID() == CGameProtocol::W3GS_MAPCHECK)
            {
                BYTEARRAY data = Packet->GetData();

                BYTEARRAY path = UTIL_ExtractCString(data, 8);
                string filePath = string(path.begin(), path.end());

                transform(filePath.begin(), filePath.end(), filePath.begin(), (int(*)(int))tolower);
                if (filePath.find("dota") != string::npos)
                    setDotaMap(true);
                else
                    setDotaMap(false);
            }
            else if (Packet->GetID() == CGameProtocol::W3GS_GAMELOADED_OTHERS)
            {
                uint32_t loadingTime = getElapsedMilliseconds() - countdownEndTime;
                uint32_t sec = loadingTime / 1000;
                uint32_t dsec = (loadingTime / 100) % 10;
                ostringstream finishedLoading;

                if (((int) Packet->GetData()[4]) == m_ChatPID)
                {
                    CONSOLE_Print("[Manufactoring] You have finished loading. "
                            "Loading time: " + QString::number(sec) + "," + QString::number(dsec) + " seconds.");
                }
                else
                {
                    QString playerName;
                    foreach(Player *player, players)
                    {
                        if(player->getPlayerId() == ((int) Packet->GetData()[4]))
                        {
                            playerName = player->getName();
                            break;
                        }
                    }

                    CONSOLE_Print("[Manufactoring] "
                            + playerName + " has finished loading. Loading time: "
                            + QString::number(sec) + "," + QString::number(dsec) + " seconds.");
                }

                playerLoadingComplete[ ((int) Packet->GetData()[4]) ] = true;
                finishedLoadingCounter++;

                if (slotListWithPlayer == finishedLoadingCounter)
                {
                    for (int i = 0; i < 12; i++)
                        playerLoadingComplete[i] = false;

                    finishedLoadingCounter = 0;

                    if (gproxy->m_PlaySound)
                    {
                        QSound::play("sounds/game_started.wav");
                    }
                }
            }
            else if (Packet->GetID() == CGameProtocol::W3GS_COUNTDOWN_END)
            {
                if (m_GameIsReliable && m_ReconnectPort > 0)
                    CONSOLE_Print("[GPROXY] game started, disconnect protection enabled");
                else
                {
                    if (m_GameIsReliable)
                        CONSOLE_Print("[GPROXY] game started but GPS handshake not complete, disconnect protection disabled");
                    else
                        CONSOLE_Print("[GPROXY] game started");
                }

                m_GameStarted = true;
                countdownEndTime = getElapsedMilliseconds();

                slotListWithPlayer = 0;
                for (vector<CIncomingSlots *> ::iterator it = slotList.begin(); it != slotList.end(); it++)
                {
                    if (((CIncomingSlots *) (*it))->GetSlotStatus() == 2 && ((CIncomingSlots *) (*it))->GetComputerStatus() == 0)
                        slotListWithPlayer++;
                    else
                    {
                        delete *it;
                        it = slotList.erase(it);
                        continue;
                    }
                }

                emit signal_setGameslots(slotList);
            }
            else if (Packet->GetID() == CGameProtocol::W3GS_REJECTJOIN)
            {
                BYTEARRAY Data = Packet->GetData();

                if (Data.size() < 5)
                    return;

                uint32_t reason = UTIL_ByteArrayToUInt32(Data, false, 4);

                switch (reason)
                {
                    case 0x09:
                        CONSOLE_Print("[GPROXY] gamelobby is full");
                        break;
                    case 0x10:
                        CONSOLE_Print("[GPROXY] game has been started");
                        break;
                    case 0x27:
                        CONSOLE_Print("[GPROXY] wrong password");
                        break;
                }
            }
            else if (Packet->GetID() == CGameProtocol::W3GS_INCOMING_ACTION)
            {
                if (m_GameIsReliable)
                {
                    // we received a game update which means we can reset the number of empty actions we have to work with
                    // we also must send any remaining empty actions now
                    // note: the lag screen can't be up right now otherwise the server made a big mistake, so we don't need to check for it

                    BYTEARRAY EmptyAction;
                    EmptyAction.push_back(0xF7);
                    EmptyAction.push_back(0x0C);
                    EmptyAction.push_back(0x06);
                    EmptyAction.push_back(0x00);
                    EmptyAction.push_back(0x00);
                    EmptyAction.push_back(0x00);

                    for (unsigned char i = m_NumEmptyActionsUsed; i < m_NumEmptyActions; i++)
                        m_LocalSocket->PutBytes(EmptyAction);

                    m_NumEmptyActionsUsed = 0;
                }

                m_ActionReceived = true;
                m_LastActionTime = getElapsedSeconds();
            }
            else if (Packet->GetID() == CGameProtocol::W3GS_START_LAG)
            {
                if (m_GameIsReliable)
                {
                    BYTEARRAY Data = Packet->GetData();

                    if (Data.size() >= 5)
                    {
                        unsigned char NumLaggers = Data[4];

                        if (Data.size() == 5 + ((unsigned int) NumLaggers) * 5)
                        {
                            for (unsigned char i = 0; i < NumLaggers; i++)
                            {
                                bool LaggerFound = false;

                                for (vector<unsigned char> ::iterator j = m_Laggers.begin(); j != m_Laggers.end(); j++)
                                {
                                    if (*j == Data[5 + i * 5])
                                        LaggerFound = true;
                                }

                                if (LaggerFound)
                                    CONSOLE_Print("[GPROXY] warning - received start_lag on known lagger");
                                else
                                    m_Laggers.push_back(Data[5 + i * 5]);
                            }
                        }
                        else
                            CONSOLE_Print("[GPROXY] warning - unhandled start_lag (2)");
                    }
                    else
                        CONSOLE_Print("[GPROXY] warning - unhandled start_lag (1)");
                }
            }
            else if (Packet->GetID() == CGameProtocol::W3GS_STOP_LAG)
            {
                if (m_GameIsReliable)
                {
                    BYTEARRAY Data = Packet->GetData();

                    if (Data.size() == 9)
                    {
                        bool LaggerFound = false;

                        for (vector<unsigned char> ::iterator i = m_Laggers.begin(); i != m_Laggers.end();)
                        {
                            if (*i == Data[4])
                            {
                                i = m_Laggers.erase(i);
                                LaggerFound = true;
                            }
                            else
                                i++;
                        }

                        if (!LaggerFound)
                            CONSOLE_Print("[GPROXY] warning - received stop_lag on unknown lagger");
                    }
                    else
                        CONSOLE_Print("[GPROXY] warning - unhandled stop_lag");
                }
            }
            else if (Packet->GetID() == CGameProtocol::W3GS_INCOMING_ACTION2)
            {
                if (m_GameIsReliable)
                {
                    // we received a fractured game update which means we cannot use any empty actions until we receive the subsequent game update
                    // we also must send any remaining empty actions now
                    // note: this means if we get disconnected right now we can't use any of our buffer time, which would be very unlucky
                    // it still gives us 60 seconds total to reconnect though
                    // note: the lag screen can't be up right now otherwise the server made a big mistake, so we don't need to check for it

                    BYTEARRAY EmptyAction;
                    EmptyAction.push_back(0xF7);
                    EmptyAction.push_back(0x0C);
                    EmptyAction.push_back(0x06);
                    EmptyAction.push_back(0x00);
                    EmptyAction.push_back(0x00);
                    EmptyAction.push_back(0x00);

                    for (unsigned char i = m_NumEmptyActionsUsed; i < m_NumEmptyActions; i++)
                        m_LocalSocket->PutBytes(EmptyAction);

                    m_NumEmptyActionsUsed = m_NumEmptyActions;
                }
            }


            // forward the data

            m_LocalSocket->PutBytes(Packet->GetData());

            // we have to wait until now to send the status message since otherwise the slotinfojoin itself wouldn't have been forwarded

            if (Packet->GetID() == CGameProtocol::W3GS_SLOTINFOJOIN)
            {
                if (m_JoinedName != username.toStdString())
                    SendLocalChat("Using battle.net name \"" + username + "\" instead of LAN name \"" + QString::fromStdString(m_JoinedName) + "\".");

                if (m_GameIsReliable)
                    SendLocalChat("This is a reliable game. Requesting GProxy++ disconnect protection from server...");
                else
                    SendLocalChat("This is an unreliable game. GProxy++ disconnect protection is disabled.");
            }
        }
        else if (Packet->GetPacketType() == GPS_HEADER_CONSTANT)
        {
            if (m_GameIsReliable)
            {
                BYTEARRAY Data = Packet->GetData();

                if (Packet->GetID() == CGPSProtocol::GPS_INIT && Data.size() == 12)
                {
                    m_ReconnectPort = UTIL_ByteArrayToUInt16(Data, false, 4);
                    m_PID = Data[6];
                    m_ReconnectKey = UTIL_ByteArrayToUInt32(Data, false, 7);
                    m_NumEmptyActions = Data[11];
                    SendLocalChat("GProxy++ disconnect protection is ready (" + QString::number((m_NumEmptyActions + 1) * 60) + " second buffer).");
                    CONSOLE_Print("[GPROXY] handshake complete, disconnect protection ready (" + QString::number((m_NumEmptyActions + 1) * 60) + " second buffer)");
                }
                else if (Packet->GetID() == CGPSProtocol::GPS_RECONNECT && Data.size() == 8)
                {
                    uint32_t LastPacket = UTIL_ByteArrayToUInt32(Data, false, 4);
                    uint32_t PacketsAlreadyUnqueued = m_TotalPacketsReceivedFromLocal - m_PacketBuffer.size();

                    if (LastPacket > PacketsAlreadyUnqueued)
                    {
                        uint32_t PacketsToUnqueue = LastPacket - PacketsAlreadyUnqueued;

                        if (PacketsToUnqueue > m_PacketBuffer.size())
                        {
                            CONSOLE_Print("[GPROXY] received GPS_RECONNECT with last packet > total packets sent");
                            PacketsToUnqueue = m_PacketBuffer.size();
                        }

                        while (PacketsToUnqueue > 0)
                        {
                            delete m_PacketBuffer.front();
                            m_PacketBuffer.pop();
                            PacketsToUnqueue--;
                        }
                    }

                    // send remaining packets from buffer, preserve buffer
                    // note: any packets in m_LocalPackets are still sitting at the end of this buffer because they haven't been processed yet
                    // therefore we must check for duplicates otherwise we might (will) cause a desync

                    queue<CCommandPacket *> TempBuffer;

                    while (!m_PacketBuffer.empty())
                    {
                        if (m_PacketBuffer.size() > m_LocalPackets.size())
                            m_RemoteSocket->PutBytes(((CCommandPacket *) m_PacketBuffer.front())->GetData());

                        TempBuffer.push(m_PacketBuffer.front());
                        m_PacketBuffer.pop();
                    }

                    m_PacketBuffer = TempBuffer;

                    // we can resume forwarding local packets again
                    // doing so prior to this point could result in an out-of-order stream which would probably cause a desync

                    m_Synchronized = true;
                }
                else if (Packet->GetID() == CGPSProtocol::GPS_ACK && Data.size() == 8)
                {
                    uint32_t LastPacket = UTIL_ByteArrayToUInt32(Data, false, 4);
                    uint32_t PacketsAlreadyUnqueued = m_TotalPacketsReceivedFromLocal - m_PacketBuffer.size();

                    if (LastPacket > PacketsAlreadyUnqueued)
                    {
                        uint32_t PacketsToUnqueue = LastPacket - PacketsAlreadyUnqueued;

                        if (PacketsToUnqueue > m_PacketBuffer.size())
                        {
                            CONSOLE_Print("[GPROXY] received GPS_ACK with last packet > total packets sent");
                            PacketsToUnqueue = m_PacketBuffer.size();
                        }

                        while (PacketsToUnqueue > 0)
                        {
                            delete m_PacketBuffer.front();
                            m_PacketBuffer.pop();
                            PacketsToUnqueue--;
                        }
                    }
                }
                else if (Packet->GetID() == CGPSProtocol::GPS_REJECT && Data.size() == 8)
                {
                    uint32_t Reason = UTIL_ByteArrayToUInt32(Data, false, 4);

                    if (Reason == REJECTGPS_INVALID)
                        CONSOLE_Print("[GPROXY] rejected by remote server: invalid data");
                    else if (Reason == REJECTGPS_NOTFOUND)
                        CONSOLE_Print("[GPROXY] rejected by remote server: player not found in any running games");

                    m_LocalSocket->Disconnect();
                }

            }
        }

        delete Packet;
    }
}

bool CGProxy::AddGame (CIncomingGameHost* game)
{
    // check for duplicates and rehosted games

    bool DuplicateFound = false;
    uint32_t OldestReceivedTime = getElapsedSeconds();

    for (vector<CIncomingGameHost*> ::iterator i = m_Games.begin(); i != m_Games.end(); i++)
    {
        if (game->GetIP() == ((CIncomingGameHost*) (*i))->GetIP() && game->GetPort() == ((CIncomingGameHost*) (*i))->GetPort())
        {
            // duplicate or rehosted game, delete the old one and add the new one
            // don't forget to remove the old one from the LAN list first

            m_UDPSocket->Broadcast(6112, m_GameProtocol->SEND_W3GS_DECREATEGAME(((CIncomingGameHost*) (*i))->GetUniqueGameID()));
            delete *i;
            *i = game;
            DuplicateFound = true;
            break;
        }

        if (game->GetGameName() != m_BNET->GetSearchGameName() && game->GetReceivedTime() < OldestReceivedTime)
            OldestReceivedTime = game->GetReceivedTime();
    }

    if (!DuplicateFound)
    {
        m_Games.push_back(game);
        gproxy->autosearch = true; //pr0 autosearch
    }

    // the game list cannot hold more than 20 games (warcraft 3 doesn't handle it properly and ignores any further games)
    // if this game puts us over the limit, remove the oldest game
    // don't remove the "search game" since that's probably a pretty important game
    // note: it'll get removed automatically by the 60 second timeout in the main loop when appropriate

    if (m_Games.size() > 20)
    {
        for (vector<CIncomingGameHost*> ::iterator i = m_Games.begin(); i != m_Games.end(); i++)
        {
            if (game->GetGameName() != m_BNET->GetSearchGameName() && game->GetReceivedTime() == OldestReceivedTime)
            {
                m_UDPSocket->Broadcast(6112, m_GameProtocol->SEND_W3GS_DECREATEGAME(((CIncomingGameHost*) (*i))->GetUniqueGameID()));
                delete *i;
                m_Games.erase(i);
                break;
            }
        }
    }

    return !DuplicateFound;
}

void CGProxy::sendGamemessage (QString message, bool alliesOnly)
{
    if (message.isEmpty() || !gproxy->m_BNET->GetInGame())
    {
        return;
    }

    if (message.length() > 254)
    {
        message = message.left(254);
    }

    BYTEARRAY toPIDs;
    BYTEARRAY packet;

    if (!gproxy->m_GameStarted)
    {
        for (vector<CIncomingSlots *> ::iterator it = slotList.begin(); it != slotList.end(); it++)
        {
            toPIDs.push_back(((CIncomingSlots *) (*it))->GetPID());
        }

        packet = gproxy->m_GameProtocol->SEND_W3GS_CHAT_TO_HOST(gproxy->m_ChatPID, toPIDs, 16, BYTEARRAY(), message);
    }
    else if (alliesOnly)
    {
        for (vector<CIncomingSlots *> ::iterator it = slotList.begin(); it != slotList.end(); it++)
        {
            if (((CIncomingSlots *) (*it))->GetTeam() == teamNumber)
            {
                toPIDs.push_back(((CIncomingSlots *) (*it))->GetPID());
            }
        }

        BYTEARRAY extraFlags;
        extraFlags.push_back(1);
        extraFlags.push_back(0);
        extraFlags.push_back(0);
        extraFlags.push_back(0);

        packet = gproxy->m_GameProtocol->SEND_W3GS_CHAT_TO_HOST(gproxy->m_ChatPID, toPIDs, 32, extraFlags, message);
    }
    else
    {
        for (vector<CIncomingSlots *> ::iterator it = slotList.begin(); it != slotList.end(); it++)
        {
            toPIDs.push_back(((CIncomingSlots *) (*it))->GetPID());
        }

        BYTEARRAY extraFlags;
        extraFlags.push_back(0);
        extraFlags.push_back(0);
        extraFlags.push_back(0);
        extraFlags.push_back(0);

        packet = gproxy->m_GameProtocol->SEND_W3GS_CHAT_TO_HOST(gproxy->m_ChatPID, toPIDs, 32, extraFlags, message);
    }

    if (CheckForwarding(message))
    {
        m_PacketBuffer.push(new CCommandPacket(W3GS_HEADER_CONSTANT, packet[1], packet));
        m_LocalPackets.push(new CCommandPacket(W3GS_HEADER_CONSTANT, packet[1], packet));
        m_TotalPacketsReceivedFromLocal++;
    }
}

void CGProxy::changeTeam (unsigned char team)
{
    BYTEARRAY toPIDs;

    for (vector<CIncomingSlots *> ::iterator it = slotList.begin(); it != slotList.end(); it++)
    {
        if (((CIncomingSlots *) (*it))->GetSlotStatus() == 2 && ((CIncomingSlots *) (*it))->GetComputerStatus() == 0)
            toPIDs.push_back(((CIncomingSlots *) (*it))->GetPID());
    }

    BYTEARRAY Packet = gproxy->m_GameProtocol->SEND_W3GS_TEAMCHANGE(gproxy->m_ChatPID, toPIDs, team);

    m_PacketBuffer.push(new CCommandPacket(W3GS_HEADER_CONSTANT, Packet[1], Packet));
    m_LocalPackets.push(new CCommandPacket(W3GS_HEADER_CONSTANT, Packet[1], Packet));
    m_TotalPacketsReceivedFromLocal++;
}

void CGProxy::SendLocalChat (QString message)
{
    if (m_LocalSocket)
    {
        if (m_GameStarted)
        {
            if (message.length() > 127)
            {
                message = message.left(127);
            }

            m_LocalSocket->PutBytes(m_GameProtocol->SEND_W3GS_CHAT_FROM_HOST(m_ChatPID, UTIL_CreateByteArray(m_ChatPID), 32, UTIL_CreateByteArray((uint32_t) 0, false), message));
        }
        else
        {
            if (message.size() > 254)
            {
                message = message.left(254);
            }

            m_LocalSocket->PutBytes(m_GameProtocol->SEND_W3GS_CHAT_FROM_HOST(m_ChatPID, UTIL_CreateByteArray(m_ChatPID), 16, BYTEARRAY(), message));
        }
    }
}

void CGProxy::SendEmptyAction ()
{
    // we can't send any empty actions while the lag screen is up
    // so we keep track of who the lag screen is currently showing (if anyone) and we tear it down, send the empty action, and put it back up

    for (vector<unsigned char> ::iterator i = m_Laggers.begin(); i != m_Laggers.end(); i++)
    {
        BYTEARRAY StopLag;
        StopLag.push_back(0xF7);
        StopLag.push_back(0x11);
        StopLag.push_back(0x09);
        StopLag.push_back(0);
        StopLag.push_back(*i);
        UTIL_AppendByteArray(StopLag, (uint32_t) 60000, false);
        m_LocalSocket->PutBytes(StopLag);
    }

    BYTEARRAY EmptyAction;
    EmptyAction.push_back(0xF7);
    EmptyAction.push_back(0x0C);
    EmptyAction.push_back(0x06);
    EmptyAction.push_back(0x00);
    EmptyAction.push_back(0x00);
    EmptyAction.push_back(0x00);
    m_LocalSocket->PutBytes(EmptyAction);

    if (!m_Laggers.empty())
    {
        BYTEARRAY StartLag;
        StartLag.push_back(0xF7);
        StartLag.push_back(0x10);
        StartLag.push_back(0);
        StartLag.push_back(0);
        StartLag.push_back(m_Laggers.size());

        for (vector<unsigned char> ::iterator i = m_Laggers.begin(); i != m_Laggers.end(); i++)
        {
            // using a lag time of 60000 ms means the counter will start at zero
            // hopefully warcraft 3 doesn't care about wild variations in the lag time in subsequent packets

            StartLag.push_back(*i);
            UTIL_AppendByteArray(StartLag, (uint32_t) 60000, false);
        }

        BYTEARRAY LengthBytes;
        LengthBytes = UTIL_CreateByteArray((uint16_t) StartLag.size(), false);
        StartLag[2] = LengthBytes[0];
        StartLag[3] = LengthBytes[1];
        m_LocalSocket->PutBytes(StartLag);
    }
}

bool CGProxy::checkStatus (int statusCode)
{
    switch (statusCode)
    {
        case 0:
        {
            CONSOLE_Print("[GPROXY] Configuration file successfully loaded");
            return true;
        }
        case 1:
        {
            ConfigGUI *configGUI = new ConfigGUI(gproxy->getConfig(), true);
            configGUI->show();
            return false;
        }
        case 2:
        {
            CONSOLE_Print("Could not access the configuration file.\n"
                    "Maybe you dont have permissions to write to that directory.\n"
                    "Try to move your folder outside \"C:\\Program Files\" or run as administrator."
                    "\nShutting down GProxy.");
            return false;
        }
        default:
        {
            CONSOLE_Print("Unknown error while loading configuration file.\n"
                    "ErrorCode: " + QString::number(statusCode));
            return false;
        }
    }
}

bool fcfgfilterfirst ()// Phyton filter
{
    return gproxy->cfgfilterfirst;

}

string fcfgfilter ()// Phyton filter
{
    return gproxy->cfgfilter;
}

bool getautosearch () //pr0 autosearch
{
    return gproxy->autosearch;
}

void autosearch (bool autosearchNew) //pr0 autosearch
{
    gproxy->autosearch = autosearchNew;
}

bool cautosearch () //pr0 cautosearch
{
    return gproxy->cautosearch;
}

bool displayautocreated ()
{
    return gproxy->displayautocreated;
}

void displayautocreated (bool newone)
{
    gproxy->displayautocreated = newone;
}

void flisting_current_games (bool newone)
{
    gproxy->m_listing_current_games = newone;
}

bool flisting_current_games ()
{
    return gproxy->m_listing_current_games;
}

CIncomingSlots::CIncomingSlots (unsigned char nPID, unsigned char ndownloadStatus, unsigned char nslotStatus,
        unsigned char ncomputerStatus, unsigned char nteam, unsigned char ncolor,
        unsigned char nrace, unsigned char ncomputerType, unsigned char nhandicap)
{
    PID = (int) nPID;
    downloadStatus = (int) ndownloadStatus;
    slotStatus = (int) nslotStatus;
    computerStatus = (int) ncomputerStatus;
    team = (int) nteam;
    color = (int) ncolor;
    race = (int) nrace;
    computerType = (int) ncomputerType;
    handicap = (int) nhandicap;
}

CIncomingSlots::~CIncomingSlots () { }

void CGProxy::setServer (const QString &server)
{
    this->server = server;
}

void CGProxy::setUsername (const QString &username)
{
    this->username = username;
}

void CGProxy::setPassword (const QString &password)
{
    this->password = password;
}

void CGProxy::setWar3version (const uint32_t &war3version)
{
    this->war3version = war3version;
}

void CGProxy::setPort (const uint16_t &port)
{
    this->port = port;
}

void CGProxy::setExeversion (const BYTEARRAY &exeversion)
{
    this->exeversion = exeversion;
}

void CGProxy::setExeversionhash (const BYTEARRAY &exeversionhash)
{
    this->exeversionhash = exeversionhash;
}

void CGProxy::setPasswordhashtype (const QString &passwordhashtype)
{
    this->passwordhashtype = passwordhashtype;
}

void CGProxy::setChannel (const QString &channel)
{
    this->channel = channel;
}

QString CGProxy::getServer ()
{
    return this->server;
}

QString CGProxy::getUsername ()
{
    return this->username;
}

QString CGProxy::getPassword ()
{
    return this->password;
}

uint32_t CGProxy::getWar3version ()
{
    return this->war3version;
}

uint16_t CGProxy::getPort ()
{
    return this->port;
}

BYTEARRAY CGProxy::getExeversion ()
{
    return this->exeversion;
}

BYTEARRAY CGProxy::getExeversionhash ()
{
    return this->exeversionhash;
}

QString CGProxy::getPasswordhashtype ()
{
    return this->passwordhashtype;
}

QString CGProxy::getChannel ()
{
    return this->channel;
}

QVector<Player*> CGProxy::getPlayers()
{
    return players;
}

void CGProxy::setPlayers(const QVector<Player*> &players)
{
    this->players = players;
}

Player* CGProxy::getLastLeaver()
{
    return lastLeaver;
}