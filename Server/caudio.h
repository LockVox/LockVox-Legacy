#ifndef CAUDIO_H
#define CAUDIO_H

#include "abstractaudio.h"
#include "emiplib/mipcomponentchain.h"
#include "jrtplib3/rtpsession.h"
#include "QList"
#include <QObject>

class CAudio :  public QObject
{
    Q_OBJECT
public:
    CAudio();
    ~CAudio();
public slots:
    //void newChannel();
private:
    MIPComponentChain m_chain;
    QList<jrtplib::RTPSession> m_session;
};

#endif // CAUDIO_H
