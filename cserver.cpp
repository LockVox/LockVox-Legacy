#include "cserver.h"

CServer::CServer()
{

    tailleMessage = 0;
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


//To delete later
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



int CServer::addChannel(CChannel * tmp){
    m_channels.push_back(tmp);


    //can return something else -> permissions;
    return 0;
}


void CServer::FillDataStreamFromClient(QDataStream & ds){


    int size = m_clients.size();

    ds << size;
    for( int i = 0; i < size; i++)
    {
        m_clients[i]->cInsertToDataStream(ds);
    }
}

void CServer::FillDataStreamFromChannel(QDataStream & ds){

    int size = m_channels.size();

    ds << size;
    for( int i = 0; i < size; i++)
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


        for(int i = 0 ; i < size; i++)
        {
            m_channels[i]->cExtractFromDataStream(ds);      //Extract ds into channel i
            ds.device()->seek(i*(PSEUDO_SIZE+3));           //Move cursor to next channel
        }
}


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

     out << 3;                                   //Flag  3 - Send Client
     FillClientFromDataStream(out);
     //test -
     qDebug() << "Packet : " << packet;
     qDebug() << "Data Stream : " << out;

     sendToAll(packet);
     packet.clear();
}

void CServer::receiveData(){

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
                    FillChannelFromDataStream(in);
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
           /*if(get_socket()->bytesAvailable() < tailleMessage){
               return;
           }*/
           tailleMessage = 0;
       }
}





QTcpSocket * CServer::get_socket(){
    return m_socket;
}


void CServer::set_socket(QTcpSocket* soc){
    m_socket  = soc;
}



























