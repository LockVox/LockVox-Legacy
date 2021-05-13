#include "cmessage.h"

CMessage::CMessage(QString from, QString to, QString message,bool isPrivate)
{

    this->from = from;
    this->to = to;
    time = QTime::currentTime();
    date = QDate::currentDate();
    this->message = message;
    this->isPrivate = isPrivate;
    hasImg = false;

}

CMessage::CMessage(QString from, QString to, QString message,QImage img,bool isPrivate)
{

    this->from = from;
    this->to = to;
    time = QTime::currentTime();
    date = QDate::currentDate();
    this->message = message;
    this->isPrivate = isPrivate;
    hasImg = true;
    image = img;

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
    tmp = tmp[1].split("</private>\n");

    if(tmp[0] == "true")
    {
        isPrivate = true;
    }
    else
    {
        isPrivate = false;
    }

    tmp[1].remove("<hasImg>");
    tmp = tmp[1].split("</hasImg>\n");

    if(tmp[0] == "true")
    {
        hasImg = true;

        tmp[1].remove("<img>");
        tmp = tmp[1].split("</img>\n");

        QByteArray array = QByteArray::fromBase64(tmp[0].toLatin1());
        image.loadFromData(array);
    }
    else
    {
        hasImg = false;
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

QString CMessage::get_time()
{
    return time.toString("hh:mm:ss");
}

QString CMessage::get_date()
{
    return date.toString("d MMM yyyy");
}

QImage CMessage::get_image()
{
        return image;
}

QString CMessage::get_from_pseudo(){
    return from_pseudo;
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
    QDomElement sender; //Sender tag
    QDomElement receiver; //Receiver tag
    QDomElement sendedtime; //Time when message sended tag
    QDomElement sendeddate; //Date when message sended tag
    QDomElement content; // Content of message tag
    QDomElement pm; //Private tag
    QDomElement hp;  //Has picture tag
    QDomElement img; //Picture tag

    QDomText t_sender; //Sender
    QDomText t_receiver; //Receiver
    QDomText t_sendedtime; //Time when message sended
    QDomText t_sendeddate; //Date when message sended
    QDomText t_content; // The message
    QDomText t_pm; //if true private message if false chanel message
    QDomText t_hp; //if true message  got a picture
    QDomText t_img; //image to base64

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

    if(isPrivate)
    {
        t_pm = xmlmessage.createTextNode("true");
        pm.appendChild(t_pm);
    }
    else
    {
        t_pm = xmlmessage.createTextNode("false");
        pm.appendChild(t_pm);
    }

    hp = xmlmessage.createElement("hasImg");
    xmlmessage.appendChild(hp);

    if(hasImg)
    {
        t_hp = xmlmessage.createTextNode("true");
        hp.appendChild(t_hp);

        QByteArray array;
        QBuffer buffer(&array);
        image.save(&buffer, "JPG");

        img = xmlmessage.createElement("img");
        xmlmessage.appendChild(img);

        t_img = xmlmessage.createTextNode(QString::fromLatin1(array.toBase64()));
        img.appendChild(t_img);
    }
    else
    {
        t_hp = xmlmessage.createTextNode("false");
        hp.appendChild(t_hp);
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

void CMessage::getSenderPseudo(QVector<CClient *> clients)
{
    foreach(CClient * c, clients){
        if(c->get_uuid() == from){
            from_pseudo = c->get_pseudo();
            return;
        }
    }
}
