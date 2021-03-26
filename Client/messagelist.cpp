#include "Client/includes/messagelist.h"
#include "Client/includes/clientlist.h"

MessageList::MessageList(QObject *parent) : QObject(parent)
{
    m_hasBeenLoad = false;
}


MessageList::~MessageList()
{
    disconnect(this);
    m_roles.clear();
    m_messages.clear();
    m_currentIndex = 0;
    m_hasBeenLoad = false;
}

QVector<CMessage> MessageList::get_messages()
{
    return m_messages;
}

void MessageList::set_messages(QVector<CMessage> messages)
{

    emit beginChangeList();

    m_messages.clear();
    m_messages = messages;
    m_hasBeenLoad = true;

    emit endChangeList();

    emit dataChanged();
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

bool MessageList::getHasBeenLoad() const
{
    return m_hasBeenLoad;
}

void MessageList::setHasBeenLoad(bool hasBeenLoad)
{
    m_hasBeenLoad = hasBeenLoad;
}
