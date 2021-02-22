#include "caudio.h"
#include <emiplib/miptime.h>
#include <emiplib/mipaveragetimer.h>
#include <emiplib/mipwavinput.h>
#include <emiplib/mipsampleencoder.h>
#ifndef WIN32
    #include <emiplib/mipossinputoutput.h>
#else
    #include <emiplib/mipwinmmoutput.h>
#endif
#include <emiplib/miprawaudiomessage.h> // Needed for MIPRAWAUDIOMESSAGE_TYPE_S16LE
#include <emiplib/mipcomponentchain.h>
#include <emiplib/mipcomponent.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

void checkError(bool returnValue, const MIPComponentChain& chain)
{
    if (returnValue == true)
        return;
    std::cerr << "An error occured in chain: " << chain.getName() << std::endl;
    std::cerr << "Error description: " << chain.getErrorString() << std::endl;
    //exit(-1);
    return;
}

void checkError(bool returnValue, const MIPComponent& component)
{
    if (returnValue == true)
        return;
    std::cerr << "An error occured in component: " << component.getComponentName() << std::endl;
    std::cerr << "Error description: " << component.getErrorString() << std::endl;
    //exit(-1);
    return;
}

CAudio::CAudio()
{

}

CAudio::~CAudio()
{

}

int CAudio::GetSoundInput()
{
    MIPTime interval(0.050); // We'll use 50 millisecond intervals
    MIPAverageTimer timer(interval);
    MIPWAVInput sndFileInput;
    MIPSampleEncoder sampEnc;
#ifndef WIN32
    MIPOSSInputOutput sndCardOutput;
#else
    MIPWinMMOutput sndCardOutput;
#endif
    m_chain = MIPComponentChain("Sound file player");
    bool returnValue;
    // We'll open the file 'soundfile.wav'
    returnValue = sndFileInput.open("soundfile.wav", interval);
    checkError(returnValue, sndFileInput);
    // Get the parameters of the soundfile. We'll use these to initialize
    // the soundcard output component further on.
    int samplingRate = sndFileInput.getSamplingRate();
    int numChannels = sndFileInput.getNumberOfChannels();
    // Initialize the soundcard output
    returnValue = sndCardOutput.open(samplingRate, numChannels, interval);
    checkError(returnValue, sndCardOutput);
    // Initialize the sample encoder
#ifndef WIN32
    // The OSS component can use several encoding types. We'll ask
    // the component to which format samples should be converted.
    returnValue = sampEnc.init(sndCardOutput.getRawAudioSubtype());
#else
    // The WinMM output component uses signed little endian 16 bit samples.
    returnValue = sampEnc.init(MIPRAWAUDIOMESSAGE_TYPE_S16LE);
#endif
    checkError(returnValue, sampEnc);
    // Next, we'll create the chain
    returnValue = m_chain.setChainStart(&timer);
    checkError(returnValue, m_chain);
    returnValue = m_chain.addConnection(&timer, &sndFileInput);
    checkError(returnValue, m_chain);
    returnValue = m_chain.addConnection(&sndFileInput, &sampEnc);
    checkError(returnValue, m_chain);
    returnValue = m_chain.addConnection(&sampEnc, &sndCardOutput);
    checkError(returnValue, m_chain);
    // Start the chain
    returnValue = m_chain.start();
    checkError(returnValue, m_chain);
    // We'll wait until enter is pressed
    MIPTime::wait(MIPTime(15.0));
    returnValue = m_chain.stop();
    checkError(returnValue, m_chain);
    // We'll let the destructors of the components take care
    // of their de-initialization.
    return 0;
}

void CAudio::OutputSound()
{

}

