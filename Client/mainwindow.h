#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "src/includes/cserver.h"
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



public slots:

    //Audio




private slots:
    void on_parameter_button_clicked();

private:



    Ui::MainWindow *ui;

    void Update();


    //Network

    CServer * m_server;











};
#endif // MAINWINDOW_H
