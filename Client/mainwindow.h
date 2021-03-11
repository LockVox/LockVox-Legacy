#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Client/cserver.h"



#include "loginwindow.h"
#include "registerwindow.h"

#include "changeusernamewindow.h"
#include "userstatuswindow.h"
#include "channelwidget.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    CServer * get_server(){
        return m_server;
    }


    QTimer * m_timer;

    //Display UI
    void showLoginWindow();
    void showRegisterWindow();
    void showMainWindow();
    void changeWindow();

public slots:

    //Audio


private slots:

    void Update();

    void on_parameter_button_clicked();
    void on_newChannel();
    void on_newClient();


    void on_changeState(int newState);
    void on_username_clicked();
    void on_status_clicked();

    void closeEvent(QCloseEvent *event);
    void on_message_line_returnPressed();

signals:
    void RequestServer(int,int,CClient * client, CChannel * chan);
    void sendMessage(QString);



private:

    Ui::MainWindow *ui;
    LoginWindow *ui_login;
    RegisterWindow *ui_register;
    QWidget * m_mainWidget;

    QList<channelWidget*> list_channel_widgets;
    QList<QVBoxLayout*> c_layout; //layout contenant les clients connecté pour chaque channel



    int m_state;
    int window;
    //Network
    CServer * m_server;

};
#endif // MAINWINDOW_H
