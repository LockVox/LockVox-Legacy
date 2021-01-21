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

/*
 * Main Process
 */
bool CDatabase::execMain() //test connexion bdd
{
    try {
        // Format a MySQL object
        conn = mysql_init(NULL);

        // Establish a MySQL connection
        if (!mysql_real_connect(conn,MY_HOSTNAME, MY_USERNAME,MY_PASSWORD, MY_DATABASE,MY_PORT_NO, MY_SOCKET, MY_OPT))
        {
            cerr << mysql_error(conn) << endl;
            return false;
        }

        // Execute a sql statement
        if (mysql_query(conn, "SHOW TABLES"))
        {
            cerr << mysql_error(conn) << endl;
            return false;
        }

        // Get a result set
        res = mysql_use_result(conn);

        // Fetch a result set
        while ((row = mysql_fetch_row(res)) != NULL)
            cout << row[0] << endl;

        // Release memories
        mysql_free_result(res);

        // Close a MySQL connection
        mysql_close(conn);
    } catch (char *e)
    {
        cerr << "[EXCEPTION] " << e << endl;
        return false;
    }
    return true;
}

string CDatabase::getHash(string mail)
{
    string hash;
    try {
        // Format a MySQL object
        conn = mysql_init(NULL);

        // Establish a MySQL connection
        if (!mysql_real_connect(conn,MY_HOSTNAME, MY_USERNAME,MY_PASSWORD, MY_DATABASE,MY_PORT_NO, MY_SOCKET, MY_OPT))
        {
            cerr << mysql_error(conn) << endl;
            return "false";
        }

        // Execute a sql statement

        string query = "SELECT password FROM utilisateurs WHERE username = \"";

        query += mail;
        query += "\";";


        if (mysql_query(conn, query.c_str()))
        {
            cerr << mysql_error(conn) << endl;
            return "false";
        }

        // Get a result set
        res = mysql_use_result(conn);
        //hash += row[0];
        while ((row = mysql_fetch_row(res)))
            hash+=row[0];

        // Fetch a result set



        // Release memories
        mysql_free_result(res);

        // Close a MySQL connection
        mysql_close(conn);
    } catch (char *e)
    {
        cerr << "[EXCEPTION] " << e << endl;
        return "false";
    }
    qDebug() << QString::fromStdString(hash);
    return hash;
}

string CDatabase::newUser(string pseudo, string mail, string password)
{
    try {
        string existMail;
        // Format a MySQL object
        conn = mysql_init(NULL);

        // Establish a MySQL connection
        if (!mysql_real_connect(conn,MY_HOSTNAME, MY_USERNAME,MY_PASSWORD, MY_DATABASE,MY_PORT_NO, MY_SOCKET, MY_OPT))
        {
            cerr << mysql_error(conn) << endl;
            return "sqlconn";
        }

        // Execute a sql statement

        string query = "SELECT mail FROM utilisateurs WHERE mail = '";

        query += mail + "';";

        qDebug() << QString::fromStdString(query);

        if (mysql_query(conn, query.c_str()))
        {
            cerr << mysql_error(conn) << endl;
            return "sqlquery";
        }

        // Get a result set
        res = mysql_use_result(conn);

        // Fetch a result set
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            existMail += row[0];
        }

        if(existMail == mail)
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
            query = "INSERT INTO utilisateurs (username,password,mail) VALUES ('";
            query += pseudo + "','" + hashed + "','" + mail + "');";

            if (mysql_query(conn, query.c_str()))
            {
                cerr << mysql_error(conn) << endl;
                return "sqlquery";
            }
            // Release memories
            mysql_free_result(res);

            // Close a MySQL connection
            mysql_close(conn);

            return "succes";
        }
    } catch (char *e)
    {
        cerr << "[EXCEPTION] " << e << endl;
        return "false";
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
    int id;

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
           id = atoi(tmp);

           QString name(row[1]);

           CClient* client = new CClient(id,name,NULL,-1,false, "");

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

CClient* CDatabase::parseClient(string email)
{
    CClient* client;
    string query = "SELECT * FROM utilisateurs WHERE username = '";
    query += email + "'";
    char* tmp =0;
    int id;

    try {

        // Format a MySQL object
        conn = mysql_init(NULL);

        // Establish a MySQL connection
        if (!mysql_real_connect(conn,MY_HOSTNAME, MY_USERNAME,
                                MY_PASSWORD, MY_DATABASE,
                                MY_PORT_NO, MY_SOCKET, MY_OPT))     //No connection
        {
            cerr << mysql_error(conn) << endl;
            client = NULL;
            return client;
        }
       ///////////////////////

        if(mysql_query(conn, query.c_str()))        //Bad query
        {
            cerr << mysql_error(conn) << endl;
            client = NULL;
            return client;
        }

        // Get result
        res = mysql_use_result(conn);

        // Fetch a result set
       while((row = mysql_fetch_row(res)) != NULL)
       {


           tmp = (char*)row[0];
           qDebug() << tmp;
           id = atoi(tmp);

           QString name(row[1]);
           qDebug() << name;

           client = new CClient(id,name,NULL,-1,false, ""); //A rework pour la description
       }

        // Release memories
        mysql_free_result(res);

        // Close a MySQL connection
        mysql_close(conn);

        return client;

        //////////////////////
    }
        catch (char *e)
        {
            cerr << "[EXCEPTION] " << e << endl;
            client = NULL;
            return client;
            //Il faudra check la taille a la sortie pour un return NULL
        }
}
