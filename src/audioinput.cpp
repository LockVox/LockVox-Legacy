#include "src/includes/audioinput.h"

AudioInput::AudioInput(QObject *parent) : QObject(parent)
{

}



QIODevice * AudioInput::get_device(){
    return m_input;
}
QAudioFormat AudioInput::get_format(){
    return m_format;
}
QAudioInput *AudioInput::get_audio_input(){
    return m_audio_input;
}
QAudioDeviceInfo AudioInput::get_audio_device_info(){
    return m_input_device_pending;
}


void AudioInput::initializeAudioInput(){

    //Set format
    m_format.setSampleRate(8000);
    m_format.setChannelCount(1);
    m_format.setSampleSize(16);
    m_format.setSampleType(QAudioFormat::SignedInt);
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setCodec("audio/pcm");

    //Set input device
    QAudioDeviceInfo input_dev_info = QAudioDeviceInfo::defaultInputDevice();
    if(input_dev_info.isNull())
    {
        QList<QAudioDeviceInfo> input_devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);

                   if (!input_devices.isEmpty())
                   {
                       m_input_device_pending = input_devices.first();
                   }
    }
    qDebug() << "Input audio device :" <<input_dev_info.deviceName();

    //Check if device can support the format
    if (!input_dev_info.isFormatSupported(m_format)) {
          qWarning() << "Default format not supported - trying to use nearest";
          m_format = input_dev_info.nearestFormat(m_format);
    }

    m_input_device_pending = input_dev_info;

    createAudioInput();
}



void AudioInput::createAudioInput(){

    //Create Audio input
    qDebug() << "Creating audio input" << Qt::endl;
    m_audio_input = new QAudioInput(m_input_device_pending, m_format, this);
    if(!m_audio_input){
        emit error("Failed to create a new audio input");
    }
}

//Start Audio input
void AudioInput::start(){
    qDebug() << "Starting Audio input";

    m_input = m_audio_input->start();

    //Connect
    connect(m_input, SIGNAL(readyRead()), this, SLOT(sendBuffer()));
}


//Stop Audio input
void AudioInput::stop(){
  if(m_input){
      m_audio_input->stop();
  }
}





void AudioInput::sendBuffer(){
    qDebug() << m_input->readAll();
}

