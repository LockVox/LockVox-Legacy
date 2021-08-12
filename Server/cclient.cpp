/**
 * @file        cclient.cpp
 * @brief       Definition of class CClient.
 * @author      LockVox Team
 * @version     0.2.0
 * @date        2021
 */

//Includes
#include "cclient.h"
#include "config.h"


/**
 * @brief Default constructor of the class CClient.
 * @see   CCllient::Client()
 */
CClient::CClient()
{
    setObjType(ObjectType::CLIENT);

    m_pseudo = "";
    m_soc = NULL;
    m_idChannel = 0;
    m_isOnline = false;
    m_buffer = new QByteArray;
    m_mail ="";
    m_uuid = NULL;
    m_packetManager = new CPacketManager();
}

/**
 * @brief Copy constructor of the class CClient
 * @overload
 * @param copy the attribut to copy
 * @see   CClient::CClient()
 */
CClient::CClient(const CClient & copy)
{ 
    setObjType(ObjectType::CLIENT);
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
    setObjType(ObjectType::CLIENT);

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
    m_packetManager = new CPacketManager();

#ifdef _DEBUG
     qDebug() << "[CREATE CLIENT] " << m_pseudo << " - " << m_uuid;
#endif
}

/**
 * @brief Custom Constructor of the class CClient
 * @overload
 * @param soc the socket used by the user
 * @see   CClient::CClient()
 */
CClient::CClient( QTcpSocket * soc)
{
    setObjType(ObjectType::CLIENT);
    m_soc = soc;
    if(m_soc != NULL)
    {
        connect(m_soc, SIGNAL(readyRead()), this, SLOT(onReceiveData()));
    }
    m_buffer = new QByteArray;
    m_packetManager = new CPacketManager();
}

/**
 * @brief Default desctructor of the class CClient.
 *
 * @see   CCllient::Client()
 */
CClient::~CClient()
{

#ifdef _DEBUG
    qDebug() << "[DEL CLIENT] " << m_pseudo << " - " << m_uuid;
#endif

    if(m_soc != NULL)
    {
        //m_soc->disconnectFromHost();
        //m_soc->~QTcpSocket();
        //m_soc = NULL;
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

void CClient::fromJSON(QJsonObject obj)
{
    try{
            m_uuid = QUuid::fromString(obj.value("uuid").toString());
            m_pseudo = obj.value("pseudo").toString();
            m_isOnline = obj.value("isOnline").toBool();
            m_description = obj.value("description").toString();
    }
    catch(char* e)
    {
        qDebug() << "Error while deserializing :" << e << Qt::endl;
    }
}

QJsonObject CClient::toJSON()
{
    QJsonObject obj;

    obj.insert("uuid", m_uuid.toString());
    obj.insert("pseudo", m_pseudo);
    obj.insert("isOnline", m_isOnline);
    obj.insert("description", m_description);

    return obj;
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


/**
 * @brief Deserialize a JSON object into the current instance of CClient.
 * @param[in]       json_obj  the JSON object to deserialize into this CClient instance.
 */

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

CPacketManager *CClient::getPacketManager() const
{
    return m_packetManager;
}

void CClient::setPacketManager(CPacketManager *packetManager)
{
    m_packetManager = packetManager;
}

void CClient::writePacket(CPacket packet)
{
    m_packetManager->sendPacket(packet, this, this);
}

CSessionCookie *CClient::getSessionCookie()
{
    return m_sessionCookie;
}

void CClient::setSessionCookie(CSessionCookie *sessionCookie)
{
    m_sessionCookie = sessionCookie;
}

/**
 * @brief This slot is called when client TCP socket send signal "readyRead", it pre process the data received and call function processData
 */
void CClient::onReceiveData()
{

#ifdef NETWORK_DEBUG
          qDebug() << "[NETWORK] - Data received from " << m_pseudo << " " << m_uuid;
#endif
    QByteArray data;
    data.append(m_soc->readAll());

    QDataStream ds(&data,QIODevice::ReadOnly);
    quint32 size;

    //Get packet size
    ds >> size;

#ifdef NETWORK_DEBUG
    qDebug() << "SIZE OF RECEIVE PACKET:" << size;
    qDebug() << "DATA RECEIVED:" << *data;
#endif

    char *p;
    ds.readBytes(p, size);

    //Create and parse packet
    CPacket packet(p, new CClient());

    //Process received data
    m_packetManager->receivePacket(packet);
    //processData(packet);
}

/**
 * @brief This function is called by onReceiveData to process received data in order to manage cases defined by the application protocol
 * @param[in] data  The QByteArray object received from the TCP socket
 */
void CClient::processData(CPacket packet) //Process received data
{

      if(packet.GetAction() == NULL || packet.GetType() == NULL)//Bad packet
      {
#ifdef _DEBUG
          qDebug() << "[PROCESSING] - Unable to deserialize packet";
#endif
          emit writeToLog("Unable to deserialize received packet :\n" + packet.GetByteArray() + "\nRequest Aborted", SERVER_WARN);
          return;
      }

      if(packet.GetAction().toInt() == -1 && packet.GetType().toInt() == -1) //Request server object
      {
#ifdef _DEBUG
          qDebug() << "[CLIENT] " << this->get_pseudo() << " - Server informations request ";
#endif
          emit sendYou(m_soc);
          return;
      }

      //Get type
      switch (packet.GetType().toInt())
      {

          case 1: //CHAN
          {
              switch (packet.GetAction().toInt())
              {
                  case 0:
                  {
                      //JOIN CHANNEL
                      packet.Deserialize_ID();
/*
                     CChannel * channel = emit whichChan(packet.get_IdChannel());

                      if(channel)
                      {
                          m_idChannel = channel->get_id();
                          channel->addUser(this);

                          CPacket ans("1","0");

                          ans.Serialize();
                          ans.Serialize_ID(channel->get_id(),m_uuid);

                          emit sendToAll(ans.GetByteArray());

                          emit writeToLog("User [" + m_uuid.toString() + "(" + m_pseudo + ")] has joined channel id ["+ QString::number(packet.get_IdChannel()) + "]", USER);
                      }
                      else
                      {
                          if(!channel)
                          {
                              emit writeToLog("User [" + m_uuid.toString() + "(" + m_pseudo + ")] is trying to join a non-existing channel id [" + QString::number(packet.get_IdChannel()) + "]", SERVER_WARN);
                              //Send error packet
                              //TODO
                          }
                      }
                      break;
                      */
                  }

                  case 1:
                  {
                      //QUIT CHAN
                      packet.Deserialize_ID();

                      //CChannel * channel = emit whichChan(packet.get_IdChannel());
/*
                      if(channel)
                      {
                          m_idChannel = -1;
                          channel->delUser(m_uuid);

                          CPacket ans("1","1");

                          ans.Serialize();
                          ans.Serialize_ID(channel->get_id(),m_uuid);

                          emit sendToAll(ans.GetByteArray());

                          emit writeToLog("User [" + m_uuid.toString() + "(" + m_pseudo + ")] has quit channel id ["+ QString::number(packet.get_IdChannel()) + "]", USER);
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
                      */
                  }

                  case 2:
                  {
                      //Text channel message
                      CMessage msg = packet.Deserialize_Message();

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
                      QList<QString> info = packet.deserialize_messageRequest();

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
                      QString tmp = packet.deserialize_ppRequest();
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
                      CChannel * c = packet.Deserialize_newChannel();
                      emit updateChan(0,c);

                      break;
                  }

                  case 6:
                  {
                      //Delete audio chan
                      CChannel * c = packet.Deserialize_newChannel();
                      emit updateChan(1,c);

                      break;
                  }

                  case 7:
                  {
                      //Rename audio chan
                      CChannel * c = packet.Deserialize_newChannel();
                      emit updateChan(2,c);

                      break;
                  }

                  case 8:
                  {
                      //Modif max user (voc)
                      CChannel * c = packet.Deserialize_newChannel();
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
                      emit writeToLog("Received invalid channel action :\n" + packet.GetByteArray() +"\nIgnored", SERVER_WARN);
                      break;
                  }
              }
              break;
          }

          case 2: //USER
          {
              switch (packet.GetAction().toInt())
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
                      QList<QString> info = packet.deserialize_messageRequest();
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
                      emit writeToLog("Received invalid user action :\n" + packet.GetByteArray() +"\nIgnored", SERVER_WARN);
                      break;
                  }
              }
              break;
          }

          default:
          {
              emit writeToLog("Received invalid request type :\n" + packet.GetByteArray() +"\nIgnored", SERVER_WARN);
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
 * @param[in] filename_list is the list of all filename of message stored into for the given channel, can be exctracted with CClient::readChannelIndex
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
 * @param[in] start_index  is used to retrieve older message, should be 0 if no message has already been sync
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



bool CClient::operator==(const CClient &client) const{
    if(this->m_uuid == client.m_uuid){
        return true;
    }
    return false;
}

bool CClient::operator==(const QTcpSocket *socket) const{
    if(this->m_soc == socket){
        return true;
    }
    return false;
}
