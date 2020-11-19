#include "qxmpp_password_checker.h"

#define PASSWORD "azerty"
#define USERNAME "lockvox"

QXmppPasswordReply::Error passwordChecker::getPassword(const QXmppPasswordRequest &request, QString &password)
{
    if (request.username() == USERNAME)
    {
        password = PASSWORD;
        return QXmppPasswordReply::NoError;
    }

    else
    {
        return QXmppPasswordReply::AuthorizationError;
    }
};
