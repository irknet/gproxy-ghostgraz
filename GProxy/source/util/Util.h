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

#ifndef UTIL_H
#define UTIL_H

#include "gproxy.h"

#include <QString>
#include <QColor>
#include <string>

using namespace std;

class Util
{
public:
    // byte arrays

    static BYTEARRAY createByteArray( unsigned char *a, int size );
    static BYTEARRAY createByteArray( unsigned char c );
    static BYTEARRAY createByteArray( uint16_t i, bool reverse );
    static BYTEARRAY createByteArray( uint32_t i, bool reverse );
    static uint16_t byteArrayToUInt16( BYTEARRAY b, bool reverse, unsigned int start = 0 );
    static uint32_t byteArrayToUInt32( BYTEARRAY b, bool reverse, unsigned int start = 0 );
    static string byteArrayToDecString( BYTEARRAY b );
    static string byteArrayToHexString( BYTEARRAY b );
    static void appendByteArray( BYTEARRAY &b, BYTEARRAY append );
    static void appendByteArrayFast( BYTEARRAY &b, BYTEARRAY &append );
    static void appendByteArray( BYTEARRAY &b, unsigned char *a, int size );
    static void appendByteArray( BYTEARRAY &b, string append, bool terminator = true );
    static void appendByteArrayFast( BYTEARRAY &b, string &append, bool terminator = true );
    static void appendByteArray( BYTEARRAY &b, uint16_t i, bool reverse );
    static void appendByteArray( BYTEARRAY &b, uint32_t i, bool reverse );
    static BYTEARRAY extractNTCString( BYTEARRAY &b, unsigned int start );
    static QString extractNTString ( BYTEARRAY &b, unsigned int start );
    static QString extractNTString ( BYTEARRAY &b, unsigned int start, unsigned int& nextByte );
    static QString extractString ( BYTEARRAY &b, unsigned int start, unsigned int length );
    static unsigned char extractHex( BYTEARRAY &b, unsigned int start, bool reverse );
    static BYTEARRAY extractNumbers( string s, unsigned int count );
    static BYTEARRAY extractHexNumbers( string s );
    static QString revertString(const QString& s);

    // conversions

    static string toString( unsigned long i );
    static string toString( unsigned short i );
    static string toString( unsigned int i );
    static string toString( long i );
    static string toString( short i );
    static string toString( int i );
    static string toString( float f, int digits );
    static string toString( double d, int digits );
    static QString toString(const QColor& color);
    static string toHexString( uint32_t i );
    static uint16_t toUInt16( string &s );
    static uint32_t toUInt32( string &s );
    static int16_t toInt16( string &s );
    static int32_t toInt32( string &s );
    static double toDouble( string &s );
    static string msToString( uint32_t ms );
    static QColor toColor(const QString& colorString);

    // files

    static bool fileExists( string file );
    static string fileRead( string file, uint32_t start, uint32_t length );
    static string fileRead( string file );
    static bool fileWrite( string file, unsigned char *data, uint32_t length );
    static string fileSafeName( string fileName );
    static string addPathSeperator( string path );

    // stat strings

    static BYTEARRAY encodeStatString( BYTEARRAY &data );
    static BYTEARRAY decodeStatString( BYTEARRAY &data );

private:
    Util();
};

#endif
