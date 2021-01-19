#include "src/includes/cmessage.h"

CMessage::CMessage()
{

}

CMessage::~CMessage()
{

}

//Getters
QString CMessage::get_from()
{
    return from;
}

QString CMessage::get_to()
{
    return to;
}

QDomDocument CMessage::get_message()
{
    return message;
}

bool CMessage::get_isPrivate()
{
    return isPrivate;
}

//Setters
void CMessage::set_from(QString from)
{
    this->from = from;
}

void CMessage::set_to (QString to)
{
    this->to = to;
}

void CMessage::set_message(QDomDocument message)
{
    this->message = message;
}

void CMessage::set_isPrivate (bool isPrivate)
{
    this->isPrivate = isPrivate;
}
