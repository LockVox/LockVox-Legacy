#include <QApplication>
#include <QThread>
#include <QString>

#include "mainwindow.h"
#include "qxmpp_server.h"
#include "qxmpp_password_checker.h"
#include "openssl/sha.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
