#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Client/cserver.h"
#include "src/includes/audioinput.h"
#include "src/includes/audiooutput.h"



#include "loginwindow.h"

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
    void changeWindow();

public slots:

    //Audio


private slots:

    void Update();
    void changeWindow(int newValue);

    void on_parameter_button_clicked();
    void on_newChannel();
    void on_newClient();


    void on_changeState(int newState);
    void on_username_clicked();
    void on_status_clicked();

    void closeEvent(QCloseEvent *event);
signals:

    void RequestServer(int,int,CClient * client, CChannel * chan);



private:

    Ui::MainWindow *ui;
    LoginWindow *ui_login;
    QWidget * m_mainWidget;

    QList<channelWidget*> list_channel_widgets;
    QList<QVBoxLayout*> c_layout; //layout contenant les clients connecté pour chaque channel



    int m_state;
    int window;
    //Network
    CServer * m_server;

};
#endif // MAINWINDOW_H
