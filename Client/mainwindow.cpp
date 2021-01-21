#include "Client/mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    //Loading UI File & Variable
    ui_login = new LoginWindow(this);
    ui->setupUi(this);
    this->setWindowTitle("LockVox");



    m_timer = new QTimer();

    window = 0;

    //Show Login Window
    showLoginWindow();


    //Initialize state
    m_state = 0;
    //Initialize server
    m_server = new CServer(); //Create CServer as Client mode

    //Connect
    connect(ui_login, SIGNAL(on_askServer(QString, QString)), m_server, SLOT(Login(QString,QString)));
    connect(m_server, SIGNAL(changeState(int)), this, SLOT(on_changeState(int)));
    connect(m_server, SIGNAL(on_Authentification(int)), this, SLOT(changeWindow(int)));
    connect(m_timer, &QTimer::timeout, this, &MainWindow::Update);

    //Initialize widgets

        //channel frame



        //user frame //


    //connect()

    m_timer->start(100);
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Update()
{
    if(window == 0 && this->isVisible()){
        changeWindow();
    }
    if(window == 1 && ui_login->isVisible()){
        changeWindow();
    }
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
    ChangeUsernameWindow *window = new ChangeUsernameWindow();
    window->show();

}

void MainWindow::on_status_clicked()
{
    userStatusWindow *status = new userStatusWindow();
    status->show();
}

void MainWindow::on_changeState(int newState){
    m_state = newState;
    ui->channel_layout->setMargin(0);


    if(m_state == 1){
        qDebug() << "Channel & Server has been load\n";


        for(int i = 0; i < m_server->get_channelList().size(); i++){
           channelWidget * button = new channelWidget(this,m_server->get_channelList()[i]);
           connect(button, SIGNAL(clicked()), button,SLOT(on_button_clicked()));
           connect(button,SIGNAL(RequestServer(int,int,CClient*,CChannel*)), m_server, SLOT(RequestServer(int,int,CClient*, CChannel*)));
           ui->channel_layout->addWidget(button);

        }

        for(int i = 0; i < m_server->get_clientList().size(); i++){
            QPushButton * button = new QPushButton(m_server->get_clientList()[i]->get_pseudo(),this);
            ui->client_layout->addWidget(button);
        }
    }
}


void MainWindow::changeWindow(int newValue){
    window=newValue;
}

void MainWindow::showLoginWindow(){
    if(this->isVisible()){
        this->hide();
        ui_login->show();
    }
}

void MainWindow::changeWindow()
{
    if(ui_login->isVisible())
       {
           ui_login->hide();
           this->show();
       }
       else
       {
           this->hide();
           ui_login->show();
       }
}

