
#DEFINES += MYSHAREDLIB_LIBRARY_EXPORT
win32 {
    contains(TEMPLATE, lib):contains(CONFIG, shared):DEFINES += HHSHAREDLIBS_EXPORT
    else:USELIB:DEFINES += HHSHAREDLIBS_IMPORT
}


INCLUDEPATH += $$OUT_PWD
DEPENDPATH += $$OUT_PWD


isEmpty(LIB_NAME){
    error(Empty LIB NAME! $$PWD $$OUT_PWD)
}


##Link with $${LIB_NAME}
contains(HHSharedLibs, $${LIB_NAME}){
    LIBS += -L$${LIB_PATH}

    CONFIG( debug, debug|release ) {
        # Debug
        message(Debug Build)
        unix:TARGET_LIB_NAME = $$quote($$join(LIB_NAME,,,_debug))
        else:TARGET_LIB_NAME = $$quote($$join(LIB_NAME,,,d))
    }else {
        # Release
        message(Release Build)
        TARGET_LIB_NAME = $$quote($$LIB_NAME)
    }

    win32:LIBS += -l$${TARGET_LIB_NAME}$${VER_MAJ}
    else:LIBS += -l$${TARGET_LIB_NAME}

    message(Link with library: $${TARGET_LIB_NAME} $$OUT_PWD)

}









