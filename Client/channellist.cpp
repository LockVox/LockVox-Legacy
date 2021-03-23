#include "channellist.h"

ChannelList::ChannelList(QObject *parent) : QObject(parent)
{

}

ChannelList::~ChannelList()
{
     disconnect(this);
}

QVector<CChannel *> ChannelList::get_channels()
{
        return m_channels;
}

void ChannelList::addChannel(CChannel *channel)
{
    emit preItemAppended();

    m_channels.append(channel);

    emit postItemAppended();
}

void ChannelList::removeChannel(CChannel *channel)
{

}

void ChannelList::setItem(CChannel *channel)
{
    //Get index
    int index = -1;
    for(int i = 0; i < m_channels.size(); i++){
        if(m_channels[i]->get_id() == channel->get_id()){
            index = i;
            break;
        }
    }

    if(index != -1)
        m_channels[index] = channel;

    emit dataChanged();
    return;
}

bool ChannelList::setItemAt(int index, CChannel *item)
{

    if(index < 0 || index >= m_channels.size())
        return false;

    m_channels[index] = item;
    return true;
}

void ChannelList::appendItem()
{
    emit preItemAppended();

    CChannel * item = new CChannel("Default", 0);
    m_channels.append(item);

    emit postItemAppended();
}

void ChannelList::removeItem(int index)
{
    emit preItemRemoved(index);

    m_channels.removeAt(index);

    emit postItemRemoved();
}
