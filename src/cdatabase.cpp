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

string CDatabase::getHash(string id)
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

        string query = "SELECT password FROM utilisateurs WHERE mail = '";

        query += id;
        query += "';";

        if (mysql_query(conn, query.c_str()))
        {
            cerr << mysql_error(conn) << endl;
            return "false";
        }

        // Get a result set
        res = mysql_use_result(conn);

        // Fetch a result set
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            hash += row[0];
        }

        // Release memories
        mysql_free_result(res);

        // Close a MySQL connection
        mysql_close(conn);
    } catch (char *e)
    {
        cerr << "[EXCEPTION] " << e << endl;
        return "false";
    }
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
