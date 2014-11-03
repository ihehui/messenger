# ##
TEMPLATE = app
TARGET = messenger
QT += core \
    gui \
    widgets \
    sql \
    network \
    xml \
    webkit \
    webkitwidgets
qtHaveModule(printsupport): QT += printsupport


# include config file
include( ../config.pri )
HHSharedLibs += HHSharedCore \
    HHSharedGUI \
    HHSharedNetwork \
    HHSharedUDT
include(../../HHSharedLibs.pri)

HHSharedLibs += HHSharedIM
include (../sharedim/sharedim.pri)

#
UI_DIR = ./ui


# Input
HEADERS += contactinfowidget/contactinfowidget.h \
    informationtips/userinfotipwindow.h \
    chatwindow/groupchatwindow.h \
    chatwindow/emoticonselecter/emoticonselectoruibase.h \
    chatwindow/emoticonselecter/customface.h \
    chatwindow/emoticonselecter/evaresource.h \
    search/userinfomodel.h \
    search/search.h \
    imageresource.h \
    imuser.h \
    settings.h \
    app_constants_client.h \
    contactsmanager/contact.h \
    networkmanager/servermanagerwindow.h \
    networkmanager/serverinfo.h \
    networkmanager/serverinfomodel.h \
    login/loginwidget.h \
    chatwindow/chatmessagewindow.h \
    chatwindow/chatwindowmanager.h \
    contactsmanager/contactsmanager.h \
    login/login.h \
    about/aboutdialog.h \
    mainwindow/mainwindow.h \
    login/applyforregistrationwidget.h \
    search/addcontactdialog.h \
    chatwindow/emoticonselecter/emoticonselector.h \
    contactsmanager/interestgroup.h \
    packetmanager/imclientpacketsparser.h \
    networkmanager/clientresourcesmanager.h \
    contractstree/contractstree.h \
    chatwindow/contactchatwidget.h \
    contactsmanager/contactgroup1.h \
    deletecontactdialog/deletecontactdialog.h \
    search/interestgroupinfomodel.h \
    servertime/servertime.h \
    contactbox/sheet_delegate_p.h \
    contactbox/contactwidget.h \
    contactbox/contactbox.h \
    chatwindow/messageview.h \
    chatwindow/messagehistoryview.h \
    imageviewer/animationcontroler.h \
    imageviewer/imageviewer.h \
    imageviewer/imageviewercontroler.h \
    chatwindow/filetransmitter/filetransmissionlistwidget.h \
    chatwindow/filetransmitter/progressinfowidget.h \
    chatwindow/filetransmitter/clientfiletransmissionmanager.h \
    chatwindow/filetransmitter/clientfiletransmissionpacketsparser.h
SOURCES += contactinfowidget/contactinfowidget.cpp \
    informationtips/userinfotipwindow.cpp \
    chatwindow/groupchatwindow.cpp \
    chatwindow/emoticonselecter/emoticonselectoruibase.cpp \
    chatwindow/emoticonselecter/customface.cpp \
    chatwindow/emoticonselecter/evaresource.cpp \
    search/userinfomodel.cpp \
    search/search.cpp \
    imageresource.cpp \
    imuser.cpp \
    settings.cpp \
    contactsmanager/contact.cpp \
    networkmanager/servermanagerwindow.cpp \
    networkmanager/serverinfo.cpp \
    networkmanager/serverinfomodel.cpp \
    login/loginwidget.cpp \
    contactsmanager/contactgroup.cpp \
    chatwindow/chatmessagewindow.cpp \
    chatwindow/chatwindowmanager.cpp \
    contactsmanager/contactsmanager.cpp \
    login/login.cpp \
    main.cpp \
    about/aboutdialog.cpp \
    mainwindow/mainwindow.cpp \
    login/applyforregistrationwidget.cpp \
    search/addcontactdialog.cpp \
    chatwindow/emoticonselecter/emoticonselector.cpp \
    contactsmanager/interestgroup.cpp \
    packetmanager/imclientpacketsparser.cpp \
    networkmanager/clientresourcesmanager.cpp \
    contractstree/contractstree.cpp \
    chatwindow/contactchatwidget.cpp \
    deletecontactdialog/deletecontactdialog.cpp \
    search/interestgroupinfomodel.cpp \
    servertime/servertime.cpp \
    contactbox/sheet_delegate.cpp \
    contactbox/contactwidget.cpp \
    contactbox/contactbox.cpp \
    chatwindow/messageview.cpp \
    chatwindow/messagehistoryview.cpp \
    imageviewer/animationcontroler.cpp \
    imageviewer/imageviewer.cpp \
    imageviewer/imageviewercontroler.cpp \
    chatwindow/filetransmitter/filetransmissionlistwidget.cpp \
    chatwindow/filetransmitter/progressinfowidget.cpp \
    chatwindow/filetransmitter/clientfiletransmissionpacketsparser.cpp \
    chatwindow/filetransmitter/clientfiletransmissionmanager.cpp
FORMS += contactinfowidget/contactinfowidget.ui \
    informationtips/userinfotipwindow.ui \
    chatwindow/contactchatwidget.ui \
    chatwindow/groupchatwindow.ui \
    search/search.ui \
    networkmanager/servermanagerwindow.ui \
    login/loginwidget.ui \
    chatwindow/chatmessagewindow.ui \
    chatwindow/chatwindowmanager.ui \
    about/aboutdialog.ui \
    mainwindow/mainwindow.ui \
    login/applyforregistrationwidget.ui \
    search/addcontactdialog.ui \
    deletecontactdialog/deletecontactdialog.ui \
    contactbox/contactwidget.ui \
    chatwindow/messagehistoryview.ui \
    imageviewer/animationcontroler.ui \
    imageviewer/imageviewercontroler.ui \
    chatwindow/filetransmitter/filetransmissionlistwidget.ui \
    chatwindow/filetransmitter/progressinfowidget.ui
RESOURCES += resources/images/emoticons/emoticons.qrc \
    resources/images/face/face.qrc \
    resources/images/chatmessagewindow/chatmessagewindow.qrc \
    resources/images/imageviewer/imageviewer.qrc \
    resources.qrc


INCLUDEPATH += chatwindow

# win32:RC_FILE = iconresource.rc
#win32:APP_ICON = resources/images/app.ico
win32:RC_ICONS = resources/images/app.ico
mac:ICON = ./resources/images/app.icns

# DESTDIR = ../bin
unix:target.path += ../bin
win32:target.path += %windir%/system32
INSTALLS += target

# define some usefull values
# QMAKE_TARGET_COMPANY	= "He Hui"
QMAKE_TARGET_PRODUCT = "Messenger"
QMAKE_TARGET_DESCRIPTION = "Crossplatform Client Application For IM Based On Qt"
DEFINES *= "APP_NAME=\"\\\"$${QMAKE_TARGET_PRODUCT}\\\"\""
