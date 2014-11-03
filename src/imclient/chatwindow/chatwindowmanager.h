#ifndef CHATWINDOWMANAGER_H
#define CHATWINDOWMANAGER_H

#include <QMainWindow>
#include <QList>
#include <QPair>
#include <QCloseEvent>


#include "ui_chatwindowmanager.h"


#include "../contactsmanager/contact.h"
//#include "chatmessagewindow.h"
#include "groupchatwindow.h"
#include "contactchatwidget.h"


//#include "../../sharedim/filetransmitter/filetransmissionmanager.h"
#include "filetransmitter/clientfiletransmissionmanager.h"

//#include "../../shared/core/singleton.h"
#include "HHSharedCore/hsingleton.h"



namespace HEHUI {

class ChatWindowManager : public QMainWindow, public Singleton<ChatWindowManager>
{
    Q_OBJECT
    friend class Singleton<ChatWindowManager>;

public:
    enum ChatWindowDisplayStyle{TabbedChatWindow = 0, MDIChatWindow, SeparatedChatWindow};
    ChatWindowManager(QWidget *parent = 0);
    ~ChatWindowManager();

protected:
    void closeEvent(QCloseEvent *);


public:
    void setChatWindowDisplayStyle(ChatWindowDisplayStyle style);
    ChatWindowDisplayStyle getChatWindowDisplayStyle();
    void switchChatWindowDisplayStyle(ChatWindowDisplayStyle style);

    bool isContactChatWindowOpen(const QString &contactID);
    bool isInterestGroupChatWindowOpen(int groupID);

    bool closeContactChatWindow(Contact *contact);
    bool closeInterestGroupChatWindow(InterestGroup *group);

    void interestGroupMemberJoinedOrQuitted(quint32 groupID, const QString &memberID, bool join);

    void contactOnlineStateChanged(Contact *contact);


signals:
    void signalSendChatMessageToCantact(Contact *contact, const QString &message, const QStringList &imageList);
    void signalSendChatMessageToInterestGroup(InterestGroup *interestGroup, const QString &message, const QStringList &imageList);
    void signalRequestDownloadImage(const QString &contactID, const QString &imageName);

    void signalRequestContactHistoryMessage(const QString &startTime, const QString &endTime, const QString &content, bool requestBackword, const QString &contactID);
    void signalRequestGrouptHistoryMessage(const QString &startTime, const QString &endTime, const QString &content, bool requestBackword, quint32 groupID);


    //FILE TX
    void signalRequestFileServerInfo();
    void signalSendUploadingFileRequest(Contact *contact, const QString &filePath, const QByteArray &fileMD5);
    void signalCancelSendingFileUploadingRequest(Contact *contact, const QByteArray &fileMD5);
    //void signalAbortFileTransmission(const QString &fileMD5);
    void signalAcceptPeerUploadFileRequest(Contact *contact, const QByteArray &fileMD5, const QString &localSavePath);
    void signalDeclinePeerUploadFileRequest(Contact *contact, const QByteArray &fileMD5);

//    void signalCancelSendingFileRequest(const QString &fileMD5);
//    void signalAbortFileTransmission(const QString &fileMD5);
//    void signalAcceptFileRequest(const QString &fileMD5, const QString &localSavePath);
//    void signalDeclineFileRequest(const QString &fileMD5);

public slots:
    void slotNewChatWithContact(const QString &contactID);
    void slotNewMessageReceivedFromContact(const QString &contactID, const QString &message, const QString &time);

    void slotNewChatWithInterestGroup(quint32 interestGroupID);
    void slotNewMessageReceivedFromInterestGroup(quint32 interestGroupID, const QString &contactID, const QString &message, const QString &time);

    void processImageDownloadResult(const QString &contactID, const QString &imageName, bool downloaded);

    void processContactHistoryMessage(const QStringList &messages, bool canFetchMore, const QString &contactID);
    void processGrouptHistoryMessage(const QStringList &messages, bool canFetchMore, quint32 groupID);

    void contactRequestUploadFile(const QString &contactID, const QByteArray &fileMD5Sum, const QString &fileName, quint64 size);
    void fileUploadRequestResponsed(const QString &contactID, const QByteArray &fileMD5Sum, bool accepted, const QString &message);

    void contactRequestDownloadFile(const QString &contactID, const QString &localBaseDir, const QString &fileName);
    void fileDownloadRequestAccepted(const QString &contactID, const QString &remoteFileName, const QByteArray &fileMD5Sum, quint64 size);
    void fileDownloadRequestDenied(const QString &contactID, const QString &remoteFileName, const QString &message);

private slots:
    void initTabWidget();
    void slotTabPageChanged();
    void slotNewTab();
    void slotcloseTab();

    void handleCloseWindowRequest();
    void handleChatWindowClosed();
    void showContextMenu(const QPoint &pos);
    void handleContactChatHistoryMenuAction();
    void handleGroupChatHistoryMenuAction();


    void switchToSubWindowView();
    void switchToTabbedView();
    void switchToSeparatedView();

    //File TX
    void sendUploadingFileRequest(const QString &filePath, const QByteArray &fileMD5, bool offline);
    void cancelSendingFileRequest(const QByteArray &fileMD5);
    void abortFileTransmission(const QByteArray &fileMD5);
    void acceptPeerUploadFileRequest(const QByteArray &fileMD5, const QString &localSavePath);
    void declineFileRequest(const QByteArray &fileMD5);



private:
    //	bool isChatWindowOpened(Contact *contact);

    ContactChatWidget * createContactChatWindow(Contact *contact);
    QMdiSubWindow* findChatWithContactMdiSubWindow(Contact *contact);
    ContactChatWidget * findContactChatTabWidget(Contact *contact);

    GroupChatWindow* createInterestGroupChatWindow(InterestGroup *group);
    QMdiSubWindow* findChatWithInterestGroupMdiSubWindow(InterestGroup *group);
    GroupChatWindow * findInterestGroupChatTabWidget(InterestGroup *group);

    QMenu * chatHistoryMenu();

    bool initFileTransmission();


private:
    Ui::ChatWindowManagerClass ui;


    ChatWindowDisplayStyle m_chatWindowDisplayStyle;

    QHash<QString/*Contact ID*/, ContactChatWidget *> m_contactChatWidgetHash;
    QHash<quint32/*Group ID*/, GroupChatWindow *> m_groupChatWidgetHash;

    QStringList m_contactChatHistoryList;
    QList<quint32> m_groupChatHistoryList;

//    QSize m_preferedSize;


    ClientFileTransmissionManager *m_fileTransmissionManager;
    ClientFileTransmissionPacketsParser *m_fileTransmissionPacketsParser;
    int m_socketConnectedToServer;
//    QHostAddress serverAddress;
//    quint16 serverPort;


};

} //namespace HEHUI


#endif // CHATWINDOWMANAGER_H
