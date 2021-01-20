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


}





channelWidget::~channelWidget()
{
    delete ui;
}



void channelWidget::on_button_clicked()
{
    // go to channel;



}
