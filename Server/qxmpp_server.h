#ifndef QXMPP_SERVER_H
#define QXMPP_SERVER_H

#include <QThread>
#include <QString>

#include "QXmppLogger.h"
#include "QXmppPasswordChecker.h"
#include "QXmppServer.h"
#include "qxmpp_password_checker.h"

class qxmpp_server
{
    public:

        qxmpp_server(QString domain);
        ~qxmpp_server();

        bool startServer();

    private:
        QString domain;
        QXmppLogger logger;
        QXmppServer server;
        passwordChecker checker;

};

#endif // QXMPP_SERVER_H
