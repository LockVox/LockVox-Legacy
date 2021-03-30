#include "Client/includes/cserver.h"

CServer::CServer()
{
        m_state = false;
        m_clientsList = new ClientList();
        m_channelsList = new ChannelList();
        m_messagesList = new MessageList();

        //Welcome message list display at Authentification on LockVox

        QString content = "Welcome! Enjoy your time on LockVox !";
        CMessage welcome("Founder", "You",content,false);

        m_messagesList->addMessage(welcome);

        m_currentChannelIndex = -1;
        m_finishLoad = false;
        m_hasChannelsLoaded = false;
        m_hasClientsLoaded = false;
        m_hasMessagesLoaded = false;
        m_hasSelfLoaded = false;
        m_hasPictureLoaded = false;
        m_currentUIState = "";

        m_socket = new QTcpSocket();


        m_self = NULL;
        qDebug() << "Starting LockVox client ! Welcome !" << Qt::endl;

}

CServer::CServer(ClientList *clients, ChannelList *channels) : m_clientsList(clients), m_channelsList(channels)
{
    getChannelsList()->get_channels();
}

CServer::~CServer()
{
    delete m_channelsList;
    delete m_clientsList;
    delete m_messagesList;

    free(m_self);

    m_currentChannelIndex = -1;

    //Load server informations - message - clients - channels
    m_hasSelfLoaded = false;
    m_hasChannelsLoaded = false;
    m_hasClientsLoaded = false;
    m_hasMessagesLoaded = false;
    m_finishLoad = false;
    m_currentUIState = "";

}

void CServer::connectServer(QString  ip)
{
    if(!ip.isEmpty()){
        this->m_ip = ip;
        emit(connected());
    }
}

void CServer::connectServer()
{
    m_socket->abort();
    m_socket->connectToHost(m_ip, 50885);
    //m_socket->connectToHostEncrypted(ip, 50885);

    /*if(!m_socket->waitForEncrypted()){
        qDebug() << "Error when connecting/encrypting socket : " << m_socket->errorString();
        return;
    }*/
    m_state = (m_socket->state() == QTcpSocket::ConnectingState);


    if(m_state){
        //connect(m_socket, SIGNAL(encrypted()), this, SLOT(ready()));
        connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReceiveData()));
        connect(m_socket, SIGNAL(disconnected()),this,SLOT(onDisconnected()));
        m_name = m_ip;

    }
    else{

    }

    qDebug() << "Socket state : " << m_socket->state();
    //qDebug() << "Encryption state : " << m_socket->isEncrypted();
}

void CServer::disconnectServer()
{
    m_socket->disconnectFromHost();
    delete(m_socket);
    m_socket = new QTcpSocket();

    m_self->~CClient();
    m_self = NULL;

    m_clientsList->~ClientList();
    m_clientsList = new ClientList();
    m_clients.clear();

    m_channelsList->~ChannelList();
    m_channelsList = new ChannelList();
    m_channels.clear();

    m_messagesList->~MessageList();
    m_messagesList = new MessageList();

    m_state = false;
    m_currentChannelIndex = -1;
    m_finishLoad = false;
    m_hasChannelsLoaded = false;
    m_hasClientsLoaded = false;
    m_hasMessagesLoaded = false;
    m_hasSelfLoaded = false;
}

void CServer::onDisconnected()
{

}

//Getters
QTcpSocket * CServer::get_socket(){
    return m_socket;
}

//Setters
void CServer::set_socket(QTcpSocket* soc){
    m_socket  = soc;
}

void CServer::set_self(CClient *c){
    m_self = c;
    emit(selfChanged(m_self));
}

//Envoie de l'audio au server grâce à un QByteArray
void CServer::sendToServer(QByteArray ba)
{
    //qDebug() << "Data has been send to Server ";
    m_socket->write(ba);
    m_socket->waitForBytesWritten();
}

QString CServer::getIp() const
{
    return m_ip;
}

void CServer::setIp(const QString &ip)
{
    m_ip = ip;
}

QString CServer::getName() const
{
    return m_name;
}

void CServer::setName(const QString &name)
{
    m_name = name;
}

MessageList *CServer::getMessagesList() const
{
    return m_messagesList;
}

void CServer::setMessagesList(MessageList *messagesList)
{
    emit m_messagesList->beginChangeList();

    m_messagesList->set_messages(messagesList->get_messages());

    emit m_messagesList->endChangeList();
}

int CServer::getCurrentChannelIndex() const
{
    return m_currentChannelIndex;
}

void CServer::setCurrentChannelIndex(int currentChannelIndex)
{
    m_currentChannelIndex = currentChannelIndex;
}

ChannelList *CServer::getChannelsList()
{
    return m_channelsList;
}

void CServer::setChannelsList(ChannelList *channelsList)
{
    m_channelsList = channelsList;
}

ClientList *CServer::getClientsList() const
{
    return m_clientsList;
}

void CServer::setClientsList(ClientList *clientsList)
{
    m_clientsList = clientsList;
}


int checkPacketIntegrity(QByteArray ba){
    CPacket p(ba, NULL);

    if(p.GetAction() == NULL && p.GetType() == NULL){
        return 0;
    }
    return 1;
}


void CServer::onReceiveData(){
static int nb_receive_packet;
    QByteArray *data = new QByteArray();
    data->append(m_socket->readAll());

    nb_receive_packet++;
    qDebug() << "Packet " << nb_receive_packet << " : " << *data << Qt::endl;

    CPacket tmp(*data,NULL);

    bool ifTrueProccess = true;

    if(tmp.GetType() == NULL | tmp.GetAction() == NULL)
    {
        int bracket = 0;
        QTextStream stream(data);
        QString buffer;

        while(!stream.atEnd())
        {
            if(bracket == 0 && !buffer.isEmpty())
            {
                ifTrueProccess = false;
                QByteArray array(buffer.toLocal8Bit());
                if(array !="\n" & array !="{\n}\n")
                {
                    //qDebug() << "Separate packets\n";
                    //qDebug() << "m_action : " << tmp.GetAction() << " m_type : " << tmp.GetType();
                    //qDebug() << array;
                    processIncomingData(array);
                }
                buffer.clear();
            }

            QString oneChar = stream.read(1);
            buffer += oneChar;
            if(oneChar == "{")
            {
                bracket++;
            }
            if(oneChar == "}")
            {
                bracket--;
            }
        }

        if(ifTrueProccess)
        {
            if(data->contains("\"mainObj\": {"))
            {
                //New packet
                if(!this->buffer.isEmpty())
                {
                    qDebug() << "New request held by multiple packet arrived while user buffer isn't empty, clearing it";
                    qDebug() << "A packet and therefore a request must have been lost nor a bad packet was received before";
                    this->buffer.clear();
                }
                this->buffer.append(*data);
            }
            else
            {
                if(this->buffer.isEmpty())
                {
                    //That's meen it's a bad packet, report to log
                    qDebug() << "Unable to deserialize received packet :" << Qt::endl << *data << Qt::endl << "Request Aborted" << Qt::endl;
                }
                else
                {
                    buffer.append(*data);
                    CPacket tmp1(this->buffer,NULL);

                    //We check if the packet is complete, otherwise we wait for the buffer to fill up
                    if(tmp1.GetType() != NULL & tmp1.GetAction() != NULL)
                    {
                        processIncomingData(this->buffer);
                        buffer.clear();
                    }
                }
            }
        }
    }
    else
    {
        if(ifTrueProccess & *data != "\n" & *data !="{\n}\n")
        {
            processIncomingData(*data);
        }
    }
    /*if(tmp.GetByteArray() == "{\n}\n")
    {
        qDebug() << "packet malformé\n";
        return;
    }*/

    if(!m_finishLoad && m_self){

        checkCompenents();
        checkFinishLoad();
        if(!m_finishLoad)
            loadAllCompenent();
    }

    if(m_finishLoad && m_currentUIState != "Home"){
        //m_messagesList->set_messages(getChannelsList()->get_channelAt(0)->getMessagesLists()->get_messages());
        emit picturesLoad();
        emit m_clientsList->dataChanged();
        emit m_channelsList->dataChanged();
        emit(selfChanged(m_self));
        emit(changeState("Home"));
        m_currentUIState = "Home";
    }


    delete data;
}

void CServer::loadAllCompenent(){

    if(!m_hasChannelsLoaded || !m_hasClientsLoaded){
        CPacket p("-1","-1");
        sendToServer(p.GetByteArray());
    }

    if(!m_hasMessagesLoaded & m_hasChannelsLoaded & m_hasClientsLoaded){
        foreach(CChannel * c, m_channelsList->get_channels()){
            if(c->getMessagesLists()->getHasBeenLoad() == false){
                CPacket request("1","3");
                request.Serialize_messageRequest(c->get_id(),20,0);
                sendToServer(request.GetByteArray());
            }
        }
    }
}

void CServer::checkCompenents(){

    //Check if current user informations has been load -
    if(m_self){
        m_hasSelfLoaded = true;
    }

    //Check if channels list has been load
    if(m_channelsList->get_channels().isEmpty()){
        m_hasChannelsLoaded = false;
    }
    else{
        m_hasChannelsLoaded = true;
    }

    //Check if clients list has been load
    if(m_clientsList->get_clients().isEmpty()){
        m_hasClientsLoaded = false;
    }
    else{
        m_hasClientsLoaded = true;
    }


    int nb_pic_load = 0;
    foreach(CClient * c, getClientsList()->get_clients()){
        if(!c->get_profilePic().isNull()){
            nb_pic_load++;
        }
    }
    //qDebug() << "Nb img load : " << nb_pic_load;

    if(nb_pic_load == getClientsList()->get_clients().size()){
        m_hasPictureLoaded = true;
    }

    int nb_list_messages_load = 0;
    if(!m_hasMessagesLoaded && m_hasChannelsLoaded && m_hasClientsLoaded){
    //Check if messages list has been load

        foreach(CChannel * c, m_channelsList->get_channels()){
            if(c->getMessagesLists()->getHasBeenLoad() == true)
                nb_list_messages_load++;
        }
        if(nb_list_messages_load == m_channelsList->get_channels().size()){
            m_hasMessagesLoaded = true;
        }
    }
}

void CServer::checkFinishLoad()
{
    if(m_hasChannelsLoaded & m_hasClientsLoaded & m_hasSelfLoaded & m_hasMessagesLoaded & m_hasPictureLoaded)
        m_finishLoad = true;
}

void CServer::processIncomingData(QByteArray data){

    CPacket * packet = new CPacket(data,NULL);
    //qDebug() << "m_type" << packet->GetType() << "m_action" << packet->GetAction() << Qt::endl;

    if(packet->GetAction().toInt() == -1 && packet->GetType().toInt() == -1)
    {
       if(!m_self){
           return;
       }
       Deserialize(data);

       CPacket ppRequest("1","4");
       ppRequest.Serialize_ppRequest(getClientsList()->get_clients()[0]->get_uuid().toString());
       qDebug() << ppRequest.GetByteArray();
       m_socket->write(ppRequest.GetByteArray());
    }

    //Récupération du type
    if(packet->GetType().toInt() == 0){
         switch (packet->GetAction().toInt()){
            case 0:
            {
                qDebug() << packet->GetByteArray();
                //New User is now online
                CClient * client = new CClient();
                client = packet->Deserialize_newClient();

                bool exist = false;

                for(int i = 0; i < get_clientList().size(); i++)
                {
                    if(m_clientsList->get_clients()[i]->get_uuid() == client->get_uuid())
                    {
                        client->set_isOnline(true);
                        m_clientsList->setItem(client);
                        exist=true;
                    }
                }

                if(!exist)
                {
                    m_clientsList->addClient(client);
                    emit m_clientsList->dataChanged();
                }
                break;
            }

            case 1:
            {
                //User is now offline
                CClient * client = packet->Deserialize_newClient();

                for(int i = 0; i < get_clientList().size(); i++)
                {
                   if(m_clientsList->get_clients()[i]->get_uuid() == client->get_uuid())
                   {
                       client->set_isOnline(false);
                       m_clientsList->setItem(client);
                       emit m_clientsList->dataChanged();
                       break;
                   }
                }
             free(client);
             break;
            }

            case 2:
            {
                //PSEUDO UPDATE
                qDebug() << "Receive pseudo update";
                CClient * client = packet->Deserialize_newClient();
                int index = 0;
                foreach(CClient * c, getClientsList()->get_clients()){
                    if(c->get_uuid() == client->get_uuid())
                        break;
                    index++;
                }

                getClientsList()->setItemAt(index,client);

                //client->set_pseudo(c->get_pseudo());
                break;
            }

            case 3:
            {
                //BIO UPDATE
                qDebug() << "Receive description update";
                CClient * client= packet->Deserialize_newClient();
                int index = 0;

                foreach(CClient * c, getClientsList()->get_clients()){
                    if(c->get_uuid() == client->get_uuid())
                        break;
                    index++;
                }

                getClientsList()->setItemAt(index,client);

                break;
            }

            case 4:
            {
                //BAN USER
                break;
            }

            case 5:
            {
                //BAN IP
                //Rajouter système de gestion du temps
                break;
            }

            case 6:
            {
                //Kick user
                break;
            }

            case 7:
            {
                CClient * c = packet->Deserialize_authAns();
                if(c == NULL){
                    //Emit error_login here -
                    return;
                }
                if(c != NULL)
                {
                    m_self = c;
                     emit(changeState("splashScreen"));
                }
                break;
            }

            case 8:
            {
                int code = packet->Deserialize_regAns();

                //Register successfully
                if(code == 1)
                {
                    set_self(packet->Deserialize_myClient());

                    if(m_self)
                    {
                        emit(changeState("splashScreen"));
                    }
                }
                break;
            }
        }
    }

    if(packet->GetType().toInt() == 1){
        switch (packet->GetAction().toInt())
        {
                case 0:
                {
                    //CONNECT CHAN
                    packet->Deserialize_ID();

                    CClient * client = get_clientById(packet->get_IdClient());
                    CChannel * channel = get_channelById(packet->get_IdChannel());

                    if(channel && client){
                        client->set_idChannel(channel->get_id());
                        channel->addUser(client);
                    }

                    qDebug() << client->get_pseudo() << " has join channel " << channel->get_name();
                    break;
                }

                case 1:
                {
                    //QUIT CHAN
                    packet->Deserialize_ID();

                    CClient * client = get_clientById(packet->get_IdClient());
                    CChannel * channel = get_channelById(packet->get_IdChannel());

                    if(channel && client){
                        client->set_idChannel(-1);
                        channel->delUser(client->get_uuid());
                    }
                    break;
                }

                case 2:
                {
                    //received message list
                    QVector<CMessage> messages_list = packet->Deserialize_MessageList();

                    //Case empty messages list
                    if(messages_list.isEmpty()){
                        int index = packet->Deserialize_MessageListInfo();
                        getChannelsList()->get_channelAt(index)->getMessagesLists()->setHasBeenLoad(true);
                        break;
                    }

                    int id = messages_list.first().get_to().toInt();
                    for(int i = 0; i < messages_list.size(); i++){
                        messages_list[i].getSenderPseudo(getClientsList()->get_clients());
                    }

                    getChannelsList()->get_channelAt(id)->getMessagesLists()->set_messages(messages_list);

                    break;
                }

                case 3:
                {
                    //Send message error
                    int code = packet->Deserialize_MessageError();
                    switch (code)
                    {
                        case 1:
                        {
                            //Received private message in public channel
                            qDebug() << "[Server] : Can't process a private message in a public channel. This error shouldn't append" << Qt::endl;
                        }

                        case 2:
                        {
                            //Spamming
                            qDebug() << "[Server] Stop spamming copy past ! You don't want *ding*ding* constantly in your ears do you ?" << Qt::endl;
                        }

                        case 3:
                        {
                            //Server side error
                            qDebug() << "[Server] Can't process your message. If this error does'nt appear for the first time, please contact your beloved moderator !" << Qt::endl;
                        }

                        default:
                        {
                            qDebug() << "Unknow error" << Qt::endl;
                        }
                    }
                    break;
                }

                case 4:
                {
                    //Received message
                    CMessage tmp = packet->Deserialize_Message();
                    tmp.getSenderPseudo(getClientsList()->get_clients());
                    getChannelsList()->get_channelAt(tmp.get_to().toInt())->getMessagesLists()->addMessage(tmp);

                    if(tmp.get_to().toInt() == m_currentChannelIndex){
                        getMessagesList()->set_messages(getChannelsList()->get_channelAt(tmp.get_to().toInt())->getMessagesLists()->get_messages());
                    }

                    break;
                }

                case 5:
                {
                    //Create chan voc
                    CChannel * c = packet->Deserialize_newChannel();
                    addChannel(c);

                    break;
                }

                case 6:
                {
                    //Delete chan voc
                    CChannel * c = packet->Deserialize_newChannel();
                    CChannel * toDelChannel = get_channelById(c->get_id());
                    DelChannel(toDelChannel);
                    break;
                }

                case 7:
                {
                    //Rename chan voc
                    CChannel * c = packet->Deserialize_newChannel();

                    CChannel * channel = get_channelById(c->get_id());
                    channel->set_name(c->get_name());
                    break;
                }

                case 8:
                {
                    //Modif max user (voc)
                   CChannel * c = packet->Deserialize_newChannel();
                   CChannel * channel = get_channelById(c->get_id());
                   channel->set_maxUsers(c->get_maxUsers());
                    break;
                }

                case 9:
                {
                    //kick user voc

                    break;
                }

                case 10:
                {
                    //Mute user voc (server side)

                    break;
                }

                case 11:
                {
                    //Create chan text

                    break;
                }

                case 12:
                {
                    //Delete chan text
                    break;
                }

                case 13:
                {
                    //Rename chan text
                    break;
                }

                case 14:
                {
                    //pp request answer

                    QList<QString> request = packet->Deserialize_ppAns();
                    if(request.first() == "error")
                    {
                        //error
                        break;
                    }
                    else
                    {
                        QUuid uuid(request.first());
                        qDebug() << "Arrived request pp for user : " << uuid << Qt::endl;
                        QImage img;
                        QByteArray array = QByteArray::fromBase64(request.last().toLatin1());
                        img.loadFromData(array);

                        foreach(CClient * c, getClientsList()->get_clients())
                        {
                                c->set_profilePic(img);
                        }
                    }
                    break;
                }
                default:
                {
                    qDebug() << "Error invalid action" << Qt::endl;
                    break;
                }
            break;
        }
    }

    if(packet->GetType().toInt() == 2){
        switch (packet->GetAction().toInt())
        {
            case 0:
            {
                //Mute (user side) ?????
                break;
            }

            case 1:
            {
                //Add friend --> later
                break;
            }

            case 2:
            {
                //Del friend
                break;
            }

            case 3:
            {
                //Send msg to friend
                break;
            }

            case 4:
            {
                //Modif pseudo (update bdd)
                break;
            }

            case 5:
            {
                //Change right
                break;
            }

            case 6:
            {
                //Get private message list
                QVector<CMessage> message_list = packet->Deserialize_MessageList();
                //appendClientMessage(message_list)
            }

            default:
            {
                qDebug() << "Error invalid action" << Qt::endl;
            }
        }
     }

}

bool CServer::Register(QString username, QString mail, QString password,QString password_confirm)
{
    connectServer();
    if(m_self){
        return false;
    }

    CPacket reg_pkt("0", "8");

    reg_pkt.Serialize_regReq(username, mail, password, password_confirm);

    if(m_socket->write(reg_pkt.GetByteArray()) == -1)
    {
        qDebug() << "Error in Register, can't write to socket" << Qt::endl;
        return false;
    }
    m_socket->waitForBytesWritten();
    return true;
}

bool CServer::Login(QString mail, QString passwd)
{
    connectServer();
    if(m_self){
        return false;
    }

    CPacket auth_pkt("0", "7");
    auth_pkt.Serialize_authReq(mail, passwd);
    if(m_socket->write(auth_pkt.GetByteArray()) == -1)
    {
        qDebug() << "Error in Login, can't write to socket" << Qt::endl;
        return false;
    }

    return true;
}

bool CServer::sendMessage(QString msg)
{
    qDebug() << "Send message: " << msg << Qt::endl;
    int id = getCurrentChannelIndex();

    CMessage message(m_self->get_uuid().toString(QUuid::WithoutBraces),QString::number(id),msg,false);
    CPacket sendMessage;

    sendMessage.SetType("1");
    sendMessage.SetAction("2");
    sendMessage.Serialize();

    sendMessage.Serialize_Message(message);
    if(m_socket->write(sendMessage.GetByteArray()) == -1)
    {
        qDebug() << "Error in Login, can't write to socket" << Qt::endl;
        return false;
    }
    return true;
}

bool CServer::sendMessage(QString msg, QUuid id)
{
    CMessage message(m_self->get_uuid().toString(QUuid::WithoutBraces),id.toString(QUuid::WithoutBraces),msg,true);
    CPacket sendMessage;

    sendMessage.SetType("2");
    sendMessage.SetAction("6");
    sendMessage.Serialize();

    sendMessage.Serialize_Message(message);
    if(m_socket->write(sendMessage.GetByteArray()) == -1)
    {
        qDebug() << "Error in Login, can't write to socket" << Qt::endl;
        return false;
    }
    return true;
}

void CServer::RequestServer(int type, int action, CClient * client, CChannel * chan){

    QString t = QString::number(type);
    QString a = QString::number(action);
    CPacket request(t,a);

    //Récupération du type
    switch (type) {
    case 0:{ //SERV
            switch (action)
            {
            case 0:
            {
                //New User is now online
                request.Serialize_newClient(client);
                sendToServer(request.GetByteArray());

                break;
            }
            case 1:
            {
                //User is now offline
                request.Serialize_newClient(client);
                sendToServer(request.GetByteArray());
                break;
            }
            case 2:
            {
                //PSEUDO UPDATE
                request.Serialize_newClient(client);
                sendToServer(request.GetByteArray());
                break;
            }
            case 3:
            {
                //Bio update
                request.Serialize_newClient(client);
                sendToServer(request.GetByteArray());
                break;
            }
            case 4:
            {
                //BAN USER
                request.Serialize_newClient(client);
                sendToServer(request.GetByteArray());

                break;
            }
            case 5:{
                //BAN IP
                //Rajouter système de gestion du temps
                break;
                }
            case 6: {
                //Kick user
                request.Serialize_newClient(client);
                sendToServer(request.GetByteArray());
                break;
                }

        break;
        }
    case 1: //CHAN
        switch (action)
        {
        case 0: {
            //CONNECT CHAN
            request.Serialize_ID(chan->get_id(),m_self->get_uuid());
            sendToServer(request.GetByteArray());
            break;
        }
        case 1: {
            //QUIT CHAN

            break;
        }
        case 5: {
            //Create chan voc


            break;
        }
        case 6: {
            //Delete chan voc

            break;
        }
        case 7: {
            //Rename chan voc

            break;
        }
        case 8: {
            //Modif max user (voc)

            break;
        }
        case 9: {
            //kick user voc

            break;
        }
        case 10: {
            //Mute user voc (server side)

            break;
        }
        case 11:
            //Create chan text --------> Qxmpp
            break;
        case 12:
            //Delete cahn text
            break;
        case 13:
            //Rename chan text
            break;
        default:
            qDebug() << "Error invalid action" << Qt::endl;
        }
        break;
    case 2: //USER
        switch (action)
        {
        case 0:
            //Mute (user side) ?????
            break;
        case 1:
            //Add friend --> later
            break;
        case 2:
            //Del friend
            break;
        case 3:
            //Send msg to friend
            break;
        case 4:
            //Modif pseudo (update bdd)
            break;
        case 5:
            //Change right
            break;
        default:
            qDebug() << "Error invalid action" << Qt::endl;
        }
        break;
    default:
        qDebug() << "That action isn't listed : " << Qt::endl;
    }
    return;
    }
}

bool CServer::changeUserName(QString pseudo)
{
        qDebug() << "New pseudo : " << pseudo << "\n";
        CClient * tmp = m_self;
        tmp->set_pseudo(pseudo);
        emit selfChanged(m_self);
        CPacket packet("0", "2");
        packet.Serialize_myClient(m_self);
        sendToServer(packet.GetByteArray());
        return true;
}

bool CServer::changeEmail(QString email)
{
    /*
        qDebug() << "New email : " << email << "\n";
        CClient * tmp = m_self;
        tmp->set_mail(email);
        emit selfChanged(m_self);
        CPacket packet("0", "2");
        packet.Serialize_myClient(m_self);
        sendToServer(packet.GetByteArray());
        return true;
    */
    return true;
}

bool CServer::changeDescription(QString description)
{

    qDebug() << "New description : " << description << "\n";
    CClient * tmp = m_self;
    tmp->set_description(description);
    selfChanged(m_self);
    CPacket packet("0", "3");
    packet.Serialize_myClient(m_self);
    sendToServer(packet.GetByteArray());
    return true;


}

QByteArray CServer::Serialize(){

    QJsonObject obj;
    QJsonArray cArray, sArray;

    foreach(CChannel * c, get_channelList()){
        cArray.append(c->serializeToObj());
    }
    foreach(CClient * c, get_clientList()){
       sArray.append(c->serializeToObj());
    }
    obj["channels"] = cArray;
    obj["clients"] = sArray;



    QJsonDocument jsonDoc(obj);
    qDebug() << jsonDoc;

    return jsonDoc.toJson();

}

void CServer::Deserialize(QByteArray in){

    QJsonDocument jsonDoc = QJsonDocument::fromJson(in);
    QJsonObject obj = jsonDoc.object();

    QJsonArray sArray = obj["clients"].toArray();
    QJsonArray cArray = obj["channels"].toArray();

    deserializeChannel(cArray);
    deserializeClients(sArray);

    /*
    QDir test;
    if(!test.exists("storage/log"))
    {
        if(!test.mkpath("storage/log"))
        {
            qDebug() << "[Log error] Can't create log directory" << Qt::endl;
        }
    }

    foreach(CClient *c, m_clients)
    {
        QString path = "storage/private/" + c->get_uuid().toString(QUuid::WithoutBraces) + "/pp.png";
        if(QFile::exists(path))
        {
            QImage tmp(path);
            c->set_profilePic(tmp);
        }
        else
        {
            path = "storage/server/pp/pp11.png";
            if(QFile::exists(path))
            {
                QImage tmp(path);
                c->set_profilePic(tmp);

                QByteArray array;
                QBuffer buffer(&array);
                c->get_profilePic().save(&buffer, "PNG");

                c->setImgPath(path);
                qDebug() << path;

            }
        }
    }*/
}

QByteArray CServer::SerializeChannels(){
    QJsonArray jsonArray;
    foreach(CChannel * c, get_channelList()){
        jsonArray.append(c->serializeToObj());
    }
    QJsonDocument jsonDoc(jsonArray);
    qDebug() << jsonDoc;

    return jsonDoc.toJson();

}

QByteArray CServer::SerializeClients(){
    QJsonArray jsonArray;
    foreach(CClient * c, get_clientList()){
        jsonArray.append(c->serializeToObj());
    }
    QJsonDocument jsonDoc(jsonArray);
    qDebug() << jsonDoc;

    return jsonDoc.toJson();
}

void CServer::DeserializeChannels(QByteArray in){

        //Deserialize byte array into a json document
        QJsonDocument jsonDoc = QJsonDocument::fromJson(in);

        if(jsonDoc.isNull()){
            qDebug() <<"JSON doc is invalid (null)" << Qt::endl;
        }

        QJsonArray jsonArray = jsonDoc.array();

        //Get each element of the array
        foreach( const QJsonValue & value, jsonArray){

            //Convert it to an json object then to a channel
            QJsonObject obj = value.toObject();
            CChannel * newChannel = deserializeToChannel(obj);
            //qDebug() << "Channel : " << newChannel->get_id()<< newChannel->get_name()<< Qt::endl;

            //check if the channel already exist or not
            bool exist = false;
            //if the channel exist, we reload it with new value
            foreach(CChannel * c, getChannelsList()->get_channels()){
                if(c->get_id() == newChannel->get_id()){
                    exist = true;
                    c->set_all(newChannel);
                }
            }
            //if the channel doesnt exist, we add it to the list of channel
            if(get_channelList().isEmpty() || exist == false){
                qDebug() << "That channel doesnt exist, gonna create it " << Qt::endl;
                addChannel(newChannel);
            }
        }

        //Print content of the actual list of channel - check
        /*
        foreach(CChannel * c, get_channelList()){
         qDebug() << "Channel :\nName: " << c->get_name()<< Qt::endl;
         qDebug() << "ID: " << c->get_id()<< Qt::endl;
         qDebug() << "MaxUsers: " << c->get_maxUsers()<< Qt::endl;
         qDebug() << "NbClients: " << c->get_nbClients()<< Qt::endl;
        }*/

}

CChannel * CServer::deserializeToChannel(QJsonObject json_obj){
    CChannel * channel = new CChannel();

    channel->deserialize(json_obj);

    return channel;
}

CClient * CServer::deserializeToClient(QJsonObject json_obj){
    CClient * client = new CClient();
    client->deserialize(json_obj);

    return client;
}

void CServer::deserializeChannel(QJsonArray & json_array){

    foreach( const QJsonValue & value, json_array){

        //Convert it to an json object then to a channel
        QJsonObject obj = value.toObject();
        CChannel * newChannel = deserializeToChannel(obj);


        //check if the channel already exist or not
        bool exist = false;
        foreach(CChannel * c, get_channelList()){
            if(c->get_id() == newChannel->get_id())
                 exist = true;
        }

        if(exist == false){
            m_channelsList->addChannel(newChannel);
            addChannel(newChannel);
        }
    }
}

void CServer::deserializeClients(QJsonArray & json_array){

    foreach( const QJsonValue & value, json_array)
    {
        qDebug() << value;
        //Convert it to an json object then to a channel
        QJsonObject obj = value.toObject();
        CClient * newClient = deserializeToClient(obj);

        //check if the channel already exist or not
        bool exist = false;
        foreach(CClient * c, get_clientList()){
            if(c->get_uuid() == newClient->get_uuid())
                 exist = true;
        }

        if(exist == false){
            addClient(newClient);
            m_clientsList->addClient(newClient);
        }

    }
}
