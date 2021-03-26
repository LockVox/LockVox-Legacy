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
#include <emiplib/mipaudiosession.h>
#include "Server/config.h"
#include <QDebug>

CAudio::CAudio()
{

}




CAudio::~CAudio()
{

}


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
    transparams.SetPortbase(AUDIO_PORTBASE+m_chan.get_id());
    in6_addr add;
    qDebug() << inet_pton(AF_INET6, "::1", &add);
    transparams.SetBindIP(add);
    int status = newSess->Create(params, &transparams, RTPTransmitter::IPv6UDPProto);
    if(status<0)
    {
        std::cerr << "1" << RTPGetErrorString(status) << std::endl;
        return false;
    }
    uint8_t* tmpaddr;
    tmpaddr = (uint8_t*)malloc(16*sizeof(uint8_t));

    //Ajout des clients
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
        qDebug() << "Session " << m_chan.get_id() << " succesfully created!" << Qt::endl;
        qDebug() << "It is running on port : " << AUDIO_PORTBASE + m_chan.get_id() << Qt::endl;
        return true;
        //Should be running
    }
    else
    {
        std::cerr << "2 " << RTPGetErrorString(status);
        return false;
    }
}



#endif //MIPCONFIG_SUPPORT_WINMM||MIPCONFIG_SUPPORT_OSS||MIPCONFIG_SUPPORT_PORTAUDIO
