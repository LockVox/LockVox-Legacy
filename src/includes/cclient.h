#ifndef CCLIENT_H
#define CCLIENT_H


#include <QString>
#include <QList>
#include <QDebug>
#include <QObject>
#include <QtNetwork>

#include "src/includes/cchannel.h"

class CClient
{
    public:
        CClient();                                          //Constructeur par défault
        CClient(const CClient & copy);                      //Constrcteur par copie
        CClient(int id, QString name, QTcpSocket * soc, int idChannel, bool online);    //Constructeur perso
        CClient(QTcpSocket * soc);

        ~CClient() {};                                      //Destructeur



        //Getters
        QString get_pseudo();
        QString get_mail();
        QTcpSocket * get_socket();
        int get_idChannel();
        int get_id();
        bool get_isOnline()
        {
            return m_isOnline;
        }
        bool get_isAuthenticate()
        {
            return m_isAuthenticate;
        }


        //Setters
        void set_pseudo(QString pseudo);
        void set_mail(QString mail);
        void set_socket(QTcpSocket * soc);
        void set_idChannel(int id);
        void set_id(int id);
        void set_all(CClient *c);
        void set_isOnline(bool online)
        {
            m_isOnline = online;
        }
        void set_isAuthenticate(bool Auth)
        {
            m_isAuthenticate = Auth;
        }

        //Optionnal
        QByteArray serialize();
        void deserialize(QByteArray & in);

        //Serialize | Deserialize
        QJsonObject serializeToObj();
        void deserialize(QJsonObject json_obj);


        //Connexion to server
        void connect(QString mail, QByteArray password);






    private slots:


    private:
        QString m_pseudo;
        QString m_mail;
        QTcpSocket * m_soc;

        int m_id;
        int m_idChannel;

        bool m_isOnline;
        bool m_isAuthenticate;


};





#endif // CCLIENT_H
