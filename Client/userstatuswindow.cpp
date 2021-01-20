#include "userstatuswindow.h"
#include "ui_userstatuswindow.h"

userStatusWindow::userStatusWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::userStatusWindow)
{
    ui->setupUi(this);
}

userStatusWindow::~userStatusWindow()
{
    delete ui;
}
