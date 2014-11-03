# get current path
PACKAGE_PWD = $$PWD

#SOURCE_TREE = $$PWD/../
SOURCE_TREE = $$PWD/

CONFIG( debug, debug|release ) {
        BIN_PATH = $${SOURCE_TREE}/bin/debug
} else {
        BIN_PATH = $${SOURCE_TREE}/bin/release
}
LIB_PATH = $${SOURCE_TREE}/lib
PLUGIN_PATH = $${BIN_PATH}/plugins/hehui
DLL_PATH = $${BIN_PATH}
HEADERS_PATH = $${SOURCE_TREE}/include


##config for application
contains(TEMPLATE, app){
    DESTDIR = $${BIN_PATH}
}

##config for library
contains(TEMPLATE, lib){
    contains(CONFIG, plugin){
        DLLDESTDIR = $${PLUGIN_PATH}

        win32:DESTDIR = $${LIB_PATH}
        else:DESTDIR = $$DLLDESTDIR
    }else{
        DLLDESTDIR = $$[QT_INSTALL_BINS]
        DESTDIR = $${LIB_PATH}
    }
    win32:QMAKE_DISTCLEAN += $${DLLDESTDIR}\\$${TARGET}.dll
}


# package name
PACKAGE_TARGET	= HeHui
mac:PACKAGE_TARGET	= $$quote(HeHui)

# package destdir
PACKAGE_DESTDIR	= $${PACKAGE_PWD}/bin

# temporary path for building
#PACKAGE_BUILD_PATH	= $${PACKAGE_PWD}/tmp/$${TARGET}
PACKAGE_BUILD_PATH	= $${SOURCE_TREE}/temp/$${TARGET}

# build mode
#CONFIG	+= qt warn_on thread x11 windows release

# define config mode paths
CONFIG( debug, debug|release ) {
        #message( DEBUG Build )
        #Output Debugging Messages, For Windows
        CONFIG	+= console

	unix:PACKAGE_TARGET	= $$quote($$join(PACKAGE_TARGET,,,_debug))
	else:PACKAGE_TARGET	= $$quote($$join(PACKAGE_TARGET,,,d))
	unix:TARGET	= $$quote($$join(TARGET,,,_debug))
	else:TARGET	= $$quote($$join(TARGET,,,d))
	unix:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/debug/.obj/unix
	win32:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/debug/.obj/win32
	mac:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/debug/.obj/mac
        #UI_DIR	= $${PACKAGE_BUILD_PATH}/debug/.ui
        #UI_DIR	= $${PACKAGE_PWD}/.ui
	MOC_DIR	= $${PACKAGE_BUILD_PATH}/debug/.moc
	RCC_DIR	= $${PACKAGE_BUILD_PATH}/debug/.rcc
} else {
        #message( RELEASE Build )
        #Disable Debug Output
        DEFINES += QT_NO_DEBUG_OUTPUT

	mac:TARGET	= $$quote($$TARGET)
	unix:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/release/.obj/unix
	win32:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/release/.obj/win32
	mac:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/release/.obj/mac
        #UI_DIR	= $${PACKAGE_BUILD_PATH}/release/.ui
        #UI_DIR	= $${PACKAGE_PWD}/.ui
	MOC_DIR	= $${PACKAGE_BUILD_PATH}/release/.moc
	RCC_DIR	= $${PACKAGE_BUILD_PATH}/release/.rcc
}


#set INCLUDEPATH
INCLUDEPATH += $${OUT_PWD} \
              $$UI_DIR \
              $$MOC_DIR \
              $$RCC_DIR  \
              $$OBJECTS_DIR


#DEPENDPATH += $$UI_DIR


# define some usefull values
#QMAKE_TARGET_COMPANY = "He Hui Team"
#QMAKE_TARGET_PRODUCT = "He Hui"
#QMAKE_TARGET_DESCRIPTION = "Crossplatform Libraries Based On Qt Of LGPL Edition"
#QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2007 - 2010 He Hui"
#PACKAGE_DOMAIN = "www.hehui.org"


#APP_ORG = "He Hui"
#APP_AUTHOR = "He Hui"
#APP_AUTHOR_EMAIL = "hehui@hehui.org"
#APP_LICENSE = "LGPL"

## define variable for source code
##DEFINES *= "APP_NAME=\"\\\"$${QMAKE_TARGET_PRODUCT}\\\"\"" \
#DEFINES *= "APP_VERSION=\"\\\"$${VERSION}\\\"\"" \
#        "APP_ORG=\"\\\"$${APP_ORG}\\\"\"" \
#        "APP_ORG_DOMAIN=\"\\\"$${PACKAGE_DOMAIN}\\\"\"" \
#        "APP_AUTHOR=\"\\\"$${APP_AUTHOR}\\\"\"" \
#        "APP_AUTHOR_EMAIL=\"\\\"$${APP_AUTHOR_EMAIL}\\\"\"" \
#        "APP_LICENSE=\"\\\"$${APP_LICENSE}\\\"\"" \
#        "APP_COPYRIGHTS=\"\\\"$${QMAKE_TARGET_COPYRIGHT}\\\"\""


win32-g++:QMAKE_LFLAGS = -enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc
#win32-g++:QMAKE_CXXFLAGS_EXCEPTIONS_OFF =
