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
     * @note        db.cnf in "%LOCKVOX_SERVER_ROOT_DIR%/storage/conf/" use 4 parameter, all of them should be there even if not used ! \n Order is important !
     *              If you didn't use one of the parameter, default value will be applied, except for MY_PASSWORD. For user security you should never use a database without password... \n
     *              Exemple with default values : \n MY_HOSTNAME=127.0.0.1 \n MY_DATABASE=lockvox_server \n MY_USERNAME=lockvox \n MY_PASSWORD=changeme \n
     *              If no configuration file found when starting the server, it will automaticcaly create a db.cnf file with these default values. After what server will close
     *              telling you that its default values and you should change them.
     * @bug         In db configuration file, if MY_HOSTNAME=domainName.something MariaDB connector didn't recognize it as an address and use your ip address, i don't f***ing know why.
     *              All work great if MY_HOSTNAME is hardcoded or if you use a standard IPV4 address in configuration file
     * @warning     DEV_DB is our MariaDB database used for testing purpose, only fake account with troll username/password should be used on this database as everyone with an
     *              internet connection could access it ! if you just want to test, use account "test" with password "test".
     *              If you want to use your own MariaDB/MySQL database, we advise you to NOT hardcode your database credential.
     *              You should go in "config.h" and comment #define DEV_DB so you can use db.cnf file to configure your own DB.
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
