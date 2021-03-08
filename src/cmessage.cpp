#include "src/includes/cmessage.h"

CMessage::CMessage(QString from, QString to, QString message,bool isPrivate)
{

    this->from = from;
    this->to = to;
    time = QTime::currentTime();
    date = QDate::currentDate();
    this->message = message;
    this->isPrivate = isPrivate;

}

CMessage::CMessage(QString xml)
{
    xml.remove("<from>");
    QStringList tmp = xml.split("</from>\n");
    from = tmp[0];

    tmp[1].remove("<to>");
    tmp = tmp[1].split("</to>\n");
    to = tmp[0];

    tmp[1].remove("<time>");
    tmp = tmp[1].split("</time>\n");
    time = QTime::fromString(tmp[0], "hh:mm:ss");

    tmp[1].remove("<date>");
    tmp = tmp[1].split("</date>\n");
    date = QDate::fromString(tmp[0],"d MMM yyyy");

    tmp[1].remove("<content>");
    tmp = tmp[1].split("</content>\n");
    message = tmp[0];


    tmp[1].remove("<private>");
    tmp = tmp[1].split("</private>");

    if(tmp[0] == "true")
    {
        isPrivate = true;
    }
    else
    {
        isPrivate = false;
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

QString CMessage::get_message()
{
    return message;
}

bool CMessage::get_isPrivate()
{
    return isPrivate;
}


QDomDocument CMessage::get_xmlmessage()
{
    if(xmlmessage.isNull())
    {
        qDebug() << "toXML() method must be called first !" << Qt::endl;
        return QDomDocument();
    }
    else
    {
        return xmlmessage;
    }
}

//Methods

void CMessage::toXML()
{
    sender = xmlmessage.createElement("from");
    xmlmessage.appendChild(sender);

    t_sender = xmlmessage.createTextNode(from);
    sender.appendChild(t_sender);

    receiver = xmlmessage.createElement("to");
    xmlmessage.appendChild(receiver);

    t_receiver = xmlmessage.createTextNode(to);
    receiver.appendChild(t_receiver);

    sendedtime = xmlmessage.createElement("time");
    xmlmessage.appendChild(sendedtime);

    t_sendedtime = xmlmessage.createTextNode(time.toString("hh:mm:ss"));
    sendedtime.appendChild(t_sendedtime);

    sendeddate = xmlmessage.createElement("date");
    xmlmessage.appendChild(sendeddate);

    t_sendeddate = xmlmessage.createTextNode(date.toString("d MMM yyyy"));
    sendeddate.appendChild(t_sendeddate);

    content = xmlmessage.createElement("content");
    xmlmessage.appendChild(content);

    t_content = xmlmessage.createTextNode(message);
    content.appendChild(t_content);

    pm = xmlmessage.createElement("private");
    xmlmessage.appendChild(pm);

    if(this->isPrivate)
    {
        t_pm = xmlmessage.createTextNode("true");
        pm.appendChild(t_pm);
    }

    else
    {
        t_pm = xmlmessage.createTextNode("false");
        pm.appendChild(t_pm);
    }

    return;
}

QString CMessage::toString()
{
    if(xmlmessage.isNull())
    {
        qDebug() << "toXML() method must be called first !" << Qt::endl;
        return "null";
    }
    else
    {
        return xmlmessage.toString();
    }
}
