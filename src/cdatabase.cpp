#include "src/includes/cdatabase.h"

CDatabase::CDatabase()
{
    // Initialize constants
    MY_HOSTNAME = "lockvox.fr";
    MY_DATABASE = "lockvox_server";
    MY_USERNAME = "lockvox";
    MY_PASSWORD = "4E96up6E3jxsX6QR";
    MY_SOCKET   = NULL;

}

string CDatabase::getHash(string mail, QString * get_err)
{
    string hash;
    try {
        // Format a MySQL object
        conn = mysql_init(NULL);

        // Establish a MySQL connection
        if (!mysql_real_connect(conn,MY_HOSTNAME, MY_USERNAME,MY_PASSWORD, MY_DATABASE,MY_PORT_NO, MY_SOCKET, MY_OPT))
        {
            *get_err = QString::fromLocal8Bit(mysql_error(conn));
            return "false";
        }

        // Execute a sql statement

        string query = "SELECT password FROM utilisateurs WHERE mail = \"";

        query += mail;
        query += "\";";


        if (mysql_query(conn, query.c_str()))
        {
            *get_err = QString::fromLocal8Bit(mysql_error(conn));
            mysql_close(conn);
            return "false";
        }

        // Get a result set
        res = mysql_use_result(conn);

        // Fetch a result set
        row = mysql_fetch_row(res);
        hash+=row[0];

        // Release memories
        mysql_free_result(res);

        // Close a MySQL connection
        mysql_close(conn);
    } catch (char *e)
    {
        *get_err = QString::fromLocal8Bit(e);
        return "false";
    }
    return hash;
}

QString CDatabase::newUser(string uuid, string pseudo, string mail, string password)
{
    try {
        QString error;
        string existMail;
        // Format a MySQL object
        conn = mysql_init(NULL);

        // Establish a MySQL connection
        if (!mysql_real_connect(conn,MY_HOSTNAME, MY_USERNAME,MY_PASSWORD, MY_DATABASE,MY_PORT_NO, MY_SOCKET, MY_OPT))
        {
            error = QString::fromLocal8Bit(mysql_error(conn));
            return error;
        }

        // Execute a sql statement

        string query = "SELECT mail FROM utilisateurs WHERE mail = '";

        query += mail + "';";

        qDebug() << QString::fromStdString(query);

        if (mysql_query(conn, query.c_str()))
        {
            error = QString::fromLocal8Bit(mysql_error(conn));
            mysql_close(conn);
            return error;
        }

        // Get a result set
        res = mysql_use_result(conn);

        // Fetch a result set
        if((row = mysql_fetch_row(res)))
        {
            // Release memories
            mysql_free_result(res);

            // Close a MySQL connection
            mysql_close(conn);

            return "mailerror";
        }
        else
        {
            string hashed = sha256(password);

            query = "INSERT INTO utilisateurs (uuid,username,password,mail) VALUES ('";
            query += uuid + "','" + pseudo + "','" + hashed + "','" + mail + "');";

            if (mysql_query(conn, query.c_str()))
            {
                error = QString::fromLocal8Bit(mysql_error(conn));
                // Release memories
                mysql_free_result(res);

                // Close a MySQL connection
                mysql_close(conn);
                return error;
            }
            // Release memories
            mysql_free_result(res);

            // Close a MySQL connection
            mysql_close(conn);

            return "success";
        }
    } catch (char *e)
    {
        return QString::fromLocal8Bit(e);
    }
}


QList<CChannel*> CDatabase::parseChannel()
{
    QList<CChannel*> list_channel;
    string query = "SELECT * FROM channel";
    char* tmp =0;
    char* tmp2 = 0;
    int id, maxUsers;



    try {

        // Format a MySQL object
        conn = mysql_init(NULL);

        // Establish a MySQL connection
        if (!mysql_real_connect(conn,MY_HOSTNAME, MY_USERNAME,
                                MY_PASSWORD, MY_DATABASE,
                                MY_PORT_NO, MY_SOCKET, MY_OPT))
        {
            cerr << mysql_error(conn) << endl;
            list_channel.clear();
            list_channel.push_back(NULL);
            return list_channel;
        }
       ///////////////////////

        if(mysql_query(conn, query.c_str()))
        {
            cerr << mysql_error(conn) << endl;
            list_channel.clear();
            list_channel.push_back(NULL);
            return list_channel;
        }

// Get and fill Channel list data
        // Get a result set
        res = mysql_use_result(conn);

        // Fetch a result set
       while((row = mysql_fetch_row(res)) != NULL)
       {
           string r = row[0];

           tmp = (char*)row[0];
           id = atoi(tmp);

           tmp2 = (char*)row[3];
           maxUsers = atoi(tmp2);

           QString name(row[1]);

           CChannel* channel = new CChannel( name,id, maxUsers );

           list_channel.push_back(channel);
       }

        // Release memories
        mysql_free_result(res);

        // Close a MySQL connection
        mysql_close(conn);

        return list_channel;

        //////////////////////
    }
        catch (char *e)
        {
            cerr << "[EXCEPTION] " << e << endl;
            list_channel.clear();
            list_channel.push_back(NULL);
            return list_channel;
            //Il faudra check la taille a la sortie pour un return NULL
        }
}

QList<CClient*> CDatabase::parseClient()
{
    QList<CClient*> list_client;
    string query = "SELECT * FROM utilisateurs";
    char* tmp =0;
    try {

        // Format a MySQL object
        conn = mysql_init(NULL);

        // Establish a MySQL connection
        if (!mysql_real_connect(conn,MY_HOSTNAME, MY_USERNAME,
                                MY_PASSWORD, MY_DATABASE,
                                MY_PORT_NO, MY_SOCKET, MY_OPT))
        {
            cerr << mysql_error(conn) << endl;
            list_client.clear();
            list_client.push_back(NULL);
            return list_client;
        }
       ///////////////////////

        if(mysql_query(conn, query.c_str()))
        {
            cerr << mysql_error(conn) << endl;
            list_client.clear();
            list_client.push_back(NULL);
            return list_client;
        }

// Get and fill Channel list data
        // Get a result set
        res = mysql_use_result(conn);

        // Fetch a result set
       while((row = mysql_fetch_row(res)) != NULL)
       {
           tmp = (char*)row[0];
           QUuid uuid = QUuid::fromString(QString::fromStdString(tmp));

           QString name(row[1]);

           CClient* client = new CClient(uuid,name,NULL,-1,false, "");

           list_client.push_back(client);
       }

        // Release memories
        mysql_free_result(res);

        // Close a MySQL connection
        mysql_close(conn);

        return list_client;

        //////////////////////
    }
        catch (char *e)
        {
            cerr << "[EXCEPTION] " << e << endl;
            list_client.clear();
            list_client.push_back(NULL);
            return list_client;
            //Il faudra check la taille a la sortie pour un return NULL
        }
}

CClient* CDatabase::parseClient(string email, QString * get_err)
{
    CClient* client;
    string query = "SELECT * FROM utilisateurs WHERE mail = '";
    query += email + "'";
    char* tmp =0;
    try {

        // Format a MySQL object
        conn = mysql_init(NULL);

        // Establish a MySQL connection
        if (!mysql_real_connect(conn,MY_HOSTNAME, MY_USERNAME,MY_PASSWORD, MY_DATABASE,MY_PORT_NO, MY_SOCKET, MY_OPT))     //No connection
        {
            *get_err = QString::fromLocal8Bit(mysql_error(conn));
            client = NULL;
            return client;
        }

        if(mysql_query(conn, query.c_str()))        //Bad query
        {
            *get_err = QString::fromLocal8Bit(mysql_error(conn));
            client = NULL;
            return client;
        }

        // Get result
        res = mysql_use_result(conn);

        // Fetch a result set

        if((row = mysql_fetch_row(res)))
        {
            QUuid uuid;
            QString name;
            tmp = (char*)row[0];
            uuid = QUuid::fromString(QString::fromStdString(tmp));
            name = row[1];
            client = new CClient(uuid,name,NULL,-1,false, ""); //A rework pour la description
        }
        else
        {
            client = NULL;
            *get_err =  "no_client";
        }

        // Release memories
        mysql_free_result(res);

        // Close a MySQL connection
        mysql_close(conn);

        return client;
    }
    catch (char *e)
    {
        *get_err = e;
        client = NULL;
        return client;
        //Il faudra check la taille a la sortie pour un return NULL
    }
}
