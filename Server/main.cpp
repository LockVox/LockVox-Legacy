#include <QApplication>
#include <QThread>
#include <QString>

#include "mainwindow.h"
#include "qxmpp_server.h"
#include "qxmpp_password_checker.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    qxmpp_server * MainServer = new qxmpp_server(QString::fromLocal8Bit("192.168.1.40"));
    MainServer->startServer();
    return a.exec();
}
