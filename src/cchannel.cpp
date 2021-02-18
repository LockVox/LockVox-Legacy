#include "src/includes/cchannel.h"

/*
@description : Default constructor of a channel
@param : none
@return : none
*/
CChannel::CChannel()
{
    m_name = "";
    m_id = 0;
    m_maxUsers = 5;
}

/*
@description : Copy constructor of a channel
@param : channel to copy
@return : none
*/
CChannel::CChannel(const CChannel & copy){
    m_name = copy.m_name;
    m_nbClients = copy.m_nbClients;
    m_maxUsers = copy.m_maxUsers;
    m_id = copy.m_id;
}

/*
@description : Customize constructor of a channel
@param : Informations of the channel
@return : none
*/
CChannel::CChannel(QString name, int id, int maxUsers) : m_name(name) , m_id(id) , m_maxUsers(maxUsers)
{


}

/*
@description : Customize constructor of a channel
@param : Informations of the channel
@return : none
*/
CChannel::CChannel(QString name, int id)
{
    m_clients.clear();
    m_name = name;
    m_id = id;

}

/*
@description : Customize constructor of a channel
@param : Informations of the channel
@return : none
*/
CChannel::CChannel(QList<CClient*> clients, QString name, int id)
{
    m_clients = clients;
    m_name = name;
    m_id = id;

}


/*
@description : return the list of clients
@param : none
@return : list of clients
*/
QList<CClient *> CChannel::get_clients()
{
    return m_clients;
}

/*
@description : return the name of the channel
@param : none
@return : name of the channel
*/
QString CChannel::get_name(){
    return m_name;
}

/*
@description : return the id of the channel
@param : none
@return : id of the channel
*/
int CChannel::get_id()
{
    return m_id;
}

/*
@description : return the maximum number of users in a channel
@param : none
@return : maximum users in a channel
*/
int CChannel::get_maxUsers(){
    return m_maxUsers;
}

/*
@description : return the number of clients in a channel
@param : none
@return : number of clients in a channel
*/
int CChannel::get_nbClients(){
    return m_nbClients;
}

/*
@description : set the list of clients of the channel
@param : list of clients
@return : none
*/
void CChannel::set_clients(QList<CClient*> clients){
    m_clients = clients;
}

/*
@description : set the name of the channel
@param : name of the channel
@return : none
*/
void CChannel::set_name(QString name){
    m_name = name;
}

/*
@description : set the id of the channel
@param : channel id
@return : none
*/
void CChannel::set_id(int id){
    m_id = id;
}

/*
@description : set the maximum users in a channel
@param : maximum number of users in a channel
@return : none
*/
void CChannel::set_maxUsers(int maxUsers){
    m_maxUsers = maxUsers;
}

/*
@description : set the number of clients in a channel
@param : number of clients
@return : none
*/
void CChannel::set_nbClients(int nb){
    m_nbClients = nb;
}

/*
@description : set fields of channel class from another channel
@param : a channel
@return : none
*/
void CChannel::set_all(CChannel * c){
    this->set_id(c->get_id());
    this->set_name(c->get_name());
    this->set_maxUsers(c->get_maxUsers());
    this->set_nbClients(c->get_nbClients());
}


/*
@description : Serialize the fields of a channel into JSON Obj
@param : none
@return : JSON Object
*/
QJsonObject CChannel::serializeToObj(){
    QJsonObject obj;
    obj["id"]= this->get_id();
    obj["name"]= this->get_name();
    obj["maxUsers"]= this->get_maxUsers();
    obj["nbClients"]= this->get_nbClients();
    return obj;
}

/*
@description : Deserialize a JSON Object and fill the channel informations
@param : JSON Object
@return : none
*/
void CChannel::deserialize(QJsonObject json_obj){
    this->set_id(json_obj["id"].toInt());
    this->set_name(json_obj["name"].toString());
    this->set_maxUsers(json_obj["maxUsers"].toInt());
    this->set_nbClients(json_obj["nbClients"].toInt());
}

/*
@description : Add a client to the list of clients
@param : reference to a client
@return : none
*/
void CChannel::addUser(CClient * c){
    m_clients.append(c);
}

/*
@description : Delete a client from the list of clients
@param : id of the client
@return : none
*/
void CChannel::delUser(int idUser){
   for(int i = 0; i < m_clients.size(); i++){
       if(m_clients[i]->get_id() == idUser){
           m_clients.removeAt(i);
       }
   }
}
