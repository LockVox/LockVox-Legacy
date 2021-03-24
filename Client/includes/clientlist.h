#ifndef CLIENTLIST_H
#define CLIENTLIST_H

#include <QObject>
#include <QModelIndex>

#include "src/includes/cclient.h"
#include "Client/includes/clientmodel.h"

class CClient;


class ClientList : public QObject
{
    Q_OBJECT

public:
    explicit ClientList(QObject *parent = nullptr);
    ~ClientList();

   //Getter
    Q_INVOKABLE QVector<CClient *> get_clients();

   //Setter


   //Add / Delete clients
   void addClient(CClient * client);
   void removeClient(CClient * client);
   void setItem(CClient *c);

   //Set client
   bool setItemAt(int index, CClient *item);

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
    QVector<CClient*> m_clients;
    int m_currentIndex;
};

#endif // CLIENTLIST_H
