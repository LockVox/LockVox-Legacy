#include "qma.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    quit_btn = new QPushButton("quit");
}

MainWindow::~MainWindow()
{
    delete ui;
}

