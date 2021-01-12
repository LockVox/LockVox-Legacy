#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QObject>
#include <QtMultimedia>



class AudioInput : public QObject
{
    Q_OBJECT
public:
    explicit AudioInput(QObject *parent = nullptr);

signals:
    void error(QString);
    void readyRead(QByteArray);



public slots:
    void initializeAudioInput();
    void createAudioInput();
    void start();
    void stop();




private slots:
    //void readMore();


private:

    //Input
    QAudioFormat m_format;
    QAudioInput * m_audio_input;
    QAudioDeviceInfo m_input_device_pending;
    QIODevice * m_input;
};

#endif // AUDIOINPUT_H
