#include "clientmodel.h"
#include "clientlist.h"

ClientModel::ClientModel( QObject *parent)
    : QAbstractListModel(parent), m_clientsList()
{
}



int ClientModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(!m_clientsList)
        return 0;

    return m_clientsList->get_clients().size();
}

QVariant ClientModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_clientsList)
        return QVariant();

    CClient c = *m_clientsList->get_clients().at(index.row());
    switch(role){
        case PseudoRole:
                   return QVariant(c.get_pseudo());
        case isOnlineRole:
                   return QVariant(c.get_isOnline());
    }
    return QVariant();
}


bool ClientModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    if(!m_clientsList)
        return false;

    CClient * item = m_clientsList->get_clients().at(index.row());
    switch(role){
        case PseudoRole:
                   item->set_pseudo(value.toString());
        case isOnlineRole:
                   item->set_isOnline(value.toBool());
    }


    if (m_clientsList->setItemAt(index.row(), item)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags ClientModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

bool ClientModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    return false;
}

bool ClientModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return false;
}



QHash<int, QByteArray> ClientModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PseudoRole] = "pseudo";
    roles[isOnlineRole] = "isOnline";

    return roles;
}

ClientList *ClientModel::getClientsList() const
{
    return m_clientsList;
}

void ClientModel::setClientsList(ClientList *clientsList)
{
    beginResetModel();

    if(m_clientsList)
        return;

    m_clientsList = clientsList;

    if(m_clientsList){
        connect(m_clientsList, &ClientList::preItemAppended,this, [=](){
            const int index = m_clientsList->get_clients().size();
            beginInsertRows(QModelIndex(), index,index);
        });

        connect(m_clientsList, &ClientList::postItemAppended,this, [=](){
            endInsertRows();
        });

        connect(m_clientsList, &ClientList::preItemRemoved, this, [=](int index){
            beginRemoveRows(QModelIndex(), index,index);
        });

        connect(m_clientsList, &ClientList::postItemRemoved,this, [=](){
            endRemoveRows();
        });
    }

    connect(m_clientsList, SIGNAL(dataChanged()), this, SLOT(do_update()));

    endResetModel();
}


void ClientModel::do_update()
{
    start_index = createIndex(0,0);
    end_index = createIndex(m_clientsList->get_clients().size()-1,0);
    emit dataChanged(start_index,end_index);
}

