#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <QObject>
#include <QtMultimedia>

class AudioOutput : public QObject
{
    Q_OBJECT
public:
    explicit AudioOutput(QObject *parent = nullptr);
    void initializeAudioOutput();
    void createAudioOutput();
    void start();
    void stop();

signals:
    void error(QString);


public slots:

    void write();


private slots:
    //void play();
    //void prePlay();
    //void verifyBuffer();



private:
    bool m_initialized;

    QAudioFormat m_format;
    QAudioOutput * m_audio_output;
    QAudioDeviceInfo m_output_device_pending;
    QIODevice * m_output;

    QList<QAudioDeviceInfo> m_available_device;

    QByteArray m_buffer;

    float m_volume;

};

#endif // AUDIOOUTPUT_H
