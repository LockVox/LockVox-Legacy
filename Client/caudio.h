#ifndef CAUDIO_H
#define CAUDIO_H

#include "abstractaudio.h"
#include "emiplib/mipcomponentchain.h"

class CAudio : public AbstractAudio
{
public:
    CAudio();
    ~CAudio();

    int GetSoundInput();
    void OutputSound();
private:
    MIPComponentChain m_chain;
};

#endif // CAUDIO_H
