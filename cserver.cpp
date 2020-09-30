#include "cserver.h"

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

CDatabase * CServer::get_database(){
    return m_db;
}

void CServer::set_database(CDatabase * db){
    m_db = db;
}





void CServer::nouvelleConnexion()
{

    //
    CClient * newClient = new CClient();
    newClient->set_socket(serveur->nextPendingConnection());



    //Implemente information exchange - client must send a pseudo, etc


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
    foreach(CClient * client, m_clients)
    {
        client->get_socket()->write(out);
    }
}




void CServer::addChannel(CChannel * tmp){
    m_channels.push_back(tmp);
}


void CServer::FillDataStreamFromClient(QDataStream & ds){


    int size = m_clients.size();

    ds << size;
    for( int i = 0 ; i < size; i++)
    {
        m_clients[i]->cInsertToDataStream(ds);
    }


}

void CServer::FillDataStreamFromChannel(QDataStream & ds){

    int size = m_channels.size();

    ds << size;
    for( int i= 0 ; i < size; i++)
    {
        m_clients[i]->cInsertToDataStream(ds);
    }
}





#define PSEUDO_SIZE 32

void CServer::FillClientFromDataStream(QDataStream & ds)
{

        int size;

        ds >> size;


        for(int i = 0; i < size; i++)
        {
            m_clients[i]->cExtractFromDataStream(ds);          // Extracting datastream to fill client i
            ds.device()->seek(i*(PSEUDO_SIZE+1));              //Move cursor to next client
        }
}


void CServer::FillChannelFromDataStream(QDataStream & ds)
{

        int size;

        ds >> size;


        for(int i = 0; i < size; i++)
        {
            m_channels[i]->cExtractFromDataStream(ds);      //Extract ds into channel i
            ds.device()->seek(i*(PSEUDO_SIZE+1));           //Move cursor to next channel
        }
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
    FillChannelFromDataStream(out);
    //test -
    qDebug() << "Packet : " << packet;
    qDebug() << "Data Stream : " << out;

    sendToAll(packet);

    packet.clear();
     qDebug() << "Clean Packet : " << packet;

    //Third - Send clients

    out << 3;                                   //Flag 3 - Send Client
     FillClientFromDataStream(out);
     //test -
     qDebug() << "Packet : " << packet;
     qDebug() << "Data Stream : " << out;

     sendToAll(packet);
     packet.clear();
}
































