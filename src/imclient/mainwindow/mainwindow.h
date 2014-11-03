#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTranslator>


#include "ui_mainwindow.h"


#include "contactsmanager/contactsmanager.h"
#include "../chatwindow/chatwindowmanager.h"
#include "../networkmanager/clientresourcesmanager.h"
#include "../packetmanager/imclientpacketsparser.h"
#include "../search/search.h"

#include "../../sharedim/constants_global_shared.h"

#include "informationtips/userinfotipwindow.h"
#include "contactinfowidget/contactinfowidget.h"
#include "deletecontactdialog/deletecontactdialog.h"
#include "../contactbox/contactbox.h"

#include "HHSharedCore/hdatabaseutility.h"
#include "HHSharedGUI/hmainwindowbase.h"
#include "HHSharedGUI/hsystemtrayiconbase.h"





namespace HEHUI {


class MainWindow: public MainWindowBase {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0, HEHUI::WindowPosition positon = HEHUI::Center);
    ~MainWindow();


    QSystemTrayIcon *SystemTrayIcon();

    void retranslateUi();

    void setTrayIconVisible(bool visible);


protected:
    void closeEvent(QCloseEvent * event);
    void changeEvent(QEvent * event);


signals:
    void signalMyOnlineStateChanged(int serverSocketID, quint8 onlineStateCode);
    void signalContactOnlineStateChanged(const QString &contactID, quint8 onlineStateCode);
    void signalServerOnlineStateChanged(bool online);


public slots:
    void slotIconActivated(QSystemTrayIcon::ActivationReason reason);

    void slotSystemConfig();
    void slotBugReport();
    void slotHelp();
    void slotAbout();

    void slotQuit();

    void slotUpdateContactsInfo2();
//    void slotUpdateBlacklistInfo();



private slots:
    void slotInitPlugin(AbstractPluginInterface *plugin);
    void slotUserVerified();
    void slotLockUI();


    void handleContextMenuEventOnContactGroup(ContactGroupBase *contactGroup, const QPoint &global_mouse_pos);
    void handleContextMenuEventOnContact(Contact *contact, const QPoint &global_mouse_pos);
    void requestChatWithContact(Contact *contact);
    void handleTooltipEventOnContactItem(Contact *contact, const QPoint &global_item_topLeft_pos, const QPoint &global_mouse_pos);

    void showProgressDialog(const QString &labelText = "", const QString & cancelButtonText = "", int minimum = 0, int maximum = 0);
    void hideProgressDialog();



    void slotMoveContactToGroup();
//    void slotMoveContactToBlacklist();
    void slotRequestDeleteContact(const QString &contactID, bool deleteMeFromOpposition = false, bool addToBlacklist = false);
    void slotDeleteContactResultReceived(const QString &contactID, bool contactDeleted = false, bool addToBlacklist = false);

    void slotProcessUpdatePasswordResult(quint8 errorTypeCode, const QString &message);

    void slotProcessLoginServerRedirected(const QString &serverAddress, quint16 serverPort, const QString &serverName);
    void slotProcessLoginResult(quint8 errorTypeCode, const QString &errorMessage);
    void slotProcessClientLastLoginInfo(const QString &extIPAddress, const QString &loginTime, const QString &LogoutTime, const QString &deviceInfo);

    void slotProcessContactStateChanged(const QString &contactID, quint8 onlineStateCode, const QString &contactHostAddress, quint16 contactHostPort);
    void slotProcessContactsOnlineInfo(const QString &contactsOnlineInfoString);
    void slotProcessUserInfo(const QString &userID/*, const QString &userInfo*/);
    //IMPORTANT
    void slotProcessContactGroupsInfo(const QString &contactGroupsInfo, quint32 personalContactGroupsInfoVersionOnServer);
    void slotProcessContactGroupsInfo2(const QString &contactGroupsInfo, quint32 personalContactGroupsInfoVersionOnServer);

    void slotProcessContactsInfoVersion(const QString &contactsInfoVersionString, quint32 contactGroupsInfoVersionOnServer);
    void slotProcessCreateOrDeleteContactGroupResult(quint32 groupID, const QString &groupName, bool createGroup, bool result);


    //void slotProcessSearchContactsResult(const QString &users);
    void slotProcessAddContactResult(const QString &contactID, const QString &userNickName, const QString &userFace, int contactGroupID, quint8 errorTypeCode, const QString &reasonMessage, quint8 onlineStateCode);
    void getNewContactSettings(const QString &contactID);
    void slotProcessPersonalMessage(const QString &userID, const QString &message);

    void slotSearch();
    void searchContact(const QString &propertiesString, bool matchExactly, bool searchOnlineUsersOnly, bool searchWebcamUsersOnly, int startIndex);
    void searchInterestGroup(const QString &keyword, int startIndex = 0);

    void addContact(const QString &userID, const QString &verificationMessage);


    void slotProcessContactRequestFromUser(const QString &userID, const QString &userNickName, const QString &userFace, const QString &verificationMessage);
    void showContactRequestFromUser(const QString &userID, const QString &userNickName, const QString &userFace, const QString &verificationMessage);

    void slotProcessChatMessageReceivedFromContact(const QString &contactID, const QString &message, const QString &time);
    void slotProcessChatMessageCachedOnServer(const QStringList &messages);
    void slotProcessImageDownloadResult(const QString &contactID, const QString &imageName, const QByteArray &image);
    void slotProcessImageDownloadRequest(const QString &contactID, const QString &imageName);
    void requestDownloadImage(const QString &contactID, const QString &imageName);


    void slotProcessInterestGroupChatMessagesReceivedFromContact(quint32 interestGroupID, const QString &contactID, const QString &message, const QString &time);
    void slotProcessInterestGroupChatMessagesCachedOnServer(const QStringList &messages);

    void slotSendChatMessageToContact(Contact *contact, const QString &message, const QStringList &imageNameList);
    void slotSendChatMessageToInterestGroup(InterestGroup *interestGroup, const QString &message, const QStringList &imageList);


    void slotProcessInterestGroupsList(const QString &interestGroupsListFromServer, quint32 interestGroupsInfoVersionOnServer);
    void slotProcessInterestGroupInfo(const QString &interestGroupInfoFromServer, quint32 groupID);
    void slotProcessInterestGroupMembersInfo(const QString &interestGroupMembersInfoFromServer, quint32 interestGroupMembersInfoVersionOnServer, quint32 groupID);
    void slotProcessCreateInterestGroupResult(quint32 groupID, const QString &groupName);
    void slotProcessDisbandInterestGroupResult(quint32 groupID, bool result);
    void slotProcessUserRequestJoinInterestGroup(quint32 groupID, const QString &verificationMessage, const QString &userID, const QString &nickName, const QString &face);
    void slotProcessUserJoinOrQuitInterestGroup(quint32 groupID, const QString &memberID, bool join);


    void handleContextMenuEventOnInterestGroupList(const QPoint &point);
    void slotCreateInterestGroup();
    void slotDisbandInterestGroup();
    void slotDeleteInterestGroupFromLocal();
    void joinInterestGroup(quint32 groupID, const QString &verificationMessage);
    void slotQuitInterestGroup();

    void interestGroupItemActivated(QListWidgetItem * item );
    void updateAllInterestGroupsInfoToUI();
    void addInterestGroupToUI(InterestGroup *interestGroup);
    void deleteInterestGroupFromUI(InterestGroup *interestGroup);
    void updateInterestGroupInfoToUI(InterestGroup *interestGroup);


    //Recent Chats
    void setupRecentChats(const QStringList &contacts, const QList<quint32> &interestGroups);
    void addOrRemoveRecentChatContact(Contact *contact, bool add = true);
    void addOrRemoveRecentChatGroup(InterestGroup *interestGroup, bool add = true);

    //History Message
    void getContactHistoryMessage(const QString &startTime, const QString &endTime, const QString &content, bool requestBackword, const QString &contactID);
    void getGrouptHistoryMessage(const QString &startTime, const QString &endTime, const QString &content, bool requestBackword, quint32 groupID);





    void on_toolButtonUserFace_clicked(){showUserInfo(m_myself);}
    void showUserInfo(IMUserBase *user);

    void requestLogin(const QHostAddress &serverHostAddress, quint16 serverPort);
    void slotProcessKickedOff();
    void loginTimeout();
    void destoryLoginTimer();

    void requestRegistrationServerInfo();
    void requestRegistration();

    void offline();

    void peerConnected(int socketID, const QString &peerAddress, quint16 peerPort);
    void signalConnectToPeerTimeout(const QHostAddress &peerAddress, quint16 peerPort);
    void peerDisconnected(const QHostAddress &peerAddress, quint16 peerPort, bool normalClose);
    void peerDisconnected(int socketID);

//////////////////////////////
    //FILE TX
    void slotRequestFileServerInfo();
    void slotSendUploadingFileRequest(Contact *contact, const QString &filePath, const QByteArray &fileMD5);
    void slotCancelSendingFileRequest(Contact *contact, const QByteArray &fileMD5);
    void slotAcceptPeerUploadFileRequest(Contact *contact, const QByteArray &fileMD5, const QString &localSavePath);
    void slotDeclinePeerUploadFileRequest(Contact *contact, const QByteArray &fileMD5);


    //File TX


//    void processFileDataRequestPacket(int socketID, const QString &contactID, const QByteArray &fileMD5, int startPieceIndex, int endPieceIndex);
//    void processFileDataReceivedPacket(int socketID, const QString &contactID, const QByteArray &fileMD5, int pieceIndex, const QByteArray &data, const QByteArray &sha1);
//    void processFileTXStatusChangedPacket(int socketID, const QString &contactID, const QByteArray &fileMD5, quint8 status);
//    void processFileTXErrorFromPeer(int socketID, const QString &contactID, const QByteArray &fileMD5, quint8 errorCode, const QString &errorString);

//    void fileDataRead(int requestID, const QByteArray &fileMD5, int pieceIndex, const QByteArray &data, const QByteArray &dataSHA1SUM);
//    void fileTXError(int requestID, const QByteArray &fileMD5, quint8 errorCode, const QString &errorString);
//    void pieceVerified(const QByteArray &fileMD5, int pieceIndex, bool verified, int verificationProgress);


private:

    void setupSystemTray();

    void initUI();

    void checkNetwork();
    void startNetwork();
    void stopNetwork();

    //退出前的清理工作
    void aboutToQuit();

    void savePreferedStyle(const QString &preferedStyle, bool useStylePalette);
    void savePreferedLanguage(const QString &preferedLanguage);

    void showDeleteContactDialog(Contact *contact, bool blacklistMode = false);


private:
    Ui::MainWindowUIClass ui;


    SystemTrayIconBase *systemTray;
    QMenu *trayMenu;
    QMenu *pluginsMenu;

    ChatWindowManager *m_chatWindowManager;


    ContactBox  *m_contactBox;

    QTreeWidgetItem *m_recentChatGroupsItem;
    QTreeWidgetItem *m_recentChatFriendsItem;
    QTreeWidgetItem *m_recentChatStrangersItem;


    UserInfoTipWindow *m_userInfoTipWindow;
    //ContactInfoWidget *m_ContactInfoWidget;

    QProgressDialog *progressDialog;

    ContactsManager *m_contactsManager;
    IMUser *m_myself;
    QString m_myUserID;
    QString m_systemID;
    IM::OnlineState stateBeforeLocking;

    //PacketHandlerBase *m_packetHandler;
    ClientResourcesManager *m_resourcesManager;
    IMClientPacketsParser *clientPacketsParser;
    bool networkStarted;


    UDPServer *m_udpServer;
    RTP *m_rtp;
    int m_socketConnectedToServer;
    QHash<int/*Socket ID*/, Contact*/*Contact*/> m_contactSocketsHash;


    Search *search;
    DeleteContactDialog *m_deleteContactDialog;

    bool autoShowSystemMessage;
    bool autoShowChatMessageFromContact;



    QHostAddress m_serverHostAddress;
    quint16 m_serverHostPort;
    bool m_verified;
    QTimer *m_loginTimer;

//    FileManager *m_fileManager;
//    QHash<int/*File TX Request ID*/, int/*Socket ID*/> fileTXRequestHash;
//    QMultiHash<int/*Socket ID*/, QByteArray/*File MD5*/> fileTXSocketHash;


};

} //namespace HEHUI

#endif // MAINWINDOW_H
