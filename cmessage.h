#ifndef CMESSAGE_H
#define CMESSAGE_H

#include <QObject>
#include <QtWidgets>

class CMessage
{
    Q_OBJECT
    public:
        CMessage();

        ~CMessage();

    private slots:

    private:
        int m_id;
        quint16 m_date;
        QString user;
        QString m_data;



};

#endif // CMESSAGE_H
