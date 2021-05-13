#include "AbstractServer.h"

AbstractServer::AbstractServer()
{

}

QList<CChannel *> AbstractServer::get_channelList()
{
    return m_channels;
}

QList<CClient *> AbstractServer::get_clientList()
{
    return m_clients;
}

QByteArray * AbstractServer::get_clientBuffer(int id)
{
    return m_buffers[id];
}

QList<CChannel> AbstractServer::get_staticChannelList()
{
    QList<CChannel> list;
    foreach(CChannel *c, m_channels)
    {
        list.append(*c);
    }
    return list;
}

QList<CClient> AbstractServer::get_staticClientList()
{
    QList<CClient> list;
    foreach(CClient *c, m_clients)
    {
        list.append(*c);
    }
    return list;
}

CChannel * AbstractServer::get_channelById(int id)
{
    foreach(CChannel * c, get_channelList())
    {
        if(c->get_id() == id)
        {
            return c;
        }
    }
    return nullptr;
}

CClient * AbstractServer::get_clientById(QUuid uuid)
{
    foreach(CClient * c, get_clientList())
    {
        if(c->get_uuid() == uuid)
        {
            return c;
        }
    }
    return nullptr;
}

void AbstractServer::set_clients(QList<CClient*> clients)
{
    foreach(CClient *c, clients)
    {
        m_clients.append(c);
        QByteArray * buffer = new QByteArray;
        m_buffers.append(buffer);
    }
}

void AbstractServer::set_channels(QList<CChannel*> channels)
{
    foreach(CChannel *c, channels)
    {
        m_channels.append(c);
    }
}

void AbstractServer::set_channel(CChannel channel, int index)
{
    m_channels[index] = &channel;
}

void AbstractServer::freeChannels()
{
    foreach(CChannel * c, m_channels)
    {
       free(c);
    }
}

void AbstractServer::freeClients()
{
    foreach(CClient * c, m_clients)
    {
       free(c);
    }
    foreach(QByteArray * a, m_buffers)
    {
        delete(a);
    }
}

void AbstractServer::addClient(CClient * client)
{
    m_clients.append(client);
    QByteArray *buffer = new QByteArray;
    m_buffers.append(buffer);
}

void AbstractServer::addChannel(CChannel * channel)
{
    m_channels.append(channel);
}

void AbstractServer::delClient(CClient * client)
{
    foreach(CClient * c, m_clients)
    {
        if(c == client)
        {
            //NYI
            //TODO
        }
    }
}

void AbstractServer::DelChannel(CChannel * channel)
{
    foreach(CChannel * c, m_channels)
    {
        if(c == channel)
        {
            //NYI
            //TODO
        }
    }
}

void AbstractServer::delClient(int index)
{
    m_clients.removeAt(index);
}

void AbstractServer::DelChannel(int index)
{
    m_channels.removeAt(index);
}

void AbstractServer::printChannels()
{
    qDebug() << "Channels : \n";
    foreach(CChannel * c, get_channelList())
    {
        qDebug() << "ID[" << c->get_id() << "] Name " << c->get_name() << Qt::endl;
    }
}

void AbstractServer::printClients()
{
    qDebug() << "Clients : \n";
    foreach(CClient * c, get_clientList())
    {
        qDebug() << "ID[" << c->get_uuid() << "] Name " << c->get_pseudo() << Qt::endl;
    }
}

void AbstractServer::onReceiveData()
{
    qDebug() << "Receive Data - Virtual member of abstract server\n";
}
