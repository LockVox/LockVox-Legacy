#include "cserver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CServer s;
    QList<CClient * > qC;
    QList<CMessage * > qM;




    //s.show();
    return a.exec();
}
