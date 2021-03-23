#include <QDebug>
#include <QObject>
#include <QtNetwork>
#include <QUuid>


#include "src/includes/cchannel.h"
#include "src/includes/cclient.h"


class CClient;
class CChannel;


class AbstractServer : public QObject
{
    Q_OBJECT
    public:
        //Constructor
        AbstractServer();

        //Return list of channels
        QList<CChannel *> get_channelList();

        //Return list of clients
        QList<CClient *> get_clientList();

        //Return the buffer of the given user (id is the the line number in m_clients)
        QByteArray * get_clientBuffer(int id);

        //Return static list of channels
        QList<CChannel> get_staticChannelList();

        //Return static list of clients;
        QList<CClient> get_staticClientList();

        CChannel * get_channelById(int id);
        CClient * get_clientById(QUuid uuid);

        //Set a list of clients
        void set_clients(QList<CClient*> clients);

        //Set a list of channels
        void set_channels(QList<CChannel*> channels);
        void set_channel(CChannel channel, int index);

        //free channels
        void freeChannels();

        //free clients
        void freeClients();

        //Add a new client
        void addClient(CClient * client);

        //Add a new channel
        void addChannel(CChannel * channel);

        //Delete client in list
        void delClient(CClient * client);

        void DelChannel(CChannel * channel);

        //Delete in list from index
        void delClient(int index);
        void DelChannel(int index);

        void printChannels();
        void printClients();

    signals:

    protected slots:
        virtual void onReceiveData();

   protected:

        QList<CChannel*> m_channels;                        //List of channels
        QList<CClient*> m_clients;                          //List of clients
        QList<QByteArray*> m_buffers;                       //List of buffers for multiples packets request

};
