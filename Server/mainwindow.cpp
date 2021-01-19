#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qxmpp_server.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_server = new CServer();
    qxmpp_server * MainServer = new qxmpp_server(QString::fromLocal8Bit("172.20.208.1"));
    MainServer->startServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

