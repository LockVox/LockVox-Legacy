/**
 * @file        cchannel.h
 * @brief       Declaration of the class CChannel
 * @details     This class represent a channel
 * @author      LockVox Team
 * @version     0.1.1
 * @date        2021
 * @copyright   GNU GPL-3.0
 */

#ifndef CCHANNEL_H
#define CCHANNEL_H

//QT Includes
#include <QJsonObject>
#include <QUuid>

//Includes
#include "LVObject.h"
#include "cclient.h"
#include "Client/includes/messagelist.h"

//Class
class CMessage;
class CClient;
class MessageList;

enum ChannelParams{
    NAME,
    NB_USERS,
    MAX_USERS
};

class CChannel : public CObject
{
    /**
     * @class       CChannel cchannel.h "cchannel.h"
     * @brief       Declaration of the class CChannel
     * @details     This class represent a channel
     * @author      LockVox Team
     * @version     0.1.1
     * @date        2021
     * @note
     * @bug
     * @warning
     * @remark
     * @copyright   GNU GPL-3.0
     */

    public:

        //Constructor
        CChannel();
        CChannel(QString name, int id);
        CChannel(QString name, int id, int maxUsers);
        CChannel(QList<CClient*> clients, QString name, int id);
        CChannel(const CChannel & copy);
        ~CChannel() {};


        //Virtual
        void fromJSON(QJsonObject obj);
        QJsonObject toJSON();

        //operator
        bool operator==(const CChannel &client) const;


        //Getters
        QList<CClient *> get_clients();
        QString get_name();
        CChannel* get_this();
        int get_id();
        int get_maxUsers();
        int get_nbClients();
        MessageList *getMessagesLists() const;

        //Setters
        void set_clients(QList<CClient*> clients);
        void set_name(QString name);
        void set_id(int id);
        void set_maxUsers(int maxUsers);
        void set_nbClients(int nb);
        void set_all(CChannel * c);
        void setMessagesLists(MessageList *messagesLists);

        //Serialize | Deserialize
        QJsonObject serializeToObj();
        void deserialize(QJsonObject json_obj);

        //Action
        void addUser(CClient * c);
        void delUser(QUuid idUser);

    signals:


    private slots:


    private:

        QString m_name;                        //!< m_name is the name of the channel
        int m_nbClients;                       //!< m_nbClients is the current number of clients in the channel
        int m_maxUsers;                        //!< m_maxUsers is the maximum number of users in a channel
        int m_id;                              //!< m_id is the ID of the channel
        QList<CClient*> m_clients;             //!< m_clients is the list of clients in the channel
        MessageList * m_messagesLists;         //!< m_messagesLists is the list of messages associate to the channel
};

#endif // CCHANNEL_H
