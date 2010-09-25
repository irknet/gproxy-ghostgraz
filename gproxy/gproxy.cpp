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

#include "gproxy.h"
#include "util.h"
#include "config.h"
#include "socket.h"
#include "commandpacket.h"
#include "bnetprotocol.h"
#include "bnet.h"
#include "gameprotocol.h"
#include "gpsprotocol.h"

#include <signal.h>
//#include <mysql.h>
//#include <mysql/mysql.h>//dont work
//#include <curses.h>
//#include <iostream>
//#include <fstream>
#include <stdlib.h>
#include <Wininet.h>//phy !games
#pragma comment(lib,"wininet")//phy !games
#include <direct.h>//same
#include <MMSystem.h>
#pragma comment(lib,"winmm.lib")
//#include <stdlib.h>//same
#include <sstream>
//#include <afxwin.h> //lag
#include <Wininet.h>//phy dl
//#include <shlwapi.h> //phy stats
//#include <conio.h>
//phy dl 
#include <urlmon.h>
#include <direct.h>
//phy dl
//phy p
#include <windows.h>

#pragma comment(lib,"urlmon") 
#pragma comment(lib,"wininet")

//tray phy/*
/*
#define ID_TASKBARICON 100
#define WM_ICONNOTIFY           (WM_USER + 101)*/
//prayasdksaf sdjkö

//#pragma comment(lib,"D:\\Tools\\lib\\opt\\libmysql.lib")


#ifdef WIN32
 #include <windows.h>
 #include <winsock.h>
#endif

#include <time.h>

#ifndef WIN32
 #include <sys/time.h>
#endif

#ifdef __APPLE__
 #include <mach/mach_time.h>
#endif

#ifdef WIN32
 #include "curses.h"
#else
 #include <curses.h>
#endif

bool gCurses = false;
vector<string> gMainBuffer;
vector<string> gGamesBuffer;
string gInputBuffer;
string gChannelName;
vector<string> gChannelUsers;
WINDOW *gMainWindow = NULL;
WINDOW *gBottomBorder = NULL;
WINDOW *gRightBorder = NULL;
WINDOW *gInputWindow = NULL;
WINDOW *gChannelWindow = NULL;
WINDOW *gGameWindow = NULL;
WINDOW *gMidBorder = NULL;
bool gMainWindowChanged = false;
bool gInputWindowChanged = false;
bool gChannelWindowChanged = false;
bool gGamesWindowChanged = false;
bool vShallCreate = false; //phy command waitgame
bool vShallCreateQuiet = false; //py command waitgame
string privategamename = "my gamename"; //phy command waitgame and priv
string botprefix = "GhostGraz"; //phy command waitgame
//bool autodetect; //phy autodetect
int GameNamesCount=0;
string GameNames[10]={"","","","","","","","","",""}; //phy insteandsend.
CGProxy *help_gproxy;
string cchatcommand;
//uint32_t PingPhy;//phy

// Manufactorings work
vector<CIncomingFriendList *> friendList;
string channelList[100];
vector<CIncomingSlots *> slots;
string playerNames[12];
bool DotAmap;
bool playerLoadingComplete[12];
int slotsWithPlayer;
int finishedLoadingCounter;
int teamNumber;
int teams;
bool gameLoaded = false;
uint32_t countdownEndTime;
vector<string> commandHistory;
int commandHistoryIndex = 0;

void addgame(string gamename)
{
	gGamesBuffer.push_back(gamename);
};


string parseTextline(string input)
{
	
	//return input;
	if (help_gproxy->m_LocalSocket&&(input.find("$host$")!=string::npos))
		input.replace(input.find("$host$"),6,help_gproxy->m_HostName);
	
	return input;

};


string gLogFile;
CGProxy *gGProxy = NULL;

string get_time()
{
	string temp;
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	strftime (buffer,80,"[%H:%M:%S]",timeinfo);
	return temp.assign(buffer);
}

void color (int c) //phy *inactive* try to change color of a single line
{
     if (c < 8) attron(COLOR_PAIR(c));
     else if (c == 7) attron(COLOR_PAIR(7));
     else if (c == 8) attron(COLOR_PAIR(0) | A_BOLD);
     else attron(COLOR_PAIR(c - 8) | A_BOLD);
}

const  int StrToInt(const std::string&  s)//phy stats
{
  return std::atoi(s.c_str());
}

bool autodetect()
{
	return gGProxy->autodetect;
}


string getfromtextfile(string input,string want)//phy dl3
{
	if (want == "botname")
	{
		return input.substr(0,input.find(","));
	}
	else if (want == "gamename")
	{
		input = input.substr(input.find(",")+1,input.size()-input.find(",")-1);
		return input.substr(0,input.find(","));
	}
	else if (want == "slots")
	{
		input = input.substr(input.find(",")+1,input.size()-input.find(",")-1);
		input = input.substr(input.find(",")+1,input.size()-input.find(",")-1);
		return input;

	}
	else if (want == "gamemode")
	{
		/*stringstream ss;
		string s;*/
		input = input.substr(input.find(",")+1,input.size()-input.find(",")-1);
		input = input.substr(0,input.find(","));
		/*ss <<input.find("-");
		ss >> s;
		CONSOLE_Print(s);*/
		if (input.find("-")>0 && input.find("-")<30 )
			input = input.substr(input.find("-")+1,input.size()-input.find("-")-1);
		else
			input = input.substr(input.find(" ")+1,input.size()-input.find(" ")-1);


		input = input.substr(0,input.find(" "));
		while (input[input.size()-1]<=57)
		{
			//CONSOLE_Print("fail");
			input=input.substr(0,input.size()-1);
		}
		return input;
	}
	else 
		return "<<<[error]>>>";
}
//phy autodetect

bool CheckAndAddGameName( string GameName )
{
	int i;
	transform ( GameName.begin(), GameName.end(), GameName.begin(), lower_case );
	bool res=false;
	for ( i = 0; i < 25; i++ )
		if (GameNames[i] == GameName)
			res = true;
	

	if ( !res )
	{
		GameNames[GameNamesCount%10]=GameName;
		GameNamesCount++;
		//CONSOLE_Print("exists not");
	} //else CONSOLE_Print("exists");
	return res;
}

bool textEndsWith( string text, string endText )
{
	if( text.length() > endText.length() )
	{
		if( 0 == text.compare(text.length() - endText.length(), endText.length(), endText) )
		{
			return true;
		}
	}

	return false;
}

uint32_t GetTime( )
{
	return GetTicks( ) / 1000;
}

uint32_t GetTicks( )
{
#ifdef WIN32
	return timeGetTime( );
#elif __APPLE__
	uint64_t current = mach_absolute_time( );
	static mach_timebase_info_data_t info = { 0, 0 };
	// get timebase info
	if( info.denom == 0 )
		mach_timebase_info( &info );
	uint64_t elapsednano = current * ( info.numer / info.denom );
	// convert ns to ms
	return elapsednano / 1e6;
#else
	uint32_t ticks;
	struct timespec t;
	clock_gettime( CLOCK_MONOTONIC, &t );
	ticks = t.tv_sec * 1000;
	ticks += t.tv_nsec / 1000000;
	return ticks;
#endif
}

void LOG_Print( string message )
{
	if( !gLogFile.empty( ) )
	{
		ofstream Log;
		Log.open( gLogFile.c_str( ), ios :: app );

		if( !Log.fail( ) )
		{
			time_t Now = time( NULL );
			string Time = asctime( localtime( &Now ) );

			// erase the newline

			Time.erase( Time.size( ) - 1 );
			Log << "[" << Time << "] " << message << endl;
			Log.close( );
		}
	}
}
/*
void  IntToString(int i, string& res)
{
    ostringstream temp;
    temp << i;
    res = temp.str();
}
*/
string IntToString(int i)
{
	ostringstream temp;
    temp << i;
    return temp.str();
}

void CheckForGame( string gamename ) //phy core alias searchforgame
{
	if (getautosearch())
	{
		gGProxy->m_BNET->SetSearchGameName( gamename ); //old one, but it works..
		CONSOLE_Print("[Phyton] Searching for ["+gamename+"].",true,1);
		autosearch(false);
	}
}

void Pspoofcheck( )//phy autospoofcheck
{
	if( gGProxy->m_BNET->GetLoggedIn( ) )
	{
		gGProxy->m_BNET->QueueChatCommand( "spoofcheck", gGProxy->m_HostName, true );
		//SendLocalChat( "Whispered to " + m_HostName + ": spoofcheck" );
	}
}

void CONSOLE_Print( string message, bool log , int vcol )
{
	//if (!(message.substr(0,18)=="[LOCAL] [GhostGraz") || displayautocreated())//!(m&&c)=!m||!c
	CONSOLE_PrintNoCRLF( message, log, vcol );
	//else 
	//	CONSOLE_PrintNoCRLF( "*nothing*", log, vcol );

	if( !gCurses )
		cout << endl;
}

void CONSOLE_FriendsReceived(vector<CIncomingFriendList *> Friends)
{
	friendList = Friends;
	gChannelWindowChanged = true;
	CONSOLE_Draw();
}

void CONSOLE_PrintNoCRLF( string message, bool log , int vcol )
{
	string s = message;
	
	if( s.substr(0,9)== "[WHISPER]" ||s.substr(0,12)=="[QUEUED] /w "|| s.substr(0,12)=="[WHISPER TO]")//black dgreen
		vcol=10;
	else if(s.substr(0,8)=="[Phyton]" || s.substr(0,9)=="[Pr0gm4n]"||s.substr(0,8)=="[CONFIG]"||s.substr(0,5)=="Bot [")
		vcol=2;
	else if( s.substr(0,15)=="[Manufactoring]" )
		vcol=14; // yellow
	else if(s.substr(0,7)=="[LOCAL]")
	{
		vcol=7;
		message=s.substr(8,s.size()-8);
		
	}
	else if (s.substr(0,8)=="[QUEUED]")
	{
		vcol=7;
		message="["+gGProxy->m_BNET->m_UserName+"]"+s.substr(8,s.size()-8);
	}
	else if(s.substr(0,6)=="[INFO]")//black cyan
	{
		vcol=11;
		message=s.substr(7,s.size()-7);
	}
	else if(s.substr(0,11)=="[TCPSOCKET]")//magenta white
		vcol=15+5*16;
	else if(s.substr(0,8)=="[GPROXY]" || s.substr(0,11)=="[UDPSOCKET]")//black magenta
		vcol=5;
	else if(s.substr(0,23)=="[BNET] joined channel [")//cyan white
		vcol=0+16*11;
	else if(s.substr(0,6)=="[BNET]")//red white
		vcol=4;
	else if(s.substr(0,7)=="[ERROR]" || s.substr(0,22)=="[Manufactoring][ERROR]")//red black
		vcol=0+16*12;
	else if(s.substr(0,7)=="[EMOTE]")
	{
		vcol=8;
		message=s.substr(8,s.size()-8);
	}
	//else vcol=15;

	if( textEndsWith( s, " has joined the game." ) || textEndsWith( s, " has left the game." ) )
	{
		vcol = 8;
	}

	if(vcol > 255 || vcol < 0)
		vcol = 15;

	string user = "no user";
	if( gGProxy && gGProxy->m_BNET )
		user = gGProxy->m_BNET->m_UserName;

	if (message.substr(0,16)!="["+user+"] /emote "&&message!="["+user+"] /emote")
	{
		gMainBuffer.push_back((char)6 + ((char)vcol + get_time()+" "+ message ));

		if( gMainBuffer.size( ) > 100 )
			gMainBuffer.erase( gMainBuffer.begin( ) );

		gMainWindowChanged = true;
		CONSOLE_Draw();

		if( log )
			LOG_Print( message );

		if( !gCurses )
			cout << message;
	}
}

void CONSOLE_ChangeChannel( string channel )
{
	gChannelName = channel;
	gChannelWindowChanged = true;
}


void CONSOLE_AddChannelUser( string name )
{
	if (vShallCreate)//phy waitgame
	{
		string tname=name;
		string tpre=botprefix;
		transform ( tpre.begin(), tpre.end(), tpre.begin(), lower_case );
		transform ( tname.begin(), tname.end(), tname.begin(), lower_case );
		if(botprefix.size()>0&&botprefix[0]!="^"[0])
			if(privategamename!=""&&tpre!=""&&tname.substr(0,tpre.size())==tpre)
			{
				gGProxy->m_BNET->QueueChatCommand("/w "+name+" !priv "+privategamename);
				autosearch(true);
				CheckForGame(privategamename);				
				if (!vShallCreateQuiet)
					gGProxy->m_BNET->QueueChatCommand("gn: "+privategamename);
				//CONSOLE_Print("off");
				vShallCreate = false;
			}else;
		else 
		{
			//CONSOLE_Print(tpre.substr(1,tpre.size()-1));
			//CONSOLE_Print(tname);
			if(privategamename!=""&&tname==tpre.substr(1,tpre.size()-1))
			{
		
			autosearch(true);
			gGProxy->m_BNET->QueueChatCommand("/w "+name+" !priv "+privategamename);
			CheckForGame(privategamename);
			if (!vShallCreateQuiet)
				gGProxy->m_BNET->QueueChatCommand("gn: "+privategamename);
				vShallCreate = false;
			}
		}
}//phy off
	for( vector<string> :: iterator i = gChannelUsers.begin( ); i != gChannelUsers.end( ); i++ )
	{
		if( *i == name )
			return;
	}

	gChannelUsers.push_back( name );
	gChannelWindowChanged = true;
}

void CONSOLE_RemoveChannelUser( string name )
{
	for( vector<string> :: iterator i = gChannelUsers.begin( ); i != gChannelUsers.end( ); )
	{
		if( *i == name )
			i = gChannelUsers.erase( i );
		else
			i++;
	}

	gChannelWindowChanged = true;
}

void CONSOLE_RemoveChannelUsers( )
{
	gChannelUsers.clear( );
	gChannelWindowChanged = true;
}

void CONSOLE_Draw()
{
	if( !gCurses )
		return;

	// draw main window
	//wattron(gMainWindow,COLOR_PAIR(vcolor));
		//setcolor(11);
	//SetConsoleTextAttribute( HANDLE(gMainWindow),COLOR_PAIR(vcolor));

	if( gMainWindowChanged )//py3
	{
		wclear( gMainWindow );
		wmove( gMainWindow, 0, 0 );

		for( vector<string> :: iterator i = gMainBuffer.begin( ); i != gMainBuffer.end( ); i++ )
		{
			//SetAttribute( data, name, flag, B_TAB, false );
//wattr_on( gMainWindow, COLOR_PAIR((int)((*i)[0])), 0 );

			for( string :: iterator j = (*i).begin( ); j != (*i).end( ); j++ )
			{

				if ( int(*j) == 6)
				{
					j++;
					wattr_on( gMainWindow, COLOR_PAIR((int)((*j))), 0 );
					j++;
				}

				signed char have = *j;
				unsigned char out;
				out = have;
				stringstream ss;
				string s;
				
				if (out == 194)
				{
					j++;
					out = *j;
				}
				if (out == 195)
				{
					j++;
					out = *j +64;
				}

				waddch( gMainWindow, /**j*/out );
				if 	(out>127&&false)
				{
					ss<<int(out);
					ss>>s;

					for( int i = 0; i < s.size(); i++)
						waddch( gMainWindow, s[i] );
				}
			}

			if( i != gMainBuffer.end( ) - 1 )
				waddch( gMainWindow, '\n' );
		}
		wattr_on( gMainWindow, COLOR_PAIR(7), 0 );

		wrefresh( gMainWindow );
		gMainWindowChanged = false;
	}
	//wattroff(gMainWindow,COLOR_PAIR(vcolor));
	// draw input window

	if( gInputWindowChanged )
	{
		wclear( gInputWindow );
		wmove( gInputWindow, 0, 0 );
		//phy r-stuff
		if ( (gInputBuffer == "/r " || gInputBuffer == "/R ") && !gGProxy->m_BNET->GetReplyTarget( ).empty() )
			gInputBuffer = "/w " + gGProxy->m_BNET->GetReplyTarget( ) + " ";
	
		//phy german_languagesupport
		if (!gInputBuffer.empty()&&int(gInputBuffer[gInputBuffer.size()-1])<0)
		{
			int last=gInputBuffer[gInputBuffer.size()-1];
			if(false&&(last==-28 /*|| last ==-33*/ || last == -10 ||last == -4 || last == -60 ||last == -42 ||last ==-36))
			{
				string s;
				stringstream ss;
				gInputBuffer[gInputBuffer.size()-1]=195;
				ss << char(192+last);
				ss >> s;
				//s.assign(char(192+last));
				gInputBuffer.append(s);
			};
		}
			
		for( string :: iterator i = gInputBuffer.begin( ); i != gInputBuffer.end( ); i++ )
		{
			signed char have = *i;
			unsigned char out;
			out = have;
			if (out == 194)
			{
				i++;
				out = *i;
			}
			if (out == 195)
			{
				i++;
				out = *i +64;
			}

			out = *i;
			
			waddch( gInputWindow, out );
		}

		wattr_on( gMainWindow, COLOR_PAIR(7), 0 );
		wrefresh( gInputWindow );
		gInputWindowChanged = false;
	}

	if (gGamesWindowChanged)
	{
		wclear( gGameWindow );
		//wmove( gGameWindow, 0, 100 );
		int inti=0;

		for( vector<string> :: iterator i = gGamesBuffer.begin( ); i != gGamesBuffer.end( ); i++ )
		{
		
			if((*i).substr(0,9)=="slots: +0"||((*i).substr(0,9)=="slots: +1"&&(*i).substr(0,10)!="slots: +10"&&(*i).substr(0,10)!="slots: +11"&&(*i).substr(0,10)!="slots: +12")||(*i).substr(0,9)=="slots: +2")
				wattr_on( gGameWindow, COLOR_PAIR(12), 0 );
			else if((*i).substr(0,9)=="slots: +3"||(*i).substr(0,9)=="slots: +4")

				wattr_on( gGameWindow, COLOR_PAIR(4), 0 );
			else if((*i)=="-------------------------------"||(*i)=="| -->  Refresh Gameslist  <-- |"||(*i)=="-----------GhostGraz-----------")
				wattr_on( gGameWindow, COLOR_PAIR(2), 0 );
			else
				wattr_on( gGameWindow, COLOR_PAIR(6), 0 );
		
			mvwaddnstr( gGameWindow, inti, 0, (*i).c_str( ), 31+1 );
			inti++;
		}
		//wattr_on( gGameWindow, COLOR_PAIR(7), 0 );

		wrefresh( gGameWindow );
		gGamesWindowChanged = false;
	}

	// draw channel window
	if( gChannelWindowChanged )
	{
		int cw = gGProxy->m_ChannelWidth; // manu
		int friends = friendList.size(); // manu
		int y = 2;
		
		memset(channelList, 0, channelList->size());

		if( !gGProxy->m_BNET->GetInGame() )
		{
			wclear( gChannelWindow );
			wattr_on( gChannelWindow, COLOR_PAIR(14), 0 );
			mvwaddnstr( gChannelWindow, 0, gChannelName.size( ) < cw ? ( cw - gChannelName.size( ) ) / 2 : 0, gChannelName.c_str( ), cw );
			mvwhline( gChannelWindow, 1, 0, 0, cw );

			for( vector<string> :: iterator i = gChannelUsers.begin( ); i != gChannelUsers.end( ); i++ )
			{
				if( y >= LINES - 7 - friends)
				{
					wattr_on( gChannelWindow, COLOR_PAIR(12), 0 );
					mvwaddnstr( gChannelWindow, y, 0, "Can't show more...", cw );
					break;
				}

				/*if (string(*i)=="GhostGraz1"||string(*i)=="GhostGraz7"||string(*i)=="GhostGraz13"||
					string(*i)=="GhostGraz2"||string(*i)=="GhostGraz8"||string(*i)=="GhostGraz14"||
					string(*i)=="GhostGraz3"||string(*i)=="GhostGraz9"||string(*i)=="GhostGraz15"||
					string(*i)=="GhostGraz4"||string(*i)=="GhostGraz10"||string(*i)=="GhostGraz16"||
					string(*i)=="GhostGraz5"||string(*i)=="GhostGraz11"||string(*i)=="GhostGraz17"||
					string(*i)=="GhostGraz6"||string(*i)=="GhostGraz12"||string(*i)=="GhostGraz18"||
					string(*i)=="GhostGraz19"||string(*i)=="GhostGraz20"||string(*i)=="eulenbaum"||string(*i)=="baerental")*/
				if (!string(*i).compare(0, string("GhostGraz").size(), "GhostGraz")||string(*i)=="eulenbaum"||string(*i)=="baerental")
					wattr_on( gChannelWindow, COLOR_PAIR(6), 0 );// GG-Bots: yellow
				else if (string(*i)=="Phyton" || string(*i)=="Manufactoring")//dgreen
					wattr_on( gChannelWindow, COLOR_PAIR(2), 0 );
				else if(string(*i)=="Pr0gm4n")
					wattr_on( gChannelWindow, COLOR_PAIR(13), 0 );
				else if(string(*i)=="Xense" || string(*i)=="klingone_graz" || string(*i)=="baerli_graz" || string(*i)=="ghostgraz_head")
					wattr_on( gChannelWindow, COLOR_PAIR(12), 0 );
				else
					wattr_on( gChannelWindow, COLOR_PAIR(7), 0 );

				channelList[y] = (*i).c_str(); // manu
				mvwaddnstr( gChannelWindow, y, 0, (*i).c_str( ), cw );
				y++;
			}
		}
		else if( gGProxy->m_BNET->GetInGame() )
		{
			wclear( gChannelWindow );
			wattr_on( gChannelWindow, COLOR_PAIR(14), 0 );
			mvwaddnstr( gChannelWindow, 0, gGProxy->m_GameName.size( ) < cw ? ( cw - gGProxy->m_GameName.size( ) ) / 2 : 0, gGProxy->m_GameName.c_str( ), cw );
			mvwhline( gChannelWindow, 1, 0, 0, cw );
			wattr_on( gChannelWindow, COLOR_PAIR(7), 0 );

			for( int i = 0; i < teams; i++ )
			{
				bool drawTeam = true;

				for( vector<CIncomingSlots *> :: iterator it = slots.begin( ); it != slots.end( ); it++ )
				{
					if( (*it)->GetTeam() == i )
					{
						if( y >= LINES - 7 - friends)
						{
							wattr_on( gChannelWindow, COLOR_PAIR(12), 0 );
							mvwaddnstr( gChannelWindow, y, 0, "Can't show more...", cw );
							break;
						}

						if ( drawTeam && DotAmap )
						{
							if( i == 0 )
							{
								channelList[y] = "The Sentinel";
								wattr_on( gChannelWindow, COLOR_PAIR(12), 0 );
								mvwaddnstr( gChannelWindow, y, 0, "The Sentinel", cw );
								wattr_on( gChannelWindow, COLOR_PAIR(7), 0 );
								y++;
							}
							else
							{
								y++;
								channelList[y] = "The Scourge";
								wattr_on( gChannelWindow, COLOR_PAIR(10), 0 );
								mvwaddnstr( gChannelWindow, y, 0, "The Scourge", cw );
								wattr_on( gChannelWindow, COLOR_PAIR(7), 0 );
								y++;
							}
							
							drawTeam = false;
						}
						else if ( drawTeam )
						{
							if( teamNumber != 0 )
								y++;

							ostringstream team;
							team << "Team ";
							team << (i+1);

							channelList[y] = team.str();
							wattr_on( gChannelWindow, COLOR_PAIR(14), 0 );
							mvwaddnstr( gChannelWindow, y, 0, team.str().c_str(), cw );
							wattr_on( gChannelWindow, COLOR_PAIR(7), 0 );
							y++;
							drawTeam = false;
						}

						if( (*it)->GetName().empty() )
						{
							string slotStatus;
							if( (*it)->GetSlotStatus() == 0)
							{
								slotStatus = "Open";
							}
							if( (*it)->GetSlotStatus() == 1)
							{
								slotStatus = "Closed";
							}
							if( (*it)->GetSlotStatus() == 2)
							{
								if( (*it)->GetComputerStatus() == 1 )
								{
									if( (*it)->GetComputerType() == 0 )
									{
										slotStatus = "Computer(Easy)";
									}
									else if( (*it)->GetComputerType() == 1 )
									{
										slotStatus = "Computer(Normal)";
									}
									else if( (*it)->GetComputerType() == 2 )
									{
										slotStatus = "Computer(Hard)";
									}
								}
								else
								{
									slotStatus = "Occupied";
								}
							}

							if( !(gGProxy->m_GameStarted) )
							{
								mvwaddnstr( gChannelWindow, y, 0, slotStatus.c_str( ), cw );
							}
							else
								y--;

						}
						else
						{
							if( gGProxy->m_GameStarted )
								if( playerLoadingComplete[ (*it)->GetPID() ] && !gameLoaded )
									wattr_on( gChannelWindow, COLOR_PAIR(14), 0 );
								else
									wattr_on( gChannelWindow, COLOR_PAIR(7), 0 );

							channelList[y] = (*it)->GetName().c_str();
							mvwaddnstr( gChannelWindow, y, 0, (*it)->GetName().c_str( ), cw );
						}
						
						y++;
					}
				}
			}
		}		

		if(friendList.size() != 0) //Friendlist [Manu]
		{
			y = LINES-5-friends; //Display friends at the bottom.
			wattr_on( gChannelWindow, COLOR_PAIR(14), 0 );
			mvwaddnstr( gChannelWindow, y, ((cw - 7) / 2), "Friends", cw );
			y++;
			mvwhline( gChannelWindow, y, 0, 0, cw );
			y++;
			
			wattr_on( gChannelWindow, COLOR_PAIR(2), 0 ); //Display online friends first and in green
			for( vector<CIncomingFriendList *> :: iterator i = friendList.begin( ); i != friendList.end( ); i++ )
			{
				if((*i)->GetArea() != 0)
				{
					channelList[y] = (*i)->GetAccount().c_str();
					mvwaddnstr( gChannelWindow, y, 0, (*i)->GetAccount().c_str(), cw );
					y++;
				}
			}

			wattr_on( gChannelWindow, COLOR_PAIR(4), 0 );//Offline friends in red
			for( vector<CIncomingFriendList *> :: iterator i = friendList.begin( ); i != friendList.end( ); i++)
			{
				if((*i)->GetArea() == 0)
				{
					channelList[y] = (*i)->GetAccount().c_str();
					mvwaddnstr( gChannelWindow, y, 0, (*i)->GetAccount().c_str(), cw );
					y++;
				}
			}
		}

		wrefresh( gChannelWindow );
		gChannelWindowChanged = false;
	}
}

void CONSOLE_Resize( )
{
	if( !gCurses )
		return;

	wresize( gMainWindow, LINES - 3, COLS - 17 );
	wresize( gBottomBorder, 1, COLS );
	wresize( gRightBorder, LINES - 3, 1 );
	wresize( gInputWindow, 2, COLS );
	wresize( gChannelWindow, LINES - 3, 16 );
	// mvwin( gMainWindow, 0, 0 );
	mvwin( gBottomBorder, LINES - 3, 0 );
	mvwin( gRightBorder, 0, COLS - 17 );
	mvwin( gInputWindow, LINES - 2, 0 );
	mvwin( gChannelWindow, 0, COLS - 16 );
	mvwhline( gBottomBorder, 0, 0, 0, COLS );
	mvwvline( gRightBorder, 0, 0, 0, LINES );
	wrefresh( gBottomBorder );
	wrefresh( gRightBorder );
	gMainWindowChanged = true;
	gInputWindowChanged = true;
	gChannelWindowChanged = true;
	CONSOLE_Draw();
}

std::wstring s2ws(const std::string& s)
{
 int len;
 int slength = (int)s.length() + 1;
 len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
 wchar_t* buf = new wchar_t[len];
 MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
 std::wstring r(buf);
 delete[] buf;
 return r;
}

void downloadfile(string url,string filename) //downloadfile("http://assets.throneit.com/games/games.txt","thr.html");
/* you need follwing libaries
#include <Wininet.h>//phy !games
#pragma comment(lib,"wininet")//phy !games
#include <direct.h>//same
#pragma comment(lib,"winmm.lib")
*/
{
	_mkdir("FGames");				//create foldere FGames
	// Was throwing errors in DEBUG mode
	/*unsigned int i;
	wchar_t wurl[255];
	
	for (i=0;i<=url.size();i++)		//
		wurl[i]=url[i];				//wurl=url
	wchar_t wfilepath[30];
	for (i = 0; i<=6;i++)			//wfilepath = FGames
		wfilepath[i]="FGames/"[i];
	char filepath[30]="FGames/";
	
	for ( i= 7;i<=filename.size()+7&&i<30;i++) //create the filepath
	{
		filepath[i]=filename[i-7];
		wfilepath[i]=filename[i-7];
	}

	remove(filepath);				//delete the old file
	if ( FAILED( CoInitialize(NULL) ) ) 
		CONSOLE_Print("[Phyton]error with dl"); 
	DeleteUrlCacheEntry(  wurl);	//deletce chache
	HRESULT hResult = URLDownloadToFile(NULL,wurl,wfilepath,0,NULL);*/		//download	
	// Changed to:
	LPCWSTR wurl;
#ifdef UNICODE
	std::wstring wurl_temp = s2ws(url);
	wurl = wurl_temp.c_str();
#else
	wurl = url.c_str();
#endif

	LPCWSTR wfilepath;
#ifdef UNICODE
		std::wstring wfilepath_temp = s2ws(string("FGames/").append(filename));
	wfilepath = wfilepath_temp.c_str();
#else
	wfilepath = string("FGames/").append(filename).c_str();
#endif

	remove(string(string("FGames/").append(filename)).c_str());		//delete the old file
	DeleteUrlCacheEntry(wurl);
	HRESULT hResult = URLDownloadToFile(NULL, wurl, wfilepath, 0, NULL);
	CoUninitialize();
}
void ConvertHTMLtoTXT(char chtml[31], char ctxt[30], string flag)
{
	char helpstrhtml[10000];
	string helphtml;
	stringstream ss;
	string s;
	//CONSOLE_Print("CONVERT");

	if (flag == "throneit.com") //aSDFJASFLÖA do not know why infile.getline is not working -.-
	{
		ifstream infile(chtml);
		ofstream outtxt;
		outtxt.open(ctxt);
		outtxt.clear();
		int i=1;
		stringstream ss2;
		if(!infile==false)
			while( !infile.eof())
			{
				//infile.getline(helpstrhtml,1000);
				infile.read(helpstrhtml,10000);
				helphtml.assign(helpstrhtml);
				s.clear();
				ss << helphtml;
				while(!ss.eof())
				{
					ss >> s;
					if (s.substr(0,10) == "FlashVars=")
					{
						
						string inti;
						ss2.clear();
						ss2 << i;
						ss2 >> inti;
						//CONSOLE_Print(inti);
						
						string gamename;
						string slots;
						int intslots;
						string output;
						string input=s.substr(11,s.size()-12);//input looks like: gn=thr3-cm2312&pc=(0/10)
						gamename = input.substr(3,input.size()-3);//thr3-cm2312&pc=(0/10)
						slots = gamename.substr(gamename.find_first_of("&")+5,gamename.size());
						slots = slots.substr(0,slots.find_first_of("/"));
						intslots = StrToInt(slots);
						intslots = 10-intslots;
						ss2.clear();
						ss2 << intslots;
						ss2 >> slots;
						slots = "+"+slots;
						
						gamename = gamename.substr(0,gamename.find_first_of("&"));//works
						
						output = "bot"+inti+","+gamename+","+slots;
						outtxt << output+"\n";
					
						//CONSOLE_Print(output);
						i++;
					}
				}
			}
			infile.close();
			outtxt.close();
	}
}


void AddGamesFromTextFile(string filename)
{
	AddGamesFromTextFile(filename,"no gamemode");
}
void AddGamesFromTextFile(string filename, string gamemode)
{
	// Is throwing errors in DEBUG mode
	/*char tempstr[30] = "FGames/";  //create the path of the textfile | works
	unsigned int i;
	for (i=7; i<filename.size()+7&&i<26;i++)
		tempstr[i]=filename[i-7];
	tempstr[filename.size()+7]="."[0];//add teh .txt
	tempstr[filename.size()+8]="t"[0];
	tempstr[filename.size()+9]="x"[0];
	tempstr[filename.size()+10]="t"[0];*/
	// Shorter and without errors
	string filepath = string("FGames/").append(filename).append(".txt");
	char *tempstr = new char[filepath.size()+1];
	tempstr[filepath.size()]=0;
	memcpy(tempstr,filepath.c_str(),filepath.size());

	//CONSOLE_Print(gamemode);
	if (gamemode=="no gamemode")
	{
		gGamesBuffer.clear();
		if(gGProxy->m_BNET->GetServer()=="europe.battle.net")
			gGamesBuffer.push_back("-----------GhostGraz-----------"); 
		else if(gGProxy->m_BNET->GetServer()=="uswest.battle.net"||gGProxy->m_BNET->GetServer()=="useast.battle.net")
			gGamesBuffer.push_back("------------ThroneIt------------"); 
		gGamesBuffer.push_back("| -->  Refresh Gameslist  <-- |");
		gGamesBuffer.push_back("-------------------------------");
	}

	ifstream infile(tempstr);	//opening the file
	char helpstr[100];			//here is the current line going to be saved
	string s;					//a string is easier to handle
	string games="[GProxy++] [lordrt + Phyton] Current Games:   ";//if you want to queue all current games...
	if(!infile == false)		//if the file has been opened
		while( !infile.eof())	//for every line of the file
		{
			infile.getline(helpstr,100);	//load the first 100 chars into helpstr
			s.assign(helpstr);				//convert helpstr into a string (s)
			if (s.size()>15)
			{
				if (gamemode == "print all" && s[0] != 35 && !getfromtextfile(s,"gamename").empty() 
					&& getfromtextfile(s,"slots") != "?") //if you want to queue all and the first char is not #
				{
					games = games + getfromtextfile(s,"gamename") + " -> "+getfromtextfile(s,"slots")+" slot(s) | ";
					if (games.size() > 200)
					{
						gGProxy->m_BNET->QueueChatCommand( games.substr(0, games.size()-2) );
						games.clear();
					}
				}
				else if (gamemode == "no gamemode" && getfromtextfile(s,"slots") != "?")	//if you want to list all current games...
					if (s[0] != 35 && !getfromtextfile(s,"gamename").empty()) //if the first char in not #
					{
						gGamesBuffer.push_back(getfromtextfile(s,"gamename"));
						gGamesBuffer.push_back(string("slots: ")+getfromtextfile(s,"slots")+" | bot: "+getfromtextfile(s,"botname"));
						gGamesBuffer.push_back("_______________________________");
						
						if( gGamesBuffer.size( ) > 100 )
							gGamesBuffer.erase( gGamesBuffer.begin( ) );
						//CONSOLE_Print(getfromtextfile(s,"gamename"));
						//s="Bot ["+getfromtextfile(s,"botname")+"] is hosting the game ["+getfromtextfile(s,"gamename")+"]. (Gamemode ["+getfromtextfile(s,"gamemode")+"] and ["+getfromtextfile(s,"slots")+"] free slots)";//print em^^
						//CONSOLE_Print(s);
						
					}
					else; //if break
				else if (getfromtextfile(s,"botname") == gamemode && getfromtextfile(s,"slots") != "?")
				{
					autosearch(true);					
					CheckForGame(getfromtextfile(s,"gamename"));
				}
				else if (gamemode==getfromtextfile(s,"gamemode"))
				{
					autosearch(true);
					CheckForGame(getfromtextfile(s,"gamename"));
				}
			}
			s.clear();
		}
		gGamesWindowChanged = true;
		CONSOLE_Draw();

		if(gamemode == "print all")	//if you want to queue all...
				gGProxy->m_BNET->QueueChatCommand(games.substr(0,games.size()-2));

		infile.close();
}

/*
void AddGamesFromTextFile (string Command,string gamemode) // dl2
{
	char tempstr[30] = "FGames/1";
	//tempstr[8]= "e"[0];
	string filename=Command.substr(3,Command.size());
	//CONSOLE_Print(filename);
	
	/*int i;
	for (i=7; i<=Command.size()-3+11;i++)
		tempstr[i]=Command[i-4];*//*
	
					int xyz=Command.size()-3+11;
					char tempstr13[13] = "FGames/1.txt";
					char tempstr14[14] = "FGames/12.txt";
					char tempstr15[15] = "FGames/123.txt";
					char tempstr16[16] = "FGames/1234.txt";
					char tempstr17[17] = "FGames/12345.txt";

					if (Command.size()==4)
					{
						tempstr13[7] = Command[3];
						ifstream infile(tempstr13);
						
						char helpstr[100];
						if(!infile==false)
						while( !infile.eof())
						{
							infile.getline(helpstr,100);
							//CheckForGame(helpstr);
							
						}
					infile.close();
					}
					else if(Command.size()==5)
					{
						tempstr14[7] = Command[3];
						tempstr14[8] = Command[4];
						ifstream infile(tempstr14);
						char helpstr[100];
						if(!infile==false)
						while( !infile.eof())
						{
							infile.getline(helpstr,100);
							//CheckForGame(helpstr);
							
						}
					infile.close();
					}
					else if(Command.size()==6)
					{
						tempstr15[7] = Command[3];
						tempstr15[8] = Command[4];
						tempstr15[9] = Command[5];
						ifstream infile(tempstr15);
						char helpstr[100];
						if(!infile==false)
						while( !infile.eof())
						{
							infile.getline(helpstr,100);
							//CheckForGame(helpstr);
							
						}
					infile.close();
					}
					else if(Command.size()==7)
					{
						tempstr16[7] = Command[3];
						tempstr16[8] = Command[4];
						tempstr16[9] = Command[5];
						tempstr16[10] = Command[6];
						ifstream infile(tempstr16);
						char helpstr[100];
						stringstream ss;
						string s;
						string games="[GProxy++] [lordrt + Phyton] Current Games:   ";
						/*if (gamemode == "print all")
						gGProxy->m_BNET->QueueChatCommand("[GProxy++] [lordrt + Phyton] Current Games: ");
						*//*
						if(!infile==false)
						while( !infile.eof())
						{
							infile.getline(helpstr,100);
							//ss << helpstr;
							//ss >> s;
							s.assign(helpstr);

							if (gamemode == "print all"&&s[0]!=35 &&!getfromtextfile(s,"gamename").empty())
							{
								games = games + getfromtextfile(s,"gamename") + " -> "+getfromtextfile(s,"slots")+" slot(s) | ";
								if (games.size()>200)
								{
									gGProxy->m_BNET->QueueChatCommand(games.substr(0,games.size()-2));
									games.clear();
								}
							}

							//CheckForGame(helpstr);
							//if (helpstr[0]!=char("#"))
							if (gamemode == "no gamemode")
							if (s[0]!=35 &&!getfromtextfile(s,"gamename").empty()) //35=#
							{
							//CONSOLE_Print(s);
								s="Bot ["+getfromtextfile(s,"botname")+"] is hosting the game ["+getfromtextfile(s,"gamename")+"]. (Gamemode ["+getfromtextfile(s,"gamemode")+"] and ["+getfromtextfile(s,"slots")+"] free slots)";
								CONSOLE_Print(s);
								
							}else;
							else
							{
								
								if (gamemode==getfromtextfile(s,"gamemode"))
									CheckForGame(getfromtextfile(s,"gamename"));
							}
							
						}
						if (gamemode == "print all")//phy1233
						{
							gGProxy->m_BNET->QueueChatCommand(games.substr(0,games.size()-2));
							//CONSOLE_Print(games.substr(0,games.size()-2));
						}
					infile.close();
					}
					else if(Command.size()==8)
					{
						tempstr17[7] = Command[3];
						tempstr17[8] = Command[4];
						tempstr17[9] = Command[5];
						tempstr17[10] = Command[6];
						tempstr17[11] = Command[7];
						ifstream infile(tempstr17);
						char helpstr[100];
						if(!infile==false)
						while( !infile.eof())
						{
							infile.getline(helpstr,100);
							//CheckForGame(helpstr);
						}
					infile.close();
					}
				}*/




//
// main
//

int main( int argc, char **argv )
{
	string CFGFile = "gproxy.cfg";

	if( argc > 1 && argv[1] )
		CFGFile = argv[1];

	// read config file
	
	CConfig CFG;
	CFG.Read( CFGFile );
	gLogFile = CFG.GetString( "log", string( ) );

	wchar_t wc[40];
	for (int i = 0; i < 40; i++)
		wc[i]="Customized GProxy++ - GhostGraz v0.5+"[i];

	SetConsoleTitle(wc); //wcahr---
/*
	//phy tray
	NOTIFYICONDATA tsym;
	tsym.cbSize = sizeof (NOTIFYICONDATA);
		tsym.hWnd   = (GetStdHandle(
  STD_OUTPUT_HANDLE
));

		tsym.uID    = 1;
		tsym.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
		tsym.uCallbackMessage = WM_APP;
		//tsym.hIcon  = hIcon;
//		strcpy (tsym.szTip, "Mein Programm...");
		Shell_NotifyIcon (NIM_ADD, &tsym);
*/
	//off

	CONSOLE_Print( "[GPROXY] starting up" );

#ifndef WIN32
	// disable SIGPIPE since some systems like OS X don't define MSG_NOSIGNAL
	signal( SIGPIPE, SIG_IGN );
#endif

#ifdef WIN32
	// initialize winsock

	CONSOLE_Print( "[GPROXY] starting winsock" );
	WSADATA wsadata;

	if( WSAStartup( MAKEWORD( 2, 2 ), &wsadata ) != 0 )
	{
		CONSOLE_Print( "[GPROXY] error starting winsock" );
		return 1;
	}

	// increase process priority

	CONSOLE_Print( "[GPROXY] setting process priority to \"above normal\"" );
	SetPriorityClass( GetCurrentProcess( ), ABOVE_NORMAL_PRIORITY_CLASS );
#endif

	string War3Path;
	string CDKeyROC;
	string CDKeyTFT;
	string Server;
	string Username;
	string cfilter = "";//phy public
	string cpublic = "";//phy filter
	bool autodetecttemp = false;//autodetect phy
	bool german_languagesupport = false; //phy language
	bool casearch = false; //pr0 cautosearch
	bool temp_displayautocreated = false;
	bool listing_current_games = false;				
	
	string Password;
	string Channel;
	uint32_t War3Version = 24;
	uint16_t Port = 6125;
	BYTEARRAY EXEVersion;
	BYTEARRAY EXEVersionHash;
	string PasswordHashType;
	//PingPhy = 0; //phy

	if( !CFG.Exists( "war3path" )  || !CFG.Exists( "cdkeyroc" ) || !CFG.Exists( "server" ) || !CFG.Exists( "username" ) || !CFG.Exists( "password" ) || !CFG.Exists( "channel" ) )
	{
		CONSOLE_Print( "", false );
		CONSOLE_Print( "  It looks like this is your first time running GProxy++.", false );
		CONSOLE_Print( "  GProxy++ needs some information about your computer and about yourself.", false );
		CONSOLE_Print( "", false );
		CONSOLE_Print( "  This information will be saved to the file \"" + CFGFile + "\" in plain text.", false );
		CONSOLE_Print( "  You can update this information at any time by editing the above file.", false );
		CONSOLE_Print( "", false );

#ifdef WIN32
		string RegistryWar3Path;
		HKEY hkey;
		long s = RegOpenKeyExA( HKEY_CURRENT_USER, "Software\\Blizzard Entertainment\\Warcraft III", 0, KEY_QUERY_VALUE, &hkey );

		if( s == ERROR_SUCCESS )
		{
			char InstallPath[256];
			DWORD InstallPathSize = 256;
			RegQueryValueExA( hkey, "InstallPath", NULL, NULL, (LPBYTE)InstallPath, &InstallPathSize );
			RegistryWar3Path = InstallPath;
			RegCloseKey( hkey );
		}

		CONSOLE_Print( "  Enter the path to your Warcraft III install folder.", false );
		CONSOLE_Print( "  If you want to use the detected path just leave it blank (press enter).", false );
		CONSOLE_Print( "", false );
		CONSOLE_PrintNoCRLF( "  Install Path [" + RegistryWar3Path + "]: ", false );
		getline( cin, War3Path );

		if( War3Path.empty( ) )
			War3Path = RegistryWar3Path;
#else
		CONSOLE_Print( "  GProxy++ needs some files from a Windows installation of Warcraft III.", false );
		CONSOLE_Print( "  These files are:", false );
		CONSOLE_Print( "    game.dll", false );
		CONSOLE_Print( "    storm.dll", false );
		CONSOLE_Print( "    war3.exe", false );
		CONSOLE_Print( "  Enter the path to the directory containing these files.", false );
		CONSOLE_Print( "", false );
		CONSOLE_PrintNoCRLF( "  Install Path: ", false );
		getline( cin, War3Path );
#endif
		War3Path = UTIL_AddPathSeperator( War3Path );

		CONSOLE_Print( "", false );
		CONSOLE_Print( "  Enter your CD key(s) with or without dashes, capital letters or lowercase.", false );
		CONSOLE_Print( "", false );

		do
		{
			CONSOLE_PrintNoCRLF( "  Reign of Chaos CD key: ", false );
			getline( cin, CDKeyROC );
			CDKeyROC.erase( remove( CDKeyROC.begin( ), CDKeyROC.end( ), '-' ), CDKeyROC.end( ) );
			transform( CDKeyROC.begin( ), CDKeyROC.end( ), CDKeyROC.begin( ), (int(*)(int))toupper );
		} while( CDKeyROC.size( ) != 26 );

		CONSOLE_Print( "", false );

		do
		{
			CONSOLE_PrintNoCRLF( "  Frozen Throne CD key: ", false );
			getline( cin, CDKeyTFT );
			CDKeyTFT.erase( remove( CDKeyTFT.begin( ), CDKeyTFT.end( ), '-' ), CDKeyTFT.end( ) );
			transform( CDKeyTFT.begin( ), CDKeyTFT.end( ), CDKeyTFT.begin( ), (int(*)(int))toupper );
		} while( !CDKeyTFT.empty( ) && CDKeyTFT.size( ) != 26 );

		CONSOLE_Print( "", false );
		CONSOLE_Print( "  Select a battle.net server to connect to.", false );
		CONSOLE_Print( "  Enter one of the following numbers (1-4) or enter a custom address.", false );
		CONSOLE_Print( "  1. US West (Lordaeron)", false );
		CONSOLE_Print( "  2. US East (Azeroth)", false );
		CONSOLE_Print( "  3. Asia (Kalimdor)", false );
		CONSOLE_Print( "  4. Europe (Northrend)", false );
		CONSOLE_Print( "", false );

		do
		{
			CONSOLE_PrintNoCRLF( "  Server: ", false );
			getline( cin, Server );
			transform( Server.begin( ), Server.end( ), Server.begin( ), (int(*)(int))tolower );

			if( Server == "1" || Server == "1." || Server == "us west" || Server == "uswest" || Server == "west" || Server == "lordaeron" )
				Server = "uswest.battle.net";
			else if( Server == "2" || Server == "2." || Server == "us east" || Server == "useast" || Server == "east" || Server == "azeroth" )
				Server = "useast.battle.net";
			else if( Server == "3" || Server == "3." || Server == "asia" || Server == "kalimdor" )
				Server = "asia.battle.net";
			else if( Server == "4" || Server == "4." || Server == "europe" || Server == "northrend" )
				Server = "europe.battle.net";
		} while( Server.empty( ) );

		CONSOLE_Print( "", false );
		CONSOLE_Print( "  What is your battle.net username?", false );
		CONSOLE_Print( "", false );

		do
		{
			CONSOLE_PrintNoCRLF( "  Username: ", false );
			getline( cin, Username );
		} while( Username.empty( ) );

		CONSOLE_Print( "", false );
		CONSOLE_Print( "  What is your battle.net password?", false );
		CONSOLE_Print( "  Note that your password will be visible as you type it.", false );
		CONSOLE_Print( "", false );

		do
		{
			CONSOLE_PrintNoCRLF( "  Password: ", false );
			getline( cin, Password );
		} while( Password.empty( ) );

		CONSOLE_Print( "", false );
		CONSOLE_Print( "  What battle.net channel do you want to start in?", false );
		CONSOLE_Print( "", false );

		do
		{
			CONSOLE_PrintNoCRLF( "  Channel: ", false );
			getline( cin, Channel );
		} while( Channel.empty( ) );

		CONSOLE_Print( "", false );
		CONSOLE_Print( "  Done. Saving configuration to \"" + CFGFile + "\".", false );

		ofstream out;
		out.open( CFGFile.c_str( ) );

		if( out.fail( ) )
			CONSOLE_Print( "  Error saving configuration file.", false );
		else
		{
			out << "### required config values" << endl;
			out << endl;
			out << "war3path = " << War3Path << endl;
			out << "cdkeyroc = " << CDKeyROC << endl;
			out << "cdkeytft = " << CDKeyTFT << endl;
			out << "server = " << Server << endl;
			out << "username = " << Username << endl;
			out << "password = " << Password << endl;
			out << "channel = " << Channel << endl;
			out << endl;
			out << "### optional config values" << endl;
			out << endl;
			out << "war3version = " << War3Version << endl;
			out << "port = " << Port << endl;
			out << "exeversion =" << endl;
			out << "exeversionhash =" << endl;
			out << "passwordhashtype =" << endl;
			out.close( );
		}
	}
	else
	{
		War3Path = CFG.GetString( "war3path", string( ) );
		CDKeyROC = CFG.GetString( "cdkeyroc", string( ) );
		CDKeyTFT = CFG.GetString( "cdkeytft", string( ) );
		Server = CFG.GetString( "server", string( ) );
		if(CFG.Exists("privategamename")) //phy priv waitgame
			privategamename = CFG.GetString( "privategamename", string( ) );
		if(CFG.Exists("botprefix"))//phy waitgame
			botprefix = CFG.GetString( "botprefix", string( ) );
		
		if (CFG.Exists("autodetect"))//autodetect phy
			if (CFG.GetString("autodetect",string()) == "true")
				autodetecttemp = true;
		if (CFG.Exists("german_languagesupport"))
			if (CFG.GetString("german_languagesupport",string()) == "true")
				german_languagesupport=true;

		if (CFG.Exists("listing_current_games")&&CFG.GetString("listing_current_games",string())=="true")
			listing_current_games=true;
	
		if(CFG.Exists("public"))//phy public
			if(CFG.GetString("public",string())=="on")
			{
				cpublic="on";
				
				CONSOLE_Print("[Phyton] Automatically listing public games (activated though config)");
			}
			else;
		else;
		if(CFG.Exists("autosearch")) //pr0 autosearch
			if(CFG.GetString("autosearch",string()) == "on" || CFG.GetString("autosearch",string()) == "true")
			{
				casearch = true; //pr0 cautosearch standard is false so there is no need of else
				CONSOLE_Print("[Pr0gm4n] Automatically adding games by Autohost-Messages in Chat", false); 
			}else;
		else;
		if(CFG.Exists("autodetect")) //pr0 autosearch
			if(CFG.GetString("autodetect",string()) == "on" || CFG.GetString("autodetect",string()) == "true")
			{
				casearch = true; //pr0 cautosearch standard is false so there is no need of else
				CONSOLE_Print("[Pr0gm4n] Automatically adding games by Autohost-Messages in Chat", false); 
			}
			else;
		else;
		
		if ( CFG.Exists("display_autocreated_game") && CFG.GetString("display_autocreated_game",string()) == "true" )
			temp_displayautocreated=true;

		if(CFG.Exists("filter"))//phy filter
		{
			cfilter=CFG.GetString("filter", string());
		
			CONSOLE_Print("[Phyton] Filter ["+CFG.GetString("filter", string())+"] loaded from config.");
		}

		if( CFG.Exists("sound") )
		{
			if( CFG.GetString( "sound", string() ) == "off" || CFG.GetString( "sound", string() ) == "false")
			{
				gGProxy->m_PlaySound = false;
			}
		}

		Username = CFG.GetString( "username", string( ) );
		Password = CFG.GetString( "password", string( ) );
		Channel = CFG.GetString( "channel", string( ) );
		War3Version = CFG.GetInt( "war3version", War3Version );
		Port = CFG.GetInt( "port", Port );
		EXEVersion = UTIL_ExtractNumbers( CFG.GetString( "exeversion", string( ) ), 4 );
		EXEVersionHash = UTIL_ExtractNumbers( CFG.GetString( "exeversionhash", string( ) ), 4 );
		PasswordHashType = CFG.GetString( "passwordhashtype", string( ) );
	}

	CONSOLE_Print( "", false );
	CONSOLE_Print( "  Welcome to GProxy++.", false );
	CONSOLE_Print( "  Server: " + Server, false );
	CONSOLE_Print( "  Username: " + Username, false );
	CONSOLE_Print( "  Channel: " + Channel, false );
	CONSOLE_Print( "", false );

	// initialize curses

	gCurses = true;
	initscr( );
	
	start_color( );
	for(int i = 1; i <= 255; i++)
	{
		init_pair(i, i%16, (i-(i%16))/16);
	}

	int channelWidth = 16;
#ifdef WIN32
	if ( CFG.Exists("width") && CFG.Exists("height") )
		resize_term( CFG.GetInt("height", int()), CFG.GetInt("width", int()) );
	else 
		resize_term( 28, 97);

	if( CFG.Exists("channelwidth") )
		channelWidth = CFG.GetInt( "channelwidth", int() );
#endif

	clear( );
	noecho( );
	cbreak( );
	int gamex=31;
	int gamey=31;
	gMainWindow = newwin( LINES - 3, COLS - (channelWidth + 1)-gamex-1, 0, 0 );
	gBottomBorder = newwin( 1, COLS, LINES - 3, 0 );
	gRightBorder = newwin( LINES - 3, 1, 0, COLS - (channelWidth + 1) );
	gInputWindow = newwin( 2, COLS, LINES - 2, 0 );
	gChannelWindow = newwin( LINES - 3, channelWidth, 0, COLS - channelWidth );
	gGameWindow = newwin( LINES - 3, 31,0,COLS - (channelWidth + 1)-gamex);
	gMidBorder = newwin(LINES - 3,1,0,COLS - (channelWidth + 2)-gamex);
	mvwhline( gBottomBorder, 0, 0, 0, COLS );
	mvwvline( gRightBorder, 0, 0, 0, LINES );
	mvwvline( gMidBorder, 0, 0, 0, COLS);
	wrefresh( gMidBorder);
	wrefresh( gBottomBorder );
	wrefresh( gRightBorder );
	scrollok( gMainWindow, TRUE );
	keypad( gInputWindow, TRUE );
	scrollok( gInputWindow, TRUE );
	CONSOLE_Print( "  Type /help at any time for help.", false );
	CONSOLE_Print( "  Press any key to continue.", false );
	CONSOLE_Print( "", false );
	CONSOLE_Print((string)(" Welcome to ")+char(6)+char(12)+"GProxy++"+char(6)+char(7)+", this mod is by"+char(6)+char(2)+" Phyton"+char(6)+char(7)+","+char(6)+char(13)+" Pr0gm4n"+char(6)+char(7)+" and "+char(6)+char(14)+"Manufactoring"+char(6)+char(7)+".");
	CONSOLE_Draw();
	wgetch( gInputWindow );
	nodelay( gInputWindow, TRUE );

	// Manufactoring
	MEVENT pos;
    mousemask( BUTTON1_CLICKED | BUTTON1_RELEASED | BUTTON3_CLICKED | BUTTON3_RELEASED, NULL);

	// initialize gproxy

	gGProxy = new CGProxy( !CDKeyTFT.empty( ), War3Path, CDKeyROC, CDKeyTFT, Server, Username, Password, Channel, War3Version, Port, EXEVersion, EXEVersionHash, PasswordHashType, cpublic, cfilter, german_languagesupport,casearch, temp_displayautocreated, listing_current_games, channelWidth );//phy added cpublic and cfilter for public and filter and autodetecttemp for autodetect.

	while( 1 )
	{
		if( gGProxy->Update( 40000 ) )
			break;

		bool Quit = false;

		int c = wgetch( gInputWindow );
		while( c != ERR )
		{
			if( c == KEY_MOUSE ) // Manufactoring (mouse hook)
			{
				if( nc_getmouse(&pos) != ERR )
				{
					int x = pos.x;
					int y = pos.y;

					// Mousehook - ChannelWindow
					if( x > (COLS - (gGProxy->m_ChannelWidth +1) ) )
					{
						string user = channelList[y];
						if( user.size() != 0 )
						{
							if( pos.bstate == BUTTON1_CLICKED || pos.bstate == BUTTON1_RELEASED )
							{
								if( channelList[y].compare("The Sentinel") == 0 )
								{
									gGProxy->SendChangeTeam( 0 );
								}
								else if( channelList[y].compare("The Scourge") == 0 )
								{
									gGProxy->SendChangeTeam( 1 );
								}
								else if( channelList[y].find("Team ") != string::npos )
								{
									int team = atoi( channelList[y].substr(5).c_str() ) -1;
									gGProxy->SendChangeTeam( team );
								}
								else
								{
									gInputBuffer.clear( );
									gInputBuffer.append( user.insert(0,"/w ").append(" ") );
								}
							}
							else if( pos.bstate == BUTTON3_CLICKED || pos.bstate == BUTTON3_RELEASED )
							{
								gGProxy->m_BNET->QueueChatCommand( user.insert(0,"/whois ") );
							}
						}
					}
					// Mousehook - GameWindow
					else if( x >COLS - (channelWidth + 2)-31 && y <LINES -3 &&x < (COLS - (gGProxy->m_ChannelWidth +1) ))
					{
						if( pos.bstate == BUTTON1_CLICKED || pos.bstate == BUTTON1_RELEASED )
						{
							if(y % 3 != 2&&gGamesBuffer.size()> y)
							{
								if (gGamesBuffer[(y / 3)*3]=="-----------GhostGraz-----------")
								{
									downloadfile("http://forum.ghostgraz.com/feeds/currentgames.txt","graz.txt");
									AddGamesFromTextFile("graz");										
								}
								else if(gGamesBuffer[(y / 3)*3]=="------------ThroneIt------------")
								{
									downloadfile("http://assets.throneit.com/games/games.html","thr.html");
									ConvertHTMLtoTXT("FGames/thr.html","FGames/thr.txt","throneit.com");
									AddGamesFromTextFile("thr");	
								}
								else
								{
									autosearch(true);
									CheckForGame(gGamesBuffer[(y / 3)*3]);	
								}
								//CONSOLE_Print(gGamesBuffer[(y / 3)*3]);
							}
						}

						if( pos.bstate == BUTTON3_CLICKED || pos.bstate == BUTTON3_RELEASED )
						{
							if(y % 3 != 2&&gGamesBuffer.size()> y)
							{
								if (gGamesBuffer[(y / 3)*3] != "-----------GhostGraz-----------")
								{
									gInputBuffer += gGamesBuffer[(y / 3)*3];
								}
								else 
								{
									downloadfile("http://forum.ghostgraz.com/feeds/currentgames.txt","graz.txt");
									AddGamesFromTextFile("graz");
								}
							}
						}
					} // [END] Mousehook - Gamewindow
				}
			}

			if( c == 8 || c == 127 || c == KEY_BACKSPACE || c == KEY_DC )
			{
				// backspace, delete

				if( !gInputBuffer.empty( ) )
					gInputBuffer.erase( gInputBuffer.size( ) - 1, 1 );
			}
			else if( c == 9 )
			{
				// tab
			}
#ifdef WIN32
			else if( c == 10 || c == 13 || c == PADENTER )
#else
			else if( c == 10 || c == 13 )
#endif
			{
				// cr, lf
				// process input buffer now

				string Command_1 = gInputBuffer; //changing it
				
				transform( Command_1.begin( ), Command_1.end( ), Command_1.begin( ), (int(*)(int))tolower );
				string Command;
				string input;
				int i; //running i
				
				for( i = 0; i < string(Command_1).size(); i++ )
				{
					if(string(Command_1)[i] == "ö"[0] || string(Command_1)[i] == "Ö"[0]
					|| string(Command_1)[i] == "ä"[0] || string(Command_1)[i] == "Ä"[0]
					|| string(Command_1)[i] == "ü"[0] || string(Command_1)[i] == "Ü"[0])
					{
						unsigned char f = 195;
						signed char f2 = f;
						unsigned char us = char( int( string(Command_1)[i] ) - 64 );
						signed char s = us;
						
						Command = Command + char(f2) + char(s); //asdfe
						i++; //öäü
					}
					else
						Command = Command + Command_1[i];
				}
				for( i = 0; i < string(gInputBuffer).size(); i++ )
				{
					if(string(gInputBuffer)[i] == "ö"[0] || string(gInputBuffer)[i] == "Ö"[0]
					|| string(gInputBuffer)[i] == "ä"[0] || string(gInputBuffer)[i] == "Ä"[0]
					|| string(gInputBuffer)[i] == "ü"[0] || string(gInputBuffer)[i] == "Ü"[0])
					{
						unsigned char f = 195;
						signed char f2 = f;
						unsigned char us = char( int( string(gInputBuffer)[i]) - 64 );
						signed char s = us;
						
						input = input + char(f2) + char(s); //asdfe
						//i++; //öäü
					}
					else
						input=input+gInputBuffer[i];
				}

				if( Command == "/commands" )
				{
					CONSOLE_Print( ">>> /commands" );
					CONSOLE_Print( "", false );
					CONSOLE_Print( "  In the GProxy++ console:", false );
					CONSOLE_Print( "   /commands           : show command list", false );
					CONSOLE_Print( "   /exit or /quit      : close GProxy++", false );
					CONSOLE_Print( "   /filter <f>         : start filtering public game names for <f>", false );
					CONSOLE_Print( "   /filteroff          : stop filtering public game names", false );
					CONSOLE_Print( "   /game <gamename>    : look for a specific game named <gamename>", false );
					CONSOLE_Print( "   /help               : show help text", false );
					CONSOLE_Print( "   /public             : enable listing of public games", false );
					CONSOLE_Print( "   /publicoff          : disable listing of public games", false );
					CONSOLE_Print( "   /r <message>        : reply to the last received whisper", false );
					CONSOLE_Print( "   /pgn <privgamename> : sets privategamename temporary to <privgamename> (Alias: /privategamename)", false); //phy ofc i have added the new commands.
					CONSOLE_Print( "   /waitgame <option>  : waits for a bot joining channel with the name-prefix set in gproxy.cfg and hosts", false);
					CONSOLE_Print( "                         a private game named <value of privategamename> (Alias: /wg <option>", false);
					CONSOLE_Print( "   Optons:   quiet     : doesn't display the message to other users in the channel (Alias: q)", false);
					CONSOLE_Print( "             off       : stops waiting for a bot to host a game", false);
					CONSOLE_Print( "",false);
					CONSOLE_Print( "   /botprefix <prefix> : sets <value of botprefix> temporary to <prefix>", false);
					CONSOLE_Print( "   /parrot <plname>    : repeats anything that Player <plname> sais in Chat with [PARROT] <Player's message>", false); 
					CONSOLE_Print( "   /parrotall          : repeats anything that Players say in Chat with [PARROT] <Player's message>", false); 
					CONSOLE_Print( "   /parrotoff          : stops /parrot and /parrotall", false); //pr0 edited /commands overview
#ifdef WIN32
					CONSOLE_Print( "   /start              : start warcraft 3", false );
#endif
					CONSOLE_Print( "   /version            : show version text", false );
					CONSOLE_Print( "", false );
					CONSOLE_Print( "  In game:", false );
					CONSOLE_Print( "   /re <message>       : reply to the last received whisper", false );
					CONSOLE_Print( "   /sc                 : whispers \"spoofcheck\" to the game host", false );
					CONSOLE_Print( "   /status             : show status information", false );
					CONSOLE_Print( "   /w <user> <message> : whispers <message> to <user>", false );
					CONSOLE_Print( "", false );
				}
				else if( Command == "/exit" || Command == "/quit" )
				{
					Quit = true;
					break;
				}
				else if( Command.substr( 0, 9 ) == "/friends " || Command.substr( 0, 3 ) == "/f ")
				{
					gGProxy->m_BNET->QueueChatCommand( gInputBuffer );
					gGProxy->m_BNET->UpdateFriendList( );
				}
				else if( Command.size( ) >= 9 && Command.substr( 0, 8 ) == "/filter " )
				{
					string Filter = gInputBuffer.substr( 8 );

					if( !Filter.empty( ) && Filter.size( ) <= 31 )
					{
						gGProxy->m_BNET->SetPublicGameFilter( Filter );
						CONSOLE_Print( "[BNET] started filtering public game names for \"" + Filter + "\"" );
					}
				}
				else if( Command == "/filteroff" )
				{
					gGProxy->m_BNET->SetPublicGameFilter( string( ) );
					CONSOLE_Print( "[BNET] stopped filtering public game names" );
				}
				else if( privategamename!="" && Command.size( ) >= 7 && Command.substr( 0, 6 ) == "/priv " )
				{
					string BotName = gInputBuffer.substr( 6 );	
					if( !BotName.empty( ) && BotName.size( ) <= 15 )
					{
						string pgn = privategamename;
						if (BotName.size()>2)
							gGProxy->m_BNET->QueueChatCommand( "/w " + BotName + " !priv " +pgn );
						else
							gGProxy->m_BNET->QueueChatCommand( "/w " + botprefix +BotName + " !priv " +pgn );
						gGProxy->m_BNET->SetSearchGameName ( pgn);
						autosearch(false);
						CONSOLE_Print( "[BNET] try to create a private game named ["+pgn+"] at Bot [" + BotName + "]." );
					}
				}
				else if (Command.substr(0,11)=="/autosearch")
				{
					if (Command=="/autosearch")
						if (gGProxy->cautosearch)
							CONSOLE_Print("[Pr0gm4n] 'autosearch' is on.");
						else
							CONSOLE_Print("[Pr0gm4n] 'autosearch' is off.");
					else if(Command=="/autodetect on")
					{
						gGProxy->cautosearch=true;
						CONSOLE_Print("[Pr0gm4n] Set 'autosearch' to [on]");
					}
					else if (Command=="/autodetect off")
					{
						gGProxy->cautosearch=false;
						CONSOLE_Print("[Pr0gm4n] Set 'autosearch' to [off]");
					}
					else
						CONSOLE_Print("[Pr0gm4n] invalid autosearch command.");
				}
				else if (Command.substr(0,3)=="/as")
				{
					if (Command=="/as")
						if (gGProxy->cautosearch)
							CONSOLE_Print("[Pr0gm4n] 'autosearch' is on.");
						else
							CONSOLE_Print("[Pr0gm4n] 'autosearch' is off.");
					else if(Command=="/as on")
					{
						gGProxy->cautosearch=true;
						CONSOLE_Print("[Pr0gm4n] Set 'autosearch' to [on]");
					}
					else if (Command=="/as off")
					{
						gGProxy->cautosearch=false;
						CONSOLE_Print("[Pr0gm4n] Set 'autosearch' to [off]");
					}
					else
						CONSOLE_Print("[Pr0gm4n] invalid autosearch command.");
				}
				else if (Command == "/dac on"|| Command == "/display_autocreated on")
				{
					displayautocreated(true);
					CONSOLE_Print("[Phyton] you are not hiding autocreated game anymore");
				}
				else if (Command == "/dac off"|| Command == "/display_autocreated off")
				{

					displayautocreated(false);
					CONSOLE_Print("[Phyton] you are now hiding autocreated games");
				}
				else if (Command == "/listing_current_games on"||Command =="/lcg on")
				{
					CONSOLE_Print("[Phyton] !games on");
					flisting_current_games(true);
				}
				else if (Command == "/listing_current_games off"||Command =="/lcg off")
				{
					CONSOLE_Print("[Phyton] !games off");
					flisting_current_games(false);
				}
				//else if (Command == "/listing_current_games repeat"||Command =="/lcg repeat")
				//	flisting_current_games();
				/*else if ( Command.substr(0,7) == "/stats ") //phy using a dumpfile of the graz-database..
				{
					MYSQL *connection, mysql;
					MYSQL_RES *result;
					MYSQL_ROW row;
					int query_state;
					string Name=Command.substr(7,Command.size()-7);
					string res;
					stringstream ss;
													   
					char querry[224] = "select count(name) , sum(fcheckleave(gp.gameid,gp.`left`,gp.leftreason,g.duration)) from gameplayers as gp, games as g where name = '               ' and g.id=gp.gameid";
					int i;
					for (i=0;i<=15 && Name.size()!=i;i++)
					querry[133+i]=Name[i];
					mysql_init(&mysql);
					connection = mysql_real_connect(&mysql,"localhost","root","pass_ghost","ghost",0,0,0);
					if (connection == NULL) CONSOLE_Print("ERROR mysql phyton");
					CONSOLE_Print(querry);

					query_state = mysql_query(connection, querry);
			
					if (query_state !=0) CONSOLE_Print("ERROR mysql phyton");

					result = mysql_store_result(connection);
					//ss << row[0];
					//ss >> res;
					int j;
					int jj;
					
					char* str = new char[30];
					
					while ( ( row = mysql_fetch_row(result)) != NULL ) 
					{
						//CONSOLE_Print(res);
						if (string(row[0])!="0")
							if (string(row[1])!="0")
							{
								
								j= StrToInt(row[0]);
								jj= StrToInt(row[1]);
								float flt =(1.0f*j / jj);
								
								sprintf_s(str,5, "%.4g" , flt );
								
								CONSOLE_Print("Player ["+ Name + "] has left every [" +str+ " st/nd/rd/th] game in average.");
							}
							else 
								CONSOLE_Print("Player ["+ Name + "] has played [" +row[0] + "] games here and has not left one of them");
						else
							CONSOLE_Print("Player ["+ Name + "] hasn't played any games here.");
					}
					mysql_free_result(result);
					mysql_close(connection);


				}*/
				else if (Command=="/thr")
				{
					downloadfile("http://assets.throneit.com/games/games.html","thr.html");
					
					ConvertHTMLtoTXT("FGames/thr.html","FGames/thr.txt","throneit.com");

					AddGamesFromTextFile("thr");
				}
				else if (Command.substr(0,4) == "/thr")
				{
					downloadfile("http://assets.throneit.com/games/games.html","thr.html");
					
					ConvertHTMLtoTXT("FGames/thr.html","FGames/thr.txt","throneit.com");
					

					AddGamesFromTextFile("thr",Command.substr(5,Command.size()-5));
				}
				else if (Command=="/dl")
				{
					
					if (gGProxy->m_BNET->GetServer()=="europe.battle.net")
					{
	
						downloadfile("http://forum.ghostgraz.com/feeds/currentgames.txt","graz.txt");
						AddGamesFromTextFile("graz");
					}
					else
					{
						downloadfile("http://assets.throneit.com/games/games.html","thr.html");
					
						ConvertHTMLtoTXT("FGames/thr.html","FGames/thr.txt","throneit.com");

						AddGamesFromTextFile("thr");
					}
				}
				
				else if (Command.substr(0,3) == "/dl")
				{
					if (gGProxy->m_BNET->GetServer()=="europe.battle.net")
					{
						downloadfile("http://forum.ghostgraz.com/feeds/currentgames.txt","graz.txt");
						AddGamesFromTextFile("graz",Command.substr(4,Command.size()-4));
					}
					else
					{
						downloadfile("http://assets.throneit.com/games/games.html","thr.html");
					
						ConvertHTMLtoTXT("FGames/thr.html","FGames/thr.txt","throneit.com");

						AddGamesFromTextFile("thr",Command.substr(4,Command.size()-4));
					}
				}
				
				else if (Command =="/privategamename" || Command == "/pgn")
					CONSOLE_Print("[Phyton] The value of 'privategamename' is: [" + privategamename +"]. Change it with the command '/privategamename <value>' alias '/pgn <value>'.");
				else if ( Command.substr(0,17)=="/privategamename ")//phy 
				{
					privategamename=Command.substr(17,Command.size()-17);
					CONSOLE_Print("[Phyton] Change value of 'privategamename' to ["+privategamename+"].");

				}
				else if ( Command.substr(0,5)=="/pgn ")//phy same again
				{
					privategamename=Command.substr(5,Command.size()-5);
					CONSOLE_Print("[Phyton] Change value of 'privategamename' to ["+privategamename+"].");

				}
				else if ( Command=="/coltest")//pr0 colortest
				{
					stringstream ss;
					string s;
					for(int i = 1; i < 16; i++)
					{
						ss << i;
						ss >> s;
						ss.clear();
						CONSOLE_Print("Color: " + s, false, i);
					}
				}
				else if (Command=="/botprefix")
					CONSOLE_Print("[Phyton] Value of 'botprefix' is ["+botprefix+"]. You can change it with '/botprefix <value>'.");
				else if ( Command.substr(0,11)=="/botprefix ")//phy
				{
					botprefix=Command.substr(11,Command.size()-11);
					CONSOLE_Print("[Phyton] Change value of 'botprefix' to ["+botprefix+"].");

				}
				else if (Command.substr(0,3) == "/wg" )
				{
					if( Command == "/wg")
					{
						//if(!(botprefix.substr(0,botprefix.size())==botprefix))
						//	CONSOLE_Print("FAIL");
						vShallCreateQuiet = false;
						vShallCreate = true;
						CONSOLE_Print("[Phyton] waiting for a bot and then trying to create a game and say [gn: "+privategamename+"].");
						gGProxy->m_BNET->QueueChatCommand("[GProxy++][Phyton] Waiting for a bot and create a game with the name ["+privategamename+"].");
					}
					else if( Command == "/wg off")
					{
						CONSOLE_Print("[Phyton] stopped waiting for a bot to create a game.");
						vShallCreate = false;
					}
					else if( Command == "/wg quiet" ||Command == "/wg q")
					{
						vShallCreate = true;
						vShallCreateQuiet = true;
						CONSOLE_Print("[Phyton] waiting for a bot and then trying to create a game [quiet].");
					}
				}
				else if( Command.substr(0,9) == "/waitgame" &&botprefix!="")//phy waitgame
				{
					if( Command == "/waitgame")
					{
						//if(!(botprefix.substr(0,botprefix.size())==botprefix))
						//	CONSOLE_Print("FAIL");
						vShallCreateQuiet = false;
						vShallCreate = true;
						CONSOLE_Print("[Phyton] waiting for a bot and then trying to create a game and say [gn: "+privategamename+"].");
						gGProxy->m_BNET->QueueChatCommand("[GProxy++][Phyton] Waiting for a bot and create a game with the name ["+privategamename+"].");
					}
					else if( Command == "/waitgame off")
					{
						CONSOLE_Print("[Phyton] stopped waiting for a bot to create a game.");
						vShallCreate = false;
					}
					else if( Command == "/waitgame quiet")
					{
						vShallCreate = true;
						vShallCreateQuiet = true;
						CONSOLE_Print("[Phyton] waiting for a bot and then trying to create a game [quiet].");
					}
				}
				else if (Command.substr(0,8)== "/parrot ")
				{
					gGProxy->parrot=Command.substr(8,Command.size()-8);
					CONSOLE_Print("[Phyton] Parrot ["+gGProxy->parrot+"]!!!111");
				}
				else if (Command == "/parrotoff")
				{
					gGProxy->parrot= "";
					CONSOLE_Print("[Phyton] Parrot off");
				}
				else if (Command =="/parrotall")
					gGProxy->parrot= "Parrot parrot";
				else if (Command =="/parrotignore")
					gGProxy->parrot= "Ignore ignore";
				else if( Command.substr(0,4) == "/sa ")
				{
					//AddGamesFromTextFile(Command.substr(4,Command.size()-4),"print all");
					downloadfile("http://forum.ghostgraz.com/unread/","test.html");
				}
				else if(Command == "/ping")
				{
					//CONSOLE_Print( CIncomingChatEvent->GetPing()
					//
					
					//gGProxy->m_BNET->m_Protocol
				}
				else if( Command.size( ) >= 7 && Command.substr( 0, 6 ) == "/game " )
				{
					string GameName = gInputBuffer.substr( 6 );

					if( !GameName.empty( ) && GameName.size( ) <= 31 )
					{
						gGProxy->m_BNET->SetSearchGameName( GameName );
						autosearch(false);
						CONSOLE_Print( "[BNET] looking for a game named \"" + GameName + "\" for up to two minutes" );
					}
				}
				else if ( Command == "/testvar on" )
					gGProxy->testvar = true;
				else if ( Command == "/testvar off" )
					gGProxy->testvar = false;
				else if( Command == "/help" )
				{
					CONSOLE_Print( ">>> /help" );
					CONSOLE_Print( "", false );
					CONSOLE_Print( "  GProxy++ connects to battle.net and looks for games for you to join.", false );
					CONSOLE_Print( "  If GProxy++ finds any they will be listed on the Warcraft III LAN screen.", false );
					CONSOLE_Print( "  To join a game, simply open Warcraft III and wait at the LAN screen.", false );
					CONSOLE_Print( "  Standard games will be white and GProxy++ enabled games will be blue.", false );
					CONSOLE_Print( "  Note: You must type \"/public\" to enable listing of public games.", false );
					CONSOLE_Print( "", false );
					CONSOLE_Print( "  If you want to join a specific game, type \"/game <gamename>\".", false );
					CONSOLE_Print( "  GProxy++ will look for that game for up to two minutes before giving up.", false );
					CONSOLE_Print( "  This is useful for private games.", false );
					CONSOLE_Print( "", false );
					CONSOLE_Print( "  Please note:", false );
					CONSOLE_Print( "  GProxy++ will join the game using your battle.net name, not your LAN name.", false );
					CONSOLE_Print( "  Other players will see your battle.net name even if you choose another name.", false );
					CONSOLE_Print( "  This is done so that you can be automatically spoof checked.", false );
					CONSOLE_Print( "", false );
					CONSOLE_Print( "  Type \"/commands\" for a full command list.", false );
					CONSOLE_Print( "", false );
				}
				else if( Command == "/public" || Command == "/publicon" || Command == "/public on" || Command == "/list" || Command == "/liston" || Command == "/list on" )
				{
					gGProxy->m_BNET->SetListPublicGames( true );
					CONSOLE_Print( "[BNET] listing of public games enabled" );
				}
				else if( Command == "/publicoff" || Command == "/public off" || Command == "/listoff" || Command == "/list off" )
				{
					gGProxy->m_BNET->SetListPublicGames( false );
					CONSOLE_Print( "[BNET] listing of public games disabled" );
				}
				else if( Command.size( ) >= 4 && Command.substr( 0, 3 ) == "/r " )
				{
					if( !gGProxy->m_BNET->GetReplyTarget( ).empty( ) )
						gGProxy->m_BNET->QueueChatCommand( gInputBuffer.substr( 3 ), gGProxy->m_BNET->GetReplyTarget( ), true );
					else
						CONSOLE_Print( "[BNET] nobody has whispered you yet" );
				}
#ifdef WIN32
				else if( Command == "/start" )
				{
					STARTUPINFO si;
					PROCESS_INFORMATION pi;
					ZeroMemory( &si, sizeof( si ) );
					si.cb = sizeof( si );
					ZeroMemory( &pi, sizeof( pi ) );
					string War3EXE;

					if( !CDKeyTFT.empty( ) )
						War3EXE = War3Path + "Frozen Throne.exe";
					else
						War3EXE = War3Path + "Warcraft III.exe";
//phy nothing
					BOOL hProcess = CreateProcessA( War3EXE.c_str( ), NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, War3Path.c_str( ), LPSTARTUPINFOA( &si ), &pi );

					if( !hProcess )
						CONSOLE_Print( "[GPROXY] failed to start warcraft 3" );
					else
					{
						CONSOLE_Print( "[GPROXY] started warcraft 3" );
						CloseHandle( pi.hProcess );
						CloseHandle( pi.hThread );
					}
				}
#endif
				else if( Command == "/version" )
				{
					CONSOLE_Print( "[GPROXY] GProxy++ Version " + gGProxy->m_Version );
					CONSOLE_Print( string("[GPROXY] ")+char(6)+char(7)+"This mod is by "
						+char(6)+char(2)+" Phyton"+char(6)+char(7)+","+char(6)+char(13)+" Pr0gm4n"+char(6)+char(7)
						+" and "+char(6)+char(14)+"Manufactoring"+char(6)+char(7)+"." );
				}
				else if(Command == "/test")
				{
				}
				else if( gGProxy->m_BNET->GetInGame() )
				{
					if( gGProxy->m_GameStarted )
					{
						if( Command.substr( 0, 3 ) == "/a " )
						{
							string message = gInputBuffer.substr( 3 );
							gGProxy->SendAllMessage( message );
						}
						else if( Command.substr( 0, 5 ) == "/all " )
						{
							string message = input.substr( 5 );
							gGProxy->SendAllMessage( message );
						}
						else
							gGProxy->SendAllyMessage( /*gInputBuffer*/input );
					}
					else
						gGProxy->SendLobbyMessage( /*gInputBuffer*/input );
				}
				else
					gGProxy->m_BNET->QueueChatCommand( /*gInputBuffer*/input );

				commandHistory.push_back( Command );
				commandHistoryIndex = commandHistory.size();

				gInputBuffer.clear( );
			}
#ifdef WIN32
			else if( c == 22 )
			{
				// paste

				char *clipboard = NULL;
				long length = 0;

				if( PDC_getclipboard( &clipboard, &length ) == PDC_CLIP_SUCCESS )
				{
					gInputBuffer += string( clipboard, length );
					PDC_freeclipboard( clipboard );
				}
			}
#endif
			else if( c == 27 )
			{
				// esc

				gInputBuffer.clear( );
			}
			else if( c >= 32 && c <= 255 )
			{
				// printable characters

				gInputBuffer.push_back( c );
			}
#ifdef WIN32
			else if( c == PADSLASH )
				gInputBuffer.push_back( '/' );
			else if( c == PADSTAR )
				gInputBuffer.push_back( '*' );
			else if( c == PADMINUS )
				gInputBuffer.push_back( '-' );
			else if( c == PADPLUS )
				gInputBuffer.push_back( '+' );
#endif
			else if( c == KEY_RESIZE )
				CONSOLE_Resize( );
			else if( c == KEY_UP )
			{
				commandHistoryIndex--;
				if( commandHistoryIndex < 0 )
					commandHistoryIndex = 0;

				if( commandHistoryIndex >= commandHistory.size() )		// This should NEVER happen here
					commandHistoryIndex = commandHistory.size() - 1;	

				if( commandHistory.size() > 0 )
				{
					gInputBuffer.clear();
					gInputBuffer.append( commandHistory.at(commandHistoryIndex) );
				}
			}
			else if( c == KEY_DOWN )
			{
				commandHistoryIndex++;
				if( commandHistoryIndex >= commandHistory.size() )
					commandHistoryIndex = commandHistory.size() - 1;

				if( commandHistoryIndex < 0 ) // This should NEVER happen here
					commandHistoryIndex = 0;

				if( commandHistory.size() > 0 )
				{
					gInputBuffer.clear();
					gInputBuffer.append( commandHistory.at(commandHistoryIndex) );
				}
			}

			// clamp input buffer size
			if( gInputBuffer.size( ) > 200 )
				gInputBuffer.erase( 200 );

			c = wgetch( gInputWindow );
			gInputWindowChanged = true;
		}

		CONSOLE_Draw();

		if( Quit )
			break;
	}

	// shutdown gproxy

	CONSOLE_Print( "[GPROXY] shutting down" );
	delete gGProxy;
	gGProxy = NULL;

#ifdef WIN32
	// shutdown winsock

	CONSOLE_Print( "[GPROXY] shutting down winsock" );
	WSACleanup( );
#endif

	// shutdown curses

	endwin( );
	return 0;
}



//
// CGProxy
//

CGProxy :: CGProxy( bool nTFT, string nWar3Path, string nCDKeyROC, string nCDKeyTFT, string nServer, string nUsername, string nPassword, string nChannel, uint32_t nWar3Version, uint16_t nPort, BYTEARRAY nEXEVersion, BYTEARRAY nEXEVersionHash, string nPasswordHashType, string cpublic, string cfilter,  bool german_languagesupport , bool casearch, bool temp_displayautocreated, bool listing_current_games, int channelWidth)
{
	m_Version = "Public Test Release 1.0 (March 11, 2010)";
	m_LocalServer = new CTCPServer( );
	m_LocalSocket = NULL;
	m_RemoteSocket = new CTCPClient( );
	m_RemoteSocket->SetNoDelay( true );
	m_UDPSocket = new CUDPSocket( );
	m_UDPSocket->SetBroadcastTarget( "127.0.0.1" );
	m_GameProtocol = new CGameProtocol( this );
	m_GPSProtocol = new CGPSProtocol( );
	m_TotalPacketsReceivedFromLocal = 0;
	m_TotalPacketsReceivedFromRemote = 0;
	m_Exiting = false;
	m_TFT = nTFT;
	m_War3Path = nWar3Path;
	m_CDKeyROC = nCDKeyROC;
	m_CDKeyTFT = nCDKeyTFT;
	m_Server = nServer;
	m_Username = nUsername;
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
	m_BNET = new CBNET( this, m_Server, string( ), 0, 0, m_CDKeyROC, m_CDKeyTFT, "USA", "United States", m_Username, m_Password, m_Channel, m_War3Version, nEXEVersion, nEXEVersionHash, nPasswordHashType, 200 );
	m_LocalServer->Listen( string( ), m_Port );
	cfgfilterfirst=true; //phy filter
	cfgfilter=cfilter;//phy filter
	cfgpublic=cpublic;//phy public
	autosearch = true; //pr0 autosearch
	cautosearch = casearch; //pr0 cautosearch
	displayautocreated=temp_displayautocreated;
	m_listing_current_games=listing_current_games;
	testvar = false;
	m_ChannelWidth = channelWidth; // manu
	help_gproxy = m_BNET->m_GProxy;//phy
	m_PlaySound = true; // manu
	
	CONSOLE_Print( "[GPROXY] GProxy++ Version " + m_Version );
}

CGProxy :: ~CGProxy( )
{
	for( vector<CIncomingGameHost *> :: iterator i = m_Games.begin( ); i != m_Games.end( ); i++ )
		m_UDPSocket->Broadcast( 6112, m_GameProtocol->SEND_W3GS_DECREATEGAME( (*i)->GetUniqueGameID( ) ) );

	delete m_LocalServer;
	delete m_LocalSocket;
	delete m_RemoteSocket;
	delete m_UDPSocket;
	delete m_BNET;

	for( vector<CIncomingGameHost *> :: iterator i = m_Games.begin( ); i != m_Games.end( ); i++ )
		delete *i;

	delete m_GameProtocol;
	delete m_GPSProtocol;
	

	while( !m_LocalPackets.empty( ) )
	{
		delete m_LocalPackets.front( );
		m_LocalPackets.pop( );
	}

	while( !m_RemotePackets.empty( ) )
	{
		delete m_RemotePackets.front( );
		m_RemotePackets.pop( );
	}

	while( !m_PacketBuffer.empty( ) )
	{
		delete m_PacketBuffer.front( );
		m_PacketBuffer.pop( );
	}
}

bool CGProxy :: Update( long usecBlock )
{
	unsigned int NumFDs = 0;

	// take every socket we own and throw it in one giant select statement so we can block on all sockets

	int nfds = 0;
	fd_set fd;
	fd_set send_fd;
	FD_ZERO( &fd );
	FD_ZERO( &send_fd );

	// 1. the battle.net socket

	NumFDs += m_BNET->SetFD( &fd, &send_fd, &nfds );

	// 2. the local server

	m_LocalServer->SetFD( &fd, &send_fd, &nfds );
	NumFDs++;

	// 3. the local socket

	if( m_LocalSocket )
	{
		m_LocalSocket->SetFD( &fd, &send_fd, &nfds );
		NumFDs++;
	}

	// 4. the remote socket

	if( !m_RemoteSocket->HasError( ) && m_RemoteSocket->GetConnected( ) )
	{
		m_RemoteSocket->SetFD( &fd, &send_fd, &nfds );
		NumFDs++;
	}

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = usecBlock;

	struct timeval send_tv;
	send_tv.tv_sec = 0;
	send_tv.tv_usec = 0;

#ifdef WIN32
	select( 1, &fd, NULL, NULL, &tv );
	select( 1, NULL, &send_fd, NULL, &send_tv );
#else
	select( nfds + 1, &fd, NULL, NULL, &tv );
	select( nfds + 1, NULL, &send_fd, NULL, &send_tv );
#endif

	if( NumFDs == 0 )
		MILLISLEEP( 50 );

	if( m_BNET->Update( &fd, &send_fd ) )
		return true;

	//
	// accept new connections
	//

	CTCPSocket *NewSocket = m_LocalServer->Accept( &fd );

	if( NewSocket )
	{
		if( m_LocalSocket )
		{
			// someone's already connected, reject the new connection
			// we only allow one person to use the proxy at a time

			delete NewSocket;
		}
		else
		{
			CONSOLE_Print( "[GPROXY] local player connected" );
			m_LocalSocket = NewSocket;
			m_LocalSocket->SetNoDelay( true );
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
			m_JoinedName.clear( );
			m_HostName.clear( );

			while( !m_PacketBuffer.empty( ) )
			{
				delete m_PacketBuffer.front( );
				m_PacketBuffer.pop( );
			}
		}
	}

	if( m_LocalSocket )
	{
		//
		// handle proxying (reconnecting, etc...)
		//

		if( m_LocalSocket->HasError( ) || !m_LocalSocket->GetConnected( ) )
		{
			CONSOLE_Print( "[GPROXY] local player disconnected" );

			if( m_BNET->GetInGame( ) )
				m_BNET->QueueEnterChat( );

			delete m_LocalSocket;
			m_LocalSocket = NULL;

			// ensure a leavegame message was sent, otherwise the server may wait for our reconnection which will never happen
			// if one hasn't been sent it's because Warcraft III exited abnormally

			if( m_GameIsReliable && !m_LeaveGameSent )
			{
				// note: we're not actually 100% ensuring the leavegame message is sent, we'd need to check that DoSend worked, etc...

				BYTEARRAY LeaveGame;
				LeaveGame.push_back( 0xF7 );
				LeaveGame.push_back( 0x21 );
				LeaveGame.push_back( 0x08 );
				LeaveGame.push_back( 0x00 );
				UTIL_AppendByteArray( LeaveGame, (uint32_t)PLAYERLEAVE_GPROXY, false );
				m_RemoteSocket->PutBytes( LeaveGame );
				m_RemoteSocket->DoSend( &send_fd );
			}

			m_RemoteSocket->Reset( );
			m_RemoteSocket->SetNoDelay( true );
			m_RemoteServerIP.clear( );
			m_RemoteServerPort = 0;
		}
		else
		{
			m_LocalSocket->DoRecv( &fd );
			ExtractLocalPackets( );
			ProcessLocalPackets( );

			if( !m_RemoteServerIP.empty( ) )
			{
				if( m_GameIsReliable && m_ActionReceived && GetTime( ) - m_LastActionTime >= 60 )
				{
					if( m_NumEmptyActionsUsed < m_NumEmptyActions )
					{
						SendEmptyAction( );
						m_NumEmptyActionsUsed++;
					}
					else
					{
						SendLocalChat( "GProxy++ ran out of time to reconnect, Warcraft III will disconnect soon." );
						CONSOLE_Print( "[GPROXY] ran out of time to reconnect" );
					}

					m_LastActionTime = GetTime( );
				}

				if( m_RemoteSocket->HasError( ) )
				{
					CONSOLE_Print( "[GPROXY] disconnected from remote server due to socket error" );

					if( m_GameIsReliable && m_ActionReceived && m_ReconnectPort > 0 )
					{
						SendLocalChat( "You have been disconnected from the server due to a socket error." );
						uint32_t TimeRemaining = ( m_NumEmptyActions - m_NumEmptyActionsUsed + 1 ) * 60 - ( GetTime( ) - m_LastActionTime );

						if( GetTime( ) - m_LastActionTime > ( m_NumEmptyActions - m_NumEmptyActionsUsed + 1 ) * 60 )
							TimeRemaining = 0;

						SendLocalChat( "GProxy++ is attempting to reconnect... (" + UTIL_ToString( TimeRemaining ) + " seconds remain)" );
						CONSOLE_Print( "[GPROXY] attempting to reconnect" );
						m_RemoteSocket->Reset( );
						m_RemoteSocket->SetNoDelay( true );
						m_RemoteSocket->Connect( string( ), m_RemoteServerIP, m_ReconnectPort );
						m_LastConnectionAttemptTime = GetTime( );
					}
					else
					{
						if( m_BNET->GetInGame( ) )
							m_BNET->QueueEnterChat( );

						m_LocalSocket->Disconnect( );
						delete m_LocalSocket;
						m_LocalSocket = NULL;
						m_RemoteSocket->Reset( );
						m_RemoteSocket->SetNoDelay( true );
						m_RemoteServerIP.clear( );
						m_RemoteServerPort = 0;
						return false;
					}
				}

				if( !m_RemoteSocket->GetConnecting( ) && !m_RemoteSocket->GetConnected( ) )
				{
					CONSOLE_Print( "[GPROXY] disconnected from remote server" );

					if( m_GameIsReliable && m_ActionReceived && m_ReconnectPort > 0 )
					{
						SendLocalChat( "You have been disconnected from the server." );
						uint32_t TimeRemaining = ( m_NumEmptyActions - m_NumEmptyActionsUsed + 1 ) * 60 - ( GetTime( ) - m_LastActionTime );

						if( GetTime( ) - m_LastActionTime > ( m_NumEmptyActions - m_NumEmptyActionsUsed + 1 ) * 60 )
							TimeRemaining = 0;

						SendLocalChat( "GProxy++ is attempting to reconnect... (" + UTIL_ToString( TimeRemaining ) + " seconds remain)" );
						CONSOLE_Print( "[GPROXY] attempting to reconnect" );
						m_RemoteSocket->Reset( );
						m_RemoteSocket->SetNoDelay( true );
						m_RemoteSocket->Connect( string( ), m_RemoteServerIP, m_ReconnectPort );
						m_LastConnectionAttemptTime = GetTime( );
					}
					else
					{
						if( m_BNET->GetInGame( ) )
							m_BNET->QueueEnterChat( );

						m_LocalSocket->Disconnect( );
						delete m_LocalSocket;
						m_LocalSocket = NULL;
						m_RemoteSocket->Reset( );
						m_RemoteSocket->SetNoDelay( true );
						m_RemoteServerIP.clear( );
						m_RemoteServerPort = 0;
						return false;
					}
				}

				if( m_RemoteSocket->GetConnected( ) )
				{
					if( m_GameIsReliable && m_ActionReceived && m_ReconnectPort > 0 && GetTime( ) - m_RemoteSocket->GetLastRecv( ) >= 20 )
					{
						CONSOLE_Print( "[GPROXY] disconnected from remote server due to 20 second timeout" );
						SendLocalChat( "You have been timed out from the server." );
						uint32_t TimeRemaining = ( m_NumEmptyActions - m_NumEmptyActionsUsed + 1 ) * 60 - ( GetTime( ) - m_LastActionTime );

						if( GetTime( ) - m_LastActionTime > ( m_NumEmptyActions - m_NumEmptyActionsUsed + 1 ) * 60 )
							TimeRemaining = 0;

						SendLocalChat( "GProxy++ is attempting to reconnect... (" + UTIL_ToString( TimeRemaining ) + " seconds remain)" );
						CONSOLE_Print( "[GPROXY] attempting to reconnect" );
						m_RemoteSocket->Reset( );
						m_RemoteSocket->SetNoDelay( true );
						m_RemoteSocket->Connect( string( ), m_RemoteServerIP, m_ReconnectPort );
						m_LastConnectionAttemptTime = GetTime( );
					}
					else
					{
						m_RemoteSocket->DoRecv( &fd );
						ExtractRemotePackets( );
						ProcessRemotePackets( );

						if( m_GameIsReliable && m_ActionReceived && m_ReconnectPort > 0 && GetTime( ) - m_LastAckTime >= 10 )
						{
							m_RemoteSocket->PutBytes( m_GPSProtocol->SEND_GPSC_ACK( m_TotalPacketsReceivedFromRemote ) );
							m_LastAckTime = GetTime( );
						}

						m_RemoteSocket->DoSend( &send_fd );
					}
				}

				if( m_RemoteSocket->GetConnecting( ) )
				{
					// we are currently attempting to connect

					if( m_RemoteSocket->CheckConnect( ) )
					{
						// the connection attempt completed

						if( m_GameIsReliable && m_ActionReceived )
						{
							// this is a reconnection, not a new connection
							// if the server accepts the reconnect request it will send a GPS_RECONNECT back requesting a certain number of packets

							SendLocalChat( "GProxy++ reconnected to the server!" );
							SendLocalChat( "==================================================" );
							CONSOLE_Print( "[GPROXY] reconnected to remote server" );

							// note: even though we reset the socket when we were disconnected, we haven't been careful to ensure we never queued any data in the meantime
							// therefore it's possible the socket could have data in the send buffer
							// this is bad because the server will expect us to send a GPS_RECONNECT message first
							// so we must clear the send buffer before we continue
							// note: we aren't losing data here, any important messages that need to be sent have been put in the packet buffer
							// they will be requested by the server if required

							m_RemoteSocket->ClearSendBuffer( );
							m_RemoteSocket->PutBytes( m_GPSProtocol->SEND_GPSC_RECONNECT( m_PID, m_ReconnectKey, m_TotalPacketsReceivedFromRemote ) );

							// we cannot permit any forwarding of local packets until the game is synchronized again
							// this will disable forwarding and will be reset when the synchronization is complete

							m_Synchronized = false;
						}
						else
							CONSOLE_Print( "[GPROXY] connected to remote server" );
					}
					else if( GetTime( ) - m_LastConnectionAttemptTime >= 10 )
					{
						// the connection attempt timed out (10 seconds)

						CONSOLE_Print( "[GPROXY] connect to remote server timed out" );

						if( m_GameIsReliable && m_ActionReceived && m_ReconnectPort > 0 )
						{
							uint32_t TimeRemaining = ( m_NumEmptyActions - m_NumEmptyActionsUsed + 1 ) * 60 - ( GetTime( ) - m_LastActionTime );

							if( GetTime( ) - m_LastActionTime > ( m_NumEmptyActions - m_NumEmptyActionsUsed + 1 ) * 60 )
								TimeRemaining = 0;

							SendLocalChat( "GProxy++ is attempting to reconnect... (" + UTIL_ToString( TimeRemaining ) + " seconds remain)" );
							CONSOLE_Print( "[GPROXY] attempting to reconnect" );
							m_RemoteSocket->Reset( );
							m_RemoteSocket->SetNoDelay( true );
							m_RemoteSocket->Connect( string( ), m_RemoteServerIP, m_ReconnectPort );
							m_LastConnectionAttemptTime = GetTime( );
						}
						else
						{
							if( m_BNET->GetInGame( ) )
								m_BNET->QueueEnterChat( );

							m_LocalSocket->Disconnect( );
							delete m_LocalSocket;
							m_LocalSocket = NULL;
							m_RemoteSocket->Reset( );
							m_RemoteSocket->SetNoDelay( true );
							m_RemoteServerIP.clear( );
							m_RemoteServerPort = 0;
							return false;
						}
					}
				}
			}

			m_LocalSocket->DoSend( &send_fd );
		}
	}
	else
	{
		//
		// handle game listing
		//

		if( GetTime( ) - m_LastRefreshTime >= 2 )
		{
			for( vector<CIncomingGameHost *> :: iterator i = m_Games.begin( ); i != m_Games.end( ); )
			{
				// expire games older than 60 seconds

				if( GetTime( ) - (*i)->GetReceivedTime( ) >= 60 )
				{
					// don't forget to remove it from the LAN list first

					m_UDPSocket->Broadcast( 6112, m_GameProtocol->SEND_W3GS_DECREATEGAME( (*i)->GetUniqueGameID( ) ) );
					delete *i;
					i = m_Games.erase( i );
					continue;
				}

				BYTEARRAY MapGameType;
				UTIL_AppendByteArray( MapGameType, (*i)->GetGameType( ), false );
				UTIL_AppendByteArray( MapGameType, (*i)->GetParameter( ), false );
				BYTEARRAY MapFlags = UTIL_CreateByteArray( (*i)->GetMapFlags( ), false );
				BYTEARRAY MapWidth = UTIL_CreateByteArray( (*i)->GetMapWidth( ), false );
				BYTEARRAY MapHeight = UTIL_CreateByteArray( (*i)->GetMapHeight( ), false );
				string GameName = (*i)->GetGameName( );

				// colour reliable game names so they're easier to pick out of the list

				if( (*i)->GetMapWidth( ) == 1984 && (*i)->GetMapHeight( ) == 1984 )
				{
					string tempstr = (*i)->GetHostName( );
					if (GameName == privategamename)
						GameName = "|cFFFF0000"+GameName;
					else if (tempstr.substr(0,9) == "GhostGraz")//phy ghostgrazgame orange
						GameName = "|cFFFF9F40"+ GameName;
					else if(GameName.substr(0,3)=="thr")
					{
						if(GameName.substr(0,4)=="thr-")
							GameName = "|cFF00FF00"+ GameName;
						else if(GameName.substr(0,5)=="thr2-")
							GameName = "|cFFCCCC00"+ GameName;
						else if(GameName.substr(0,5)=="thr3-")
							GameName = "|cFFFF0000"+ GameName;
					}
					else
						GameName = "|cFF4080C0" + GameName;

					// unfortunately we have to truncate them
					// is this acceptable?

					if( GameName.size( ) > 31 )
						GameName = GameName.substr( 0, 31 );
				}
				//m_UDPSocket->Broadcast( 6112, m_GameProtocol->SEND_W3GS_GAMEINFO( m_TFT, m_War3Version, MapGameType, MapFlags, MapWidth, MapHeight, GameName, "throneit bot", 20, (*i)->GetMapPath( ), (*i)->GetMapCRC( ), 10, 10, m_Port, (*i)->GetUniqueGameID( ), (*i)->GetUniqueGameID( ) ) );

				m_UDPSocket->Broadcast( 6112, m_GameProtocol->SEND_W3GS_GAMEINFO( m_TFT, m_War3Version, MapGameType, MapFlags, MapWidth, MapHeight, GameName, (*i)->GetHostName( ), (*i)->GetElapsedTime( ), (*i)->GetMapPath( ), (*i)->GetMapCRC( ), 12, 12, m_Port, (*i)->GetUniqueGameID( ), (*i)->GetUniqueGameID( ) ) );
				/*stringstream ss;
				string s;
				ss << (*i)->GetUniqueGameID( );
				ss >> s;
				CONSOLE_Print(s);*/
				i++;
			}

			m_LastRefreshTime = GetTime( );
		}
	}

	return m_Exiting;
}

void CGProxy :: ExtractLocalPackets( )
{
	if( !m_LocalSocket )
		return;

	string *RecvBuffer = m_LocalSocket->GetBytes( );
	BYTEARRAY Bytes = UTIL_CreateByteArray( (unsigned char *)RecvBuffer->c_str( ), RecvBuffer->size( ) );

	// a packet is at least 4 bytes so loop as long as the buffer contains 4 bytes

	while( Bytes.size( ) >= 4 )
	{
		// byte 0 is always 247

		if( Bytes[0] == W3GS_HEADER_CONSTANT )
		{
			// bytes 2 and 3 contain the length of the packet

			uint16_t Length = UTIL_ByteArrayToUInt16( Bytes, false, 2 );

			if( Length >= 4 )
			{
				if( Bytes.size( ) >= Length )
				{
					// we have to do a little bit of packet processing here
					// this is because we don't want to forward any chat messages that start with a "/" as these may be forwarded to battle.net instead
					// in fact we want to pretend they were never even received from the proxy's perspective

					bool Forward = true;
					BYTEARRAY Data = BYTEARRAY( Bytes.begin( ), Bytes.begin( ) + Length );

					if( Bytes[1] == CGameProtocol :: W3GS_CHAT_TO_HOST )
					{
						if( Data.size( ) >= 5 )
						{
							unsigned int i = 5;
							unsigned char Total = Data[4];
							
							BYTEARRAY toPIDs = BYTEARRAY( Data.begin( ) + i, Data.begin( ) + i + Total );
							int FromPID = (int) Data[i];

							if( Total > 0 && Data.size( ) >= i + Total )
							{
								i += Total;
								unsigned char Flag = Data[i + 1];
								i += 2;

								BYTEARRAY ExtraFlags;
								string MessageString;

								if( Flag == 16)
								{
									// Chat message
									BYTEARRAY Message = UTIL_ExtractCString( Data, i );
									MessageString = string ( Message.begin(), Message.end() );
								}
								else if( Flag == 32 )
								{
									// Extra flags
									ExtraFlags = BYTEARRAY( Data.begin( ) + i, Data.begin( ) + i+4 );
									BYTEARRAY Message = UTIL_ExtractCString( Data, i + 4 );
									MessageString = string( Message.begin( ), Message.end( ) );
								}

								if( Flag == 16 || Flag == 32 ) // Chat message
								{
									if( Flag == 16 )
									{
										CONSOLE_Print( "[LOBBY]["+gGProxy->m_Username+"] "+MessageString );
									}
									else // GameStarted
									{
										// ExtraFlags[0] stores the type of chat message as defined by Warcraft III.
										if( ExtraFlags[0] == 0 ) // 0 is an All message
										{
											CONSOLE_Print( string() +char(6) +char(15) +"[ALL]["+gGProxy->m_Username+"] "+MessageString );
										}
										else if( ExtraFlags[0] == 1 ) // 1 is an Allies message
										{
											CONSOLE_Print( "[ALLY]["+gGProxy->m_Username+"] "+MessageString );
										}
										else if( ExtraFlags[0] == 2 ) // 2 is an Observer/Referee message
										{
											CONSOLE_Print( string() +char(6) +char(3) +"[OBSERVER]["+gGProxy->m_Username+"] "+MessageString );
										}
										else if( ExtraFlags[0] >= 3 ) // 3+ are private messages
										{
											// Based on limited testing it seems that
											// the extra flags' first byte contains 3 plus the recipient's colour to denote a private message.
											// Due to the fact that the Datapacket also contains the FromPID, we do not need to extract the color.

											CONSOLE_Print( string() +char(6) +char(11) +"[PRIVATE][TO]["+gGProxy->m_Username+"] "+MessageString );
										}
									}

									gMainWindowChanged = true;
									CONSOLE_Draw();
								}

								Forward = CheckForwarding( MessageString );
							}
						}
					}

					if( Forward )
					{
						m_LocalPackets.push( new CCommandPacket( W3GS_HEADER_CONSTANT, Bytes[1], Data ) );
						m_PacketBuffer.push( new CCommandPacket( W3GS_HEADER_CONSTANT, Bytes[1], Data ) );
						m_TotalPacketsReceivedFromLocal++;
					}

					*RecvBuffer = RecvBuffer->substr( Length );
					Bytes = BYTEARRAY( Bytes.begin( ) + Length, Bytes.end( ) );
				}
				else
					return;
			}
			else
			{
				CONSOLE_Print( "[GPROXY] received invalid packet from local player (bad length)" );
				m_Exiting = true;
				return;
			}
		}
		else
		{
			CONSOLE_Print( "[GPROXY] received invalid packet from local player (bad header constant)" );
			m_Exiting = true;
			return;
		}
	}
}

bool CGProxy :: CheckForwarding( string MessageString )
{
	bool Forward = true;
	string Command = MessageString;
	transform( Command.begin( ), Command.end( ), Command.begin( ), (int(*)(int))tolower );

	if( Command.size( ) >= 1 && Command.substr( 0, 1 ) == "/" )
	{
		Forward = false;
		
		if( Command.substr( 0, 3 ) == "/p " || Command.substr( 0, 8 ) == "/phrase " )
		{
			string filePath = "phrase/";

			if( Command.substr( 0, 3 ) == "/p " )
				filePath.append( Command.substr(3) );
			else if( Command.substr( 0, 8 ) == "/phrase " )
				filePath.append( Command.substr(8) );

			if( !textEndsWith( filePath, ".txt" ) )
			{
				filePath.append( ".txt" );
			}
			//
				char help[100];
				int i;
				for (i = 0;i<100;i++)
					help[i]=NULL;
				for (i = 0;i<filePath.size();i++)
					help[i]=filePath[i];
				fstream infile( help );
			//
			if(infile) // File exists
			{
				char messageText[224]; // Maximum chat text is 224 bytes.
				do //while( !infile.eof() )
				{
					infile.getline( messageText, 224 );
					string tmp=parseTextline(messageText);
					//messageText=parseTextline(messageText);

					if(gGProxy->m_BNET->GetInGame()&&(messageText[0]!="#"[0]))
					{
						if((messageText[0]!="%"[0]))
						{
							if(messageText[0]!="~"[0])
								if (gGProxy->m_GameStarted)
									gGProxy->SendAllMessage( tmp );
								else
									gGProxy->SendLobbyMessage( tmp );
							else
							{
								
								Sleep(50);
							
					
								
							}
						}
						else 
						{
							//cchatcommand=tmp.substr(1);
							//CreateThread( NULL, 0, LPTHREAD_START_ROUTINE(docommand), NULL, 0, NULL );
							gGProxy->m_BNET->QueueChatCommand( tmp.substr(1) );
						}
					}
					else if(messageText[0]!="#"[0])
						gGProxy->m_BNET->QueueChatCommand( tmp );
				}while( !infile.eof() );
				//SendLocalChat("->>>>>>finished sending->>>>>>>>");
				infile.close();	
			}
			else
			{
				CONSOLE_Print(string()+ "[ERROR] File \""+/*filePath*/ help+"\" does not exist!" );
				SendLocalChat(string()+ "File \""+/*filePath*/help+"\" does not exist!" );
			}
		}
		else if( Command.size( ) >= 5 && Command.substr( 0, 4 ) == "/re " )
		{
			if( m_BNET->GetLoggedIn( ) )
			{
				if( !m_BNET->GetReplyTarget( ).empty( ) )
				{
					m_BNET->QueueChatCommand( MessageString.substr( 4 ), m_BNET->GetReplyTarget( ), true );
					SendLocalChat( "Whispered to " + m_BNET->GetReplyTarget( ) + ": " + MessageString.substr( 4 ) );
				}
				else
					SendLocalChat( "Nobody has whispered you yet." );
			}
			else
				SendLocalChat( "You are not connected to battle.net." );
		}
		else if( Command == "/sc" || Command == "/spoof" || Command == "/spoofcheck" || Command == "/spoof check" )
		{
			if( m_BNET->GetLoggedIn( ) )
			{
				if( !m_GameStarted )
				{
					m_BNET->QueueChatCommand( "spoofcheck", m_HostName, true );
					SendLocalChat( "Whispered to " + m_HostName + ": spoofcheck" );
				}
				else
					SendLocalChat( "The game has already started." );
			}
			else
				SendLocalChat( "You are not connected to battle.net." );
		}
		else if( Command.substr(0,4) == "/wh " )
		{
			string mess=Command.substr(4,Command.size()-4);
			m_BNET->QueueChatCommand(mess,m_HostName,true);
			SendLocalChat( "Whispered to host ["+m_HostName+"] "+mess);
		}
		else if( Command.substr(0,5) == "/whs " )
		{	
			string mess=Command.substr(5,Command.size()-5);
			m_BNET->QueueChatCommand("!say " + mess,m_HostName,true);
			SendLocalChat( "Whispered to host ["+m_HostName+"] !say "+mess);
		}
		else if( Command.substr(0,5)=="/cmd ")
			m_BNET->QueueChatCommand(Command.substr(5,Command.size()-5));
		else if ( Command == "/host")
			SendLocalChat( "Hosting player/bot is ["+m_HostName+"]. (use '/wh <message>' to whisper to him)" );
		else if( Command == "/status" )
		{
			if( m_LocalSocket )
			{
				if( m_GameIsReliable && m_ReconnectPort > 0 )
					SendLocalChat( "GProxy++ disconnect protection: Enabled" );
				else
					SendLocalChat( "GProxy++ disconnect protection: Disabled" );

				if( m_BNET->GetLoggedIn( ) )
					SendLocalChat( "battle.net: Connected" );
				else
					SendLocalChat( "battle.net: Disconnected" );
			}
		}
		else if( Command.size( ) >= 4 && Command.substr( 0, 3 ) == "/w " )
		{
			if( m_BNET->GetLoggedIn( ) )
			{
				string Message;
				string Target;
				string Prefix;
				int Messagebegins;
				Messagebegins = MessageString.find_first_of(" ",4);
				

				m_BNET->QueueChatCommand( MessageString );
				SendLocalChat( "Whisper to "+MessageString.substr(3,Messagebegins-3)+": "+MessageString.substr(Messagebegins));
			}
			else
				SendLocalChat( "You are not connected to battle.net." );
		}
	}

	return Forward;
}

void CGProxy :: ProcessLocalPackets( )
{
	if( !m_LocalSocket )
		return;

	while( !m_LocalPackets.empty( ) )
	{
		CCommandPacket *Packet = m_LocalPackets.front( );
		m_LocalPackets.pop( );
		BYTEARRAY Data = Packet->GetData( );

		if( Packet->GetPacketType( ) == W3GS_HEADER_CONSTANT )
		{
			if( Packet->GetID( ) == CGameProtocol :: W3GS_REQJOIN )
			{
				if( Data.size( ) >= 20 )
				{
					// parse

					uint32_t HostCounter = UTIL_ByteArrayToUInt32( Data, false, 4 );
					uint32_t EntryKey = UTIL_ByteArrayToUInt32( Data, false, 8 );
					unsigned char Unknown = Data[12];
					uint16_t ListenPort = UTIL_ByteArrayToUInt16( Data, false, 13 );
					uint32_t PeerKey = UTIL_ByteArrayToUInt32( Data, false, 15 );
					BYTEARRAY Name = UTIL_ExtractCString( Data, 19 );
					string NameString = string( Name.begin( ), Name.end( ) );
					BYTEARRAY Remainder = BYTEARRAY( Data.begin( ) + Name.size( ) + 20, Data.end( ) );

					if( Remainder.size( ) == 18 )
					{
						// lookup the game in the main list

						bool GameFound = false;

						for( vector<CIncomingGameHost *> :: iterator i = m_Games.begin( ); i != m_Games.end( ); i++ )
						{
							if( (*i)->GetUniqueGameID( ) == EntryKey )
							{
								CONSOLE_Print( "[GPROXY] local player requested game name [" + (*i)->GetGameName( ) + "]" );

								if( NameString != m_Username )
									CONSOLE_Print( "[GPROXY] using battle.net name [" + m_Username + "] instead of requested name [" + NameString + "]" );

								CONSOLE_Print( "[GPROXY] connecting to remote server [" + (*i)->GetIPString( ) + "] on port " + UTIL_ToString( (*i)->GetPort( ) ) );
								m_RemoteServerIP = (*i)->GetIPString( );
								m_RemoteServerPort = (*i)->GetPort( );
								m_RemoteSocket->Reset( );
								m_RemoteSocket->SetNoDelay( true );
								m_RemoteSocket->Connect( string( ), m_RemoteServerIP, m_RemoteServerPort );
								m_LastConnectionAttemptTime = GetTime( );
								m_GameIsReliable = ( (*i)->GetMapWidth( ) == 1984 && (*i)->GetMapHeight( ) == 1984 );
								m_GameStarted = false;

								// rewrite packet

								BYTEARRAY DataRewritten;
								DataRewritten.push_back( W3GS_HEADER_CONSTANT );
								DataRewritten.push_back( Packet->GetID( ) );
								DataRewritten.push_back( 0 );
								DataRewritten.push_back( 0 );
								UTIL_AppendByteArray( DataRewritten, (*i)->GetHostCounter( ), false );
								UTIL_AppendByteArray( DataRewritten, (uint32_t)0, false );
								DataRewritten.push_back( Unknown );
								UTIL_AppendByteArray( DataRewritten, ListenPort, false );
								UTIL_AppendByteArray( DataRewritten, PeerKey, false );
								UTIL_AppendByteArray( DataRewritten, m_Username );
								UTIL_AppendByteArrayFast( DataRewritten, Remainder );
								BYTEARRAY LengthBytes;
								LengthBytes = UTIL_CreateByteArray( (uint16_t)DataRewritten.size( ), false );
								DataRewritten[2] = LengthBytes[0];
								DataRewritten[3] = LengthBytes[1];
								Data = DataRewritten;

								// tell battle.net we're joining a game (for automatic spoof checking)
								m_BNET->QueueJoinGame( (*i)->GetGameName( ) );

								// save the hostname for later (for manual spoof checking)
								m_JoinedName = NameString;
								m_HostName = (*i)->GetHostName( );
								GameFound = true;

								// Manufactorings work
								m_GameName = (*i)->GetGameName( );
								gChannelWindowChanged = true;
								CONSOLE_Draw();

								break;
							}
						}

						if( !GameFound )
						{
							CONSOLE_Print( "[GPROXY] local player requested unknown game (expired?)" );
							m_LocalSocket->Disconnect( );
						}
					}
					else
						CONSOLE_Print( "[GPROXY] received invalid join request from local player (invalid remainder)" );
				}
				else
					CONSOLE_Print( "[GPROXY] received invalid join request from local player (too short)" );
			}
			else if( Packet->GetID( ) == CGameProtocol :: W3GS_LEAVEGAME )
			{
				m_LeaveGameSent = true;
				m_LocalSocket->Disconnect( );
				slots.clear();
				gGProxy->m_BNET->QueueChatCommand( string("/whois ").append(gGProxy->m_HostName) );
			}
			else if( Packet->GetID( ) == CGameProtocol :: W3GS_CHAT_TO_HOST )
			{
				// handled in ExtractLocalPackets (yes, it's ugly)
			}
		}

		// warning: do not forward any data if we are not synchronized (e.g. we are reconnecting and resynchronizing)
		// any data not forwarded here will be cached in the packet buffer and sent later so all is well

		if( m_RemoteSocket && m_Synchronized )
			m_RemoteSocket->PutBytes( Data );

		delete Packet;
	}
}

void CGProxy :: ExtractRemotePackets( )
{
	string *RecvBuffer = m_RemoteSocket->GetBytes( );
	BYTEARRAY Bytes = UTIL_CreateByteArray( (unsigned char *)RecvBuffer->c_str( ), RecvBuffer->size( ) );

	// a packet is at least 4 bytes so loop as long as the buffer contains 4 bytes

	while( Bytes.size( ) >= 4 )
	{
		if( Bytes[0] == W3GS_HEADER_CONSTANT || Bytes[0] == GPS_HEADER_CONSTANT )
		{
			// bytes 2 and 3 contain the length of the packet

			uint16_t Length = UTIL_ByteArrayToUInt16( Bytes, false, 2 );

			if( Length >= 4 )
			{
				if( Bytes.size( ) >= Length )
				{
					m_RemotePackets.push( new CCommandPacket( Bytes[0], Bytes[1], BYTEARRAY( Bytes.begin( ), Bytes.begin( ) + Length ) ) );

					if( Bytes[0] == W3GS_HEADER_CONSTANT )
						m_TotalPacketsReceivedFromRemote++;

					*RecvBuffer = RecvBuffer->substr( Length );
					Bytes = BYTEARRAY( Bytes.begin( ) + Length, Bytes.end( ) );
				}
				else
					return;
			}
			else
			{
				CONSOLE_Print( "[GPROXY] received invalid packet from remote server (bad length)" );
				m_Exiting = true;
				return;
			}
		}
		else
		{
			CONSOLE_Print( "[GPROXY] received invalid packet from remote server (bad header constant)" );
			m_Exiting = true;
			return;
		}
	}
}

void CGProxy :: ProcessRemotePackets( )
{
	if( !m_LocalSocket || !m_RemoteSocket )
		return;

	while( !m_RemotePackets.empty( ) )
	{
		CCommandPacket *Packet = m_RemotePackets.front( );
		m_RemotePackets.pop( );

		if( Packet->GetPacketType( ) == W3GS_HEADER_CONSTANT )
		{
			if( Packet->GetID( ) == CGameProtocol :: W3GS_CHAT_FROM_HOST )
			{
				BYTEARRAY Data = Packet->GetData( );
				unsigned int i = 5;

				unsigned char toPID_Count = Data[4];
				BYTEARRAY toPIDs = BYTEARRAY( Data.begin( ) + i, Data.begin( ) + i + toPID_Count );
				
				i += toPID_Count;

				int FromPID = (int) Data[i];
				unsigned char Flag = Data[i+1];
				i += 2;
				BYTEARRAY ExtraFlags;
				string MessageString;

				if( Flag == 16)
				{
					// Chat message
					BYTEARRAY Message = UTIL_ExtractCString( Data, i );
					MessageString = string ( Message.begin(), Message.end() );
				}
				else if( Flag >= 17 && Flag <= 20 )
				{
					// team/colour/race/handicap change request
				}
				else if( Flag == 32 )
				{
					// Extra flags
					ExtraFlags = BYTEARRAY( Data.begin( ) + i, Data.begin( ) + i+4 );
					BYTEARRAY Message = UTIL_ExtractCString( Data, i + 4 );
					MessageString = string( Message.begin( ), Message.end( ) );
				}

				if( Flag == 16 || Flag == 32 ) // Chat message
				{
					string playerName;
					if( playerNames[FromPID].substr(0,2) == "|c" || playerNames[FromPID].substr(0,2) == "|C" ) // Colored name
						playerName = string() + char(6) + char(14) + playerNames[FromPID].substr(10) + char(6) + char(7);
					else 
						playerName = playerNames[FromPID];

					// If the message is a autogenerated ban message
					if( MessageString.find("Player [") != string::npos
						&& MessageString.find("] was banned by player [") != string::npos
						&& MessageString.find("] on server [") != string::npos )
					{
						sndPlaySound(L"Sounds\\Player got banned.wav",SND_ASYNC | SND_FILENAME);
					}

					// If the message is a autogenerated "same ip" message
					if( (MessageString.find("Player [") != string::npos 
						&& MessageString.find("] has the same IP address as: ") != string::npos) 
						|| MessageString.find("same IPs:") != string::npos )
					{
						sndPlaySound(L"Sounds\\Same IP Sound.wav",SND_ASYNC | SND_FILENAME);
					}

					if( Flag == 16 )
					{
						CONSOLE_Print( "[LOBBY]["+playerName+"] "+MessageString );
					}
					else // GameStarted
					{
						// ExtraFlags[0] stores the type of chat message as defined by Warcraft III.
						if( ExtraFlags[0] == 0 ) // 0 is an All message
						{
							CONSOLE_Print( string() +char(6) +char(15) +"[ALL]["+playerName+"] "+MessageString );
						}
						else if( ExtraFlags[0] == 1 ) // 1 is an Allies message
						{
							CONSOLE_Print( "[ALLY]["+playerName+"] "+MessageString );
						}
						else if( ExtraFlags[0] == 2 ) // 2 is an Observer/Referee message
						{
							CONSOLE_Print( string() +char(6) +char(3) +"[OBSERVER]["+playerName+"] "+MessageString );
						}
						else if( ExtraFlags[0] >= 3 ) // 3+ are private messages
						{
							// Based on limited testing it seems that
							// the extra flags' first byte contains 3 plus the recipient's colour to denote a private message.
							// Due to the fact that the Datapacket also contains the FromPID, we do not need to extract the color.

							CONSOLE_Print( string() +char(6) +char(11) +"[PRIVATE][FROM]["+playerNames[FromPID]+"] "+MessageString );
						}
					}

					gMainWindowChanged = true;
					CONSOLE_Draw();
				}
			}
			if( Packet->GetID( ) == CGameProtocol :: W3GS_PLAYERLEAVE_OTHERS)
			{
				BYTEARRAY Data = Packet->GetData( );

				/*for( vector<CIncomingSlots *> :: iterator it = slots.begin( ); it != slots.end( ); it++ )
				{
					if( (*it)->GetPID() == ((int) Data[4]) )
					{
						delete *it;
						it = slots.erase(it);
						break;
					}
				}*/

				if( gGProxy->m_GameStarted )
				{
					if( gGProxy->m_PlaySound )
					{
						sndPlaySound(L"Sounds\\Player leaves.wav",SND_ASYNC | SND_FILENAME);
					}
					CONSOLE_Print( "[ALL] "+playerNames[((int) Data[4])]+" has left the game." );
				}
				else
					CONSOLE_Print( "[LOBBY] "+playerNames[((int) Data[4])]+" has left the game." );

				gChannelWindowChanged = true;
				CONSOLE_Draw();
			}
			if( Packet->GetID( ) == CGameProtocol :: W3GS_SLOTINFOJOIN )
			{
				BYTEARRAY Data = Packet->GetData( );
				
				if( Data.size( ) >= 6 )
				{
					uint16_t SlotInfoSize = UTIL_ByteArrayToUInt16( Data, false, 4 );

					if( Data.size( ) >= 7 + SlotInfoSize )
					{
						m_ChatPID = Data[6 + SlotInfoSize];
						playerNames[((int) m_ChatPID)] = m_Username;
					}
					
					// send a GPS_INIT packet
					// if the server doesn't recognize it (e.g. it isn't GHost++) we should be kicked
					CONSOLE_Print( "[GPROXY] join request accepted by remote server" );
					if( m_GameIsReliable )
					{
						CONSOLE_Print( "[GPROXY] detected reliable game, starting GPS handshake" );
						m_RemoteSocket->PutBytes( m_GPSProtocol->SEND_GPSC_INIT( 1 ) );
					}
					else
						CONSOLE_Print( "[GPROXY] detected standard game, disconnect protection disabled" );

					unsigned int i = 6;
					unsigned char totalSlots = Data[i];
					i += 1;
					slots.clear();

					teams = 0;
					vector <int> processedTeams;

					for( int j = 0; j < totalSlots; j++ )
					{
						slots.push_back( new CIncomingSlots( Data[i], Data[i+1], Data[i+2], Data[i+3], 
							Data[i+4], Data[i+5], Data[i+6], Data[i+7], Data[i+8] ) );

						if( ((int) Data[i]) == m_ChatPID )
							teamNumber = (int) Data[i+4];

						if ( processedTeams.size() == 0 )
						{
							processedTeams.push_back( (int) Data[i+4] );
							teams++;
						}
						else
						{
							bool teamProcessed = false;
							for ( vector<int> :: iterator it = processedTeams.begin(); it != processedTeams.end(); it++ )
							{
								if( (*it) == (int) Data[i+4] )
								{
									teamProcessed = true;
									break;
								}
							}

							if( !teamProcessed )
							{
								processedTeams.push_back ((int) Data[i+4]);
								teams++;
							}
						}
						
						i += 9;
					}

					gChannelWindowChanged = true;
					CONSOLE_Draw();
				}
				else
					CONSOLE_Print( "[SLOTINFOJOIN] Small data recieved!" );
			}
			else if ( Packet->GetID( ) == CGameProtocol :: W3GS_SLOTINFO )
			{
				BYTEARRAY Data = Packet->GetData( );
				
				uint16_t SlotInfoSize = UTIL_ByteArrayToUInt16( Data, false, 4 );

				if( Data.size( ) >= 7 + SlotInfoSize )
				{
					m_ChatPID = Data[6 + SlotInfoSize];
					playerNames[((int) m_ChatPID)] = m_Username;
				}

				unsigned int i = 6;
				unsigned char totalSlots = Data[i];
				i += 1;

				teams = 0;
				vector <int> processedTeams;

				for( int j = 0; j < totalSlots; j++ )
				{
					for( vector<CIncomingSlots *> :: iterator it = slots.begin( ); it != slots.end( ); it++ )
					{
						if( ((*it)->GetColor() == ((int) Data[i+5])) )
						{
							(*it)->SetPID( ((int) Data[i]) );
							(*it)->SetDownloadStatus( ((int) Data[i+1]) );
							(*it)->SetSlotStatus( ((int) Data[i+2]) );
							(*it)->SetComputerStatus( ((int) Data[i+3]) );
							(*it)->SetTeam( ((int) Data[i+4]) );
							(*it)->SetColor( ((int) Data[i+5]) );
							(*it)->SetRace( ((int) Data[i+6]) );
							(*it)->SetComputerType( ((int) Data[i+7]) );
							(*it)->SetHandicap( ((int) Data[i+8]) );
							(*it)->SetName( playerNames[((int) Data[i])] );
							break;
						}
					}

					if ( processedTeams.size() == 0 )
					{
						processedTeams.push_back( (int) Data[i+4] );
						teams++;
					}
					else
					{
						bool teamProcessed = false;
						for ( vector<int> :: iterator it = processedTeams.begin(); it != processedTeams.end(); it++ )
						{
							if( (*it) == (int) Data[i+4] )
							{
								teamProcessed = true;
								break;
							}
						}

						if( !teamProcessed )
						{
							processedTeams.push_back ((int) Data[i+4]);
							teams++;
						}
					}
					i += 9;
				}

				gChannelWindowChanged = true;
				CONSOLE_Draw();
			}
			else if ( Packet->GetID( ) == CGameProtocol :: W3GS_PLAYERINFO )
			{
				BYTEARRAY data = Packet->GetData( );

				unsigned int i = 8;
				int PID = (int) data[i];
				i += 1;
				BYTEARRAY Name = UTIL_ExtractCString( data, i );
				string playersName = string(Name.begin(), Name.end());

				playerNames[PID] = playersName;
				CONSOLE_Print( "[LOBBY] "+playersName+" has joined the game." );

				for( vector<CIncomingFriendList *> :: iterator i = friendList.begin( ); i != friendList.end( ); i++ )
				{
					if( (*i)->GetAccount().compare(playersName) == 0 )
					{
						sndPlaySound(L"Sounds\\VIP joins.wav",SND_ASYNC | SND_FILENAME);
					}
				}
			}
			else if ( Packet->GetID( ) == CGameProtocol :: W3GS_MAPCHECK )
			{
				BYTEARRAY data = Packet->GetData( );

				BYTEARRAY path = UTIL_ExtractCString( data, 8 );
				string filePath = string(path.begin(), path.end());

				transform( filePath.begin( ), filePath.end( ), filePath.begin( ), (int(*)(int))tolower );
				if( filePath.find("dota") != string::npos )
					DotAmap = true;
				else
					DotAmap = false;
			}
			else if( Packet->GetID( ) == CGameProtocol :: W3GS_GAMELOADED_OTHERS )
			{
				uint32_t loadingTime = GetTicks() - countdownEndTime;
				uint32_t sec = loadingTime / 1000;
				uint32_t dsec = (loadingTime / 100)%10;
				ostringstream finishedLoading;

				if( ((int) Packet->GetData()[4]) == m_ChatPID )
				{
					finishedLoading << "[Manufactoring] You have finished loading. Loading time: ";
					finishedLoading << sec;
					finishedLoading << ",";
					finishedLoading << dsec;
					finishedLoading << " seconds.";
					CONSOLE_Print( finishedLoading.str() );
				}
				else
				{
					finishedLoading << "[Manufactoring] ";
					finishedLoading << playerNames[((int) Packet->GetData()[4])];
					finishedLoading << " has finished loading. Loading time: ";
					finishedLoading << sec;
					finishedLoading << ",";
					finishedLoading << dsec;
					finishedLoading << " seconds.";
					CONSOLE_Print( finishedLoading.str() );
				}
				
				playerLoadingComplete[ ((int) Packet->GetData()[4]) ] = true;
				finishedLoadingCounter++;

				if( slotsWithPlayer == finishedLoadingCounter )
				{
					gameLoaded = true;
					if( gGProxy->m_PlaySound )
					{
						sndPlaySound(L"Sounds\\Game countdown started.wav",SND_ASYNC | SND_FILENAME);
					}
				}
				
				gChannelWindowChanged = true;
				CONSOLE_Draw();
			}
			else if( Packet->GetID( ) == CGameProtocol :: W3GS_COUNTDOWN_END )
			{
				if( m_GameIsReliable && m_ReconnectPort > 0 )
					CONSOLE_Print( "[GPROXY] game started, disconnect protection enabled" );
				else
				{
					if( m_GameIsReliable )
						CONSOLE_Print( "[GPROXY] game started but GPS handshake not complete, disconnect protection disabled" );
					else
						CONSOLE_Print( "[GPROXY] game started" );
				}

				m_GameStarted = true;
				countdownEndTime = GetTicks();

				slotsWithPlayer = 0;
				for( vector<CIncomingSlots *> :: iterator it = slots.begin( ); it != slots.end( ); it++ )
				{
					if( (*it)->GetSlotStatus() == 2 && (*it)->GetComputerStatus() == 0 )
						slotsWithPlayer++;
					else
					{
						delete *it;
						it = slots.erase(it);
						continue;
					}
				}
			}
			else if( Packet->GetID( ) == CGameProtocol :: W3GS_REJECTJOIN )
			{
				BYTEARRAY Data = Packet->GetData();

				if( Data.size() < 5 )
					return;

				uint32_t reason = UTIL_ByteArrayToUInt32( Data, false, 4 );

				switch(reason)
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
			else if( Packet->GetID( ) == CGameProtocol :: W3GS_INCOMING_ACTION )
			{
				if( m_GameIsReliable )
				{
					// we received a game update which means we can reset the number of empty actions we have to work with
					// we also must send any remaining empty actions now
					// note: the lag screen can't be up right now otherwise the server made a big mistake, so we don't need to check for it

					BYTEARRAY EmptyAction;
					EmptyAction.push_back( 0xF7 );
					EmptyAction.push_back( 0x0C );
					EmptyAction.push_back( 0x06 );
					EmptyAction.push_back( 0x00 );
					EmptyAction.push_back( 0x00 );
					EmptyAction.push_back( 0x00 );

					for( unsigned char i = m_NumEmptyActionsUsed; i < m_NumEmptyActions; i++ )
						m_LocalSocket->PutBytes( EmptyAction );

					m_NumEmptyActionsUsed = 0;
				}

				m_ActionReceived = true;
				m_LastActionTime = GetTime( );
			}
			else if( Packet->GetID( ) == CGameProtocol :: W3GS_START_LAG )
			{
				if( m_GameIsReliable )
				{
					BYTEARRAY Data = Packet->GetData( );

					if( Data.size( ) >= 5 )
					{
						unsigned char NumLaggers = Data[4];

						if( Data.size( ) == 5 + NumLaggers * 5 )
						{
							for( unsigned char i = 0; i < NumLaggers; i++ )
							{
								bool LaggerFound = false;

								for( vector<unsigned char> :: iterator j = m_Laggers.begin( ); j != m_Laggers.end( ); j++ )
								{
									if( *j == Data[5 + i * 5] )
										LaggerFound = true;
								}

								if( LaggerFound )
									CONSOLE_Print( "[GPROXY] warning - received start_lag on known lagger" );
								else
									m_Laggers.push_back( Data[5 + i * 5] );
							}
						}
						else
							CONSOLE_Print( "[GPROXY] warning - unhandled start_lag (2)" );
					}
					else
						CONSOLE_Print( "[GPROXY] warning - unhandled start_lag (1)" );
				}
			}
			else if( Packet->GetID( ) == CGameProtocol :: W3GS_STOP_LAG )
			{
				if( m_GameIsReliable )
				{
					BYTEARRAY Data = Packet->GetData( );

					if( Data.size( ) == 9 )
					{
						bool LaggerFound = false;

						for( vector<unsigned char> :: iterator i = m_Laggers.begin( ); i != m_Laggers.end( ); )
						{
							if( *i == Data[4] )
							{
								i = m_Laggers.erase( i );
								LaggerFound = true;
							}
							else
								i++;
						}

						if( !LaggerFound )
							CONSOLE_Print( "[GPROXY] warning - received stop_lag on unknown lagger" );
					}
					else
						CONSOLE_Print( "[GPROXY] warning - unhandled stop_lag" );
				}
			}
			else if( Packet->GetID( ) == CGameProtocol :: W3GS_INCOMING_ACTION2 )
			{
				if( m_GameIsReliable )
				{
					// we received a fractured game update which means we cannot use any empty actions until we receive the subsequent game update
					// we also must send any remaining empty actions now
					// note: this means if we get disconnected right now we can't use any of our buffer time, which would be very unlucky
					// it still gives us 60 seconds total to reconnect though
					// note: the lag screen can't be up right now otherwise the server made a big mistake, so we don't need to check for it

					BYTEARRAY EmptyAction;
					EmptyAction.push_back( 0xF7 );
					EmptyAction.push_back( 0x0C );
					EmptyAction.push_back( 0x06 );
					EmptyAction.push_back( 0x00 );
					EmptyAction.push_back( 0x00 );
					EmptyAction.push_back( 0x00 );

					for( unsigned char i = m_NumEmptyActionsUsed; i < m_NumEmptyActions; i++ )
						m_LocalSocket->PutBytes( EmptyAction );

					m_NumEmptyActionsUsed = m_NumEmptyActions;
				}
			}
			

			// forward the data

			m_LocalSocket->PutBytes( Packet->GetData( ) );

			// we have to wait until now to send the status message since otherwise the slotinfojoin itself wouldn't have been forwarded

			if( Packet->GetID( ) == CGameProtocol :: W3GS_SLOTINFOJOIN )
			{
				if( m_JoinedName != m_Username )
					SendLocalChat( "Using battle.net name \"" + m_Username + "\" instead of LAN name \"" + m_JoinedName + "\"." );

				if( m_GameIsReliable )
					SendLocalChat( "This is a reliable game. Requesting GProxy++ disconnect protection from server..." );
				else
					SendLocalChat( "This is an unreliable game. GProxy++ disconnect protection is disabled." );
			}
		}
		else if( Packet->GetPacketType( ) == GPS_HEADER_CONSTANT )
		{
			if( m_GameIsReliable )
			{
				BYTEARRAY Data = Packet->GetData( );

				if( Packet->GetID( ) == CGPSProtocol :: GPS_INIT && Data.size( ) == 12 )
				{
					m_ReconnectPort = UTIL_ByteArrayToUInt16( Data, false, 4 );
					m_PID = Data[6];
					m_ReconnectKey = UTIL_ByteArrayToUInt32( Data, false, 7 );
					m_NumEmptyActions = Data[11];
					SendLocalChat( "GProxy++ disconnect protection is ready (" + UTIL_ToString( ( m_NumEmptyActions + 1 ) * 60 ) + " second buffer)." );
					CONSOLE_Print( "[GPROXY] handshake complete, disconnect protection ready (" + UTIL_ToString( ( m_NumEmptyActions + 1 ) * 60 ) + " second buffer)" );
				}
				else if( Packet->GetID( ) == CGPSProtocol :: GPS_RECONNECT && Data.size( ) == 8 )
				{
					uint32_t LastPacket = UTIL_ByteArrayToUInt32( Data, false, 4 );
					uint32_t PacketsAlreadyUnqueued = m_TotalPacketsReceivedFromLocal - m_PacketBuffer.size( );

					if( LastPacket > PacketsAlreadyUnqueued )
					{
						uint32_t PacketsToUnqueue = LastPacket - PacketsAlreadyUnqueued;

						if( PacketsToUnqueue > m_PacketBuffer.size( ) )
						{
							CONSOLE_Print( "[GPROXY] received GPS_RECONNECT with last packet > total packets sent" );
							PacketsToUnqueue = m_PacketBuffer.size( );
						}

						while( PacketsToUnqueue > 0 )
						{
							delete m_PacketBuffer.front( );
							m_PacketBuffer.pop( );
							PacketsToUnqueue--;
						}
					}

					// send remaining packets from buffer, preserve buffer
					// note: any packets in m_LocalPackets are still sitting at the end of this buffer because they haven't been processed yet
					// therefore we must check for duplicates otherwise we might (will) cause a desync

					queue<CCommandPacket *> TempBuffer;

					while( !m_PacketBuffer.empty( ) )
					{
						if( m_PacketBuffer.size( ) > m_LocalPackets.size( ) )
							m_RemoteSocket->PutBytes( m_PacketBuffer.front( )->GetData( ) );

						TempBuffer.push( m_PacketBuffer.front( ) );
						m_PacketBuffer.pop( );
					}

					m_PacketBuffer = TempBuffer;

					// we can resume forwarding local packets again
					// doing so prior to this point could result in an out-of-order stream which would probably cause a desync

					m_Synchronized = true;
				}
				else if( Packet->GetID( ) == CGPSProtocol :: GPS_ACK && Data.size( ) == 8 )
				{
					uint32_t LastPacket = UTIL_ByteArrayToUInt32( Data, false, 4 );
					uint32_t PacketsAlreadyUnqueued = m_TotalPacketsReceivedFromLocal - m_PacketBuffer.size( );

					if( LastPacket > PacketsAlreadyUnqueued )
					{
						uint32_t PacketsToUnqueue = LastPacket - PacketsAlreadyUnqueued;

						if( PacketsToUnqueue > m_PacketBuffer.size( ) )
						{
							CONSOLE_Print( "[GPROXY] received GPS_ACK with last packet > total packets sent" );
							PacketsToUnqueue = m_PacketBuffer.size( );
						}

						while( PacketsToUnqueue > 0 )
						{
							delete m_PacketBuffer.front( );
							m_PacketBuffer.pop( );
							PacketsToUnqueue--;
						}
					}
				}
				else if( Packet->GetID( ) == CGPSProtocol :: GPS_REJECT && Data.size( ) == 8 )
				{
					uint32_t Reason = UTIL_ByteArrayToUInt32( Data, false, 4 );

					if( Reason == REJECTGPS_INVALID )
						CONSOLE_Print( "[GPROXY] rejected by remote server: invalid data" );
					else if( Reason == REJECTGPS_NOTFOUND )
						CONSOLE_Print( "[GPROXY] rejected by remote server: player not found in any running games" );

					m_LocalSocket->Disconnect( );
				}
					
			}
			
			else if (testvar)
				{
					stringstream ss;
					string s;
					ss << Packet->GetID( );
					ss >> s;
					CONSOLE_Print( s );
		

				}
		}

		delete Packet;
	}
}

bool CGProxy :: AddGame( CIncomingGameHost *game )
{
	// check for duplicates and rehosted games

	bool DuplicateFound = false;
	uint32_t OldestReceivedTime = GetTime( );

	for( vector<CIncomingGameHost *> :: iterator i = m_Games.begin( ); i != m_Games.end( ); i++ )
	{
		if( game->GetIP( ) == (*i)->GetIP( ) && game->GetPort( ) == (*i)->GetPort( ) )
		{
			// duplicate or rehosted game, delete the old one and add the new one
			// don't forget to remove the old one from the LAN list first

			m_UDPSocket->Broadcast( 6112, m_GameProtocol->SEND_W3GS_DECREATEGAME( (*i)->GetUniqueGameID( ) ) );
			delete *i;
			*i = game;
			DuplicateFound = true;
			break;
		}

		if( game->GetGameName( ) != m_BNET->GetSearchGameName(  ) && game->GetReceivedTime( ) < OldestReceivedTime )
			OldestReceivedTime = game->GetReceivedTime( );
	}

	if( !DuplicateFound )
	{
		m_Games.push_back( game );
		gGProxy->autosearch = true; //pr0 autosearch
	}

	// the game list cannot hold more than 20 games (warcraft 3 doesn't handle it properly and ignores any further games)
	// if this game puts us over the limit, remove the oldest game
	// don't remove the "search game" since that's probably a pretty important game
	// note: it'll get removed automatically by the 60 second timeout in the main loop when appropriate

	if( m_Games.size( ) > 20 )
	{
		for( vector<CIncomingGameHost *> :: iterator i = m_Games.begin( ); i != m_Games.end( ); i++ )
		{
			if( game->GetGameName( ) != m_BNET->GetSearchGameName(  ) && game->GetReceivedTime( ) == OldestReceivedTime )
			{
				m_UDPSocket->Broadcast( 6112, m_GameProtocol->SEND_W3GS_DECREATEGAME( (*i)->GetUniqueGameID( ) ) );
				delete *i;
				m_Games.erase( i );
				break;
			}
		}
	}

	return !DuplicateFound;
}

void CGProxy :: SendAllMessage( string message )
{
	if( message.empty() )
		return;

	if( !gGProxy->m_BNET->GetInGame() )
	{
		CONSOLE_Print( "[Manufactoring][ERROR] Cannot send \"AllMessage\". You need to be ingame." );
		return;
	}

	if( gGProxy->m_GameStarted && !gameLoaded )
	{
		CONSOLE_Print( "[Manufactoring][ERROR] Cannot send \"AllMessage\". Game is loading." );
	}
	else if( !gGProxy->m_GameStarted )
	{
		CONSOLE_Print( "[Manufactoring][ERROR] Cannot send \"AllMessage\". You are currently in the lobby." );
	}
	else
	{
		if( message.size( ) > 254 )
				message = message.substr( 0, 254 );

		BYTEARRAY toPIDs;

		for( vector<CIncomingSlots *> :: iterator it = slots.begin(); it != slots.end(); it++ )
		{
			if( (*it)->GetSlotStatus() == 2 && (*it)->GetComputerStatus() == 0 )
				toPIDs.push_back( (*it)->GetPID() );
		}

		BYTEARRAY extraFlags;
		extraFlags.push_back(0);
		extraFlags.push_back(0);
		extraFlags.push_back(0);
		extraFlags.push_back(0);

		BYTEARRAY Packet = gGProxy->m_GameProtocol->SEND_W3GS_CHAT_TO_HOST( gGProxy->m_ChatPID, toPIDs, 32, extraFlags, message );
		
		if( CheckForwarding( message ) )
		{
			m_PacketBuffer.push( new CCommandPacket( W3GS_HEADER_CONSTANT, Packet[1], Packet ) );
			m_LocalPackets.push( new CCommandPacket( W3GS_HEADER_CONSTANT, Packet[1], Packet ) );
			m_TotalPacketsReceivedFromLocal++;
		}
	}
}

void CGProxy :: SendAllyMessage( string message )
{
	if( message.empty() )
		return;

	if( !gGProxy->m_BNET->GetInGame() )
	{
		CONSOLE_Print( "[Manufactoring][ERROR] Cannot send \"AllMessage\". You need to be ingame." );
		return;
	}

	if(false && gGProxy->m_GameStarted && !gameLoaded )//phy deactivated
	{
		CONSOLE_Print( "[Manufactoring][ERROR] Cannot send \"AllMessage\". Game is loading." );
	}
	else if( !gGProxy->m_GameStarted )
	{
		CONSOLE_Print( "[Manufactoring][ERROR] Cannot send \"AllMessage\". You are currently in the lobby." );
	}
	else
	{
		if( message.size( ) > 254 )
				message = message.substr( 0, 254 );

		BYTEARRAY toPIDs;

		for( vector<CIncomingSlots *> :: iterator it = slots.begin(); it != slots.end(); it++ )
		{
			if( (*it)->GetSlotStatus() == 2 && (*it)->GetComputerStatus() == 0 && (*it)->GetTeam() == teamNumber )
				toPIDs.push_back( (*it)->GetPID() );
		}

		BYTEARRAY extraFlags;
		extraFlags.push_back(1);
		extraFlags.push_back(0);
		extraFlags.push_back(0);
		extraFlags.push_back(0);

		BYTEARRAY Packet = gGProxy->m_GameProtocol->SEND_W3GS_CHAT_TO_HOST( gGProxy->m_ChatPID, toPIDs, 32, extraFlags, message );
		
		if( CheckForwarding( message ) )
		{
			m_PacketBuffer.push( new CCommandPacket( W3GS_HEADER_CONSTANT, Packet[1], Packet ) );
			m_LocalPackets.push( new CCommandPacket( W3GS_HEADER_CONSTANT, Packet[1], Packet ) );
			m_TotalPacketsReceivedFromLocal++;
		}
	}
}

void CGProxy :: SendLobbyMessage( string message )
{
	if( message.empty() )
		return;

	if( !gGProxy->m_BNET->GetInGame() )
	{
		CONSOLE_Print( "[Manufactoring][ERROR] Cannot send lobby message. You need to be in a lobby." );
		return;
	}
	else if( gGProxy->m_GameStarted )
	{
		CONSOLE_Print( "[Manufactoring][ERROR] Cannot send lobby message. The game has already started." );
		return;
	}
	else
	{
		if( message.size( ) > 254 )
				message = message.substr( 0, 254 );

		BYTEARRAY toPIDs;

		for( vector<CIncomingSlots *> :: iterator it = slots.begin(); it != slots.end(); it++ )
		{
			if( (*it)->GetSlotStatus() == 2 && (*it)->GetComputerStatus() == 0 )
				toPIDs.push_back( (*it)->GetPID() );
		}

		BYTEARRAY Packet = gGProxy->m_GameProtocol->SEND_W3GS_CHAT_TO_HOST( gGProxy->m_ChatPID, toPIDs, 16, BYTEARRAY(), message );
		
		if( CheckForwarding( message ) )
		{
			m_PacketBuffer.push( new CCommandPacket( W3GS_HEADER_CONSTANT, Packet[1], Packet ) );
			m_LocalPackets.push( new CCommandPacket( W3GS_HEADER_CONSTANT, Packet[1], Packet ) );
			m_TotalPacketsReceivedFromLocal++;
		}
	}
}

void CGProxy :: SendChangeTeam( unsigned char team )
{
	BYTEARRAY toPIDs;

	for( vector<CIncomingSlots *> :: iterator it = slots.begin(); it != slots.end(); it++ )
	{
		if( (*it)->GetSlotStatus() == 2 && (*it)->GetComputerStatus() == 0 )
			toPIDs.push_back( (*it)->GetPID() );
	}

	BYTEARRAY Packet = gGProxy->m_GameProtocol->SEND_W3GS_TEAMCHANGE( gGProxy->m_ChatPID, toPIDs, team );
	
	m_PacketBuffer.push( new CCommandPacket( W3GS_HEADER_CONSTANT, Packet[1], Packet ) );
	m_LocalPackets.push( new CCommandPacket( W3GS_HEADER_CONSTANT, Packet[1], Packet ) );
	m_TotalPacketsReceivedFromLocal++;
}

void CGProxy :: SendLocalChat( string message )
{
	if( m_LocalSocket )
	{
		if( m_GameStarted )
		{
			if( message.size( ) > 127 )
				message = message.substr( 0, 127 );

			m_LocalSocket->PutBytes( m_GameProtocol->SEND_W3GS_CHAT_FROM_HOST( m_ChatPID, UTIL_CreateByteArray( m_ChatPID ), 32, UTIL_CreateByteArray( (uint32_t)0, false ), message ) );
		}
		else
		{
			if( message.size( ) > 254 )
				message = message.substr( 0, 254 );

			m_LocalSocket->PutBytes( m_GameProtocol->SEND_W3GS_CHAT_FROM_HOST( m_ChatPID, UTIL_CreateByteArray( m_ChatPID ), 16, BYTEARRAY( ), message ) );
		}
	}
}

void CGProxy :: SendEmptyAction( )
{
	// we can't send any empty actions while the lag screen is up
	// so we keep track of who the lag screen is currently showing (if anyone) and we tear it down, send the empty action, and put it back up

	for( vector<unsigned char> :: iterator i = m_Laggers.begin( ); i != m_Laggers.end( ); i++ )
	{
		BYTEARRAY StopLag;
		StopLag.push_back( 0xF7 );
		StopLag.push_back( 0x11 );
		StopLag.push_back( 0x09 );
		StopLag.push_back( 0 );
		StopLag.push_back( *i );
		UTIL_AppendByteArray( StopLag, (uint32_t)60000, false );
		m_LocalSocket->PutBytes( StopLag );
	}

	BYTEARRAY EmptyAction;
	EmptyAction.push_back( 0xF7 );
	EmptyAction.push_back( 0x0C );
	EmptyAction.push_back( 0x06 );
	EmptyAction.push_back( 0x00 );
	EmptyAction.push_back( 0x00 );
	EmptyAction.push_back( 0x00 );
	m_LocalSocket->PutBytes( EmptyAction );

	if( !m_Laggers.empty( ) )
	{
		BYTEARRAY StartLag;
		StartLag.push_back( 0xF7 );
		StartLag.push_back( 0x10 );
		StartLag.push_back( 0 );
		StartLag.push_back( 0 );
		StartLag.push_back( m_Laggers.size( ) );

		for( vector<unsigned char> :: iterator i = m_Laggers.begin( ); i != m_Laggers.end( ); i++ )
		{
			// using a lag time of 60000 ms means the counter will start at zero
			// hopefully warcraft 3 doesn't care about wild variations in the lag time in subsequent packets

			StartLag.push_back( *i );
			UTIL_AppendByteArray( StartLag, (uint32_t)60000, false );
		}

		BYTEARRAY LengthBytes;
		LengthBytes = UTIL_CreateByteArray( (uint16_t)StartLag.size( ), false );
		StartLag[2] = LengthBytes[0];
		StartLag[3] = LengthBytes[1];
		m_LocalSocket->PutBytes( StartLag );
	}
}
bool fcfgfilterfirst()//phy filter
{
return gGProxy->cfgfilterfirst;

}
string fcfgfilter()//phy filter
{
return gGProxy->cfgfilter;
}
string parrot()//phy parrot
{
	return gGProxy->parrot;
}
void saychat(string message)//phy parrot
{
	gGProxy->m_BNET->QueueChatCommand(message);
}
bool getautosearch() //pr0 autosearch
{
	return gGProxy->autosearch;
}

void autosearch(bool autosearchNew) //pr0 autosearch
{
	gGProxy->autosearch = autosearchNew;
}

bool cautosearch() //pr0 cautosearch
{
	return gGProxy->cautosearch;
}

bool displayautocreated()
{
	return gGProxy->displayautocreated;
}

void displayautocreated(bool newone)
{
	gGProxy->displayautocreated=newone;
}
void flisting_current_games(bool newone)
{
	gGProxy->m_listing_current_games = newone;
}
bool flisting_current_games()
{
	return gGProxy->m_listing_current_games;
}

CIncomingSlots :: CIncomingSlots(unsigned char nPID, unsigned char ndownloadStatus, unsigned char nslotStatus, 
								 unsigned char ncomputerStatus, unsigned char nteam, unsigned char ncolor, 
								 unsigned char nrace, unsigned char ncomputerType, unsigned char nhandicap)
{
	PID =				(int) nPID;
	downloadStatus =	(int) ndownloadStatus;
	slotStatus =		(int) nslotStatus;
	computerStatus =	(int) ncomputerStatus;
	team =				(int) nteam;
	color =				(int) ncolor;
	race =				(int) nrace;
	computerType =		(int) ncomputerType;
	handicap =			(int) nhandicap;
}

CIncomingSlots :: ~CIncomingSlots( )
{
}