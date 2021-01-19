#include "qxmpp_client.h"

qxmpp_client::qxmpp_client(QString mail, QString password, QString domain)
{
    client.logger()->setLoggingType(QXmppLogger::StdoutLogging);
    config.setDomain(domain);
    config.setPassword(password);
    config.setUser(mail);
}

bool qxmpp_client::connectToServer()
{
    client.connectToServer(config);
    return client.isConnected();
}
