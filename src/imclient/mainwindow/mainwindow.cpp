/*
 *
 *
 *
 *
 */



#include <QtGui>
#include <QPair>
#include <QListWidget>
#include <QFileInfo>

#include "mainwindow.h"
#include "../contactbox/sheet_delegate_p.h"
#include "../about/aboutdialog.h"

#include "../settings.h"
#include "../imageresource.h"

#include "../search/addcontactdialog.h"
#include "../servertime/servertime.h"

#include "../../sharedim/constants_global_shared.h"

//#include "HHSharedCore/hdatabaseutility.h"
//#include "HHSharedGUI/hloginbase.h"
#include "HHSharedGUI/hguiinterface.h"
#include "HHSharedGUI/hpluginmanagerwindow.h"
#include "HHSharedCore/hutilities.h"



namespace HEHUI {


MainWindow::MainWindow(QWidget *parent, HEHUI::WindowPosition positon) :
    MainWindowBase(parent)
{
    
    ui.setupUi(this);

    setWindowTitle(QString(APP_NAME) + " Build " + QString(APP_VERSION));
    //setWindowFlags(Qt::SplashScreen | Qt::WindowStaysOnTopHint);
    //setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
    setWindowFlags(Qt::Dialog);


    //使窗口居中
    //Center the window
    //moveWindow(positon);

    //resize(QSize(0,0));
    //showMinimized();



    m_chatWindowManager = 0;
    //    expandListViewManager = 0;
    m_userInfoTipWindow = 0;

    progressDialog = 0;




    //初始化UI
    //Init the UI
    initUI();



    //创建托盘图标
    //Create the system tray
    setupSystemTray();

    if(Settings::instance()->getRestoreWindowStateOnStartup()){
        Settings::instance()->restoreState(this);
    }else{
        //使窗口居中
        //Center the window
        moveWindow(positon);
    }

    loadPlugins();



    m_contactsManager = ContactsManager::instance();

    m_myself = IMUser::instance();
    m_myUserID = "";
    m_systemID = "system";
    stateBeforeLocking = IM::ONLINESTATE_OFFLINE;



    //检查自动登陆
    //Check Auto Login
    QTimer::singleShot(100, ui.loginPage, SLOT(slotCheckAutoLogin()));
    /*
  bool invisibleLogin = Settings::instance()->getInvisibleLogin();
 bool autoLogin = Settings::instance()->getAutoLogin();
 if(autoLogin){
  QString uid = Settings::instance()->getRecentUser();
  QByteArray pswd = QByteArray::fromBase64(Settings::instance()->getRecentUserPassword());

  User::instance()->setUserID(uid);
  User::instance()->setPassword(pswd);
  User::instance()->setOnlineState(invisibleLogin ? User::INVISIBLE : User::ONLINE);
  ui.loginPage->verifyUser();

 }
*/



    //m_packetHandler = 0;
    m_resourcesManager = 0;
    clientPacketsParser = 0;
    networkStarted = false;

    m_udpServer = 0;
    m_rtp = 0;
    m_socketConnectedToServer = INVALID_SOCK_ID;

    checkNetwork();

    search = 0;
    m_deleteContactDialog = 0;

    autoShowSystemMessage = false;
    autoShowChatMessageFromContact = false;



    //m_ContactInfoWidget = new ContactInfoWidget(this);

    m_serverHostAddress = QHostAddress::Null;
    m_serverHostPort = 0;
    m_verified = false;

    m_loginTimer = 0;

//    m_fileManager = 0;


}

MainWindow::~MainWindow() {
    qDebug()<<"----MainWindow::~MainWindow()";

    /*

 if(User::instance()->getOnlineState() != User::OFFLINE && User::instance()->getOnlineState() != User::INVISIBLE){
                emit signalUserOnlineStateChanged(User::OFFLINE);
                qDebug() << "----MainWindow::~MainWindow()~~User::OFFLINE";
 }

 if(Settings::instance()->getRestoreWindowStateOnStartup()){
  Settings::instance()->saveState(this);
 }

 //关闭所有的数据库连接
 //Close all database connections
 DatabaseUtility::closeAllDBConnections();

 systemTray->hide();

*/


    //    delete m_userInfoTipWindow;
    //    m_userInfoTipWindow = 0;

    deleteLater();

}

void MainWindow::closeEvent(QCloseEvent * event) {
    if (Settings::instance()->getHideOnClose()) {
        showMinimized();
        hide();
        event->ignore();
    }else{

        aboutToQuit();

        event->accept();
    }

}


void MainWindow::changeEvent ( QEvent * event ){
    if(event->type() == QEvent::WindowStateChange){
        switch (windowState ()) {
        case Qt::WindowNoState:
            //ui.actionShowMaximized->setEnabled(true);
            //ui.actionShowFullScreen->setEnabled(true);
            ui.actionShowMinimized->setEnabled(true);
            ui.actionShowNormal->setEnabled(false);
            break;
        case Qt::WindowMinimized:
            //ui.actionShowMaximized->setEnabled(true);
            //ui.actionShowFullScreen->setEnabled(true);
            ui.actionShowMinimized->setEnabled(false);
            ui.actionShowNormal->setEnabled(true);
            break;
        case Qt::WindowMaximized:
            //ui.actionShowMaximized->setEnabled(false);
            //ui.actionShowFullScreen->setEnabled(true);
            ui.actionShowMinimized->setEnabled(true);
            ui.actionShowNormal->setEnabled(true);
            break;
        case Qt::WindowFullScreen:
            //ui.actionShowMaximized->setEnabled(false);
            //ui.actionShowFullScreen->setEnabled(false);
            ui.actionShowMinimized->setEnabled(true);
            ui.actionShowNormal->setEnabled(true);
            break;
        default:
            //TODO: Other State
            break;
        }

        event->accept();
        return;
    }

    event->ignore();

}



void MainWindow::initUI(){

    ui.menuView->addSeparator();
    ui.menuView->addMenu(getStyleMenu(Settings::instance()->getStyle(), Settings::instance()->getPalette()));
    QString qmLocale = Settings::instance()->getLanguage();
    QString qmPath = QApplication::applicationDirPath() + QDir::separator () + QString(LANGUAGE_FILE_DIR);
    ui.menuView->addMenu(getLanguageMenu(qmPath, qmLocale));

    pluginsMenu = getPluginsMenu();
    menuBar()->insertMenu(ui.menuHelp->menuAction(), pluginsMenu);


    connect(ui.actionBugReport, SIGNAL(triggered()), this, SLOT(slotBugReport()));

    connect(ui.actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));

    connect(ui.actionHelp, SIGNAL(triggered()), this, SLOT(slotHelp()));

    connect(ui.actionSystemConfigure, SIGNAL(triggered()), this, SLOT(slotSystemConfig()));

    connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(slotQuit()));

    connect(ui.toolButtonSerach, SIGNAL(clicked()), this, SLOT(slotSearch()));
    connect(ui.lockToolButton, SIGNAL(clicked()), this, SLOT(slotLockUI()));

    ui.stackedWidget->setCurrentWidget(ui.loginPage);
    //登陆页验证完成后的信号处理
    //Process the signal from 'ui.loginPage'
    connect(ui.loginPage, SIGNAL(signalUserVerified()), this, SLOT(slotUserVerified()));

    //changeStyle(Settings::instance()->getStyle());


    m_chatWindowManager = ChatWindowManager::instance();


    m_contactBox= new ContactBox(ui.pageContacts);
    ui.gridLayoutPageContacts->addWidget(m_contactBox, 0, 0, 1, 1);
    connect(m_contactBox, SIGNAL(signalContextMenuEventOnContactGroup(ContactGroupBase *, const QPoint &)), this, SLOT(handleContextMenuEventOnContactGroup(ContactGroupBase *, const QPoint &)));
    connect(m_contactBox, SIGNAL(signalContextMenuEventOnContact(Contact *, const QPoint &)), this, SLOT(handleContextMenuEventOnContact(Contact *, const QPoint &)));
    connect(m_contactBox, SIGNAL(signalContactItemActivated(Contact *)), this, SLOT(requestChatWithContact(Contact *)));
    connect(m_contactBox, SIGNAL(signalTooltipEventOnContact(Contact *, const QPoint &, const QPoint &)), this, SLOT(handleTooltipEventOnContactItem(Contact *, const QPoint &, const QPoint &)));




    m_recentChatGroupsItem = new QTreeWidgetItem();
    m_recentChatGroupsItem->setText(0, tr("Groups"));
    ui.treeWidgetRecentChats->addTopLevelItem(m_recentChatGroupsItem);

    m_recentChatFriendsItem= new QTreeWidgetItem();
    m_recentChatFriendsItem->setText(0, tr("Friends"));
    ui.treeWidgetRecentChats->addTopLevelItem(m_recentChatFriendsItem);

    m_recentChatStrangersItem= new QTreeWidgetItem();
    m_recentChatStrangersItem->setText(0, tr("Strangers"));
    ui.treeWidgetRecentChats->addTopLevelItem(m_recentChatStrangersItem);

    ui.treeWidgetRecentChats->setItemDelegate(new SheetDelegate(ui.treeWidgetRecentChats, ui.treeWidgetRecentChats));
    ui.treeWidgetRecentChats->expandAll();



    m_userInfoTipWindow = new UserInfoTipWindow(this);
    connect(m_userInfoTipWindow, SIGNAL(showUserInfoRequested(IMUserBase*)), this, SLOT(showUserInfo(IMUserBase*)));


    connect(ui.listWidgetGroups, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(handleContextMenuEventOnInterestGroupList(const QPoint &)));
    connect(ui.listWidgetGroups, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(interestGroupItemActivated(QListWidgetItem*)));


}

void MainWindow::checkNetwork(){

    //m_packetHandler = 0;
    m_resourcesManager = ClientResourcesManager::instance();
    clientPacketsParser = 0;
    networkStarted = false;

    startNetwork();


}

void MainWindow::startNetwork(){

    if(networkStarted){
        qWarning()<<"Network service has already started!";
        return;
    }



    QString errorMessage = "";
    m_udpServer = m_resourcesManager->startUDPServer(QHostAddress::Any, 0, true, &errorMessage);
    if(!m_udpServer){
        QMessageBox::critical(this, tr("Error"), tr("Can not start UDP listening! %1").arg(errorMessage));
    }else{
        qWarning()<<QString("UDP listening on port %1!").arg(m_udpServer->localPort());
    }

    m_rtp = m_resourcesManager->startRTP(QHostAddress::Any, 0, true, &errorMessage);
    if(!errorMessage.isEmpty()){
        QMessageBox::critical(this, tr("Error"), errorMessage);
    }
    //connect(m_rtp, SIGNAL(connected(int, const QString &, quint16)), this, SLOT(peerConnected(int, const QString &, quint16)));
    connect(m_rtp, SIGNAL(disconnected(int)), this, SLOT(peerDisconnected(int)));



    clientPacketsParser = new IMClientPacketsParser(m_resourcesManager, this);
    //    connect(clientPacketsParser, SIGNAL(signalServerDeclarePacketReceived(const QString&, quint16, const QString&, const QString&)), this, SLOT(serverFound(const QString& ,quint16, const QString&, const QString&)), Qt::QueuedConnection);

    connect(clientPacketsParser, SIGNAL(signalServerDeclarePacketReceived(const QString&, quint16, const QString&, const QString&)), ui.loginPage, SIGNAL(signalServerFound(const QString& , quint16, const QString&, const QString&)), Qt::QueuedConnection);
    connect(ui.loginPage, SIGNAL(requestRegistrationServerInfo()), this, SLOT(requestRegistrationServerInfo()), Qt::QueuedConnection);
    connect(ui.loginPage, SIGNAL(registration()), this, SLOT(requestRegistration()), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalRegistrationServerInfoReceived(quint8, bool, const QString &, quint8, const QString &, bool)), ui.loginPage, SIGNAL(signalRegistrationServerInfoReceived(quint8, bool, const QString &, quint8, const QString &, bool)), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalRegistrationResultReceived(quint8, quint32, const QString&)), ui.loginPage, SIGNAL(signalRegistrationResultReceived(quint8, quint32, const QString&)), Qt::QueuedConnection);
    connect(ui.loginPage, SIGNAL(signalRequestLogin(const QHostAddress &, quint16 )), this, SLOT(requestLogin(const QHostAddress &, quint16)));
    connect(ui.loginPage, SIGNAL(signalLookForServer(const QHostAddress &, quint16 )), clientPacketsParser, SLOT(sendClientLookForServerPacket(const QHostAddress &, quint16)), Qt::QueuedConnection);
    connect(ui.loginPage, SIGNAL(signalKickedOff()), this, SLOT(slotProcessKickedOff()));

    connect(this,SIGNAL(signalMyOnlineStateChanged(int, quint8)), clientPacketsParser, SLOT(changeMyOnlineState(int, quint8)), Qt::QueuedConnection);
    
    connect(clientPacketsParser, SIGNAL(signalUpdatePasswordResultReceived(quint8, const QString&)), this, SLOT(slotProcessUpdatePasswordResult(quint8, const QString&)), Qt::QueuedConnection);
    
    connect(clientPacketsParser, SIGNAL(signalLoginServerRedirected(const QString &, quint16, const QString &)), this, SLOT(slotProcessLoginServerRedirected(const QString &, quint16, const QString &)), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalLoginResultReceived(quint8, const QString &)), this, SLOT(slotProcessLoginResult(quint8, const QString &)), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalClientLastLoginInfoPacketReceived(const QString &, const QString &, const QString &, const QString &)), this, SLOT(slotProcessClientLastLoginInfo(const QString &, const QString &, const QString &, const QString &)), Qt::QueuedConnection);

    connect(clientPacketsParser, SIGNAL(signalContactStateChangedPacketReceived(const QString &, quint8, const QString &, quint16)), this, SLOT(slotProcessContactStateChanged(const QString &, quint8, const QString &, quint16)), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalContactsOnlineInfoPacketReceived(const QString & )), this, SLOT(slotProcessContactsOnlineInfo(const QString & )), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalUserInfoPacketReceived(const QString &)), this, SLOT(slotProcessUserInfo(const QString &)), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalContactGroupsInfoPacketReceived(const QString &, quint32 )), this, SLOT(slotProcessContactGroupsInfo(const QString &, quint32 )), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalContactsInfoVersionPacketReceived(const QString, quint32)), this, SLOT(slotProcessContactsInfoVersion(const QString, quint32)), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalCreateOrDeleteContactGroupResultPacketReceived(quint32,const QString &,bool,bool)), this, SLOT(slotProcessCreateOrDeleteContactGroupResult(quint32, const QString &,bool,bool)), Qt::QueuedConnection);


    //connect(clientPacketsParser, SIGNAL(signalSearchContactsResultPacketReceived(const QString &)), this, SLOT(slotProcessSearchContactsResult(const QString &)), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalAddContactRequestFromUserPacketReceived(const QString &, const QString &, const QString &, const QString & )), this, SLOT(slotProcessContactRequestFromUser(const QString &, const QString &, const QString &, const QString & )), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalAddContactResultPacketReceived(const QString &, const QString &, const QString &, int, quint8, const QString &, quint8 )), this, SLOT(slotProcessAddContactResult(const QString &, const QString &, const QString &, int, quint8, const QString &, quint8)), Qt::QueuedConnection);

    connect(clientPacketsParser, SIGNAL(signalDeleteContactResultPacketReceived(const QString &, bool, bool)), this, SLOT(slotDeleteContactResultReceived(const QString &, bool, bool)), Qt::QueuedConnection);


    connect(clientPacketsParser, SIGNAL(signalPersonalMessagePacketReceived(const QString &, const QString &)), this, SLOT(slotProcessPersonalMessage(const QString &, const QString &)), Qt::QueuedConnection);

    connect(clientPacketsParser, SIGNAL(signalChatMessageReceivedFromContact(const QString &, const QString &, const QString &)), this, SLOT(slotProcessChatMessageReceivedFromContact(const QString &, const QString &, const QString &)), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalChatMessageCachedOnServerReceived(const QStringList & )), this, SLOT(slotProcessChatMessageCachedOnServer(const QStringList & )), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalImageDownloadResultReceived(const QString &, const QString &, const QByteArray & )), this, SLOT(slotProcessImageDownloadResult(const QString &, const QString &, const QByteArray & )), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalImageDownloadRequestReceived(const QString &, const QString &)), this, SLOT(slotProcessImageDownloadRequest(const QString &, const QString &)), Qt::QueuedConnection);


    connect(clientPacketsParser, SIGNAL(signalInterestGroupChatMessageReceivedFromContact(quint32, const QString &, const QString &, const QString &)), this, SLOT(slotProcessInterestGroupChatMessagesReceivedFromContact(quint32, const QString &, const QString &, const QString &)), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalInterestGroupChatMessagesCachedOnServerReceived(const QStringList & )), this, SLOT(slotProcessInterestGroupChatMessagesCachedOnServer(const QStringList & )), Qt::QueuedConnection);

    
    connect(clientPacketsParser, SIGNAL(signalInterestGroupsListPacketReceived(const QString &, quint32 )), this, SLOT(slotProcessInterestGroupsList(const QString &, quint32 )), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalInterestGroupInfoPacketReceived(const QString &, quint32 )), this, SLOT(slotProcessInterestGroupInfo(const QString &, quint32 )), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalInterestGroupMembersInfoPacketReceived(const QString &, quint32, quint32 )), this, SLOT(slotProcessInterestGroupMembersInfo(const QString &, quint32, quint32 )), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalCreateInterestGroupResultPacketReceived(quint32, const QString &)), this, SLOT(slotProcessCreateInterestGroupResult(quint32, const QString &)), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalDisbandInterestGroupResultPacketReceived(quint32, bool)), this, SLOT(slotProcessDisbandInterestGroupResult(quint32, bool)), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalUserRequestJoinInterestGroupsPacketReceived(quint32, const QString &, const QString &, const QString &, const QString &)), this, SLOT(slotProcessUserRequestJoinInterestGroup(quint32, const QString &, const QString &, const QString &, const QString &)), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalUserJoinOrQuitInterestGroupPacketReceived(quint32, const QString &, bool)), this, SLOT(slotProcessUserJoinOrQuitInterestGroup(quint32, const QString &, bool)), Qt::QueuedConnection);

    
    //File TX
    connect(clientPacketsParser, SIGNAL(signalContactRequestUploadFile(const QString &, const QByteArray &, const QString &, quint64)), m_chatWindowManager, SLOT(contactRequestUploadFile(const QString &, const QByteArray &, const QString &,quint64)), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalContactRequestDownloadFile(const QString &, const QString &, const QString &)), m_chatWindowManager, SLOT(contactRequestDownloadFile(const QString &, const QString &, const QString &)), Qt::QueuedConnection);

    connect(clientPacketsParser, SIGNAL(signalFileDownloadRequestAccepted(const QString &, const QString &, const QByteArray &, quint64 )), m_chatWindowManager, SLOT(fileDownloadRequestAccepted(const QString &, const QString &, const QByteArray &, quint64 )), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalFileDownloadRequestDenied(const QString &, const QString &, const QString &)), m_chatWindowManager, SLOT(fileDownloadRequestDenied(const QString &, const QString &, const QString &)), Qt::QueuedConnection);
    connect(clientPacketsParser, SIGNAL(signalFileUploadRequestResponsed(const QString &, const QByteArray &, bool, const QString &)), m_chatWindowManager, SLOT(fileUploadRequestResponsed(const QString &, const QByteArray &, bool, const QString &)), Qt::QueuedConnection);


//    connect(clientPacketsParser, SIGNAL(signalFileDataRequested(int, const QString &, const QByteArray &, int, int )), this, SLOT(processFileDataRequestPacket(int, const QString &, const QByteArray &, int, int )), Qt::QueuedConnection);
//    connect(clientPacketsParser, SIGNAL(signalFileDataReceived(int, const QString &, const QByteArray &, int, const QByteArray &, const QByteArray &)), this, SLOT(processFileDataReceivedPacket(int, const QString &, const QByteArray &, int, const QByteArray &, const QByteArray &)), Qt::QueuedConnection);
//    connect(clientPacketsParser, SIGNAL(signalFileTXStatusChanged(int, const QString &, const QByteArray &,quint8)), this, SLOT(processFileTXStatusChangedPacket(int, const QString &, const QByteArray &, quint8)), Qt::QueuedConnection);
//    connect(clientPacketsParser, SIGNAL(signalFileTXError(int, const QString &, const QByteArray &, quint8 , const QString &)), this, SLOT(processFileTXErrorFromPeer(int, const QString &, const QByteArray &, quint8 , const QString &)), Qt::QueuedConnection);




    connect(m_chatWindowManager, SIGNAL(signalSendChatMessageToCantact(Contact *, const QString &, const QStringList &)), this, SLOT(slotSendChatMessageToContact(Contact *, const QString &, const QStringList &)), Qt::QueuedConnection);
    connect(m_chatWindowManager, SIGNAL(signalSendChatMessageToInterestGroup(InterestGroup*, const QString &, const QStringList &)), this, SLOT(slotSendChatMessageToInterestGroup(InterestGroup*, const QString &, const QStringList &)), Qt::QueuedConnection);
    connect(m_chatWindowManager, SIGNAL(signalRequestDownloadImage(const QString &, const QString &)), this, SLOT(requestDownloadImage(const QString &, const QString &)), Qt::QueuedConnection);
    connect(m_chatWindowManager, SIGNAL(signalRequestContactHistoryMessage(const QString &, const QString &, const QString &, bool, const QString &)), this, SLOT(getContactHistoryMessage(const QString &, const QString &, const QString &, bool, const QString &)), Qt::QueuedConnection);
    connect(m_chatWindowManager, SIGNAL(signalRequestGrouptHistoryMessage(const QString &, const QString &, const QString &, bool, quint32)), this, SLOT(getGrouptHistoryMessage(const QString &, const QString &, const QString &, bool, quint32)), Qt::QueuedConnection);

    //File TX
    connect(m_chatWindowManager, SIGNAL(signalRequestFileServerInfo()), this, SLOT(slotRequestFileServerInfo()), Qt::QueuedConnection);
    connect(m_chatWindowManager, SIGNAL(signalSendUploadingFileRequest(Contact *, const QString &, const QByteArray &)), this, SLOT(slotSendUploadingFileRequest(Contact *, const QString &, const QByteArray &)), Qt::QueuedConnection);
    connect(m_chatWindowManager, SIGNAL(signalCancelSendingFileUploadingRequest(Contact *, const QByteArray &)), this, SLOT(slotCancelSendingFileRequest(Contact *, const QByteArray &)), Qt::QueuedConnection);
    connect(m_chatWindowManager, SIGNAL(signalAcceptPeerUploadFileRequest(Contact *, const QByteArray &, const QString &)), this, SLOT(slotAcceptPeerUploadFileRequest(Contact *, const QByteArray &, const QString &)), Qt::QueuedConnection);
    connect(m_chatWindowManager, SIGNAL(signalDeclinePeerUploadFileRequest(Contact *, const QByteArray &)), this, SLOT(slotDeclinePeerUploadFileRequest(Contact *, const QByteArray &)), Qt::QueuedConnection);

    //Single Process Thread
    //QtConcurrent::run(clientPacketsParser, &ClientPacketsParser::run);
    //IMPORTANT For Multi-thread
    //    QThreadPool::globalInstance()->setMaxThreadCount(MIN_THREAD_COUNT);
    //    QtConcurrent::run(clientPacketsParser, &ClientPacketsParser::startparseIncomingPackets);
    //    QtConcurrent::run(clientPacketsParser, &ClientPacketsParser::startprocessOutgoingPackets);


    //TODO:
    //clientPacketsParser->sendClientLookForServerPacket(networkManager->localTCPListeningAddress(), networkManager->localTCPListeningPort(), networkManager->hostName(), getServerLastUsed());
    //QTimer::singleShot(60000, this, SLOT(checkHasAnyServerBeenFound()));


    networkStarted = true;


#if defined(Q_OS_WIN32)
    //                SetProcessWorkingSetSize(GetCurrentProcess(), 0xFFFFFFFF, 0xFFFFFFFF);
    //                SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);

    //    wm->freeMemory();
#endif

}

void MainWindow::stopNetwork(){

    destoryLoginTimer();

    m_serverHostAddress = QHostAddress::Null;
    m_serverHostPort = 0;


    if(clientPacketsParser){
        clientPacketsParser->logout(m_socketConnectedToServer);
        //QTimer::singleShot(1000, clientPacketsParser, SLOT(aboutToQuit()));
        Utilities::msleep(1000);
        delete clientPacketsParser;
        clientPacketsParser = 0;
    }

    if(m_udpServer){
        m_udpServer->close();
    }

    if(m_rtp){
        m_rtp->stopServers();
    }
    m_contactSocketsHash.clear();


    //    ClientResourcesManager::cleanInstance();
    delete m_resourcesManager;
    m_resourcesManager = 0;

    PacketHandlerBase::clean();

    m_verified = false;


}


void MainWindow::slotInitPlugin(AbstractPluginInterface *plugin){
    qDebug("----MainWindow::slotInitPlugin(AbstractPluginInterface *plugin)");

    if(!plugin){
        return;
    }

    GUIInterface *guiPlugin = static_cast<GUIInterface *> (plugin);
    if(guiPlugin){
        guiPlugin->initialize(this, pluginsMenu, 0, systemTray, APP_NAME, APP_VERSION);
        pluginsMenu->addMenu(guiPlugin->menu());

    }

    //    int pluginsCount = guiPluginsList().size();
    //    if(!pluginsCount){
    //            return;
    //    }
    //
    //
    //
    //    for(int i = 0; i< pluginsCount; i++){
    //            GUIInterface *plugin = guiPluginsList().at(i);
    //            plugin->initialize(this, ui.menuPlugins, 0, systemTray, APP_NAME, APP_VERSION);
    //    }

}




//void MainWindow::loadTranslations() {
//	//动态语言切换,在 QWidget::languageChange() 事件中重新翻译UI
//	//Dynamic language switch, retranslate the UI when QWidget::languageChange() event emitted.

//	qDebug() <<"----MainWindow::loadTranslations()~~Locale Language:"<< QLocale::system().name();

//	//全局变量,在此初始化
//	//Global Variables, init here
//	qmLocale = Settings::instance()->getLanguage();
//	qmPath = QApplication::applicationDirPath() + QDir::separator () + QString(LANGUAGE_FILE_DIR);
//	QDir dir;

//	if (dir.exists(qmPath)) {

//		qDebug() << "----MainWindow::loadTranslations()~~Switch Langague to: " << qmLocale;
//		qApp->installTranslator(&appTranslator);
//		qApp->installTranslator(&qtTranslator);

//		appTranslator.load(LANGUAGE_FILE_PREFIX + qmLocale, qmPath);
//		qtTranslator.load(QString("qt_") + qmLocale, qmPath);


//	} else {

//		dir.mkdir(LANGUAGE_FILE_DIR);
//	}

//	//更新菜单
//	//Update the menus
//	setupMenus();

//}

//void MainWindow::setupMenus() {

//    qDebug()<<"----MainWindow::setupMenus()";


//    QStringList translationLanguages = Utilities::availableTranslationLanguages(qmPath);
//    //如果没有找到语言文件或者首选语言是英语,则将"en_US"置为选中
//    //If language files found or the preferred language is English, set the "en_US" menu checked
//    if (translationLanguages.isEmpty()) {
//            ui.actionLanguageDefaultEnglish->setChecked(true);
//            return;
//    }

//    if (!translationLanguages.contains(qmLocale)){
//            qmLocale = "en_US";
//    }

//    if(qmLocale == "en_US"){
//        ui.actionLanguageDefaultEnglish->setChecked(true);
//    }

//    //为每一个语言文件生成动作为菜单
//    //Make action and menu for each language file
//    for (int i = 0; i < translationLanguages.size(); i++){
//        QString translationLanguage = translationLanguages[i];
//            QAction *action = new QAction(tr("&%1 %2") .arg(i + 1).arg(translationLanguage), this);
//            action->setCheckable(true);
//            action->setData(translationLanguage);

//            ui.menuLanguage->addAction(action);
//            languageActionGroup->addAction(action);

//            if (qmLocale == translationLanguage) {
//                    action->setChecked(true);
//                    action->trigger();
//            }

//    }

//        //更新样式菜单
//        //update Style Menu
//        foreach(QAction *action, ui.menuStyle->actions()){
//                if(action->data().toString() == Settings::instance()->getStyle()){
//                        action->setChecked(true);
//                }
//        }
//        ui.actionUseStylesPalette->setChecked(Settings::instance()->getPalette());


//}


/*

void MainWindow::restoreWindowState() {
 restoreState( Settings::instance()->value( "MainWindow/State" ).toByteArray() );
 QPoint p = Settings::instance()->value( "MainWindow/Position" ).toPoint();
 QSize s = Settings::instance()->value( "MainWindow/Size" ).toSize();
 if ( !p.isNull() && !s.isNull() )
 {
  resize( s );
  move( p );
 }

 if ( Settings::instance()->value( "MainWindow/Maximized", false ).toBool() )
  showMaximized();

 if ( Settings::instance()->value( "MainWindow/Hidden", false ).toBool() )
   hide();

}

void MainWindow::saveWindowState() {

 Settings::instance()->setValue( "MainWindow/Position", pos() );
 Settings::instance()->setValue( "MainWindow/Size", size() );
 Settings::instance()->setValue( "MainWindow/State", saveState() );
 Settings::instance()->setValue( "MainWindow/Hidden", isHidden() );

}

*/



void MainWindow::setupSystemTray() {
    
    
    trayMenu = new QMenu(this);
    trayMenu->addMenu(pluginsMenu);
    trayMenu->addSeparator();
    trayMenu->addAction(ui.actionShowMaximized);
    trayMenu->addAction(ui.actionShowMinimized);
    trayMenu->addAction(ui.actionShowFullScreen);
    trayMenu->addAction(ui.actionShowNormal);
    trayMenu->addSeparator();
    trayMenu->addAction(ui.actionQuit);
    
    
    QIcon defTrayIcon = 	ImageResource::createIcon((QString(RESOURCE_PATH)+QString(APP_ICON_PATH)), "", QIcon::Disabled);
    
    TrayIconData data(STIDT_Unknown, "DefaultSystemTrayIconData", m_myUserID);
    data.setToolTip(APP_NAME);
    data.setMenu(trayMenu);
    data.setFirstIcon(defTrayIcon);
    
    systemTray = new SystemTrayIconBase(data, this);
    
    
    //        systemTray = new SystemTrayIconBase(defTrayIcon, this);
    //systemTray->setIcon(QIcon(QString(RESOURCE_PATH)+QString(APP_ICON_PATH)));
    //systemTray->setIcon(ImageResource::createIconSet((QString(RESOURCE_PATH)+QString(APP_ICON_PATH)), "", QIcon::Disabled));
    //systemTray->setToolTip(APP_NAME);
    connect(systemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(slotIconActivated(QSystemTrayIcon::ActivationReason)));
    
    
    
    
    //systemTray->setContextMenu(trayMenu);
    
    
    
    systemTray->show();





}

QSystemTrayIcon* MainWindow::SystemTrayIcon() {
    if (!systemTray) {
        setupSystemTray();
    }

    return systemTray;
}

void MainWindow::setTrayIconVisible(bool visible) {
    visible = true;
}

void MainWindow::slotIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    
    TrayIconData data = systemTray->currentTrayIconData();
    int dataTypeCode = data.getDataType();
    SystemTrayIconDataType dataType = STIDT_Unknown;
    if(dataTypeCode > 0){
        dataType = SystemTrayIconDataType(dataTypeCode);
    }
    
    switch(dataType){
    case STIDT_Unknown:
    {
        qWarning()<<"Unknown SystemTrayIconDataType: "<<dataTypeCode;
        //if(reason == QSystemTrayIcon::DoubleClick){
        if(reason != QSystemTrayIcon::Context){
            this->showNormal();
            this->activateWindow();
            this->raise();
        }
    }
        break;
    case STIDT_SystemMessage:
    {

    }
        break;
    case STIDT_ServerAnnouncement:
    {

    }
        break;
    case STIDT_FriendshipApplicationResult:
    {
        if(reason == QSystemTrayIcon::DoubleClick){
            systemTray->removeTrayIconData(data.getID());

            QString contactID = data.getData().toString();
            getNewContactSettings(contactID);
        }

    }
        break;
    case STIDT_FriendshipApplicationFromContact:
    {
        if(reason == QSystemTrayIcon::DoubleClick){
            systemTray->removeTrayIconData(data.getID());

            QStringList list = data.getData().toString().split(QChar(PACKET_DATA_SEPARTOR));
            if(list.size() == 4){
                showContactRequestFromUser(list.at(0), list.at(1), list.at(2), list.at(3));
            }

        }

    }
        break;
    case STIDT_ContactChatMessage:
    {
        QString contactID = data.getID();
//        systemTray->removeTrayIconData(contactID);
        m_contactBox->chatMessageFromContactRead(m_contactsManager->getUser(contactID));

        requestChatWithContact(m_contactsManager->getUser(contactID));

//        QHash<QString/*Time*/, QVariant/*Message*/> hashData = data.getData().toHash();
//        QStringList times = hashData.keys();
//        times.sort();
//        foreach(QString time, times){
//            //qDebug()<<"contactID:"<<contactID<<" Time:"<<time<<" Msg:"<<hashData.value(time).toString();
//            chatWindowManager->slotNewMessageReceivedFromContact(contactID, hashData.value(time).toString(), time);
//        }


    }
        break;

    case STIDT_InterestGroupChatMessage:
    {
        quint32 interestGroupID = data.getID().toUInt();
        systemTray->removeTrayIconData(data.getID());

        //QHash<QString/*Time*/, QStringList/*Contact ID, Message*/ > hashData = data.getData().toHash();
        QHash<QString/*Time*/, QVariant/*Contact ID, Message*/ > hashData = data.getData().toHash();

        QStringList times = hashData.keys();
        times.sort();
        foreach(QString time, times){
            QStringList list = hashData.value(time).toStringList();
            m_chatWindowManager->slotNewMessageReceivedFromInterestGroup(interestGroupID, list.at(0), list.at(1), time);
        }

    }
        break;

    case STIDT_InterestGroupMemberJoinedOrQuitted:
    {
        systemTray->removeTrayIconData(data.getID());

        QStringList list = data.getData().toStringList();
        quint32 groupID = list.at(0).toUInt();
        QString memberID = list.at(1);
        quint8 join = list.at(2).toUInt();

        InterestGroup *group = m_contactsManager->getInterestGroup(groupID);
        if(!group){return;}
        QString groupName = group->getGroupName();
        if(groupName.trimmed().isEmpty()){
            groupName = list.at(0);
        }else{
            groupName = groupName + "(" + list.at(0) +")";
        }
        QMessageBox::information(0, tr("System Message"), tr("%1 have %2 the group %3!").arg( (m_myUserID==memberID)?tr("You"):memberID ).arg(join?tr("joined"):tr("quitted")).arg(groupName));

    }
        break;

    default:
        qWarning()<<"Unknown SystemTrayIconDataType: "<<dataTypeCode;

    }
    

}



//void MainWindow::slotChangeStyle(QAction * action) {
//	//更新样式
//	//Update the app style
//	preferedStyle = action->data().toString();
//	//QApplication::setStyle(preferedStyle);
//	changeStyle(preferedStyle);
//	//slotChangePalette(ui.actionUseStylesPalette->isChecked());
//	if (!action->isChecked()) {
//		action->setChecked(true);
//	}


//	Settings::instance()->setStyle(action->data().toString());
//}

//void MainWindow::changeStyle(const QString &style) {

//	QApplication::setStyle(style);

//	slotChangePalette(Settings::instance()->getPalette());
//	//保存首选样式
//	//Save the preferred style
//	Settings::instance()->setStyle(style);

//}

//void MainWindow::slotChangePalette(bool useStylePalette) {
//	if (useStylePalette) {
//		QApplication::setPalette(QApplication::style()->standardPalette());
//	} else {
//		QApplication::setPalette(originalPalette);
//	}

//	Settings::instance()->setPalette(useStylePalette);
//}

//void MainWindow::slotChangeLanguage(QAction *action) {

//	QString lang = action->data().toString();
//	if (lang.isEmpty()){
//		lang = "en_US";
//	}

//	QString langFile = QString(LANGUAGE_FILE_PREFIX) + lang + QString(".qm");
//	appTranslator.load(langFile, qmPath);
//	qDebug()<<"----MainWindow::slotChangeLanguage(...)~~Loading language file :"<<langFile ;
//	langFile = "qt_" + lang + QString(".qm");
//	qtTranslator.load(langFile, qmPath);
//	qDebug()<<"----MainWindow::slotChangeLanguage(...)~~Loading language file :"<<langFile ;

//	if (!action->isChecked()) {
//		action->setChecked(true);
//	}


//	preferedLanguage = lang;
//	Settings::instance()->setLanguage(lang);

//}



/*
void MainWindow::slotSystemInfo() {
 //如果窗口已打开,
 //If the window has already been opened, activate it
 //if(SystemInfo::isRunning() == true){
 //     foreach (QMdiSubWindow *window, ui.mdiArea->subWindowList()) {
 //    	 SystemInfo *systemInfo = qobject_cast<SystemInfo *>(window->widget());
 //         if (systemInfo)
 //        	 ui.mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
 //             return ;
 //     }

 //	return;
 //}

 //SystemInfo *systemInfo = new SystemInfo(this);
 //ui.mdiArea->addSubWindow(systemInfo, Qt::Dialog);
 //systemInfo->show();

 QMessageBox::information(this,tr("~_~"),tr(" Not accomplished !"));
 qDebug() << "slotSystemInfo";


}




void MainWindow::slotUserManager(){
 //如果窗口已打开,
 //If the window has already been opened, activate it
 //if(UserManagerMainWindow::isRunning() == true){

 //     foreach (QMdiSubWindow *window, ui.mdiArea->subWindowList()) {
 //    	 UserManagerMainWindow *userManager = qobject_cast<UserManagerMainWindow *>(window->widget());
 //         if (userManager)
 //        	 ui.mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
 //             return ;
 //     }

 //	return;
 //}



 User user;
 LoginBase login(&user);
 if (!login.isVerified()) {
  return ;
 }


 //UserManagerMainWindow *userManager = new UserManagerMainWindow(this);
 //ui.mdiArea->addSubWindow(userManager, Qt::Dialog);
 //userManager->show();

 QMessageBox::information(this,tr("~_~"),tr(" Not accomplished !"));
 qDebug() << "slotUserManager";

}

void MainWindow::slotSqlExplorer(){
 //如果窗口已打开,
 //If the window has already been opened, activate it
 //if(SqlExplorer::isRunning() == true){
 //     foreach (QMdiSubWindow *window, ui.mdiArea->subWindowList()) {
 //    	 SqlExplorer *sqlExplorer = qobject_cast<SqlExplorer *>(window->widget());
 //         if (sqlExplorer)
 //        	 ui.mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
 //             return ;
 //     }
//
 //	return;
 //}


 User user;
 LoginBase login(&user);
 if (!login.isVerified()) {
  return ;
 }

 //SqlExplorer *sqlExplorer = new SqlExplorer(this);
 //ui.mdiArea->addSubWindow(sqlExplorer, Qt::Dialog);
 //sqlExplorer->show();
 QMessageBox::information(this,tr("~_~"),tr(" Not accomplished !"));
 qDebug() << "slotSqlExplorer";

}
*/



void MainWindow::slotSystemConfig() {
    QMessageBox::information(this,tr("~_~"),tr(" Not accomplished !"));
    qDebug() << "----MainWindow::slotSystemConfig()";
}

void MainWindow::slotBugReport() {
    QMessageBox::information(this,tr("~_~"),tr(" Not accomplished !"));
    qDebug() << "----MainWindow::MainWindow::slotSystemConfig()()";
}

void MainWindow::slotHelp() {
    QMessageBox::information(this,tr("~_~"),tr(" Not accomplished !"));
    qDebug() << "----MainWindow::slotHelp()";

}

void MainWindow::slotAbout() {
    AboutDialog aboutDlg(this);
    aboutDlg.exec();

}

void MainWindow::slotQuit() {
    qDebug() << "----MainWindow::slotQuit()";

    aboutToQuit();

    QTimer::singleShot(1000, qApp, SLOT(quit()));
    //qApp->quit();

}

void MainWindow::retranslateUi() {

    //重新翻译UI
    //Retranslate UI
    ui.retranslateUi(this);


}


void MainWindow::aboutToQuit(){
    qDebug() << "----MainWindow::aboutToQuit()";


    if(m_myself->getOnlineState() != IM::ONLINESTATE_OFFLINE && m_myself->getOnlineState() != IM::ONLINESTATE_INVISIBLE){
        m_myself->setOnlineState(IM::ONLINESTATE_OFFLINE);
        emit signalMyOnlineStateChanged(m_socketConnectedToServer, quint8(IM::ONLINESTATE_OFFLINE));
        qDebug() << "----MainWindow::doWorkbeforeQuit()~~IMUser::OFFLINE";
    }

    m_myself->saveMyInfoToLocalDatabase();

    if(Settings::instance()->getRestoreWindowStateOnStartup()){
        Settings::instance()->saveState(this);
    }



    //关闭所有的数据库连接
    //Close all database connections
    DatabaseUtility::closeAllDBConnections();

    systemTray->hide();

    stopNetwork();

    //TODO:清理


}

void MainWindow::savePreferedStyle(const QString &preferedStyle, bool useStylePalette){
    Settings::instance()->setStyle(preferedStyle);
    Settings::instance()->setPalette(useStylePalette);

}

void MainWindow::savePreferedLanguage(const QString &preferedLanguage){
    Settings::instance()->setLanguage(preferedLanguage);

}

void MainWindow::showDeleteContactDialog(Contact *contact, bool blacklistMode){

    if(!m_deleteContactDialog){
        m_deleteContactDialog = new DeleteContactDialog(this);
        connect(m_deleteContactDialog, SIGNAL(signalRequestDeleteContact(const QString &, bool, bool)), this, SLOT(slotRequestDeleteContact(const QString &, bool, bool)));
        connect(clientPacketsParser, SIGNAL(signalDeleteContactResultPacketReceived(const QString &, bool)), m_deleteContactDialog, SLOT(deleteContactResultReceivedFromServer(const QString &, bool)));
    }

    m_deleteContactDialog->setContact(contact, blacklistMode);
    m_deleteContactDialog->show();

}

void MainWindow::slotUpdateContactsInfo2(){
    qDebug()<<"----MainWindow::slotUpdateContactsInfo()";



//    m_contactsManager->slotFetchAllContactsInfo(friendBox);

    ui.contactsToolBox->setEnabled(true);
    ui.stackedWidget->setCurrentWidget(ui.mainPage);

    setWindowTitle(m_myUserID);
    //ui.toolButtonUserFace->setIcon(ImageResource::createIconForContact(imUser->getFace(), true));
    //setWindowIcon(HEHUI::ImageResourceBase::createIconSet(IMUser::instance()->getHeadPortrait()));
    //setWindowIcon(HEHUI::ImageResourceBase::createIconSet(":/resources/images/locked.png"));


}

//void MainWindow::slotUpdateBlacklistInfo(){
//    ui.listWidgetBlacklist->clear();

//    QStringList blacklist = imUser->getBlacklist();
//    foreach (QString contactID, blacklist) {
//        if(contactID.trimmed().isEmpty()){continue;}
//        Contact *contact = 0;
//        QString face = "", nickName = contactID;
//        if(m_contactsManager->hasUserInfo(contactID)){
//            contact = m_contactsManager->getUser(contactID);
//            face = contact->getFace();
//            nickName = contact->getNickName();
//        }

//        QListWidgetItem *item = new QListWidgetItem(ImageResource::createIconForContact(face), nickName, ui.listWidgetBlacklist);
//        item->setData(Qt::UserRole, contactID);
//        ui.listWidgetBlacklist->addItem(item);
//    }


//}

void MainWindow::updateAllInterestGroupsInfoToUI(){
    
    ui.listWidgetGroups->clear();
    
    QList<InterestGroup *> interestGroups = m_contactsManager->getInterestGroupsList();
//    QIcon ico = ImageResource::createIconForInterestGroup();
    foreach (InterestGroup *group, interestGroups) {
//        QListWidgetItem *item = new QListWidgetItem(ico, group->getGroupName(), ui.listWidgetGroups);
//        item->setData(Qt::UserRole, group->getGroupID());
//        ui.listWidgetGroups->addItem(item);
        addInterestGroupToUI(group);

    }
    
    
}

void MainWindow::addInterestGroupToUI(InterestGroup *interestGroup){

    if(!interestGroup){return;}

    QIcon ico = ImageResource::createIconForInterestGroup(interestGroup->getState());
    QListWidgetItem *item = new QListWidgetItem(ico, interestGroup->getGroupName(), ui.listWidgetGroups);
    item->setData(Qt::UserRole, interestGroup->getGroupID());
    ui.listWidgetGroups->addItem(item);

}

void MainWindow::deleteInterestGroupFromUI(InterestGroup *interestGroup){

    if(!interestGroup){return;}

    quint32 groupID = interestGroup->getGroupID();
    for(int i=0; i<ui.listWidgetGroups->count(); i++){
        QListWidgetItem *item = ui.listWidgetGroups->item(i);
        if(!item){continue;}
        if(item->data(Qt::UserRole).toUInt() == groupID){
            ui.listWidgetGroups->removeItemWidget(item);
            return;
        }
    }

}

void MainWindow::updateInterestGroupInfoToUI(InterestGroup *interestGroup){

    if(!interestGroup){return;}

    quint32 groupID = interestGroup->getGroupID();
    for(int i=0; i<ui.listWidgetGroups->count(); i++){
        QListWidgetItem *item = ui.listWidgetGroups->item(i);
        if(!item){continue;}
        if(item->data(Qt::UserRole).toUInt() == groupID){
            item->setText(interestGroup->getGroupName());

            QIcon ico = ImageResource::createIconForInterestGroup(interestGroup->getState());
            item->setIcon(ico);
            return;
        }
    }

    qCritical()<<"ERROR! Can not find interest group item!";
    
}

void MainWindow::setupRecentChats(const QStringList &contacts, const QList<quint32> &interestGroups){
    qDebug()<<"--MainWindow::setupRecentChats(...) "<<" contacts:"<<contacts;

    if(!contacts.isEmpty()){
        foreach (QString contactID, contacts) {
            Contact *c = m_contactsManager->getUser(contactID);
            if(!c){continue;}

            addOrRemoveRecentChatContact(c, true);
        }
    }

    if(!interestGroups.isEmpty()){
        foreach (quint32 groupID, interestGroups) {
            InterestGroup *group = m_contactsManager->getInterestGroup(groupID);
            if(!group){continue;}

            addOrRemoveRecentChatGroup(group, true);
        }
    }

}

void MainWindow::addOrRemoveRecentChatContact(Contact *contact, bool add){

    if(!contact){
        return;
    }

    QString contactID = contact->getUserID();

    QTreeWidgetItem *existedFriendItem = 0;
    for(int i=0; i<m_recentChatFriendsItem->childCount(); i++){
        QTreeWidgetItem *item = m_recentChatFriendsItem->child(i);
        if(!item){continue;}
        if(item->data(0, Qt::UserRole).toString() == contactID){
            existedFriendItem = item;
            break;
        }
    }

    QTreeWidgetItem *existedStrangerItem = 0;
    for(int i=0; i<m_recentChatStrangersItem->childCount(); i++){
        QTreeWidgetItem *item = m_recentChatStrangersItem->child(i);
        if(!item){continue;}
        if(item->data(0, Qt::UserRole).toString() == contactID){
            existedStrangerItem = item;
            break;
        }
    }

    if(add){
        QTreeWidgetItem *topItem = 0;

        if(contact->isFriend()){
            if(existedFriendItem){
                m_recentChatFriendsItem->removeChild(existedFriendItem);
                m_recentChatFriendsItem->insertChild(0, existedFriendItem);
                return;
            }else if(existedStrangerItem){
                m_recentChatStrangersItem->removeChild(existedStrangerItem);
                m_recentChatFriendsItem->insertChild(0, existedStrangerItem);
                return;
            }

            topItem = m_recentChatFriendsItem;

        }else{
            if(existedStrangerItem){
                m_recentChatStrangersItem->removeChild(existedFriendItem);
                m_recentChatStrangersItem->insertChild(0, existedStrangerItem);
                return;
            }else if(existedFriendItem){
                m_recentChatFriendsItem->removeChild(existedFriendItem);
                m_recentChatStrangersItem->addChild(existedFriendItem);
            }

            topItem = m_recentChatStrangersItem;

        }

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, contact->displayName());
        item->setData(0, Qt::UserRole, QVariant(contactID));

        QIcon ico = ImageResource::createIconForContact(contact->getFace(), contact->getOnlineState());
        item->setIcon(0, ico);

        topItem->insertChild(0, item);

    }else{
        if(existedFriendItem){
            m_recentChatFriendsItem->removeChild(existedFriendItem);
            delete existedFriendItem;
        }

        if(existedStrangerItem){
            m_recentChatStrangersItem->removeChild(existedStrangerItem);
            delete existedStrangerItem;
        }

    }


}

void MainWindow::addOrRemoveRecentChatGroup(InterestGroup *interestGroup, bool add){

    if(!interestGroup){
        return;
    }

    quint32 groupID = interestGroup->getGroupID();

    QTreeWidgetItem *existedItem = 0;
    for(int i=0; i<m_recentChatGroupsItem->childCount(); i++){
        QTreeWidgetItem *item = m_recentChatGroupsItem->child(i);
        if(!item){continue;}
        if(item->data(0, Qt::UserRole).toUInt() == groupID){
            existedItem = item;
            break;
        }
    }

    if(add){
        if(existedItem){
            m_recentChatGroupsItem->removeChild(existedItem);
            m_recentChatGroupsItem->insertChild(0, existedItem);
            return;
        }

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, interestGroup->getGroupName());
        item->setData(0, Qt::UserRole, QVariant(groupID));

        QIcon ico = ImageResource::createIconForInterestGroup(interestGroup->getState());
        item->setIcon(0, ico);

        m_recentChatGroupsItem->insertChild(0, item);
    }else{
        if(existedItem){
            m_recentChatGroupsItem->removeChild(existedItem);
            delete existedItem;
        }

    }


}

void MainWindow::getContactHistoryMessage(const QString &startTime, const QString &endTime, const QString &content, bool requestBackword, const QString &contactID){

    QStringList messages;
    bool canFetchMore = false;
    m_contactsManager->getContactHistoryMessage(startTime, endTime, content, requestBackword, contactID, &messages, &canFetchMore);
    m_chatWindowManager->processContactHistoryMessage(messages, canFetchMore, contactID);

}

void MainWindow::getGrouptHistoryMessage(const QString &startTime, const QString &endTime, const QString &content, bool requestBackword, quint32 groupID){

    QStringList messages;
    bool canFetchMore = false;
    m_contactsManager->getGrouptHistoryMessage(startTime, endTime, content, requestBackword, groupID, &messages, &canFetchMore);
    m_chatWindowManager->processGrouptHistoryMessage(messages, canFetchMore, groupID);
}

void MainWindow::slotRequestFileServerInfo(){
    qDebug()<<"--MainWindow::slotRequestFileServerInfo()";
    clientPacketsParser->requestFileServerInfo(m_socketConnectedToServer);
}

void MainWindow::slotSendUploadingFileRequest(Contact *contact, const QString &filePath, const QByteArray &fileMD5){
    QFileInfo info(filePath);
    clientPacketsParser->requestUploadFile(contact, fileMD5, info.fileName(), info.size() );
}

void MainWindow::slotCancelSendingFileRequest(Contact *contact, const QByteArray &fileMD5){
    clientPacketsParser->cancelUploadFileRequest(contact, fileMD5);
}

void MainWindow::slotAcceptPeerUploadFileRequest(Contact *contact, const QByteArray &fileMD5, const QString &localSavePath){
    clientPacketsParser->responseFileUploadRequest(contact, fileMD5, true, "");
}

void MainWindow::slotDeclinePeerUploadFileRequest(Contact *contact, const QByteArray &fileMD5){
    clientPacketsParser->responseFileUploadRequest(contact, fileMD5, false, "");
}


void MainWindow::slotUserVerified(){
    qDebug()<<"--MainWindow::slotUserVerified()";

    if(ui.loginPage->getState() == LoginWidget::VERIFYING){
        m_verified = true;

        m_myUserID = m_myself->getUserID();
        Settings::instance()->setCurrentUser(m_myUserID);


//        slotUpdateContactsInfo();
        

        m_myself->setOnlineState(m_myself->getStateAfterLoggedin());
        //systemTray->resetTrayIcon(ImageResource::createIcon((QString(RESOURCE_PATH)+QString(APP_ICON_PATH)), "", QIcon::Normal));
        systemTray->resetTrayIcon(ImageResource::createIconForContact((QString(RESOURCE_PATH)+QString(APP_ICON_PATH)), m_myself->getOnlineState()));

        systemTray->resetToolTip(m_myUserID);
        //systemTray->setIcon(ImageResource::createIcon((QString(RESOURCE_PATH)+QString(APP_ICON_PATH)), "", QIcon::Normal));
        //systemTray->setToolTip(imUser->getUserID());
        
        ui.toolButtonUserFace->setIcon(ImageResource::createIconForContact(m_myself->getFace(), IM::ONLINESTATE_ONLINE));
        //ui.toolButtonUserFace->setIcon(ImageResource::createMixedIcon((QString(RESOURCE_PATH)+QString(APP_ICON_PATH)), imUser->getOnlineState()));

        ui.labelUserNickName->setText(m_myself->getNickName());
        ui.labelUserID->setText(m_myUserID);

        
        m_contactsManager->loadInterestGroups();
        updateAllInterestGroupsInfoToUI();    
        
    }else{
        //解锁时恢复在线状态
        //Restore user online state after unlocking
//        ui.stackedWidget->setCurrentWidget(ui.mainPage);
        m_myself->setOnlineState(stateBeforeLocking);
        
        //TODO:是否要放在外面
        if(m_myself->getOnlineState() != IM::ONLINESTATE_INVISIBLE){
            //systemTray->resetTrayIcon(ImageResource::createIcon((QString(RESOURCE_PATH)+QString(APP_ICON_PATH)), "", QIcon::Normal));
            systemTray->resetTrayIcon(ImageResource::createIconForContact((QString(RESOURCE_PATH)+QString(APP_ICON_PATH)), m_myself->getOnlineState()));

            emit signalMyOnlineStateChanged(m_socketConnectedToServer, quint8(m_myself->getOnlineState()));
        }
    }

    ui.stackedWidget->setCurrentWidget(ui.mainPage);

    ui.loginPage->switchUI(LoginWidget::NORMAL);
    

}


void MainWindow::slotLockUI(){
    ui.loginPage->lockUI();
    ui.stackedWidget->setCurrentWidget(ui.loginPage);

    stateBeforeLocking = m_myself->getOnlineState();
    if(stateBeforeLocking != IM::ONLINESTATE_INVISIBLE && stateBeforeLocking != IM::ONLINESTATE_AWAY){
        m_myself->setOnlineState(IM::ONLINESTATE_AWAY);
        emit signalMyOnlineStateChanged(m_socketConnectedToServer, quint8(IM::ONLINESTATE_AWAY));
    }

}

void MainWindow::handleContextMenuEventOnContactGroup(ContactGroupBase *contactGroup, const QPoint &global_mouse_pos){


    if(!contactGroup){return;}
    int groupID = contactGroup->getGroupID();


    m_userInfoTipWindow->hideUserInfoTip();

    QMenu contextMenu;
    contextMenu.addAction(tr("Expand all"), m_contactBox, SLOT(expandAll()));
    contextMenu.addAction(tr("Collapse all"), m_contactBox, SLOT(collapseAll()));
    contextMenu.addSeparator();

        QAction actionRenameGroupName(tr("Rename Group"), &contextMenu);
        QAction actionDeleteGroup(tr("Delete Group"), &contextMenu);
        QAction actionCreateNewGroup(tr("Create New Group"), &contextMenu);

        if(ContactGroupBase::isUserCreatedGroup(groupID)){
            contextMenu.addAction(&actionRenameGroupName);
            if(m_myself->countOfContactGroupMembers(groupID) == 0){
                contextMenu.addAction(&actionDeleteGroup);
            }
        }

        contextMenu.addAction(&actionCreateNewGroup);

        QAction *action = contextMenu.exec(global_mouse_pos);
        if(action == &actionRenameGroupName){
            bool ok = false;
            QString labelText = tr("New Name:\n(Only word-characters up to 16 are acceptable!)");
            QString newGroupName = QInputDialog::getText(this, tr("Rename Group"),
                                                         labelText, QLineEdit::Normal,
                                                         contactGroup->getGroupName(), &ok);
            if (ok && !newGroupName.isEmpty()){
                int pos = 0;
                QRegExpValidator rxValidator(this);
                QRegExp rx("\\b\\w{0,16}\\b");
                rxValidator.setRegExp(rx);
                if(rxValidator.validate(newGroupName, pos) != QValidator::Acceptable){
                    QMessageBox::critical(this, tr("Error"), tr("Invalid Group Name!"));
                    return ;
                }

                if(m_myself->hasContactGroup(newGroupName)){
                    QMessageBox::critical(this, tr("Error"), tr("Group with the same name already exists!"));
                    return;
                }

//                showProgressDialog();

                clientPacketsParser->renameContactGroup(m_socketConnectedToServer, groupID, newGroupName);

                m_contactsManager->renameContactGroupToDatabase(groupID, newGroupName);
                m_myself->renameContactGroup(groupID, newGroupName);

                m_contactBox->updateContactGroupItemInfo(contactGroup);

            }


        }else if(action == &actionDeleteGroup){
            QString groupName = m_myself->getContactGroupName(groupID);
            int ret = QMessageBox::question(this, tr("Delete Contact Group"), tr("Are you sure you want to delete the the group '%1' ?").arg(groupName), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
            if(ret == QMessageBox::No){return;}
            showProgressDialog();
            clientPacketsParser->createOrDeleteContactGroup(m_socketConnectedToServer, groupID, "", false);
//            m_contactsManager->deleteGroupFromDatabase(groupIDString);
//            m_contactsManager->slotDeleteContactGroupFromUI(friendBox, groupIDString);


        }else if(action == &actionCreateNewGroup){

            bool ok = false;
            QString labelText = tr("Group Name:\n(Only word-characters up to 16 are acceptable!)");
            QString newGroupName = QInputDialog::getText(this, tr("Create New Group"),
                                                         labelText, QLineEdit::Normal,
                                                         "", &ok);
            if (ok && !newGroupName.isEmpty()){
                int pos = 0;
                QRegExpValidator rxValidator(this);
                QRegExp rx("\\b\\w{0,16}\\b");
                rxValidator.setRegExp(rx);
                if(rxValidator.validate(newGroupName, pos) != QValidator::Acceptable){
                    QMessageBox::critical(this, tr("Error"), tr("Invalid Group Name!"));
                    return ;
                }
                //TODO:
                m_myself->saveMyInfoToLocalDatabase();
                if(m_myself->hasContactGroup(newGroupName)){
                    QMessageBox::critical(this, tr("Error"), tr("Group already exists!"));
                    return;
                }

                showProgressDialog();

//                int newGroupID = m_contactsManager->slotAddNewContactGroupToDatabase(0, newGroupName);
                clientPacketsParser->createOrDeleteContactGroup(m_socketConnectedToServer, 0, newGroupName, true);
//                m_contactsManager->slotAddNewContactGroupToUI(friendBox, newGroupID, newGroupName);

            }

        }



}

void MainWindow::handleContextMenuEventOnContact(Contact *contact, const QPoint &global_mouse_pos){


    m_userInfoTipWindow->hideUserInfoTip();

    if(!contact){
        QMessageBox::critical(this, tr("Error"), tr("Contact does not exist!"));
        return;
    }
    QString contactID = contact->getUserID();

    QMenu menu;

    int oldGroupID = contact->getContactGroupID();
    QList<ContactGroupBase *> groups = m_myself->getContactGroups();
    groups.removeAll(m_myself->getContactGroup(oldGroupID));

    if(!groups.isEmpty() && contact->isFriend()){
        QMenu *menuMoveContactToGroup = menu.addMenu(tr("Move To"));
        foreach (ContactGroupBase *group, groups) {
            QAction *action = menuMoveContactToGroup->addAction(group->getGroupName());
            action->setData(contactID);
            connect(action, SIGNAL(triggered()), this, SLOT(slotMoveContactToGroup()));
        }
        menuMoveContactToGroup->addSeparator();
    }
//    QAction actionMoveToBlacklist(tr("Blacklist"), menuMoveContactToGroup);
//    actionMoveToBlacklist.setData(contactID);
//    connect(&actionMoveToBlacklist, SIGNAL(triggered()), this, SLOT(slotMoveContactToBlacklist()));
//    menuMoveContactToGroup->addAction(&actionMoveToBlacklist);


    QAction actionAddAsContact(tr("Add to Contact List"), &menu);
    if(!contact->isFriend()){
        menu.addAction(&actionAddAsContact);
    }

    QAction actionDeleteContact(tr("Delete Contact"), &menu);
    menu.addAction(&actionDeleteContact);

    QAction actionBlockContact(tr("Block Contact"), &menu);
    menu.addAction(&actionBlockContact);


    QAction *executedAction = menu.exec(global_mouse_pos);
    if(executedAction == &actionAddAsContact){
        if(contact->isStranger()){
            AddContactDialog dlg(contact, true, this);
            if(dlg.exec() != QDialog::Accepted){return;}
            addContact(contactID, dlg.getMessage());

        }else if(contact->isBlacklisted()){
            AddContactDialog dlg(contact, true, this);
            if(dlg.exec() != QDialog::Accepted){return;}
            slotRequestDeleteContact(contactID);
            addContact(contactID, dlg.getMessage());
        }

    }else if(executedAction == &actionDeleteContact){
        if(oldGroupID == ContactGroupBase::Group_Strangers_ID){
            //TODO:Close chat window
            if(!m_chatWindowManager->closeContactChatWindow(contact)){
                return;
            }
            m_contactBox->addOrRemoveContactItem(contact, false);
            m_contactsManager->slotdeleteContactFromDatabase(contact);

        }else if(oldGroupID == ContactGroupBase::Group_Blacklist_ID){
            slotRequestDeleteContact(contactID);
            showProgressDialog();
        }else{
            showDeleteContactDialog(contact, false);
        }

    }else if(executedAction == &actionBlockContact){
        showDeleteContactDialog(contact, true);
    }



}

void MainWindow::requestChatWithContact(Contact *contact){

    QString contactID = contact->getUserID();
    m_chatWindowManager->slotNewChatWithContact(contactID);

    systemTray->removeAllTrayIconData(contactID);
}

void MainWindow::handleTooltipEventOnContactItem(Contact *contact, const QPoint &global_item_topLeft_pos, const QPoint &global_mouse_pos){

    if(!contact){
        m_userInfoTipWindow->hideUserInfoTip();
        return;
    }


    //QString tip = QString("<b><h4>ID:%1</h4></b>").arg(contactID);
    //QToolTip::showText(global_mouse_pos, tip);


    QSize userInfoTipWindowSize = m_userInfoTipWindow->size();
    QPoint p = ui.contactsToolBox->mapToGlobal(QPoint(0,0));
    int x = p.x()-userInfoTipWindowSize.width();
    if(x < 0){
        x = p.x() + ui.contactsToolBox->width();
    }

    //    qDebug()<<"global_item_topLeft_pos:x:"<<global_item_topLeft_pos.x()<<" y:"<<global_item_topLeft_pos.y();
    //    qDebug()<<"global_mouse_pos:x:"<<global_mouse_pos.x()<<" y:"<<global_mouse_pos.y();
    //    qDebug()<<"x:"<<x<<" y:"<<global_item_topLeft_pos.y();

    m_userInfoTipWindow->showUserInfoTip(contact, mapTo(this, QPoint(x, global_item_topLeft_pos.y())) );

    activateWindow();
    raise();

}


void MainWindow::showProgressDialog(const QString &labelText, const QString & cancelButtonText, int minimum, int maximum){
    if(!progressDialog){
        progressDialog = new QProgressDialog(this);
    }

    if(labelText.trimmed().isEmpty()){
        progressDialog->setLabelText(tr("Operation in progress"));
    }else{
        progressDialog->setLabelText(labelText);
    }

    if(cancelButtonText.trimmed().isEmpty()){
        progressDialog->setCancelButtonText(tr("&Cancel"));
    }else{
        progressDialog->setCancelButtonText(cancelButtonText);
    }

    progressDialog->setRange(minimum, maximum);

    progressDialog->showNormal();
    progressDialog->raise();
    progressDialog->activateWindow();

}

void MainWindow::hideProgressDialog(){
    if(progressDialog){
        progressDialog->reset();
        progressDialog->hide();
    }
}

void MainWindow::slotMoveContactToGroup(){

    QAction *action = qobject_cast<QAction *>(sender());
    if(!action){return;}

    QString newGroupName = action->text();
    QString contactID = action->data().toString();
    Contact *contact = m_contactsManager->getUser(contactID);
    if(!contact){return;}
    quint32 oldGroupID = contact->getContactGroupID();


    if(contact->isFriend()){
        ContactGroupBase *group = m_myself->getContactGroup(newGroupName);
        if(!group){
            QMessageBox::critical(this, tr("Error"), tr("Contact group '%1' does not exist!").arg(newGroupName));
            return;
        }
        quint32 newGroupID = group->getGroupID();
        clientPacketsParser->moveContactToGroup(m_socketConnectedToServer, contactID, oldGroupID, newGroupID);
        m_myself->moveContactToAnotherGroup(contactID, oldGroupID, newGroupID);
        m_contactsManager->moveContact(contactID, oldGroupID, newGroupID);
        m_contactsManager->saveContactInfoToDatabase(contactID);

        //m_contactsManager->moveContactToUI(friendBox, oldGroupID, newGroupID, contactID);

        m_contactBox->moveContact(contact, m_myself->getContactGroup(oldGroupID), group);

        return;
    }

    if(contact->isStranger()){
        AddContactDialog dlg(contact, true, this);
        if(dlg.exec() != QDialog::Accepted){return;}
        addContact(contactID, dlg.getMessage());

    }else if(contact->isBlacklisted()){
        AddContactDialog dlg(contact, true, this);
        if(dlg.exec() != QDialog::Accepted){return;}
        slotRequestDeleteContact(contactID);
        addContact(contactID, dlg.getMessage());
    }



//    m_imUser->saveMyInfoToLocalDatabase();

    action->disconnect();
    action->deleteLater();
}

//void MainWindow::slotMoveContactToBlacklist(){
//    QAction *action = qobject_cast<QAction *>(sender());
//    if(!action){return;}

//    //QString newGroupName = action->text();
//    QString contactID = action->data().toString();
//    Contact *contact = m_contactsManager->getUser(contactID);
//    if(!contact){return;}
//    if(contact->isBlacklisted()){
//        return;
//    }


//    bool deleteMeFromOpposition = true;

//    slotRequestDeleteContact(contactID, deleteMeFromOpposition, true);

////    clientPacketsParser->addOrDeleteBlacklistedContact(m_socketConnectedToServer, contactID, true);


////    m_imUser->saveMyInfoToLocalDatabase();

//    action->disconnect();
//    action->deleteLater();
//}

void MainWindow::slotRequestDeleteContact(const QString &contactID, bool deleteMeFromOpposition, bool addToBlacklist){

    clientPacketsParser->deleteContact(m_socketConnectedToServer, contactID, deleteMeFromOpposition, addToBlacklist);

}

void MainWindow::slotDeleteContactResultReceived(const QString &contactID, bool contactDeleted, bool addToBlacklist){

    if(!contactDeleted){return;}

    Contact *contact = m_contactsManager->getUser(contactID);
    if(!contact){
        //QMessageBox::critical(this, tr("Error"), tr("Contact '%1' does not exist!").arg(contactID));
        return;
    }


    systemTray->removeAllTrayIconData(contactID);

    m_contactsManager->deleteContact(contactID, addToBlacklist);
    m_contactsManager->saveContactInfoToDatabase(contactID);

    m_contactBox->addOrRemoveContactItem(contact, false);
}

void MainWindow::slotProcessUpdatePasswordResult(quint8 errorTypeCode, const QString &message){
    
    QMessageBox::critical(this, tr("Error"), tr("Password Update Failed!"));
}

void MainWindow::slotProcessLoginServerRedirected(const QString &serverAddress, quint16 serverPort, const QString &serverName){
    //TODO
    QMessageBox::information(this, tr("Redirected"), tr("Redirected to %1:%2").arg(serverAddress).arg(serverPort));

    requestLogin(QHostAddress(serverAddress), serverPort);

}

void MainWindow::slotProcessLoginResult(quint8 errorTypeCode, const QString &errorMessage){

    destoryLoginTimer();
    ui.loginPage->slotProcessLoginResult(errorTypeCode, errorMessage);


}

void MainWindow::slotProcessClientLastLoginInfo(const QString &extIPAddress, const QString &loginTime, const QString &LogoutTime, const QString &deviceInfo){
    //TODO:

}

void MainWindow::slotProcessContactStateChanged(const QString &contactID, quint8 onlineStateCode, const QString &contactHostAddress, quint16 contactHostPort){

    Contact *contact = m_contactsManager->getUser(contactID);
    if(!contact){
        return;
    }
    
    IM::OnlineState state = IM::OnlineState(onlineStateCode);
    contact->setOnlineState(state);
    contact->setLastLoginExternalHostAddress(contactHostAddress);
    contact->setLastLoginExternalHostPort(contactHostPort);

    //TODO:Tip
    m_contactBox->updateContactItemInfo(contact);

    QString nickname = contact->getNickName();
    switch(state){
    case IM::ONLINESTATE_ONLINE:
    {
        systemTray->showMessage(QString(APP_NAME), tr("%1 is online!").arg(nickname), QSystemTrayIcon::Information);
    }
        break;
    case IM::ONLINESTATE_INVISIBLE:
    case IM::ONLINESTATE_OFFLINE:
    {
        systemTray->showMessage(QString(APP_NAME), tr("%1 is offline!").arg(nickname), QSystemTrayIcon::Information);
    }
        break;
    default:
        break;
    }

    m_chatWindowManager->contactOnlineStateChanged(contact);

    update();


}

void MainWindow::slotProcessContactsOnlineInfo(const QString &contactsOnlineInfoString){
    qDebug()<<"--MainWindow::slotProcessContactsOnlineInfo(...)"<<" contactsOnlineInfoString:"<<contactsOnlineInfoString;

    QStringList contactsOnlineInfo = contactsOnlineInfoString.split(QString(UNIT_SEPARTOR));
    foreach (QString infoString, contactsOnlineInfo) {
        QStringList infoList = infoString.split(QString(PACKET_DATA_SEPARTOR));
        QString contactID = infoList.at(0);
        Contact *contact = m_contactsManager->getUser(contactID);
        if(!contact){
            qDebug()<<"---------------XXXXXXXXXXXXX:"<<contactID;
            continue;
        }
        contact->setOnlineState(IM::OnlineState(infoList.at(1).toUInt()));
        qDebug()<<"-----------------------OnlineState:"<<contact->getOnlineState();
        contact->setLastLoginExternalHostAddress(infoList.at(2));
        contact->setLastLoginExternalHostPort(infoList.at(3).toUInt());

        m_contactBox->updateContactItemInfo(contact);
    }


}

void MainWindow::slotProcessUserInfo(const QString &userID/*, const QString &userInfo*/){
    qDebug()<<"--MainWindow::slotProcessUserInfo(...)";

    if(userID == m_myUserID){
        //imUser->setPersonalSummaryInfo(userInfo);
        ui.toolButtonUserFace->setIcon(ImageResource::createIconForContact(m_myself->getFace(), m_myself->getOnlineState()));
        ui.labelUserNickName->setText(m_myself->getNickName());
        ui.labelUserID->setText(m_myUserID);
        //imUser->saveMyInfoToLocalDatabase();
    }else{
        //TODO
        Contact *contact = m_contactsManager->getUser(userID);
        if(!contact){return;}
//        contact->setPersonalInfoString(userInfo);
//        contactsManager->saveContactInfoToDatabase(userID);

        m_contactBox->updateContactItemInfo(contact);
    }


}

void MainWindow::slotProcessContactGroupsInfo(const QString &contactGroupsInfo, quint32 personalContactGroupsInfoVersionOnServer){
    qDebug()<<"--MainWindow::slotProcessContactGroupsInfo(...)"<<" -contactGroupsInfo:"<<contactGroupsInfo;

    //STRING FORMATE: GroupID,GroupName,UserID,,UserID,...||GroupID,...
    //e.g. 100,Group100,user1,user2,user3||101,Group101,user4


    m_contactBox->clear();
    m_contactsManager->resetAllContactGroupInDatabase();
    m_myself->setContactGroupsInfoString(contactGroupsInfo);
    m_contactsManager->slotFetchAllContactsInfoFromDB();

    QStringList recentContacts;
    QList<quint32> recentInterestGroups;
    m_contactsManager->slotFetchRecentChatsFromDB(&recentContacts, &recentInterestGroups);


    QList<ContactGroupBase *> groups = m_myself->getContactGroups(false, false);
    ContactGroupBase * strangersGroup = m_myself->strangersGroup();
    groups.removeAll(strangersGroup);

    QHash<QString/*Contact ID*/, Contact*> users = m_contactsManager->getAllUsers();
    foreach (ContactGroupBase *contactGroup, groups) {
        quint32 groupID = contactGroup->getGroupID();
        QString groupName = contactGroup->getGroupName();
        if(ContactGroupBase::isUserCreatedGroup(groupID)){
            bool ok = m_contactsManager->slotAddNewContactGroupToDatabase(groupID, groupName);
            if(!ok){
                QMessageBox::critical(this, tr("Error"), tr("Can not save contact group '%1' !").arg(groupName));
            }
        }
        //m_contactsManager->slotAddNewContactGroupToUI(friendBox, groupID, groupName);

        QStringList members = contactGroup->members();
        foreach (QString contactID, members) {
            Contact *contact = users.take(contactID);
            if(!contact){
                contact =  m_contactsManager->createNewContact(contactID, groupID);
            }
            if(contact->getContactGroupID() != groupID){
                contact->setContactGroupID(groupID);
                m_contactsManager->saveContactInfoToDatabase(contactID);
            }
        }

    }


    QStringList strangers = users.keys();
    strangersGroup->setMembers(strangers);
    qDebug()<<"--------strangers:"<<strangers;

    QList<Contact*> strangersList;
    int strangersGroupID = ContactGroupBase::Group_Strangers_ID;
    foreach (Contact *contact, users.values()) {
        contact->setContactGroupID(strangersGroupID);
        if(recentContacts.contains(contact->getUserID())){
            strangersList.append(contact);
        }

    }


//    slotUpdateContactsInfo();



    m_contactBox->loadAllContacts();
    m_contactBox->collapseAll();
    m_contactBox->setContactGroupItemExpanded(m_myself->friendsGroup(), true);

    if(!m_myself->isStrangersShown()){
        m_contactBox->setContactGroupItemHidden(m_myself->strangersGroup(), true);
    }




    ui.contactsToolBox->setEnabled(true);



    //TODO:Setup recent contacts
    setupRecentChats(recentContacts, recentInterestGroups);


    ui.stackedWidget->setCurrentWidget(ui.mainPage);
    setWindowTitle(m_myUserID);


    if(contactGroupsInfo.trimmed().isEmpty()){
        QMessageBox::critical(this, tr("Error"), tr("Invalid contact groups info!"));
    }



}

void MainWindow::slotProcessContactGroupsInfo2(const QString &contactGroupsInfo, quint32 personalContactGroupsInfoVersionOnServer){
    qDebug()<<"--MainWindow::slotProcessContactGroupsInfo(...)"<<" -contactGroupsInfo:"<<contactGroupsInfo;

    //STRING FORMATE: GroupID,GroupName,UserID,,UserID,...||GroupID,...
    //e.g. 100,Group100,user1,user2,user3||101,Group101,user4


    m_contactBox->clear();
    m_contactsManager->resetAllContactGroupInDatabase();
    m_myself->setContactGroupsInfoString(contactGroupsInfo);
//    ContactGroupBase * strangersGroup = m_imUser->addContactGroup(ContactGroupBase::Group_Strangers_ID, ContactGroupBase::Group_Strangers_Name);
    //    strangersGroup->setGroupName(ContactGroupBase::Group_Strangers_Name);


    QList<ContactGroupBase *> groups = m_myself->getContactGroups(false, false);
    QHash<QString/*Contact ID*/, Contact*> users = m_contactsManager->getAllUsers();
//    bool sync = true;
    foreach (ContactGroupBase *contactGroup, groups) {
        quint32 groupID = contactGroup->getGroupID();
        QString groupName = contactGroup->getGroupName();
        if(ContactGroupBase::isUserCreatedGroup(groupID)){
            bool ok = m_contactsManager->slotAddNewContactGroupToDatabase(groupID, groupName);
            if(!ok){
//                sync = false;
                QMessageBox::critical(this, tr("Error"), tr("Can not save contact group '%1' !").arg(groupName));
            }
        }

        QStringList members = contactGroup->members();
        foreach (QString contactID, members) {
            Contact *contact = users.take(contactID);
            if(!contact){
                contact =  m_contactsManager->createNewContact(contactID, groupID);
                //clientPacketsParser->requestContactInfo(m_socketConnectedToServer, contactID);
            }
            if(contact->getContactGroupID() != groupID){
                contact->setContactGroupID(groupID);
                m_contactsManager->saveContactInfoToDatabase(contactID);
            }
        }
    }

    ContactGroupBase * strangersGroup = m_myself->strangersGroup();
    strangersGroup->setMembers(users.keys());
    foreach (Contact *contact, users.values()) {
        contact->setContactGroupID(ContactGroupBase::Group_Strangers_ID);
    }


//    if(sync){
//        m_imUser->setPersonalContactGroupsVersion(personalContactGroupsInfoVersionOnServer);
//        m_imUser->saveMyInfoToLocalDatabase();
//    }


    slotUpdateContactsInfo2();
//    m_contactsManager->slotFetchAllContactsInfo(friendBox);

}

void MainWindow::slotProcessContactsInfoVersion(const QString &contactsInfoVersionString, quint32 contactGroupsInfoVersionOnServer){
    qDebug()<<"--MainWindow::slotProcessContactsInfoVersion(...)"<<" -contactsInfoVersionString:"<<contactsInfoVersionString <<"   contactGroupsInfoVersionOnServer:"<<contactGroupsInfoVersionOnServer;

    //FORMATE: UserID,PersonalSummaryInfoVersion,PersonalDetailInfoVersion,PersonalMessageInfoVersion;UserID,...
    //e.g. user1,10,10,2;user2,5,6,15;user3,11,10,20


    if(contactsInfoVersionString.trimmed().isEmpty()){return;}

    bool needToUpdateContactGroupsInfo = false;


    QStringList list = contactsInfoVersionString.split(";");
    foreach (QString info, list) {
        QStringList infoList = info.split(",");
        if(infoList.size() != 4 ){
            qCritical()<<"ERROR! Invalid version info format!";
            continue;
        }
        QString contactID = infoList.at(0);
        quint32 summaryInfoVersion = infoList.at(1).toUInt();
        quint32 detailInfoVersion = infoList.at(2).toUInt();
        quint32 personalMessageInfoVersion = infoList.at(3).toUInt();

        Contact *contact = m_contactsManager->getUser(contactID);
        if(!contact){
            //NOTE:Need to sync data with server
            needToUpdateContactGroupsInfo = true;
            contact = m_contactsManager->createNewContact(contactID);
            //clientPacketsParser->requestContactInfo(m_socketConnectedToServer, contactID);
            continue;
        }else{
            //int groupID = imUser->groupIDThatContactBelongsTo(contactID);
            int groupID = contact->getContactGroupID();
            if(/*groupID == ContactGroupBase::Group_Blacklist_ID ||*/ groupID == ContactGroupBase::Group_Strangers_ID){
                //NOTE:Need to sync data with server
                needToUpdateContactGroupsInfo = true;
                continue;
            }

            if(summaryInfoVersion != contact->getPersonalSummaryInfoVersion()){
                clientPacketsParser->requestContactInfo(m_socketConnectedToServer, contactID);
            }
            if(detailInfoVersion != contact->getPersonalDetailInfoVersion()){
                clientPacketsParser->requestContactInfo(m_socketConnectedToServer, contactID, false);
            }
            if(personalMessageInfoVersion != contact->getPersonalMessageInfoVersion()){
                clientPacketsParser->requestPersonalMessage(m_socketConnectedToServer, contactID);
            }

        }


    }

    if(needToUpdateContactGroupsInfo /*|| (contactGroupsInfoVersionOnServer != m_imUser->getPersonalContactGroupsVersion())*/ ){
        clientPacketsParser->requestPersonalContactGroupsInfo(m_socketConnectedToServer);
    }


}

void MainWindow::slotProcessCreateOrDeleteContactGroupResult(quint32 groupID, const QString &groupName, bool createGroup, bool result){

    qDebug()<<"--MainWindow::slotProcessCreateOrDeleteContactGroupResult(...)"<<" groupID:"<<groupID<<" groupName:"<<groupName<<" createGroup:"<<createGroup<<" result:"<<result;

    hideProgressDialog();

    if(result){
        bool ok = false;
        if(createGroup){
            ok = m_contactsManager->slotAddNewContactGroupToDatabase(groupID, groupName);
            m_myself->addContactGroup(groupID, groupName);

            m_contactBox->insertContactGroupItem(m_myself->getContactGroup(groupID));
            //m_contactBox->addOrRemoveContactGroupItem(m_myself->getContactGroup(groupID), true);
        }else{
            m_contactBox->addOrRemoveContactGroupItem(m_myself->getContactGroup(groupID), false);

            ok = m_contactsManager->deleteContactGroupFromDatabase(groupID);
            m_myself->deleteContactGroup(groupID);
        }

        if(ok){
//            m_myself->updatePersonalContactGroupsInfoVersion();
            m_myself->saveMyInfoToLocalDatabase();
        }

    }else{
        QString errorMsg = tr("Failed to %1 group '%2'! ").arg(createGroup?tr("create"):tr("delete")).arg(groupName);
        QMessageBox::critical(this, tr("Error"), QString("%1").arg(errorMsg));
    }

}


//void MainWindow::slotProcessSearchContactsResult(const QString &users){

//}

void MainWindow::slotProcessAddContactResult(const QString &contactID, const QString &userNickName, const QString &userFace, int contactGroupID, quint8 errorTypeCode, const QString &reasonMessage, quint8 onlineStateCode){
    qDebug()<<"--MainWindow::slotProcessAddContactResult(...) "<<"  contactID:"<<contactID;

    IM::ErrorType type = IM::ErrorType(errorTypeCode);
    switch(type){
    case IM::ERROR_NoError :
    {

        //Save contact to default group
        quint32 groupID = ContactGroupBase::Group_Friends_ID;
        Contact *contact = m_contactsManager->getUser(contactID);
        if(!contact){
            contact =  m_contactsManager->createNewContact(contactID, groupID, userNickName, userFace);
        }
        Q_ASSERT(contact);
        contact->setOnlineState(IM::OnlineState(onlineStateCode));
        //contact->setContactGroupID(groupID);
        //m_contactsManager->saveContactInfoToDatabase(contactID);


        if(contact->getContactGroupID() != contactGroupID){
            contact->setContactGroupID(groupID);
            m_contactsManager->saveContactInfoToDatabase(contactID);
        }
        m_myself->saveMyInfoToLocalDatabase();

        m_contactBox->addOrRemoveContactItem(contact, true);


        //show trayicon info
        if(autoShowSystemMessage){
            getNewContactSettings(contactID);
        }else{
            //TODO
            TrayIconData data(STIDT_FriendshipApplicationResult, QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz"), contactID);
            data.setToolTip(contactID);
            data.settrayIconType(TrayIconData::TRAYICON_Flash);
            data.setFirstIcon(QIcon(":/resources/images/systemmessage.png"));
            data.setData(contactID);
            
            systemTray->appendTrayIconData(data);
        }
        
    }
        break;
    case IM::ERROR_RequestDenied :
    {
        QString msg = tr("User %1 rejected your request that add (s)he as a contact!)").arg(contactID);
        if(!reasonMessage.trimmed().isEmpty()){
            msg += tr("<p>Message: %1</p>").arg(reasonMessage);
        }
        QMessageBox::critical(this, tr("Request Rejected"), msg);
    }
        break;
    default:
        QMessageBox::critical(this, tr("Error"), tr("Can not add user %1 as a contact! Error code:%2").arg(contactID).arg(errorTypeCode));
        
    }

    
}

void MainWindow::getNewContactSettings(const QString &contactID){
    
    Contact *contact = m_contactsManager->getUser(contactID);
    if(!contact){
        return;
    }
    
    int existingGroupID = m_myself->groupIDThatContactBelongsTo(contactID);

    AddContactDialog dlg(contact, false);
    dlg.exec();
    quint32 newGroupID = dlg.getGroupID();
    QString remarkName = dlg.getNewName();
    if(!remarkName.trimmed().isEmpty()){
        contact->setRemarkName(remarkName);
        m_contactsManager->saveContactInfoToDatabase(contactID);
    }
    qDebug()<<"---------existingGroupID:"<<existingGroupID<<"  newGroupID:"<<newGroupID;
    if(existingGroupID == newGroupID){
        return;
    }

    clientPacketsParser->moveContactToGroup(m_socketConnectedToServer, contactID, existingGroupID, newGroupID);

    m_contactsManager->moveContact(contactID, existingGroupID, newGroupID);
    m_contactsManager->saveContactInfoToDatabase(contactID);
//    m_imUser->saveMyInfoToLocalDatabase();


    m_contactBox->moveContact(contact, m_myself->getContactGroup(existingGroupID), m_myself->getContactGroup(newGroupID));

    
}

void MainWindow::slotProcessPersonalMessage(const QString &userID, const QString &message){
    qDebug()<<"--MainWindow::slotProcessPersonalMessage(...)";


    //TODO:Update personal message to UI

    if(userID.toLower() == m_myUserID.toLower()){
        m_myself->setPersonalMessage(message);
        m_myself->saveMyInfoToLocalDatabase();

    }else{

        Contact *ct = m_contactsManager->getUser(userID);
        if(!ct){
            return;
        }

        ct->setPersonalMessage(message);

        m_contactsManager->saveContactInfoToDatabase(userID);

    }



}

void MainWindow::slotSearch(){
    if(!search){
        search = new Search();
        connect(search, SIGNAL(signalSearchContact(const QString &, bool, bool, bool, int)), this, SLOT(searchContact(const QString &, bool, bool, bool, int)), Qt::QueuedConnection);
        connect(search, SIGNAL(signalSearchInterestGroup(const QString &, int)), this, SLOT(searchInterestGroup(const QString &, int)));

        connect(search, SIGNAL(signalAddContact(const QString&, const QString&)), this, SLOT(addContact(const QString&, const QString&)));
        connect(search, SIGNAL(signalJoinInterestGroup(quint32, const QString&)), this, SLOT(joinInterestGroup(quint32, const QString&)));


        connect(clientPacketsParser, SIGNAL(signalSearchContactsResultPacketReceived(const QString &)), search, SLOT(slotSearchContactsResultPacketReceived(const QString &)), Qt::QueuedConnection);
        connect(clientPacketsParser, SIGNAL(signalSearchInterestGroupsResultPacketReceived(const QString &)), search, SLOT(slotSearchInterestGroupsResultPacketReceived(const QString &)), Qt::QueuedConnection);

    }

    search->show();

}

void MainWindow::searchContact(const QString &propertiesString, bool matchExactly, bool searchOnlineUsersOnly, bool searchWebcamUsersOnly, int startIndex){
    clientPacketsParser->searchContact(m_socketConnectedToServer, propertiesString, matchExactly, searchOnlineUsersOnly, searchWebcamUsersOnly, startIndex);
}

void MainWindow::searchInterestGroup(const QString &keyword, int startIndex){
    clientPacketsParser->searchInterestGroup(m_socketConnectedToServer, keyword, startIndex);
}

void MainWindow::addContact(const QString &userID, const QString &verificationMessage){
    clientPacketsParser->addContact(m_socketConnectedToServer, userID, verificationMessage);
}



void MainWindow::slotProcessContactRequestFromUser(const QString &userID, const QString &userNickName, const QString &userFace, const QString &verificationMessage){

    if(autoShowSystemMessage){
        showContactRequestFromUser(userID, userNickName, userFace, verificationMessage);
    }else{

        TrayIconData data(STIDT_FriendshipApplicationFromContact, QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz"), userID);
        data.setToolTip(userID);
        data.settrayIconType(TrayIconData::TRAYICON_Flash);
        data.setFirstIcon(QIcon(":/resources/images/systemmessage.png"));

        QStringList list;
        list << userID << userNickName << userFace << verificationMessage;
        data.setData(list.join(QChar(PACKET_DATA_SEPARTOR)));

        systemTray->appendTrayIconData(data);
    }

}

void MainWindow::showContactRequestFromUser(const QString &userID, const QString &userNickName, const QString &userFace, const QString &verificationMessage){


    Contact user(userID, userNickName, this);
    user.setFace(userFace);

    AddContactDialog dlg(&user, verificationMessage);
    if(dlg.exec() == QDialog::Accepted){
        if(dlg.requestRejected()){
            clientPacketsParser->responseAddContactRequestFromUser(m_socketConnectedToServer, userID, false, 0, dlg.getMessage());
        }else{
            clientPacketsParser->responseAddContactRequestFromUser(m_socketConnectedToServer, userID, true, dlg.getGroupID(), "");
//            clientPacketsParser->addContact(m_socketConnectedToServer, userID, dlg.getMessage(), dlg.getGroupID());
        }
    }

}



void MainWindow::slotProcessChatMessageReceivedFromContact(const QString &contactID, const QString &message, const QString &time){
    qDebug()<<"--MainWindow::slotProcessChatMessageReceivedFromContact(...)--Contact:"<<contactID<<" Message:"<<message;


    Contact *contact = m_contactsManager->getUser(contactID);
    if(!contact){return;}

    QString timeString = time;
    if(timeString.isEmpty()){
        timeString = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    }


    m_contactsManager->saveContactChatMessageToDatabase(contactID, m_myUserID, message, timeString);

    //if(chatWindowManager->isVisible() || autoShowChatMessageFromContact){
    if(m_chatWindowManager->isContactChatWindowOpen(contactID) || autoShowChatMessageFromContact){
        m_chatWindowManager->slotNewMessageReceivedFromContact(contactID, message, timeString);
    }else{

        m_contactBox->chatMessageReceivedFromContact(contact);

        contact->appandUnreadMessage(timeString, message);

        //TODO:
        QHash<QString/*Time*/, QVariant/*Message*/> data;

        if(systemTray->trayIconDataExists(contactID)){
//            data = systemTray->getData(contactID).toHash();
//            data.insertMulti(timeString, message);
//            systemTray->setData(contactID, QVariant(data));
        }else{
            TrayIconData trayIconData(STIDT_ContactChatMessage, contactID, contactID);
            trayIconData.setToolTip(contactID);
            trayIconData.settrayIconType(TrayIconData::TRAYICON_Flash);
            trayIconData.setFirstIcon(ImageResource::createIconForContact(contact->getFace(), IM::ONLINESTATE_ONLINE));

//            data.insertMulti(timeString, message);
//            trayIconData.setData(QVariant(data));
            systemTray->appendTrayIconData(trayIconData);
        }

        systemTray->updateSystemTrayIcon();
    }


}

void MainWindow::slotProcessChatMessageCachedOnServer(const QStringList &messages){
    qDebug()<<"--MainWindow::slotProcessChatMessageCachedOnServer(...)";

    qDebug()<<"----ChatMessageCachedOnServer! Messages:"<<messages;


    foreach (QString messageString, messages) {
        QStringList info = messageString.split(QString(PACKET_DATA_SEPARTOR));
        if(info.size() == 3){
            slotProcessChatMessageReceivedFromContact(info.at(0), info.at(1), info.at(2));

        }else{
            qWarning()<<"Invalid Chat Message Format!";
        }
    }


}

void MainWindow::slotProcessInterestGroupChatMessagesReceivedFromContact(quint32 interestGroupID, const QString &contactID, const QString &message, const QString &time){

    InterestGroup *group = m_contactsManager->getInterestGroup(interestGroupID);
    if(!group){
        group = new InterestGroup(interestGroupID, "", this);
        //TODO:
        clientPacketsParser->requestInterestGroupInfo(m_socketConnectedToServer, interestGroupID);
        m_contactsManager->addNewInterestGroupToDatabase(group);
    }

    Contact *contact = m_contactsManager->getUser(contactID);
    if(!contact){
        contact =  m_contactsManager->createNewContact(contactID);
//        contact = new Contact(contactID, this);
        clientPacketsParser->requestContactInfo(m_socketConnectedToServer, contactID);
//        contactsManager->slotAddNewContactToDatabase(contact);
    }


    QString timeString = time;
    if(timeString.isEmpty()){
        timeString = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    }


    m_contactsManager->saveInterestGroupChatMessageToDatabase(contactID, interestGroupID, message, timeString);


//    if(chatWindowManager->isVisible() || autoShowChatMessageFromContact){
    if(m_chatWindowManager->isInterestGroupChatWindowOpen(interestGroupID) || autoShowChatMessageFromContact){
        m_chatWindowManager->slotNewMessageReceivedFromInterestGroup(interestGroupID, contactID, message, timeString);
    }else{
        //TODO:
        group->appandUnreadMessage(contact, message, timeString);


        QHash<QString/*Time*/, QVariant/*Contact ID, Message*/ > data;
        //QHash<QString/*Time*/, QStringList/*Contact ID, Message*/ > data;

        QString interestGroupIDString = QString::number(interestGroupID);

        if(systemTray->trayIconDataExists(interestGroupIDString)){
            data = systemTray->getData(interestGroupIDString).toHash();
            QStringList list;
            list << contactID << message;
            data.insertMulti(timeString, list);
            systemTray->setData(interestGroupIDString, QVariant(data));
        }else{
            TrayIconData trayIconData(STIDT_InterestGroupChatMessage, interestGroupIDString, interestGroupIDString);
            trayIconData.setToolTip(group->getGroupName());
            trayIconData.settrayIconType(TrayIconData::TRAYICON_Flash);
            trayIconData.setFirstIcon(ImageResource::createIconForInterestGroup());

            QStringList list;
            list << contactID << message;
            data.insertMulti(timeString, list);
            //            data.insertMulti(timeString, qMakePair(contactID, message));
            trayIconData.setData(QVariant(data));
            systemTray->appendTrayIconData(trayIconData);
        }

        systemTray->updateSystemTrayIcon();
    }


}


void MainWindow::slotProcessInterestGroupChatMessagesCachedOnServer(const QStringList &messages){

    foreach (QString messageString, messages) {
        QStringList info = messageString.split(QString(PACKET_DATA_SEPARTOR));
        if(info.size() == 4){
            slotProcessInterestGroupChatMessagesReceivedFromContact(info.at(0).toUInt(), info.at(1), info.at(2), info.at(3));

        }else{
            qWarning()<<"Invalid Chat Message Format!";
        }
    }


}

void MainWindow::slotProcessImageDownloadResult(const QString &contactID, const QString &imageName, const QByteArray &image){
    qDebug()<<"--MainWindow::slotProcessImageDownloadResult(...)"<<" contactID:"<<contactID<<" imageName:"<<imageName;

    if(image.isNull()){
        qCritical()<<QString("ERROR! Image '%1' downloading failed!").arg(imageName);
        m_chatWindowManager->processImageDownloadResult(contactID, imageName, false);
        return;
    }

    QString md5String = QCryptographicHash::hash(image, QCryptographicHash::Md5).toHex();
    QFileInfo fileInfo(imageName);
    if(md5String != fileInfo.baseName()){
        qCritical()<<"ERROR! Image from "<<contactID<<" is damaged!";

        m_chatWindowManager->processImageDownloadResult(contactID, imageName, false);
        return;
    }


    QString imageCachePath = Settings::instance()->getImageCacheDir();
    QString filePath = imageCachePath + "/" + imageName;


    QFile file(filePath);
    if (!file.open(QFile::WriteOnly)) {
        qCritical()<< QString("ERROR! Failed to write image '%1'! %2").arg(filePath).arg(file.errorString());

        m_chatWindowManager->processImageDownloadResult(contactID, imageName, false);
        return ;
    }
    file.write(image);
    file.flush();
    file.close();


    m_chatWindowManager->processImageDownloadResult(contactID, imageName, true);

}

void MainWindow::slotProcessImageDownloadRequest(const QString &contactID, const QString &imageName){
    qDebug()<<"--MainWindow::slotProcessImageDownloadRequest(...) "<<" contactID:"<<contactID<<" imageName"<<imageName;

    QString imageCachePath = Settings::instance()->getImageCacheDir();
    QString filePath = imageCachePath + "/" + imageName;

    QFile file(filePath);
    if(file.exists()){
        if (file.open(QFile::ReadOnly)) {
            QByteArray image = file.readAll();
            file.close();

            QString md5String = QCryptographicHash::hash(image, QCryptographicHash::Md5).toHex();
            QFileInfo fileInfo(imageName);
            if(md5String == fileInfo.baseName()){
                Contact *contact = m_contactsManager->getUser(contactID);
                if(!contact || contact->getOnlineState() == IM::ONLINESTATE_OFFLINE || m_myself->isSyncAllChatMessagesToServer()){
                    //Send image to server
                    clientPacketsParser->sendImageToContact(m_socketConnectedToServer, contactID, imageName, image, true);
                }else{
                    //Send image to contact directly
                    clientPacketsParser->sendImageToContact(contact->getSocketID(), m_myUserID, imageName, image, false);
                }
                return ;
            }

            qCritical()<<"ERROR! Image "<<imageName<<" is damaged!";
            file.remove();

        }else{
            qCritical()<< QString("ERROR! Failed to open image '%1'! %2").arg(imageName).arg(file.errorString());
        }
    }

    clientPacketsParser->sendImageToContact(m_socketConnectedToServer, contactID, imageName, QByteArray(), true);


}

void MainWindow::requestDownloadImage(const QString &contactID, const QString &imageName){
    qDebug()<<"--MainWindow::requestDownloadImage(...) "<<" contactID:"<<contactID<<" imageName"<<imageName;

    Contact *contact = m_contactsManager->getUser(contactID);
    if(!contact || contact->getOnlineState() == IM::ONLINESTATE_OFFLINE || m_myself->isSyncAllChatMessagesToServer()){
        //Send image downloading request to server
        clientPacketsParser->requestDownloadImageFromContact(m_socketConnectedToServer, contactID, imageName, true);
    }else{
        //Send image downloading request to contact directly
        clientPacketsParser->requestDownloadImageFromContact(contact->getSocketID(), m_myUserID, imageName, false);
    }

}


void MainWindow::slotSendChatMessageToContact(Contact *contact, const QString &message, const QStringList &imageNameList){


    //Contact *contact = contactsManager->getUser(contactID);
    if(!contact){return;}
    QString contactID = contact->getUserID();
    if(contact->getOnlineState() == IM::ONLINESTATE_OFFLINE || m_myself->isSyncAllChatMessagesToServer()){
        clientPacketsParser->sendChatMessageToServer(m_socketConnectedToServer, contactID, message, imageNameList);
    }else{
        clientPacketsParser->sendChatMessageToContact(contact->getSocketID(), contactID, message);
    }

    m_contactsManager->saveContactChatMessageToDatabase(m_myUserID, contactID, message);
    

}

void MainWindow::slotSendChatMessageToInterestGroup(InterestGroup *interestGroup, const QString &message, const QStringList &imageList){

    //TODO:

    if(!interestGroup){
        return;
    }

    if(!interestGroup->getState()){
        QMessageBox::critical(this, tr("Error"), tr("Can not send message! The group is disbanded or you are not member of it!"));
        return;
    }


    //Send message to server
    clientPacketsParser->sendInterestGroupChatMessageToServer(m_socketConnectedToServer, interestGroup->getGroupID(), message, imageList);

//    QStringList members = interestGroup->members();
//    foreach (QString contactID , members) {
//        Contact *contact = m_contactsManager->getUser(contactID);
//        if(!contact){continue;}
//        if(contact->getOnlineState() == IM::ONLINESTATE_OFFLINE){
//            //            clientPacketsParser->sendChatMessageToServer(contactID, message);
//        }else{
//            clientPacketsParser->sendInterestGroupChatMessageToContact(m_socketConnectedToServer, contactID, interestGroup->getGroupID(), message, contact->getLastLoginExternalHostAddress(), contact->getLastLoginExternalHostPort());
//        }
//    }

    m_contactsManager->saveInterestGroupChatMessageToDatabase(m_myUserID, interestGroup->getGroupID(), message);


}

void MainWindow::slotProcessInterestGroupsList(const QString &interestGroupsListFromServer, quint32 interestGroupsInfoVersionOnServer){
    qDebug()<<"--MainWindow::slotProcessInterestGroupsList(..) "<<"----interestGroupsListFromServer:"<<interestGroupsListFromServer;

    //Interest Groups List Format: GroupID,GroupInfoVersion,MemberListInfoVersion;GroupID,GroupInfoVersion,MemberListInfoVersion

    QList<InterestGroup *> interestGroupsOnLocal = m_contactsManager->getInterestGroupsList();

    QList<quint32> groupsOnServer;
    QStringList infoList = interestGroupsListFromServer.split(";");
    foreach (QString info, infoList) {
        QStringList list = info.split(",");
        if(list.size() != 3){
            qCritical()<<"Invalid Interest Group Info!";
            continue;
        }
        quint32 groupID = list.at(0).toUInt();
        groupsOnServer.append(groupID);

        quint32 groupInfoVersion = list.at(1).toUInt();
        quint32 memberListInfoVersion = list.at(2).toUInt();
        //qDebug()<<"Server: groupInfoVersion:"<<groupInfoVersion<<" memberListInfoVersion:"<<memberListInfoVersion;

        InterestGroup *group = m_contactsManager->getInterestGroup(groupID);
        if(group){
            //qDebug()<<"Local: groupInfoVersion:"<<group->getGroupInfoVersion()<<" memberListInfoVersion:"<<group->getGroupMemberListInfoVersion();
            interestGroupsOnLocal.removeAll(group);

            if(groupInfoVersion != group->getGroupInfoVersion()){
                clientPacketsParser->requestInterestGroupInfo(m_socketConnectedToServer, groupID);
            }else if(memberListInfoVersion != group->getGroupMemberListInfoVersion()){
                clientPacketsParser->requestInterestGroupMembersInfo(m_socketConnectedToServer, groupID);
            }
        }else{
            group = new InterestGroup(groupID, "", this);
            m_contactsManager->addNewInterestGroupToDatabase(group);
            //TODO:
            clientPacketsParser->requestInterestGroupInfo(m_socketConnectedToServer, groupID);
            
        }
        
    }

    foreach (InterestGroup *group, interestGroupsOnLocal) {
        if(!group){continue;}
        if(group->getState() == 1 ){
            group->setState(0);
            m_contactsManager->saveInterestGroupInfoToDatabase(group);
        }
    }
    

    m_myself->setInterestGroupInfoVersion(interestGroupsInfoVersionOnServer);
    
    m_myself->saveMyInfoToLocalDatabase();

    updateAllInterestGroupsInfoToUI();

}

void MainWindow::slotProcessInterestGroupInfo(const QString &interestGroupInfoFromServer, quint32 groupID){

    InterestGroup *interestGroup = m_contactsManager->getInterestGroup(groupID);
    if(!interestGroup){
        return;
    }
    
    quint32 oldMembersInfoVersion = interestGroup->getGroupMemberListInfoVersion();
    
    interestGroup->setGroupInfoString(interestGroupInfoFromServer);
    qWarning()<<"interestGroupInfoFromServer:"<<interestGroupInfoFromServer;
    
    if(oldMembersInfoVersion != interestGroup->getGroupMemberListInfoVersion()){
        clientPacketsParser->requestInterestGroupMembersInfo(m_socketConnectedToServer, groupID);
        interestGroup->setGroupMemberListInfoVersion(oldMembersInfoVersion);
    }

    updateInterestGroupInfoToUI(interestGroup);
    m_contactsManager->saveInterestGroupInfoToDatabase(interestGroup);
    

    
    
}

void MainWindow::slotProcessInterestGroupMembersInfo(const QString &interestGroupMembersInfoFromServer, quint32 interestGroupMembersInfoVersionOnServer, quint32 groupID){
    qDebug()<<"--MainWindow::slotProcessInterestGroupMembersInfo(...) "<<" groupID:"<<groupID;
    //qDebug()<<"interestGroupMembersInfoVersionOnServer:"<<interestGroupMembersInfoVersionOnServer;

    InterestGroup *interestGroup = m_contactsManager->getInterestGroup(groupID);
    if(!interestGroup){
        qCritical()<<"No such InterestGroup:"<<groupID;
        return;
    }
    
    QHash <QString/*Member's ID*/, InterestGroup::MemberRole/*Member's Role*/> membersHash;
    QStringList infoList = interestGroupMembersInfoFromServer.split(QString(PACKET_DATA_SEPARTOR));
    foreach (QString info, infoList) {
        QStringList list = info.split(CONTACT_INFO_SEPARATOR);
        QString contactID = list.at(0);
        quint32 contactInfoVersion = list.at(1).toUInt();
        InterestGroup::MemberRole memberRole = InterestGroup::MemberRole(list.at(2).toUInt());
        membersHash.insert(contactID, memberRole);

        if(contactID == m_myself->getUserID()){continue;}
        qDebug()<<"contactID:"<<contactID<<" contactInfoVersion:"<<contactInfoVersion<<" memberRole:"<<memberRole;

        Contact *contact = m_contactsManager->getUser(contactID);
        if(!contact){
            contact =  m_contactsManager->createNewContact(contactID);
            //clientPacketsParser->requestContactInfo(m_socketConnectedToServer, contactID);

//            contact = new Contact(contactID, this);
//            contactsManager->slotAddNewContactToDatabase(contact);
        }//else{
            if(contactInfoVersion != contact->getPersonalSummaryInfoVersion()){
                clientPacketsParser->requestContactInfo(m_socketConnectedToServer, contactID);
            }
        //}

            }
    
    interestGroup->setMembersHash(membersHash);
    
    if(!membersHash.isEmpty()){
        if(m_contactsManager->saveInterestGroupMembersToDatabase(interestGroup)){
            interestGroup->setGroupMemberListInfoVersion(interestGroupMembersInfoVersionOnServer);
        }
    }

    m_contactsManager->saveInterestGroupInfoToDatabase(interestGroup);


    
    //TODO
    
    
}

void MainWindow::slotProcessCreateInterestGroupResult(quint32 groupID, const QString &groupName){

    hideProgressDialog();

    if(!groupID){
        QMessageBox::critical(this, tr("Error"), tr("Failed to create group '%1'!").arg(groupName));
        return;
    }

    //if(m_myself->isMemberOfInterestGroup(groupID)){return;}
    if(m_contactsManager->getInterestGroupIDsList().contains(groupID)){return;}

    InterestGroup *group = new InterestGroup(groupID, "", this);
    group->setGroupName(groupName);
    group->setCreatorID(m_myUserID);
    group->addMember(m_myUserID, InterestGroup::Role_Creator);

    m_contactsManager->addNewInterestGroupToDatabase(group);

    addInterestGroupToUI(group);
    m_contactsManager->saveInterestGroupInfoToDatabase(group);

}

void MainWindow::slotProcessDisbandInterestGroupResult(quint32 groupID, bool result){

    hideProgressDialog();

    InterestGroup *group = m_contactsManager->getInterestGroup(groupID);
    if(!group){return;}

    if(!result){
        QMessageBox::critical(this, tr("Error"), tr("Failed to disband group '%1(%2)'!").arg(group->getGroupName()).arg(groupID));
        return;
    }

    QMessageBox::critical(this, tr("Message"), tr("Group '%1(%2)' has been disbanded!").arg(group->getGroupName()).arg(groupID));

    group->setState(0);

    updateInterestGroupInfoToUI(group);
    m_contactsManager->saveInterestGroupInfoToDatabase(group);

}

void MainWindow::slotProcessUserRequestJoinInterestGroup(quint32 groupID, const QString &verificationMessage, const QString &userID, const QString &nickName, const QString &face){

    qDebug()<<"--MainWindow::slotProcessUserRequestJoinInterestGroup(...)"<<" groupID:"<<groupID<<" verificationMessage:"<<verificationMessage<<" userID:"<<userID;

    //TODO


}

void MainWindow::slotProcessUserJoinOrQuitInterestGroup(quint32 groupID, const QString &memberID, bool join){

    qDebug()<<"--MainWindow::slotProcessUserJoinOrQuitInterestGroup(...)"<<" groupID:"<<groupID;

    //TODO


    InterestGroup *group;

    if(memberID == m_myUserID){
        if(join){
            group = new InterestGroup(groupID, "", this);
            m_contactsManager->addNewInterestGroupToDatabase(group);
            addInterestGroupToUI(group);

            clientPacketsParser->requestInterestGroupInfo(m_socketConnectedToServer, groupID);
        }else{
            group = m_contactsManager->getInterestGroup(groupID);
            if(!group){return;}
            group->setState(0);
            updateInterestGroupInfoToUI(group);

            m_chatWindowManager->closeInterestGroupChatWindow(group);
        }
        //TODO:Notify User

    }else{
        group = m_contactsManager->getInterestGroup(groupID);
        if(!group){return;}

        if(join){
            group->addMember(memberID, InterestGroup::Role_Member);
        }else{
            group->deleteMember(memberID);
        }
        group->updateMemberListInfoVersion();
        //TODO

        m_chatWindowManager->interestGroupMemberJoinedOrQuitted(groupID, memberID, join);

    }

    m_contactsManager->saveInterestGroupInfoToDatabase(group);

    if(memberID == m_myUserID || group->isAdmin(m_myUserID)){
        //show trayicon info
        if(autoShowSystemMessage){
            QString groupName = group->getGroupName();
            if(groupName.trimmed().isEmpty()){
                groupName = QString::number(groupID);
            }else{
                groupName = groupName + "(" + QString::number(groupID) +")";
            }
            QMessageBox::information(0, tr("System Message"), tr("%1 have %2 the group %3!").arg( (m_myUserID==memberID)?tr("You"):memberID ).arg(join?tr("joined"):tr("quitted")).arg(groupName));

        }else{
            //TODO
            TrayIconData data(STIDT_InterestGroupMemberJoinedOrQuitted, QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz"), QString::number(groupID));
            data.setToolTip(tr("System Message"));
            data.settrayIconType(TrayIconData::TRAYICON_Flash);
            data.setFirstIcon(QIcon(":/resources/images/systemmessage.png"));

            QStringList list;
            list << QString::number(groupID) << memberID << QString::number(quint8(join?1:0)) ;
            data.setData(list);

            systemTray->appendTrayIconData(data);
        }
    }





}


void MainWindow::handleContextMenuEventOnInterestGroupList(const QPoint &point){

    QListWidgetItem * item = ui.listWidgetGroups->itemAt(point);
    QPoint globalPoint = ui.listWidgetGroups->mapToGlobal(point);
    QMenu contextMenu;

    QAction *actionCreateNewGroup = contextMenu.addAction(tr("Create New Group"));
    connect(actionCreateNewGroup, SIGNAL(triggered()), this, SLOT(slotCreateInterestGroup()));
    if(!item){
        contextMenu.exec(globalPoint);
        return;
    }

    quint32 groupID = item->data(Qt::UserRole).toUInt();
    InterestGroup *group = m_contactsManager->getInterestGroup(groupID);


    QMessageBox::information(this, "", QString::number(groupID));


    if(group->getState()){
        if(group->isCreator(m_myUserID)){
            QAction *actionDisbandGroup = contextMenu.addAction(tr("Disband Group"));
            actionDisbandGroup->setData(QVariant(groupID));
            connect(actionDisbandGroup, SIGNAL(triggered()), this, SLOT(slotDisbandInterestGroup()));
        }

        QAction *quitGroupAction = contextMenu.addAction(tr("Quit Group"));
        quitGroupAction->setData(QVariant(groupID));
        connect(quitGroupAction, SIGNAL(triggered()), this, SLOT(slotQuitInterestGroup()));


    }else{
        QAction *actionDeleteGroup = contextMenu.addAction(tr("Delete Group Info"));
        actionDeleteGroup->setData(QVariant(groupID));
        connect(actionDeleteGroup, SIGNAL(triggered()), this, SLOT(slotDeleteInterestGroupFromLocal()));
    }


    contextMenu.exec(globalPoint);

}

void MainWindow::slotCreateInterestGroup(){

    bool ok = false;
    QString labelText = tr("Group Name:\n(Only word-characters up to 32 are acceptable!)");
    QString groupName = QInputDialog::getText(this, tr("Create Group"),
                                                 labelText, QLineEdit::Normal,
                                                 "", &ok);
    if (ok && !groupName.isEmpty()){
        int pos = 0;
        QRegExpValidator rxValidator(this);
        QRegExp rx("\\b\\w{0,32}\\b");
        rxValidator.setRegExp(rx);
        if(rxValidator.validate(groupName, pos) != QValidator::Acceptable){
            QMessageBox::critical(this, tr("Error"), tr("Invalid Group Name!"));
            return ;
        }

//        QList<quint32> groups = m_myself->getInterestGroups();
//        foreach (quint32 groupID, groups) {
//            InterestGroup *group = m_contactsManager->getInterestGroup(groupID);
//            if(!group){continue;}
//            if(group->getGroupName() == groupName){
//                QMessageBox::critical(this, tr("Error"), tr("Group with the same name already exists!"));
//                return;
//            }
//        }

        QList<InterestGroup *> interestGroupsOnLocal = m_contactsManager->getInterestGroupsList();
        foreach (InterestGroup *group, interestGroupsOnLocal) {
            if(!group){continue;}
            if(group->getGroupName() == groupName){
                QMessageBox::critical(this, tr("Error"), tr("Group with the same name already exists!"));
                return;
            }
        }

        showProgressDialog();
        clientPacketsParser->requestCreateInterestGroup(m_socketConnectedToServer, groupName);
    }

}

void MainWindow::slotDisbandInterestGroup(){
    QAction *action = qobject_cast<QAction *>(sender());
    if(!action){return;}

    quint32 groupID = action->data().toUInt();
    InterestGroup *group = m_contactsManager->getInterestGroup(groupID);
    if(!group){return;}

    int ret = QMessageBox::question(this, tr("Question"),
                                    tr("Do you really want to disband the group '%1(%2)' ?").arg(group->getGroupName()).arg(groupID),
                                    QMessageBox::Yes|QMessageBox::No,
                                    QMessageBox::No);

    if(ret == QMessageBox::No){return;}

    showProgressDialog();
    clientPacketsParser->requestDisbandInterestGroup(m_socketConnectedToServer, groupID);

}

void MainWindow::slotDeleteInterestGroupFromLocal(){
    QAction *action = qobject_cast<QAction *>(sender());
    if(!action){return;}

    quint32 groupID = action->data().toUInt();
    InterestGroup *group = m_contactsManager->getInterestGroup(groupID);
    if(!group){return;}

    int ret = QMessageBox::question(this, tr("Question"),
                                    tr("All history messages from this group will be deleted! <p>Do you really want to delete the group '%1(%2)' from local ? </p>").arg(group->getGroupName()).arg(groupID),
                                    QMessageBox::Yes|QMessageBox::No,
                                    QMessageBox::No);

    if(ret == QMessageBox::No){return;}

    m_contactsManager->removeInterestGroupFromLocalDB(groupID);
    deleteInterestGroupFromUI(group);

}

void MainWindow::joinInterestGroup(quint32 groupID, const QString &verificationMessage){
    clientPacketsParser->joinOrQuitInterestGroup(m_socketConnectedToServer, groupID, true, verificationMessage);
}

void MainWindow::slotQuitInterestGroup(){

    QAction *action = qobject_cast<QAction *>(sender());
    if(!action){return;}

    quint32 groupID = action->data().toUInt();
    InterestGroup *group = m_contactsManager->getInterestGroup(groupID);
    if(!group){return;}

    int ret = QMessageBox::question(this, tr("Quit Group"),
                                    tr("<p>Do you really want to quit the group '%1(%2)' ? </p>").arg(group->getGroupName()).arg(groupID),
                                    QMessageBox::Yes|QMessageBox::No,
                                    QMessageBox::No);

    if(ret == QMessageBox::No){return;}

    clientPacketsParser->joinOrQuitInterestGroup(m_socketConnectedToServer, groupID, false);
}

void MainWindow::interestGroupItemActivated(QListWidgetItem * item ){

    QVariant data = item->data(Qt::UserRole);
    quint32 interestGroupID = data.toUInt();
//    InterestGroup *group = m_contactsManager->getInterestGroup(interestGroupID);
//    if(!group){
//        qCritical()<<"Error:No such interest group:"<<interestGroupID;
//        return;
//    }
//    item->setText(group->getGroupName());


    m_chatWindowManager->slotNewChatWithInterestGroup(interestGroupID);
    systemTray->removeAllTrayIconData(data.toString());
}


void MainWindow::showUserInfo(IMUserBase *user){

    Q_ASSERT_X(user, "MainWindow::showUserInfo(...)", "Invalid IMUserBase!");

    ContactInfoWidget *  m_ContactInfoWidget = ContactInfoWidget::getContactInfoWidget(user);
    if(!m_ContactInfoWidget){
        m_ContactInfoWidget = new ContactInfoWidget(user);
    }
    m_ContactInfoWidget->show();
    m_ContactInfoWidget->raise();


}

void MainWindow::requestLogin(const QHostAddress &serverHostAddress, quint16 serverPort){

    Q_ASSERT(m_rtp);


    QString errorMessage;
    if(m_socketConnectedToServer == INVALID_SOCK_ID){
        m_socketConnectedToServer = m_rtp->connectToHost(QHostAddress(serverHostAddress), serverPort, 10000, &errorMessage);
    }
    if(m_socketConnectedToServer == INVALID_SOCK_ID){
        qCritical()<<tr("Can not connect to host! %1").arg(errorMessage);

        ui.loginPage->slotProcessLoginResult(IM::ERROR_ConnectionFailed);
        return;
    }

    m_serverHostAddress = serverHostAddress;
    m_serverHostPort = serverPort;

    //    peerConnected(m_socketConnectedToServer, serverHostAddress.toString(), serverPort);

    clientPacketsParser->requestLogin(m_socketConnectedToServer);

//    QTimer::singleShot(10000, this, SLOT(loginTimeout()));

    if(!m_loginTimer){
        m_loginTimer = new QTimer(this);
        m_loginTimer->setSingleShot(true);
        connect(m_loginTimer, SIGNAL(timeout()), this, SLOT(loginTimeout()));
    }
    m_loginTimer->start(10000);

}

void MainWindow::slotProcessKickedOff(){

    //ui.stackedWidget->setCurrentWidget(ui.mainPage);
    offline();

    QString text = tr("You have been disconnected as someone has signed in with your ID on another computer!");
    QString informativeText = "";
    QMessageBox msgBox(this);
    QPushButton *signinAgainButton = msgBox.addButton(tr("&Sign in again"), QMessageBox::ActionRole);
    QPushButton *okButton = msgBox.addButton(QMessageBox::Ok);
    msgBox.setDefaultButton(okButton);
    msgBox.setWindowTitle(tr("Offline Alert"));
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText(text);
    msgBox.setInformativeText(informativeText);
    //msgBox.setDetailedText(detailMessage);
    msgBox.exec();

    if(msgBox.clickedButton() == signinAgainButton){
        requestLogin(m_serverHostAddress, m_serverHostPort);
    }else if(msgBox.clickedButton() == okButton){

    }

}

void MainWindow::loginTimeout(){

    destoryLoginTimer();

    if(!m_verified){
        ui.loginPage->slotProcessLoginResult(IM::ERROR_Timeout);
    }

}

void MainWindow::destoryLoginTimer(){
    if(!m_loginTimer){
       return;
    }

    m_loginTimer->stop();
    delete m_loginTimer;
    m_loginTimer = 0;

}

void MainWindow::requestRegistrationServerInfo(){
    qDebug()<<"--MainWindow::requestRegistrationServerInfo()";

    int socketID = INVALID_SOCK_ID;

    QHostAddress svrAddress = QHostAddress(m_myself->getLoginServerAddress());
    quint16 svrPort = m_myself->getLoginServerPort();

    if(svrAddress == m_serverHostAddress && svrPort == m_serverHostPort && m_rtp->isSocketConnected(m_socketConnectedToServer)){
        socketID = m_socketConnectedToServer;
    }else{
        QString errorMessage;
        socketID = m_rtp->connectToHost(svrAddress, svrPort, 10000, &errorMessage);
        if(socketID == INVALID_SOCK_ID){
            QMessageBox::critical(this, tr("Error"), tr("Failed to contact server! <br>%1").arg(errorMessage));
            return;
        }
        m_rtp->closeSocket(m_socketConnectedToServer);
    }

    m_serverHostAddress = svrAddress;
    m_serverHostPort = svrPort;
    m_socketConnectedToServer = socketID;


    bool ok = clientPacketsParser->requestRegistrationServerInfo(m_socketConnectedToServer);
    if(!ok){
        QMessageBox::critical(this, tr("Error"), tr("Failed to send request! <br>%1").arg(m_rtp->lastErrorString()));
    }

}

void MainWindow::requestRegistration(){

    //TODO
    int socketID = INVALID_SOCK_ID;
    QHostAddress svrAddress = QHostAddress::Null;
    quint32 svrPort = 0;

    QStringList serverAddressInfo = m_myself->getRegistrationServerAddressInfo().split(":");
    if(serverAddressInfo.size() == 2){
        svrAddress = QHostAddress(serverAddressInfo.at(0));
        svrPort = serverAddressInfo.at(1).toUInt();
    }else{
        svrAddress = QHostAddress(m_myself->getLoginServerAddress());
        svrPort = m_myself->getLoginServerPort();
    }

    if(QHostAddress(svrAddress) == m_serverHostAddress && svrPort == m_serverHostPort && m_rtp->isSocketConnected(m_socketConnectedToServer)){
        socketID = m_socketConnectedToServer;
    }else{
        QString errorMessage;
        socketID = m_rtp->connectToHost(svrAddress, svrPort, 10000, &errorMessage);
        if(socketID == INVALID_SOCK_ID){
            QMessageBox::critical(this, tr("Error"), tr("Failed to register! <br>%1").arg(errorMessage));
            return;
        }
    }

    bool ok = clientPacketsParser->registration(socketID, m_myUserID, m_myself->getPassword());
    if(!ok){
        QMessageBox::critical(this, tr("Error"), tr("Failed to send registration request! <br>%1").arg(m_rtp->lastErrorString()));
    }

}

void MainWindow::offline(){
    qDebug()<<"--MainWindow::offline()";

    m_myself->setOnlineState(IM::ONLINESTATE_OFFLINE);

    m_rtp->closeSocket(m_socketConnectedToServer);
    m_socketConnectedToServer = INVALID_SOCK_ID;
    m_verified = false;
    systemTray->resetTrayIcon(ImageResource::createIconForContact((QString(RESOURCE_PATH)+QString(APP_ICON_PATH)), IM::ONLINESTATE_OFFLINE));

    emit signalServerOnlineStateChanged(false);

}

void MainWindow::peerConnected(int socketID, const QString &peerAddress, quint16 peerPort){
    qWarning()<<QString("Connected! "+peerAddress+":"+QString::number(peerPort));

    if(QHostAddress(peerAddress) == m_serverHostAddress && peerPort == m_serverHostPort){
        //clientPacketsParser->requestLogin(m_socketConnectedToServer);
        m_socketConnectedToServer = socketID;

        emit signalServerOnlineStateChanged(true);
    }else{

    }

}

void MainWindow::signalConnectToPeerTimeout(const QHostAddress &peerAddress, quint16 peerPort){
    qCritical()<<QString("Connecting Timeout! "+peerAddress.toString()+":"+QString::number(peerPort));

}

void MainWindow::peerDisconnected(const QHostAddress &peerAddress, quint16 peerPort, bool normalClose){
    qDebug()<<QString("Disconnected! "+peerAddress.toString()+":"+QString::number(peerPort));

    if(!normalClose){
        qCritical()<<QString("ERROR! Peer %1:%2 Closed Unexpectedly!").arg(peerAddress.toString()).arg(peerPort);
    }

    if(peerAddress == m_serverHostAddress && peerPort == m_serverHostPort){
        //TODO
//        m_verified = false;
//        emit signalServerOnlineStateChanged(false);
//        systemTray->resetTrayIcon(ImageResource::createIconForContact((QString(RESOURCE_PATH)+QString(APP_ICON_PATH)), IM::ONLINESTATE_OFFLINE));
        offline();

        if(!normalClose){
            requestLogin(m_serverHostAddress, m_serverHostPort);
        }
    }

}

void MainWindow::peerDisconnected(int socketID){
    qWarning()<<"Peer Disconnected! Socket ID:"<<socketID;

    if(socketID == m_socketConnectedToServer){
        //m_socketConnectedToServer = INVALID_SOCK_ID;
        //QMessageBox::critical(this, tr("Error"), tr("Disconnected from server!"));
        offline();
        return;
    }

    m_contactSocketsHash.remove(socketID);


}

//void MainWindow::processFileDataRequestPacket(int socketID, const QString &contactID, const QByteArray &fileMD5, int startPieceIndex, int endPieceIndex){

//    Q_ASSERT(m_fileManager);

//    if( (startPieceIndex == -1) && (endPieceIndex == -1) ){
//        QList<int> completedPieces = m_fileManager->completedPieces(fileMD5);
//        foreach (int pieceIndex, completedPieces) {
//            fileTXRequestHash.insert(m_fileManager->readPiece(fileMD5, pieceIndex), socketID);
//            //QCoreApplication::processEvents();
//        }

//    }else{
//        Q_ASSERT(endPieceIndex >= startPieceIndex);
//        for(int i=startPieceIndex; i<=endPieceIndex; i++){
//            fileTXRequestHash.insert(m_fileManager->readPiece(fileMD5, i), socketID);
//            //QCoreApplication::processEvents();
//        }

//    }

//    //    int id = m_fileManager->readPiece(fileMD5, pieceIndex);
//    //    fileTXRequestHash.insert(id, socketID);

//}

//void MainWindow::processFileDataReceivedPacket(int socketID, const QString &contactID, const QByteArray &fileMD5, int pieceIndex, const QByteArray &data, const QByteArray &sha1){

//    Q_ASSERT(m_fileManager);
//    m_fileManager->writePiece(fileMD5, pieceIndex, data, sha1);


//    //        clientPacketsParser->requestFileData(socketID, fileTXWithAdmin->pos(), FILE_PIECE_LENGTH);


//}

//void MainWindow::processFileTXStatusChangedPacket(int socketID, const QString &contactID, const QByteArray &fileMD5, quint8 status){

//    //IM::FileTXStatus status = IM::FileTXStatus(status);
//    switch(status){
//    case quint8(IM::File_TX_Preparing):
//    {

//    }
//        break;
//    case quint8(IM::File_TX_Receiving):
//    {

//    }
//        break;
//    case quint8(IM::File_TX_Sending):
//    {

//    }
//        break;
//    case quint8(IM::File_TX_Progress):
//    {

//    }
//        break;
//    case quint8(IM::File_TX_Paused):
//    {

//    }
//        break;
//    case quint8(IM::File_TX_Aborted):
//    {
//        QList<int> sockets = fileTXSocketHash.keys(fileMD5);
//        if(sockets.contains(socketID) && sockets.size() <= 1){
//            m_fileManager->closeFile(fileMD5);
//        }
//    }
//        break;
//    case quint8(IM::File_TX_Done):
//    {
//        QList<int> sockets = fileTXSocketHash.keys(fileMD5);
//        if(sockets.contains(socketID) && sockets.size() <= 1){
//            m_fileManager->closeFile(fileMD5);
//        }

//    }
//        break;
//    default:
//        break;
//    }

//}

//void MainWindow::processFileTXErrorFromPeer(int socketID, const QString &contactID, const QByteArray &fileMD5, quint8 errorCode, const QString &errorString){
//    qDebug()<<"--MainWindow::processFileTXErrorFromPeer(...) " <<" socketID:"<<socketID;
//    qCritical()<<errorString;

//}

//void MainWindow::fileDataRead(int requestID, const QByteArray &fileMD5, int pieceIndex, const QByteArray &data, const QByteArray &dataSHA1SUM){
//    qDebug()<<"--MainWindow::fileDataRead(...) "<<" pieceIndex:"<<pieceIndex<<" size:"<<data.size();

//    int socketID = fileTXRequestHash.take(requestID);
//    Contact *contact = m_contactSocketsHash.value(socketID);
//    if(!contact){return;}

//    clientPacketsParser->sendFileData(socketID, contact->getUserID(), fileMD5, pieceIndex, &data, &dataSHA1SUM);

//}

//void MainWindow::fileTXError(int requestID, const QByteArray &fileMD5, quint8 errorCode, const QString &errorString){
//    qCritical()<<errorString;

//    if(requestID){
//        int socketID = fileTXRequestHash.take(requestID);
//        Contact *contact = m_contactSocketsHash.value(socketID);
//        if(!contact){return;}

//        clientPacketsParser->fileTXError(socketID, contact->getUserID(), fileMD5, errorCode, errorString);
//    }else{
//        //TODO:
//    }


//}

//void MainWindow::pieceVerified(const QByteArray &fileMD5, int pieceIndex, bool verified, int verificationProgress){
//    qDebug()<<"--MainWindow::pieceVerified(...) "<<" pieceIndex:"<<pieceIndex<<" verified:"<<verified<< "verificationProgress:"<<verificationProgress;

//    QList<int> sockets = fileTXSocketHash.keys(fileMD5);
//    if(sockets.isEmpty()){
//        //TODO:
//        //m_fileManager->closeFile(fileMD5);
//    }

//    if(verified){

//        if(verificationProgress == 100){
//            qWarning()<<"Done!";
//            foreach (int socketID, sockets) {
//                Contact *contact = m_contactSocketsHash.value(socketID);
//                if(!contact){continue;}
//                clientPacketsParser->fileTXStatusChanged(socketID, contact->getUserID(), fileMD5, quint8(IM::File_TX_Done));
//            }

//        }else{
//            //TODO:
//            //            int uncompletedPieceIndex = m_fileManager->getOneUncompletedPiece(fileMD5);
//            //            qDebug()<<"uncompletedPieceIndex:"<<uncompletedPieceIndex;
//            //            if(uncompletedPieceIndex < 0){
//            //                return;
//            //            }
//            //            clientPacketsParser->requestFileData(sockets.first(), fileMD5, uncompletedPieceIndex);


//            //if((pieceIndex % FILE_PIECES_IN_ONE_REQUEST) == 0){
//            //    qDebug()<<"----0----pieceIndex:"<<pieceIndex;
//            //    clientPacketsParser->requestFileData(sockets.first(), fileMD5, pieceIndex + 1, pieceIndex + FILE_PIECES_IN_ONE_REQUEST);
//            //}

//            if((pieceIndex % FILE_PIECES_IN_ONE_REQUEST) == 0){
//                //TODO:P2P
//                int socketID = sockets.first();
//                Contact *contact = m_contactSocketsHash.value(socketID);
//                if(!contact){return;}

//                if(pieceIndex == 0 ){
//                    clientPacketsParser->requestFileData(socketID, contact->getUserID(), fileMD5, 1, 2 * FILE_PIECES_IN_ONE_REQUEST);
//                }else{
//                    clientPacketsParser->requestFileData(socketID, contact->getUserID(), fileMD5, pieceIndex + FILE_PIECES_IN_ONE_REQUEST + 1, pieceIndex + 2 * FILE_PIECES_IN_ONE_REQUEST);
//                }
//            }

//        }


//    }else{
//        //TODO:P2P
//        int socketID = sockets.first();
//        Contact *contact = m_contactSocketsHash.value(socketID);
//        if(!contact){return;}
//        qCritical()<<"ERROR! Verification Failed! Piece:"<<pieceIndex;
//        clientPacketsParser->requestFileData(socketID, contact->getUserID(), fileMD5, pieceIndex, pieceIndex);
//    }

//}



//void MainWindow::slotChangeContactOnlineState(const QString &contactID, quint8 onlineState, const QString &peerAddress, quint16 peerPort, const QString &greetingInfo){

//	qDebug()<<"----MainWindow::slotChangeContactOnlineState(...)";
//	qDebug()<<"----MainWindow::slotChangeContactOnlineState(...)~~contactID:"<<contactID;
//	qDebug()<<"----MainWindow::slotChangeContactOnlineState(...)~~greetingInfo:"<<greetingInfo;


//	contactsManager->slotChangeContactOnlineState(contactID, onlineState, peerAddress, peerPort, greetingInfo);

//        Contact *contact = contactsManager->getUser(contactID);
//        int groupID = contact->getContactGroupID();
//	ContactGroup *group = contactsManager->getContactGroup(groupID);
//	QString groupName = group->getGroupName();
//        if(onlineState == quint8(IM::OFFLINE) || onlineState == quint8(IM::INVISIBLE)){
//			friendsListView->updateObjectItemIcon(groupName, contactID, ImageResource::createIconForContact("", false));
//			//QMessageBox::information(this, "Offline", QString("IMUser %1 Offline").arg(contactID));
//			qDebug()<<"----MainWindow::slotChangeContactOnlineState(...)~~"<<QString("IMUser %1 Offline").arg(contactID);
//	}else{
//			friendsListView->updateObjectItemIcon(groupName, contactID, ImageResource::createIconForContact("", true));
//			//QMessageBox::information(this, "Online", QString("IMUser %1 Online").arg(contactID));
//			qDebug()<<"----MainWindow::slotChangeContactOnlineState(...)~~"<<QString("IMUser %1 Online").arg(contactID);
//	}
//	//friendsListView->updateObjectItemIcon(groupName, contactID, ":/resources/images/app.png");

//}

//void MainWindow::slotUpdateContactInfo(const QString &contactID, quint8 onlineState, const QString &peerAddress, quint16 peerPort, const QString &info){

//	qDebug()<<"----MainWindow::slotUpdateContactInfo(...)";
//	qDebug()<<"----MainWindow::slotUpdateContactInfo(...)~~contactID:"<<contactID;
//	qDebug()<<"----MainWindow::slotUpdateContactInfo(...)~~info:"<<info;

//	slotChangeContactOnlineState(contactID, onlineState, peerAddress, peerPort, info);

//	//TODO:更新其它信息

//}















} //namespace HEHUI





