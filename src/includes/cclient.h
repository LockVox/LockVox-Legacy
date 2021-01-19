#ifndef CROLE_H
#define CROLE_H

#include <QString>
#include <QList>
#include <QDebug>
#include <QObject>
#include <QtNetwork>

#include "cchannel.h"

class CClient
{
    public:
        CClient();                                          //Constructeur par défault
        CClient(const CClient & copy);                      //Constrcteur par copie
        CClient(int id, QString name, QTcpSocket * soc, int idChannel, bool online);    //Constructeur perso
        CClient(QTcpSocket * soc);

        ~CClient() {};                                      //Destructeur



        //Getters
        QString get_pseudo();
        QTcpSocket * get_socket();
        int get_idChannel();
        int get_id();
        bool get_isOnline(){
            return m_isOnline;
        }


        //Setters
        void set_pseudo(QString pseudo);
        void set_socket(QTcpSocket * soc);
        void set_idChannel(int id);
        void set_id(int id);
        void set_all(CClient *c);
        void set_isOnline(bool online){
            m_isOnline = online;
        }




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


        bool m_isOnline;


};


class crole
{
public:
    crole();
    crole(crole* p_father);

    //Parent+enfants
    void AddFather(crole* p_father);
    void AddChild(crole* p_child);
    void DelChild(QString p_name);
    void DelChild(int p_id);

    //Getter+Setter
    void SetWrite(bool v);
    void SetSpeak(bool v);
    void SetKick(bool v);
    void SetDelete(bool v);
    void SetSee(bool v);
    void SetMute(bool v);

    bool GetWrite();
    bool GetSpeak();
    bool GetKick();
    bool GetDelete();
    bool GetSee();
    bool GetMute();


private:
    int m_id;                   //ID du role
    QString m_name;             //Nom du role
    crole* m_father;            //Le parent du role
    QList<crole*> m_childrens;  //Les enfants du role
    /*Chaque bool correspond à un droit associé au groupe*/
    bool m_write;
    bool m_speak;
    bool m_kick;
    bool m_delete;
    bool m_see;
    bool m_mute;

    void SetId();
};

#endif // CROLE_H
