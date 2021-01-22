#ifndef CLIENT_CUSTOMWIDGET_H
#define CLIENT_CUSTOMWIDGET_H

#include <QWidget>

#include "src\includes\cclient.h"

namespace Ui {
class Client_customWidget;
}

class Client_customWidget : public QWidget
{
    Q_OBJECT

public:
    explicit Client_customWidget(QWidget *parent = nullptr);
    Client_customWidget(QWidget *parent, CClient * c);
    ~Client_customWidget();

    CClient * get_client(){
        return m_client;
    }


private:
    Ui::Client_customWidget *ui;
    CClient * m_client;

};

#endif // CLIENT_CUSTOMWIDGET_H
