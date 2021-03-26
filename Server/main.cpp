
#include <QApplication>
#include <QThread>
#include <QString>

#include "mainwindow.h"

#include "Server/config.h"
#include "winsock.h"
#pragma comment(lib,"ws2_32.lib")


int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

    MainWindow w;
    w.show();


    /*WSADATA Data;
    WSAStartup(MAKEWORD(2,2), &Data);
    CChannel tmpchan("", 0);
    CAudio tmp;
    tmp.AddSession(tmpchan);
    getc(stdin);
    WSACleanup();*/
    return a.exec();
}

