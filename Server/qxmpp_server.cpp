#include "src/includes/qxmpp_server.h"

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

int startServer()
{
    printf("Server start...\n");

    const QString domain = QString::fromLocal8Bit("127.0.0.1");
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

    return 0;
}
