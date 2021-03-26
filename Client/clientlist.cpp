#include "clientlist.h"

ClientList::ClientList(QObject *parent) : QObject(parent)
{

}


ClientList::~ClientList()
{
    disconnect(this);
    m_roles.clear();
    m_clients.clear();
    m_currentIndex = 0;
}

QVector<CClient *> ClientList::get_clients()
{
    return m_clients;
}


void ClientList::addClient(CClient * c)
{
    emit preItemAppended();

    m_clients.append(c);

    emit postItemAppended();
}

void ClientList::setItem(CClient* c){

    //Get index
    int index = -1;
    for(int i = 0; i < m_clients.size(); i++){
        if(m_clients[i]->get_uuid() == c->get_uuid()){
            index = i;
            break;
        }
    }

    if(index != -1)
        m_clients[index] = c;

    emit dataChanged();
    return;
}



bool ClientList::setItemAt(int index, CClient *item)
{

    if(index < 0 || index >= m_clients.size())
        return false;

    m_clients[index] = item;
    return true;
}

void ClientList::appendItem()
{
    emit preItemAppended();

    CClient * item = new CClient(NULL,"Default", NULL, 1,true,"");
    m_clients.append(item);

    emit postItemAppended();
}

void ClientList::removeItem(int index)
{
    emit preItemRemoved(index);

    m_clients.removeAt(index);

    emit postItemRemoved();
}
