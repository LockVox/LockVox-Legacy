QT       += core gui multimedia xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network sql

CONFIG += c++11 console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Server/caudio.cpp \
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
    Server/sha256.cpp \
    Client/messagelist.cpp

HEADERS += \
    Server/caudio.h \
    Server/mainwindow.h \
    Server/sha256.h \
    Server/cserver.h \
    Server/config.h \
    src/includes/cmessage.h \
    src/includes/AbstractServer.h \
    src/includes/abstractaudio.h \
    src/includes/cchannel.h \
    src/includes/cclient.h \
    src/includes/cdatabase.h \
    src/includes/cpacket.h \
    src/includes/cmessage.h \
    src/includes/crole.h \
    Client/includes/messagelist.h


FORMS += \
    Server/mainwindow.ui


INCLUDEPATH += src \
    src/include \
    libs/emiplib/include \
    libs/emiplib/portaudio \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/libs/libmariadb/win32/ -llibmariadb

INCLUDEPATH += $$PWD/libs/libmariadb/include \
    $$PWD/libs/libmariadb/win32
DEPENDPATH += $$PWD/libs/libmariadb/win32

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/emiplib/lib/ -ljthread
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/emiplib/lib/ -ljthread_d

INCLUDEPATH += $$PWD/libs/emiplib/include
DEPENDPATH += $$PWD/libs/emiplib/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/emiplib/lib/ -ljrtplib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/emiplib/lib/ -ljrtplib_d


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/emiplib/lib/ -lemiplib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/emiplib/lib/ -lemiplib_d

win32: LIBS += -L$$PWD/libs/emiplib/lib/ -lportaudio

INCLUDEPATH += $$PWD/libs/emiplib/include/portaudio
DEPENDPATH += $$PWD/libs/emiplib/include/portaudio
