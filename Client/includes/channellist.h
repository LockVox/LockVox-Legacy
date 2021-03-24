#ifndef CHANNELLIST_H
#define CHANNELLIST_H

#include <QObject>
#include <QModelIndex>

#include "cchannel.h"
#include "channelmodel.h"

class CChannel;


class ChannelList : public QObject
{
    Q_OBJECT

public:
    explicit ChannelList(QObject *parent = nullptr);
    ~ChannelList();

   //Getter
    Q_INVOKABLE QVector<CChannel *> get_channels();
    CChannel * get_channelAt(int index);
   //Setter


   //Add / Delete clients
   void addChannel(CChannel * channel);
   void removeChannel(CChannel * channel);
   void setItem(CChannel * channel);

   //Set client
   bool setItemAt(int index, CChannel *item);

signals:
   void dataChanged();
   void preItemAppended();
   void postItemAppended();

   void preItemRemoved(int index);
   void postItemRemoved();

public slots:
    void appendItem();
    void removeItem(int index);



private:
    QVector<int> m_roles;
    QVector<CChannel*> m_channels;
    int m_currentIndex;
};

#endif // CHANNELLIST_H
