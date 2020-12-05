#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QObject>
#include <QtMultimedia>

#define AUDIO_BUFFER_SIZE 1024

class AudioInput : public QObject
{
    Q_OBJECT
public:

    QIODevice * get_device();
    QAudioFormat get_format();
    QAudioInput *get_audio_input();
    QAudioDeviceInfo get_audio_device_info();

    void set_device(QIODevice * dev);




    explicit AudioInput(QObject *parent = nullptr);
    void initializeAudioInput();
    void createAudioInput();
    void start();

    void stop();



signals:
    void error(QString);
    void dataReady(QByteArray data);


public slots:
    void readyRead();




private slots:
    void sendBuffer();


private:

    //Input
    QAudioFormat m_format;
    QAudioInput * m_audio_input;
    QAudioDeviceInfo m_input_device_pending;
    QIODevice * m_input;
};

#endif // AUDIOINPUT_H
