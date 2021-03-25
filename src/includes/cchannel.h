#ifndef CCHANNEL_H
#define CCHANNEL_H

#include <QJsonObject>
#include <QUuid>
#include "src/includes/cclient.h"
#include "Client/includes/messagelist.h"

class CMessage;
class CClient;
class MessageList;

class CChannel
{
    public:

    //Constructor
        CChannel();
        CChannel(QString name, int id);
        CChannel(QString name, int id, int maxUsers);
        CChannel(QList<CClient*> clients, QString name, int id);
        CChannel(const CChannel & copy);
        ~CChannel() {};

    //Getters
        QList<CClient *> get_clients();
        QString get_name();
        CChannel* get_this();
        int get_id();
        int get_maxUsers();
        int get_nbClients();

    //Setters
        void set_clients(QList<CClient*> clients);
        void set_name(QString name);
        void set_id(int id);
        void set_maxUsers(int maxUsers);
        void set_nbClients(int nb);
        void set_all(CChannel * c);

     //Serialize | Deserialize
        QJsonObject serializeToObj();
        void deserialize(QJsonObject json_obj);

        //Action
        void addUser(CClient * c);
        void delUser(QUuid idUser);

        MessageList *getMessagesLists() const;
        void setMessagesLists(MessageList *messagesLists);

signals:


private:
        QList<CClient*> m_clients;                                  //List of clients in the channel
        MessageList * m_messagesLists;


        QString m_name;                                             //Name of the channel
        int m_nbClients;                                            //Nuber of clients in the channel
        int m_maxUsers;                                             //Maximum number of users in a channel
        int m_id;                                                   //ID of the channel
};

#endif // CCHANNEL_H
