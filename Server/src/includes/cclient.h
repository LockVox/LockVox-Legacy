#ifndef CCLIENT_H
#define CCLIENT_H

#include <QObject>
#include <QtNetwork>

#include "cchannel.h"
#include "crole.h"

class CClient
{
    public:
        //Constructors
        CClient();
        CClient(QString name, QTcpSocket * soc, int id);
        CClient(QTcpSocket * soc);
        CClient(const CClient & copy);
        ~CClient();


        //Getters
        QString get_pseudo();
        QTcpSocket * get_socket();
        int get_idChannel();
        int GetUserID();
        bool GetMuted();

        //Setters
        void set_pseudo(QString pseudo);
        void set_socket(QTcpSocket * soc);
        void set_id(int id);
        void set_description(QString p_desc) {m_description = p_desc;};

        void cInsertToDataStream(QDataStream & ds);
        void cExtractFromDataStream(QDataStream & ds);
        void SetMute(bool val);







    private slots:

    private:
       int m_userid;
       QString m_pseudo;
       QString m_description;
       QTcpSocket * m_soc;
       QList<crole*> m_roles;
       int m_idChannel;
       bool m_muted;

};

#endif // CCLIENT_H
