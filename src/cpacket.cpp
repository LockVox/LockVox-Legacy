#include "src/includes/cpacket.h"

CPacket::CPacket()
{
    header[0] = header[1] = 0xFF;
}

char SetType(QString m_type)
{
    if(m_type.compare("Server"), Qt::CaseInsensitive)
    {
        return 0x00;
    }
    if(m_type.compare("Channel"), Qt::CaseInsensitive)
    {
        return 0x01;
    }
    if(m_type.compare("Client"), Qt::CaseInsensitive)
    {
        return 0x02;
    }
}

char ServerAction(QString m_action)
{
    if(m_action.compare("Connect"), Qt::CaseInsensitive)

    if(m_action.compare("Disconnect"), Qt::CaseInsensitive);
    if(m_action.compare("Nickname"), Qt::CaseInsensitive);
    if(m_action.compare("Description"), Qt::CaseInsensitive);
    if(m_action.compare("Ban"), Qt::CaseInsensitive);
    if(m_action.compare("IPBan"), Qt::CaseInsensitive);
    if(m_action.compare("Kick"), Qt::CaseInsensitive);
    return 0x00;
}
char ChannelAction(QString m_action)
{
    return 0x00;
}

char UserAction(QString m_action)
{
    return 0x00;
}
