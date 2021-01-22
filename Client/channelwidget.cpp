#include "channelwidget.h"
#include "ui_channelwidget.h"



#include <QtGui>
#include <QWidget>
#include <QLayout>

channelWidget::channelWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::channelWidget)
{
    ui->setupUi(this);
}

channelWidget::channelWidget(QWidget *parent,CChannel* channel):
    QWidget(parent),
    ui(new Ui::channelWidget)
{
    ui->setupUi(this);
    this->channel = channel;

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->channel_name = channel->get_name();
    ui->channel_name->setText(this->channel_name);


    client_layout = new QVBoxLayout(this);
    setLayout(client_layout);

}

channelWidget::~channelWidget()
{
    delete ui;
}


void channelWidget::on_button_clicked()
{
    // go to channel;
    RequestServer(1,0, NULL,channel);

}

void channelWidget::mousePressEvent(QMouseEvent *event)
{
        //stuff
    // go to channel;
    RequestServer(1,0, NULL,channel);
}

void channelWidget::on_AddClient(CClient * c){
    Client_customWidget * newClient = new Client_customWidget(this,c);
    client_layout->addWidget(newClient);
    clients.append(newClient);

    setLayout(client_layout);
}
void channelWidget::on_DelClient(CClient * c){
    for(int i = 0; i < clients.size(); i++){
          if(clients[i]->get_client()->get_id() == c->get_id())
          {
              QLayoutItem* item;
              while ( ( item = client_layout->takeAt( 0 ) ) != NULL )
              {
                  if(item->widget() == clients[i]){
                      delete item->widget();
                      delete item;
                  }
              }
              clients.removeAt(i);
          }
    }
}

void channelWidget::Update(){
    if(channel->get_clients().isEmpty()){
        return;
    }

    for(int i = 0; i < channel->get_clients().size(); i++){
        bool exist = false;
        for(int j = 0; j < clients.size(); j++){
            if(clients[j]->get_client()->get_id() == channel->get_clients()[i]->get_id()){
                exist = true;
            }
        }
        if(!exist && channel->get_clients()[i]->get_idChannel() == channel->get_id()){
            on_AddClient(channel->get_clients()[i]);
            qDebug() << "Action : add " << channel->get_clients()[i]->get_pseudo() << " in " << channel->get_name();
         }
        if(exist && channel->get_clients()[i]->get_idChannel() != channel->get_id() ){
            on_DelClient(channel->get_clients()[i]);
             qDebug() << "Action : rm " << channel->get_clients()[i]->get_pseudo() << " in " << channel->get_name();
        }
   }

}
