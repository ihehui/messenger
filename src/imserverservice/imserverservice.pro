TARGET = imserverservice
TEMPLATE = app
CONFIG += qt \
    console
QT += core \
    sql \
    network
QT -= gui

# include config file
include( ../config.pri )
HHSharedLibs += HHSharedCore \
    HHSharedNetwork \
    HHSharedService \
    HHSharedUDT
include(../../HHSharedLibs.pri)
HHSharedLibs += HHSharedIM \
    HHSharedIMServer
include(../imserver/imserver.pri)
include(../sharedim/sharedim.pri)
 
# ###
DEPENDPATH += 
INCLUDEPATH += 
SOURCES += main.cpp \
    serverservice/serverservice.cpp
HEADERS += \
    serverservice/serverservice.h \
    app_constants_imserverservice.h

# ##
# unix:target.path += ../bin
# win32:target.path += %windir%/system32
# INSTALLS += target
RESOURCES += resources.qrc

# win32:RC_FILE = iconresource.rc
#win32:APP_ICON = resources/images/app.ico
win32:RC_ICONS = resources/images/app.ico
mac:ICON = ./resources/images/app.icns

# define some usefull values
# QMAKE_TARGET_COMPANY	= "He Hui Team"
QMAKE_TARGET_PRODUCT = "Server Service Application For IM System"
QMAKE_TARGET_DESCRIPTION = "Crossplatform Service Application Based On Qt"
DEFINES *= "APP_NAME=\"\\\"$${QMAKE_TARGET_PRODUCT}\\\"\""
