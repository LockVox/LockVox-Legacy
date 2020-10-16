#ifndef CCHANNEL_H
#define CCHANNEL_H

#include "cclient.h"
#include "cmessage.h"



#include <QObject>
#include <QtWidgets>
#include <QVariant>

#include "cserver.h"
#include "ClassType.pb.h"


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
      //Méthodes


        void loadMessage();
        void cInsertToDataStream(QDataStream & ds);
        void cExtractFromDataStream(QDataStream & ds);

         static void initCChannelSystem();

    private slots:






    private:

        QList<CClient*> m_clients;
        QList<CMessage*> m_messages;

        QString m_name;
        int m_nbClients;
        int m_maxUsers;
        int m_id;


        ClassType::Channel * p_channel;

};


Q_DECLARE_METATYPE(CChannel)


#endif // CCHANNEL_H
