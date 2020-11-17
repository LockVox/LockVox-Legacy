#include "qxmpp_server.h"
#include <QCoreApplication>

#define USERNAME "lockvox"
#define PASSWORD "azerty"

class passwordChecker : public QXmppPasswordChecker
{
    /// Retrieves the password for the given username.
    QXmppPasswordReply::Error getPassword(const QXmppPasswordRequest &request, QString &password) override
    {
        if (request.username() == USERNAME) {
            password = PASSWORD;
            return QXmppPasswordReply::NoError;
        } else {
            return QXmppPasswordReply::AuthorizationError;
        }
    };

    /// Returns true as we implemented getPassword().
    bool hasGetPassword() const override
    {
        return true;
    };
};

int startServer(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    printf("Server start...\n");

    const QString domain = QString::fromLocal8Bit("192.168.1.40");
    QXmppLogger logger;
    logger.setLoggingType(QXmppLogger::StdoutLogging);

    printf("Logger created...\n");
    passwordChecker checker;

    QXmppServer server;
    server.setDomain(domain);
    server.setLogger(&logger);
    server.setPasswordChecker(&checker);
    server.listenForClients();
    server.listenForServers();

    return app.exec();
}
