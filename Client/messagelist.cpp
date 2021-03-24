#include "Client/includes/messagelist.h"
#include "Client/includes/clientlist.h"

MessageList::MessageList(QObject *parent) : QObject(parent)
{
    CMessage m("taga","test","Ceci est un message", false);
    for(int i = 0; i < 10; i++)
        m_messages.append(m);
}


MessageList::~MessageList()
{
    disconnect(this);
}

QVector<CMessage> MessageList::get_messages()
{
    return m_messages;
}

void MessageList::set_messages(QVector<CMessage> messages)
{
    m_messages.clear();
    m_messages = messages;

}


void MessageList::addMessage(CMessage m)
{
    emit preItemAppended();

    m_messages.append(m);

    emit postItemAppended();
}

void MessageList::setItem(CMessage c){

    //Get index
    int index = -1;
    for(int i = 0; i < m_messages.size(); i++){
        /*if(m_messages[i]->get_() == c->get_uuid()){
            index = i;
            break;
        }*/
    }

    if(index != -1)
        m_messages[index] = c;

    emit dataChanged();
    return;
}



bool MessageList::setItemAt(int index, CMessage item)
{

    if(index < 0 || index >= m_messages.size())
        return false;

    m_messages[index] = item;
    return true;
}

void MessageList::appendItem()
{
    emit preItemAppended();

    //CMessage * item = new CMessage("test");
    //m_messages.append(item);

    emit postItemAppended();
}

void MessageList::removeItem(int index)
{
    emit preItemRemoved(index);

    m_messages.removeAt(index);

    emit postItemRemoved();
}
