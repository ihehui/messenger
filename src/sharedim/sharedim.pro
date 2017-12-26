include(sharedim.pri)
isEmpty(LIB_NAME):LIB_NAME = HHSharedIM

# ##
TEMPLATE = lib

# CONFIG += static
CONFIG += shared
TARGET = $${LIB_NAME}
QT += core \
    sql \
    network

# include config file
include(../config.pri)

HHSharedLibs += HHSharedCore \
    HHSharedNetwork \
    HHSharedUDT \
    HHSharedENET
include(../../HHSharedLibs.pri)

# Input
HEADERS += \
    constants_global_shared.h \
    imuserbase.h \
    sharedimlib.h \
    udtprotocol.h \
    tcpserver.h \
    rtp.h \
    resourcesmanager.h \
    filemanager.h \
    contactgroupbase.h \
    interestgroupbase.h \
    imgroupbase.h \
    resourcesmanagerinstance.h \
    impackets.h \
    enetprotocol.h \
    filetransmitter/filetransmissionpacketsparserbase.h \
    networkmanagerbase.h \
    filetransmitter/filetransmissionmanagerbase.h
SOURCES += \
    imuserbase.cpp \
    udtprotocol.cpp \
    tcpserver.cpp \
    rtp.cpp \
    resourcesmanager.cpp \
    filemanager.cpp \
    contactgroupbase.cpp \
    interestgroupbase.cpp \
    imgroupbase.cpp \
    resourcesmanagerinstance.cpp \
    impackes.cpp \
    enetprotocol.cpp \
    filetransmitter/filetransmissionpacketsparserbase.cpp \
    networkmanagerbase.cpp \
    filetransmitter/filetransmissionmanagerbase.cpp
FORMS += 
RESOURCES += resources.qrc

DEFINES += SHAREDIMLIB_LIBRARY_EXPORT


!build_pass:CONFIG(debug, debug|release|debug_and_release|build_all) {
    message(Debug Build!)
    DEFINES += DEBUG __DEBUG__
}else{
    message( "Release Build! No qDebug() output." )
    DEFINES += QT_NO_DEBUG_OUTPUT
}


# ##
unix:target.path += /usr/lib
win32:target.path += %windir%/system32
INSTALLS += target

# define some usefull values
# QMAKE_TARGET_COMPANY	= "He Hui"
QMAKE_TARGET_PRODUCT = "Shared Library For IM"
QMAKE_TARGET_DESCRIPTION = "Crossplatform Shared Library For IM Based On Qt"
