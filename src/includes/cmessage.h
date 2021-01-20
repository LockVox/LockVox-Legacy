#ifndef CMESSAGE_H
#define CMESSAGE_H

#include <QString>
#include <QtXml>
#include <QTime>
#include <QDate>
#include <QFile>

class CMessage
{
    public:
        CMessage(QString from, QString to,bool isPrivate);
        ~CMessage();

        //Getters
        QString get_from();
        QString get_to();

        QDomDocument get_message();

        QDomText get_tsender();
        QDomText get_treceiver();
        QDomText get_tsendedtime();
        QDomText get_tsendeddate();
        QDomText get_tcontent();
        QDomText get_tpm();

        bool get_isPrivate();

        //Setters
        void set_text(QDomText text);
        void set_text(QString text);


    private:

        QString from;
        QString to;

        QDomDocument message; //XML Document containing message

        QDomElement sender;
        QDomElement receiver;
        QDomElement sendedtime;
        QDomElement sendeddate;
        QDomElement content; // Content of message
        QDomElement pm; //Pirvate tag

        QDomText t_sender;
        QDomText t_receiver;
        QDomText t_sendedtime;
        QDomText t_sendeddate;
        QDomText t_content; //Text of message
        QDomText t_pm; // private content

        bool isPrivate; //if true private message if false chanel message
        QTime time; //Time when message sended
        QDate date;


};

#endif // CMESSAGE_H
