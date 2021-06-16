#ifndef CLIENTMODEL_H
#define CLIENTMODEL_H

#include <QAbstractListModel>
#include "Client/includes/cclient.h"

class ClientList;

class ClientModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ClientList *m_clientsList READ getClientsList WRITE setClientsList)

public:

    enum ModelRoles{
        PseudoRole = Qt::UserRole+1,
        isOnlineRole,
        imageRole,
        descriptionRole,
        uuidRole
    };

    explicit ClientModel(QObject *parent = nullptr);


    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
              int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    //Get role
    virtual QHash<int, QByteArray> roleNames() const override;


    ClientList *getClientsList() const;
    void setClientsList(ClientList *clientsList);



public slots:
    void do_update();


signals:


private slots:

private:
    ClientList *m_clientsList;
    QModelIndex start_index;
    QModelIndex end_index;
};

#endif // CLIENTMODEL_H
