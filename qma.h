#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "cserver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //Getters
    CServer * get_server();


    //Setters
    void set_server(CServer *s);


    //
    void initServer();                          //Init server -

private slots:


private:
    Ui::MainWindow *ui;


    CServer * m_server;                         //Server

};
#endif // MAINWINDOW_H
