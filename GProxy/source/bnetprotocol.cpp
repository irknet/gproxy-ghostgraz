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

#include "gproxy.h"
#include "util/Util.h"
#include "bnetprotocol.h"
#include "bnet.h"

#include <QTextCodec>


uint32_t CIncomingChatEvent::GetPingPhy()
{
	return m_Ping;
}

CBNETProtocol :: CBNETProtocol( )
{
	unsigned char ClientToken[] = { 220, 1, 203, 7 };
	m_ClientToken = Util::createByteArray( ClientToken, 4 );
}

CBNETProtocol :: ~CBNETProtocol( )
{

}

///////////////////////
// RECEIVE FUNCTIONS //
///////////////////////

bool CBNETProtocol :: RECEIVE_SID_NULL( BYTEARRAY data )
{
	// DEBUG_Print( "RECEIVED SID_NULL" );
	// DEBUG_Print( data );

	// 2 bytes					-> Header
	// 2 bytes					-> Length

	return ValidateLength( data );
}

vector<CIncomingGameHost *> CBNETProtocol :: RECEIVE_SID_GETADVLISTEX( BYTEARRAY data )
{
	// DEBUG_Print( "RECEIVED SID_GETADVLISTEX" );
	// DEBUG_Print( data );

	// 2 bytes					-> Header
	// 2 bytes					-> Length
	// 4 bytes					-> GamesFound
	// for( 1 .. GamesFound )
	//		2 bytes				-> GameType
	//		2 bytes				-> Parameter
	//		4 bytes				-> Language ID
	//		2 bytes				-> AF_INET
	//		2 bytes				-> Port
	//		4 bytes				-> IP
	//		4 bytes				-> zeros
	//		4 bytes				-> zeros
	//		4 bytes				-> Status
	//		4 bytes				-> ElapsedTime
	//		null term string	-> GameName
	//		1 byte				-> GamePassword
	//		1 byte				-> SlotsTotal
	//		8 bytes				-> HostCounter (ascii hex format)
	//		null term string	-> StatString

	vector<CIncomingGameHost *> Games;

	if( ValidateLength( data ) && data.size( ) >= 8 )
	{
		unsigned int i = 8;
		uint32_t GamesFound = Util::byteArrayToUInt32( data, false, 4 );

		while( GamesFound > 0 )
		{
			GamesFound--;

			if( data.size( ) < i + 33 )
				break;

			uint16_t GameType = Util::byteArrayToUInt16( data, false, i );
			i += 2;
			uint16_t Parameter = Util::byteArrayToUInt16( data, false, i );
			i += 2;
			uint32_t LanguageID = Util::byteArrayToUInt32( data, false, i );
			i += 4;
			// AF_INET
			i += 2;
			uint16_t Port = Util::byteArrayToUInt16( data, true, i );
			i += 2;
			BYTEARRAY IP = BYTEARRAY( data.begin( ) + i, data.begin( ) + i + 4 );
			i += 4;
			// zeros
			i += 4;
			// zeros
			i += 4;
			uint32_t Status = Util::byteArrayToUInt32( data, false, i );
			i += 4;
			uint32_t ElapsedTime = Util::byteArrayToUInt32( data, false, i );
			i += 4;
			BYTEARRAY GameName = Util::extractNTCString( data, i );
			i += GameName.size( ) + 1;

			if( data.size( ) < i + 1 )
				break;

			BYTEARRAY GamePassword = Util::extractNTCString( data, i );
			i += GamePassword.size( ) + 1;

			if( data.size( ) < i + 10 )
				break;

			// SlotsTotal is in ascii hex format

			unsigned char SlotsTotal = data[i];
			unsigned int c;
			stringstream SS;
			SS << string( 1, SlotsTotal );
			SS >> hex >> c;
			SlotsTotal = c;
			i++;

			// HostCounter is in reverse ascii hex format
			// e.g. 1  is "10000000"
			// e.g. 10 is "a0000000"
			// extract it, reverse it, parse it, construct a single uint32_t

			BYTEARRAY HostCounterRaw = BYTEARRAY( data.begin( ) + i, data.begin( ) + i + 8 );
			string HostCounterString = string( HostCounterRaw.rbegin( ), HostCounterRaw.rend( ) );
			uint32_t HostCounter = 0;

			for( int j = 0; j < 4; j++ )
			{
				unsigned int c;
				stringstream SS;
				SS << HostCounterString.substr( j * 2, 2 );
				SS >> hex >> c;
				HostCounter |= c << ( 24 - j * 8 );
			}

			i += 8;
			BYTEARRAY StatString = Util::extractNTCString( data, i );
			i += StatString.size( ) + 1;

			Games.push_back( new CIncomingGameHost( GameType, Parameter, LanguageID, Port, IP, Status, ElapsedTime, string( GameName.begin( ), GameName.end( ) ), SlotsTotal, HostCounter, StatString ) );
		}
	}

	return Games;
}

bool CBNETProtocol :: RECEIVE_SID_ENTERCHAT( BYTEARRAY data )
{
	// DEBUG_Print( "RECEIVED SID_ENTERCHAT" );
	// DEBUG_Print( data );

	// 2 bytes					-> Header
	// 2 bytes					-> Length
	// null terminated string	-> UniqueName

	if( ValidateLength( data ) && data.size( ) >= 5 )
	{
		m_UniqueName = Util::extractNTCString( data, 4 );
		return true;
	}

	return false;
}

CIncomingChatEvent *CBNETProtocol :: RECEIVE_SID_CHATEVENT( BYTEARRAY data )
{
	// DEBUG_Print( "RECEIVED SID_CHATEVENT" );
	// DEBUG_Print( data );

	// 2 bytes					-> Header
	// 2 bytes					-> Length
	// 4 bytes					-> EventID
	// 4 bytes					-> ???
	// 4 bytes					-> Ping
	// 12 bytes					-> ???
	// null terminated string	-> User
	// null terminated string	-> Message

	if( ValidateLength( data ) && data.size( ) >= 29 )
	{
		BYTEARRAY EventID = BYTEARRAY( data.begin( ) + 4, data.begin( ) + 8 );
		BYTEARRAY Ping = BYTEARRAY( data.begin( ) + 12, data.begin( ) + 16 );
		BYTEARRAY User = Util::extractNTCString( data, 28 );
                QString message = Util::extractNTString( data, User.size( ) + 29 );

		switch( Util::byteArrayToUInt32( EventID, false ) )
		{
		case CBNETProtocol :: EID_SHOWUSER:
		case CBNETProtocol :: EID_JOIN:
		case CBNETProtocol :: EID_LEAVE:
		case CBNETProtocol :: EID_WHISPER:
		case CBNETProtocol :: EID_TALK:
		case CBNETProtocol :: EID_BROADCAST:
		case CBNETProtocol :: EID_CHANNEL:
		case CBNETProtocol :: EID_USERFLAGS:
		case CBNETProtocol :: EID_WHISPERSENT:
		case CBNETProtocol :: EID_CHANNELFULL:
		case CBNETProtocol :: EID_CHANNELDOESNOTEXIST:
		case CBNETProtocol :: EID_CHANNELRESTRICTED:
		case CBNETProtocol :: EID_INFO:
		case CBNETProtocol :: EID_ERROR:
		case CBNETProtocol :: EID_EMOTE:
			return new CIncomingChatEvent(	(CBNETProtocol :: IncomingChatEvent)Util::byteArrayToUInt32( EventID, false ),
												Util::byteArrayToUInt32( Ping, false ),
												string( User.begin( ), User.end( ) ),
												message );
		}

	}

	return NULL;
}

bool CBNETProtocol :: RECEIVE_SID_CHECKAD( BYTEARRAY data )
{
	// DEBUG_Print( "RECEIVED SID_CHECKAD" );
	// DEBUG_Print( data );

	// 2 bytes					-> Header
	// 2 bytes					-> Length

	return ValidateLength( data );
}

bool CBNETProtocol :: RECEIVE_SID_STARTADVEX3( BYTEARRAY data )
{
	// DEBUG_Print( "RECEIVED SID_STARTADVEX3" );
	// DEBUG_Print( data );

	// 2 bytes					-> Header
	// 2 bytes					-> Length
	// 4 bytes					-> Status

	if( ValidateLength( data ) && data.size( ) >= 8 )
	{
		BYTEARRAY Status = BYTEARRAY( data.begin( ) + 4, data.begin( ) + 8 );

		if( Util::byteArrayToUInt32( Status, false ) == 0 )
			return true;
	}

	return false;
}

BYTEARRAY CBNETProtocol :: RECEIVE_SID_PING( BYTEARRAY data )
{
	// DEBUG_Print( "RECEIVED SID_PING" );
	// DEBUG_Print( data );

	// 2 bytes					-> Header
	// 2 bytes					-> Length
	// 4 bytes					-> Ping

	if( ValidateLength( data ) && data.size( ) >= 8 )
		return BYTEARRAY( data.begin( ) + 4, data.begin( ) + 8 );

	return BYTEARRAY( );
}

bool CBNETProtocol :: RECEIVE_SID_LOGONRESPONSE( BYTEARRAY data )
{
	// DEBUG_Print( "RECEIVED SID_LOGONRESPONSE" );
	// DEBUG_Print( data );

	// 2 bytes					-> Header
	// 2 bytes					-> Length
	// 4 bytes					-> Status

	if( ValidateLength( data ) && data.size( ) >= 8 )
	{
		BYTEARRAY Status = BYTEARRAY( data.begin( ) + 4, data.begin( ) + 8 );

		if( Util::byteArrayToUInt32( Status, false ) == 1 )
			return true;
	}

	return false;
}

bool CBNETProtocol :: RECEIVE_SID_AUTH_INFO( BYTEARRAY data )
{
	// DEBUG_Print( "RECEIVED SID_AUTH_INFO" );
	// DEBUG_Print( data );

	// 2 bytes					-> Header
	// 2 bytes					-> Length
	// 4 bytes					-> LogonType
	// 4 bytes					-> ServerToken
	// 4 bytes					-> ???
	// 8 bytes					-> MPQFileTime
	// null terminated string	-> IX86VerFileName
	// null terminated string	-> ValueStringFormula

	if( ValidateLength( data ) && data.size( ) >= 25 )
	{
		m_LogonType = BYTEARRAY( data.begin( ) + 4, data.begin( ) + 8 );
		m_ServerToken = BYTEARRAY( data.begin( ) + 8, data.begin( ) + 12 );
		m_MPQFileTime = BYTEARRAY( data.begin( ) + 16, data.begin( ) + 24 );
		m_IX86VerFileName = Util::extractNTCString( data, 24 );
		m_ValueStringFormula = Util::extractNTCString( data, m_IX86VerFileName.size( ) + 25 );
		return true;
	}

	return false;
}

bool CBNETProtocol :: RECEIVE_SID_AUTH_CHECK( BYTEARRAY data )
{
	// DEBUG_Print( "RECEIVED SID_AUTH_CHECK" );
	// DEBUG_Print( data );

	// 2 bytes					-> Header
	// 2 bytes					-> Length
	// 4 bytes					-> KeyState
	// null terminated string	-> KeyStateDescription

	if( ValidateLength( data ) && data.size( ) >= 9 )
	{
		m_KeyState = BYTEARRAY( data.begin( ) + 4, data.begin( ) + 8 );
		m_KeyStateDescription = Util::extractNTCString( data, 8 );

		if( Util::byteArrayToUInt32( m_KeyState, false ) == KR_GOOD )
			return true;
	}

	return false;
}

bool CBNETProtocol :: RECEIVE_SID_AUTH_ACCOUNTLOGON( BYTEARRAY data )
{
	// DEBUG_Print( "RECEIVED SID_AUTH_ACCOUNTLOGON" );
	// DEBUG_Print( data );

	// 2 bytes					-> Header
	// 2 bytes					-> Length
	// 4 bytes					-> Status
	// if( Status == 0 )
	//		32 bytes			-> Salt
	//		32 bytes			-> ServerPublicKey

	if( ValidateLength( data ) && data.size( ) >= 8 )
	{
		BYTEARRAY status = BYTEARRAY( data.begin( ) + 4, data.begin( ) + 8 );

		if( Util::byteArrayToUInt32( status, false ) == 0 && data.size( ) >= 72 )
		{
			m_Salt = BYTEARRAY( data.begin( ) + 8, data.begin( ) + 40 );
			m_ServerPublicKey = BYTEARRAY( data.begin( ) + 40, data.begin( ) + 72 );
			return true;
		}
	}

	return false;
}

bool CBNETProtocol :: RECEIVE_SID_AUTH_ACCOUNTLOGONPROOF( BYTEARRAY data )
{
	// DEBUG_Print( "RECEIVED SID_AUTH_ACCOUNTLOGONPROOF" );
	// DEBUG_Print( data );

	// 2 bytes					-> Header
	// 2 bytes					-> Length
	// 4 bytes					-> Status

	if( ValidateLength( data ) && data.size( ) >= 8 )
	{
		BYTEARRAY Status = BYTEARRAY( data.begin( ) + 4, data.begin( ) + 8 );

		if( Util::byteArrayToUInt32( Status, false ) == 0 )
			return true;
	}

	return false;
}

BYTEARRAY CBNETProtocol :: RECEIVE_SID_WARDEN( BYTEARRAY data )
{
	// DEBUG_Print( "RECEIVED SID_WARDEN" );
	// DEBUG_PRINT( data );

	// 2 bytes					-> Header
	// 2 bytes					-> Length
	// n bytes					-> Data

	if( ValidateLength( data ) && data.size( ) >= 4 )
		return BYTEARRAY( data.begin( ) + 4, data.end( ) );

	return BYTEARRAY( );
}

QList<Friend*> CBNETProtocol :: RECEIVE_SID_FRIENDSLIST( BYTEARRAY data )
{
    // DEBUG_Print( "RECEIVED SID_FRIENDSLIST" );
    // DEBUG_Print( data );

    QList<Friend*> friends;

    if(ValidateLength(data) && data.size() >= 5)
    {
        unsigned char numberOfEntries = data[4];
        unsigned int i = 5;

        for (unsigned char entryNumber = 0; entryNumber != numberOfEntries; ++entryNumber)
        {
            QString name = Util::extractNTString(data, i, i);
            unsigned char location = data[i++];
            unsigned char status = data[i++];
            QString productCode = Util::revertString(Util::extractString(data, i, 4));
            i += 4;
            QString locationName = Util::extractNTString(data, i, i);

            Friend* f = new Friend();
            f->setEntryNumber(entryNumber);
            f->setName(name);
            f->setLocation(location);
            f->setStatus(status);
            f->setProduct(Product(productCode));
            f->setLocationName(locationName);
            friends.append(f);
        }
    }

    return friends;
}

Friend* CBNETProtocol::RECEIVE_SID_FRIENDSUPDATE(BYTEARRAY data)
{
    if(ValidateLength(data) && data.size() >= 4)
    {
        unsigned char entryNumber = data[4];
        unsigned char location = data[5];
        unsigned char status = data[6];
        QString productCode = Util::revertString(Util::extractString(data, 7, 4));
        QString locationName = Util::extractNTString(data, 11);

        Friend* f = new Friend();
        f->setEntryNumber(entryNumber);
        f->setLocation(location);
        f->setStatus(status);

        /*
         * COPY FROM BNET DOCS:
         * http://www.bnetdocs.org/?op=packet&pid=384
         *
         * Note that there is a Battle.net server bug in which
         * when you are automatically sent this packet,
         * the Product ID is your own Product ID instead of your friend's.
         * So if you were to be using WAR3, for example, and a friend signs on using SEXP,
         * the Product ID in this packet will be WAR3.
         *
         * To receive the *correct* Product ID for the user, you may request another update
         * for this user (by sending 0x66 back to the server with the same Entry Number),
         * or request the full list again.
         */
        f->setProduct(Product(productCode));
        f->setLocationName(locationName);
        return f;
    }

    return NULL;
}

Friend* CBNETProtocol::RECEIVE_SID_FRIENDSADD(BYTEARRAY data)
{
    if(ValidateLength(data) && data.size() >= 4)
    {
        unsigned int i = 4;
        QString name = Util::extractNTString(data, i, i);
        unsigned char status = data[i++];
        unsigned char location = data[i++];
        QString productCode = Util::revertString(Util::extractString(data, i, 4));
        i += 4;
        QString locationName = Util::extractNTString(data, i);

        Friend* f = new Friend();
        f->setName(name);
        f->setStatus(status);
        f->setLocation(location);
        f->setProduct(Product(productCode));
        f->setLocationName(locationName);
        return f;
    }

    return NULL;
}

unsigned char CBNETProtocol::RECEIVE_SID_FRIENDSREMOVE(BYTEARRAY data)
{
    if(ValidateLength(data) && data.size() == 5)
    {
        return data[4];
    }

    return -1;
}

BYTEARRAY CBNETProtocol::RECEIVE_SID_MESSAGEBOX (BYTEARRAY data)
{
    if (ValidateLength(data) && data.size() >= 5)
    {
        return BYTEARRAY( data.begin( ) + 4, data.end( ) );
    }

    return BYTEARRAY();
}

////////////////////
// SEND FUNCTIONS //
////////////////////

BYTEARRAY CBNETProtocol :: SEND_PROTOCOL_INITIALIZE_SELECTOR( )
{
	BYTEARRAY packet;
	packet.push_back( 1 );
	// DEBUG_Print( "SENT PROTOCOL_INITIALIZE_SELECTOR" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CBNETProtocol :: SEND_SID_NULL( )
{
	BYTEARRAY packet;
	packet.push_back( BNET_HEADER_CONSTANT );	// BNET header constant
	packet.push_back( SID_NULL );				// SID_NULL
	packet.push_back( 0 );						// packet length will be assigned later
	packet.push_back( 0 );						// packet length will be assigned later
	AssignLength( packet );
	// DEBUG_Print( "SENT SID_NULL" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CBNETProtocol :: SEND_SID_STOPADV( )
{
	BYTEARRAY packet;
	packet.push_back( BNET_HEADER_CONSTANT );	// BNET header constant
	packet.push_back( SID_STOPADV );			// SID_STOPADV
	packet.push_back( 0 );						// packet length will be assigned later
	packet.push_back( 0 );						// packet length will be assigned later
	AssignLength( packet );
	// DEBUG_Print( "SENT SID_STOPADV" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CBNETProtocol :: SEND_SID_GETADVLISTEX( string gameName, uint32_t numGames )
{
	unsigned char Condition1[]	= {   0,   0 };
	unsigned char Condition2[]	= {   0,   0 };
	unsigned char Condition3[]	= {   0,   0,   0,   0 };
	unsigned char Condition4[]	= {   0,   0,   0,   0 };

	if( gameName.empty( ) )
	{
		Condition1[0] = 0;		Condition1[1] = 224;
		Condition2[0] = 127;	Condition2[1] = 0;
	}
	else
	{
		Condition1[0] = 255;	Condition1[1] = 3;
		Condition2[0] = 0;		Condition2[1] = 0;
		Condition3[0] = 255;	Condition3[1] = 3;		Condition3[2] = 0;		Condition3[3] = 0;
		numGames = 1;
	}

	BYTEARRAY packet;
	packet.push_back( BNET_HEADER_CONSTANT );			// BNET header constant
	packet.push_back( SID_GETADVLISTEX );				// SID_GETADVLISTEX
	packet.push_back( 0 );								// packet length will be assigned later
	packet.push_back( 0 );								// packet length will be assigned later
	Util::appendByteArray( packet, Condition1, 2 );
	Util::appendByteArray( packet, Condition2, 2 );
	Util::appendByteArray( packet, Condition3, 4 );
	Util::appendByteArray( packet, Condition4, 4 );
	Util::appendByteArray( packet, numGames, false );	// maximum number of games to list
	Util::appendByteArrayFast( packet, gameName );		// Game Name
	packet.push_back( 0 );								// Game Password is NULL
	packet.push_back( 0 );								// Game Stats is NULL
	AssignLength( packet );
	// DEBUG_Print( "SENT SID_GETADVLISTEX" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CBNETProtocol :: SEND_SID_ENTERCHAT( )
{
	BYTEARRAY packet;
	packet.push_back( BNET_HEADER_CONSTANT );	// BNET header constant
	packet.push_back( SID_ENTERCHAT );			// SID_ENTERCHAT
	packet.push_back( 0 );						// packet length will be assigned later
	packet.push_back( 0 );						// packet length will be assigned later
	packet.push_back( 0 );						// Account Name is NULL on Warcraft III/The Frozen Throne
	packet.push_back( 0 );						// Stat String is NULL on CDKEY'd products
	AssignLength( packet );
	// DEBUG_Print( "SENT SID_ENTERCHAT" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CBNETProtocol :: SEND_SID_JOINCHANNEL(CBNETProtocol::JoinChannelFlags flag, string channel)
{
    BYTEARRAY packet;
    packet.push_back( BNET_HEADER_CONSTANT );       // BNET header constant
    packet.push_back( SID_JOINCHANNEL );            // SID_JOINCHANNEL
    packet.push_back( 0 );                          // packet length will be assigned later
    packet.push_back( 0 );                          // packet length will be assigned later

    unsigned char flags[4];
    flags[1] = 0;
    flags[2] = 0;
    flags[3] = 0;
    switch (flag)
    {
        case CBNETProtocol::JCF_FIRST_JOIN:
            flags[0] = 1;
            break;
        case CBNETProtocol::JCF_FORCED_JOIN:
            flags[0] = 2;
            break;
        case CBNETProtocol::JCF_NOCREATE_JOIN:
            // Do the default action.
        default:
            flags[0] = 0;
    }
    Util::appendByteArray( packet, flags, 4 );

    Util::appendByteArrayFast( packet, channel );
    AssignLength( packet );
    // DEBUG_Print( "SENT SID_JOINCHANNEL" );
    // DEBUG_Print( packet );
    return packet;
}

BYTEARRAY CBNETProtocol::SEND_SID_CHATCOMMAND (QString command)
{
    BYTEARRAY packet;
    packet.push_back(BNET_HEADER_CONSTANT);
    packet.push_back(SID_CHATCOMMAND);
    packet.push_back(0);
    packet.push_back(0);

    QByteArray message = command.toUtf8();
    for(int i = 0; i < message.length(); i++)
    {
        packet.push_back(message.at(i));
    }
    packet.push_back(0);

    AssignLength(packet);

    return packet;
}

BYTEARRAY CBNETProtocol :: SEND_SID_CHECKAD( )
{
	unsigned char Zeros[] = { 0, 0, 0, 0 };

	BYTEARRAY packet;
	packet.push_back( BNET_HEADER_CONSTANT );	// BNET header constant
	packet.push_back( SID_CHECKAD );			// SID_CHECKAD
	packet.push_back( 0 );						// packet length will be assigned later
	packet.push_back( 0 );						// packet length will be assigned later
	Util::appendByteArray( packet, Zeros, 4 );	// ???
	Util::appendByteArray( packet, Zeros, 4 );	// ???
	Util::appendByteArray( packet, Zeros, 4 );	// ???
	Util::appendByteArray( packet, Zeros, 4 );	// ???
	AssignLength( packet );
	// DEBUG_Print( "SENT SID_CHECKAD" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CBNETProtocol :: SEND_SID_STARTADVEX3( unsigned char state, BYTEARRAY mapGameType, BYTEARRAY mapFlags, BYTEARRAY mapWidth, BYTEARRAY mapHeight, string gameName, string hostName, uint32_t upTime, string mapPath, BYTEARRAY mapCRC, BYTEARRAY mapSHA1, uint32_t hostCounter )
{
	// todotodo: sort out how GameType works, the documentation is horrendous

/*

Game type tag: (read W3GS_GAMEINFO for this field)
 0x00000001 - Custom
 0x00000009 - Blizzard/Ladder
Map author: (mask 0x00006000) can be combined
*0x00002000 - Blizzard
 0x00004000 - Custom
Battle type: (mask 0x00018000) cant be combined
 0x00000000 - Battle
*0x00010000 - Scenario
Map size: (mask 0x000E0000) can be combined with 2 nearest values
 0x00020000 - Small
 0x00040000 - Medium
*0x00080000 - Huge
Observers: (mask 0x00700000) cant be combined
 0x00100000 - Allowed observers
 0x00200000 - Observers on defeat
*0x00400000 - No observers
Flags:
 0x00000800 - Private game flag (not used in game list)

*/

	unsigned char Unknown[]		= { 255,  3,  0,  0 };
	unsigned char CustomGame[]	= {   0,  0,  0,  0 };

	string HostCounterString = Util::toHexString( hostCounter );

	if( HostCounterString.size( ) < 8 )
		HostCounterString.insert( 0, 8 - HostCounterString.size( ), '0' );

	HostCounterString = string( HostCounterString.rbegin( ), HostCounterString.rend( ) );

	BYTEARRAY packet;

	// make the stat string

	BYTEARRAY StatString;
	Util::appendByteArrayFast( StatString, mapFlags );
	StatString.push_back( 0 );
	Util::appendByteArrayFast( StatString, mapWidth );
	Util::appendByteArrayFast( StatString, mapHeight );
	Util::appendByteArrayFast( StatString, mapCRC );
	Util::appendByteArrayFast( StatString, mapPath );
	Util::appendByteArrayFast( StatString, hostName );
	StatString.push_back( 0 );
	Util::appendByteArrayFast( StatString, mapSHA1 );
	StatString = Util::encodeStatString( StatString );

	if( mapGameType.size( ) == 4 && mapFlags.size( ) == 4 && mapWidth.size( ) == 2 && mapHeight.size( ) == 2 && !gameName.empty( ) && !hostName.empty( ) && !mapPath.empty( ) && mapCRC.size( ) == 4 && mapSHA1.size( ) == 20 && StatString.size( ) < 128 && HostCounterString.size( ) == 8 )
	{
		// make the rest of the packet

		packet.push_back( BNET_HEADER_CONSTANT );						// BNET header constant
		packet.push_back( SID_STARTADVEX3 );							// SID_STARTADVEX3
		packet.push_back( 0 );											// packet length will be assigned later
		packet.push_back( 0 );											// packet length will be assigned later
		packet.push_back( state );										// State (16 = public, 17 = private, 18 = close)
		packet.push_back( 0 );											// State continued...
		packet.push_back( 0 );											// State continued...
		packet.push_back( 0 );											// State continued...
		Util::appendByteArray( packet, upTime, false );					// time since creation
		Util::appendByteArrayFast( packet, mapGameType );				// Game Type, Parameter
		Util::appendByteArray( packet, Unknown, 4 );						// ???
		Util::appendByteArray( packet, CustomGame, 4 );					// Custom Game
		Util::appendByteArrayFast( packet, gameName );					// Game Name
		packet.push_back( 0 );											// Game Password is NULL
		packet.push_back( 98 );											// Slots Free (ascii 98 = char 'b' = 11 slots free) - note: do not reduce this as this is the # of PID's Warcraft III will allocate
		Util::appendByteArrayFast( packet, HostCounterString, false );	// Host Counter
		Util::appendByteArrayFast( packet, StatString );					// Stat String
		packet.push_back( 0 );											// Stat String null terminator (the stat string is encoded to remove all even numbers i.e. zeros)
		AssignLength( packet );
	}
	else
		CONSOLE_Print( ColoredMessage("[BNETPROTO] invalid parameters passed to SEND_SID_STARTADVEX3", ColoredMessage::ERROR) );

	// DEBUG_Print( "SENT SID_STARTADVEX3" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CBNETProtocol :: SEND_SID_NOTIFYJOIN( string gameName )
{
	unsigned char ProductID[]		= {  0, 0, 0, 0 };
	unsigned char ProductVersion[]	= { 14, 0, 0, 0 };	// Warcraft III is 14

	BYTEARRAY packet;
	packet.push_back( BNET_HEADER_CONSTANT );			// BNET header constant
	packet.push_back( SID_NOTIFYJOIN );					// SID_NOTIFYJOIN
	packet.push_back( 0 );								// packet length will be assigned later
	packet.push_back( 0 );								// packet length will be assigned later
	Util::appendByteArray( packet, ProductID, 4 );		// Product ID
	Util::appendByteArray( packet, ProductVersion, 4 );	// Product Version
	Util::appendByteArrayFast( packet, gameName );		// Game Name
	packet.push_back( 0 );								// Game Password is NULL
	AssignLength( packet );
	// DEBUG_Print( "SENT SID_NOTIFYJOIN" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CBNETProtocol :: SEND_SID_PING( BYTEARRAY pingValue )
{
	BYTEARRAY packet;

	if( pingValue.size( ) == 4 )
	{
		packet.push_back( BNET_HEADER_CONSTANT );		// BNET header constant
		packet.push_back( SID_PING );					// SID_PING
		packet.push_back( 0 );							// packet length will be assigned later
		packet.push_back( 0 );							// packet length will be assigned later
		Util::appendByteArrayFast( packet, pingValue );	// Ping Value
		AssignLength( packet );
	}
	else
		CONSOLE_Print( ColoredMessage("[BNETPROTO] invalid parameters passed to SEND_SID_PING", ColoredMessage::ERROR) );

	// DEBUG_Print( "SENT SID_PING" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CBNETProtocol :: SEND_SID_LOGONRESPONSE( BYTEARRAY clientToken, BYTEARRAY serverToken, BYTEARRAY passwordHash, string accountName )
{
	// todotodo: check that the passed BYTEARRAY sizes are correct (don't know what they should be right now so I can't do this today)

	BYTEARRAY packet;
	packet.push_back( BNET_HEADER_CONSTANT );			// BNET header constant
	packet.push_back( SID_LOGONRESPONSE );				// SID_LOGONRESPONSE
	packet.push_back( 0 );								// packet length will be assigned later
	packet.push_back( 0 );								// packet length will be assigned later
	Util::appendByteArrayFast( packet, clientToken );	// Client Token
	Util::appendByteArrayFast( packet, serverToken );	// Server Token
	Util::appendByteArrayFast( packet, passwordHash );	// Password Hash
	Util::appendByteArrayFast( packet, accountName );	// Account Name
	AssignLength( packet );
	// DEBUG_Print( "SENT SID_LOGONRESPONSE" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CBNETProtocol :: SEND_SID_NETGAMEPORT( uint16_t serverPort )
{
	BYTEARRAY packet;
	packet.push_back( BNET_HEADER_CONSTANT );			// BNET header constant
	packet.push_back( SID_NETGAMEPORT );				// SID_NETGAMEPORT
	packet.push_back( 0 );								// packet length will be assigned later
	packet.push_back( 0 );								// packet length will be assigned later
	Util::appendByteArray( packet, serverPort, false );	// local game server port
	AssignLength( packet );
	// DEBUG_Print( "SENT SID_NETGAMEPORT" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CBNETProtocol :: SEND_SID_AUTH_INFO( unsigned int ver, bool TFT, string countryAbbrev, string country )
{
	unsigned char ProtocolID[]		= {   0,   0,   0,   0 };
	unsigned char PlatformID[]		= {  54,  56,  88,  73 };	// "IX86"
	unsigned char ProductID_ROC[]	= {  51,  82,  65,  87 };	// "WAR3"
	unsigned char ProductID_TFT[]	= {  80,  88,  51,  87 };	// "W3XP"
//	unsigned char Version[]			= { ver,   0,   0,   0 };
	unsigned char Language[]		= {  83,  85, 110, 101 };	// "enUS"
	unsigned char LocalIP[]			= { 127,   0,   0,   1 };
	unsigned char TimeZoneBias[]	= {  44,   1,   0,   0 };	// 300 minutes (GMT -0500)
	unsigned char LocaleID[]		= {   9,   4,   0,   0 };	// 0x0409 English (United States)
	unsigned char LanguageID[]		= {   9,   4,   0,   0 };	// 0x0409 English (United States)

	BYTEARRAY packet;
	packet.push_back( BNET_HEADER_CONSTANT );				// BNET header constant
	packet.push_back( SID_AUTH_INFO );						// SID_AUTH_INFO
	packet.push_back( 0 );									// packet length will be assigned later
	packet.push_back( 0 );									// packet length will be assigned later
	Util::appendByteArray( packet, ProtocolID, 4 );			// Protocol ID
	Util::appendByteArray( packet, PlatformID, 4 );			// Platform ID

	if( TFT )
		Util::appendByteArray( packet, ProductID_TFT, 4 );	// Product ID (TFT)
	else
		Util::appendByteArray( packet, ProductID_ROC, 4 );	// Product ID (ROC)

//	Util::appendByteArray( packet, Version, 4 );				// Version
        Util::appendByteArray( packet, ver, false);
	Util::appendByteArray( packet, Language, 4 );			// Language
	Util::appendByteArray( packet, LocalIP, 4 );				// Local IP for NAT compatibility
	Util::appendByteArray( packet, TimeZoneBias, 4 );		// Time Zone Bias
	Util::appendByteArray( packet, LocaleID, 4 );			// Locale ID
	Util::appendByteArray( packet, LanguageID, 4 );			// Language ID
	Util::appendByteArrayFast( packet, countryAbbrev );		// Country Abbreviation
	Util::appendByteArrayFast( packet, country );			// Country
	AssignLength( packet );
	// DEBUG_Print( "SENT SID_AUTH_INFO" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CBNETProtocol :: SEND_SID_AUTH_CHECK( bool TFT, BYTEARRAY clientToken, BYTEARRAY exeVersion, BYTEARRAY exeVersionHash, BYTEARRAY keyInfoROC, BYTEARRAY keyInfoTFT, string exeInfo, string keyOwnerName )
{
	uint32_t NumKeys = 0;

	if( TFT )
		NumKeys = 2;
	else
		NumKeys = 1;

	BYTEARRAY packet;

	if( clientToken.size( ) == 4 && exeVersion.size( ) == 4 && exeVersionHash.size( ) == 4 && keyInfoROC.size( ) == 36 && ( !TFT || keyInfoTFT.size( ) == 36 ) )
	{
		packet.push_back( BNET_HEADER_CONSTANT );			// BNET header constant
		packet.push_back( SID_AUTH_CHECK );					// SID_AUTH_CHECK
		packet.push_back( 0 );								// packet length will be assigned later
		packet.push_back( 0 );								// packet length will be assigned later
		Util::appendByteArrayFast( packet, clientToken );	// Client Token
		Util::appendByteArrayFast( packet, exeVersion );		// EXE Version
		Util::appendByteArrayFast( packet, exeVersionHash );	// EXE Version Hash
		Util::appendByteArray( packet, NumKeys, false );		// number of keys in this packet
		Util::appendByteArray( packet, (uint32_t)0, false );	// boolean Using Spawn (32 bit)
		Util::appendByteArrayFast( packet, keyInfoROC );		// ROC Key Info

		if( TFT )
			Util::appendByteArrayFast( packet, keyInfoTFT );	// TFT Key Info

		Util::appendByteArrayFast( packet, exeInfo );		// EXE Info
		Util::appendByteArrayFast( packet, keyOwnerName );	// CD Key Owner Name
		AssignLength( packet );
	}
	else
		CONSOLE_Print( ColoredMessage("[BNETPROTO] invalid parameters passed to SEND_SID_AUTH_CHECK", ColoredMessage::ERROR) );

	// DEBUG_Print( "SENT SID_AUTH_CHECK" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CBNETProtocol :: SEND_SID_AUTH_ACCOUNTLOGON( BYTEARRAY clientPublicKey, string accountName )
{
	BYTEARRAY packet;

	if( clientPublicKey.size( ) == 32 )
	{
		packet.push_back( BNET_HEADER_CONSTANT );				// BNET header constant
		packet.push_back( SID_AUTH_ACCOUNTLOGON );				// SID_AUTH_ACCOUNTLOGON
		packet.push_back( 0 );									// packet length will be assigned later
		packet.push_back( 0 );									// packet length will be assigned later
		Util::appendByteArrayFast( packet, clientPublicKey );	// Client Key
		Util::appendByteArrayFast( packet, accountName );		// Account Name
		AssignLength( packet );
	}
	else
		CONSOLE_Print( ColoredMessage("[BNETPROTO] invalid parameters passed to SEND_SID_AUTH_ACCOUNTLOGON", ColoredMessage::ERROR) );

	// DEBUG_Print( "SENT SID_AUTH_ACCOUNTLOGON" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CBNETProtocol :: SEND_SID_AUTH_ACCOUNTLOGONPROOF( BYTEARRAY clientPasswordProof )
{
	BYTEARRAY packet;

	if( clientPasswordProof.size( ) == 20 )
	{
		packet.push_back( BNET_HEADER_CONSTANT );					// BNET header constant
		packet.push_back( SID_AUTH_ACCOUNTLOGONPROOF );				// SID_AUTH_ACCOUNTLOGONPROOF
		packet.push_back( 0 );										// packet length will be assigned later
		packet.push_back( 0 );										// packet length will be assigned later
		Util::appendByteArrayFast( packet, clientPasswordProof );	// Client Password Proof
		AssignLength( packet );
	}
	else
		CONSOLE_Print( ColoredMessage("[BNETPROTO] invalid parameters passed to SEND_SID_AUTH_ACCOUNTLOGON", ColoredMessage::ERROR) );

	// DEBUG_Print( "SENT SID_AUTH_ACCOUNTLOGONPROOF" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CBNETProtocol :: SEND_SID_WARDEN( BYTEARRAY wardenResponse )
{
	BYTEARRAY packet;
	packet.push_back( BNET_HEADER_CONSTANT );			// BNET header constant
	packet.push_back( SID_WARDEN );						// SID_WARDEN
	packet.push_back( 0 );								// packet length will be assigned later
	packet.push_back( 0 );								// packet length will be assigned later
	Util::appendByteArrayFast( packet, wardenResponse );	// warden response
	AssignLength( packet );
	// DEBUG_Print( "SENT SID_WARDEN" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CBNETProtocol :: SEND_SID_FRIENDSLIST( )
{
    BYTEARRAY packet;
    packet.push_back( BNET_HEADER_CONSTANT );	// BNET header constant
    packet.push_back( SID_FRIENDSLIST );	// SID_FRIENDSLIST
    packet.push_back( 0 );			// packet length will be assigned later
    packet.push_back( 0 );			// packet length will be assigned later
    AssignLength( packet );
    // DEBUG_Print( "SENT SID_FRIENDSLIST" );
    // DEBUG_Print( packet );
    return packet;
}

BYTEARRAY CBNETProtocol::SEND_SID_FRIENDSUPDATE(unsigned char entryNumber)
{
    BYTEARRAY packet;
    packet.push_back( BNET_HEADER_CONSTANT );	// BNET header constant
    packet.push_back( SID_FRIENDSUPDATE );	// SID_FRIENDSUPDATE
    packet.push_back( 0 );			// packet length will be assigned later
    packet.push_back( 0 );			// packet length will be assigned later
    packet.push_back( entryNumber );
    AssignLength( packet );
    // DEBUG_Print( "SENT SID_FRIENDSUPDATE" );
    // DEBUG_Print( packet );
    return packet;
}

BYTEARRAY CBNETProtocol :: SEND_SID_CLANMEMBERLIST( )
{
	unsigned char Cookie[] = { 0, 0, 0, 0 };

	BYTEARRAY packet;
	packet.push_back( BNET_HEADER_CONSTANT );	// BNET header constant
	packet.push_back( SID_CLANMEMBERLIST );		// SID_CLANMEMBERLIST
	packet.push_back( 0 );						// packet length will be assigned later
	packet.push_back( 0 );						// packet length will be assigned later
	Util::appendByteArray( packet, Cookie, 4 );	// cookie
	AssignLength( packet );
	// DEBUG_Print( "SENT SID_CLANMEMBERLIST" );
	// DEBUG_Print( packet );
	return packet;
}

/////////////////////
// OTHER FUNCTIONS //
/////////////////////

bool CBNETProtocol :: AssignLength( BYTEARRAY &content )
{
	// insert the actual length of the content array into bytes 3 and 4 (indices 2 and 3)

	BYTEARRAY LengthBytes;

	if( content.size( ) >= 4 && content.size( ) <= 65535 )
	{
		LengthBytes = Util::createByteArray( (uint16_t)content.size( ), false );
		content[2] = LengthBytes[0];
		content[3] = LengthBytes[1];
		return true;
	}

	return false;
}

bool CBNETProtocol :: ValidateLength( BYTEARRAY &content )
{
	// verify that bytes 3 and 4 (indices 2 and 3) of the content array describe the length

	uint16_t Length;
	BYTEARRAY LengthBytes;

	if( content.size( ) >= 4 && content.size( ) <= 65535 )
	{
		LengthBytes.push_back( content[2] );
		LengthBytes.push_back( content[3] );
		Length = Util::byteArrayToUInt16( LengthBytes, false );

		if( Length == content.size( ) )
			return true;
	}

	return false;
}

//
// CIncomingGameHost
//

uint32_t CIncomingGameHost :: NextUniqueGameID = 1;

CIncomingGameHost :: CIncomingGameHost( uint16_t nGameType, uint16_t nParameter, uint32_t nLanguageID, uint16_t nPort, BYTEARRAY &nIP, uint32_t nStatus, uint32_t nElapsedTime, string nGameName, unsigned char nSlotsTotal, uint32_t nHostCounter, BYTEARRAY &nStatString )
{
	m_GameType = nGameType;
	m_Parameter = nParameter;
	m_LanguageID = nLanguageID;
	m_Port = nPort;
	m_IP = nIP;
	m_Status = nStatus;
	m_ElapsedTime = nElapsedTime;
	m_GameName = nGameName;
	m_SlotsTotal = nSlotsTotal;
	m_HostCounter = nHostCounter;
	m_StatString = nStatString;
	m_UniqueGameID = NextUniqueGameID++;
	m_ReceivedTime = getElapsedSeconds();

	// decode stat string

	BYTEARRAY StatString = Util::decodeStatString( m_StatString );
	BYTEARRAY MapFlags;
	BYTEARRAY MapWidth;
	BYTEARRAY MapHeight;
	BYTEARRAY MapCRC;
	BYTEARRAY MapPath;
	BYTEARRAY HostName;

	if( StatString.size( ) >= 14 )
	{
		unsigned int i = 13;
		MapFlags = BYTEARRAY( StatString.begin( ), StatString.begin( ) + 4 );
		MapWidth = BYTEARRAY( StatString.begin( ) + 5, StatString.begin( ) + 7 );
		MapHeight = BYTEARRAY( StatString.begin( ) + 7, StatString.begin( ) + 9 );
		MapCRC = BYTEARRAY( StatString.begin( ) + 9, StatString.begin( ) + 13 );
		MapPath = Util::extractNTCString( StatString, 13 );
		i += MapPath.size( ) + 1;

		m_MapFlags = Util::byteArrayToUInt32( MapFlags, false );
		m_MapWidth = Util::byteArrayToUInt16( MapWidth, false );
		m_MapHeight = Util::byteArrayToUInt16( MapHeight, false );
		m_MapCRC = MapCRC;
		m_MapPath = string( MapPath.begin( ), MapPath.end( ) );


		if( StatString.size( ) >= i + 1 )
		{
			HostName = Util::extractNTCString( StatString, i );
			m_HostName = string( HostName.begin( ), HostName.end( ) );
		}
	}
}

CIncomingGameHost :: ~CIncomingGameHost( )
{

}

string CIncomingGameHost :: GetIPString( )
{
	string Result;

	if( m_IP.size( ) >= 4 )
	{
		for( unsigned int i = 0; i < 4; i++ )
		{
			Result += Util::toString( (unsigned int)m_IP[i] );

			if( i < 3 )
				Result += ".";
		}
	}

	return Result;
}

//
// CIncomingChatEvent
//

CIncomingChatEvent :: CIncomingChatEvent( CBNETProtocol :: IncomingChatEvent nChatEvent, uint32_t nPing, string nUser, QString nMessage )
{
	m_ChatEvent = nChatEvent;
	m_Ping = nPing;
	m_User = QString::fromStdString(nUser);
	m_Message = nMessage;
}

CIncomingChatEvent :: ~CIncomingChatEvent( )
{

}
