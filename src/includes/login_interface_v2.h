#ifndef LOGIN_INTERFACE_V2_H
#define LOGIN_INTERFACE_V2_H

#include <QMainWindow>
#include "register_interface_v2.h"
namespace Ui {
class login_interface_V2;
}

class login_interface_V2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit login_interface_V2(QWidget *parent = nullptr);
    ~login_interface_V2();
    bool eventFilter(QObject *o, QEvent *e);



private:
    Ui::login_interface_V2 *ui;

};

#endif // LOGIN_INTERFACE_V2_H
