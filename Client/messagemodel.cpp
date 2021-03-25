#include "messagemodel.h"
#include "messagelist.h"

MessageModel::MessageModel(QObject *parent)
    : QAbstractListModel(parent)
{


}


MessageList *MessageModel::getMessagesList() const
{
    return m_messagesList;
}

void MessageModel::setMessagesList(MessageList *messagesList)
{
    beginResetModel();

    m_messagesList = messagesList;

    if(m_messagesList){
        connect(m_messagesList, &MessageList::preItemAppended,this, [=](){
            const int index = m_messagesList->get_messages().size();
            beginInsertRows(QModelIndex(), index,index);
        });

        connect(m_messagesList, &MessageList::postItemAppended,this, [=](){
            endInsertRows();
        });

        connect(m_messagesList, &MessageList::preItemRemoved, this, [=](int index){
            beginRemoveRows(QModelIndex(), index,index);
        });

        connect(m_messagesList, &MessageList::postItemRemoved,this, [=](){
            endRemoveRows();
        });

        connect(m_messagesList, &MessageList::beginChangeList, this, [=](){
            beginResetModel();
        });

        connect(m_messagesList, &MessageList::endChangeList, this, [=](){
            endResetModel();
        });

    }

    connect(m_messagesList, SIGNAL(dataChanged()), this, SLOT(do_update()));
    connect(m_messagesList, SIGNAL(listChanged(MessageList*)), this, SLOT(onListChanged(MessageList*)));

    endResetModel();
}


int MessageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(!m_messagesList)
        return 0;

    return m_messagesList->get_messages().size();
}

QVariant MessageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_messagesList)
        return QVariant();

    CMessage c = m_messagesList->get_messages().at(index.row());
    switch(role){
        case FromRole:
                   return QVariant(c.get_from());
        case ToRole:
                   return QVariant(c.get_to());
        case TimeRole:
                   return QVariant(c.get_time());
        case DateRole:
                   return QVariant(c.get_date());
        case MessageRole:
                   return QVariant(c.get_message());

    }
    return QVariant();
}


bool MessageModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    if(!m_messagesList)
        return false;

    CMessage item = m_messagesList->get_messages().at(index.row());
    switch(role){
        /*case FromRole:

        case ToRole:

        case TimeRole:

        case DateRole:

        case MessageRole:
               */
    }


    if (m_messagesList->setItemAt(index.row(), item)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags MessageModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

bool MessageModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    return false;
}

bool MessageModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return false;
}



QHash<int, QByteArray> MessageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[FromRole] = "from";
    roles[ToRole] = "to";
    roles[TimeRole] = "time";
    roles[DateRole] = "date";
    roles[MessageRole] = "content";

    return roles;
}

void MessageModel::do_update()
{
    start_index = createIndex(0,0);
    end_index = createIndex(m_messagesList->get_messages().size()-1,0);
    emit dataChanged(start_index,end_index);
}

void MessageModel::onListChanged(MessageList *msgList)
{
    beginResetModel();

    m_messagesList = msgList;

    endResetModel();
}

