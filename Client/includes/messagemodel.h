#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H


#include <QAbstractListModel>
#include "src/includes/cmessage.h"

class MessageList;

class MessageModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(MessageList *m_messagesList READ getMessagesList WRITE setMessagesList)

    public:

        enum ModelRoles{
            FromRole = Qt::UserRole+1,
            ToRole,
            TimeRole,
            DateRole,
            MessageRole,

        };

        explicit MessageModel(QObject *parent = nullptr);


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


        MessageList *getMessagesList() const;
        void setMessagesList(MessageList *messagesList);

public slots:
        void do_update();
        void onListChanged(MessageList* msgList);

signals:


private slots:

    private:
        MessageList *m_messagesList;
        QModelIndex start_index;
        QModelIndex end_index;
};

#endif // MESSAGEMODEL_H
