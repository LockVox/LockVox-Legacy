#ifndef CCLIENT_H
#define CCLIENT_H

#include <QObject>
#include <QtNetwork>

#include "cchannel.h"

class CClient
{
    public:
        CClient();                                          //Constructeur par défault
        CClient(const CClient & copy);                      //Constrcteur par copie
        CClient(int id, QString name, QTcpSocket * soc, int idChannel);    //Constructeur perso
        CClient(QTcpSocket * soc);

        ~CClient() {};                                      //Destructeur



        //Getters
        QString get_pseudo();
        QTcpSocket * get_socket();
        int get_idChannel();
        int get_id();



        //Setters
        void set_pseudo(QString pseudo);
        void set_socket(QTcpSocket * soc);
        void set_idChannel(int id);
        void set_id(int id);
        void set_all(CClient *c);




        //Optionnal
        QByteArray serialize();
        void deserialize(QByteArray & in);

        //Serialize | Deserialize
        QJsonObject serializeToObj();
        void deserialize(QJsonObject json_obj);






    private slots:


    private:
        QString m_pseudo;
        QTcpSocket * m_soc;

        int m_id;
        int m_idChannel;


};




#endif // CCLIENT_H
