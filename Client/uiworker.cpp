#include "uiworker.h"

#include "clientmodel.h"
#include "clientlist.h"

#include "channelmodel.h"
#include "channellist.h"

#include "messagelist.h"
#include "messagemodel.h"

#include "Client/includes/config.h"

UIWorker::UIWorker(QGuiApplication *app)
{
    m_server  = new CServer();
    MessageList *m_messageList = new MessageList();

    qmlRegisterType<ClientModel>("Client", 1,0,"ClientModel");
    qmlRegisterUncreatableType<ClientList>("Client", 1,0, "ClientList",
         QStringLiteral("ClientLit should not be created in QML"));

    qmlRegisterType<ChannelModel>("Channel", 1,0,"ChannelModel");
    qmlRegisterUncreatableType<ChannelList>("Channel", 1,0, "ChannelList",
         QStringLiteral("ClientLit should not be created in QML"));

    qmlRegisterType<MessageModel>("Message", 1,0,"MessageModel");
    qmlRegisterUncreatableType<MessageList>("Message", 1,0, "MessageList",
         QStringLiteral("ClientLit should not be created in QML"));

    m_engine.rootContext()->setContextProperty("clientsList", m_server->getClientsList());
    m_engine.rootContext()->setContextProperty("channelsList", m_server->getChannelsList());
    m_engine.rootContext()->setContextProperty("messagesList", m_messageList );


    //Load main.qml
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&m_engine, &QQmlApplicationEngine::objectCreated,
                     app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);


    m_engine.load(url);
    //Get rootobject from qml file
    m_rootObject = m_engine.rootObjects().first();

    //Get all compenents in the window
    m_window = m_rootObject->findChild<QObject*>("window");

    m_login = m_rootObject->findChild<QObject*>("login");
    m_register = m_rootObject->findChild<QObject*>("register");

    m_connectServer = m_rootObject->findChild<QObject*>("connect_server");
    m_userinfo = m_rootObject->findChild<QObject*>("userInfo");
    m_userparameter = m_rootObject->findChild<QObject*>("user_parameters");


    //Check if compenents has been load correctly
    if(!m_login || !m_register || !m_connectServer){
        qDebug("Some objects hasn't been initialized correctly");
    }

    //Connect UI - Server
    QObject::connect(m_login, SIGNAL(login_request(QString,QString)),
             m_server, SLOT(Login(QString,QString)));
    QObject::connect(this, SIGNAL(login_request(QString,QString)),
             m_server, SLOT(Login(QString,QString)));
    QObject::connect(m_register, SIGNAL(register_request(QString,QString,QString,QString)),
             m_server, SLOT(Register(QString,QString,QString,QString)));
    QObject::connect(m_connectServer, SIGNAL(connect_server(QString)),
                     m_server, SLOT(connectServer(QString)));
    QObject::connect(this, SIGNAL(connect_server(QString)),
                     m_server, SLOT(connectServer(QString)));
    QObject::connect(m_server, SIGNAL(changeState(QString)),
                     this, SLOT(onChangeState(QString)));
    QObject::connect(m_server, SIGNAL(selfChanged(CClient*)),
                     this, SLOT(onSelfChanged(CClient*)));

//The code below is only for developement, ensure that this functionnality is disable when distributing the application
#ifdef AUTO_CONNECT
    emit(connect_server(IP_ADDRESS));
#endif

#ifdef AUTO_LOGIN
    emit(login_request(USERNAME,PASSWORD));
#endif
}

void UIWorker::onChangeState(QString newState){

    if(newState== "Home"){
        QQmlProperty(m_window, "state").write("Home");
    }
}

void UIWorker::onSelfChanged(CClient* c){
    //Access here to UserInfo Qml Element, modify it has u wish with CClient methods
    // home
    QObject * username = m_userinfo->findChild<QObject*>("username");
    username->setProperty("text", c->get_pseudo());


    // in parameter widget
    QObject * usernameParamBigTitle = m_userparameter->findChild<QObject*>("username");
    QObject * username_label = m_userparameter->findChild<QObject*>("big_username");
    QObject * email_label = m_userparameter->findChild<QObject*>("email");

    usernameParamBigTitle->setProperty("text",c->get_pseudo());
    username_label->setProperty("text",c->get_pseudo());
    qDebug()<< c->get_mail();
    email_label->setProperty("text",c->get_mail());
}
