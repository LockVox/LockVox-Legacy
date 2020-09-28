#include "cserver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CServer s;


    //test serializaion channel
    QList<CClient * > qC;
    QList<CMessage * > qM;
    CChannel * tmp = new CChannel(qC,qM,"ici c paris", 1);

    QDataStream ds;
    s.addChannel(tmp);
    s.m_channels[0]->set_maxUsers(7);
    s.m_channels[0]->cInsertToDataStream(ds);
    s.m_channels[0]->cExtractFromDataStream(ds);







    //s.show();
    return a.exec();
}
