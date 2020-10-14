#include "src/includes/login_interface_v2.h"
#include "ui_login_interface_v2.h"
#include "src/includes/register_interface_v2.h"

// include others ui headers //


login_interface_V2::login_interface_V2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::login_interface_V2)
{
    ui->setupUi(this);
    this->setFixedSize(640,550);

    // clickable label
    ui->label->installEventFilter(this);




}

login_interface_V2::~login_interface_V2()
{
    delete ui;
}

bool login_interface_V2::eventFilter(QObject *o, QEvent *e)
{
    if(o == ui->label && e->type() == QEvent::MouseButtonPress)
    {
        // display register interface //
        hide();
        register_interface_V2 * register_interface;
        register_interface = new register_interface_V2(this);
        register_interface->setWindowTitle("LockVox");
        register_interface->setWindowIcon(QIcon(":/pictures/icon.png"));
        register_interface->show();


    }

    if(o == ui->label_2 && e->type() == QEvent::MouseButtonPress)
    {
    //do something


    }


    return QObject::eventFilter(o,e);
}




