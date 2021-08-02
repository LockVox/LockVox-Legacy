QT     += core xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network sql

CONFIG += c++11 console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
#src
    src/AbstractServer.cpp \
    src/cchannel.cpp \
    src/cmessage.cpp \
    src/cpacket.cpp \
    src/csessioncookie.cpp \
    src/crole.cpp \
#Server
    Server/main.cpp \
    Server/cserver.cpp \
    Server/cdatabase.cpp \
    Server/cclient.cpp \
    Server/caudio.cpp \
    Server/qsslserver.cpp \
    Server/sha256.cpp \
    Server/cookiesmanager.cpp \
    Client/messagelist.cpp

HEADERS += \
#src
    src/includes/AbstractServer.h \
    src/includes/cchannel.h \
    src/includes/cmessage.h \
    src/includes/cpacket.h \
    src/includes/csessioncookie.h \
    src/includes/crole.h \
#Server
    Server/includes/caudio.h \
    Server/includes/cookiesmanager.h \
    Server/includes/sha256.h \
    Server/includes/cserver.h \
    Server/includes/config.h \
    Server/includes/cclient.h \
    Server/includes/cdatabase.h \
    Server/includes/qsslserver.h \
    Client/includes/messagelist.h

FORMS +=


INCLUDEPATH += src \
    src/includes \
    Server/includes

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/libs/libmariadb/win32/ -llibmariadb

INCLUDEPATH += $$PWD/libs/libmariadb/include \
    $$PWD/libs/libmariadb/win32
DEPENDPATH += $$PWD/libs/libmariadb/win32


#Includes paths for libs
INCLUDEPATH += $$PWD/libs/include/
DEPENDPATH += $$PWD/libs/include/

INCLUDEPATH += $$PWD/libs/include/portaudio
DEPENDPATH += $$PWD/libs/include/portaudio

INCLUDEPATH += $$PWD/libs/include/opus
DEPENDPATH += $$PWD/libs/include/opus

INCLUDEPATH += $$PWD/libs/include/jthread
DEPENDPATH += $$PWD/libs/include/jthread

INCLUDEPATH += $$PWD/libs/include/jrtplib3
DEPENDPATH += $$PWD/libs/include/jrtplib3

INCLUDEPATH += $$PWD/libs/include/emiplib
DEPENDPATH += $$PWD/libs/include/emiplib


#portaudio
win32: LIBS += -L$$PWD/libs/lib/ -lportaudio_x64

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/lib/portaudio_x64.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/libs/lib/libportaudio_x64.a

#opus
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/lib/ -lopus

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/lib/libopus.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/lib/opus.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/lib/opus.lib
#jthread
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/lib/ -ljthread

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/lib/libjthread.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/lib/libjthread_d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/lib/jthread.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/lib/jthread_d.lib


#jrtplib
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/lib/ -ljrtplib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/lib/ -ljrtplib_d

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/lib/jrtplib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/lib/jrtplib_d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/lib/jrtplib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/lib/jrtplib_d.lib


#emiplib
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/lib/ -lemiplib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/lib/ -lemiplib_d

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/lib/emiplib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/lib/emiplib_d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/lib/emiplib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/lib/emiplib_d.lib
