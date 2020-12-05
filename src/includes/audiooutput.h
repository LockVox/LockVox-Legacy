#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <QObject>
#include <QtMultimedia>

class AudioOutput : public QObject
{
    Q_OBJECT
public:
    explicit AudioOutput(QObject *parent = nullptr);


    QIODevice * get_device();





    void initializeAudioOutput();
    void createAudioOutput();
    void start();
    void stop();



    QIODevice * m_output;
    QAudioOutput * m_audio_output;


signals:
    void error(QString);


public slots:
    void writeData(QByteArray);


private slots:
    //void play();
    //void prePlay();
    //void verifyBuffer();



private:
    bool m_initialized;

    QAudioFormat m_format;

    QAudioDeviceInfo m_output_device_pending;


    QList<QAudioDeviceInfo> m_available_device;

    QByteArray m_buffer;

    float m_volume;

};

#endif // AUDIOOUTPUT_H
