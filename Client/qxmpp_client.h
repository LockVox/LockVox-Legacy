#ifndef QXMPP_CLIENT_H
#define QXMPP_CLIENT_H

#include "QXmppClient.h"
#include "QXmppLogger.h"
#include "QXmppConfiguration.h"

#include <QString>

using namespace std;

class qxmpp_client
{
    public:
        qxmpp_client(QString mail, QString password, QString domain);
        ~qxmpp_client();
        bool connectToServer();

    private:
        QXmppClient client;
        QXmppConfiguration config;

};

#endif // QXMPP_CLIENT_H
