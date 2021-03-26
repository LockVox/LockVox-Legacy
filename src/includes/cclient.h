#ifndef CCLIENT_H
#define CCLIENT_H


#include <QString>
#include <QList>
#include <QDebug>
#include <QObject>
#include <QtNetwork>
#include <QUuid>
#include <QImage>

#include "src/includes/cchannel.h"

class CClient
{
    public:
        CClient();                                          //Constructeur par défault
        CClient(const CClient & copy);                      //Constrcteur par copie
        //CClient(int id, QString name, QTcpSocket * soc, int idChannel, bool online, QString description);    //Constructeur perso
        CClient(QUuid id, QString name, QTcpSocket * soc, int idChannel, bool online, QString description);
        CClient(QTcpSocket * soc);

        ~CClient();                                      //Destructeur



        //Getters
        QString get_pseudo();
        QString get_mail();
        QTcpSocket * get_socket();
        int get_idChannel();
        //int get_id();
        bool get_isOnline();
        bool get_isAuthenticate();
        QString get_description();
        QUuid get_uuid();
        QImage get_profilePic();

        //Setters
        void set_pseudo(QString pseudo);
        void set_mail(QString mail);
        void set_socket(QTcpSocket * soc);
        void set_idChannel(int id);
        //void set_id(int id);
        void set_all(CClient *c);
        void set_isOnline(bool online);
        void set_isAuthenticate(bool Auth);
        void set_description(QString d);
        void set_uuid(QUuid uuid);
        void set_profilePic(QImage img);
        //Optionnal
        QByteArray serialize();
        void deserialize(QByteArray & in);

        //Serialize | Deserialize
        QJsonObject serializeToObj();
        void deserialize(QJsonObject json_obj);

    private slots:


    private:
        QString m_pseudo;
        QString m_mail;
        QString m_description;
        QTcpSocket * m_soc;

        //int m_id;
        QUuid m_uuid;
        int m_idChannel;

        bool m_isOnline;
        bool m_isAuthenticate;

        QImage profilePic;
};

#endif // CCLIENT_H
