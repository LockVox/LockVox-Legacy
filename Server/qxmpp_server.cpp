#include <QThread>
#include <QString>

#include "qxmpp_server.h"


#define USERNAME "lockvox"
#define PASSWORD "azerty"

qxmpp_server::qxmpp_server(QString domain)
{
    this->domain = domain;
    logger.setLoggingType(QXmppLogger::StdoutLogging);
    server.setDomain(this->domain);
    server.setLogger(&logger);
    server.setPasswordChecker(&checker);
}

bool qxmpp_server::startServer()
{
    printf("Starting QXmpp server...\n");
    server.listenForClients();
    bool started = server.listenForServers();

    if(started == false)
    {
        printf("It seems like QXmpp server dosen't want to start...\n");
        return false;
    }

    else
    {
        printf("QXmpp server started\n");
        return true;
    }
}

