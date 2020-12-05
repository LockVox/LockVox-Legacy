#include "Client/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //Initialize widgets


    //Initialize server
    m_server = new CServer(2); //Create CServer as Client mode


    //Initialize Audio
    m_server->m_audio_in = new AudioInput();
    m_server->m_audio_out = new AudioOutput();

    m_server->m_audio_in->initializeAudioInput();
    m_server->m_audio_out->initializeAudioOutput();

    //Start streaming audio on socket

    m_server->m_audio_in->start();
    //connect(this->m_server->m_audio_in->get_device(), SIGNAL(readyRead()), this->m_server, SLOT(sendToServer()));
    connect(this->m_server->m_audio_in, SIGNAL(dataReady(QByteArray)),this->m_server ,SLOT(sendToServer(QByteArray)));




    m_server->m_audio_out->m_output = m_server->m_audio_out->m_audio_output->start();

    //connect(m_audio_in->get_device(), SIGNAL(readyRead()),this, SLOT(sendToServer()));

}



MainWindow::~MainWindow()
{
    delete ui;
}

