#ifndef QXMPP_CLIENT_H
#define QXMPP_CLIENT_H

#include "QXmppClient.h"

class echoClient : public QXmppClient
{
    Q_OBJECT

public:
    echoClient(QObject *parent = nullptr);
    ~echoClient() override;

public slots:
    void messageReceived(const QXmppMessage &);
};

int startClient(int argc, char * argv[]);
#endif
