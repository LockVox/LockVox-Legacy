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

QXmppPasswordReply *passwordChecker::checkPassword(const QXmppPasswordRequest &request)
{
    auto *reply = new QXmppPasswordReply;

    QString secret;
    QXmppPasswordReply::Error error = getPassword(request, secret);
    if (error == QXmppPasswordReply::NoError)
    {
        if (sha256(request.password().toStdString()) != secret.toStdString())
        {
            reply->setError(QXmppPasswordReply::AuthorizationError);
        }
    }
    else
    {
        reply->setError(error);
    }

    // reply is finished
    reply->finishLater();
    return reply;
}

