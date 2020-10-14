#include "src/includes/cserver.h"

#include <QDebug>
CServer::CServer()
{
    // Création et disposition des widgets de la fenêtre
    etatServeur = new QLabel;
    boutonQuitter = new QPushButton(tr("Quitter"));
    connect(boutonQuitter, SIGNAL(clicked()), qApp, SLOT(quit()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(etatServeur);
    layout->addWidget(boutonQuitter);
    setLayout(layout);

    setWindowTitle(tr("ZeroChat - Serveur"));

    // Gestion du serveur
    serveur = new QTcpServer(this);
    if (!serveur->listen(QHostAddress::Any, 50885)) // Démarrage du serveur sur toutes les IP disponibles et sur le port 50585
    {
        // Si le serveur n'a pas été démarré correctement
        etatServeur->setText(tr("Le serveur n'a pas pu être démarré. Raison :<br />") + serveur->errorString());
    }
    else
    {
        // Si le serveur a été démarré correctement
        etatServeur->setText(tr("Le serveur a été démarré sur le port <strong>") + QString::number(serveur->serverPort()) + tr("</strong>.<br />Des clients peuvent maintenant se connecter."));
        connect(serveur, SIGNAL(newConnection()), this, SLOT(nouvelleConnexion()));
    }

    tailleMessage = 0;
}

//Getters
CDatabase * CServer::get_database(){
    return m_db;
}

QList<CChannel *> CServer::get_channelList(){
    return m_channels;
}

QList<CClient *> CServer::get_clientList(){
    return m_clients;
}

QList<CChannel> CServer::get_staticChannelList(){
    QList<CChannel> list;
    foreach(CChannel *c, m_channels)
        list.append(*c);
    return list;
}

QList<CClient> CServer::get_staticClientList(){
    QList<CClient> list;
    foreach(CClient *c, m_clients)
        list.append(*c);
    return list;
}

QTcpSocket * CServer::get_socket(){
    return m_socket;
}


//Setters
void CServer::set_database(CDatabase * db){
    m_db = db;
}

void CServer::set_clients(QList<CClient> clients){
    foreach(CClient c, clients)
    {
        m_clients.append(&c);

    }
}

void CServer::set_channels(QList<CChannel> channels){
    this->get_channelList().clear();
    foreach(CChannel c, channels)
    {
        m_channels.append(&c);
    }
}

void CServer::set_socket(QTcpSocket* soc){
    m_socket  = soc;
}

void CServer::freeChannels(){
    foreach(CChannel * c, m_channels)
    {
       free(c);
    }
}


void CServer::nouvelleConnexion()
{

    //
    CClient * newClient = new CClient();
    newClient->set_socket(serveur->nextPendingConnection());

    this->get_clientList().append(newClient);

    connect(newClient->get_socket(), SIGNAL(readyRead()), this, SLOT(receiveData()));
    connect(newClient->get_socket(), SIGNAL(disconnected()), this, SLOT(deconnexionClient()));
}

void CServer::deconnexionClient()

{
    //Looking for sender -
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if(socket == 0)
        return;

    //Compare to clients list
    for( int i = 0 ; i  < m_clients.size(); i++){
        if(m_clients[i]->get_socket() == socket){
            free(m_clients[i]);
            m_clients.removeAt(i);
            qDebug() << "Client " << i << " has disconnect" << Qt::endl;
        }
    }

    //send msg to everybody to say someone disconnect (id client)

}

void CServer::sendToChannel(const QString &message, int id_channel)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    //preparation of the packet
    out << (quint16) 0;
    out << message;
    out.device()->seek(0);
    out << (quint16) (packet.size() - sizeof(quint16));

    //send msg to all client of the channel
    for( int i = 0; i < m_channels[id_channel]->get_clients().size(); i++)
    {
        m_channels[id_channel]->get_clients()[i]->get_socket()->write(packet);
    }
}

void CServer::sendToClient(const QString &message, CClient * client)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    //preparation of the packet
    out << (quint16) 0;
    out << message;
    out.device()->seek(0);
    out << (quint16) (packet.size() - sizeof(quint16));

    //send msg to the client
    client->get_socket()->write(packet);
}

void CServer::envoyerATous(const QString &message)
{
    // Préparation du paquet
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0; // On écrit 0 au début du paquet pour réserver la place pour écrire la taille
    out << message; // On ajoute le message à la suite
    out.device()->seek(0); // On se replace au début du paquet
    out << (quint16) (paquet.size() - sizeof(quint16)); // On écrase le 0 qu'on avait réservé par la longueur du message


    // Envoi du paquet préparé à tous les clients connectés au serveur
    for (int i = 0; i < clients.size(); i++)
    {
        clients[i]->write(paquet);
    }
}

void CServer::sendToAll(QByteArray out)
{
    if(m_clients.isEmpty() == true)
    {
        qDebug() << "there is no client ! ";
    }
    else{
        foreach(CClient * client, m_clients)
            {
                client->get_socket()->write(out);
            }
    }
}

void CServer::addChannel(CChannel * tmp){
    m_channels.push_back(tmp);
}


//param : none
//Send CServer, CChannel and CClient to clients.
//A flag is used for each type of object
//1: CServer  / 2: CChannel / 3: CClient
void CServer::SendObjectsToClient()
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    //First - Send Server Object -
    //Flag 1 - Send Server







    //Second - Send Channels
    out << "2";                                 //Flag 2 - Send Channel
    this->ChannelsToDatastream(out);
    //test -
    qDebug() << "Packet : " << packet;
    qDebug() << "Data Stream : " << out;

    sendToAll(packet);

    packet.clear();
     qDebug() << "Clean Packet : " << packet;

    //Third - Send clients

    out << 3;                                   //Flag 3 - Send Client
    ClientsToDatastream(out);
    //test -
    qDebug() << "Packet : " << packet;
    qDebug() << "Data Stream : " << out;

    sendToAll(packet);
    packet.clear();
}

void CServer::sReceiveData(){

    /*
    QDataStream in(get_socket());

    int flag = -1;
    if(tailleMessage == 0)
    {
        if(get_socket()->bytesAvailable() < (int)sizeof(quint16)){
            return;
        }
        in >> flag;
        qDebug() << "Flag found : " << flag << Qt::endl;
        switch(flag){

            case 1:
                    //Not implement yet - send CServer object
                break;

            case 2:
                 this->data(in);
                break;
            case 3:
                 FillClientFromDataStream(in);
                break;
            default:
                qDebug() << "Flag not found - \n";
                return;
            break;
        }
        //No more bytes ready for reading
        if(get_socket()->bytesAvailable() < tailleMessage){
            return;
        }
        tailleMessage = 0;
    }
*/
}

void CServer::cReceiveData(){}


void CServer::ClientsToDatastream(QDataStream & ds)
{
    QVariant serialize = QVariant::fromValue(get_staticClientList());
    QVariantList list;
    QSequentialIterable it = serialize.value<QSequentialIterable>();

    if(serialize.canConvert(QVariant::List) == true)
    {
        for(const QVariant &c : it)
            list << c;
        ds << serialize;
    }
    else{
        qDebug() << "Error - unable to convert QVariant into QVariant List" << Qt::endl;
    }


}

void CServer::ChannelsToDatastream(QDataStream & ds){
    QVariant serialize = QVariant::fromValue(get_staticChannelList());                            //Set channel's list in QVariant
    QVariantList list;
    QSequentialIterable it = serialize.value<QSequentialIterable>();                        //Set iterable

    if(serialize.canConvert(QVariant::List) == true)                                        //if convertable
    {
        for(const QVariant &c : it)                                                         //Put a member of the list in QVariantList
            list << c;
        ds << serialize;
    }
    else{
        qDebug() << "Error - unable to convert QVariant into QVariant List" << Qt::endl;
    }

}

void CServer::DatastreamToChannels(QDataStream & ds){

    QList<CChannel>deserializeList;
    QVariant in;
    ds >> in;
    foreach(QVariant c, in.value<QVariantList>())
        deserializeList << c.value<CChannel>();
    foreach(CChannel c, deserializeList)
     {
        qDebug() << "This is here";
        qDebug() << "Set channel -\nName :" << c.get_name() << "\nID : " <<c.get_id() << "\nMax users: " << c.get_maxUsers() << "\nNb Users : " << c.get_nbClients() << Qt::endl;
    }
    this->set_channels(deserializeList);
}

void CServer::DatastreamToClients(QDataStream & ds){


    QList<CClient>deserializeList;
    QVariant in;
    ds >> in;
    foreach(QVariant c, in.value<QVariantList>())
    {

        deserializeList << c.value<CClient>();
    }

    this->set_clients(deserializeList);


}




























