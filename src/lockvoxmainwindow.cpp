#include "lockvoxmainwindow.h"
#include "ui_lockvoxmainwindow.h"

LockVoxMainWindow::LockVoxMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LockVoxMainWindow)
{
    ui->setupUi(this);
}

LockVoxMainWindow::~LockVoxMainWindow()
{
    delete ui;
}
