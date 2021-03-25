/**
 * \file audiosession.cpp
 */
#pragma comment(lib, "Ws2_32.lib")

#include <emiplib/mipconfig.h>

#if (defined(MIPCONFIG_SUPPORT_WINMM) || defined(MIPCONFIG_SUPPORT_OSS) || defined(MIPCONFIG_SUPPORT_PORTAUDIO) )
#include "caudio.h"

#include <emiplib/mipavcodecencoder.h>
#include <emiplib/mipaudiosession.h>
#include <emiplib/mippainputoutput.h>
#include <iostream>
#include <jrtplib3/rtpipv4address.h>
#include <jrtplib3/rtpipv6address.h>
#include <jrtplib3/rtpudpv6transmitter.h>
#include <jrtplib3/rtpsessionparams.h>
#include <jrtplib3/rtpsession.h>
#include <jrtplib3/rtptransmitter.h>

#include "Server/config.h"




/***************************************************************/
/***************************************************************/
/************************AUDIO CLASS*****************************/
/***************************************************************/
/***************************************************************/
/***************************************************************/




using namespace jrtplib;

void checkRet(bool ret,const MIPErrorBase &obj)
{
    if (!ret)
    {
        std::cerr << obj.getErrorString() << std::endl;
        exit(-1);
    }
}




bool CAudio::AddSession(CChannel m_chan)
{
    RTPSession * newSess = new RTPSession();
    RTPSessionParams params;
    params.SetOwnTimestampUnit(float(1.0/DEFAULT_SAMP_RATE));
    params.SetReceiveMode(RTPTransmitter::ReceiveMode::AcceptSome);
    params.SetNeedThreadSafety(true);
    RTPUDPv6TransmissionParams transparams;
    transparams.SetPortbase(AUDIO_PORTBASE);
    int status = newSess->Create(params, &transparams, RTPTransmitter::IPv6UDPProto);
    if(status<0)
    {
        std::cerr << RTPGetErrorString(status) << std::endl;
        return false;
    }
    uint8_t* tmpaddr;
    tmpaddr = (uint8_t*)malloc(16*sizeof(uint8_t));
    for(auto c : m_chan.get_clients())
    {
        RTPIPv6Address* tmp = new RTPIPv6Address();
        //Chopper l'ip du client + conversion
        for(int i = 0 ; i <16 ; i++)
            tmpaddr[i] = c->get_socket()->peerAddress().toIPv6Address()[i];
        tmp->SetIP(tmpaddr);
        //tmp->SetPortW((uint16_t)(AUDIO_PORTBASE+m_chan.get_id()));
        //L'ajouter au RTP
        newSess->AddDestination(*tmp);
    }
    if(newSess->IsActive())
    {
        m_session.push_back(newSess);
        return true;
    }
    else
    {
        std::cerr << RTPGetErrorString(status);
        return false;
    }
}

/* , AUDIO_PORTBASE + m_chan.get_id()

/*int main(void)
{
#ifdef WIN32
    WSADATA dat;
    WSAStartup(MAKEWORD(2,2),&dat);
#endif // WIN32
#ifdef MIPCONFIG_SUPPORT_PORTAUDIO
    std::string errStr;

    if (!MIPPAInputOutput::initializePortAudio(errStr))
    {
        std::cerr << "Can't initialize PortAudio: " << errStr << std::endl;
        return -1;
    }
#endif // MIPCONFIG_SUPPORT_PORTAUDIO

    MIPAudioSessionParams Aparams;
    MyAudioSession audioSess;
    bool ret;

    int audioPort = 6000;

    Aparams.setPortbase(audioPort);
//	Aparams.setCompressionType(MIPAudioSessionParams::Speex);
//	Aparams.setAcceptOwnPackets(true);
//	Aparams.setInputDevice("/dev/dsp1");
//	Aparams.setOutputDevice("/dev/dsp");
//	Aparams.setSpeexEncoding(MIPAudioSessionParams::UltraWideBand);
//	Aparams.setSpeexOutgoingPayloadType(97);
    Aparams.setSpeexIncomingPayloadType(97);
    Aparams.setOpusIncomingPayloadType(98);

    std::cout << "Starting audio session at portbase " << audioPort << std::endl;

    ret = audioSess.init(&Aparams);
    checkRet(ret, audioSess);

    uint8_t ipLocal[4] = { 127, 0, 0, 1 };
    ret = audioSess.addDestination(RTPIPv4Address(ipLocal, audioPort));

    std::string dummy;
    std::cout << "Type something to exit" << std::endl;
    std::cin >> dummy;

    std::cout << "Exiting..." << std::endl;

    audioSess.destroy();

#ifdef MIPCONFIG_SUPPORT_PORTAUDIO
    MIPPAInputOutput::terminatePortAudio();
#endif // MIPCONFIG_SUPPORT_PORTAUDIO
#ifdef WIN32
    WSACleanup();
#endif // WIN32

    return 0;
}

#else

#include <iostream>

int main(void)
{
    std::cerr << "Not all necessary components are available to run this example." << std::endl;
    return 0;
}*/

#endif
