#ifndef CAUDIO_H
#define CAUDIO_H

#include "abstractaudio.h"
#include "src/includes/cchannel.h"
#include "emiplib/mipcomponentchain.h"
#include "jrtplib3/rtpsession.h"
#include <QList>
#include <QObject>

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
    //QList<MIPComponentChain> m_chain;
    QList<jrtplib::RTPSession*> m_session;
};

#endif // CAUDIO_H
