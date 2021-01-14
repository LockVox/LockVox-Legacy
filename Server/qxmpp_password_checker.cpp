#include "qxmpp_password_checker.h"

#include <openssl/sha.h>
#include <iostream>
#include <iomanip>
#include <sstream>

#define PASSWORD "azerty"
#define USERNAME "lockvox"

Proc::Proc()
{
    // Initialize constants
    MY_HOSTNAME = "lockvox.fr";
    MY_DATABASE = "lockvox_server";
    MY_USERNAME = "lockvox";
    MY_PASSWORD = "4E96up6E3jxsX6QR";
    MY_SOCKET   = NULL;

}

/*
 * Main Process
 */
bool Proc::execMain()
{
    try {
        // Format a MySQL object
        conn = mysql_init(NULL);

        // Establish a MySQL connection
        if (!mysql_real_connect(
                conn,
                MY_HOSTNAME, MY_USERNAME,
                MY_PASSWORD, MY_DATABASE,
                MY_PORT_NO, MY_SOCKET, MY_OPT)) {
            cerr << mysql_error(conn) << endl;
            return false;
        }

        // Execute a sql statement
        if (mysql_query(conn, "SHOW TABLES")) {
            cerr << mysql_error(conn) << endl;
            return false;
        }

        // Get a result set
        res = mysql_use_result(conn);

        // Fetch a result set
        cout << "* MySQL - SHOW TABLES in `"
             << MY_DATABASE << "`" << endl;
        while ((row = mysql_fetch_row(res)) != NULL)
            cout << row[0] << endl;

        // Release memories
        mysql_free_result(res);

        // Close a MySQL connection
        mysql_close(conn);
    } catch (char *e) {
        cerr << "[EXCEPTION] " << e << endl;
        return false;
    }
    return true;
}

string Proc::getHash(string id)
{
    string tmpquery = "SELECT password FROM utilisateurs WHERE mail = '";

    tmpquery += id;
    tmpquery += "';";

    const char * query = tmpquery.c_str();
    if (mysql_query(conn,query))
    {
        cerr << mysql_error(conn) << endl;
        return "badquery";
    }

    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);

    string hash(*row);

    return hash;
}


QXmppPasswordReply::Error passwordChecker::checkpasswd(const QXmppPasswordRequest &request, string &password)
{
    string hash;
    try
    {
        Proc main;
        bool res = main.execMain();
        if(!res)
        {
            cout << "Can't connect to database !" << endl;
        }
        hash = main.getHash(request.username().toStdString());

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

    unsigned char hashed[SHA256_DIGEST_LENGTH];
    const char * passwd = password.c_str();

    SHA256_CTX sha256;

    SHA256_Init(&sha256);
    SHA256_Update(&sha256,passwd,strlen(passwd));
    SHA256_Final(hashed, &sha256);
};

