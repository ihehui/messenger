/*
 ****************************************************************************
 * imclientpacketsparser.h
 *
 * Created On: 2010-7-13
 *     Author: 贺辉
 *    License: LGPL
 *    Comment:
 *
 *
 *    =============================  Usage  =============================
 *|
 *|
 *    ===================================================================
 *
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 ****************************************************************************
 */

/*
 ***************************************************************************
 * Last Modified On: 2012-3-4
 * Last Modified By: 贺辉
 ***************************************************************************
 */

#ifndef IMCLIENTPACKETSPARSER_H_
#define IMCLIENTPACKETSPARSER_H_



#include <QHostAddress>
#include <QHostInfo>
#include <QDebug>
#include <QFile>


#include "../sharedim/constants_global_shared.h"
#include "../sharedim/impackets.h"

#include "../networkmanager/clientresourcesmanager.h"
#include "imuser.h"


#include "HHSharedCore/Cryptography"
#include "HHSharedNetwork/NetworkUtilities"



namespace HEHUI
{


class IMClientPacketsParser : public QObject
{
    Q_OBJECT
public:
    IMClientPacketsParser(ClientResourcesManager *resourcesManager, QObject *parent = 0);
    virtual ~IMClientPacketsParser();



    //    void startHeartbeat(int interval = HEARTBEAT_TIMER_INTERVAL);
    //    void stopHeartbeat();


    int socketConnectedToServer();

public slots:
    void parseIncomingPacketData(const PacketBase &packet);


    bool sendClientLookForServerPacket(const QString &targetAddress = QString(IM_SERVER_IPMC_ADDRESS), quint16 targetPort = IM_SERVER_IPMC_LISTENING_PORT)
    {
        qDebug() << "----sendClientLookForServerPacket(...) targetAddress:" << targetAddress;

        QHostAddress address = QHostAddress(targetAddress);
        if(address.isNull()) {
            address = QHostAddress(QString(IM_SERVER_IPMC_ADDRESS));
        }
        quint16 port = targetPort;
        if(!port) {
            port = IM_SERVER_IPMC_LISTENING_PORT;
        }

        return sendClientLookForServerPacket(address, port);
    }

    bool sendClientLookForServerPacket(const QHostAddress &targetAddress, quint16 targetPort)
    {
        //qDebug()<<"----sendClientLookForServerPacket(...)";
        ServerDiscoveryPacket packet(ServerDiscoveryPacket::SERVER_GATE);
        packet.serverAnnouncement = 0;
        packet.version = QString(APP_VERSION);

        QByteArray ba = packet.toByteArray();
        if(ba.isEmpty()) {
            return false;
        }

        return m_udpServer->sendDatagram(packet.toByteArray(), targetAddress, targetPort);
    }


    bool requestDataForward(int socketID, const QString &receiverID, const QByteArray &data)
    {
        qDebug() << "--requestDataForward(...) receiverID:" << receiverID;

        DataForwardPacket p(sessionEncryptionKey);
        p.data = data;
        p.receiver = receiverID;
        p.isRequest = 1;

        QByteArray ba = p.toByteArray();
        if(ba.isEmpty()) {
            return false;
        }

        return m_rtp->sendReliableData(socketID, &ba);
    }



    bool requestRegistrationServerInfo(int serverSocketID, const QString &clientVersion = APP_VERSION)
    {
        qWarning() << "--requestRegistrationServerInfo(...)";

        RgeistrationPacket packet(RgeistrationPacket::REGISTRATION_SERVER_INFO, RgeistrationPacket::getLocalID().toUtf8());
        packet.ServerInfo.requestServerInfo = 1;
        packet.ServerInfo.version = clientVersion;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool registration(int serverSocketID, const QString &userID, const QString &password)
    {
        qWarning() << "--registration(...)";

        RgeistrationPacket packet(RgeistrationPacket::REGISTRATION_INFO, RgeistrationPacket::getLocalID().toUtf8());
        packet.RgeistrationInfo.userID = userID;
        packet.RgeistrationInfo.password = password;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool requestUpdatePassword(int serverSocketID)
    {
        qDebug() << "--requestUpdatePassword(...)";

        UpdatePasswordPacket packet(UpdatePasswordPacket::INFO_TYPE_UPDATE_MODE_INFO, sessionEncryptionKey);
        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool updatePassword(const QString &captcha, const QString &oldPassword, const QString &newPassword, const QString &securityAnswer, const QString &smsCaptcha)
    {
        qDebug() << "--updatePassword(...)";

        UpdatePasswordPacket packet(UpdatePasswordPacket::INFO_TYPE_AUTH_INFO_FROM_CLIENT, sessionEncryptionKey);
        packet.AuthInfo.oldPassword = oldPassword;
        packet.AuthInfo.newPassword = newPassword;
        packet.AuthInfo.securityAnswer = securityAnswer;
        packet.AuthInfo.captcha = captcha;
        packet.AuthInfo.smsCaptcha = smsCaptcha;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(m_socketConnectedToServer, &ba);
    }


    bool requestLoginServerInfo(int serverSocketID)
    {
        qDebug() << "--requestLoginServerInfo(...)";

        ServerDiscoveryPacket packet(ServerDiscoveryPacket::SERVER_LOGIN);
        packet.serverAnnouncement = 0;
        packet.version = QString(APP_VERSION);

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool requestLogin(int serverSocketID)
    {
        qDebug() << "--requestLogin(...)";

//        m_myUserID = myself->getUserID();
//        sessionEncryptionKey = cryptography->SHA1(myself->getPassword().toUtf8());
//        Q_ASSERT_X(!m_myUserID.isEmpty(), "requestLogin", "Invalid User ID!");
//        if(m_myUserID.isEmpty()) {
//            qCritical() << "Invalid User ID!";
//            return false;
//        }
//        qWarning() << "My User ID:" << m_myUserID;

        return login(serverSocketID);
    }

    bool changeMyOnlineState(int serverSocketID, quint8 onlineStateCode)
    {
        qDebug() << "--changeMyOnlineState(...)";

        OnlineStateChangedPacket packet(sessionEncryptionKey);
        packet.stateCode = onlineStateCode;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool logout(int serverSocketID)
    {
        qDebug() << "--logout(...)";
        return changeMyOnlineState(serverSocketID, IM::ONLINESTATE_OFFLINE);
    }

    bool requestInterestGroupsList(int serverSocketID)
    {
        qDebug() << "------------------requestInterestGroupsList(...)";

        InterestGroupsInfoPacket packet(InterestGroupsInfoPacket::PIT_GROUPS_LIST, sessionEncryptionKey);
        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool requestInterestGroupInfo(int serverSocketID, quint32 groupID)
    {

        InterestGroupsInfoPacket packet(InterestGroupsInfoPacket::PIT_GROUP_INFO, sessionEncryptionKey);
        packet.GroupID = groupID;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool requestCreateInterestGroup(int serverSocketID, const QString &groupName, quint8 groupType)
    {
        qDebug() << "----requestCreateInterestGroup(...)";

        InterestGroupsInfoPacket packet(InterestGroupsInfoPacket::PIT_GROUP_CREATION, sessionEncryptionKey);
        packet.GroupCreationInfo.name = groupName;
        packet.GroupCreationInfo.type = groupType;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool requestDisbandInterestGroup(int serverSocketID, quint32 groupID)
    {
        qDebug() << "----requestDisbandInterestGroup(...)";
        InterestGroupsInfoPacket packet(InterestGroupsInfoPacket::PIT_GROUP_DELETION, sessionEncryptionKey);
        packet.GroupID = groupID;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool joinOrQuitInterestGroup(int serverSocketID, quint32 groupID, bool join, const QString &verificationMessage = "")
    {
        qWarning() << "--joinOrQuitInterestGroup(...)";
        InterestGroupsInfoPacket packet(InterestGroupsInfoPacket::PIT_UNKNOWN, sessionEncryptionKey);
        packet.GroupID = groupID;

        if(join) {
            packet.setPacketSubType(quint8(InterestGroupsInfoPacket::PIT_GROUP_MEMBER_APPLICATION));
            packet.MemberApplicationInfo.userID = m_myUserID;
            packet.MemberApplicationInfo.message = verificationMessage;
        } else {
            packet.setPacketSubType(quint8(InterestGroupsInfoPacket::PIT_GROUP_MEMBER_DELETION));
            packet.MemberDeletionInfo.userID = m_myUserID;
        }

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool requestPersonalContactGroupsInfo(int serverSocketID)
    {
        //qWarning()<<"--requestPersonalContactGroupsInfo(...)";
        ContactGroupsInfoPacket packet(ContactGroupsInfoPacket::PIT_GROUPS_LIST, sessionEncryptionKey);

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool requestContactInfo(int serverSocketID, const QString &contactID, bool summaryInfo = true)
    {
        qDebug() << "--requestContactInfo(...)";
        ContactInfoPacket packet(ContactInfoPacket::PIT_CONTACT_INFO, sessionEncryptionKey);
        packet.ContactID = contactID;
        packet.info.isSummaryInfo = quint8(summaryInfo ? 1 : 0);

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool requestPersonalInfo(int serverSocketID)
    {
        qDebug() << "--requestPersonalInfo(...)";
        return requestContactInfo(serverSocketID, m_myUserID);
    }

    bool requestFriending(int serverSocketID, const QString &contactID, const QString &verificationMessage = "", quint32 groupID = ContactGroupBase::Group_Friends_ID)
    {
        qWarning() << "--requestFriending(...)";
        ContactInfoPacket packet(ContactInfoPacket::PIT_FRIENDING_REQUEST, sessionEncryptionKey);
        packet.ContactID = contactID;
        packet.ContactFriendingRequest.message = verificationMessage;
        packet.ContactFriendingRequest.groupID = groupID;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool responseFriendingRequestFromUser(int serverSocketID, const QString &userID, quint8 errorCode, const QString &extraMessage = "")
    {
        qDebug() << "--responseFriendingRequestFromUser(...)";
        ContactInfoPacket packet(ContactInfoPacket::PIT_FRIENDING_RESULT, sessionEncryptionKey);
        packet.ContactID = userID;
        packet.ContactFriendingResult.errorCode = errorCode;
        packet.ContactFriendingResult.message = extraMessage;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool deleteContact(int serverSocketID, const QString &contactID, bool deleteMeFromOpposition = false, bool blockForever = false)
    {
        qDebug() << "--deleteContact(...)";
        ContactInfoPacket packet(ContactInfoPacket::PIT_CONTACT_DELETION, sessionEncryptionKey);
        packet.ContactID = contactID;
        packet.ContactDeletionInfo.deleteMeFromOpposition = deleteMeFromOpposition;
        packet.ContactDeletionInfo.blockForever = blockForever;
        //packet.ContactDeletionInfo.errorCode = errorCode;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool moveContactToGroup(int serverSocketID, const QString &contactID, quint32 oldGroupID, quint32 newGroupID)
    {
        qDebug() << "--moveContactToGroup(...)";
        ContactInfoPacket packet(ContactInfoPacket::PIT_GROUP_CHANGE, sessionEncryptionKey);
        packet.ContactID = contactID;
        packet.ContactChangeGroup.oldGroupID = oldGroupID;
        packet.ContactChangeGroup.newGroupID = newGroupID;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool updateContactRemarkInfo(int serverSocketID, quint32 contactID, const QString &remarkName, const QString &extraRemarkInfo)
    {
        qDebug() << "--updateContactRemarkInfo(...)";
        ContactInfoPacket packet(ContactInfoPacket::PIT_CONTACT_REMARK, sessionEncryptionKey);
        packet.ContactID = contactID;
        packet.ContactRemarkInfo.newRemarkName = remarkName;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool createContactGroup(int serverSocketID, quint32 parentGroupID, const QString &groupName)
    {
        qDebug() << "--createContactGroup(...)";
        ContactGroupsInfoPacket packet(ContactGroupsInfoPacket::PIT_GROUP_CREATION, sessionEncryptionKey);
        packet.GroupCreationInfo.name = groupName;
        packet.GroupCreationInfo.parentID = parentGroupID;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool deleteContactGroup(int serverSocketID, quint32 groupID)
    {
        qDebug() << "--deleteContactGroup(...)";
        ContactGroupsInfoPacket packet(ContactGroupsInfoPacket::PIT_GROUP_DELETION, sessionEncryptionKey);
        packet.GroupDeletionInfo.groupID = groupID;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool renameContactGroup(int serverSocketID, quint32 groupID, const QString &newGroupName)
    {
        qDebug() << "--renameContactGroup(...)";
        ContactGroupsInfoPacket packet(ContactGroupsInfoPacket::PIT_GROUP_RENAMING, sessionEncryptionKey);
        packet.GroupRenamingInfo.groupID = groupID;
        packet.GroupRenamingInfo.newName = newGroupName;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool searchContact(int serverSocketID, const QString &keyword, quint8 searchOnlineUsersOnly,
                       quint8 searchWebcamUsersOnly, quint16 location, quint16 hometown,
                       quint8 gender, quint8 age, bool matchExactly, int startIndex
                      )
    {
        qDebug() << "--searchContact(...)";
        SearchInfoPacket packet(SearchInfoPacket::PIT_SEARCH_CONTACT_CONDITIONS, sessionEncryptionKey);
        packet.SearchContactConditions.keyword = keyword;
        packet.SearchContactConditions.isOnline = searchOnlineUsersOnly;
        packet.SearchContactConditions.hasWebcam = searchWebcamUsersOnly;
        packet.SearchContactConditions.location = location;
        packet.SearchContactConditions.hometown = hometown;
        packet.SearchContactConditions.gender = gender;
        packet.SearchContactConditions.age = age;
        packet.SearchContactConditions.matchExactly = matchExactly;
        packet.SearchContactConditions.startIndex = startIndex;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool searchInterestGroup(int serverSocketID, const QString &keyword, int startIndex)
    {
        qDebug() << "--searchInterestGroup(...)";
        SearchInfoPacket packet(SearchInfoPacket::PIT_SEARCH_INTEREST_GROUP_CONDITIONS, sessionEncryptionKey);
        packet.SearchInterestGroupConditions.keyword = keyword;
        packet.SearchInterestGroupConditions.startIndex = startIndex;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool sendChatMessageToContact(int peerSocketID, const QString &contactID, const QString &message, const QStringList &imageNameList)
    {
        qDebug() << "--sendChatMessageToContact(...)";
        ChatMessagePacket packet(ChatMessagePacket::PIT_CONTACT_CHAT_MESSAGE, sessionEncryptionKey);
        packet.ContactChatMessage.contactID = contactID;
        packet.ContactChatMessage.message = message;
        packet.ContactChatMessage.imageNames = imageNameList.join(",");;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendInterestGroupChatMessageToServer(int serverSocketID, quint32 interestGroupID , const QString &message, const QStringList &imageNameList)
    {
        qDebug() << "--sendInterestGroupChatMessageToServer(...)";
        ChatMessagePacket packet(ChatMessagePacket::PIT_GROUP_CHAT_MESSAGE, sessionEncryptionKey);
        packet.GroupChatMessage.groupID = interestGroupID;
        packet.GroupChatMessage.message = message;
        packet.GroupChatMessage.imageNames = imageNameList.join(",");;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool sendImageToContact(int peerSocketID, const QString &contactID, const QString &imageName, const QByteArray &image)
    {
        qDebug() << "--sendImageToContact(...)";
        ChatMessagePacket packet(ChatMessagePacket::PIT_CHAT_IMAGE, sessionEncryptionKey);
        packet.ChatImage.isRequest = 0;
        packet.ChatImage.contactID = contactID;
        packet.ChatImage.name = imageName;
        packet.ChatImage.image = image;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool requestDownloadImageFromContact(int peerSocketID, const QString &contactID, const QString &imageName)
    {
        qDebug() << "--requestImageFromContact(...)";
        ChatMessagePacket packet(ChatMessagePacket::PIT_CHAT_IMAGE, sessionEncryptionKey);
        packet.ChatImage.isRequest = 1;
        packet.ChatImage.contactID = contactID;
        packet.ChatImage.name = imageName;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool requestSessionEncryptionKeyWithContact(int serverSocketID, quint32 contactID)
    {
        qDebug() << "--requestSessionEncryptionKeyWithContact(...)";
        ChatMessagePacket packet(ChatMessagePacket::PIT_SESSION_ENCRYPTION_KEY_WITH_CONTACT, sessionEncryptionKey);
        packet.SessionEncryptionKeyWithContact.contactID = contactID;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool requestCaptcha(int serverSocketID)
    {
        qDebug() << "--requestCaptcha(...)";

        CaptchaInfoPacket packet(CaptchaInfoPacket::CAPTCHA_REQUEST, sessionEncryptionKey);

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }























    bool requestSendFileToContact(int peerSocketID, const QString &contactID, const QStringList &filePathList)
    {
        //TODO:
        return true;
    }

    bool sendChatMessageToContacts(const QStringList &contacts, const QString &message, const QStringList &imagePathList)
    {
        //TODO:
        return true;
    }



    //FILE TX
    bool requestFileServerInfo(SOCKETID serverSocketID)
    {
        FileTransferPacket packet(FileTransferPacket::FT_FILE_SERVER_INFO, sessionEncryptionKey);
        //packet.FileServerInfo.address = "";
        //packet.FileServerInfo.port = 0;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool responseFileServerInfo(SOCKETID socketID, quint32 address, quint16 tcpPort, quint16 rtpPort)
    {
        FileTransferPacket packet(FileTransferPacket::FT_FILE_SERVER_INFO, sessionEncryptionKey);
        packet.ContactID = m_myUserID;
        packet.FileServerInfo.request = false;
        packet.FileServerInfo.lanAddress = address;
        packet.FileServerInfo.tcpPort = tcpPort;
        packet.FileServerInfo.rtpPort = rtpPort;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(socketID, &ba);
    }




private slots:


    //    int crypto(QByteArray *destination, const QByteArray &source, bool encrypt);

    ///////////////////////////////////////////////
    //quint16 login(const QString &serverAddress, quint16 serverPort){
    bool login(int serverSocketID)
    {
        //qWarning()<<"--login() "<<m_serverAddress.toString()<<":"<<m_serverUDPListeningPort;


        Packet::setLocalID(myself->getUserID());


        //TODO:Device Info
        QString deviceInfo = "PC";

        //TODO:密码保存方式
        QByteArray encryptedPassword = myself->encryptedPassword();
        QByteArray key = encryptedPassword;
        LoginPacket packet(LoginPacket::INFO_TYPE_AUTH_INFO_FROM_CLIENT, key);
        packet.AuthInfo.password = encryptedPassword;
        packet.AuthInfo.stateAfterLoggedin = quint8(myself->getStateAfterLoggedin());
        packet.AuthInfo.deviceInfo = deviceInfo;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }





signals:
    //void  signalHeartbeatPacketReceived(const QString &contactID);
    //void  signalConfirmationOfReceiptPacketReceived(quint16 packetSerialNumber1, quint16 packetSerialNumber2);

    void signalServerDeclarePacketReceived(const ServerDiscoveryPacket &packet);
    void signalServerAnnouncementPacketReceived(const AnnouncementPacket &packet);

    void signalRegistrationPacketReceived(const RgeistrationPacket &packet);
    void signalUpdatePasswordResultReceived(const UpdatePasswordPacket &packet);

    void signalLoginResultReceived(quint8 errorType, const QString &errorMessage = "");
    void signalLoginServerRedirected(const QString &serverAddress, quint16 serverPort, const QString &serverName);
    void signalClientLastLoginInfoPacketReceived(const QString &extIPAddress, const QString &loginTime, const QString &LogoutTime, const QString &deviceInfo);

    void signalContactStateChangedPacketReceived(const QString &contactID, quint8 onlineState, const QString &contactHostAddress, quint16 contactHostPort);
    void signalContactsOnlineInfoPacketReceived(const QString &contactsOnlineInfoString);


    void signalContactGroupsInfoPacketReceived(const ContactGroupsInfoPacket &packet);
    void signalInterestGroupsInfoPacketReceived(const InterestGroupsInfoPacket &packet);
    void signalContactInfoPacketReceived(const ContactInfoPacket &packet);
    void signalSearchContactsResultPacketReceived(const SearchInfoPacket &packet);
    void signalChatMessageReceivedFromContact(const ChatMessagePacket &packet);
    void signalCaptchaInfoPacketReceived(const CaptchaInfoPacket &packet);
    void signalFileTransferPacketReceived(const FileTransferPacket &packet);
















    //void signalContactStateChangedPacketReceived(const QString &contactID, IM::OnlineState onlineState, const QString &contactHostAddress, quint16 contactHostPort);


//    void signalCreateOrDeleteContactGroupResultPacketReceived(quint32 groupID, const QString &groupName, bool createGroup, bool result);


    void signalSearchInterestGroupsResultPacketReceived(const QString &groupsString);









    void contactGroupInfoPacketReceived();

    void interestGroupsListPacketReceived();
    void interestGroupInfoPacketReceived();

    void systemGroupsListPacketReceived();
    void systemGroupInfoPacketReceived();

    void signalServerResponseAddContactPacketReceived();
    void signalServerResponseDeleteContactPacketReceived();
    void signalServerResponseMoveContactPacketReceived();
    void signalServerResponseModifyContactRemarkPacketReceived();

    void signalClientSoftwareUpdatePacketReceived();



    ///////////////////////////
    //File TX
    void signalContactRequestUploadFile(const QString &contactID, const QByteArray &fileMD5Sum, const QString &fileName, quint64 size);
    void signalContactRequestDownloadFile(const QString &contactID, const QString &localBaseDir, const QString &fileName);

    void signalFileDownloadRequestAccepted(const QString &contactID, const QString &remoteFileName, const QByteArray &fileMD5Sum, quint64 size);
    void signalFileDownloadRequestDenied(const QString &contactID, const QString &remoteFileName, const QString &message);
    void signalFileUploadRequestResponsed(const QString &contactID, const QByteArray &fileMD5Sum, bool accepted, const QString &message);



public:
    QStringList runningNICAddresses();
    void setSessionEncryptionKeyWithContact(const QString &contactID, const QByteArray &key);


private slots:

    void processLoginPacket(const LoginPacket &packet);



private:


    QTimer *heartbeatTimer;
    //        QTimer *processWaitingForReplyPacketsTimer;

    //ClientNetworkManager *m_networkManager;
    //PacketHandlerBase *m_packetHandlerBase;


    //QDateTime serverLastOnlineTime;

    QMutex mutex;

    IMUser *myself;
    QString m_myUserID;
    QString m_serverName;
    int m_socketConnectedToServer;

    Cryptography *cryptography;
    QByteArray sessionEncryptionKey;

    QHash<QString/*Contact's ID*/, QByteArray/*Session Encryption Key*/> sessionEncryptionKeyWithContactHash;


    QMultiHash<QString /*Peer ID*/, QPair<quint16 /*Packet Serial Number*/, QDateTime/*Received Time*/> > m_receivedPacketsHash;


    UDPServer *m_udpServer;
    RTP *m_rtp;
//    UDTProtocol *m_udtProtocol;
    TCPServer *m_tcpServer;
    ENETProtocol *m_enetProtocol;



};

}

#endif /* IMCLIENTPACKETSPARSER_H_ */
