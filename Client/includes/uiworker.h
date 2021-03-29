#ifndef UIWORKER_H
#define UIWORKER_H

#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlProperty>
#include <QQuickItem>
#include <QSignalSpy>

#include "Client/includes/cserver.h"
#include "imageprovider.h"

class CServer;
class UIWorker : public QObject
{
    Q_OBJECT
public:
    explicit UIWorker(QGuiApplication *app);


public slots:
    //void onUpdateWindow();
    void onChangeState(QString);
    void onSelfChanged(CClient*);
    void onCurrentIndexChanged(int index);

    void onConnected();
    void onDisconnected();

    void onQuit();
    void onDisconnect();
    void onChangeServer();

    void onConfirmQuit(int);
signals:
    void changeState(QString newState);
    void connect_server(QString);
    void login_request(QString,QString);
    void change_username(QString);

private:
    //Server
    CServer * m_server;
    //Engine - (MainWindow)
    QQmlApplicationEngine m_engine;
    //Root object to access to the other object of the engine
    QObject* m_rootObject;

    //Objects referencing to the different object of the engine
    QObject * m_window;

    QObject * m_menuBar;

    QObject * m_login;
    QObject * m_register;

    QObject * m_quitPopup;

    QObject * m_connectServer;
    QObject * m_userinfo;
    QObject * m_messageWindow;
    QObject * m_userparameter;

    QObject * m_listChannels;
    QObject * m_stateServer;

    QObject * m_changeusername;
    int m_confirmQuit;

    ImageProvider * m_imgProvider;

    QQmlContext * m_context;



};

#endif // UIWORKER_H
