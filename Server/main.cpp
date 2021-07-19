#pragma comment(lib,"advapi32.lib")
#pragma comment(lib,"emiplib.lib")
#pragma comment(lib,"jrtplib.lib")
#pragma comment(lib,"jthread.lib")
#pragma comment(lib,"portaudio_x64.lib")

#include <QApplication>
#include <QThread>
#include <QString>

#include "Server/config.h"
#include "Server/includes/cserver.h"

#include "mipconfig.h"
#include "mipcomponent.h"
#include "mipaveragetimer.h"
#include "mipaudiosession.h"
#include "miptime.h"
#include "mipcomponentchain.h"
#include "mipcomponent.h"
#include <iostream>
#include <string>
#include "mipaveragetimer.h"
#include "mipmessagedumper.h"
#include <windows.h>

#include "winsock.h"
#pragma comment(lib,"ws2_32.lib")



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    #ifdef WIN32
        WSADATA dat;
        WSAStartup(MAKEWORD(2,2),&dat);
    #endif // WIN32
    MIPAudioSession *s = new MIPAudioSession();

    CServer * server = new CServer();
    server->start();


    /*WSADATA Data;
    WSAStartup(MAKEWORD(2,2), &Data);
    CChannel tmpchan("", 0);
    CAudio tmp;
    tmp.AddSession(tmpchan);
    getc(stdin);
    WSACleanup();*/
    return a.exec();
}
