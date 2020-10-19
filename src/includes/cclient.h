#ifndef CCLIENT_H
#define CCLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QVariant>

#include "cchannel.h"

class CClient
{
    public:
        CClient();                                          //Constructeur par défault
        CClient(const CClient & copy);                      //Constrcteur par copie
        CClient(QString name, QTcpSocket * soc, int id);    //Constructeur perso
        CClient(QTcpSocket * soc);

        ~CClient() {};                                      //Destructeur



        //Getters
        QString get_pseudo();
        QTcpSocket * get_socket();
        int get_idChannel();

        //Setters
        void set_pseudo(QString pseudo);
        void set_socket(QTcpSocket * soc);
        void set_idChannel(int id);



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


        int m_idChannel;

        friend QDataStream & operator << (QDataStream & out, const CClient & value);
        friend QDataStream & operator >> (QDataStream & in, CClient & value);
};


Q_DECLARE_METATYPE(CClient)
QDataStream & operator << (QDataStream & out, const CClient & client);
QDataStream & operator >> (QDataStream & in, CClient & client);







#endif // CCLIENT_H
