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
    m_server = new CServer();


    m_socket = new QTcpSocket();

    m_socket->abort();
    m_socket->connectToHost("127.0.0.1", 50885);







    m_audio_in = new AudioInput();
    m_audio_out = new AudioOutput();

    m_audio_in->initializeAudioInput();
    m_audio_out->initializeAudioOutput();






}

MainWindow::~MainWindow()
{
    delete ui;
}

