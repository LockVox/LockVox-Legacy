
#include <emiplib/mipcomponentchain.h>
#include <emiplib/miptime.h>
#include <emiplib/mipaveragetimer.h>
#include <emiplib/mipwavinput.h>
#include <emiplib/mipsampleencoder.h>
#ifndef WIN32
    #include <mipossinputoutput.h>
#else
    #include <emiplib/mipwinmmoutput.h>
#endif
#include <emiplib/mipulawencoder.h>
#include <emiplib/miprtpulawencoder.h>
#include <emiplib/miprtpcomponent.h>
#include <emiplib/miprtpdecoder.h>
#include <emiplib/miprtpulawdecoder.h>
#include <emiplib/mipulawdecoder.h>
#include <emiplib/mipaudiomixer.h>
#include <emiplib/miprawaudiomessage.h> // Needed for MIPRAWAUDIOMESSAGE_TYPE_S16LE
#include <emiplib/mipaudiosession.h>

#include <jrtplib3/rtpsession.h>
#include <jrtplib3/rtpsessionparams.h>
#include <jrtplib3/rtpipv4address.h>
#include <jrtplib3/rtpudpv4transmitter.h>
#include <jrtplib3/rtperrors.h>

#include <stdio.h>
#include <iostream>


#include <QApplication>

#include "channelwidget.h"
#include "mainwindow.h"

void checkError(bool returnValue, const MIPComponent &component)
{
    if (returnValue == true)
        return;
    std::cerr << "An error occured in component: " << component.getComponentName();
    std::cerr << "Error description: " << component.getErrorString() << std::endl;
    exit(-1);
}
void checkError(bool returnValue, const MIPComponentChain &chain)
{
    if (returnValue == true)
        return;
    std::cerr << "An error occured in chain: " << chain.getName() << std::endl;
    std::cerr << "Error description: " << chain.getErrorString() << std::endl;
    exit(-1);
}



int main(int argc, char *argv[])
{
   MIPAudioSession audio_session;

   MIPTime interval(0.020);
   MIPAverageTimer timer = MIPAverageTimer(interval);

   MIPAudioMixer mixer;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();


}
