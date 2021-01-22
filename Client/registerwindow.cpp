#include "registerwindow.h"
#include "ui_registerwindow.h"

RegisterWindow::RegisterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);   
    connect(ui->registerBtn, SIGNAL(clicked()), this, SLOT(on_registerBtn_clicked()));
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow:: on_registerBtn_clicked()
{
    //////////////////////////////
    //Envoi des données register au server
    //////////////////////////////






}
