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

#ifndef GPROXY_H
#define GPROXY_H

// standard integer sizes for 64 bit compatibility
#include <stdint.h>

#include "Config.h"
#include "data/Player.h"
#include "data/Slot.h"
#include "data/ColoredMessage.h"
#include "data/Friend.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include <QString>
#include <QObject>
#include <QVector>
#include <QList>

using namespace std;

typedef vector<unsigned char> BYTEARRAY;

// time
unsigned long getElapsedSeconds();
unsigned long getElapsedMilliseconds();

// network

#undef FD_SETSIZE
#define FD_SETSIZE 512

class CIncomingFriendList
{
private:
	string m_Account;
	unsigned char m_Status;
	unsigned char m_Area;
	string m_Location;

public:
	CIncomingFriendList( string nAccount, unsigned char nStatus, unsigned char nArea, string nLocation );
	~CIncomingFriendList( );

	string GetAccount( )		{ return m_Account; }
	unsigned char GetStatus( )	{ return m_Status; }
	unsigned char GetArea( )	{ return m_Area; }
	string GetLocation( )		{ return m_Location; }
	string GetDescription( );

private:
	string ExtractStatus( unsigned char status );
	string ExtractArea( unsigned char area );
	string ExtractLocation( string location );
};

// output

void LOG_Print( const QString& message, bool printTimestamp = true, bool lineBreak = true );
void CONSOLE_Print( const ColoredMessage& coloredMessage, bool log = true, bool printTimestamp = true, bool lineBreak = true);
void CheckForGame( string gamename );//phy core
string parrot();//phy parrot
string IntToString(int i);
bool fcfgfilterfirst();//phy filter
string fcfgfilter();//phy filter
bool autodetect();
bool getautosearch(); //pr0 autosearch
void autosearch(bool autosearchNew); //pr0 autosearch
bool cautosearch(); //pr0 cautosearch
bool displayautocreated();
void displayautocreated(bool newone);
void AddGamesFromTextFile (string Command,string gamemode);
void AddGamesFromTextFile (string Command,string gamemode);
void AddGamesFromTextFile(string filename, string gamemode);
void AddGamesFromTextFile(string filename);
bool flisting_current_games();
void flisting_current_games(bool newone);
void downloadfile(string url, string filename);
void ConvertHTMLtoTXT(string html, string txt, string flag);


//
// CGProxy
//

class CTCPServer;
class CTCPSocket;
class CTCPClient;
class CUDPSocket;
class CBNET;
class CIncomingGameHost;
class CGameProtocol;
class CGPSProtocol;
class CCommandPacket;

class CGProxy : public QObject
{
    Q_OBJECT

public:
    // <editor-fold defaultstate="collapsed" desc="Attributes">
    // TODO Make these attributes private and create setters and getters.
    string m_Version;
    CTCPServer* m_LocalServer;
    CTCPSocket* m_LocalSocket;
    CTCPClient* m_RemoteSocket;
    CUDPSocket* m_UDPSocket;
    CBNET* m_BNET;
    vector<CIncomingGameHost*> m_Games;
    CGameProtocol* m_GameProtocol;
    CGPSProtocol* m_GPSProtocol;
    queue<CCommandPacket*> m_LocalPackets;
    queue<CCommandPacket*> m_RemotePackets;
    queue<CCommandPacket*> m_PacketBuffer;
    vector<unsigned char> m_Laggers;
    uint32_t m_TotalPacketsReceivedFromLocal;
    uint32_t m_TotalPacketsReceivedFromRemote;
    bool m_Exiting;
    bool m_TFT;
    string m_War3Path;
    string m_CDKeyROC;
    string m_CDKeyTFT;
    uint32_t m_LastConnectionAttemptTime;
    uint32_t m_LastRefreshTime;
    string m_RemoteServerIP;
    uint16_t m_RemoteServerPort;
    bool m_GameIsReliable;
    bool m_GameStarted;
    bool m_LeaveGameSent;
    bool m_ActionReceived;
    bool m_Synchronized;
    uint16_t m_ReconnectPort;
    unsigned char m_PID;
    unsigned char m_ChatPID;
    uint32_t m_ReconnectKey;
    unsigned char m_NumEmptyActions;
    unsigned char m_NumEmptyActionsUsed;
    unsigned int m_LastAckTime;
    unsigned int m_LastActionTime;
    string m_JoinedName;
    string m_HostName;

    string m_GameName;
    int m_ChannelWidth;
    bool m_PlaySound;
    bool m_joinStats;

    string cfgpublic;//phy public
    string cfgfilter;//phy filter
    bool cfgfilterfirst;//phy filter
    bool testvar;//phy
    bool autodetect;
    bool autosearch; //pr0 autosearch
    bool cautosearch; //pr0 cautosearch
    bool displayautocreated;
    bool m_listing_current_games;
    uint32_t hostCounter;
    //</editor-fold>

    CGProxy();
    ~CGProxy( );
    void connectSignalsAndSlots();
    void initVariables(string cpublic ,string cfilter, bool temp_displayautocreated,
        bool listing_current_games, bool connect);
    void initConfigurations();
    void cleanup();
    bool checkStatus( int statusCode );
    bool Update( long usecBlock );

    void ExtractLocalPackets( );
    void ProcessLocalPackets( );
    void ExtractRemotePackets( );
    void ProcessRemotePackets( );

    bool AddGame( CIncomingGameHost* game );
    void SendLocalChat( QString message );
    void sendGamemessage(QString message, bool alliesOnly = false);
    bool CheckForwarding ( QString message );
    void changeTeam( unsigned char team );
    void SendEmptyAction( );
    void showWelcomeMessages();
    void spoofcheck();
    void leaveLobby();

    // <editor-fold defaultstate="collapsed" desc="Getters and setters">
    void setServer(const QString& server);
    void setUsername(const QString& username);
    void setPassword(const QString& password);
    void setWar3version(const uint32_t& war3version);
    void setPort(const uint16_t& port);
    void setExeversion(const BYTEARRAY& exeversion);
    void setExeversionhash(const BYTEARRAY& exeversionhash);
    void setPasswordhashtype(const QString& passwordhashtype);
    void setChannel(const QString& channel);
    void setPlayers(const QList<Player*>& players);
    void setSlotList(const QList<Slot*> slotList);

    QString getServer();
    QString getUsername();
    QString getPassword();
    uint32_t getWar3version();
    uint16_t getPort();
    BYTEARRAY getExeversion();
    BYTEARRAY getExeversionhash();
    QString getPasswordhashtype();
    QString getChannel();
    QList<Player*> getPlayers();
    Player* getLastLeaver();
    QList<Slot*> getSlotList();

    // FIXME Implementation in source file.
    void setPrivategamename(QString privategamename) { this->privategamename = privategamename; }
    void setBotprefix(QString botprefix) { this->botprefix = botprefix; }
    void setVShallCreate(bool vShallCreate) { this->vShallCreate = vShallCreate; }
    void setVShallCreateQuiet(bool vShallCreateQuiet) { this->vShallCreateQuiet = vShallCreateQuiet; }
    void setParrot(QString parrot) { this->parrot = parrot; }
    void setWar3Path(QString war3Path) { this->war3Path = war3Path; }
    void setCDKeyTFT(QString cdKeyTFT) { this->cdKeyTFT = cdKeyTFT; }
    void setCDKeyROC(QString cdKeyROC) { this->cdKeyROC = cdKeyROC; }
    void setCdKeyOwner(QString owner) { this->cdKeyOwner = owner; }
    void setDotaMap(bool dotaMap) { this->dotaMap = dotaMap; }
    void setConfig(Config *config) { this->config = config; }

    QString getPrivategamename() { return privategamename; }
    QString getBotprefix() { return botprefix; }
    bool getVShallCreate() { return vShallCreate; }
    bool getVShallCreateQuiet() { return vShallCreateQuiet; }
    QString getParrot() { return parrot; }
    QString getWar3Path() { return war3Path; }
    QString getCDKeyTFT() { return cdKeyTFT; }
    QString getCDKeyROC() { return cdKeyROC; }
    QString getCdKeyOwner() { return cdKeyOwner; }
    
    bool isDotaMap() { return dotaMap; }
    Config *getConfig() { return config; }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Signal emitting functions">
    void addMessage(const ColoredMessage& coloredMessage, bool log = true, bool printTimestamp = true, bool lineBreak = true);
    void changeChannel(QString channel);
    void addChannelUser(QString username, QString clanTag);
    void removeChannelUser(QString username);
    void updateFriendlist(QList<Friend*> friends);
    void updateFriend(Friend* f);
    void addFriend(Friend* f);
    void removeFriend(unsigned char entryNumber);
    void showErrorMessage(QString errorMessage);
    // </editor-fold>

public slots:
    void applyConfig();

private:
    // <editor-fold defaultstate="collapsed" desc="Attributes">
    Config* config;
    QString server;
    QString username;
    QString password;
    uint32_t war3version;
    uint16_t port;
    BYTEARRAY exeversion;
    BYTEARRAY exeversionhash;
    QString passwordhashtype;
    QString channel;
    QString privategamename;
    QString botprefix;
    bool vShallCreate;
    bool vShallCreateQuiet;
    QString parrot;
    QString war3Path;
    QString cdKeyROC;
    QString cdKeyTFT;
    QString cdKeyOwner;
    bool dotaMap;
    QList<Slot*> slotList;
    QList<Player*> players;
    Player* lastLeaver;
    // </editor-fold>

signals:
    void signal_startUpdateThread();
    void signal_addMessage(ColoredMessage, bool, bool, bool);
    void signal_changeChannel(QString);
    void signal_addChannelUser(QString, QString);
    void signal_removeChannelUser(QString);
    void signal_updateFriendlist(QList<Friend*>);
    void signal_updateFriend(Friend*);
    void signal_addFriend(Friend*);
    void signal_removeFriend(unsigned char);
    void signal_setGameslots(QList<Slot*>);
    void signal_showErrorMessage(QString);
    void signal_playerJoined(const ColoredMessage&);
    void signal_stopDownloadThread();
    void signal_showConfigDialog(bool);
    void signal_initConfigurations();
    void signal_applyConfig();
};


#endif
