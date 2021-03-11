#include "Client/mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    //Loading UI File & Variable
    ui_login = new LoginWindow(this);
    ui_register = new RegisterWindow(this);


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
    connect(ui_register, SIGNAL(on_askServer(QString,QString,QString,QString)), m_server, SLOT(Register(QString,QString,QString,QString)));
    connect(ui_login, SIGNAL(displayRegisterWindow(int)), this, SLOT(on_changeState(int)));
    connect(m_server, SIGNAL(changeState(int)), this, SLOT(on_changeState(int)));
    connect(m_server, SIGNAL(on_Authentification(int)), this, SLOT(on_changeState(int)));
    //connect(m_server, SIGNAL(updateMainWindow()), this, SLOT(Update(int)));
    connect(this, SIGNAL(RequestServer(int,int,CClient * client, CChannel * chan)), m_server, SLOT(RequestServer(int,int,CClient * client, CChannel * chan)));
    connect(m_timer, &QTimer::timeout, this, &MainWindow::Update);
    connect(this, SIGNAL(sendMessage(QString)), m_server, SLOT(sendMessage(QString)));


    //Initialize widgets

        //channel frame


        //user frame //

    m_timer->start(100);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Update()
{
    if(m_state == 1){
        for(int i = 0; i < list_channel_widgets.size(); i++){
            list_channel_widgets[i]->Update();
        }
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
    changeWindow();



    if(m_state == 1){
        ui->channel_layout->setMargin(0);
        for(int i = 0; i < m_server->get_channelList().size(); i++)
        {
           //ui->channel_layout->inse(layout_per_channel[i]);
           channelWidget * button = new channelWidget(this,m_server->get_channelList()[i]);
           list_channel_widgets.push_back(button);

           //button->setFlat( true );
           //button->setCheckable( true );
           button->setMinimumSize(250, 61);
           ui->channel_layout->addWidget(button);
           connect(button,SIGNAL(RequestServer(int,int,CClient*,CChannel*)), m_server, SLOT(RequestServer(int,int,CClient*, CChannel*)));
        }

        for(int i = 0; i < m_server->get_clientList().size(); i++){
            QPushButton * button = new QPushButton(m_server->get_clientList()[i]->get_pseudo(),this);
            ui->client_layout->addWidget(button);
        }
    }
}


void MainWindow::changeWindow(){
    if(m_state == -1){
        showLoginWindow();
        return;
    }

    if(m_state == 0){
        showRegisterWindow();
        return;
    }

    if(m_state == 1){
        showMainWindow();
        return;
    }
}

void MainWindow::showLoginWindow(){
    if(this->isVisible())
        this->hide();
    if(ui_register->isVisible())
        ui_register->hide();

    ui_login->show();
}

void MainWindow::showRegisterWindow(){
    if(this->isVisible())
        this->hide();
    if(ui_login->isVisible())
        ui_login->hide();

    ui_register->show();
}

void MainWindow::showMainWindow(){
    if(ui_register->isVisible())
        ui_register->hide();
    if(ui_login->isVisible())
        ui_login->hide();

    this->show();
}



void MainWindow::closeEvent(QCloseEvent *event)
{
        emit(RequestServer(0,1, m_server->get_self(), NULL));
        event->accept();
}

void MainWindow::on_message_line_returnPressed()
{
    QString message = ui->message_line->text();
    emit(sendMessage(message));
    ui->message_line->clear();
}
