#ifndef CAUDIO_H
#define CAUDIO_H
#pragma comment(lib, "Ws2_32.lib")

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
#include <rtpipv4address.h>
#include <rtpudpv4transmitter.h>
#include <rtperrors.h>
#if !(defined(WIN32) || defined(_WIN32_WCE))
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <unistd.h>
#endif // !(WIM32 || _WIN32_WCE)
#include <stdio.h>
#include <iostream>
#include <string>

#include "src/includes/abstractaudio.h"

class CAudio : public AbstractAudio
{
public:
    CAudio();
    ~CAudio();

    int SendSoundToRTP();
    void OutputSound();
private:
    void onThreadExit(bool error, const std::string &errorComponent, const std::string &errorDescription);
    void checkError(bool returnValue, const MIPComponent &component);
    void checkError(bool returnValue, const MIPComponentChain &chain);
    void checkError(int status);

};

#endif // CAUDIO_H
