#include "mainwindow.h"

#include <QApplication>
#include <iostream>

#include "channelwidget.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    channelWidget * button = new channelWidget();
    button->show();

    MainWindow w;
    w.show();
    return a.exec();
}
