#include "src/includes/cmessage.h"

CMessage::CMessage(QString from, QString to,bool isPrivate)
{
    this->from = from;
    this->to = to;
    this->isPrivate = isPrivate;

    sender = message.createElement("from");
    message.appendChild(sender);

    t_sender = message.createTextNode(this->from);
    sender.appendChild(t_sender);

    receiver = message.createElement("to");
    message.appendChild(receiver);

    t_receiver = message.createTextNode(this->to);
    receiver.appendChild(t_receiver);

    sendedtime = message.createElement("time");
    message.appendChild(sendedtime);

    time = QTime::currentTime();

    t_sendedtime = message.createTextNode(time.toString("hh:mm:ss"));
    sendedtime.appendChild(t_sendedtime);

    sendeddate = message.createElement("date");
    message.appendChild(sendeddate);

    date = QDate::currentDate();

    t_sendeddate = message.createTextNode(date.toString("d MMM yyyy"));
    sendeddate.appendChild(t_sendeddate);

    content = message.createElement("content");
    message.appendChild(content);

    pm = message.createElement("private");
    message.appendChild(pm);

    if(this->isPrivate)
    {
        t_pm = message.createTextNode("true");
        pm.appendChild(t_pm);
    }

    else
    {
        t_pm = message.createTextNode("false");
        pm.appendChild(t_pm);
    }
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


QDomText CMessage:: get_tsender()
{
    return t_sender;
}

QDomText CMessage::get_treceiver()
{
    return t_receiver;
}

QDomText CMessage::get_tsendedtime()
{
    return t_sendedtime;
}

QDomText CMessage::get_tsendeddate()
{
    return t_sendeddate;
}

QDomText CMessage::get_tcontent()
{
    return t_content;
}

QDomText CMessage::get_tpm()
{
    return t_pm;
}


//Setters

void CMessage::set_text(QDomText text)
{
    t_content = text;
    content.appendChild(t_content);
}

void CMessage::set_text(QString text)
{
    t_content = message.createTextNode(text);
    content.appendChild(t_content);
}
