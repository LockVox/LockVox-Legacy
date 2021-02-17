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
   QString pwd, pwdConf, name, mail;
   int pwdTest = -1;

   name = ui->email->text();
   mail = ui->username->text();
   pwd = ui->password->text();
   pwdConf = ui->password_confirm->text();


   pwdTest = QString::compare(pwd, pwdConf, Qt::CaseInsensitive);

   if(pwdTest == 0)
   {
       //REGISTER SERVER
       emit(on_askServer(name, mail, pwd));


   }
   else
   {
       QMessageBox msgBox;
       msgBox.setText("Password are not the same");
       msgBox.exec();
   }


}
