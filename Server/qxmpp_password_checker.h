#ifndef QXMPP_PASSWORDCHECKER_H
#define QXMPP_PASSWORDCHECKER_H

#include "QXmppPasswordChecker.h"
#include "mysql.h"
#include "includes/cdatabase.h"
#include "sha256.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

class passwordChecker : public QXmppPasswordChecker
{

    /// Retrieves the password for the given username.
    QXmppPasswordReply::Error getPassword(const QXmppPasswordRequest &request, QString &password) override;
    QXmppPasswordReply *checkPassword(const QXmppPasswordRequest &request) override;


    /// Returns true as we implemented getPassword().
    bool hasGetPassword() const override
    {
        return true;
    };
};

#endif // QXMPP_PASSWORDCHECKER_H
