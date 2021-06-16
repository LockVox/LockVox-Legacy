/**
 * @file        cdatabase.h
 * @brief       Declaration of the class CDatabase
 * @details     This class represent an user
 * @author      Valentin Monnot (LockVox Team)
 * @version     0.2.0
 * @date        2021
 * @copyright   GNU GPL-3.0
 */

#ifndef CDATABASE_H
#define CDATABASE_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <QApplication>

#include "mysql.h"
#include "sha256.h"

#include "cchannel.h"
#include "cclient.h"

using namespace std;

class CChannel;

class CDatabase : public QThread
{
    /**
     * @class       CDatabase cdatabase.h "cdatabase.h"
     * @brief       SQL Database Class
     * @details     This class represente the main database (MySQL/MariaDB) and its function using mariadb connector lib
     * @author      Valentin Monnot (LockVox Team)
     * @version     0.2.0
     * @date        2021
     * @note
     * @bug
     * @warning
     * @remark
     * @copyright   GNU GPL-3.0
     */

    Q_OBJECT
    public:
        CDatabase();           // Constructor
        QString init();

        string getHash(string id, QString * get_err);
        QString newUser(string uuid,string pseudo, string mail, string password);
        QString updateUser(string uuid, string pseudo, string mail, string description);
        QString updateDescription(string uuid, string description);
        QString updateUsername(string uuid, string username);
        QString changePassword(string mail, string uuid, string password, string newPass);
        QString updateChannel(string id, string name, string maxuser);
        QString deleteUser(string uuid);
        QString deleteChannel(string id);


        QList<CChannel*> parseChannel();
        QList<CClient*> parseClient();
        CClient* parseClient(string email, QString * get_err);

    private:
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
};

#endif // CDATABASE_H
