QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network sql

CONFIG += c++11 console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/cchannel.cpp \
    src/cclient.cpp \
    src/main.cpp \
    src/cserver.cpp \
    src/cdatabase.cpp \
    src/qma.cpp \
    src/qxmpp_server.cpp

HEADERS += \
    src/includes/cchannel.h \
    src/includes/cclient.h \
    src/includes/cserver.h \
    src/includes/cdatabase.h \
    src/includes/qma.h \
    src/includes/qxmpp_server.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/src \
    $$PWD/src/include \
    $$PWD/src/ui

FORMS += src/ui/mainwindow.ui \
    src/ui/login_interface.ui \
    src/ui/register_interface.ui

DISTFILES += \
    ressources/qmodel/lockvox.qmodel \
    ressources/translation/LockVox_en_US.ts

RESOURCES += \
    ressources/ressources.qrc \
    src/ui/ui.qrc

TRANSLATIONS += \
    ressources/translation/LockVox_fr_FR.ts \
    ressources/translation/LockVox_en_US.ts

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

win32: LIBS += -L$$PWD/libs/srtp2/win32/ -lsrtp2

INCLUDEPATH += $$PWD/libs/srtp2/win32
DEPENDPATH += $$PWD/libs/srtp2/win32

unix:!macx: LIBS += -L$$PWD/libs/srtp2/unix/ -lsrtp2

INCLUDEPATH += $$PWD/libs/srtp2/unix
DEPENDPATH += $$PWD/libs/srtp2/unix
