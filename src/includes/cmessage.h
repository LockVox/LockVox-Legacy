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
        CMessage(QString from, QString to,QString message,bool isPrivate); //Create message from var
        CMessage(QString xml); //Create message from xml
        ~CMessage();

        //Getters
        QString get_from();
        QString get_to();
        QString get_message();
        bool get_isPrivate();

        QDomDocument get_xmlmessage();

        void toXML();

    private:


        //Var part

        QString from; //Sender
        QString to; //Receiver
        QTime time; //Time when message sended
        QDate date; //Date when message sended
        QString message; // The message
        bool isPrivate; //if true private message if false chanel message


        //XML part

        QDomDocument xmlmessage; //XML Document containing message and information

        QDomElement sender; //Sender tag
        QDomElement receiver; //Receiver tag
        QDomElement sendedtime; //Time when message sended tag
        QDomElement sendeddate; //Date when message sended tag
        QDomElement content; // Content of message tag
        QDomElement pm; //Private tag

        QDomText t_sender; //Sender
        QDomText t_receiver; //Receiver
        QDomText t_sendedtime; //Time when message sended
        QDomText t_sendeddate; //Date when message sended
        QDomText t_content; // The message
        QDomText t_pm; //if true private message if false chanel message

};

#endif // CMESSAGE_H
