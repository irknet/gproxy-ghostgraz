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

#include <Config.h>
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


using namespace std;

typedef vector<unsigned char> BYTEARRAY;

// time

unsigned int GetTime( );		// seconds
unsigned int GetTicks( );		// milliseconds

#ifdef WIN32
 #define MILLISLEEP( x ) Sleep( x )
#else
 #define MILLISLEEP( x ) usleep( ( x ) * 1000 )
#endif

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

class CIncomingSlots
{
private:
	int PID;
	int downloadStatus;
	int slotStatus;
	int computerStatus;
	int team;
	int color;
	int race;
	int computerType;
	int handicap;
	string name;

public:
	CIncomingSlots( unsigned char PID, unsigned char downloadStatus, unsigned char slotStatus,
	unsigned char computerStatus,	unsigned char team, unsigned char color, unsigned char race,
	unsigned char computerType, unsigned char handicap );
	~CIncomingSlots( );

	int GetPID( )									{ return PID; }
	int GetDownloadStatus( )						{ return downloadStatus; }
	int GetSlotStatus( )							{ return slotStatus; }
	int GetComputerStatus( )						{ return computerStatus; }
	int GetTeam( )									{ return team; }
	int GetColor( )									{ return color; }
	int GetRace( )									{ return race; }
	int GetComputerType( )							{ return computerType; }
	int GetHandicap( )								{ return handicap; }
	string GetName( )								{ return name; }
	void SetPID				( int nPID )			{ PID =				nPID; }
	void SetDownloadStatus	( int ndownloadStatus )	{ downloadStatus =	ndownloadStatus; }
	void SetSlotStatus		( int nslotStatus )		{ slotStatus =		nslotStatus; }
	void SetComputerStatus	( int ncomputerStatus )	{ computerStatus =	ncomputerStatus; }
	void SetTeam			( int nteam )			{ team =			nteam; }
	void SetColor			( int ncolor )			{ color =			ncolor; }
	void SetRace			( int nrace )			{ race =			nrace; }
	void SetComputerType	( int ncomputerType )	{ computerType =	ncomputerType; }
	void SetHandicap		( int nhandicap )		{ handicap =		nhandicap; }
	void SetName			( string nname )		{ name =			nname; }
};

class CPlayer
{
private:
	string name;
	unsigned int stayPercent;

public:
	CPlayer( ) { }
	CPlayer( string nname ) { name = nname; }
	~CPlayer( ) { }

	string GetName( ) { return name; }
	unsigned int GetStayPercent( ) { return stayPercent; }
	void SetName( string nname ) { name = nname; }
	void SetStayPercent( unsigned int nstayPercent ) { stayPercent = nstayPercent; }
};

// output

void LOG_Print( string message );
void CONSOLE_Print( QString message, bool log = true );
//void CONSOLE_ChangeChannel( string channel );
//void CONSOLE_AddChannelUser( string name, string clanTag );
//void CONSOLE_RemoveChannelUser( string name );
//void CONSOLE_RemoveChannelUsers( );
//void CONSOLE_FriendsReceived(vector<CIncomingFriendList *> Friends);
void CheckForGame( string gamename );//phy core
string parrot();//phy parrot
string IntToString(int i);
void Pspoofcheck( );//phy autostpoofcheck
bool fcfgfilterfirst();//phy filter
string fcfgfilter();//phy filter
void saychat(string message);//phy parrot
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

private:
    Config* config;
    QString privategamename;
    QString botprefix;
    bool vShallCreate;
    bool vShallCreateQuiet;
    QString parrot;
    QString war3Path;
    QString cdKeyROC;
    QString cdKeyTFT;
    bool dotaMap;

public:
    string m_Version;
    CTCPServer *m_LocalServer;
    CTCPSocket *m_LocalSocket;
    CTCPClient *m_RemoteSocket;
    CUDPSocket *m_UDPSocket;
    CBNET* m_BNET;
    vector<CIncomingGameHost *> m_Games;
    CGameProtocol* m_GameProtocol;
    CGPSProtocol* m_GPSProtocol;
    queue<CCommandPacket *> m_LocalPackets;
    queue<CCommandPacket *> m_RemotePackets;
    queue<CCommandPacket *> m_PacketBuffer;
    vector<unsigned char> m_Laggers;
    uint32_t m_TotalPacketsReceivedFromLocal;
    uint32_t m_TotalPacketsReceivedFromRemote;
    bool m_Exiting;
    bool m_TFT;
    string m_War3Path;
    string m_CDKeyROC;
    string m_CDKeyTFT;
    string m_Server;
    QString username;
    string m_Password;
    string m_Channel;
    uint32_t m_War3Version;
    uint16_t m_Port;
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

    string cfgpublic;//phy public
    string cfgfilter;//phy filter
    bool cfgfilterfirst;//phy filter
    bool testvar;//phy
    bool autodetect;
    bool autosearch; //pr0 autosearch
    bool cautosearch; //pr0 cautosearch
    bool displayautocreated;
    bool m_listing_current_games;

    CGProxy();
    ~CGProxy( );

    void init(string nServer, string nUsername, string nPassword, string nChannel, uint32_t nWar3Version, uint16_t nPort, BYTEARRAY nEXEVersion, BYTEARRAY nEXEVersionHash, string nPasswordHashType, string cpublic ,string cfilter,bool german_languagesupport, bool casearch, bool temp_displayautocreated, bool listing_current_games, int channelWidth);
    void cleanup();

    // processing functions

    bool Update( long usecBlock );

    void ExtractLocalPackets( );
    void ProcessLocalPackets( );
    void ExtractRemotePackets( );
    void ProcessRemotePackets( );

    bool AddGame( CIncomingGameHost *game );
    void SendLocalChat( string message );
    void SendAllMessage ( string message );			// Manufactoring
    void SendAllyMessage ( string message );		// Manufactoring
    void SendLobbyMessage ( string message );		// Manufactoring
    bool CheckForwarding ( string MessageString );	// Manufactoring
    void SendChangeTeam( unsigned char team );		// Manufactoring
    void SendEmptyAction( );

    void setPrivategamename(QString privategamename) { this->privategamename = privategamename; }
    void setBotprefix(QString botprefix) { this->botprefix = botprefix; }
    void setVShallCreate(bool vShallCreate) { this->vShallCreate = vShallCreate; }
    void setVShallCreateQuiet(bool vShallCreateQuiet) { this->vShallCreateQuiet = vShallCreateQuiet; }
    void setParrot(QString parrot) { this->parrot = parrot; }
    void setWar3Path(QString war3Path) { this->war3Path = war3Path; }
    void setCDKeyTFT(QString cdKeyTFT) { this->cdKeyTFT = cdKeyTFT; }
    void setCDKeyROC(QString cdKeyROC) { this->cdKeyROC = cdKeyROC; }
    void setDotaMap(bool dotaMap) { this->dotaMap = dotaMap; }
    void setConfig(Config* config) { this->config = config; }

    QString getPrivategamename() { return privategamename; }
    QString getBotprefix() { return botprefix; }
    bool getVShallCreate() { return vShallCreate; }
    bool getVShallCreateQuiet() { return vShallCreateQuiet; }
    QString getParrot() { return parrot; }
    QString getWar3Path() { return war3Path; }
    QString getCDKeyTFT() { return cdKeyTFT; }
    QString getCDKeyROC() { return cdKeyROC; }
    bool isDotaMap() { return dotaMap; }
    Config* getConfig() { return config; }
    bool checkStatus( int statusCode );

    void addMessage(QString msg, bool log = true);
    void changeChannel(QString channel);
    void addChannelUser(QString username, QString clanTag);
    void removeChannelUser(QString username);
    void friendUpdate(vector<CIncomingFriendList *> friendList);
    void clearFriendlist();
    void addFriend(QString username, bool online);

signals:
    void signal_addMessage(QString, bool);
    void signal_changeChannel(QString);
    void signal_addChannelUser(QString, QString);
    void signal_removeChannelUser(QString);
    void signal_clearFriendlist();
    void signal_addFriend(QString, bool);
    void signal_setGameslots(vector<CIncomingSlots*>);
};


#endif
