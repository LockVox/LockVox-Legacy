#ifndef CPACKET_H
#define CPACKET_H

#include "cclient.h"
#include "cchannel.h"
#include "cserver.h"
#include <QString>
#include <QJsonArray>


class CPacket
{
public:
    CPacket();
    CPacket(QByteArray n_data, CClient * p_sender);     //n_data = network data ; Initialise à partir d'un paquet reçu

    //Getters
    char GetType();                 //Retourne le code du type d'action
    char GetAction();               //Retourne le code de l'action en fonction de son type
    CClient GetSender();            //Retourne l'envoyeur du message
    QJsonDocument GetData();        //Retourne la data

    //Setters
    void SetType(char p_type);          //Définit le type d'action (utilise le code)
    void SetAction(char p_action);      //Définit l'action en fonction de son type (utilise le code)
    void SetClient(CClient * p_client);
    void SetData(QJsonDocument);        //
    //Text<-->code
    QString TypeDecode(char p_type);        //Retourne le "nom" du type d'action
    QString ServerDecode(char p_action);    //Retourne le "nom" de l'action (Sever)
    QString ChannelDecode(char p_action);   //Retourne le "nom" de l'action (Channel)
    QString UserDecode(char p_action);      //Retourne le "nom" de l'action (User)

    char TypeEncode(QString p_type);        //Retourne le code du type d'action
    char ServerEncode(QString p_action);    //Retourne le code de l'action (Serveur)
    char ChannelEncode(QString p_action);   //Retourne le code de l'action (Channel)
    char UserEncode(QString p_action);      //Retourne le code de l'action (Client)

    //Serialiaze
    QByteArray Serialize();                 //Serialisation d'un paquet pour l'envoi à travers le réseau


    //Deserialize
    void Deserialize();                //Deserialisation d'un paquet reçu à travers le réseau
    QList<CClient*> DeserializeClient(QJsonArray  in);      //Deserialisation de la structure Client
    QList<CChannel*> DeserializeChannel(QJsonArray  in);    //Deserialisation de la structure Channel
    //CServer DeserializeServer(QJsonArray  in);      //Deserialisation de la structure Serveur

private:
    char m_type;            //Type d'action
    char m_action;          //Code d'action
    CClient m_client;       //Client qui a envoyé le paquet
    QJsonDocument m_data;   //Data du paquet
};

#endif // CPACKET_H
