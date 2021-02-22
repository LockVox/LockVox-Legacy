#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE

namespace Ui { class LoginWindow; }

QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();



public slots:
    void on_loginBtn_clicked();
    void on_registerBtn_clicked();


signals:
    void on_askServer(QString,QString);
    void displayRegisterWindow(int);


private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
