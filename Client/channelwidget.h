#ifndef CHANNELWIDGET_H
#define CHANNELWIDGET_H

#include <QWidget>
#include <QString>
#include "src/includes/cchannel.h"
#include "client_customwidget.h"

#include <QPushButton>
#include <QLayout>
#include <QLabel>

namespace Ui {
class channelWidget;
}

class channelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit channelWidget(QWidget *parent = nullptr);
    channelWidget(QWidget *parent,CChannel *Channel); // constructor
    ~channelWidget();

    CChannel * get_channel(){
        return channel;
    }

    void on_AddClient(CClient * c);
    void on_DelClient(CClient * c);

    void Update();

private slots:
    void on_button_clicked();
    void mousePressEvent(QMouseEvent *event);




signals:
    void RequestServer(int,int,CClient*,CChannel*);
private:
    Ui::channelWidget *ui;
    QVBoxLayout * client_layout;
    QList<Client_customWidget*> clients;


    QString channel_name;
    QString channel_pic;
    CChannel *channel;
    int id_channel;

};

#endif // CHANNELWIDGET_H
