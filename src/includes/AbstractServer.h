#include <QDebug>
#include <QtWidgets>
#include <QtNetwork>


#include "src/includes/cchannel.h"
#include "src/includes/cclient.h"


class CClient;
class CChannel;


class AbstractServer : public QWidget
{
    Q_OBJECT
    public:
        //Constructor
        AbstractServer(){

        };

        //Return list of channels
        QList<CChannel *> get_channelList(){
            return m_channels;
        }

        //Return list of clients
        QList<CClient *> get_clientList(){
            return m_clients;
        }

        //Return static list of channels
        QList<CChannel> get_staticChannelList(){
            QList<CChannel> list;
            foreach(CChannel *c, m_channels)
                list.append(*c);
            return list;
        }

        //Return static list of clients;
        QList<CClient> get_staticClientList(){
            QList<CClient> list;
            foreach(CClient *c, m_clients)
                list.append(*c);
            return list;
        }


        CChannel * get_channelById(int id){
            foreach(CChannel * c, get_channelList()){
                if(c->get_id() == id)
                    return c;
            }
            return NULL;
        }

        CClient * get_clientById(int id){
            foreach(CClient * c, get_clientList()){
                if(c->get_id() == id)
                    return c;
            }
            return NULL;
        }


        //Set a list of clients
        void set_clients(QList<CClient*> clients){
            foreach(CClient *c, clients)
            {
                m_clients.append(c);
            }
        }

        //Set a list of channels
        void set_channels(QList<CChannel*> channels){
            foreach(CChannel *c, channels)
            {
                m_channels.append(c);
            }
        }

        void set_channel(CChannel channel, int index){
            m_channels[index] = &channel;
        };


        //free channels
        void freeChannels(){
            foreach(CChannel * c, m_channels){
               free(c);
            }
        }

        //free clients
        void freeClients(){
            foreach(CClient * c, m_clients){
               free(c);
            }
        }

        //Add a new client
        void addClient(CClient * client){
            m_clients.append(client);
        }
        //Add a new channel
        void addChannel(CChannel * channel){
            m_channels.append(channel);
        }


        //Delete client in list
        void delClient(CClient * client){
            foreach(CClient * c, m_clients){
                if(c == client){

                }
            }
        }

        void DelChannel(CChannel * tmp){
            qDebug() << "DelChannel(CChannel * tmp) not implement yet" << Qt::endl;
        }

        //Delete client in list from his index
        void delClient(int index){
            m_clients.removeAt(index);
        }

        void DelChannel(int index){
            m_channels.removeAt(index);
        }

        void printChannels(){
            qDebug() << "Channels : \n";
            foreach(CChannel * c, get_channelList())
                qDebug() << "ID" << c->get_id() << "Name " << c->get_name()<< "\n";
        }

        void printClients(){
            qDebug() << "Clients : \n";
            foreach(CClient * c, get_clientList())
                qDebug() << "ID" << c->get_id() << "Name " << c->get_pseudo()<< "\n";
        }




    signals:


    private slots:
        virtual void onReceiveData(){
            qDebug() << "Receive Data - Virtual member of abstract server\n";
        };



    private:

        QList<CChannel*> m_channels;                        //List of channels
        QList<CClient*> m_clients;                          //List of clients










};
