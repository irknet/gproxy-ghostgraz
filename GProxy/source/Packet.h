#ifndef PACKET_H
#define	PACKET_H

#include "gproxy.h"

#include <QObject>
#include <QQueue>

class Packet : public QObject
{
    Q_OBJECT

public:
    Packet();
    Packet(const BYTEARRAY &bytearray);
    virtual ~Packet();

    void assignLength();

    void addByte(const uint8_t &byte);
    void addWord(const uint16_t &word);
    void addDWord(const uint32_t &dword);
    void addQWord(const uint64_t &qword);
    void addString(const QString &text);
    void addNTString(const QString &text);

    uint8_t removeByte();
    uint16_t removeWord();
    uint32_t removeDWord();
    uint64_t removeQWord();
    QString removeString(const int &length);
    QString removeNTString();

private:
    QQueue<uint8_t> data;
};

#endif	/* PACKET_H */

