#ifndef CMESSAGE_H
#define CMESSAGE_H

#include <QString>
#include <QtXml>
#include <QTime>
#include <QDate>
#include <QFile>
#include <QImage>
#include <QDebug>

class CMessage
{
    public:
        CMessage(QString from, QString to,QString message,bool isPrivate); //Create message from var
        CMessage(QString from, QString to,QString message, QImage img,bool isPrivate); //Create message with image from var
        CMessage(QString xml); //Create message from xml
        ~CMessage();

        //Getters
        QString get_from(); //Return sender
        QString get_to(); //Return receiver
        QString get_message(); //return content of the message
        QString get_time(); //Return time when message was sent
        QString get_date(); //Return date when message was sent
        QImage get_image();  //Return image in message if message got an image
        bool get_isPrivate(); //return if it's a private message


        QDomDocument get_xmlmessage(); //Return xmlmessage in QDomDocument Format, toXML() must be called first

        void toXML(); //Init QDomDocument xmlmessage from var
        QString toString(); //Convert QDomDocument xmlmessage in to string, toXML() must be called first

    private:


        //Var part

        QString from; //Sender
        QString to; //Receiver
        QTime time; //Time when message sended
        QDate date; //Date when message sended
        QString message; // The message
        QImage image; //Image hasImg  must be true
        bool isPrivate; //if true private message if false chanel message
        bool hasImg; //if true message has an image


        //XML part

        QDomDocument xmlmessage; //XML Document containing message and information
};

#endif // CMESSAGE_H
