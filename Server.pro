QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network sql

CONFIG += c++11 console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Server/qxmpp_password_checker.cpp \
    Server/qxmpp_server.cpp \
    Server/sha256.cpp \
    src/audioinput.cpp \
    src/audiooutput.cpp \
    src/cchannel.cpp \
    src/cclient.cpp \
    src/cdatabase.cpp \
    src/cpacket.cpp \
    src/cserver.cpp \
    Server/main.cpp \
    Server/mainwindow.cpp

HEADERS += \
    Server/mainwindow.h \
    Server/qxmpp_password_checker.h \
    Server/qxmpp_server.h \
    Server/sha256.h \
    src/includes/audioinput.h \
    src/includes/audiooutput.h \
    src/includes/cchannel.h \
    src/includes/cclient.h \
    src/includes/cdatabase.h \
    src/includes/cpacket.h \
    src/includes/cserver.h

FORMS += \
    Server/mainwindow.ui


INCLUDEPATH += src \
    src/include \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32: LIBS += -L$$PWD/libs/QXmpp/win32/ -lqxmpp

INCLUDEPATH += $$PWD/libs/QXmpp/win32 \
    $$PWD/libs/QXmpp/win32/base \
    $$PWD/libs/QXmpp/win32/client \
    $$PWD/libs/QXmpp/win32/server

DEPENDPATH += $$PWD/libs/QXmpp/win32

unix:!macx: LIBS += -L$$PWD/libs/QXmpp/unix/ -lqxmpp

INCLUDEPATH += $$PWD/libs/QXmpp/unix \
    $$PWD/libs/QXmpp/unix/base \
    $$PWD/libs/QXmpp/unix/client \
    $$PWD/libs/QXmpp/unix/server

DEPENDPATH += $$PWD/libs/QXmpp/unix

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

