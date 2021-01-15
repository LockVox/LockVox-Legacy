#include "qxmpp_password_checker.h"

#define PASSWORD "azerty"
#define USERNAME "lockvox"

QXmppPasswordReply::Error passwordChecker::getPassword(const QXmppPasswordRequest &request, QString &password)
{
    CDatabase bdd;
    string hash = bdd.getHash(request.username().toStdString());

    if (hash == "false")
    {
        cout << "Erreur de requete SQL" << endl;
        return QXmppPasswordReply::AuthorizationError;
    }

    string hashed = sha256(password.toStdString());
    if(hashed == hash)
    {
        return QXmppPasswordReply::NoError;
    }

    else
    {
        return QXmppPasswordReply::AuthorizationError;

    }
};

