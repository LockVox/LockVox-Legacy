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
#include "src/includes/LVObject.h"
#include "src/includes/csessioncookie.h"
#include "sha256.h"
#include "cpacketmanager.h"

class CMessage;
class CChannel;
class CSessionCookie;
class CPacket;
class CPacketManager;
struct sAuthentication;
struct sRegister;

#define USER 0
#define SERVER 1
#define SERVER_WARN 2
#define SERVER_ERR 3
#define DB_ERR 4

using namespace std;

enum ClientParams{
    USERNAME,
    DESCRIPTION,
    MAIL
};

enum ChannelParams;


class CClient : public CObject
{
    /**
     * @class       CClient cclient.h "cclient.h"
     * @brief       Declaration of the class CClient
     * @details     This class represent an user
     * @author      LockVox Team
     * @version     0.2.0
     * @date        2021
     * @note
     * @bug         QAbstractSocket tells us that waitForBytes written isn't allowed in unconnected state when client alt-f4. Mybe it comes from onDisconnect from CServer.
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


        //Virtual
        void fromJSON(QJsonObject obj);
        QJsonObject toJSON();


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
        CSessionCookie * getSessionCookie();
        CPacketManager *getPacketManager() const;
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
        void setSessionCookie(CSessionCookie *sessionCookie);
        void setPacketManager(CPacketManager *packetManager);


        //
        void writePacket(CPacket packet);


        //Process
        void processData(CPacket packet);

        //Messages
        bool createChannelIndex(string filename, QString path_to_index); //Creates an index.json for an empty channel and add the first message of it
        QList<QString> readChannelIndex(QString path_to_index); //Returns the list of names of all messages files contained in the given index
        bool insertChannelIndex(QString path_to_index, QList<QString> filename_list);  //Update index.json when inserting new message to it
        QList<CMessage> createMessageList(QString id, bool isPrivate,int nb_msg_to_sync, QUuid sender, int start_index); //Creates a QList of CMessage stored localy using index.json

        //static function


        //Operator
        bool operator==(const CClient &client) const;
        bool operator==(const QTcpSocket *socket) const;

    signals:

        void writeToLog(QString error, int level);
        void sendYou(QTcpSocket * socket);


        void updateClient(ClientParams param, CClient * client, QString newString);
        void updateChan(int update_type, CChannel * channel);

        CChannel * whichChan(int id);
        void sendToAll(QByteArray out);


        void auth(sAuthentication info);
        void reg(sRegister info);

        void shutdown();
        void params();
        void options();
        void server_pp();

        void user_connect();
        void user_diconnect();
        void user_updateParams(ClientParams param, CClient * target, QString newString);

        void chan_addUser();
        void chan_delUser();
        void chan_updateParams(ChannelParams param, CChannel * target, QString newString);









    private slots:
        void onReceiveData();
        void sendToClient(QByteArray out);

    private:

        QString m_pseudo;                   //!< m_pseudo is the username of the user
        QString m_mail;                     //!< m_mail is the email of the user
        QString m_description;              //!< m_description is the description of the user

        CSessionCookie *m_sessionCookie;   //!< Session cookie
        QTcpSocket * m_soc;                 //!< m_socket is the TCP Socket of the user

        QUuid m_uuid;                       //!< m_uuid is the UUID of the user
        int m_idChannel;                    //!< m_idChannel is the ID of the current channel of the user

        bool m_isOnline;                    //!< m_isOnline represent if the user is online or not
        bool m_isAuthenticate;              //!< m_isAuthenticate represent if the user has pass the authentification

        QImage profilePic;                  //!< profilePic is the profile picture of the user

        QByteArray * m_buffer;

        CPacketManager *m_packetManager;
};




#endif // CCLIENT_H
