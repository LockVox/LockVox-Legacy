#include "channelwidget.h"
#include "ui_channelwidget.h"

#include <QtGui>
#include <QWidget>
#include <QLayout>

channelWidget::channelWidget(QWidget *parent) :
    QPushButton(parent),
    ui(new Ui::channelWidget)
{
    ui->setupUi(this);

}

channelWidget::channelWidget(QWidget *parent,CChannel* channel):
    QPushButton(parent),
    ui(new Ui::channelWidget)
{
    ui->setupUi(this);
    this->channel = channel;

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->channel_name = channel->get_name();
    ui->channel_name->setText(this->channel_name);

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
