#include "client_customwidget.h"
#include "ui_client_customwidget.h"

Client_customWidget::Client_customWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client_customWidget)
{
    ui->setupUi(this);


}

Client_customWidget::Client_customWidget(QWidget *parent, CClient *c) :
    QWidget(parent),
    ui(new Ui::Client_customWidget)
{
    ui->setupUi(this);
    m_client = c;

    if(m_client){
        ui->label->setText(m_client->get_pseudo());
     }
}

Client_customWidget::~Client_customWidget()
{
    delete ui;
}
