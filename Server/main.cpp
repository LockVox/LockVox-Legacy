

#include "emiplib/mipaudiosession.h"



#include <QApplication>
#include <QThread>
#include <QString>

#include "mainwindow.h"





int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();


    MIPAudioSession * m_audio_session;
    WSAData m_audio_input;


    return a.exec();
}
