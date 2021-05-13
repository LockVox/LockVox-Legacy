#ifndef MESSAGELIST_H
#define MESSAGELIST_H

#include <QObject>
#include <QModelIndex>

#include "cmessage.h"
#include "Client/includes/messagemodel.h"

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

   bool getHasBeenLoad() const;
   void setHasBeenLoad(bool hasBeenLoad);

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

    bool m_hasBeenLoad;
};

#endif // MESSAGELIST_H
