#include "changeusernamewindow.h"
#include "ui_changeusernamewindow.h"

ChangeUsernameWindow::ChangeUsernameWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangeUsernameWindow)
{
    ui->setupUi(this);
}

ChangeUsernameWindow::~ChangeUsernameWindow()
{
    delete ui;
}

void ChangeUsernameWindow::on_pushButton_2_clicked()
{

}
