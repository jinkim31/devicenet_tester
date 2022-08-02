#ifndef PACKETTRANSLATOR_H
#define PACKETTRANSLATOR_H

#include <QByteArray>
#include <QString>

class PacketTranslator
{
public:
    PacketTranslator();
    bool push(char b);
    const QByteArray& get();
    const QString getStr();
private:
    QByteArray byteArray;
};

#endif // PACKETTRANSLATOR_H
