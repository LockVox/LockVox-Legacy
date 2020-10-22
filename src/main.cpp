#include "src/includes/qma.h"
#include "src/includes/lockvoxmainwindow.h"

#include <QApplication>
#include <iostream>


#define PROTOBUF_USE_DDLS

int main(int argc, char *argv[])
{
    QApplication Main(argc, argv);

    QString locale = QLocale::system().name().section('_', 0, 0);

    QTranslator translator;
    translator.load(QString("LockVox_") + locale);
    Main.installTranslator(&translator);



    LockVoxMainWindow w;
    w.setWindowTitle("LockVox");
    w.setWindowIcon(QIcon(":/pictures/icon.png"));
    w.show();

    CServer * server = new CServer();


    CClient *client = new CClient(5,"taga", NULL, 5);
    server->addClient(client);

    CChannel *channel = new CChannel();
    CChannel *channel2 = new CChannel();
    CChannel *channel3 = new CChannel();
    QList<CChannel> final;

    channel->set_name("channel11111111111111111111111111");
    channel->set_maxUsers(3);
    channel->set_nbClients(10);
    channel->set_id(7);

    channel2->set_name("channel2");
    channel2->set_maxUsers(5);
    channel2->set_nbClients(4);
    channel2->set_id(8);

    channel3->set_name("channel3");
    channel3->set_maxUsers(7);
    channel3->set_nbClients(7);
    channel3->set_id(9);

    server->addChannel(channel);
    server->addChannel(channel2);
    server->addChannel(channel3);



    /*
    QByteArray out = server->SerializeChannels();
    qDebug() << "packet :" <<out << Qt::endl;

    QByteArray in;
    server->DeserializeChannels(in);
*/

/*
    qDebug() << "Start serialization" << Qt::endl;

    server->ChannelsToDatastream(stream_out);
    qDebug() << "Serialization done" << Qt::endl;
    stream_in << stream_out;

    server->DatastreamToChannels(stream_in);
    qDebug() << server->get_channelList().length();
    foreach(CChannel * c, server->get_channelList())
        qDebug() << "Set channel -\nName :" << c->get_name() << "\nID : " <<c->get_id() << "\nMax users: " << c->get_maxUsers() << "\nNb Users : " << c->get_nbClients() << Qt::endl;
    qDebug() << "Deseralization done" << Qt::endl;

*/

/*
    QVariant serialize_in ;
    QVariant serialize_out = QVariant::fromValue(server->get_staticChannelList());

    serialize_out.canConvert(QVariant::List); //returns true

    QVariantList list;
    QSequentialIterable it= serialize_out.value<QSequentialIterable>();
    for( const QVariant &v : it)
        list << v;
    stream_out << serialize_out;
    stream_in >>serialize_in;

    QList<CChannel> deserializeList;
    foreach(QVariant v, serialize_in.value<QVariantList>())
        deserializeList << v.value<CChannel>();




   // final = serialize_in.value<QList<CChannel>>();

    for(int j = 0; j < 3; j++ )
    {
        qDebug() << "test";
        qDebug() << deserializeList[j].get_name() << "\nmax users:" << deserializeList[j].get_maxUsers() << "\n nbclients : " << deserializeList[j].get_nbClients() <<"\n id:" << deserializeList[j].get_id();
    }*/

    /*stream_out << server->m_channels.length();
    for(int i = 0; i < 3; i++)
    {
        serialize_out = QVariant::fromValue(*server->m_channels[i]);
        stream_out << serialize_out;
        serialize_out = NULL;
    }

    //serialize_out = QVariant::fromValue(client);
    int length;
    stream_in >> length;
    stream_in >>serialize_in;


    //final = serialize_in.value<CClient>();
    for(int j = 0; j <length; j++ )
    {
        final = serialize_in.value<CChannel>();
        qDebug() << final.get_name() << "\nmax users:" << final.get_maxUsers() << "\n nbclients : " << final.get_nbClients() <<"\n id:" << final.get_id();
    }
    //qDebug() << final.get_pseudo();

    //if(final.get_pseudo() == client.get_pseudo() && final.get_idChannel() == client.get_idChannel()){

     //   qDebug() << "Seems working";

    //}*/

    //server->set_database(new CDatabase());

    return Main.exec();
}

