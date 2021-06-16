/**
 * @file        cclient.h
 * @brief       Declaration of the class CClient
 * @details     This class represent an user
 * @author      LockVox Team
 * @version     0.2.0
 * @date        2021
 * @copyright   GNU GPL-3.0
 */

#ifndef CCLIENT_H
#define CCLIENT_H

//QT Includes
#include <QString>
#include <QList>
#include <QDebug>
#include <QObject>
#include <QtNetwork>
#include <QUuid>
#include <QImage>

#include "cpacket.h"
#include "sha256.h"

class CMessage;
class CChannel;

#define SERVER 0
#define SERVER_WARN 1
#define SERVER_ERR 2
#define DB_ERR 3

using namespace std;

class CClient : public QThread
{
    /**
     * @class       CClient cclient.h "cclient.h"
     * @brief       Declaration of the class CClient
     * @details     This class represent an user
     * @author      LockVox Team
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

        //Constructor
        CClient();
        CClient(const CClient & copy);
        CClient(QUuid id, QString name, QTcpSocket * soc, int idChannel, bool online, QString description);
        CClient(QTcpSocket * soc);
        ~CClient();

        //Getters
        QString get_pseudo();
        QString get_mail();
        QString get_description();
        QTcpSocket * get_socket();
        int get_idChannel();
        bool get_isOnline();
        bool get_isAuthenticate();
        QUuid get_uuid();
        QImage get_profilePic();

        //Setters
        void set_pseudo(QString pseudo);
        void set_mail(QString mail);
        void set_socket(QTcpSocket * soc);
        void set_idChannel(int id);
        void set_all(CClient *c);
        void set_isOnline(bool online);
        void set_isAuthenticate(bool Auth);
        void set_description(QString d);
        void set_uuid(QUuid uuid);
        void set_profilePic(QImage img);

        //Serialize | Deserialize
        QJsonObject serializeToObj();
        void deserialize(QJsonObject json_obj);

        //Process
        void processData(QByteArray data);

        //Messages
        bool createChannelIndex(string filename, QString path_to_index); //Creates an index.json for an empty channel and add the first message of it
        QList<QString> readChannelIndex(QString path_to_index); //Returns the list of names of all messages files contained in the given index
        bool insertChannelIndex(QString path_to_index, QList<QString> filename_list);  //Update index.json when inserting new message to it
        QList<CMessage> createMessageList(QString id, bool isPrivate,int nb_msg_to_sync, QUuid sender, int start_index); //Creates a QList of CMessage stored localy using index.json

    signals:
        void writeToLog(QString error, int level);
        void sendYou(QTcpSocket * socket);

        void updateMe(int update_type, CClient * client);
        void updateChan(int update_type, CChannel * channel);

        void authMe(QList<QString> info, CClient * client);
        void regMe(QList<QString> info, CClient * Client);

        CChannel * whichChan(int id);

        void sendToAll(QByteArray out);

    private slots:
        void onReceiveData();
        void sendToClient(QByteArray out);


    private:

        QString m_pseudo;                   //!< m_pseudo is the username of the user
        QString m_mail;                     //!< m_mail is the email of the user
        QString m_description;              //!< m_description is the description of the user

        QTcpSocket * m_soc;                 //!< m_socket is the TCP Socket of the user

        QUuid m_uuid;                       //!< m_uuid is the UUID of the user
        int m_idChannel;                    //!< m_idChannel is the ID of the current channel of the user

        bool m_isOnline;                    //!< m_isOnline represent if the user is online or not
        bool m_isAuthenticate;              //!< m_isAuthenticate represent if the user has pass the authentification

        QImage profilePic;                  //!< profilePic is the profile picture of the user

        QByteArray * m_buffer;

};

#endif // CCLIENT_H
