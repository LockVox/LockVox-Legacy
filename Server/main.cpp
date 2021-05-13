
#include <QApplication>
#include <QThread>
#include <QString>

#include "Server/config.h"
#include "Server/includes/cserver.h"
#include "winsock.h"
#pragma comment(lib,"ws2_32.lib")


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CServer * server = new CServer();
    server->start();

    /*WSADATA Data;
    WSAStartup(MAKEWORD(2,2), &Data);
    CChannel tmpchan("", 0);
    CAudio tmp;
    tmp.AddSession(tmpchan);
    getc(stdin);
    WSACleanup();*/
    return a.exec();
}
