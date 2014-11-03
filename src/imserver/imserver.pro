include(imserver.pri)
isEmpty(LIB_NAME):LIB_NAME = HHSharedIMServer

# ##
TEMPLATE = lib

# CONFIG += static
CONFIG += shared
TARGET = $${LIB_NAME}

# ##
# TEMPLATE = lib
# CONFIG += shared
# TARGET = HHSharedServer
QT += core \
    sql \
    network \
    xml
QT -= gui

# include config file
include( ../config.pri )
HHSharedLibs += HHSharedCore \
    HHSharedNetwork \
    HHSharedUDT
include(../../HHSharedLibs.pri)
HHSharedLibs += HHSharedIM
include (../sharedim/sharedim.pri)

# #
#UI_DIR = ./ui

# ##
#DEPENDPATH += .

INCLUDEPATH += . \
    packetmanager \
    usersmanager

# Input
HEADERS += \
    serverutilities.h \
    settings.h \
    server.h \
    app_constants_server.h \
    packetmanager/serverpacketsparser.h \
    usersmanager/usersmanager.h \
    usersmanager/userinfo.h \
    serverlib.h \
    usersmanager/interestgroup.h \
    usersmanager/contactgroup1.h \
    filetransmitter/serverfiletransmissionmanager.h \
    filetransmitter/serverfiletransmissionpacketsparser.h \
    filetransmitter/fileservermanager.h \
    filetransmitter/fileserver.h
SOURCES += \
    serverutilities.cpp \
    settings.cpp \
    server.cpp \
    packetmanager/serverpacketsparser.cpp \
    usersmanager/usersmanager.cpp \
    usersmanager/userinfo.cpp \
    usersmanager/contactgroup.cpp \
    usersmanager/interestgroup.cpp \
    filetransmitter/serverfiletransmissionmanager.cpp \
    filetransmitter/serverfiletransmissionpacketsparser.cpp \
    filetransmitter/fileservermanager.cpp \
    filetransmitter/fileserver.cpp
FORMS += 
RESOURCES += resources.qrc
win32 { 
    # DLLDESTDIR = $${HHSHAREDLIBS_BIN_TARGET_PATH}
    DLLDESTDIR = $$[QT_INSTALL_BINS]
    QMAKE_DISTCLEAN += $$[QT_INSTALL_BINS]\\$${TARGET}.dll
}

DEFINES += SERVERLIB_LIBRARY_EXPORT


# ##
unix:target.path += /usr/lib
win32:target.path += %windir%/system32
INSTALLS += target

# define some usefull values
QMAKE_TARGET_PRODUCT = "Server Library For IM System"
QMAKE_TARGET_DESCRIPTION = "Crossplatform Server Library For IM System Based On Qt"
DEFINES *= "APP_NAME=\"\\\"$${QMAKE_TARGET_PRODUCT}\\\"\""
