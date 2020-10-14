#include "src/includes/register_interface_v2.h"
#include "ui_register_interface_v2.h"
#include "src/includes/login_interface_v2.h"
#include <QCryptographicHash>
#include <QMessageBox>
#include <QString>
#include <QSqlQuery>
#include <qDebug>

// library to test database
#include <QtSql/QSqlDatabase>


register_interface_V2::register_interface_V2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::register_interface_V2)
{
    ui->setupUi(this);
    ui->label->installEventFilter(this);


}

register_interface_V2::~register_interface_V2()
{
    delete ui;

}

bool register_interface_V2::eventFilter(QObject *o, QEvent *e)
{
    if(o == ui->label && e->type() == QEvent::MouseButtonPress)
    {
        // display register interface //
        hide();
        login_interface_V2 * login_interface;
        login_interface = new login_interface_V2(this);
        login_interface->setWindowTitle("LockVox");
        login_interface->setWindowIcon(QIcon(":/pictures/icon.png"));
        login_interface->show();


    }

    return QObject::eventFilter(o,e);
}



void register_interface_V2::on_pushButton_clicked()
{
    // DON'T FORGET TO DELETE AFTER AND USE THE REAL DATABASE
    //***************************//
    //connection to database test
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setHostName("localhost");
    m_db.setDatabaseName("test");
    m_db.setUserName("admin");
    m_db.setPassword("");
    bool ok = m_db.open();
    if(ok == true)
        QMessageBox::information(this,"Database","connection establish");
        printf("Connection establish");




    // get registration informations
    QString username = ui->email->text();
    QString email = ui->username->text();

    // generate a password hash in Sha384
    QCryptographicHash CalculateSha384(QCryptographicHash::Sha384);
    QByteArray input;
    input.append(ui->password->text());
    CalculateSha384.addData(input);
    QString hash_password = CalculateSha384.result().toHex();



    // if labels are empty
    if(username == "")
    {
        ui->email->setPlaceholderText("enter a valid username");
    }
    if(email == "")
    {
        ui->username->setPlaceholderText("enter a valid email adress");
    }
    if(ui->password->text() =="")
    {
        ui->password->setPlaceholderText("you must enter a password");
    }
    //TODO : add security to password
    //*******************************//

    //TODO : avoid sql-injections
    //******************************//



    //if the second password didnt match the first one
    if(ui->password->text()!=ui->password_confirm->text())
    {
        QMessageBox::warning(this,"Password","You didn't type the same password");
    }



    //TODO : sql request
    //****************************//
    QSqlQuery registration_qrry;

    int id_user;
    registration_qrry.exec("SELECT COUNT(*) from utilisateur;");
    if (registration_qrry.next())
    {
        id_user = registration_qrry.value(0).toInt();
        id_user++;

        registration_qrry.prepare("INSERT INTO utilisateur("
                                  "ID_utilisateur,"
                                  "img,"
                                  " usename,"
                                  " password,"
                                  " email)"
                                  " VALUES (:ID_utilisateur, :img, :usename , :password, :email);");

        registration_qrry.bindValue(":ID_utilisateur",id_user);
        registration_qrry.bindValue(":img","");
        registration_qrry.bindValue(":usename",username);
        registration_qrry.bindValue(":password",hash_password);
        registration_qrry.bindValue(":email",email);
        registration_qrry.exec();
        if(!registration_qrry.exec()) {
            //qDebug() << "SQL Statement Error" << registration_qrry.lastError();
        }
    }
    m_db.commit();
    m_db.close();
}
