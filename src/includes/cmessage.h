#ifndef CMESSAGE_H
#define CMESSAGE_H

#include <QString>
#include <QtXml>

class CMessage
{
    public:
        CMessage();
        ~CMessage();

        //Getters
        QString get_from();
        QString get_to();
        QDomDocument get_message();
        bool get_isPrivate();

        //Setters
        void set_from(QString from);
        void set_to (QString to);
        void set_message(QDomDocument message);
        void set_isPrivate (bool isPrivate);


    private:
        QString from;
        QString to;
        QDomDocument message; //XML Document containing message
        bool isPrivate; //if true private message if false chanel message

};

#endif // CMESSAGE_H
