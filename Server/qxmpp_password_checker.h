#ifndef QXMPP_PASSWORDCHECKER_H
#define QXMPP_PASSWORDCHECKER_H

#include "QXmppPasswordChecker.h"
#include "mysql.h"

#include <openssl/sha.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

class Proc
{
    const char* MY_HOSTNAME;
    const char* MY_DATABASE;
    const char* MY_USERNAME;
    const char* MY_PASSWORD;
    const char* MY_SOCKET;
    enum {
        MY_PORT_NO = 3306,
        MY_OPT     = 0
    };
    MYSQL     *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

public:
    Proc();           // Constructor
    bool execMain();  // Main Process
    string getHash(string id);
};

class passwordChecker : public QXmppPasswordChecker
{

    /// Retrieves the password for the given username.
    QXmppPasswordReply::Error getPassword(const QXmppPasswordRequest &request, QString &password) override;

    /// Returns true as we implemented getPassword().
    bool hasGetPassword() const override
    {
        return true;
    };
};

#endif // QXMPP_PASSWORDCHECKER_H
