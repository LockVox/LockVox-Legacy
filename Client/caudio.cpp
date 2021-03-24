/*#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "config.h"
#include "caudio.h"
// We'll be using an RTPSession instance from the JRTPLIB library. The following
// function checks the JRTPLIB error code.
void CAudio::checkError(bool returnValue, const MIPComponent &component)
{
    if (returnValue)
        return;
    std::cerr << "An error occured in component: " << component.getComponentName() << std::endl;
    std::cerr << "Error description: " << component.getErrorString() << std::endl;
    exit(-1);
}
void CAudio::checkError(bool returnValue, const MIPComponentChain &chain)
{
    if (returnValue == true)
        return;
    std::cerr << "An error occured in chain: " << chain.getName() << std::endl;
    std::cerr << "Error description: " << chain.getErrorString() << std::endl;
    exit(-1);
}
void CAudio::checkError(int status)
{
    if (status >= 0)
        return;
    std::cerr << "An error occured in the RTP component: " << std::endl;
    std::cerr << "Error description: " << jrtplib::RTPGetErrorString(status) << std::endl;
    exit(-1);
}

void CAudio::onThreadExit(bool error, const std::string &errorComponent, const std::string &errorDescription)
{
    if (!error)
        return;
    std::cerr << "An error occured in the background thread." << std::endl;
    std::cerr << "    Component: " << errorComponent << std::endl;
    std::cerr << "    Error description: " << errorDescription << std::endl;
}

int CAudio::SendSoundToRTP()
{

#ifdef WIN32
    WSADATA dat;
    WSAStartup(MAKEWORD(2,2),&dat);
#endif // WIN32
    std::string err;
    MIPTime interval(0.05); // We'll use 20 millisecond intervals.
    MIPAverageTimer timer(interval);
    MIPPAInputOutput sndFileInput;
    sndFileInput.initializePortAudio(err);
    MIPSamplingRateConverter sampConv, sampConv2;
    MIPSampleEncoder floatEnc, sampEnc, sampEnc2, sampEnc3;
    MIPULawEncoder uLawEnc;
    MIPRTPULawEncoder rtpEnc;
    MIPRTPComponent rtpComp;
    MIPRTPDecoder rtpDec;
    MIPRTPULawDecoder rtpULawDec;
    MIPULawDecoder uLawDec;
    MIPAudioMixer mixer;
#ifndef WIN32
    MIPOSSInputOutput sndCardOutput;
#else
    MIPWinMMOutput sndCardOutput;
#endif
    floatEnc.init(MIPRAWAUDIOMESSAGE_TYPE_FLOAT);
    MIPComponentChain chain("RTP voice sender");
    jrtplib::RTPSession rtpSession;
    bool returnValue;
    // We'll open the file 'soundfile.wav'.
    returnValue = sndFileInput.open(8000,1,interval, MIPPAInputOutput::ReadOnly);
    checkError(returnValue, sndFileInput);
    // We'll convert to a sampling rate of 8000Hz and mono sound.
    int samplingRate = 8000;
    int numChannels = 1;
    returnValue = sampConv.init(samplingRate, numChannels);
    checkError(returnValue, sampConv);
    // Initialize the sample encoder: the RTP U-law audio encoder
    // expects native endian signed 16 bit samples.
    returnValue = sampEnc.init(MIPRAWAUDIOMESSAGE_TYPE_S16);
    checkError(returnValue, sampEnc);
    // Convert samples to U-law encoding
    returnValue = uLawEnc.init();
    checkError(returnValue, uLawEnc);
    // Initialize the RTP audio encoder: this component will create
    // RTP messages which can be sent to the RTP component.
    returnValue = rtpEnc.init();
    checkError(returnValue, rtpEnc);
    // We'll initialize the RTPSession object which is needed by the
    // RTP component.
    jrtplib::RTPUDPv4TransmissionParams transmissionParams;
    jrtplib::RTPSessionParams sessionParams;
    int status;
    transmissionParams.SetPortbase(AUDIO_PORTBASE);
    sessionParams.SetOwnTimestampUnit(1.0/((double)samplingRate));
    sessionParams.SetMaximumPacketSize(64000);
    sessionParams.SetAcceptOwnPackets(false);       // Le serveur renverra tout mais le client ignorera ses paquets
    status = rtpSession.Create(sessionParams,&transmissionParams);
    checkError(status);
    // Instruct the RTP session to send data to ourselves.
    status = rtpSession.AddDestination(jrtplib::RTPIPv4Address(ntohl(inet_addr(IP_ADDRESS)), AUDIO_PORTBASE));
    checkError(status);
    // Tell the RTP component to use this RTPSession object.
    returnValue = rtpComp.init(&rtpSession);
    checkError(returnValue, rtpComp);
    // Initialize the RTP audio decoder.
    returnValue = rtpDec.init(true, 0, &rtpSession);
    checkError(returnValue, rtpDec);
    // Register the U-law decoder for payload type 0
    returnValue = rtpDec.setPacketDecoder(0,&rtpULawDec);
    checkError(returnValue, rtpDec);
    // Convert U-law encoded samples to linear encoded samples
    returnValue = uLawDec.init();
    checkError(returnValue, uLawDec);
    // Transform the received audio data to floating point format.
    returnValue = sampEnc2.init(MIPRAWAUDIOMESSAGE_TYPE_FLOAT);
    checkError(returnValue, sampEnc2);
    // We'll make sure that received audio frames are converted to the right
    // sampling rate.
    returnValue = sampConv2.init(samplingRate, numChannels);
    checkError(returnValue, sampConv2);
    // Initialize the mixer.
    returnValue = mixer.init(samplingRate, numChannels, interval);
    checkError(returnValue, mixer);
    // Initialize the soundcard output.
    returnValue = sndCardOutput.open(samplingRate, numChannels, interval);
    checkError(returnValue, sndCardOutput);
    //Init des filtres
    MIPAudioFilter filtre;
    filtre.init(samplingRate,1,interval);
    filtre.setLowFilter(300);
    filtre.setHighFilter(3000);

#ifndef WIN32
    // The OSS component can use several encodings. We'll check
    // what encoding type is being used and inform the sample encoder
    // of this.
    uint32_t audioSubtype = sndCardOutput.getRawAudioSubtype();
    returnValue = sampEnc3.init(audioSubtype);
#else
    // The WinMM soundcard output component uses 16 bit signed little
    // endian data.
    returnValue = sampEnc3.init(MIPRAWAUDIOMESSAGE_TYPE_S16LE);
#endif
    checkError(returnValue, sampEnc3);
    // Next, we'll create the chain
    returnValue = chain.setChainStart(&sndFileInput);
    checkError(returnValue, chain);
    returnValue = chain.addConnection(&sndFileInput, &floatEnc);
    checkError(returnValue, chain);
    returnValue = chain.addConnection(&floatEnc, &filtre);
    checkError(returnValue, chain);
    returnValue = chain.addConnection(&filtre, &sampConv);
    checkError(returnValue, chain);
    returnValue = chain.addConnection(&sampConv, &sampEnc);
    checkError(returnValue, chain);
    returnValue = chain.addConnection(&sampEnc, &uLawEnc);
    checkError(returnValue, chain);
    returnValue = chain.addConnection(&uLawEnc, &rtpEnc);
    checkError(returnValue, chain);
    returnValue = chain.addConnection(&rtpEnc, &rtpComp);
    checkError(returnValue, chain);
    returnValue = chain.addConnection(&rtpComp, &rtpDec);
    checkError(returnValue, chain);
    // This is where the feedback chain is specified: we want
    // feedback from the mixer to reach the RTP audio decoder,
    // so we'll specify that over the links in between, feedback
    // should be transferred.
    returnValue = chain.addConnection(&rtpDec, &uLawDec, true);
    checkError(returnValue, chain);
    returnValue = chain.addConnection(&uLawDec, &sampEnc2, true);
    checkError(returnValue, chain);
    returnValue = chain.addConnection(&sampEnc2, &sampConv2, true);
    checkError(returnValue, chain);
    returnValue = chain.addConnection(&sampConv2, &mixer, true);
    checkError(returnValue, chain);
    returnValue = chain.addConnection(&mixer, &sampEnc3);
    checkError(returnValue, chain);
    returnValue = chain.addConnection(&sampEnc3, &sndCardOutput);
    checkError(returnValue, chain);
    // Start the chain
    returnValue = chain.start();
    checkError(returnValue, chain);
    // We'll wait until enter is pressed
    getc(stdin);
    returnValue = chain.stop();
    checkError(returnValue, chain);
    rtpSession.Destroy();
    // We'll let the destructors of the other components take care
    // of their de-initialization.
#ifdef WIN32
    WSACleanup();
#endif
    return 0;
}

*/









