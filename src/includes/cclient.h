/**
 * @file        cclient.h
 * @brief       Declaration of the class CClient
 * @details     This class represent an user
 * @author
 * @version     0.1
 * @date        2020
 * @copyright   GNU Public License.
 */


#ifndef CCLIENT_H
#define CCLIENT_H

//QT Includes
#include <QString>
#include <QList>
#include <QDebug>
#include <QObject>
#include <QtNetwork>
#include <QUuid>
#include <QImage>

//Includes
#include "src/includes/cchannel.h"



class CClient
{
    /**
     * @class       CClient cclient.h "cclient.h"
     * @brief       Declaration of the class CClient
     * @details     This class represent an user
     * @author
     * @version     0.1
     * @date        2020
     * @note
     * @bug
     * @warning     Une mauvaise utilisation peut faire planter votre application (c'est votre faute)
     * @remark      Une remarque à faire ?
     * @copyright   GNU Public License.
     */

    public:

        //Constructor
        CClient();
        CClient(const CClient & copy);
        CClient(QUuid id, QString name, QTcpSocket * soc, int idChannel, bool online, QString description);
        CClient(QTcpSocket * soc);
        ~CClient();

        //Getters
        QString get_pseudo();
        QString get_mail();
        QString get_description();
        QTcpSocket * get_socket();
        int get_idChannel();
        bool get_isOnline();
        bool get_isAuthenticate();
        QUuid get_uuid();
        QImage get_profilePic();

        //Setters
        void set_pseudo(QString pseudo);
        void set_mail(QString mail);
        void set_socket(QTcpSocket * soc);
        void set_idChannel(int id);
        void set_all(CClient *c);
        void set_isOnline(bool online);
        void set_isAuthenticate(bool Auth);
        void set_description(QString d);
        void set_uuid(QUuid uuid);
        void set_profilePic(QImage img);

        //Serialize | Deserialize
        QJsonObject serializeToObj();
        void deserialize(QJsonObject json_obj);

    signals:


    private slots:


    private:

        QString m_pseudo;                   //!< m_pseudo is the username of the user
        QString m_mail;                     //!< m_mail is the email of the user
        QString m_description;              //!< m_description is the description of the user

        QTcpSocket * m_soc;                 //!< m_socket is the TCP Socket of the user

        QUuid m_uuid;                       //!< m_uuid is the UUID of the user
        int m_idChannel;                    //!< m_idChannel is the ID of the current channel of the user

        bool m_isOnline;                    //!< m_isOnline represent if the user is online or not
        bool m_isAuthenticate;              //!< m_isAuthenticate represent if the user has pass the authentification

        QImage profilePic;                  //!< profilePic is the profile picture of the user

};

#endif // CCLIENT_H
