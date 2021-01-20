#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Client/cserver.h"
#include "src/includes/audioinput.h"
#include "src/includes/audiooutput.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Update();


public slots:

    //Audio


private slots:
    void on_parameter_button_clicked();
    void on_newChannel();
    void on_newClient();

    void on_changeState(int newState);

private:

    Ui::MainWindow *ui; 

    int m_state;
    //Network

    CServer * m_server;











};
#endif // MAINWINDOW_H
