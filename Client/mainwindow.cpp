#include "Client/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("LockVox");

    //Initialize state
    m_state = 0;

    //Initialize server
    m_server = new CServer(); //Create CServer as Client mode


    //Connect
    connect(m_server, SIGNAL(changeState(int)), this, SLOT(on_changeState(int)));




    //Initialize widgets

        //channel frame



        //user frame //


    //connect()
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Update()
{

}



// if we click on the parameter button of the main window //
void MainWindow::on_parameter_button_clicked()
{
    // open user interface
}

void MainWindow::on_newChannel(){

}

void MainWindow::on_newClient(){

}

void MainWindow::on_username_clicked()
{
    //ChangeUsernameWindow *window = new ChangeUsernameWindow();
    //window->show();

}

void MainWindow::on_status_clicked()
{
    //userStatusWindow *status = new userStatusWindow();
    //status->show();
}

void MainWindow::on_changeState(int newState){
    m_state = newState;

    if(m_state == 1){
        qDebug() << "Channel & Server has been load\n";

        for(int i = 0; i < m_server->get_channelList().size(); i++){
            QPushButton * button = new QPushButton(m_server->get_channelList()[i]->get_name(),this);
            ui->channel_layout->addWidget(button);
        }

        for(int i = 0; i < m_server->get_clientList().size(); i++){
            QPushButton * button = new QPushButton(m_server->get_clientList()[i]->get_pseudo(),this);
            ui->client_layout->addWidget(button);
        }
    }
}

