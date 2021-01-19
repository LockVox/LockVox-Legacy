#include "src/includes/audiooutput.h"

AudioOutput::AudioOutput(QObject *parent) : QObject(parent)
{

}




QIODevice * AudioOutput::get_device(){
    return m_output;
}


void AudioOutput::initializeAudioOutput(){
    //Set format
    m_format.setSampleRate(8000);
    m_format.setChannelCount(1);
    m_format.setSampleSize(16);
    m_format.setSampleType(QAudioFormat::SignedInt);
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setCodec("audio/pcm");
    //Set input device
    QAudioDeviceInfo output_dev_info = QAudioDeviceInfo::defaultOutputDevice();
    if(output_dev_info.isNull())
    {
        QList<QAudioDeviceInfo> output_devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
        m_available_device = output_devices;
                   if (!output_devices.isEmpty())
                   {
                       m_output_device_pending = output_devices.first();
                   }
    }
    qDebug() << "Input audio device :" <<output_dev_info.deviceName();

    //Check if device can support the format
    if (!output_dev_info.isFormatSupported(m_format)) {
          qWarning() << "Default format not supported - trying to use nearest";
          m_format = output_dev_info.nearestFormat(m_format);
    }


    m_output_device_pending = output_dev_info;

    //Create audio output
    createAudioOutput();
}

void AudioOutput::createAudioOutput(){
    //Create Audio input
    qDebug() << "Creating audio input" << Qt::endl;
    m_audio_output = new QAudioOutput(m_output_device_pending, m_format, this);
    if(!m_audio_output){
        emit error("Failed to create a new audio input");
    }

    //Connect widget to function


}

void AudioOutput::start(){
    if(!m_output){
        m_output = m_audio_output->start();
    }
}

void AudioOutput::stop() {
    if(m_output){
        m_audio_output->stop();
    }
}

void AudioOutput::writeData(QByteArray data){
    qDebug() << "wrote: " << m_output->write(data.data(), data.size()) << " down from: " << data.size();
}
