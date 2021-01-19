#ifndef CPACKET_H
#define CPACKET_H

#include <QString>

#include "src/includes/cchannel.h"
#include "src/includes/cclient.h"

class CPacket
{
public:
    CPacket();
    char SetType(QString m_type);
    char ServerAction(QString m_action);
    char ChannelAction(QString m_action);
    char UserAction(QString m_action);

    enum Type {Server,
              Channel,
              Client};
    enum ActServer{Serv_Connect,
                   Serv_Disconnect,
                   Serv_Nickname,
                   Serv_Description,
                   Serv_Ban,
                   Serv_IPBan,
                   Serv_Kick};
    enum ActChannel{Chan_Connect,
                    Chan_Disconnect,
                    Chan_Send};

private:
    char header[2];

};

#endif // CPACKET_H
