#include "caudio.h"
#include <QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QQuickView>
#include <QQuickItem>


#include "Client/includes/uiworker.h"
#include "Client/includes/config.h"



int main(int argc, char *argv[])
{


    /*uint8_t* addr = (uint8_t*)malloc(16*sizeof(uint8_t));
        inet_pton(AF_INET6, IP_ADDRESS, addr);
        CAudio var(addr,0);
        printf("Bien init");
        getc(stdin);
        var.StopChain();*/


        #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        #endif

        QGuiApplication app(argc,argv);
        //app.setWindowIcon(QIcon("Client/qml/lock_vox_logo_miniature1.png"));
        UIWorker * ui  = new UIWorker(&app);

        app.exec();
}


