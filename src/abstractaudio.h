/*#ifndef ABSTRACTAUDIO_H
#define ABSTRACTAUDIO_H

#include <emiplib/mipcomponentchain.h>
#include <emiplib/mipaudiosession.h>
#include <emiplib/miptime.h>
#include <emiplib/mipaveragetimer.h>
#include <emiplib/mipwavinput.h>
#include <emiplib/mipsampleencoder.h>
#ifndef WIN32
    #include <emiplib/mipossinputoutput.h>
#else
    #include <emiplib/mipwinmminput.h>
    #include <emiplib/mipwinmmoutput.h>
#endif
#include <emiplib/miprawaudiomessage.h> // Needed for MIPRAWAUDIOMESSAGE_TYPE_S16LE
#include <emiplib/mipcomponentchain.h>
#include <emiplib/mipcomponent.h>
#include <emiplib/mipaudiosession.h>
#include "emiplib/mipulawdecoder.h"
#include "emiplib/mipulawencoder.h"

#include "emiplib/miprtpulawencoder.h"
#include "emiplib/miprtpulawdecoder.h"

#include "jrtplib3/rtpsession.h"

#include <QObject>

class AbstractAudio : public QObject
{
    Q_OBJECT
public:
    AbstractAudio(){};
    ~AbstractAudio(){};
    MIPComponentChain m_chain;
};

#endif // ABSTRACTAUDIO_H
*/
