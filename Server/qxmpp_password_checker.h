#ifndef QXMPP_PASSWORDCHECKER_H
#define QXMPP_PASSWORDCHECKER_H

#include "QXmppPasswordChecker.h"


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
