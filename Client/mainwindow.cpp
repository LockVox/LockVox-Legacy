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


    //Initialize Audio Input
    m_server->m_audio_in = new AudioInput();
    m_server->m_audio_out = new AudioOutput();
    //Initialize Audio Output
    m_server->m_audio_in->initializeAudioInput();
    m_server->m_audio_out->initializeAudioOutput();

    //Start streaming audio
    m_server->m_audio_in->start();
    m_server->m_audio_out->m_output = m_server->m_audio_out->m_audio_output->start();


    connect(this->m_server->m_audio_in, SIGNAL(dataReady(QByteArray)),this->m_server ,SLOT(sendToServer(QByteArray)));

}



MainWindow::~MainWindow()
{
    delete ui;
}

