#include "Packet.h"
#include <QTextCodec>
#include <vector>

Packet::Packet ()
{

}

Packet::Packet(const BYTEARRAY &bytearray)
{
    for(unsigned int i = 0; i < bytearray.size(); i++)
    {
        data.enqueue(bytearray.at(i));
    }
}

Packet::~Packet ()
{
}

void Packet::addByte(const uint8_t &byte)
{
    data.enqueue(byte);
}

void Packet::addWord(const uint16_t &word)
{
    data.enqueue((uint8_t) ((word & 0x00FF) >> 0));
    data.enqueue((uint8_t) ((word & 0xFF00) >> 8));
}

void Packet::addDWord(const uint32_t &dword)
{
    data.enqueue((uint8_t) ((dword & 0x000000FF) >> 0));
    data.enqueue((uint8_t) ((dword & 0x0000FF00) >> 8));
    data.enqueue((uint8_t) ((dword & 0x00FF0000) >> 16));
    data.enqueue((uint8_t) ((dword & 0xFF000000) >> 24));
}

void Packet::addQWord(const uint64_t &qword)
{
    data.enqueue((uint8_t) ((qword & 0x00000000000000FFLLU) >> 0));
    data.enqueue((uint8_t) ((qword & 0x000000000000FF00LLU) >> 8));
    data.enqueue((uint8_t) ((qword & 0x0000000000FF0000LLU) >> 16));
    data.enqueue((uint8_t) ((qword & 0x00000000FF000000LLU) >> 24));
    data.enqueue((uint8_t) ((qword & 0x000000FF00000000LLU) >> 32));
    data.enqueue((uint8_t) ((qword & 0x0000FF0000000000LLU) >> 40));
    data.enqueue((uint8_t) ((qword & 0x00FF000000000000LLU) >> 48));
    data.enqueue((uint8_t) ((qword & 0xFF00000000000000LLU) >> 56));
}

void Packet::addString(const QString &text)
{
    QByteArray textBytes;
    textBytes.append(text);
    QString utf8String = QTextCodec::codecForName("UTF-8")->toUnicode(textBytes);

    for(int i = 0; i < utf8String.length(); i++)
    {
        data.enqueue((uint8_t) utf8String.at(i).toAscii());
    }
}

void Packet::addNTString(const QString &text)
{
    QByteArray textBytes;
    textBytes.append(text);
    QString utf8String = QTextCodec::codecForName("UTF-8")->toUnicode(textBytes);

    for(int i = 0; i < utf8String.length(); i++)
    {
        data.enqueue((uint8_t) utf8String.at(i).toAscii());
    }

    data.enqueue(0);
}

uint8_t Packet::removeByte()
{
    if(data.length() < 1)
    {
        return 0;
    }

    return data.dequeue();
}

uint16_t Packet::removeWord ()
{
    if(data.length() < 2)
    {
        return 0;
    }

    return (uint16_t) ((data.dequeue() << 0) & 0x00FF) | (uint16_t) ((data.dequeue() << 8) & 0xFF00);
}

uint32_t Packet::removeDWord ()
{
    if(data.length() < 4)
    {
        return 0;
    }

    return (uint32_t) ((data.dequeue() << 0) & 0x000000FF)
            | (uint32_t) ((data.dequeue() << 8) & 0x0000FF00)
            | (uint32_t) ((data.dequeue() << 16) & 0x00FF0000)
            | (uint32_t) ((data.dequeue() << 24) & 0xFF000000);
}

uint64_t Packet::removeQWord ()
{
    if(data.length() < 8)
    {
        return 0;
    }

    return (((uint64_t) data.dequeue() << 0L) & 0x00000000000000FFLLU)
            | (((uint64_t) data.dequeue() << 8L) & 0x000000000000FF00LLU)
            | (((uint64_t) data.dequeue() << 16L) & 0x0000000000FF0000LLU)
            | (((uint64_t) data.dequeue() << 24L) & 0x00000000FF000000LLU)
            | (((uint64_t) data.dequeue() << 32L) & 0x000000FF00000000LLU)
            | (((uint64_t) data.dequeue() << 40L) & 0x0000FF0000000000LLU)
            | (((uint64_t) data.dequeue() << 48L) & 0x00FF000000000000LLU)
            | (((uint64_t) data.dequeue() << 56L) & 0xFF00000000000000LLU);
}

QString Packet::removeString(const int &length)
{
    if(data.length() < length)
    {
        return 0;
    }

    QByteArray encodedString;
    for(int i = 0; i < length; i++)
    {
        encodedString.append(data.dequeue());
    }

    return QTextCodec::codecForName("UTF-8")->toUnicode(encodedString);
}

QString Packet::removeNTString()
{
    if(data.length() < 2)
    {
        return 0;
    }

    QByteArray encodedString;
    while(data.length() > 0)
    {
        uint8_t b = data.dequeue();
        if(b == 0)
        {
            return QTextCodec::codecForName("UTF-8")->toUnicode(encodedString);
        }
        else
        {
            encodedString.append(b);
        }
    }

    return QTextCodec::codecForName("UTF-8")->toUnicode(encodedString);
}

void Packet::assignLength()
{
    if( data.length( ) >= 4 && data.length() <= 65535 )
    {
        data[2] = ((uint8_t) data.length());
        data[3] = ((uint8_t) data.length() >> 8);
    }
}