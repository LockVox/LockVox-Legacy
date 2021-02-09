#include <QApplication>
#include <QThread>
#include <QString>

#include "mainwindow.h"

#include "emiplib/mipcomponent.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();


    return a.exec();
}
