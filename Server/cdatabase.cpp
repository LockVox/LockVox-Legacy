#include "cdatabase.h"
#include "Server/includes/config.h"

CDatabase::CDatabase()
{
    // Initialize constants
    MY_SOCKET   = NULL;
}

QString CDatabase::init(QString username, QString password, int port, QString hostname, QString database)
{
#ifdef DEV_DB
    MY_HOSTNAME = "lockvox.fr";
    MY_DATABASE = "lockvox_server";
    MY_USERNAME = "lockvox";
    MY_PASSWORD = "4E96up6E3jxsX6QR";
#else
    //This doesn't work
    QList<QByteArray> params;
    params.append(hostname.toUtf8().toBase64());
    params.append(database.toUtf8().toBase64());
    params.append(username.toUtf8().toBase64());
    params.append(password.toUtf8().toBase64());

    MY_HOSTNAME=params[0];
    MY_DATABASE=params[1];
    MY_USERNAME=params[2];
    MY_PASSWORD=params[3];
    /*
    QDir test;
    if(!test.exists("storage/conf"))
    {
        if(!test.mkpath("storage/conf"))
        {
            qDebug() << "[Conf error] Can't create conf directory" << Qt::endl;
            abort();
        }
    }
    else
    {
        if(!QFile::exists("storage/conf/db.cnf"))
        {
            QFile cnf("storage/conf/db.cnf");
            if(!cnf.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                qDebug() << "Can't create default DB configuration file" << Qt::endl;
                abort();
            }
            QTextStream stream(&cnf);

            stream << "MY_HOSTNAME=127.0.0.1" << Qt::endl;
            stream << "MY_DATABASE=lockvox_server" << Qt::endl;
            stream << "MY_USERNAME=lockvox" << Qt::endl;
            stream << "MY_PASSWORD=changeme" << Qt::endl;

            cnf.close();

            qDebug() << "No DB configuration file was found, I generate you the default one." << Qt::endl << "Please go to $LOCKVOX_SERVER_PATH/storage/conf and modify db.cnf with your db credential" << Qt::endl;
            abort();
        }
        else
        {
            QFile cnf("storage/conf/db.cnf");
            if(!cnf.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                qDebug() << "Can't open DB configuration file" << Qt::endl;
                abort();
            }
            QByteArray db_cnf = cnf.readAll();
            QList<QByteArray> cnf_list = db_cnf.split('\n');

            for(int i = 0; i < 4; i++)
            {
                if(!cnf_list[i].isNull())
                {
                    QList<QByteArray> tmp = cnf_list[i].split('=');
                    switch (i)
                    {
                        case 0:
                        {
                            if(tmp[0] == "MY_HOSTNAME" && !tmp[1].isEmpty())
                            {
                                MY_HOSTNAME = tmp[1].toStdString().c_str();
                            }
                            else
                            {
                                if(tmp[1].isEmpty())
                                {
                                    qDebug() << "No hostname set for MY_HOSTNAME, localhost will be applied by default";
                                    MY_HOSTNAME = "127.0.0.1";
                                }
                                else
                                {
                                    qDebug() << "Invalide first parameter in DB configuration file. Should be MY_HOSTNAME";
                                    abort();
                                }
                            }
                            break;
                        }
                        case 1:
                        {
                           if(tmp[0] == "MY_DATABASE" && !tmp[1].isEmpty())
                           {
                               MY_DATABASE = tmp[1].toStdString().c_str();
                           }
                           else
                           {
                               if(tmp[1].isEmpty())
                               {
                                   qDebug() << "No DB set for MY_DATABASE, lockvox_server will be applied by default";
                                   MY_DATABASE = "lockvox_server";
                               }
                               else
                               {
                                   qDebug() << "Invalide seconde parameter in DB configuration file. Should be MY_DATABASE";
                                   abort();
                               }
                           }
                           break;
                        }
                        case 2:
                        {
                           if(tmp[0] == "MY_USERNAME" && !tmp[1].isEmpty())
                           {
                               MY_USERNAME = tmp[1].toStdString().c_str();
                           }
                           else
                           {
                               if(tmp[1].isEmpty())
                               {
                                   qDebug() << "No username set for MY_USERNAME, lockvox will be applied by default";
                                   MY_USERNAME = "lockvox";
                               }
                               else
                               {
                                   qDebug() << "Invalide third parameter in DB configuration file. Should be MY_USERNAME";
                                   abort();
                               }
                           }
                           break;
                        }
                        case 3:
                        {
                           if(tmp[0] == "MY_PASSWORD" && !tmp[1].isEmpty())
                           {
                               MY_PASSWORD = tmp[1].toStdString().c_str();
                           }
                           else
                           {
                               if(tmp[1].isEmpty())
                               {
                                   qDebug() << "No password set for MY_PASSWORD, please set your MY_DB password";
                                   abort();
                               }
                               else
                               {
                                   qDebug() << "Invalide fourth parameter in DB configuration file. Should be MY_PASSWORD";
                                   abort();
                               }
                           }
                           break;
                        }
                        default:
                        {
                            qDebug() << "Too much parameters in db.cnf, ignoring the rest";
                            break;
                        }
                    }
                }
                else
                {
                    qDebug() << "Can't initialyse DB conf, 4 parameters required" << Qt::endl;
                    abort();
                }
            }
        }
    }*/
#endif

    try
    {
        conn = mysql_init(NULL);
        if (!mysql_real_connect(conn,MY_HOSTNAME, MY_USERNAME,MY_PASSWORD, MY_DATABASE,MY_PORT_NO, MY_SOCKET, MY_OPT))
        {
            mysql_close(conn);
            return QString::fromLocal8Bit(mysql_error(conn));
        }
        return "success";
    }
    catch (char *e)
    {
        qDebug() << QString::fromLocal8Bit(e);
    }
}

string CDatabase::getHash(string mail, QString * get_err)
{
    try
    {
        string hash;
        string query = "SELECT password FROM utilisateurs WHERE mail = \"";
        query += mail;
        query += "\";";

        if (mysql_query(conn, query.c_str()))
        {
            *get_err = QString::fromLocal8Bit(mysql_error(conn));
            return "false";
        }

        // Get a result set
        res = mysql_use_result(conn);

        // Fetch a result set
        row = mysql_fetch_row(res);
        hash+=row[0];

        // Release memories
        mysql_free_result(res);
        return hash;
    }
    catch (char *e)
    {
        *get_err = QString::fromLocal8Bit(e);
        return "false";
    }
}

QString CDatabase::newUser(string uuid, string pseudo, string mail, string password)
{
    try
    {
        QString error;
        string existMail;

        // Execute a sql statement

        string query = "SELECT mail FROM utilisateurs WHERE mail = '";

        query += mail + "';";

        qDebug() << QString::fromStdString(query);

        if (mysql_query(conn, query.c_str()))
        {
            error = QString::fromLocal8Bit(mysql_error(conn));
            return error;
        }

        // Get a result set
        res = mysql_use_result(conn);

        // Fetch a result set
        if((row = mysql_fetch_row(res)))
        {
            // Release memories
            mysql_free_result(res);
            return "mailerror";
        }
        else
        {
            string hashed = sha256(password);

            query = "INSERT INTO utilisateurs (uuid,username,password,mail,description) VALUES ('";
            query += uuid + "','" + pseudo + "','" + hashed + "','" + mail + "','This is a default description');";

            if (mysql_query(conn, query.c_str()))
            {
                error = QString::fromLocal8Bit(mysql_error(conn));
                // Release memories
                mysql_free_result(res);
                return error;
            }
            // Release memories
            mysql_free_result(res);
            return "success";
        }
    }
    catch (char *e)
    {
        return QString::fromLocal8Bit(e);
    }
}


QList<CChannel*> CDatabase::parseChannel()
{
    QList<CChannel*> list_channel;
    try
    {
        string query = "SELECT * FROM channel";
        char* tmp =0;
        char* tmp2 = 0;
        int id, maxUsers;

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
        return list_channel;
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
    try
    {
        string query = "SELECT * FROM utilisateurs";
        char* tmp =0;
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

            QString description(row[5]);

            CClient* client = new CClient(uuid,name,NULL,-1,false, description);
            client->start();

            list_client.push_back(client);
        }

        // Release memories
        mysql_free_result(res);
        return list_client;
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
    try {

        string query = "SELECT * FROM utilisateurs WHERE mail = '";
        query += email + "'";
        char* tmp =0;

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
        *get_err = "success";
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

QString CDatabase::updateUser(string uuid, string pseudo, string mail, string description)
{
    try
    {
        // Execute a sql statement
        string query = "UPDATE utilisateurs SET username = '" + pseudo + "', mail = '" + mail + "', description = '" + description + "' WHERE uuid = '" + uuid + "';";
        if (mysql_query(conn, query.c_str()))
        {
            return QString::fromLocal8Bit(mysql_error(conn));
        }
        return "success";
    }
    catch (char *e)
    {
        return QString::fromLocal8Bit(e);
    }
}

QString CDatabase::updateDescription(string uuid, string description)
{
    try
    {
        if(uuid =="" || description == "")
        {
            return "Missing paramater";
        }

        string query = "UPDATE utilisateurs SET description = '" + description + "' WHERE uuid = '" +  uuid + "';";
        if (mysql_query(conn, query.c_str()))
        {
            return QString::fromLocal8Bit(mysql_error(conn));
        }
        return "success";
    }
    catch (char *e)
    {
        return QString::fromLocal8Bit(e);
    }
}

QString CDatabase::updateUsername(string uuid, string username)
{
    try
    {
        if(uuid =="" || username == "")
        {
           return "Missing paramater";
        }

        string query = "UPDATE utilisateurs SET username = '" + username + "' WHERE uuid = '" +  uuid + "';";
        if (mysql_query(conn, query.c_str()))
        {
            return QString::fromLocal8Bit(mysql_error(conn));
        }
        return "success";
    }
    catch (char *e)
    {
        return QString::fromLocal8Bit(e);
    }
}

QString CDatabase::changePassword(string mail, string uuid, string password, string newPass)
{
    try
    {
        QString error;
        //Il doit rentrer son mdp aussi
        if(getHash(mail, &error) == sha256(password))   //On vérifie que l'ancien mdp est correct
        {
            string query = "UPDATE utilisateur SET password = '" + sha256(newPass) + "' WHERE uuid = " + uuid + ";";
            if(mysql_query(conn, query.c_str()))
            {
                return QString::fromLocal8Bit(mysql_error(conn));

            }
            return "success";
        }
        return "password"; //Bad password
    }
    catch (char *e)
    {
        return QString::fromLocal8Bit(e);
    }
}

QString CDatabase::updateChannel(string id, string name, string maxuser)
{
    try {
        // Execute a sql statement
        string query = "UPDATE channel SET name = '" + name + "', max_users = '" + maxuser + "' WHERE ID_channel = '" + id + "';";
        if (mysql_query(conn, query.c_str()))
        {
            return QString::fromLocal8Bit(mysql_error(conn));
        }
        return "success";
    }
    catch (char *e)
    {
        return QString::fromLocal8Bit(e);
    }
}

QString CDatabase::deleteUser(string uuid)
{
    try
    {
        QString error;
        error = QString::fromStdString(uuid); //Fuck the warning
        //TODO
    }
    catch (char *e)
    {
        return QString::fromLocal8Bit(e);
    }
    return "success";
}

QString CDatabase::deleteChannel(string id)
{
    try
    {
        string query = "DELETE FROM 'channel' WHERE uuid = '" +  id + "';";
        qDebug() <<"deleteUser request : " <<  QString::fromStdString(query);
        if (mysql_query(conn, query.c_str()))
        {
            return QString::fromLocal8Bit(mysql_error(conn));
        }
        return "success";
    }
    catch (char *e)
    {
        return QString::fromLocal8Bit(e);
    }
}
