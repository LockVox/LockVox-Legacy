



#ifndef CAUDIO_H
#define CAUDIO_H

#include <mipconfig.h>
#include <mipcomponentchain.h>
#include <mipcomponent.h>
#include <miptime.h>
#include <mipaveragetimer.h>
#include <mipwavinput.h>
#include <mipsamplingrateconverter.h>
#include <mipsampleencoder.h>
#ifndef WIN32
    #include <mipossinputoutput.h>
#else
    #include <mipwinmmoutput.h>
#endif
#include <mipaudiofilter.h>
#include <mipulawencoder.h>
#include <miprtpulawencoder.h>
#include <miprtpcomponent.h>
#include <mippainputoutput.h>
#include <miprtpdecoder.h>
#include <miprtpulawdecoder.h>
#include <mipulawdecoder.h>
#include <mipaudiomixer.h>
#include <miprawaudiomessage.h> // Needed for MIPRAWAUDIOMESSAGE_TYPE_S16LE etc
#include <rtpsession.h>
#include <rtpsessionparams.h>
#include <rtpipv6address.h>
#include <rtpudpv6transmitter.h>
#include <rtperrors.h>
#if !(defined(WIN32) || defined(_WIN32_WCE))
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <unistd.h>
#endif // !(WIM32 || _WIN32_WCE)
#include <stdio.h>
#include <iostream>
#include <string>
#include <QList>
#include <QObject>
#include <cchannel.h>
#include <src/includes/cclient.h>

class CAudio :  public QObject
{
    Q_OBJECT
public:
    CAudio();
    ~CAudio();
    bool AddSession(CChannel m_chan);
    bool DelSession(CChannel m_chan);
public slots:
    //void newChannel();
private:
    QList<MIPComponentChain> m_chain;
    QList<jrtplib::RTPSession*> m_session;
};


#endif // CAUDIO_H
