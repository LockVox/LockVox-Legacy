#ifndef CCHANNEL_H
#define CCHANNEL_H

#include <QJsonObject>
#include "src/includes/cclient.h"

class CMessage;
class CClient;

class CChannel
{


public:

    CChannel();

    CChannel(QList<CClient*> clients, QList<CMessage*> msg, QString name, int id);
    CChannel(const CChannel & copy);
    ~CChannel() {};

  //Getters
    QList<CClient *> get_clients();
    QList<CMessage *> get_message();
    QString get_name();
    CChannel* get_this();
    int get_id();
    int get_maxUsers();
    int get_nbClients();

  //Setters
    void set_clients(QList<CClient*> clients);
    void set_message(QList<CMessage*>message);
    void set_name(QString name);
    void set_id(int id);
    void set_maxUsers(int maxUsers);
    void set_nbClients(int nb);
    void set_all(CChannel * c);

    //Optionnal
    QByteArray serialize();
    void deserialize(QByteArray & in);

    //Serialize | Deserialize
    QJsonObject serializeToObj();
    void deserialize(QJsonObject json_obj);


signals:


private:
    QList<CClient*> m_clients;
    QList<CMessage*> m_messages;

    QString m_name;
    int m_nbClients;
    int m_maxUsers;
    int m_id;
};

#endif // CCHANNEL_H
