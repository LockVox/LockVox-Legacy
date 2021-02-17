#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMessageBox>


namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);
    ~RegisterWindow();

private slots:
    void on_registerBtn_clicked();

signals :
    void on_askServer(QString, QString, QString);

private:
    Ui::RegisterWindow *ui;
};

#endif // REGISTERWINDOW_H
