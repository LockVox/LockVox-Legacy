/**
 * @file        cmessage.h
 * @brief       Declaration of the class CMessage
 * @details     CMessage are simply the messages class so that client can communicate between them...
 * @author      Valentin Monnot (LockVox Team)
 * @version     0.1.1
 * @date        2021
 * @copyright   GNU GPL-3.0
 */

#ifndef CMESSAGE_H
#define CMESSAGE_H


//QT Includes
#include <QString>
#include <QtXml>
#include <QTime>
#include <QDate>
#include <QFile>
#include <QImage>
#include <QDebug>

//Includes
#include "cclient.h"
#include "cchannel.h"

//Class definition
class CClient;

//Class
class CMessage
{
    /**
     * @class       CMessage cmessage.h "cmessage.h"
     * @brief       Declaration of the class CMessage
     * @details     CMessage class is the representation of a message. (Sender, Receiver, content, ...)
     * @author      Valentin Monnot (LockVox Team)
     * @version     0.1.1
     * @date        2021
     * @note
     * @bug
     * @warning
     * @remark
     * @copyright   GNU GPL-3.0
     */

    public:
        //Constructor
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
        QString get_from_pseudo();

        QDomDocument get_xmlmessage(); //Return xmlmessage in QDomDocument Format, toXML() must be called first

        void toXML(); //Init QDomDocument xmlmessage from var
        QString toString(); //Convert QDomDocument xmlmessage in to string, toXML() must be called first

        void getSenderPseudo(QVector<CClient*> clients);

    private:

        QString from;                   //!< Sender of the message
        QString to;                     //!< Receiver of the message
        QTime time;                     //!< Time when message has been send
        QDate date;                     //!< Date of the message
        QString message;                //!< Content of the message
        QImage image;                   //!< Image hasImg  must be true
        bool isPrivate;                 //!< if true private message if false chanel message
        bool hasImg;                    //!< if true message has an image

        QString from_pseudo;            //!< From is an UUID - This is the pseudo of the sender
        //XML part

        QDomDocument xmlmessage;        //!< XML Document containing message and informations
};

#endif // CMESSAGE_H
