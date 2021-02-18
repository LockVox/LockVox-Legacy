QT       += core gui multimedia xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network sql

CONFIG += c++11 console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/AbstractServer.cpp \
    src/abstractaudio.cpp \
    src/cchannel.cpp \
    src/cclient.cpp \
    src/cdatabase.cpp \
    src/cmessage.cpp \
    src/cpacket.cpp \
    Server/cserver.cpp \
    Server/main.cpp \
    Server/mainwindow.cpp \
    src/crole.cpp \
    Server/sha256.cpp

HEADERS += \
    Server/mainwindow.h \
    Server/sha256.h \
    Server/cserver.h \
    src/includes/cmessage.h \
    src/includes/AbstractServer.h \
    src/includes/abstractaudio.h \
    src/includes/cchannel.h \
    src/includes/cclient.h \
    src/includes/cdatabase.h \
    src/includes/cpacket.h \
    src/includes/cmessage.h \
    src/includes/crole.h


FORMS += \
    Server/mainwindow.ui


INCLUDEPATH += src \
    src/include \
    libs/emiplib/include \



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/libs/libmariadb/win32/ -llibmariadb

INCLUDEPATH += $$PWD/libs/libmariadb/include \
    $$PWD/libs/libmariadb/win32
DEPENDPATH += $$PWD/libs/libmariadb/win32

win32: LIBS += -L$$PWD/libs/openssl/win32/lib/ -llibcrypto
win32: LIBS += -L$$PWD/libs/openssl/win32/lib/ -llibssl
win32: LIBS += -L$$PWD/libs/openssl/win32/lib/ -lopenssl

INCLUDEPATH += $$PWD/libs/openssl/include \
    $$PWD/libs/openssl/win32
DEPENDPATH += $$PWD/libs/openssl/include \
    $$PWD/libs/openssl/win32



win32: LIBS += -L$$PWD/libs/emiplib/lib/ -ljthread

INCLUDEPATH += $$PWD/libs/emiplib/include/jthread
DEPENDPATH += $$PWD/libs/emiplib/include/jthread

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/emiplib/lib/jthread.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/libs/emiplib/lib/libjthread.a

win32: LIBS += -L$$PWD/libs/emiplib/lib/ -ljrtplib

INCLUDEPATH += $$PWD/libs/emiplib/include/jrtplib3
DEPENDPATH += $$PWD/libs/emiplib/include/jrtplib3

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/emiplib/lib/jrtplib.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/libs/emiplib/lib/libjrtplib.a
