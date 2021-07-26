#ifndef CAUDIO_H
#define CAUDIO_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning (disable: 4100)
#include <string>
#include <iostream>
#include "config.h"
#include "mipaudiosession.h"
#include "miptime.h"
#include "mipaveragetimer.h"
#include "mippainputoutput.h"
#include "mipsamplingrateconverter.h"
#include "mipsampleencoder.h"
#include "mipulawencoder.h"
#include "mipulawdecoder.h"
#include "miprtpulawencoder.h"
#include "miprtpulawdecoder.h"
#include "miprtpdecoder.h"
#include "mipaudiomixer.h"
#include "mipwinmminput.h"
#include "mipwinmmoutput.h"
#include "mipaudiofilter.h"
#include "rtpsession.h"
#include "rtpudpv6transmitter.h"
#include "rtpsessionparams.h"
#include "rtpipv6address.h"
#include "miprtpcomponent.h"



#ifdef WIN32
    #include "winsock.h"
    #include "mipwinmmoutput.h"
#endif


#include "mipaudiosession.h"
#include "mipcomponentchain.h"
#include "mipcomponent.h"

class CAudio
{
public:
 CAudio();
 CAudio(unsigned char* ipaddr, int port);
 ~CAudio();


 void checkError(bool returnValue, const MIPComponent &component);
 void checkError(bool returnValue, const MIPComponentChain &chain);
 void checkError(int status);
 void onThreadExit(bool error, const std::string &errorComponent, const std::string &errorDescription);
private:
 MIPComponentChain m_chain;
 MIPRTPComponent m_rtp;
};
#endif
