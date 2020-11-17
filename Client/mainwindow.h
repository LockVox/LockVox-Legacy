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

private:



    Ui::MainWindow *ui;


    //Network

    QTcpSocket * m_socket;





    //Audio
    AudioInput * m_audio_in;
    AudioOutput * m_audio_out;

};
#endif // MAINWINDOW_H
