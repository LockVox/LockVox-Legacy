#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();



public slots:
    void on_loginBtn_clicked();

signals:
    void on_askServer(QString,QString);

private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
