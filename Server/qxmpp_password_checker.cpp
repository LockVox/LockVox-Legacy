#include "qxmpp_password_checker.h"

#define PASSWORD "azerty"
#define USERNAME "lockvox"

QXmppPasswordReply::Error passwordChecker::getPassword(const QXmppPasswordRequest &request, QString &password)
{
    string hash;
    try
    {
        CDatabase bdd;
        bool res = bdd.execMain();
        if(!res)
        {
            cout << "Can't connect to database !" << endl;
        }
        hash = bdd.getHash(request.username().toStdString());

        if(hash == "")
        {
            cout << "Identifiant incorrect" << endl;
            return QXmppPasswordReply::AuthorizationError;
        }

        else
        {
            if (hash == "badquery")
            {
                cout << "Erreur de requete SQL" << endl;
                return QXmppPasswordReply::AuthorizationError;
            }
        }
    } catch (char *e)
    {
        cerr << "[EXCEPTION]" << e << endl;
        return QXmppPasswordReply::AuthorizationError;
    }
    string hashed = "filsdepute";
    if(hashed == hash)
    {
        return QXmppPasswordReply::NoError;
    }

    else
    {
        return QXmppPasswordReply::AuthorizationError;
    }
};

