#include "qxmpp_password_checker.h"

QXmppPasswordReply::Error passwordChecker::getPassword(const QXmppPasswordRequest &request, QString &password)
{
    CDatabase bdd;
    string hash = bdd.getHash(request.username().toStdString());
    if (hash == "false")
    {
        cout << "Erreur de requete SQL" << endl;
        return QXmppPasswordReply::AuthorizationError;
    }

    QString tmp = QString::fromStdString(hash);
    password = tmp;
    return QXmppPasswordReply::NoError;

};

