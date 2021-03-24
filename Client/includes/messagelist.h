#ifndef MESSAGELIST_H
#define MESSAGELIST_H

#include <QObject>
#include <QModelIndex>

#include "src/includes/cmessage.h"
#include "Client/messagemodel.h"

class CMessage;

class MessageList : public QObject
{
    Q_OBJECT
public:
    explicit MessageList(QObject *parent = nullptr);
    ~MessageList();

   //Getter
    Q_INVOKABLE QVector<CMessage> get_messages();

   //Setter
   void set_messages(QVector<CMessage> messages);


   //Add / Delete clients
   void addMessage(CMessage client);
   void removeMessage(CMessage client);
   void setItem(CMessage c);

   //Set client
   bool setItemAt(int index, CMessage item);

signals:
   void dataChanged();
   void preItemAppended();
   void postItemAppended();

   void preItemRemoved(int index);
   void postItemRemoved();

   void beginChangeList();
   void endChangeList();
   void listChanged(MessageList * msgList);

public slots:
    void appendItem();
    void removeItem(int index);



private:
    QVector<int> m_roles;
    QVector<CMessage> m_messages;
    int m_currentIndex;
};

#endif // MESSAGELIST_H
