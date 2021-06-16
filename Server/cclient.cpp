/**
 * @file Server/cclient.cpp
 * @brief Definition of class CClient.
 * @author
 * @version
 * @date
 */

//Includes
#include "cclient.h"

/**
 * @brief Default constructor of the class CClient.
 *
 * @see   CCllient::Client()
 */
CClient::CClient()
{
    m_pseudo = "";
    m_soc = NULL;
    m_idChannel = 0;
    m_isOnline = false;
    m_buffer = new QByteArray;
}

/**
 * @brief Copy constructor of the class CClient
 * @overload
 * @param copy the attribut to copy
 *
 * @see   CClient::CClient()
 */
CClient::CClient(const CClient & copy)
{ 
    Q_UNUSED(copy);
    /*
    m_pseudo = copy.m_pseudo;
    m_idChannel = copy.m_idChannel;
    m_soc = copy.m_soc;
    m_isOnline = copy.m_isOnline;
    */
}

/**
 * @brief Custom Constructor of the class CClient
 * @overload
 * @param id the UUID of the user
 * @param pseudo the username of the user
 * @param soc the socket used by the user
 * @param idChannel the ID of the current channel
 * @param online either the user is online or not
 * @param description the description of the user
 * @see   CClient::CClient()
 */
CClient::CClient(QUuid id,QString pseudo, QTcpSocket * soc, int idChannel, bool online, QString description)
{
    m_uuid = id;
    m_pseudo = pseudo;
    m_soc = soc;
    if(m_soc != NULL)
    {
        connect(m_soc, SIGNAL(readyRead()), this, SLOT(onReceiveData()));
    }
    m_idChannel = idChannel;
    m_isOnline = online;
    m_description = description;
    m_isAuthenticate = false;
    m_buffer = new QByteArray;
}

/**
 * @brief Custom Constructor of the class CClient
 * @overload
 * @param soc the socket used by the user
 * @see   CClient::CClient()
 */
CClient::CClient( QTcpSocket * soc)
{
    m_soc = soc;
    if(m_soc != NULL)
    {
        connect(m_soc, SIGNAL(readyRead()), this, SLOT(onReceiveData()));
    }
    m_buffer = new QByteArray;
}

/**
 * @brief Default desctructor of the class CClient.
 *
 * @see   CCllient::Client()
 */
CClient::~CClient()
{
    if(m_soc != NULL)
    {
        m_soc->disconnectFromHost();
        m_soc->~QTcpSocket();
        m_soc = NULL;
    }

    m_pseudo.clear();
    m_mail.clear();
    m_description.clear();
    m_uuid.~QUuid();
    m_idChannel = -1;

    m_isOnline = false;
    m_isAuthenticate = false;

    profilePic.~QImage();
    delete(m_buffer);
}

/**
 * @brief Getter of attribute m_pseudo
 * @callergraph
 * @return m_pseudo the value of attribute m_pseudo
 * @retval QString the value of attribute m_pseudo
 */
QString CClient::get_pseudo()
{
    return m_pseudo;
}

/**
 * @brief Getter of attribute m_mail
 * @callergraph
 * @return m_mail the value of attribute m_mail
 * @retval QString the value of attribute m_mail
 */
QString CClient::get_mail()
{
    return m_mail;
}

/**
 * @brief Getter of attribute m_soc
 * @callergraph
 * @return m_soc the value of attribute m_soc
 * @retval QTcpSocket the value of attribute m_soc
 */
QTcpSocket * CClient::get_socket()
{
        return m_soc;
}

/**
 * @brief Getter of attribute m_idChannel
 * @callergraph
 * @return m_idChannel the value of attribute m_idChannel
 * @retval int the value of attribute m_idChannel
 */
int CClient::get_idChannel(){
    return m_idChannel;
}

/**
 * @brief Getter of attribute m_uuid
 * @callergraph
 * @return m_uuid the value of attribute m_uuid
 * @retval QUuid the value of attribute m_uuid
 */
QUuid CClient::get_uuid()
{
    return m_uuid;
}

/**
 * @brief Getter of attribute profilePic
 * @callergraph
 * @return profilePic the value of attribute profilePic
 * @retval QImage the value of attribute profilePic
 */
QImage CClient::get_profilePic()
{
    if(profilePic.isNull())
         return QImage();
    return profilePic;
}

/**
 * @brief Getter of attribute m_isOnline
 * @callergraph
 * @return m_isOnline the value of attribute m_isOnline
 * @retval bool the value of attribute m_isOnline
 */
bool CClient::get_isOnline()
{
    return m_isOnline;
}

/**
 * @brief Getter of attribute m_isAuthenticate
 * @callergraph
 * @return m_isAuthenticate the value of attribute m_isAuthenticate
 * @retval bool the value of attribute m_isAuthenticate
 */
bool CClient::get_isAuthenticate()
{
    return m_isAuthenticate;
}

/**
 * @brief Getter of attribute m_description
 * @callergraph
 * @return m_description the value of attribute m_description
 * @retval QString the value of attribute m_description
 */
QString CClient::get_description()
{
    return m_description;
};

/**
 * @brief Setter of attribute m_pseudo
 * @callgraph
 * @param pseudo the username of the user
 * @exception range_error if pseudo is empty
 */
void CClient::set_pseudo(QString pseudo)
{
    if(!pseudo.isEmpty())
        m_pseudo = pseudo;
}

/**
 * @brief Setter of attribute m_mail
 * @callgraph
 * @param mail the email of the user
 * @exception range_error if mail is empty
 */
void CClient::set_mail (QString mail)
{
    if(!mail.isEmpty())
        m_mail = mail;
}

/**
 * @brief Setter of attribute m_soc
 * @callgraph
 * @param soc the socket of the user
 * @exception
 */
void CClient::set_socket(QTcpSocket * soc)
{
    m_soc = soc;
    if(m_soc != NULL)
    {
        connect(m_soc, SIGNAL(readyRead()), this, SLOT(onReceiveData()));
    }
}

/**
 * @brief Setter of attribute m_idChannel
 * @callgraph
 * @param id the ID of the current channel
 * @exception range_error if id is lower than -1
 */
void CClient::set_idChannel(int id){
    if(id >= -1)
        m_idChannel = id;
}

/**
 * @brief Setter of attribute m_isOnline
 * @callgraph
 * @param online either the user is online or not
 * @exception
 */
void CClient::set_isOnline(bool online)
{
    m_isOnline = online;
}

/**
 * @brief Setter of attribute m_isAuthenticate
 * @callgraph
 * @param Auth either the user has passed authentification or not
 * @exception
 */
void CClient::set_isAuthenticate(bool Auth)
{
    m_isAuthenticate = Auth;
}

/**
 * @brief Setter of attribute m_description
 * @callgraph
 * @param d the description of the user
 * @exception
 */
void CClient::set_description(QString d)
{
    m_description = d;
};

/**
 * @brief Setter of attribute m_uuid
 * @callgraph
 * @param uuid the UUID of the user
 * @exception range_error if uuid is null
 */
void CClient::set_uuid(QUuid uuid)
{
    if(!uuid.isNull())
        m_uuid = uuid;
}
/**
 * @brief Setter of attribute profilePic
 * @callgraph
 * @param img the profile picture of the user
 * @exception range_error if img is null
 */
void CClient::set_profilePic(QImage img)
{
    if(!img.isNull())
        profilePic = img;
}

/**
 * @brief Set the different attribute of a CClient from another an existing instance of CClient.
 * @param[in]       c  the CClient instance which contains the attribute to copy.
 */
void CClient::set_all(CClient *c){
    this->set_uuid(c->get_uuid());
    this->set_pseudo(c->get_pseudo());
    this->set_idChannel(c->get_idChannel());
    this->set_mail(c->get_mail());
}

/**
 * @brief Serialize attributes of an user into a JSON object.
 * @return      obj  the JSON object which contains the attribute of the user.
 */
QJsonObject CClient::serializeToObj(){
    QJsonObject obj;
    obj["uuid"] = this->get_uuid().toString();
    obj["idChannel"]= this->get_idChannel();
    obj["pseudo"]= this->get_pseudo();
    obj["isOnline"] = this->get_isOnline();
    obj["description"] = this->get_description();

    return obj;
}

/**
 * @brief Deserialize a JSON object into the current instance of CClient.
 * @param[in]       json_obj  the JSON object to deserialize into this CClient instance.
 */
void CClient::deserialize(QJsonObject json_obj){
    QUuid tmp = QUuid::fromString(json_obj["uuid"].toString());
    this->set_uuid(tmp);
    this->set_idChannel(json_obj["idChannel"].toInt());
    this->set_pseudo(json_obj["pseudo"].toString());
    this->set_isOnline(json_obj["isOnline"].toBool());
    this->set_description(json_obj["description"].toString());
}

/**
 * @brief This is a slot you call with a signal from another class to send data to the client using its TCP socket
 * @param[in] out  The QbyteArray object to send to the client.
 */
void CClient::sendToClient(QByteArray out)
{
    if(m_soc != NULL && m_soc->isOpen())
    {
        m_soc->write(out);
        m_soc->waitForBytesWritten();
    }
}

/**
 * @brief This slot is called when client TCP socket send signal "readyRead", it pre process the data received and call function processData
 */
void CClient::onReceiveData()
{
  //Get data
  QByteArray *data = new QByteArray();
  data->append(m_soc->readAll());

  //Process data
  CPacket tmp(*data, this); //Check if valid packet, if not, may be a splitted packet or multiple packet
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
              if(array !="\n")
              {
                  processData(array);
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
              if(!m_buffer->isEmpty())
              {
                  emit writeToLog("[" + m_uuid.toString() + "(" + m_pseudo +
                             ")] New request held by multiple packet arrived while user buffer isn't empty, clearing it",SERVER_WARN);
                  emit writeToLog("A packet and therefore a request must have been lost nor a bad packet was received before",SERVER_WARN);
                  m_buffer->clear();
              }
              m_buffer->append(*data);
          }
          else
          {
              if(m_buffer->isEmpty())
              {
                  //That's meen it's a bad packet, report to log
                  emit writeToLog("Unable to deserialize received packet :\n" + *data + "\nRequest Aborted", SERVER_WARN);
              }
              else
              {
                  m_buffer->append(*data);
                  CPacket tmp1(*m_buffer, this);

                  //We check if the packet is complete, otherwise we wait for the buffer to fill up
                  if(tmp1.GetType() != NULL & tmp1.GetAction() != NULL)
                  {
                      processData(*m_buffer);
                      m_buffer->clear();
                  }
              }
          }
      }
  }
  else
  {
      if(ifTrueProccess & *data != "\n")
      {
          processData(*data);
      }
  }
}

/**
 * @brief This function is called by onReceiveData to process received data in order to manage cases defined by the application protocol
 * @param[in] data  The QByteArray object received from the TCP socket
 */
void CClient::processData(QByteArray data) //Process received data
{

      CPacket* packet = new CPacket(data, this);

      if(packet->GetAction() == NULL || packet->GetType() == NULL)//Bad packet
      {
          emit writeToLog("Unable to deserialize received packet :\n" + packet->GetByteArray() + "\nRequest Aborted", SERVER_WARN);
          return;
      }

      if(packet->GetAction().toInt() == -1 && packet->GetType().toInt() == -1) //Request server object
      {
          emit sendYou(m_soc);
          return;
      }

      //Get type
      switch (packet->GetType().toInt())
      {
          case 0: //SERV
          {
              switch (packet->GetAction().toInt())
              {
                  /*case 0: //Did we use that ?
                  {
                      //SERV CONNECT
                      //Check Auth -
                      //If Auth is Ok
                      CClient * client = packet->Deserialize_newClient();
                      //addClient(client);

                      if(m_isOnline == false)
                      {
                          m_isOnline = true;
                      }

                      //Update info -
                      CPacket ans("0","0");
                      ans.Serialize_newClient(client,false);
                      //sendToAll(ans.GetByteArray());
                      break;
                  }*/

                  case 1:
                  {
                      //SERV DISCONNECT
                      //Update online users
                      CClient * client = packet->Deserialize_newClient();
                      if(m_uuid != NULL)
                      {
                          m_isOnline = false;
                      }

                      //User is not online anymore
                      CPacket ans("0","1");
                      ans.Serialize_newClient(client,false);

                      //Send Update
                      emit sendToAll(packet->GetByteArray());

                      free(client);

                      break;
                  }

                  case 2:
                  {
                      //PSEUDO UPDATE
                      CClient * client = packet->Deserialize_myClient();

                      emit writeToLog("User [" + m_uuid.toString() + "(" + m_pseudo + ")] change username to [" + client->get_pseudo() + "]", SERVER);
                      //Apply changement
                      m_pseudo = client->get_pseudo();

                      emit updateMe(0,client);

                      break;
                  }

                  case 3:
                  {
                      //BIO UPDATE
                      CClient * client = packet->Deserialize_myClient();

                      emit writeToLog("User [" + m_uuid.toString() + "(" + m_pseudo + ")] change description to [" + m_description + "]", SERVER);
                      //Apply changement
                      m_description = client->get_description();

                      emit updateMe(1,client);

                      break;
                  }

                  case 4:
                  {
                      //BAN USER
                      //Check if user have right to before...
                      //TODO

                      break;
                  }

                  case 5:
                  {
                      //BAN IP
                      //Maybe add time ?
                      //TODO
                      break;
                  }

                  case 6:
                  {
                      //Kick user
                      //CClient * client = packet->Deserialize_newClient();

                      /*if(get_clientById(client->get_uuid()) != NULL)
                      {
                          * We should not use .removeOne but implemente AbstractServer::delClient()
                          * Right must be implemeted and we must verify user have right to kick before implement kick
                          writeToLog("User [" + client->get_uuid().toString() + "(" + client->get_pseudo() + ")] Has been kicked from server" ,SERVER);
                          sendToAll(packet->GetByteArray());
                          m_clients.removeOne(get_clientById(client->get_uuid()));
                          client->get_socket()->close();

                      }*/

                      break;
                  }

                  case 7:
                  {
                      //Auth request
                      QList<QString> info = packet->Deserialize_auth();
                      emit authMe(info, this);

                      break;
                  }

                  case 8 :
                  {
                      //Get Information from request
                      //REGISTER
                      QList<QString> info = packet->Deserialize_regReq();
                      emit regMe(info, this);

                      break;
                  }

                  default:
                  {
                      emit writeToLog("Received invalid server action :\n" + packet->GetByteArray() +"\nIgnored", SERVER_WARN);
                      break;
                  }
              }
              break;
          }

          case 1: //CHAN
          {
              switch (packet->GetAction().toInt())
              {
                  case 0:
                  {
                      //JOIN CHANNEL
                      packet->Deserialize_ID();

                      CChannel * channel = emit whichChan(packet->get_IdChannel());

                      if(channel)
                      {
                          m_idChannel = channel->get_id();
                          channel->addUser(this);

                          CPacket ans("1","0");

                          ans.Serialize();
                          ans.Serialize_ID(channel->get_id(),m_uuid);

                          emit sendToAll(ans.GetByteArray());

                          emit writeToLog("User [" + m_uuid.toString() + "(" + m_pseudo + ")] has joined channel id ["+ QString::number(packet->get_IdChannel()) + "]", SERVER);
                      }
                      else
                      {
                          if(!channel)
                          {
                              emit writeToLog("User [" + m_uuid.toString() + "(" + m_pseudo + ")] is trying to join a non-existing channel id [" + QString::number(packet->get_IdChannel()) + "]", SERVER_WARN);
                              //Send error packet
                              //TODO
                          }
                      }
                      break;
                  }

                  case 1:
                  {
                      //QUIT CHAN
                      packet->Deserialize_ID();

                      CChannel * channel = emit whichChan(packet->get_IdChannel());

                      if(channel)
                      {
                          m_idChannel = -1;
                          channel->delUser(m_uuid);

                          CPacket ans("1","1");

                          ans.Serialize();
                          ans.Serialize_ID(channel->get_id(),m_uuid);

                          emit sendToAll(ans.GetByteArray());

                          emit writeToLog("User [" + m_uuid.toString() + "(" + m_pseudo + ")] has quit channel id ["+ QString::number(packet->get_IdChannel()) + "]", SERVER);
                      }
                      else
                      {
                          if(!channel)
                          {
                              emit writeToLog("[" + m_soc->peerAddress().toString() + "] Try to exit a non-existing channel ???",SERVER_ERR);
                              break;
                          }
                      }
                      break;
                  }

                  case 2:
                  {
                      //Text channel message
                      CMessage msg = packet->Deserialize_Message();

                      if(msg.get_to().toInt() <= -1 || emit whichChan(msg.get_to().toInt()) == nullptr)
                      {
                          emit writeToLog("[" + m_soc->peerAddress().toString() + " Received a message for an unknown channel ("+msg.get_to() +") ? Ignored.", SERVER_ERR);
                          return;
                      }

                      //Verify if it's a non private message
                      if(msg.get_isPrivate() != false)
                      {
                          emit writeToLog("[" + m_soc->peerAddress().toString() + "Received a private message for a text channel ? Ignored.", SERVER_WARN);
                          //Send error packet to client
                          CPacket err("1","3");

                          err.Serialize_MessageError(1);
                          m_soc->write(err.GetByteArray());
                          m_soc->waitForBytesWritten();

                          break;
                      }
                      msg.toXML();
                      string filename = sha256(msg.toString().toStdString());

                      QDir test;
                      QString path = "storage/public/" +msg.get_to();

                      if(!test.exists(path))
                      {
                          if(!test.mkpath(path))
                          {
                              emit writeToLog("Can't create path [" + path + "]",SERVER_ERR);
                              //Send error to client
                              //TODO
                              break;
                          }
                      }

                      path = "storage/public/" + msg.get_to() + "/" + QString::fromStdString(filename) + ".xml";
                      if(QFile::exists(path))
                      {
                          emit writeToLog("[" + m_uuid.toString() + "(" + m_pseudo + ")] is spamming copy paste message in channel [" + msg.get_to() + "]",SERVER_WARN);
                          //Send error packet to client
                          CPacket err("1","3");

                          err.Serialize_MessageError(2);
                          m_soc->write(err.GetByteArray());
                          m_soc->waitForBytesWritten();

                          break;
                      }

                      QFile file(path);

                      if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
                      {
                          emit writeToLog("Can't save received message from [" + msg.get_from() + "] to [" + msg.get_to() + "]",SERVER_ERR);

                          CPacket err("1","3");

                          err.Serialize_MessageError(3);
                          m_soc->write(err.GetByteArray());
                          m_soc->waitForBytesWritten();

                          break;
                      }

                      QTextStream stream(&file);
                      stream << msg.toString();
                      file.close();

                      path = "storage/public/" +  msg.get_to()  + "/" + "index.json";

                      if(QFile::exists(path))
                      {
                          QList<QString> filename_list;
                          filename_list = readChannelIndex(path);
                          if(filename_list.isEmpty())
                          {
                              emit writeToLog("Error while reading channel index for channel [" + msg.get_to() + "]",SERVER_ERR);

                              CPacket err("1","3");

                              err.Serialize_MessageError(3);
                              m_soc->write(err.GetByteArray());
                              m_soc->waitForBytesWritten();

                              break;
                          }
                          else
                          {
                              if(filename_list.first() == "noIndex")
                              {
                                  emit writeToLog("No index in index.json from channel [" + msg.get_to() + "] Try to create one",SERVER_WARN);
                                  if(!createChannelIndex(filename,path))
                                  {
                                      emit writeToLog("Error while creating channel index from channel [" + msg.get_to() + "]",SERVER_ERR);

                                      CPacket err("1","3");

                                      err.Serialize_MessageError(3);
                                      m_soc->write(err.GetByteArray());
                                      m_soc->waitForBytesWritten();

                                      break;
                                  }
                                  else
                                  {
                                      emit writeToLog("Index created successfully for channel [" + msg.get_to() + "]",SERVER);
                                  }
                              }
                              else
                              {
                                  filename_list.append(QString::fromStdString(filename));
                                  if(!insertChannelIndex(path,filename_list))
                                  {

                                      emit writeToLog("Error while inserting in channel index for channel [" + msg.get_to() + "]",SERVER_ERR);
                                      CPacket err("1","3");

                                      err.Serialize_MessageError(3);
                                      m_soc->write(err.GetByteArray());
                                      m_soc->waitForBytesWritten();

                                      break;
                                  }
                              }
                          }
                      }
                      else
                      {
                          if(!createChannelIndex(filename,path))
                          {
                              emit writeToLog("Error while creating channel index from channel [" + msg.get_to() + "]",SERVER_ERR);

                              CPacket err("1","3");

                              err.Serialize_MessageError(3);
                              m_soc->write(err.GetByteArray());
                              m_soc->waitForBytesWritten();

                              break;
                          }
                      }
                      //Send to all
                      CPacket msgToAll("1","4");

                      msgToAll.Serialize_Message(msg);
                      emit sendToAll(msgToAll.GetByteArray());

                      break;
                  }

                  case 3:
                  {
                      QList<QString> info = packet->deserialize_messageRequest();

                      QList<CMessage> messages_list = createMessageList(info.at(1), false, info.at(2).toInt(), m_uuid, info.at(3).toInt());

                      if(messages_list.last().get_from() == "allIsSync")
                      {
                          break;
                      }

                      if(messages_list.last().get_from() == "noIndex")
                      {
                          CPacket reqAns("1","2");

                          reqAns.Serialize_MessageListInfo(info.at(1).toInt());
                          reqAns.Serialize_MessageList(messages_list);

                          m_soc->write(reqAns.GetByteArray());
                          m_soc->waitForBytesWritten();

                          break;
                      }


                      CPacket reqAns("1","2");

                      reqAns.Serialize_MessageListInfo(info.at(1).toInt());
                      reqAns.Serialize_MessageList(messages_list);

                      m_soc->write(reqAns.GetByteArray());
                      m_soc->waitForBytesWritten();

                      break;
                  }

                  case 4:
                  {
                      //Request profile picture
                      QString tmp = packet->deserialize_ppRequest();
                      if(tmp.size() != 38)
                      {
                          emit writeToLog("[" + m_uuid.toString(QUuid::WithoutBraces) + "(" + m_pseudo + ")] Error in request :", SERVER_ERR);
                          emit writeToLog(tmp, SERVER_ERR);
                          break;
                      }
                      QUuid requested(tmp);

                      QString path = "storage/private/" + requested.toString(QUuid::WithoutBraces) + "/pp.png";
                      if(QFile::exists(path))
                      {
                          QImage img(path);
                          CPacket ppAns("1","14");

                          ppAns.Serialize_ppAnswer(img,requested);
                          ppAns.GetByteArray().size();

                          m_soc->write(ppAns.GetByteArray());
                          m_soc->waitForBytesWritten();
                      }
                      else
                      {
                          emit writeToLog("Unable to find profile pic for user [" + requested.toString(QUuid::WithoutBraces) + "] Random new one generated" , SERVER_WARN);

                          int random = QRandomGenerator::global()->bounded(0,18);
                          path = "storage/server/pp/pp" + QString::number(random) + ".png";
                          if(QFile::exists(path))
                          {
                              QImage tmp(path);
                              QDir test;

                              if(test.exists("storage/private/" + requested.toString(QUuid::WithoutBraces)))
                              {
                                  tmp.save("storage/private/" + requested.toString(QUuid::WithoutBraces) + "/pp.png","PNG");
                              }
                              else
                              {
                                  test.mkpath("storage/private/" + requested.toString(QUuid::WithoutBraces));
                                  tmp.save("storage/private/" + requested.toString(QUuid::WithoutBraces) + "/pp.png","PNG");
                              }

                              CPacket ppAns("1","14");

                              ppAns.Serialize_ppAnswer(tmp,requested);
                              ppAns.GetByteArray().size();

                              m_soc->write(ppAns.GetByteArray());
                              m_soc->waitForBytesWritten();
                          }
                      }
                      break;
                  }

                  case 5:
                  {
                      //Create audio chan
                      CChannel * c = packet->Deserialize_newChannel();
                      emit updateChan(0,c);

                      break;
                  }

                  case 6:
                  {
                      //Delete audio chan
                      CChannel * c = packet->Deserialize_newChannel();
                      emit updateChan(1,c);

                      break;
                  }

                  case 7:
                  {
                      //Rename audio chan
                      CChannel * c = packet->Deserialize_newChannel();
                      emit updateChan(2,c);

                      break;
                  }

                  case 8:
                  {
                      //Modif max user (voc)
                      CChannel * c = packet->Deserialize_newChannel();
                      emit updateChan(3,c);

                      break;
                  }

                  case 9:
                  {
                      //kick user voc
                      //TODO
                      break;
                  }

                  case 10:
                  {
                      //Mute user voc (server side)
                      //TODO
                      break;
                  }

                  case 11:
                  {
                      //Create chan text --------> Qxmpp
                      //TODO
                      break;
                  }

                  case 12:
                  {
                      //Delete cahn text
                      //TODO
                      break;
                  }

                  case 13:
                  {
                      //Rename chan text
                      //TODO
                      break;
                  }

                  default:
                  {
                      emit writeToLog("Received invalid channel action :\n" + packet->GetByteArray() +"\nIgnored", SERVER_WARN);
                      break;
                  }
              }
              break;
          }

          case 2: //USER
          {
              switch (packet->GetAction().toInt())
              {
                  case 0:
                  {
                      //Mute (user side) ?????
                      //TODO
                      break;
                  }

                  case 1:
                  {
                      //Add friend --> later
                      //TODO
                      break;
                  }

                  case 2:
                  {
                      //Del friend
                      //TODO
                      break;
                  }

                  case 3:
                  {
                      //Send msg to friend
                      //TODO
                      break;
                  }

                  case 4:
                  {
                      //Modif pseudo (update bdd)
                      //TODO
                      break;
                  }

                  case 5:
                  {
                      //Change right
                      //TODO
                      break;
                  }

                  case 6:
                  {
                      //Request message list
                      QList<QString> info = packet->deserialize_messageRequest();
                      QList<CMessage> messages_list = createMessageList(info.at(1), true, info.at(2).toInt(), m_uuid, info.at(3).toInt());

                      if(messages_list.last().get_from() == "allIsSync")
                      {
                          break;
                      }
                      if(messages_list.last().get_from() == "noIndex")
                      {
                          break;
                      }

                      CPacket reqAns("2","6");

                      reqAns.Serialize_MessageList(messages_list);
                      m_soc->write(reqAns.GetByteArray());
                      m_soc->waitForBytesWritten();
                      break;
                  }

                  default:
                  {
                      emit writeToLog("Received invalid user action :\n" + packet->GetByteArray() +"\nIgnored", SERVER_WARN);
                      break;
                  }
              }
              break;
          }

          default:
          {
              emit writeToLog("Received invalid request type :\n" + packet->GetByteArray() +"\nIgnored", SERVER_WARN);
              break;
          }
      }
      return;
}

/**
 * @brief Create an index.json for an empty channel and add the first message of it
 * @param[in] filename should be the exacte file name of the message file
 * @param[in] path_to_index should be something like : "storage/[public|private]/[id]/index.json"
 * @return true if channel index has been created successfully, otherwise false
 */
bool CClient::createChannelIndex(string filename, QString path_to_index)
{
   QFile index(path_to_index);
   if(!index.open(QIODevice::WriteOnly | QIODevice::Text))
   {
       emit writeToLog("Can't open index of channel",SERVER_ERR);
       return false;
   }

   QJsonArray array;
   QJsonObject obj;
   obj.insert("id","0");
   obj.insert("filename",QString::fromStdString(filename));
   array.append(obj);

   QJsonObject main;
   main["index"] = array;

   QJsonDocument jsonIndex;
   jsonIndex.setObject(main);

   QTextStream streamIndex(&index);
   streamIndex << jsonIndex.toJson();
   index.close();
   return true;
}

/**
 * @brief returns the list of names of all messages files contained in the given index
 * @param[in] path_to_index should be something like : "storage/[public|private]/[id]/index.json"
 * @return A list of message filename
 * @retval A QList of QString countaining all message filename in the given index. Return an empty QList if no index or empty index was found
 */
QList<QString> CClient::readChannelIndex(QString path_to_index)
{
   QFile indexJson(path_to_index);
   QList<QString> null;

   if(!indexJson.open(QIODevice::ReadOnly | QIODevice::Text))
   {
       emit writeToLog("Can't open index of channel",SERVER_ERR);
       return null;
   }

   QByteArray json = indexJson.readAll();
   indexJson.close();
   QJsonDocument jsonIndex = QJsonDocument::fromJson(json);
   QJsonObject jsonObj = jsonIndex.object();

   if(jsonObj.contains("index"))
   {
       QJsonArray array = jsonObj.value("index").toArray();
       int index = 0;
       QList<QString> filename_list;

       while(!array.isEmpty())
       {
           QJsonObject tmp = array.first().toObject();
           if(tmp.value("id").toInt() == index)
           {
               filename_list.append(tmp.value("filename").toString());
           }
           else
           {
               emit writeToLog("Error in index.json not containing correct message index",SERVER_ERR);
               return null;
           }
           array.removeFirst();
           index++;
       }
       return filename_list;
   }
   else
   {
       emit writeToLog(("Error while reading index.json, no field \"index\" found"), SERVER_ERR);
       return null;
   }
}

/**
 * @brief Update index.json when inserting new message to it
 * @param[in] path_to_index should be something like : "storage/[public|private]/[id]/index.json"
 * @param[in] filename_list is the list of all filename of message stored into for the given channel, can be exctracted with readChannelIndex
 * @return true if the function had successfully updated the index, otherwise false
 */
bool CClient::insertChannelIndex(QString path_to_index, QList<QString> filename_list)
{
   QFile indexJson(path_to_index);
   if(!indexJson.open(QIODevice::WriteOnly | QIODevice::Text))
   {
       emit writeToLog("Can't open index of channel",SERVER_ERR);
       return false;
   }

   QJsonArray array;
   int index = 0;

   while(!filename_list.isEmpty())
   {
       QJsonObject obj;
       obj.insert("id", index);
       obj.insert("filename", filename_list.first());
       array.append(obj);
       filename_list.removeFirst();
       index++;
   }

   QJsonObject main;
   main["index"] = array;

   QJsonDocument jsonIndex;
   jsonIndex.setObject(main);

   QTextStream streamIndex(&indexJson);
   streamIndex << jsonIndex.toJson();
   indexJson.close();
   return true;
}

/**
 * @brief Create a QList of CMessage stored localy using index.json
 * @param[in] id  refers to id of channel or user for storage
 * @param[in] isPrivate  tells if it's a private message or not
 * @param[in] nb_msg_to_sync  to tell how much message you want to sync should be -1 to retrieve all
 * @param[in] sender  refere tu uuid of sender, used to gather private message
 * @param start_index  is used to retrieve older message, should be 0 if no message has already been sync
 * @return A list of message
 * @retval A Qlist of CMessage countaining all message selected according to given information, if an error has occured, return the error has name of the sender (field "from") in the first message of the QList
 */
QList<CMessage> CClient::createMessageList(QString id, bool isPrivate, int nb_msg_to_sync, QUuid sender, int start_index)
{
   QString default_path, path;
   QList<CMessage> message_list;
   int index;

   if(!isPrivate)
   {
       default_path = "storage/public/" + id + "/";
   }
   else
   {
       default_path = "storage/private/" + sender.toString(QUuid::WithoutBraces) + "/" + id + "/";
   }

   QList<QString> filename_list = readChannelIndex(default_path + "index.json");

   if(filename_list.isEmpty() || filename_list.last() == "no_index")
   {
       CMessage noIndex("noIndex","null","null",false);
       message_list.append(noIndex);
       return message_list;
   }

   if(start_index == 0)
   {
       index = nb_msg_to_sync;
   }
   else
   {
       if(start_index >= filename_list.size())
       {
           CMessage allIsSync("allIsSync","null","null",false);
           message_list.append(allIsSync);
           return message_list;
       }
       else
       {
           if((start_index + nb_msg_to_sync) >= filename_list.size())
           {
               index = filename_list.size() - 1;
           }
           else
           {
               index = start_index + nb_msg_to_sync;
           }
       }
   }

   if((nb_msg_to_sync == -1) | (nb_msg_to_sync >= filename_list.size() && start_index == 0))
   {
       foreach(QString filename, filename_list)
       {
           path = default_path + filename + ".xml";
           if(QFile::exists(path))
           {
               QFile file(path);
               if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
               {
                   emit writeToLog("Can't open message file [" + filename + "]",SERVER_ERR);
               }
               else
               {
                   CMessage tmp(QString::fromLocal8Bit(file.readAll()));
                   message_list.append(tmp);
               }
           }
           else
           {
               emit writeToLog("Message [" + filename + "] may have been deleted and not removed from index [" + id + "]",SERVER_ERR);
           }
       }
       return message_list;
   }
   else
   {
       for(int i = index; i > start_index; i--)
       {
           path = default_path + filename_list[filename_list.size() - i] + ".xml";
           if(QFile::exists(path))
           {
               QFile file(path);
               if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
               {
                   emit writeToLog("Can't open message file [" + filename_list[filename_list.size() - i] + "]",SERVER_ERR);
               }
               else
               {
                   CMessage tmp(QString::fromLocal8Bit(file.readAll()));
                   message_list.append(tmp);
               }
           }
           else
           {
               emit writeToLog("Message [" + filename_list[filename_list.size() - i] + "] may have been deleted and not removed from index [" + id + "]",SERVER_ERR);
           }
       }
       return message_list;
   }
}
