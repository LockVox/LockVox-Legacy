#include "qxmpp_client.h"
#include "QXmppLogger.h"
#include "QXmppMessage.h"
#include <QCoreApplication>

echoClient::echoClient(QObject *parent)
    : QXmppClient(parent)
{
    connect(this, &QXmppClient::messageReceived, this, &echoClient::messageReceived);
}

echoClient::~echoClient()
{
}

void echoClient::messageReceived(const QXmppMessage &message)
{
    QString from = message.from();
    QString msg = message.body();

    sendPacket(QXmppMessage("Lol ?", from, "Your message: " + msg));
}

int startClient(int argc, char * argv[])
{
    QCoreApplication app(argc, argv);
    echoClient client;
    client.logger()->setLoggingType(QXmppLogger::StdoutLogging);
    client.connectToServer("lockvox@192.168.1.40", "azerty");

    return app.exec();
}
