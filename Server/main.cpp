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


    //qxmpp_server * MainServer = new qxmpp_server(QString::fromLocal8Bit("172.20.208.1"));
    //MainServer->startServer();
    CDatabase bdd;
    if(bdd.execMain())
    {
       string hash = bdd.getHash("monnot@et.esiea.fr");
       cout << "hash :" << hash << endl;
    }

    else
    {
        cout << "couldn't establish database connexion !" << endl;
    }

    return a.exec();
}
