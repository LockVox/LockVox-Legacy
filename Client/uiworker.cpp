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
    m_engine.rootContext()->setContextProperty("messagesList", m_server->getMessagesList() );


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

    m_menuBar = m_rootObject->findChild<QObject*>("menu_bar");

    m_login = m_rootObject->findChild<QObject*>("login");
    m_register = m_rootObject->findChild<QObject*>("register");

    m_connectServer = m_rootObject->findChild<QObject*>("connect_server");
    m_stateServer = m_rootObject->findChild<QObject*>("stateServer");
    m_userinfo = m_rootObject->findChild<QObject*>("userInfo");
    m_messageWindow = m_rootObject->findChild<QObject*>("message_window");
    m_userparameter = m_rootObject->findChild<QObject*>("user_parameters");

    m_listChannels = m_rootObject->findChild<QObject*>("listChannels");


    //Check if compenents has been load correctly
    if(!m_login || !m_register || !m_connectServer || !m_userinfo || !m_listChannels || !m_messageWindow || ! m_stateServer){
        qDebug("Some objects hasn't been initialized correctly");
    }

    QObject::connect(m_listChannels, SIGNAL(currentIndexChanged(int)),
                     this, SLOT(onCurrentIndexChanged(int)));

//Connect UI - Server
    //Connect Login - Register request
    QObject::connect(m_login, SIGNAL(login_request(QString,QString)),
             m_server, SLOT(Login(QString,QString)));
    QObject::connect(this, SIGNAL(login_request(QString,QString)),
             m_server, SLOT(Login(QString,QString)));
    QObject::connect(m_register, SIGNAL(register_request(QString,QString,QString,QString)),
             m_server, SLOT(Register(QString,QString,QString,QString)));

    //Connect to the Server -
    QObject::connect(m_connectServer, SIGNAL(connect_server(QString)),
                     m_server, SLOT(connectServer(QString)));
    QObject::connect(this, SIGNAL(connect_server(QString)),
                     m_server, SLOT(connectServer(QString)));
    //Connect state changement
    QObject::connect(m_server, SIGNAL(changeState(QString)),
                     this, SLOT(onChangeState(QString)));
    QObject::connect(m_server, SIGNAL(selfChanged(CClient*)),
                     this, SLOT(onSelfChanged(CClient*)));
    //Connect Message
    QObject::connect(m_messageWindow, SIGNAL(sendMessage(QString)),
                     m_server,SLOT(sendMessage(QString)));

    //Connect state server -
    QObject::connect(m_server, SIGNAL(connected()),
                     this, SLOT(onConnected()));
    QObject::connect(m_server, SIGNAL(disconnected()),
                     this, SLOT(onDisconnected()));

    QObject::connect(m_menuBar, SIGNAL(quit()),
                     m_server, SLOT(onQuit()));

    //QObject::connect()

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
    else if(newState == "splashScreen"){
        QQmlProperty(m_window, "state").write("splashScreen");
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

void UIWorker::onCurrentIndexChanged(int index)
{

    qDebug() << "Current Channel Index : " << index << Qt::endl;
    if(m_server->getCurrentChannelIndex() == index){
        return;
    }

    m_server->setCurrentChannelIndex(index);

    emit m_server->getMessagesList()->listChanged(m_server->getChannelsList()->get_channelAt(index)->getMessagesLists());
    //Change m_messagesList
    //m_server->setMessagesList(m_server->getChannelsList()->get_channelAt(index)->getMessagesLists());

}

void UIWorker::onConnected()
{
    QObject * state = m_stateServer->findChild<QObject*>("state");
    if(!state){
        qDebug() << "Unable to load \"state\" object in m_stateServer" << Qt::endl;
        return;
    }
    state->setProperty("text", "You are currently connect to " + m_server->getName());
    return;
}

void UIWorker::onDisconnected()
{
    QObject * state = m_stateServer->findChild<QObject*>("state");
    if(!state){
        qDebug() << "Unable to load \"state\" object in m_stateServer" << Qt::endl;
        return;
    }
    state->setProperty("text", "You are not connected to any server :'(");
    return;
}
