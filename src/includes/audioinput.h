#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QObject>
#include <QtMultimedia>



class AudioInput : public QObject
{
    Q_OBJECT
public:

    QIODevice * get_device();
    QAudioFormat get_format();
    QAudioInput *get_audio_input();
    QAudioDeviceInfo get_audio_device_info();





    explicit AudioInput(QObject *parent = nullptr);
    void initializeAudioInput();
    void createAudioInput();
    void start();

    void stop();



signals:
    void error(QString);
    void readyRead(QByteArray);



public slots:





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
