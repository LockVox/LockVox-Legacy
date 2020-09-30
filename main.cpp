#include "qma.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("LockVox");
    w.setWindowIcon(QIcon(":/pictures/icon.png"));
    w.show();

    CServer * server = new CServer();

    //Test - Serialize Object
   /* CClient * client = new CClient("taga",NULL, 7);
    QByteArray packet;

    QDataStream ds(&packet, QIODevice::ReadWrite);

    client->cInsertToDataStream(ds);
    client->cExtractFromDataStream(ds);*/


    server->set_database(new CDatabase());
    return a.exec();
}
