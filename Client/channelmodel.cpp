#include "channelmodel.h"
#include "channellist.h"

ChannelModel::ChannelModel( QObject *parent)
    : QAbstractListModel(parent), m_channelsList()
{
}



int ChannelModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(!m_channelsList)
        return 0;

    return m_channelsList->get_channels().size();
}

QVariant ChannelModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_channelsList)
        return QVariant();

    CChannel c = *m_channelsList->get_channels().at(index.row());
    switch(role){
        case NameRole:
                   return QVariant(c.get_name());
        case nbUsersRole:
                   return QVariant(c.get_nbClients());
        case maxUsersRole:
                   return QVariant(c.get_maxUsers());
    }
    return QVariant();
}


bool ChannelModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    if(!m_channelsList)
        return false;

    CChannel * item = m_channelsList->get_channels().at(index.row());
    switch(role){
        case NameRole:
                   item->set_name(value.toString());
        case nbUsersRole:
                   item->set_nbClients(value.toInt());
        case maxUsersRole:
            item->set_maxUsers(value.toInt());
    }


    if (m_channelsList->setItemAt(index.row(), item)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags ChannelModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

bool ChannelModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    return false;
}

bool ChannelModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return false;
}



QHash<int, QByteArray> ChannelModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[nbUsersRole] = "nbUsers";
    roles[maxUsersRole] = "maxUsers";
    return roles;
}

ChannelList *ChannelModel::getChannelsList() const
{
    return m_channelsList;
}

void ChannelModel::setChannelsList(ChannelList *channelsList)
{
    beginResetModel();

    if(m_channelsList)
        return;

    m_channelsList = channelsList;

    if(m_channelsList){
        connect(m_channelsList, &ChannelList::preItemAppended,this, [=](){
            const int index = m_channelsList->get_channels().size();
            beginInsertRows(QModelIndex(), index,index);
        });

        connect(m_channelsList, &ChannelList::postItemAppended,this, [=](){
            endInsertRows();
        });

        connect(m_channelsList, &ChannelList::preItemRemoved, this, [=](int index){
            beginRemoveRows(QModelIndex(), index,index);
        });

        connect(m_channelsList, &ChannelList::postItemRemoved,this, [=](){
            endRemoveRows();
        });
    }

    connect(m_channelsList, SIGNAL(dataChanged()), this, SLOT(do_update()));

    endResetModel();
}


void ChannelModel::do_update()
{
    start_index = createIndex(0,0);
    end_index = createIndex(m_channelsList->get_channels().size()-1,0);
    emit dataChanged(start_index,end_index);
}
