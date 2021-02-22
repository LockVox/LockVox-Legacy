#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    connect(ui->registerButton, & QPushButton::clicked, this, &LoginWindow::on_registerBtn_clicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &LoginWindow::on_loginBtn_clicked);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_loginBtn_clicked()
{
   QString mail,pwd;
   bool testmail = true;
   bool testpwd = true;

   mail = ui->email->text();
   pwd = ui->password->text();

   if( mail.isEmpty() && pwd.isEmpty())
   {
       //QMessageBox msgboxEmptyField;
       //msgboxEmptyField.setText("Some fields are empty");
       //msgboxEmptyField.exec();
   }
   else
   {
        emit(on_askServer(mail,pwd));
   }
}

void LoginWindow::on_registerBtn_clicked()
{
    emit(displayRegisterWindow(0));
}
