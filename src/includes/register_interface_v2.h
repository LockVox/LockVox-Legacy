#ifndef REGISTER_INTERFACE_V2_H
#define REGISTER_INTERFACE_V2_H

#include <QMainWindow>
#include <src/includes/login_interface_v2.h>
#include "clickablelabel.h"

namespace Ui {
class register_interface_V2;
}

class register_interface_V2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit register_interface_V2(QWidget *parent = nullptr);
    ~register_interface_V2();
    bool eventFilter(QObject *o, QEvent *e);


private slots:
    void on_pushButton_clicked();

private:
    Ui::register_interface_V2 *ui;
};

#endif // REGISTER_INTERFACE_V2_H
