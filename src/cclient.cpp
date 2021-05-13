/**
 * @file cclient.cpp
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
}

/**
 * @brief Copy constructor of the class CClient
 * @overload
 * @param copy the attribut to copy
 *
 * @see   CClient::CClient()
 */
CClient::CClient( const CClient & copy)
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
    m_idChannel = idChannel;
    m_isOnline = online;
    m_description = description;
    m_isAuthenticate = false;
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
 * @param[out]      obj  the JSON object which contains the attribute of the user.
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









