#include "mainwindow.h"
#include "qxmpp_server.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    //startServer(argc, argv);
}
