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

    QImage img("storage/server/pp/pp0.png");

    m_imgProvider = new ImageProvider(m_server->getClientsList(),img);
    m_engine.addImageProvider("ImageProvider", m_imgProvider);

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

    m_quitPopup = m_rootObject->findChild<QObject*>("quit_popup");
    m_connectServer = m_rootObject->findChild<QObject*>("connect_server");
    m_stateServer = m_rootObject->findChild<QObject*>("stateServer");
    m_userinfo = m_rootObject->findChild<QObject*>("userInfo");
    m_messageWindow = m_rootObject->findChild<QObject*>("message_window");
    m_userparameter = m_rootObject->findChild<QObject*>("user_parameters");

    m_listChannels = m_rootObject->findChild<QObject*>("listChannels");
    m_listClients = m_rootObject->findChild<QObject*>("listClients");
    m_changeusername = m_rootObject->findChild<QObject*>("user_parameters");

    //Check if compenents has been load correctly
    if(!m_login || !m_register || !m_connectServer || !m_userinfo || !m_listChannels || !m_messageWindow || ! m_stateServer ||  ! m_changeusername){
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
                     this, SLOT(onQuit()));
    QObject::connect(m_menuBar, SIGNAL(disconnect()),
                     this, SLOT(onDisconnect()));
    //QObject::connect(m_menuBar, SIGNAL(change_server()),
    //                this, SLOT(onChangeServer()));

    QObject::connect(m_quitPopup, SIGNAL(confirmQuit(int)),
                     this, SLOT(onConfirmQuit(int)));

    //Connect change username :

     QObject::connect(m_changeusername, SIGNAL(change_username(QString)),
                     m_server, SLOT(changeUserName(QString)));


     //connect change email
     QObject::connect(m_changeusername,SIGNAL(change_email(QString)),

                      m_server,SLOT(changeEmail(QString)));

     // connect change description

     QObject::connect(m_changeusername,SIGNAL(change_description(QString)),

                      m_server,SLOT(changeDescription(QString)));
    QObject::connect(m_server, SIGNAL(picturesLoad()),
                     this, SLOT(onPicturesLoad()));


    //QObject::connect()

//The code below is only for developement, ensure that this functionnality is disable when distributing the application

#ifdef AUTO_LOGIN
    m_server->setIp(IP);
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
    else if(newState == "état de base"){
         QQmlProperty(m_window, "state").write("état de base");
    }
}

void UIWorker::onSelfChanged(CClient* c){
    //Access here to UserInfo Qml Element, modify it has u wish with CClient methods
    // home
    QObject * username = m_userinfo->findChild<QObject*>("username");
    QObject * description = m_userinfo->findChild<QObject*>("description");

    username->setProperty("text", c->get_pseudo());
    description->setProperty("text",c->get_description());

    if(c->get_description().isEmpty())
    {
        description->setProperty("text","no description");
    }


    // in parameter widget
    QObject * usernameParamBigTitle = m_userparameter->findChild<QObject*>("username");
    QObject * username_label = m_userparameter->findChild<QObject*>("big_username");
    QObject * email_label = m_userparameter->findChild<QObject*>("email");
    QObject * description_label = m_userparameter->findChild<QObject*>("big_description");




    description_label->setProperty("text",c->get_description());

    if(c->get_description().isEmpty())
    {
        description_label->setProperty("text","no description type one");

    }

    usernameParamBigTitle->setProperty("text",c->get_pseudo());
    username_label->setProperty("text",c->get_pseudo());


    //email_label->setProperty("text",c->get_mail());
    email_label->setProperty("text",c->get_mail());
    qDebug()<< "USER mail :"<< c->get_mail();


}

void UIWorker::onCurrentIndexChanged(int index)
{

    qDebug() << "Current Channel Index : " << index << Qt::endl;
    if(m_server->getCurrentChannelIndex() == index){
        return;
    }

    m_server->setCurrentChannelIndex(index);

    m_server->getMessagesList()->set_messages(m_server->getChannelsList()->get_channelAt(index)->getMessagesLists()->get_messages());
    //emit m_server->getMessagesList()->listChanged(m_server->getChannelsList()->get_channelAt(index)->getMessagesLists());



    QObject * channel_title = m_messageWindow->findChild<QObject*>("channel_name");
    QObject * nb_channel_member = m_messageWindow->findChild<QObject*>("channel_members");

    //QString channel = m_server->getChannelsList()->get_channelAt(index)->get_name();


    // get the number of connected persons on channel
    QString nb_members =  QString::number(m_server->getChannelsList()->get_channelAt(index)->get_nbClients());
    QString nb_max = QString::number(m_server->getChannelsList()->get_channelAt(index)->get_maxUsers());

    QString nb_person_online_on_channel = nb_members+"/"+nb_max+" members";



    nb_channel_member->setProperty("text",nb_person_online_on_channel);
    channel_title->setProperty("text",m_server->getChannelsList()->get_channelAt(index)->get_name());
    //Change m_messagesList
    //m_server->setMessagesList(m_server->getChannelsList()->get_channelAt(index)->getMessagesLists());

}

void UIWorker::onPicturesLoad()
{

    m_imgProvider->setClientsList(m_server->getClientsList());

    auto contentItem = m_listClients->property("contentItem").value<QQuickItem *>();
    auto contentItemChildren = contentItem->childItems();

    for (auto childItem: contentItemChildren )
        {
            if (childItem->objectName() == "clients"){
                QObject * pseudo = childItem->findChild<QObject*>("username");
                QString new_img = QString("image://ImageProvider/");
                new_img += pseudo->property("text").toString();

                QObject * img = childItem->findChild<QObject*>("img");
                img->setProperty("source", "");
                img->setProperty("source", new_img);
             }
        }
}

void UIWorker::onConnected()
{
    QObject * state = m_stateServer->findChild<QObject*>("state");
    if(!state){
        qDebug() << "Unable to load \"state\" object in m_stateServer" << Qt::endl;
        return;
    }
    state->setProperty("text", "You are currently connect to " + m_server->getIp());
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

void UIWorker::onQuit()
{
    //qDebug("Quit..");
    //m_quitPopup->setProperty("visible", true);
}

void UIWorker::onConfirmQuit(int q){


    m_confirmQuit = q;

    if(m_confirmQuit == 0){
        qDebug() << "Cancel Quit";
        m_quitPopup->setProperty("visible", false);
    }
    else if(m_confirmQuit == 1){
        qDebug() << "Confirm Quit";
        //Call here desctructor of CServer then UIWorker

        m_engine.quit();
    }
}

void UIWorker::onDisconnect()
{
    qDebug("Disconnecting..");

    m_server->disconnectServer();

    onChangeState("état de base");
}

void UIWorker::onChangeServer()
{
    qDebug("Change server..");
}
