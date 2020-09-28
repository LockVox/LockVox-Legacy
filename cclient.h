#ifndef CCLIENT_H
#define CCLIENT_H

#include <QObject>
#include <QtNetwork>

#include "cchannel.h"



class CClient : public QWidget
{
    Q_OBJECT
    public:
        CClient();
        CClient(QString name, QTcpSocket * soc, int id);
        CClient(QTcpSocket * soc);

        ~CClient();


        //Getters
        QString get_pseudo();
        QTcpSocket * get_socket();
        int get_idChannel();

        //Setters
        void set_pseudo(QString pseudo);
        void set_socket(QTcpSocket * soc);
        void set_idChannel(int id);

        void cInsertToDataStream(QDataStream & ds);
        void cExtractFromDataStream(QDataStream & ds);


    private slots:


    private:
        QString m_pseudo;
        QTcpSocket * m_soc;


        int m_idChannel;

};

#endif // CCLIENT_H
