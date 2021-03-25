#include "Client/includes/cserver.h"

CServer::CServer()
{
        m_state = false;
        m_clientsList = new ClientList();
        m_channelsList = new ChannelList();
        m_messagesList = new MessageList();

        m_currentChannelIndex = 0;

        m_socket = new QTcpSocket();
        m_self = NULL;
        qDebug() << "Starting LockVox client ! Welcome !" << Qt::endl;


}

CServer::CServer(ClientList *clients, ChannelList *channels) : m_clientsList(clients), m_channelsList(channels)
{
    getChannelsList()->get_channels();
}


void CServer::connectServer(QString  test){

    m_socket->abort();
    m_socket->connectToHost(test, 50885);

    m_state = (m_socket->state() == QTcpSocket::ConnectingState);
    qDebug() << m_state;
    if(m_state){
        connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReceiveData()));
    }
    else{

    }

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

void CServer::sendToServer(){

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

void CServer::onReceiveData(){
    // On détermine quel client envoie le message (recherche du QTcpSocket du client)

    QByteArray *data = new QByteArray();
    data->append(m_socket->readAll());

    int bracket = 0;
    bool ifTrueProccess = true;
    CPacket tmp(*data,NULL);
    if(tmp.GetAction().isNull() || tmp.GetType().isNull())
    {
        if(!data->isEmpty())
        {
            ifTrueProccess = false;
            QTextStream stream(data);
            QString buffer;

            while(!stream.atEnd())
            {
                if(bracket == 0 && !buffer.isEmpty())
                {
                    QByteArray array(buffer.toLocal8Bit());
                    processIncomingData(array);
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
        }
    }
    //Process data
    if(ifTrueProccess)
    {
        processIncomingData(*data);
    }
    delete data;
}



void CServer::processIncomingData(QByteArray data){

    CPacket * packet = new CPacket(data,NULL);
    qDebug() << "m_type : " << packet->GetType() << " m_action : " << packet->GetAction() << Qt::endl;

    if(packet->GetAction().toInt() == -1 && packet->GetType().toInt() == -1)
    {
       Deserialize(data);
       qDebug("Receive obj serv");
       if(m_self && !m_channelsList->get_channels().isEmpty() && !m_clientsList->get_clients().isEmpty())
       {
           emit(changeState("Home"));
           foreach(CChannel * c, m_channelsList->get_channels())
           {
               CPacket request("1","3");
               request.Serialize_messageRequest(c->get_id(),20,0);
               m_socket->write(request.GetByteArray());
           }
       }
    }

    //Récupération du type
    if(packet->GetType().toInt() == 0){
         switch (packet->GetAction().toInt()){
            case 0:
            {
                //New User is now online

                CClient * client = new CClient();
                client = packet->Deserialize_newClient();

                bool exist = true;

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
                   }
                }
             free(client);
             break;
            }

            case 2:
            {
                //PSEUDO UPDATE
                CClient * c = packet->Deserialize_newClient();
                CClient * client = get_clientById(c->get_uuid());
                client->set_pseudo(c->get_pseudo());
                break;
            }

            case 3:
            {
                //BIO UPDATE
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
                set_self(packet->Deserialize_authAns());
                if(m_self)
                {
                    CPacket ObjServRequest("-1","-1");
                    sendToServer(ObjServRequest.GetByteArray());
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
                        CPacket ObjServRequest("-1","-1");
                        sendToServer(ObjServRequest.GetByteArray());
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
                    int id = messages_list.first().get_to().toInt();
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
                    getChannelsList()->get_channelAt(tmp.get_to().toInt())->getMessagesLists()->addMessage(tmp);
                    if(tmp.get_to().toInt() == m_currentChannelIndex){
                        getMessagesList()->addMessage(tmp);
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
            foreach(CChannel * c, get_channelList()){
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

    foreach( const QJsonValue & value, json_array){
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
