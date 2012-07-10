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
#include "gameprotocol.h"

//
// CGameProtocol
//

CGameProtocol :: CGameProtocol( CGProxy *nGProxy )
{
	m_GProxy = nGProxy;
}

CGameProtocol :: ~CGameProtocol( )
{

}

///////////////////////
// RECEIVE FUNCTIONS //
///////////////////////

////////////////////
// SEND FUNCTIONS //
////////////////////

BYTEARRAY CGameProtocol :: SEND_W3GS_CHAT_FROM_HOST( unsigned char fromPID, BYTEARRAY toPIDs, unsigned char flag, BYTEARRAY flagExtra, QString message )
{
	BYTEARRAY packet;

	if( !toPIDs.empty( ) && !message.isEmpty( ) && message.size( ) < 255 )
	{
		packet.push_back( W3GS_HEADER_CONSTANT );		// W3GS header constant
		packet.push_back( W3GS_CHAT_FROM_HOST );		// W3GS_CHAT_FROM_HOST
		packet.push_back( 0 );							// packet length will be assigned later
		packet.push_back( 0 );							// packet length will be assigned later
		packet.push_back( toPIDs.size( ) );				// number of receivers
		Util::appendByteArrayFast( packet, toPIDs );		// receivers
		packet.push_back( fromPID );					// sender
		packet.push_back( flag );						// flag
		Util::appendByteArrayFast( packet, flagExtra );	// extra flag

                QByteArray messageBytes = message.toUtf8();
                for(int i = 0; i < messageBytes.length(); i++)
                {
                    packet.push_back(messageBytes.at(i));
                }
                packet.push_back(0);

		AssignLength( packet );
	}
	else
		CONSOLE_Print(ColoredMessage( "[GAMEPROTO] invalid parameters passed to SEND_W3GS_CHAT_FROM_HOST", ColoredMessage::ERROR) );

	// DEBUG_Print( "SENT W3GS_CHAT_FROM_HOST" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CGameProtocol :: SEND_W3GS_CHAT_TO_HOST( unsigned char fromPID, BYTEARRAY toPIDs, unsigned char flag, BYTEARRAY flagExtra, QString message )
{
	BYTEARRAY packet;

	if( !toPIDs.empty( ) && !message.isEmpty( ) && message.size( ) < 255 )
	{
		packet.push_back( W3GS_HEADER_CONSTANT );		// W3GS header constant
		packet.push_back( W3GS_CHAT_TO_HOST );			// W3GS_CHAT_TO_HOST
		packet.push_back( 0 );							// packet length will be assigned later
		packet.push_back( 0 );							// packet length will be assigned later
		packet.push_back( toPIDs.size( ) );				// number of receivers
		Util::appendByteArrayFast( packet, toPIDs );		// receivers
		packet.push_back( fromPID );					// sender
		packet.push_back( flag );						// flag
		Util::appendByteArrayFast( packet, flagExtra );	// extra flag

                QByteArray messageBytes = message.toUtf8();
                for(int i = 0; i < messageBytes.length(); i++)
                {
                    packet.push_back(messageBytes.at(i));
                }
                packet.push_back(0);

		AssignLength( packet );
	}
	else
		CONSOLE_Print(ColoredMessage( "[GAMEPROTO] invalid parameters passed to SEND_W3GS_CHAT_TO_HOST", ColoredMessage::ERROR) );

	// DEBUG_Print( "SENT W3GS_CHAT_FROM_HOST" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CGameProtocol :: SEND_W3GS_TEAMCHANGE( unsigned char fromPID, BYTEARRAY toPIDs, unsigned char team )
{
	BYTEARRAY packet;

	if( !toPIDs.empty( ) )
	{
		packet.push_back( W3GS_HEADER_CONSTANT );
		packet.push_back( W3GS_CHAT_TO_HOST );
		packet.push_back( 0 );
		packet.push_back( 0 );
		packet.push_back( toPIDs.size( ) );
		Util::appendByteArrayFast( packet, toPIDs );
		packet.push_back( fromPID );
		packet.push_back( ((unsigned char) 17) );
		packet.push_back( team );
		AssignLength( packet );
	}
	else
		CONSOLE_Print(ColoredMessage( "[GAMEPROTO] invalid parameters passed to SEND_W3GS_TEAMCHANGE", ColoredMessage::ERROR) );

	// DEBUG_Print( "SENT W3GS_CHAT_FROM_HOST" );
	// DEBUG_Print( packet );
	return packet;
}


BYTEARRAY CGameProtocol :: SEND_W3GS_SEARCHGAME( bool TFT, unsigned char war3Version )
{
	unsigned char ProductID_ROC[]	= {          51, 82, 65, 87 };	// "WAR3"
	unsigned char ProductID_TFT[]	= {          80, 88, 51, 87 };	// "W3XP"
	unsigned char Version[]			= { war3Version,  0,  0,  0 };
	unsigned char Unknown[]			= {           0,  0,  0,  0 };

	BYTEARRAY packet;
	packet.push_back( W3GS_HEADER_CONSTANT );				// W3GS header constant
	packet.push_back( W3GS_SEARCHGAME );					// W3GS_SEARCHGAME
	packet.push_back( 0 );									// packet length will be assigned later
	packet.push_back( 0 );									// packet length will be assigned later

	if( TFT )
		Util::appendByteArray( packet, ProductID_TFT, 4 );	// Product ID (TFT)
	else
		Util::appendByteArray( packet, ProductID_ROC, 4 );	// Product ID (ROC)

	Util::appendByteArray( packet, Version, 4 );				// Version
	Util::appendByteArray( packet, Unknown, 4 );				// ???
	AssignLength( packet );
	// DEBUG_Print( "SENT W3GS_SEARCHGAME" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CGameProtocol :: SEND_W3GS_GAMEINFO( bool TFT, unsigned char war3Version, BYTEARRAY mapGameType, BYTEARRAY mapFlags, BYTEARRAY mapWidth, BYTEARRAY mapHeight, string gameName, string hostName, uint32_t upTime, string mapPath, BYTEARRAY mapCRC, uint32_t slotsTotal, uint32_t slotsOpen, uint16_t port, uint32_t hostCounter, uint32_t entryKey )
{
	unsigned char ProductID_ROC[]	= {          51, 82, 65, 87 };	// "WAR3"
	unsigned char ProductID_TFT[]	= {          80, 88, 51, 87 };	// "W3XP"
	unsigned char Version[]			= { war3Version,  0,  0,  0 };
	unsigned char Unknown[]			= {           1,  0,  0,  0 };

	BYTEARRAY packet;

	if( mapGameType.size( ) == 4 && mapFlags.size( ) == 4 && mapWidth.size( ) == 2 && mapHeight.size( ) == 2 && !gameName.empty( ) && !hostName.empty( ) && !mapPath.empty( ) && mapCRC.size( ) == 4 )
	{
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
		StatString = Util::encodeStatString( StatString );

		// make the rest of the packet

		packet.push_back( W3GS_HEADER_CONSTANT );						// W3GS header constant
		packet.push_back( W3GS_GAMEINFO );								// W3GS_GAMEINFO
		packet.push_back( 0 );											// packet length will be assigned later
		packet.push_back( 0 );											// packet length will be assigned later

		if( TFT )
			Util::appendByteArray( packet, ProductID_TFT, 4 );			// Product ID (TFT)
		else
			Util::appendByteArray( packet, ProductID_ROC, 4 );			// Product ID (ROC)

		Util::appendByteArray( packet, Version, 4 );						// Version
		Util::appendByteArray( packet, hostCounter, false );				// Host Counter
		Util::appendByteArray( packet, entryKey, false );				// Entry Key
		Util::appendByteArrayFast( packet, gameName );					// Game Name
		packet.push_back( 0 );											// ??? (maybe game password)
		Util::appendByteArrayFast( packet, StatString );					// Stat String
		packet.push_back( 0 );											// Stat String null terminator (the stat string is encoded to remove all even numbers i.e. zeros)
		Util::appendByteArray( packet, slotsTotal, false );				// Slots Total
		Util::appendByteArrayFast( packet, mapGameType );				// Game Type
		Util::appendByteArray( packet, Unknown, 4 );						// ???
		Util::appendByteArray( packet, slotsOpen, false );				// Slots Open
		Util::appendByteArray( packet, upTime, false );					// time since creation
		Util::appendByteArray( packet, port, false );					// port
		AssignLength( packet );
	}
	else
		CONSOLE_Print(ColoredMessage( "[GAMEPROTO] invalid parameters passed to SEND_W3GS_GAMEINFO", ColoredMessage::ERROR) );

	// DEBUG_Print( "SENT W3GS_GAMEINFO" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CGameProtocol :: SEND_W3GS_CREATEGAME( bool TFT, unsigned char war3Version )
{
	unsigned char ProductID_ROC[]	= {          51, 82, 65, 87 };	// "WAR3"
	unsigned char ProductID_TFT[]	= {          80, 88, 51, 87 };	// "W3XP"
	unsigned char Version[]			= { war3Version,  0,  0,  0 };
	unsigned char HostCounter[]		= {           1,  0,  0,  0 };

	BYTEARRAY packet;
	packet.push_back( W3GS_HEADER_CONSTANT );				// W3GS header constant
	packet.push_back( W3GS_CREATEGAME );					// W3GS_CREATEGAME
	packet.push_back( 0 );									// packet length will be assigned later
	packet.push_back( 0 );									// packet length will be assigned later

	if( TFT )
		Util::appendByteArray( packet, ProductID_TFT, 4 );	// Product ID (TFT)
	else
		Util::appendByteArray( packet, ProductID_ROC, 4 );	// Product ID (ROC)

	Util::appendByteArray( packet, Version, 4 );				// Version
	Util::appendByteArray( packet, HostCounter, 4 );			// Host Counter
	AssignLength( packet );
	// DEBUG_Print( "SENT W3GS_CREATEGAME" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CGameProtocol :: SEND_W3GS_REFRESHGAME( uint32_t players, uint32_t playerSlots )
{
	unsigned char HostCounter[]	= { 1, 0, 0, 0 };

	BYTEARRAY packet;
	packet.push_back( W3GS_HEADER_CONSTANT );			// W3GS header constant
	packet.push_back( W3GS_REFRESHGAME );				// W3GS_REFRESHGAME
	packet.push_back( 0 );								// packet length will be assigned later
	packet.push_back( 0 );								// packet length will be assigned later
	Util::appendByteArray( packet, HostCounter, 4 );		// Host Counter
	Util::appendByteArray( packet, players, false );		// Players
	Util::appendByteArray( packet, playerSlots, false );	// Player Slots
	AssignLength( packet );
	// DEBUG_Print( "SENT W3GS_REFRESHGAME" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CGameProtocol :: SEND_W3GS_DECREATEGAME( uint32_t hostCounter )
{
	BYTEARRAY packet;
	packet.push_back( W3GS_HEADER_CONSTANT );			// W3GS header constant
	packet.push_back( W3GS_DECREATEGAME );				// W3GS_DECREATEGAME
	packet.push_back( 0 );								// packet length will be assigned later
	packet.push_back( 0 );								// packet length will be assigned later
	Util::appendByteArray( packet, hostCounter, false );	// Host Counter
	AssignLength( packet );
	// DEBUG_Print( "SENT W3GS_DECREATEGAME" );
	// DEBUG_Print( packet );
	return packet;
}

BYTEARRAY CGameProtocol :: SEND_W3GS_LEAVEGAME( uint32_t reason )
{
        /*
         * Reasons:
         * 0x01 PLAYERLEAVE_DISCONNECT
         * 0x07 PLAYERLEAVE_LOST
         * 0x08 PLAYERLEAVE_LOSTBUILDINGS
         * 0x09 PLAYERLEAVE_WON
         * 0x0A PLAYERLEAVE_DRAW
         * 0x0B PLAYERLEAVE_OBSERVER
         * 0x0D PLAYERLEAVE_LOBBY
         */

	BYTEARRAY packet;
	packet.push_back( W3GS_HEADER_CONSTANT );			// W3GS header constant
	packet.push_back( W3GS_LEAVEGAME );				// W3GS_LEAVEGAME
	packet.push_back( 0 );						// packet length will be assigned later
	packet.push_back( 0 );						// packet length will be assigned later
	Util::appendByteArray( packet, reason, false );                 // Reason
	AssignLength( packet );
	// DEBUG_Print( "SENT W3GS_LEAVEGAME" );
	// DEBUG_Print( packet );
	return packet;
}

//BYTEARRAY CGameProtocol :: SEND_W3GS_REQJOIN(uint32_t hostCounter,
//        uint32_t entryKey, unsigned char unknownByte, uint16_t listenPort,
//        uint32_t peerKey, string playerName, uint32_t unknownDWORD,
//        uint16_t internalPort, uint32_t internalIP)
//{
//    BYTEARRAY packet;
//
//    packet.push_back( W3GS_HEADER_CONSTANT );
//    packet.push_back( W3GS_REQJOIN );
//    packet.push_back( 0 );
//    packet.push_back( 0 );
//    Util::appendByteArray( packet, hostCounter, false );
//    Util::appendByteArray( packet, entryKey, false );
//    packet.push_back(unknownByte);
//    Util::appendByteArray( packet, listenPort, false );
//    Util::appendByteArray( packet, peerKey, false );
//    Util::appendByteArrayFast( packet, playerName );
//    Util::appendByteArray( packet, unknownDWORD, false );
//    Util::appendByteArray( packet, internalPort, false );
//    Util::appendByteArray( packet, internalIP, false );
//    AssignLength( packet );
//    CONSOLE_Print(ColoredMessage("SENDING REQJOIN");
//
//    return packet;
//}

/////////////////////
// OTHER FUNCTIONS //
/////////////////////

bool CGameProtocol :: AssignLength( BYTEARRAY &content )
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

bool CGameProtocol :: ValidateLength( BYTEARRAY &content )
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
