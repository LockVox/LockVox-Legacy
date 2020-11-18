#include "QXmppLogger.h"
#include "QXmppMessage.h"
#include "QXmppClient.h"
#include <QApplication>
#include <QCoreApplication>
#include <QThread>
#include <iostream>

QXmppClient client;

class Worker : public QObject
{
    Q_OBJECT

public slots:
    int startClient()
    {
        client.logger()->setLoggingType(QXmppLogger::StdoutLogging);
        client.connectToServer("lockvox@192.168.1.40", "azerty");
        if(client.isConnected())
        {
            client.sendPacket(QXmppMessage("Lockvox1", "Lockvox2","Does it work ?"));
        }
        return 1;
    }

    int isConnected()
    {
        return client.isConnected();
    }
    int sendMessage(QString from, QString to, QString message)
    {
        return client.sendPacket(QXmppMessage(from,to,message));
    }

signals:
    void resultReady(const QString &result);
};

class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    Controller() {
        Worker *worker = new Worker;
        worker->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &Controller::operate, worker, &Worker::startClient);
        connect(worker, &Worker::resultReady, this, &Controller::handleResults);
        workerThread.start();
    }
    ~Controller() {
        workerThread.quit();
        workerThread.wait();
    }
public slots:
    void handleResults(const QString &);
signals:
    void operate(const QString &);
};
