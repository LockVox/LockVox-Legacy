#include "Client/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //Initialize widgets







    //Connect widget to action




    //Initialize server
    m_server = new CServer(2); //Create CServer as Client mode

    m_server->set_socket(new QTcpSocket());

    m_server->get_socket()->abort();
    m_server->get_socket()->connectToHost("127.0.0.1", 50885);






    m_audio_in = new AudioInput();
    m_audio_out = new AudioOutput();

    m_audio_in->initializeAudioInput();
    m_audio_out->initializeAudioOutput();

    m_audio_in->start();
    m_audio_out->start();

    connect(m_audio_in->get_device(), SIGNAL(readyRead()),this, SLOT(sendToServer()));

}


void MainWindow::sendToServer(){
    qDebug() << "Send to server - ";
    m_server->get_socket()->write(m_audio_in->get_device()->readAll());
}

MainWindow::~MainWindow()
{
    delete ui;
}

