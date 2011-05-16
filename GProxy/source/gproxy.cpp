/*

   Copyright 2010 Trevor Hogan

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

 */

// todotodo: GHost++ may drop the player even after they reconnect if they run out of time and haven't caught up yet

#include <QtGui/QApplication>
#include <QMessageBox>
#include <QIcon>
#include <QSound>
#include <QElapsedTimer>

#include "gproxy.h"
#include "util.h"
#include "config_.h"
#include "socket.h"
#include "commandpacket.h"
#include "bnetprotocol.h"
#include "bnet.h"
#include "gameprotocol.h"
#include "gpsprotocol.h"
#include "MainGUI.h"
#include "GProxyUpdateThread.h"
#include "DownloadThread.h"
#include "ConfigGUI.h"

#include <signal.h>
#include <stdlib.h>
#include <direct.h>
#include <sstream>
#include <windows.h>
#include <winsock.h>
//#include "mysql/include/mysql.h" // has to be after winsock
#include <time.h>

#ifndef WIN32
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <mach/mach_time.h>
#endif

CGProxy *gproxy;
MainGUI *mainGUI;
GProxyUpdateThread *gUpdateThread;
DownloadThread *dt;
QElapsedTimer timer;

string gLogFile;
vector<string> gGamesBuffer;

/* Phytons variables */
int GameNamesCount = 0;
string GameNames[10] = {"", "", "", "", "", "", "", "", "", ""}; // Phyton insteandsend.
string cchatcommand;

/* Manufactorings work */
vector<CIncomingSlots *> slotList;
CPlayer players[12];
bool playerLoadingComplete[12];
int slotListWithPlayer;
int finishedLoadingCounter;
int teamNumber;
int teams;
unsigned int countdownEndTime;
//MYSQL *conn;
//MYSQL_RES *res;
//MYSQL_ROW row;

void addgame(string gamename)
{
    gGamesBuffer.push_back(gamename);
};

string parseTextline(string input)
{
    if (gproxy->m_LocalSocket && (input.find("$host$") != string::npos))
    {
        input.replace(input.find("$host$"), 6, gproxy->m_HostName);
    }

    return input;
};

string get_time()
{
    string temp;
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "[%H:%M:%S]", timeinfo);
    return temp.assign(buffer);
}

bool textEndsWith(string text, string endText)
{
    if (text.length() > endText.length())
    {
        if (0 == text.compare(text.length() - endText.length(), endText.length(), endText))
        {
            return true;
        }
    }

    return false;
}

unsigned long getElapsedSeconds()
{
    return timer.elapsed() / 1000;
}

unsigned long getElapsedMilliseconds()
{
    return timer.elapsed();
}

//unsigned int GetTime()
//{
//    return GetTicks() / 1000;
//}
//
//unsigned int GetTicks()
//{
//#ifdef WIN32
//    return timeGetTime();
//#elif __APPLE__
//    uint64_t current = mach_absolute_time();
//    static mach_timebase_info_data_t info = {0, 0};
//    // get timebase info
//    if (info.denom == 0)
//        mach_timebase_info(&info);
//    uint64_t elapsednano = current * (info.numer / info.denom);
//    // convert ns to ms
//    return elapsednano / 1e6;
//#else
//    uint32_t ticks;
//    struct timespec t;
//    clock_gettime(CLOCK_MONOTONIC, &t);
//    ticks = t.tv_sec * 1000;
//    ticks += t.tv_nsec / 1000000;
//    return ticks;
//#endif
//}

void LOG_Print(string message)
{
    if (!gLogFile.empty())
    {
        ofstream Log;
        Log.open(gLogFile.c_str(), ios::app);

        if (!Log.fail())
        {
            time_t Now = time(NULL);
            string Time = asctime(localtime(&Now));

            // erase the newline

            Time.erase(Time.size() - 1);
            Log << "[" << Time << "] " << message << endl;
            Log.close();
        }
    }
}

// Phyton core alias for searchforgame
void CheckForGame(string gamename)
{
    if (getautosearch())
    {
        gproxy->m_BNET->SetSearchGameName(gamename); //old one, but it works..
        CONSOLE_Print("[Phyton] Searching for [" + QString::fromStdString(gamename) + "].", true);
        gproxy->autosearch = false;
    }
}

// Phyton autospoofcheck
void Pspoofcheck()
{
    if (gproxy->m_BNET->GetLoggedIn())
    {
        gproxy->m_BNET->QueueChatCommand("spoofcheck", gproxy->m_HostName, true);
    }
}

void CONSOLE_Print(QString message, bool log)
{
    gproxy->addMessage(message, log);
}

//
// main
//

int main(int argc, char **argv)
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

    timer.start();

    CONSOLE_Print("[GPROXY] starting up");
    CONSOLE_Print("[GPROXY] Trying to loading configuration file");

    Config *config = new Config("gproxy.cfg");
    gproxy->setConfig(config);

    int status = config->loadConfig();
    if(gproxy->checkStatus(status) == false)
    {
        return 1;
    }

    mainGUI->init();

    if(config->getBoolean("log"))
    {
        gLogFile = "gproxy_log.txt";
    }
    else
    {
        gLogFile = "";
    }

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

    gproxy->setWar3Path(config->getString("war3path"));
    gproxy->setCDKeyROC(config->getString("cdkeyroc"));
    gproxy->setCDKeyTFT(config->getString("cdkeytft"));
    string Server = config->getString("server").toStdString();

    gproxy->setPrivategamename(config->getString("privategamename"));
    gproxy->setBotprefix(config->getString("botprefix"));
    bool casearch = config->getBoolean("autosearch");
    gproxy->m_PlaySound = config->getBoolean("sound");
    string Username = config->getString("username").toStdString();
    string Password = config->getString("password").toStdString();
    string Channel = config->getString("channel").toStdString();
    uint32_t War3Version = config->getInt("war3version");
    uint16_t Port = config->getInt("port");
    BYTEARRAY EXEVersion = UTIL_ExtractNumbers(config->getString("exeversion").toStdString(), 4);
    BYTEARRAY EXEVersionHash = UTIL_ExtractNumbers(config->getString("exeversionhash").toStdString(), 4);
    string PasswordHashType = config->getString("passwordhashtype").toStdString();

    CONSOLE_Print("", false);
    CONSOLE_Print("  Welcome to GProxy++.", false);
    CONSOLE_Print("  Server: " + QString::fromStdString(Server), false);
    CONSOLE_Print("  Username: " + QString::fromStdString(Username), false);
    CONSOLE_Print("  Channel: " + QString::fromStdString(Channel), false);
    CONSOLE_Print("", false);
    CONSOLE_Print("  Type /help at any time for help.", false);
    CONSOLE_Print("", false);
    CONSOLE_Print("Welcome to <font color=\"red\">GProxy++</font>, "
            "this mod is by <font color=\"darkgreen\">Phyton</font>, "
            "<font color=\"deeppink\">Pr0gm4n</font> and "
            "<font color=\"gold\">Manufactoring</font>.");

    gproxy->init(Server, Username, Password, Channel, War3Version, Port, EXEVersion, EXEVersionHash, PasswordHashType, "", "", true, casearch, true, false, 200);

    dt = new DownloadThread(mainGUI);
    dt->start();

    gUpdateThread = new GProxyUpdateThread(gproxy);
    gUpdateThread->start();

    return app.exec();
}

//
// CGProxy
//

CGProxy::CGProxy() { }

void CGProxy::init(string nServer, string nUsername, string nPassword, string nChannel, uint32_t nWar3Version, uint16_t nPort, BYTEARRAY nEXEVersion, BYTEARRAY nEXEVersionHash, string nPasswordHashType, string cpublic ,string cfilter,bool german_languagesupport, bool casearch, bool temp_displayautocreated, bool listing_current_games, int channelWidth)
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
    m_Server = nServer;
    username = QString::fromStdString(nUsername);
    m_Password = nPassword;
    m_Channel = nChannel;
    m_War3Version = nWar3Version;
    m_Port = nPort;
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
    m_BNET = new CBNET(this, mainGUI, m_Server, string(), 0, 0, "USA", "United States", username.toStdString(), m_Password, m_Channel, m_War3Version, nEXEVersion, nEXEVersionHash, nPasswordHashType, 200);
    m_LocalServer->Listen(string(), m_Port);
    cfgfilterfirst = true; // Phyton filter
    cfgfilter = cfilter; // Phyton filter
    cfgpublic = cpublic; // Phyton public
    autosearch = true; //pr0 autosearch
    cautosearch = casearch; //pr0 cautosearch
    displayautocreated = temp_displayautocreated;
    m_listing_current_games = listing_current_games;
    testvar = false;
    m_PlaySound = true; // manu
    vShallCreate = false;
    vShallCreateQuiet = false;
    parrot = "";
    dotaMap = false;

    CONSOLE_Print("[GPROXY] Customized GProxy++ Version " + QString::fromStdString(m_Version));
}

CGProxy::~CGProxy()
{
    for (vector<CIncomingGameHost *> ::iterator i = m_Games.begin(); i != m_Games.end(); i++)
        m_UDPSocket->Broadcast(6112, m_GameProtocol->SEND_W3GS_DECREATEGAME(((CIncomingGameHost *) (*i))->GetUniqueGameID()));

    delete m_LocalServer;
    delete m_LocalSocket;
    delete m_RemoteSocket;
    delete m_UDPSocket;
    delete m_BNET;

    for (vector<CIncomingGameHost *> ::iterator i = m_Games.begin(); i != m_Games.end(); i++)
        delete *i;

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

void CGProxy::cleanup()
{
    CONSOLE_Print("[GPROXY] shutting down running threads");
    delete gUpdateThread;
    delete dt;
    gUpdateThread = NULL;
    dt = NULL;

#ifdef WIN32
    CONSOLE_Print("[GPROXY] shutting down winsock");
    WSACleanup();
#endif

    CONSOLE_Print("[GPROXY] shutting down");
    delete mainGUI;
    mainGUI = NULL;
    delete gproxy;
    gproxy = NULL;
}

void CGProxy::addMessage(QString msg, bool log)
{
    emit signal_addMessage(msg, log);
}

void CGProxy::changeChannel(QString channel)
{
    emit signal_changeChannel(channel);
}

void CGProxy::addChannelUser(QString username, QString clanTag)
{
    // Phyton waitgame
    if (gproxy->getVShallCreate())
    {
        QString tname = username.toLower();
        QString tpre = gproxy->getBotprefix().toLower();

        if (gproxy->getBotprefix().size() > 0 && gproxy->getBotprefix()[0] != "^"[0])
            if (gproxy->getPrivategamename() != "" && tpre != "" && tname.mid(0, tpre.size()) == tpre)
            {
                gproxy->m_BNET->QueueChatCommand("/w " + username.toStdString()
                        + " !priv " + gproxy->getPrivategamename().toStdString());
                autosearch = true;
                CheckForGame(gproxy->getPrivategamename().toStdString());
                if (!gproxy->getVShallCreateQuiet())
                {
                    gproxy->m_BNET->QueueChatCommand("gn: " + gproxy->getPrivategamename().toStdString());
                }
                gproxy->setVShallCreate(false);
            }
            else;
        else
        {
            if (gproxy->getPrivategamename() != "" && tname == tpre.mid(1, tpre.size() - 1))
            {
                autosearch = true;
                gproxy->m_BNET->QueueChatCommand("/w " + username.toStdString()
                        + " !priv " + gproxy->getPrivategamename().toStdString());
                CheckForGame(gproxy->getPrivategamename().toStdString());
                if (!gproxy->getVShallCreateQuiet())
                {
                    gproxy->m_BNET->QueueChatCommand("gn: " + gproxy->getPrivategamename().toStdString());
                }
                gproxy->setVShallCreate(false);
            }
        }
    }

    emit signal_addChannelUser(username, clanTag);
}

void CGProxy::removeChannelUser(QString username)
{
    emit signal_removeChannelUser(username);
}

void CGProxy::friendUpdate(vector<CIncomingFriendList *> friendList)
{
    clearFriendlist();

    for (vector<CIncomingFriendList *> ::iterator it = friendList.begin();
            it != friendList.end(); it++)
    {
        addFriend(QString::fromStdString(((*it))->GetAccount()),
                (((*it))->GetArea() != 0),
                QString::fromStdString((*it)->GetLocation()));
    }
}

void CGProxy::clearFriendlist()
{
    emit signal_clearFriendlist();
}

void CGProxy::addFriend(QString username, bool online, QString location)
{
    emit signal_addFriend(username, online, location);
}

bool CGProxy::Update(long usecBlock)
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
        MILLISLEEP(50);

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

                        SendLocalChat("GProxy++ is attempting to reconnect... (" + UTIL_ToString(TimeRemaining) + " seconds remain)");
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

                        SendLocalChat("GProxy++ is attempting to reconnect... (" + UTIL_ToString(TimeRemaining) + " seconds remain)");
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

                        SendLocalChat("GProxy++ is attempting to reconnect... (" + UTIL_ToString(TimeRemaining) + " seconds remain)");
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

                            SendLocalChat("GProxy++ is attempting to reconnect... (" + UTIL_ToString(TimeRemaining) + " seconds remain)");
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
            for (vector<CIncomingGameHost *> ::iterator i = m_Games.begin(); i != m_Games.end();)
            {
                // expire games older than 60 seconds

                if (getElapsedSeconds() - ((CIncomingGameHost *) (*i))->GetReceivedTime() >= 60)
                {
                    // don't forget to remove it from the LAN list first

                    m_UDPSocket->Broadcast(6112, m_GameProtocol->SEND_W3GS_DECREATEGAME(((CIncomingGameHost *) (*i))->GetUniqueGameID()));
                    delete *i;
                    i = m_Games.erase(i);
                    continue;
                }

                BYTEARRAY MapGameType;
                UTIL_AppendByteArray(MapGameType, ((CIncomingGameHost *) (*i))->GetGameType(), false);
                UTIL_AppendByteArray(MapGameType, ((CIncomingGameHost *) (*i))->GetParameter(), false);
                BYTEARRAY MapFlags = UTIL_CreateByteArray(((CIncomingGameHost *) (*i))->GetMapFlags(), false);
                BYTEARRAY MapWidth = UTIL_CreateByteArray(((CIncomingGameHost *) (*i))->GetMapWidth(), false);
                BYTEARRAY MapHeight = UTIL_CreateByteArray(((CIncomingGameHost *) (*i))->GetMapHeight(), false);
                string GameName = ((CIncomingGameHost *) (*i))->GetGameName();

                // colour reliable game names so they're easier to pick out of the list

                if (((CIncomingGameHost *) (*i))->GetMapWidth() == 1984 &&
                        ((CIncomingGameHost *) (*i))->GetMapHeight() == 1984)
                {
                    if (GameName == gproxy->getPrivategamename().toStdString())
                    {
                        GameName = "|cFFFF0000" + GameName;
                    }
                    else if (((CIncomingGameHost *) (*i))->GetHostName().substr(0, 9) == "GhostGraz")
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

                m_UDPSocket->Broadcast(6112, m_GameProtocol->SEND_W3GS_GAMEINFO(m_TFT, m_War3Version, MapGameType, MapFlags, MapWidth, MapHeight, GameName, (*i)->GetHostName(), (*i)->GetElapsedTime(), (*i)->GetMapPath(), (*i)->GetMapCRC(), 12, 12, m_Port, (*i)->GetUniqueGameID(), (*i)->GetUniqueGameID()));
                i++;
            }

            m_LastRefreshTime = getElapsedSeconds();
        }
    }

    return m_Exiting;
}

void CGProxy::ExtractLocalPackets()
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

                                Forward = CheckForwarding(MessageString);
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

bool CGProxy::CheckForwarding(string MessageString)
{
    bool Forward = true;
    string Command = MessageString;
    transform(Command.begin(), Command.end(), Command.begin(), (int(*)(int))tolower);

    if (Command.size() >= 1 && Command.substr(0, 1) == "/")
    {
        Forward = false;

        if (Command.substr(0, 3) == "/p " || Command.substr(0, 8) == "/phrase ")
        {
            string filePath = "phrase/";

            if (Command.substr(0, 3) == "/p ")
                filePath.append(Command.substr(3));
            else if (Command.substr(0, 8) == "/phrase ")
                filePath.append(Command.substr(8));

            if (!textEndsWith(filePath, ".txt"))
            {
                filePath.append(".txt");
            }
            //
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
//                                Sleep(50);
                            }
                        }
                        else
                        {
                            gproxy->m_BNET->QueueChatCommand(tmp.substr(1));
                        }
                    }
                    else if (messageText[0] != "#"[0])
                        gproxy->m_BNET->QueueChatCommand(tmp);
                }
                while (!infile.eof());
                //SendLocalChat("->>>>>>finished sending->>>>>>>>");
                infile.close();
            }
            else
            {
                CONSOLE_Print("[ERROR] File \"" + QString::fromStdString(help) + "\" does not exist!");
                SendLocalChat(string() + "File \"" + help + "\" does not exist!");
            }
        }
        else if (Command.size() >= 5 && Command.substr(0, 4) == "/re ")
        {
            if (m_BNET->GetLoggedIn())
            {
                if (!m_BNET->GetReplyTarget().empty())
                {
                    m_BNET->QueueChatCommand(MessageString.substr(4), m_BNET->GetReplyTarget(), true);
                    SendLocalChat("Whispered to " + m_BNET->GetReplyTarget() + ": " + MessageString.substr(4));
                }
                else
                    SendLocalChat("Nobody has whispered you yet.");
            }
            else
                SendLocalChat("You are not connected to battle.net.");
        }
        else if (Command == "/sc" || Command == "/spoof" || Command == "/spoofcheck" || Command == "/spoof check")
        {
            if (m_BNET->GetLoggedIn())
            {
                if (!m_GameStarted)
                {
                    m_BNET->QueueChatCommand("spoofcheck", m_HostName, true);
                    SendLocalChat("Whispered to " + m_HostName + ": spoofcheck");
                }
                else
                    SendLocalChat("The game has already started.");
            }
            else
                SendLocalChat("You are not connected to battle.net.");
        }
        else if (Command.substr(0, 4) == "/wh ")
        {
            string mess = Command.substr(4, Command.size() - 4);
            m_BNET->QueueChatCommand(mess, m_HostName, true);
            SendLocalChat("Whispered to host [" + m_HostName + "] " + mess);
        }
        else if (Command.substr(0, 5) == "/whs ")
        {
            string mess = Command.substr(5, Command.size() - 5);
            m_BNET->QueueChatCommand("!say " + mess, m_HostName, true);
            SendLocalChat("Whispered to host [" + m_HostName + "] !say " + mess);
        }
        else if (Command.substr(0, 5) == "/cmd ")
            m_BNET->QueueChatCommand(Command.substr(5, Command.size() - 5));
        else if (Command == "/host")
            SendLocalChat("Hosting player/bot is [" + m_HostName + "]. (use '/wh <message>' to whisper to him)");
        else if (Command == "/status")
        {
            if (m_LocalSocket)
            {
                if (m_GameIsReliable && m_ReconnectPort > 0)
                    SendLocalChat("GProxy++ disconnect protection: Enabled");
                else
                    SendLocalChat("GProxy++ disconnect protection: Disabled");

                if (m_BNET->GetLoggedIn())
                    SendLocalChat("battle.net: Connected");
                else
                    SendLocalChat("battle.net: Disconnected");
            }
        }
        else if (Command.size() >= 4 && Command.substr(0, 3) == "/w ")
        {
            if (m_BNET->GetLoggedIn())
            {
                int Messagebegins = Command.find_first_of(" ", 4);

                if (Messagebegins != -1)
                {
                    m_BNET->QueueChatCommand(MessageString);
                    SendLocalChat("Whisper to " + MessageString.substr(3, Messagebegins - 3) + ": " + MessageString.substr(Messagebegins));
                }
            }
            else
                SendLocalChat("You are not connected to battle.net.");
        }
        else if (Command.size() >= 2 && Command.substr(0, 2) == "/s" && Command.substr(0, 3) != "/sd")
        {
            QString message;

            if (Command.size() == 2)
            {
                message = "!stats";
            }
            else
            {
                message = QString("!stats").append(
                        QString::fromStdString(Command.substr(2)));
            }

            if (!gproxy->m_BNET->GetInGame())
            {
                gproxy->m_BNET->QueueChatCommand(message.toStdString());
            }
            else
            {
                gproxy->sendGamemessage(message);
            }
        }
        else if (Command.size() >= 3 && Command.substr(0, 3) == "/sd")
        {
            QString message;

            if (Command.size() == 3)
            {
                message = "!statsdota";
            }
            else
            {
                message = QString("!statsdota").append(
                        QString::fromStdString(Command.substr(3)));
            }

            if (!gproxy->m_BNET->GetInGame())
            {
                gproxy->m_BNET->QueueChatCommand(message.toStdString());
            }
            else
            {
                gproxy->sendGamemessage(message);
            }
        }
    }
    else if (Command.size() >= 1 && Command.substr(0, 1) == "!")
    {
        if (Command.substr(0, 6) == "!stats" || Command.substr(0, 10) == "!statsdota")
        {
            return true;
        }
        else if (Command.size() >= 2 && Command.substr(0, 2) == "!s" && Command.substr(0, 3) != "!sd")
        {
            QString message;

            if (Command == "!s")
            {
                message = "!stats";
            }
            else if (Command.substr(2, 1) == " ")
            {
                message = QString("!stats").append(
                        QString::fromStdString(Command.substr(2)));
            }

            if (!gproxy->m_BNET->GetInGame())
            {
                gproxy->m_BNET->QueueChatCommand(message.toStdString());
            }
            else
            {
                gproxy->sendGamemessage(message);
            }
        }
        else if (Command.size() >= 3 && Command.substr(0, 3) == "!sd")
        {
            QString message;

            if (Command == "!sd")
            {
                message = "!statsdota";
            }
            else if (Command.substr(3, 1) == " ")
            {
                message = QString("!statsdota").append(
                        QString::fromStdString(Command.substr(3)));
            }

            if (!gproxy->m_BNET->GetInGame())
            {
                gproxy->m_BNET->QueueChatCommand(message.toStdString());
            }
            else
            {
                gproxy->sendGamemessage(message);
            }
        }
    }

    return Forward;
}

void CGProxy::ProcessLocalPackets()
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

                        for (vector<CIncomingGameHost *> ::iterator i = m_Games.begin(); i != m_Games.end(); i++)
                        {
                            if (((CIncomingGameHost *) (*i))->GetUniqueGameID() == EntryKey)
                            {
                                CONSOLE_Print("[GPROXY] local player requested game name [" + QString::fromStdString(((CIncomingGameHost *) (*i))->GetGameName()) + "]");

                                if (NameString != username.toStdString())
                                    CONSOLE_Print("[GPROXY] using battle.net name [" + username + "] instead of requested name [" + QString::fromStdString(NameString) + "]");

                                CONSOLE_Print("[GPROXY] connecting to remote server [" + QString::fromStdString(((CIncomingGameHost *) (*i))->GetIPString()) + "] on port " + QString::number(((CIncomingGameHost *) (*i))->GetPort()));
                                m_RemoteServerIP = ((CIncomingGameHost *) (*i))->GetIPString();
                                m_RemoteServerPort = ((CIncomingGameHost *) (*i))->GetPort();
                                m_RemoteSocket->Reset();
                                m_RemoteSocket->SetNoDelay(true);
                                m_RemoteSocket->Connect(string(), m_RemoteServerIP, m_RemoteServerPort);
                                m_LastConnectionAttemptTime = getElapsedSeconds();
                                m_GameIsReliable = (((CIncomingGameHost *) (*i))->GetMapWidth() == 1984 && ((CIncomingGameHost *) (*i))->GetMapHeight() == 1984);
                                m_GameStarted = false;

                                // rewrite packet

                                BYTEARRAY DataRewritten;
                                DataRewritten.push_back(W3GS_HEADER_CONSTANT);
                                DataRewritten.push_back(Packet->GetID());
                                DataRewritten.push_back(0);
                                DataRewritten.push_back(0);
                                UTIL_AppendByteArray(DataRewritten, ((CIncomingGameHost *) (*i))->GetHostCounter(), false);
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
                                m_BNET->QueueJoinGame(((CIncomingGameHost *) (*i))->GetGameName());

                                // save the hostname for later (for manual spoof checking)
                                m_JoinedName = NameString;
                                m_HostName = ((CIncomingGameHost *) (*i))->GetHostName();
                                GameFound = true;

                                // Manufactorings work
                                m_GameName = ((CIncomingGameHost *) (*i))->GetGameName();
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
                slotList.clear();
                gproxy->m_BNET->QueueChatCommand(string("/whois ").append(gproxy->m_HostName));
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

void CGProxy::ExtractRemotePackets()
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

void CGProxy::ProcessRemotePackets()
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
                    string playerName = players[FromPID].GetName();

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
                        LOG_Print("[LOBBY][" + playerName + "] " + MessageString);
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

                /*for( vector<CIncomingSlots *> :: iterator it = slotList.begin( ); it != slotList.end( ); it++ )
                {
                        if( (*it)->GetPID() == ((int) Data[4]) )
                        {
                                delete *it;
                                it = slotList.erase(it);
                                break;
                        }
                }*/

                if (gproxy->m_GameStarted)
                {
                    if (gproxy->m_PlaySound)
                    {
                        QSound::play("sounds/leaver_detected.wav");
                    }
                    CONSOLE_Print("[ALL] " + QString::fromStdString(players[((int) Data[4])].GetName()) + " has left the game.");
                }
                else
                    CONSOLE_Print("[LOBBY] " + QString::fromStdString(players[((int) Data[4])].GetName()) + " has left the game.");
            }
            if (Packet->GetID() == CGameProtocol::W3GS_SLOTINFOJOIN)
            {
                BYTEARRAY Data = Packet->GetData();

                if (Data.size() >= 6)
                {
                    unsigned int SlotInfoSize = ((unsigned int) UTIL_ByteArrayToUInt16(Data, false, 4));

                    if (Data.size() >= 7 + SlotInfoSize)
                    {
                        m_ChatPID = Data[6 + SlotInfoSize];
                        players[((int) m_ChatPID)].SetName(username.toStdString());
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
                    players[((int) m_ChatPID)].SetName(username.toStdString());
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
                            ((CIncomingSlots *) (*it))->SetName(players[((int) Data[i])].GetName());

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
                int PID = (int) data[i];
                i += 1;
                BYTEARRAY Name = UTIL_ExtractCString(data, i);
                string playersName = UTIL_ToColoredText(string(Name.begin(), Name.end()));

                //playerNames[PID] = playersName;

                players[PID] = CPlayer(playersName);
                CONSOLE_Print("[LOBBY] " + QString::fromStdString(playersName) + " has joined the game.");

//                for (vector<CIncomingFriendList *> ::iterator i = vFriendList.begin(); i != vFriendList.end(); i++)
//                {
//                    if (((CIncomingFriendList *) (*i))->GetAccount().compare(playersName) == 0)
//                    {
//                        QSound::play("sounds/vip_joins.wav");
//                    }
//                }
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
                    CONSOLE_Print("[Manufactoring] "
                            + QString::fromStdString(players[((int) Packet->GetData()[4])].GetName())
                            + " has finished loading. Loading time: " + QString::number(sec) + "," + QString::number(dsec) + " seconds.");
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
                    SendLocalChat("Using battle.net name \"" + username.toStdString() + "\" instead of LAN name \"" + m_JoinedName + "\".");

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
                    SendLocalChat("GProxy++ disconnect protection is ready (" + UTIL_ToString((m_NumEmptyActions + 1) * 60) + " second buffer).");
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

bool CGProxy::AddGame(CIncomingGameHost *game)
{
    // check for duplicates and rehosted games

    bool DuplicateFound = false;
    uint32_t OldestReceivedTime = getElapsedSeconds();

    for (vector<CIncomingGameHost *> ::iterator i = m_Games.begin(); i != m_Games.end(); i++)
    {
        if (game->GetIP() == ((CIncomingGameHost *) (*i))->GetIP() && game->GetPort() == ((CIncomingGameHost *) (*i))->GetPort())
        {
            // duplicate or rehosted game, delete the old one and add the new one
            // don't forget to remove the old one from the LAN list first

            m_UDPSocket->Broadcast(6112, m_GameProtocol->SEND_W3GS_DECREATEGAME(((CIncomingGameHost *) (*i))->GetUniqueGameID()));
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
        for (vector<CIncomingGameHost *> ::iterator i = m_Games.begin(); i != m_Games.end(); i++)
        {
            if (game->GetGameName() != m_BNET->GetSearchGameName() && game->GetReceivedTime() == OldestReceivedTime)
            {
                m_UDPSocket->Broadcast(6112, m_GameProtocol->SEND_W3GS_DECREATEGAME(((CIncomingGameHost *) (*i))->GetUniqueGameID()));
                delete *i;
                m_Games.erase(i);
                break;
            }
        }
    }

    return !DuplicateFound;
}

void CGProxy::sendGamemessage(QString message, bool alliesOnly)
{
    if(message.isEmpty() || !gproxy->m_BNET->GetInGame())
    {
        return;
    }

    if (message.length() > 254)
    {
        message = message.left(254);
    }

    BYTEARRAY toPIDs;
    BYTEARRAY packet;

    if(!gproxy->m_GameStarted)
    {
        for (vector<CIncomingSlots *> ::iterator it = slotList.begin(); it != slotList.end(); it++)
        {
            toPIDs.push_back(((CIncomingSlots *) (*it))->GetPID());
        }

        packet = gproxy->m_GameProtocol->SEND_W3GS_CHAT_TO_HOST(gproxy->m_ChatPID, toPIDs, 16, BYTEARRAY(), message.toStdString());
    }
    else if(alliesOnly)
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

        packet = gproxy->m_GameProtocol->SEND_W3GS_CHAT_TO_HOST(gproxy->m_ChatPID, toPIDs, 32, extraFlags, message.toStdString());
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

        packet = gproxy->m_GameProtocol->SEND_W3GS_CHAT_TO_HOST(gproxy->m_ChatPID, toPIDs, 32, extraFlags, message.toStdString());
    }

    if (CheckForwarding(message.toStdString()))
    {
        m_PacketBuffer.push(new CCommandPacket(W3GS_HEADER_CONSTANT, packet[1], packet));
        m_LocalPackets.push(new CCommandPacket(W3GS_HEADER_CONSTANT, packet[1], packet));
        m_TotalPacketsReceivedFromLocal++;
    }
}

void CGProxy::changeTeam(unsigned char team)
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

void CGProxy::SendLocalChat(string message)
{
    if (m_LocalSocket)
    {
        if (m_GameStarted)
        {
            if (message.size() > 127)
                message = message.substr(0, 127);

            m_LocalSocket->PutBytes(m_GameProtocol->SEND_W3GS_CHAT_FROM_HOST(m_ChatPID, UTIL_CreateByteArray(m_ChatPID), 32, UTIL_CreateByteArray((uint32_t) 0, false), message));
        }
        else
        {
            if (message.size() > 254)
                message = message.substr(0, 254);

            m_LocalSocket->PutBytes(m_GameProtocol->SEND_W3GS_CHAT_FROM_HOST(m_ChatPID, UTIL_CreateByteArray(m_ChatPID), 16, BYTEARRAY(), message));
        }
    }
}

void CGProxy::SendEmptyAction()
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
            ConfigGUI *configGUI = new ConfigGUI(gproxy->getConfig());

            if (configGUI->exec() == QDialog::Accepted)
            {
                CONSOLE_Print("[GPROXY] Configuration file successfully saved and initialized.");
                return true;
            }
            else
            {
                CONSOLE_Print("[ERROR] Could not save configuration file. Aborted by User interaction.");

                QMessageBox msgBox;
                msgBox.setWindowIcon(QIcon(":/images/Error.png"));
                msgBox.setWindowTitle("Error");
                msgBox.setText("Could not save configuration file.\n"
                        "Aborted by User interaction."
                        "\nShutting down GProxy.");
                msgBox.exec();

                return false;
            }
        }
        case 2:
        {
            CONSOLE_Print("[ERROR] Error while loading configuration file. Check your file permissions.");

            QMessageBox msgBox;
            msgBox.setWindowIcon(QIcon(":/images/Error.png"));
            msgBox.setWindowTitle("Error");
            msgBox.setText("Could not access the configuration file.\n"
                    "Maybe you dont have permissions to write to that directory.\n"
                    "Try to move your folder outside \"C:\\Program Files\" or run as administrator."
                    "\nShutting down GProxy.");
            msgBox.exec();

            return false;
        }
        default:
        {
            CONSOLE_Print("[ERROR] Unknown error while loading configuration file.\n"
                    "Error code: " + QString::number(statusCode));

            QMessageBox msgBox;
            msgBox.setWindowIcon(QIcon(":/images/Error.png"));
            msgBox.setWindowTitle("Error");
            msgBox.setText("Unknown error while loading configuration file.\n"
                    "ErrorCode: " + QString::number(statusCode));
            msgBox.exec();

            return false;
        }
    }
}

bool fcfgfilterfirst()// Phyton filter
{
    return gproxy->cfgfilterfirst;

}

string fcfgfilter()// Phyton filter
{
    return gproxy->cfgfilter;
}

void saychat(string message)// Phyton parrot
{
    gproxy->m_BNET->QueueChatCommand(message);
}

bool getautosearch() //pr0 autosearch
{
    return gproxy->autosearch;
}

void autosearch(bool autosearchNew) //pr0 autosearch
{
    gproxy->autosearch = autosearchNew;
}

bool cautosearch() //pr0 cautosearch
{
    return gproxy->cautosearch;
}

bool displayautocreated()
{
    return gproxy->displayautocreated;
}

void displayautocreated(bool newone)
{
    gproxy->displayautocreated = newone;
}

void flisting_current_games(bool newone)
{
    gproxy->m_listing_current_games = newone;
}

bool flisting_current_games()
{
    return gproxy->m_listing_current_games;
}

CIncomingSlots::CIncomingSlots(unsigned char nPID, unsigned char ndownloadStatus, unsigned char nslotStatus,
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

CIncomingSlots::~CIncomingSlots() { }