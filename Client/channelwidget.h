#ifndef CHANNELWIDGET_H
#define CHANNELWIDGET_H

#include <QWidget>
#include <QString>
#include "src/includes/cchannel.h"

#include <QPushButton>

namespace Ui {
class channelWidget;
}

class channelWidget : public QPushButton
{
    Q_OBJECT

public:
    explicit channelWidget(QWidget *parent = nullptr);
    channelWidget(QWidget *parent,CChannel *Channel); // constructor
    ~channelWidget();

private slots:
    void on_button_clicked();

private:
    Ui::channelWidget *ui;
    QString channel_name;
    QString channel_pic;
    CChannel *channel;
    int id_channel;

};

#endif // CHANNELWIDGET_H
