#include "mainwindow.h"

#include <QApplication>
#include "src/includes/cdatabase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    CDatabase * db = new CDatabase();
    free(db);
    w.show();
    return a.exec();
}
