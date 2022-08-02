#include "packet_translator.h"

PacketTranslator::PacketTranslator()
{

}

bool PacketTranslator::push(char b)
{
    switch(b)
    {
    case '\r':
    {
        return true;
        break;
    }
    case '\n':
    {
        byteArray.clear();
        break;
    }
    default:
    {
        byteArray.append(b);
    }
    }

    return false;
}

const QByteArray &PacketTranslator::get()
{
    return byteArray;
}

const QString PacketTranslator::getStr()
{
    return QString::fromStdString(byteArray.toStdString());
}
