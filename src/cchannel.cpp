#include "cchannel.h"


/**
 * @brief Default constructor of the class CChannel.
 *
 * @see   CChannel::CChannel()
 */
CChannel::CChannel()
{
    m_name = "";
    m_id = 0;
    m_maxUsers = 5;
    m_nbClients = 0;
    m_messagesLists = new MessageList();
}

/**
 * @brief Copy constructor of the class CChannel
 * @overload
 * @param copy the attribut to copy
 *
 * @see   CChannel::CChannel()
 */
CChannel::CChannel(const CChannel & copy)
{
    m_name = copy.m_name;
    m_nbClients = copy.m_nbClients;
    m_maxUsers = copy.m_maxUsers;
    m_id = copy.m_id;
    m_messagesLists = copy.m_messagesLists;
}

/**
 * @brief Custom Constructor of the class CChannel
 * @overload
 * @param id the ID of the channel
 * @param name the name of the channel
 * @param maxUsers the maximum number of user in the channel
 * @see   CChannel::CChannel()
 */
CChannel::CChannel(QString name, int id, int maxUsers) : m_name(name)  , m_maxUsers(maxUsers), m_id(id)
{
    m_nbClients = 0;
    m_messagesLists = new MessageList();
}

/**
 * @brief Custom Constructor of the class CChannel
 * @overload
 * @param name the name of the channel
 * @param id the ID of the channel
 * @see   CChannel::CChannel()
 */
CChannel::CChannel(QString name, int id)
{
    m_name = name;
    m_id = id;
    m_nbClients = 0;
    m_messagesLists = new MessageList();
}

/**
 * @brief Custom Constructor of the class CChannel
 * @overload
 * @param name the name of the channel
 * @param id the ID of the channel
 * @param clients the lists of clients who are in the channel
 * @see   CChannel::CChannel()
 */
CChannel::CChannel(QList<CClient*> clients, QString name, int id)
{
    m_clients = clients;
    m_name = name;
    m_id = id;
    m_nbClients = m_clients.size();
    m_messagesLists = new MessageList();
}


/**
 * @brief Getter of attribute m_clients
 * @callergraph
 * @return m_clients the value of attribute m_clients
 * @retval QList<CClient *> the value of attribute m_clients
 */
QList<CClient *> CChannel::get_clients()
{
    return m_clients;
}

/**
 * @brief Getter of attribute m_name
 * @callergraph
 * @return m_name the value of attribute m_name
 * @retval QString the value of attribute m_name
 */
QString CChannel::get_name()
{
    return m_name;
}

/**
 * @brief Getter of attribute m_id
 * @callergraph
 * @return m_id the value of attribute m_id
 * @retval int the value of attribute m_id
 */
int CChannel::get_id()
{
    return m_id;
}

/**
 * @brief Getter of attribute m_maxUsers
 * @callergraph
 * @return m_maxUsers the value of attribute m_maxUsers
 * @retval int the value of attribute m_maxUsers
 */
int CChannel::get_maxUsers()
{
    return m_maxUsers;
}

/**
 * @brief Getter of attribute m_nbClients
 * @callergraph
 * @return m_nbClients the value of attribute m_nbClients
 * @retval int the value of attribute m_nbClients
 */
int CChannel::get_nbClients()
{
    return m_nbClients;
}

/**
 * @brief Getter of attribute m_messagesLists
 * @callergraph
 * @return m_messagesLists the value of attribute m_messagesLists
 * @retval MessageList the value of attribute m_messagesLists
 */
MessageList *CChannel::getMessagesLists() const
{
    return m_messagesLists;
}

/**
 * @brief Setter of attribute m_clients
 * @callgraph
 * @param clients the list of clients who are in the channel
 * @exception range_error if clients is empty
 */
void CChannel::set_clients(QList<CClient*> clients)
{
    if(!clients.isEmpty())
        m_clients = clients;
}

/**
 * @brief Setter of attribute m_name
 * @callgraph
 * @param name the name of the channel
 * @exception range_error if name is empty
 */
void CChannel::set_name(QString name)
{
    if(!name.isEmpty())
        m_name = name;
}

/**
 * @brief Setter of attribute m_id
 * @callgraph
 * @param id the ID of the channel
 * @exception range_error if ID is lower than 0
 */
void CChannel::set_id(int id)
{
    if(id >= 0)
        m_id = id;
}

/**
 * @brief Setter of attribute m_maxUsers
 * @callgraph
 * @param maxUsers the maximum number of users in the channel
 * @exception
 */
void CChannel::set_maxUsers(int maxUsers)
{
    m_maxUsers = maxUsers;
}

/**
 * @brief Setter of attribute m_nbClients
 * @callgraph
 * @param nb the number of users in the channel
 * @exception range_error if the number of users is greater than the maximum number of users in the channel
 */
void CChannel::set_nbClients(int nb)
{
    if(nb <= m_maxUsers)
        m_nbClients = nb;
}

/**
 * @brief Setter of attribute m_messagesLists
 * @callgraph
 * @param messagesList the list of messages associate to this channel
 * @exception
 */
void CChannel::setMessagesLists(MessageList *messagesList)
{
    m_messagesLists = messagesList;
}

/**
 * @brief Set the different attribute of a CChannel from another an existing instance of CChannel.
 * @param[in]       c  the CChannel instance which contains the attribute to copy.
 */
void CChannel::set_all(CChannel * c)
{
    this->set_id(c->get_id());
    this->set_name(c->get_name());
    this->set_maxUsers(c->get_maxUsers());
    this->set_nbClients(c->get_nbClients());
}


/**
 * @brief Serialize attributes of an user into a JSON object.
 * @param[out]      obj  the JSON object which contains the attribute of the channel.
 */
QJsonObject CChannel::serializeToObj()
{
    QJsonObject obj;
    obj["id"]= this->get_id();
    obj["name"]= this->get_name();
    obj["maxUsers"]= this->get_maxUsers();
    obj["nbClients"]= this->get_nbClients();
    return obj;
}

/**
 * @brief Deserialize a JSON object into the current instance of CChannel.
 * @param[in]       json_obj  the JSON object to deserialize into this CChannel instance.
 */
void CChannel::deserialize(QJsonObject json_obj)
{
    this->set_id(json_obj["id"].toInt());
    this->set_name(json_obj["name"].toString());
    this->set_maxUsers(json_obj["maxUsers"].toInt());
    this->set_nbClients(json_obj["nbClients"].toInt());
}

/**
 * @brief Add an user to the list of users of this CChannel instance.
 * @param[in]       c the user to add to the list of users
 */
void CChannel::addUser(CClient * c)
{
    //Add the user
    m_clients.append(c);
    //Update the number of users
    m_nbClients = m_clients.size();
}

/**
 * @brief Remove an user to the list of users of this CChannel instance.
 * @param[in]       idUser the ID of the user to remove to the list of users
 */
void CChannel::delUser(QUuid idUser)
{
    //Look for the index of the user to remove
   for(int i = 0; i < m_clients.size(); i++)
   {
       if(m_clients[i]->get_uuid() == idUser)
       {
           //Remove the user
           m_clients.removeAt(i);
       }
   }
   //Update the number of users
   m_nbClients = m_clients.size();
}

