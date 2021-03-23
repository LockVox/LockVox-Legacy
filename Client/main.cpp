#include <QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QQuickView>
#include <QQuickItem>


#include "Client/includes/uiworker.h"
#include "Client/includes/config.h"




int main(int argc, char *argv[])
{


#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    //Check if engine has been load correctly
    /*if(engine.rootObjects().isEmpty()){
        qDebug("Empty rootObjects");
        return -1;
    }*/

    QGuiApplication app(argc,argv);
    UIWorker * ui  = new UIWorker(&app);




    return app.exec();
}


