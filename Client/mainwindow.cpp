#include "Client/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("LockVox");

    //Initialize server
    m_server = new CServer(); //Create CServer as Client mode


    //Initialize widgets

        //channel frame



        //user frame //


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
