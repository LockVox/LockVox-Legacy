#ifndef COBJECT_H
#define COBJECT_H

#include <QThread>
#include <QJsonObject>

class CObject : public QThread {


    public:
        enum ObjectType{
            UNKNOWN,
            SERVER,
            CHANNEL,
            CLIENT,
            MESSAGE
        };


        CObject();
        ~CObject();

        virtual void fromJSON(QJsonObject obj);
        virtual QJsonObject toJSON();

        ObjectType getObjType() const;
        void setObjType(const ObjectType &obj);

    protected :

        bool m_isPrivate;
        ObjectType m_obj;

};





#endif //COBJECT_H
