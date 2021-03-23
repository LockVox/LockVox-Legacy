#ifndef CHANNELMODEL_H
#define CHANNELMODEL_H

#include <QAbstractListModel>
#include "cchannel.h"

class ChannelList;

class ChannelModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ChannelList *m_channelsList READ getChannelsList WRITE setChannelsList)

public:

    enum ModelRoles{
        NameRole = Qt::UserRole+1,
        nbUsersRole,
        maxUsersRole
    };

    explicit ChannelModel(QObject *parent = nullptr);


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


    ChannelList *getChannelsList() const;
    void setChannelsList( ChannelList* channelsList);



public slots:
    void do_update();


signals:
    void currentIndexChanged(int index);


private slots:

private:
    ChannelList *m_channelsList;
    QModelIndex start_index;
    QModelIndex end_index;
};



#endif // CHANNELMODEL_H
