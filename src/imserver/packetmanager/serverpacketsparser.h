/*
 ****************************************************************************
 * serverpacketsparser.h
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
 * Last Modified On: 2010-9-3
 * Last Modified By: 贺辉
 ***************************************************************************
 */

#ifndef SERVERPACKETSPARSER_H_
#define SERVERPACKETSPARSER_H_



#include <QHostAddress>
#include <QHostInfo>
#include <QDebug>

#include "../app_constants_server.h"
#include "../usersmanager/usersmanager.h"

#include "../sharedim/resourcesmanagerinstance.h"
#include "../sharedim/impackets.h"

#include "HHSharedCore/hcryptography.h"



namespace HEHUI
{


class ServerPacketsParser : public QObject, public UsersManager
{
    Q_OBJECT
public:
    ServerPacketsParser(ResourcesManagerInstance *resourcesManager, QObject *parent = 0);
    virtual ~ServerPacketsParser();








public slots:
    void parseIncomingPacketData(const PacketBase &packet);


    bool sendServerDeclarePacket(const QHostAddress peerAddress, quint16 peerPort, quint32 ip = 0, ServerDiscoveryPacket::ServerType serverType = ServerDiscoveryPacket::SERVER_LOGIN)
    {

        //qDebug()<<"Server Instance ID:"<<serverInstanceID;

        ServerDiscoveryPacket packet(serverType);
        packet.serverAnnouncement = 1;
        packet.version = QString(APP_VERSION);
        packet.ip = ip;
        packet.rtpPort = 0;
        packet.tcpPort = 0;
        packet.serverInstanceID = m_serverInstanceID;

        return m_ipmcServer->sendDatagram(packet.toByteArray(), peerAddress, peerPort);
    }

    bool forwardData(int peerSocketID, const QByteArray &data, const QByteArray &sessionEncryptionKey)
    {
        qDebug() << "--forwardData(...)";

        DataForwardPacket p(sessionEncryptionKey);
        p.data = data;

        QByteArray ba = p.toByteArray();
        if(ba.isEmpty()) {
            return false;
        }

        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendServerAnnouncementPacket(const QHostAddress peerAddress, quint16 peerPort, const QByteArray &sessionEncryptionKey, unsigned int id , const QString &announcement, bool mustRead = true)
    {
        qDebug() << "----sendServerAnnouncementPacket(...)";
        AnnouncementPacket packet(AnnouncementPacket::ANNOUNCEMENT_INFO, sessionEncryptionKey);
        packet.AnnouncementInfo.announcementID = id;
        packet.AnnouncementInfo.content = announcement;

        return m_ipmcServer->sendDatagram(packet.toByteArray(), peerAddress, peerPort);
    }


    bool sendRegistrationServerInfoPacket(int peerSocketID, const QByteArray &encryptionKey, quint8 regMode, const QString &message, const QString &serverAddress)
    {
        qDebug() << "--sendRegistrationServerInfoPacket(...)";
        RgeistrationPacket packet(RgeistrationPacket::REGISTRATION_SERVER_INFO, encryptionKey);
        packet.ServerInfo.requestServerInfo = 0;
        packet.ServerInfo.version = APP_VERSION;
        packet.ServerInfo.regMode = regMode;
        packet.ServerInfo.message = message;
        packet.ServerInfo.address = serverAddress;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendClientRegistrationResultPacket(int peerSocketID, const QByteArray &encryptionKey, quint32 sysID, const QString &userID, const QString &password, quint8 errorTypeCode)
    {
        qWarning() << "--sendClientRegistrationResultPacket(...)";
        RgeistrationPacket packet(RgeistrationPacket::REGISTRATION_RESULT, encryptionKey);
        packet.RgeistrationResult.sysID = sysID;
        packet.RgeistrationResult.userID = userID;
        packet.RgeistrationResult.password = password;
        packet.RgeistrationResult.errorCode = errorTypeCode;
        packet.RgeistrationResult.activationMode = UsersManager::activationMode();

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendUpdatePasswordAuthInfoPacket(int peerSocketID, const QString &userID, quint8 authMode, const QByteArray &captchaImage, const QString &securityQuestion, const QString &email, quint32 cellphoneNO, const QString &url)
    {
        qWarning() << "--sendUpdatePasswordAuthInfoPacket(...)";

        //TODO
        UpdatePasswordPacket packet(UpdatePasswordPacket::INFO_TYPE_AUTH_INFO_FROM_SERVER, getUserSessionEncryptionKey(userID));
        packet.AuthInfo.authMode = authMode;
        packet.AuthInfo.captchaImage = captchaImage;
        packet.AuthInfo.userID = userID;
        packet.AuthInfo.securityQuestion = securityQuestion;
        packet.AuthInfo.email = email;
        packet.AuthInfo.cellphoneNO = cellphoneNO;
        packet.AuthInfo.url = url;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendClientUpdatePasswordResultPacket(int peerSocketID, const QString &userID, quint8 updateResult)
    {
        qDebug() << "--sendClientUpdatePasswordResultPacket(...)";

        UpdatePasswordPacket packet(UpdatePasswordPacket::INFO_TYPE_UPDATE_RESULT, getUserSessionEncryptionKey(userID));
        packet.AuthInfo.updateResult = updateResult;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendLoginServerInfoPacket(int peerSocketID, const QHostAddress &serverAddress, quint16 serverPort)
    {
        qDebug() << "--sendLoginServerInfoPacket(...)";

        ServerDiscoveryPacket packet(ServerDiscoveryPacket::SERVER_LOGIN);
        packet.serverAnnouncement = 1;
        packet.version = QString(APP_VERSION);
        packet.ip = serverAddress.toIPv4Address();
        packet.rtpPort = serverPort;
        packet.tcpPort = serverPort;
        packet.serverInstanceID = m_serverInstanceID;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendClientLoginFailedPacket(int peerSocketID, quint8 errorTypeCode, const QByteArray &encryptionKey)
    {
        qDebug() << "--sendClientLoginFailedPacket(...)";

        LoginPacket packet(LoginPacket::INFO_TYPE_LOGIN_FAILED, encryptionKey);
        packet.AuthResultInfo.loggedin = 0;
        packet.AuthResultInfo.errorType = errorTypeCode;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendClientLoginSucceededPacket(int peerSocketID, const QString &userID, const QByteArray &encryptedPassword, const QByteArray &newSessionEncryptionKey, quint32 personalSummaryInfoVersion,
                                        quint32 personalDetailInfoVersionOnServer, quint32 personalContactGroupsInfoVersionOnServer,
                                        quint32 interestGroupInfoVersionOnServer, quint32 blacklistInfoVersionOnServer)
    {

        qDebug() << "--sendClientLoginSucceededPacket(...)";

        LoginPacket packet(LoginPacket::INFO_TYPE_LOGIN_SUCCEEDED, encryptedPassword);
        packet.AuthResultInfo.loggedin = 1;
        packet.AuthResultInfo.errorType = quint8(IM::ERROR_NoError);

        packet.AuthResultInfo.sessionEncryptionKey = newSessionEncryptionKey;
        packet.AuthResultInfo.personalSummaryInfoVersionOnServer = personalSummaryInfoVersion;
        packet.AuthResultInfo.personalDetailInfoVersionOnServer = personalDetailInfoVersionOnServer;
        packet.AuthResultInfo.personalContactGroupsInfoVersionOnServer = personalContactGroupsInfoVersionOnServer;
        packet.AuthResultInfo.interestGroupsInfoVersionOnServer = interestGroupInfoVersionOnServer;
        packet.AuthResultInfo.personalMessageInfoVersionOnServer = blacklistInfoVersionOnServer;

        uint serverTime = QDateTime::currentDateTime().toUTC().toTime_t();
        packet.AuthResultInfo.serverTime = serverTime;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendClientLastLoginInfoPacket(int peerSocketID, const QByteArray &sessionEncryptionKey, const QString &extIPAddress, uint loginTime, uint logoutTime, const QString &deviceInfo )
    {
        qDebug() << "--sendClientLastLoginInfoPacket(...)";

        LoginPacket packet(LoginPacket::INFO_TYPE_PREVIOUS_LOGIN_INFO, sessionEncryptionKey);
        packet.PreviousLoginInfo.loginIP = extIPAddress;
        packet.PreviousLoginInfo.loginTime = loginTime;
        packet.PreviousLoginInfo.logoutTime = logoutTime;
        packet.PreviousLoginInfo.deviceInfo = deviceInfo;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendContactOnlineStatusChangedPacket(int peerSocketID, const QByteArray &sessionEncryptionKey, quint8 onlineStateCode, const QString &contactID, const QString &contactHostAddress, quint16 contactHostPort)
    {
        qDebug() << "--sendContactOnlineStatusChangedPacket(...)";

        OnlineStateChangedPacket packet(sessionEncryptionKey);
        packet.stateCode = onlineStateCode;
        packet.contactID = contactID;
        packet.contactIP = contactHostAddress;
        packet.contactPort = contactHostPort;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendContactsOnlineInfo(int peerSocketID, UserInfo *userInfo)
    {
        if(!userInfo) {
            return false;
        }

        QStringList contacts = userInfo->getAllContacts();
        QStringList contactsOnlineInfo;
        foreach (QString contactID, contacts) {
            UserInfo *info = getOnlineUserInfo(contactID);
            if(info) {
                IM::OnlineState state = info->getOnlineState();
                if(state != IM::ONLINESTATE_OFFLINE && state != IM::ONLINESTATE_INVISIBLE) {
                    QStringList onlineInfo;
                    onlineInfo.append(contactID);
                    onlineInfo.append(QString::number(quint8(state)));
                    onlineInfo.append(info->getLastLoginExternalHostAddress());
                    onlineInfo.append(QString::number(info->getLastLoginExternalHostPort()));
                    contactsOnlineInfo.append(onlineInfo.join(QString(PACKET_DATA_SEPARTOR)));
                }

            }
        }
        if(contactsOnlineInfo.isEmpty()) {
            return false;
        }

        OnlineContacts packet(userInfo->getSessionEncryptionKey());
        packet.contactsOnlineInfoString = contactsOnlineInfo.join(QString(UNIT_SEPARTOR));

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendUserInterestGroupsListPacket(int peerSocketID, UserInfo *userInfo)
    {
        InterestGroupsInfoPacket packet(InterestGroupsInfoPacket::PIT_GROUPS_LIST, userInfo->getSessionEncryptionKey());
        packet.GroupsList.groups = getInterestGroupsListForUser(userInfo);
        packet.GroupsList.version = userInfo->getInterestGroupInfoVersion();

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendUserInterestGroupInfoPacket(int peerSocketID, UserInfo *userInfo, quint32 groupID)
    {
        InterestGroupsInfoPacket packet(InterestGroupsInfoPacket::PIT_GROUP_INFO, userInfo->getSessionEncryptionKey());
        packet.GroupID = groupID;
        packet.GroupInfo.infoString = getInterestGroupInfoStringForUser(userInfo, groupID);

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendCreateInterestGroupResultPacket(int peerSocketID, UserInfo *userInfo, quint32 groupID, const QString &groupName)
    {
        qDebug() << "--sendCreateInterestGroupResultPacket() " << " groupID:" << groupID << " peerSocketID:" << peerSocketID;
        InterestGroupsInfoPacket packet(InterestGroupsInfoPacket::PIT_GROUP_CREATION, userInfo->getSessionEncryptionKey());
        packet.GroupID = groupID;
        packet.GroupCreationInfo.name = groupName;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendDisbandInterestGroupResultPacket(int peerSocketID, UserInfo *userInfo, quint32 groupID, bool result)
    {
        qDebug() << "--sendDisbandInterestGroupResultPacket() " << " groupID:" << groupID << " peerSocketID:" << peerSocketID;
        InterestGroupsInfoPacket packet(InterestGroupsInfoPacket::PIT_GROUP_DELETION, userInfo->getSessionEncryptionKey());
        packet.GroupID = groupID;
        packet.GroupDeletionInfo.deleted = result;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendUserJoinInterestGroupResultToUserPacket(int peerSocketID, quint32 groupID, const QString &memberID, bool approved, const QString &message, const QString &admin, const QByteArray &sessionEncryptionKey)
    {
        qDebug() << "--sendUserJoinInterestGroupResultToUserPacket(...)";
        InterestGroupsInfoPacket packet(InterestGroupsInfoPacket::PIT_GROUP_MEMBER_APPLICATION, sessionEncryptionKey);
        packet.GroupID = groupID;
        packet.MemberApplicationInfo.userID = memberID;
        packet.MemberApplicationInfo.message = message;
        packet.MemberApplicationInfo.approved = approved;
        packet.MemberApplicationInfo.admin = admin;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendUserJoinInterestGroupRequestToAdminPacket(int peerSocketID, quint32 groupID, const QString &userID, const QString &verificationMessage, const QByteArray &sessionEncryptionKey)
    {
        qDebug() << "--sendUserJoinInterestGroupRequestToAdminPacket(...)";
        InterestGroupsInfoPacket packet(InterestGroupsInfoPacket::PIT_GROUP_MEMBER_APPLICATION, sessionEncryptionKey);
        packet.GroupID = groupID;
        packet.MemberApplicationInfo.userID = userID;
        packet.MemberApplicationInfo.message = verificationMessage;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendUserQuitInterestGroupResultToUserPacket(int peerSocketID, quint32 groupID, const QString &memberID, const QString &admin, const QByteArray &sessionEncryptionKey)
    {
        qDebug() << "--sendUserQuitInterestGroupResultToUserPacket(...)";
        InterestGroupsInfoPacket packet(InterestGroupsInfoPacket::PIT_GROUP_MEMBER_DELETION, sessionEncryptionKey);
        packet.GroupID = groupID;
        packet.MemberDeletionInfo.userID = memberID;
        packet.MemberDeletionInfo.blockForever = false;
        packet.MemberDeletionInfo.admin = admin;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }



    bool sendPersonalContactGroupsInfoPacket(int peerSocketID, const QString &contactGroupsInfo, quint32 personalContactGroupsInfoVersionOnServer, const QString &contactInfoVersionList, const QByteArray &sessionEncryptionKey)
    {
        qDebug() << "--sendPersonalContactGroupsInfoPacket(...)";
        ContactGroupsInfoPacket packet(ContactGroupsInfoPacket::PIT_GROUPS_LIST, sessionEncryptionKey);
        packet.GroupsList.groupsInfo = contactGroupsInfo;
        packet.GroupsList.version = personalContactGroupsInfoVersionOnServer;
        packet.GroupsList.contactInfoVersionList = contactInfoVersionList;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendCreateContactGroupResultPacket(int peerSocketID, quint32 groupID, const QString &groupName, quint8 errorCode, const QByteArray &sessionEncryptionKey)
    {
        qDebug() << "--sendCreateContactGroupResultPacket(...)";
        ContactGroupsInfoPacket packet(ContactGroupsInfoPacket::PIT_GROUP_CREATION, sessionEncryptionKey);
        packet.GroupCreationInfo.name = groupName;
        packet.GroupCreationInfo.groupID = groupID;
        packet.GroupCreationInfo.errorCode = errorCode;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendDeleteContactGroupResultPacket(int peerSocketID, quint32 groupID, quint8 errorCode, const QByteArray &sessionEncryptionKey)
    {
        qDebug() << "--sendDeleteContactGroupResultPacket(...)";
        ContactGroupsInfoPacket packet(ContactGroupsInfoPacket::PIT_GROUP_DELETION, sessionEncryptionKey);
        packet.GroupDeletionInfo.groupID = groupID;
        packet.GroupDeletionInfo.errorCode = errorCode;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }


    bool sendUserInfoPacket(int peerSocketID, const QString &contactID, bool isSummaryInfo, const QString &infoString, const QByteArray &sessionEncryptionKey)
    {
        qDebug() << "--sendUserInfoPacket(...)";
        ContactInfoPacket packet(ContactInfoPacket::PIT_CONTACT_INFO, sessionEncryptionKey);
        packet.ContactID = contactID;
        packet.info.isSummaryInfo = quint8(isSummaryInfo ? 1 : 0);
        packet.info.infoString = infoString;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendAddContactRequestFromUserPacket(int serverSocketID, const QString &userID, const QString &nickName, const QString &userFace, const QString &verificationMessage, const QByteArray &sessionEncryptionKey)
    {
        qDebug() << "--sendAddContactRequestFromUserPacket(...)";
        ContactInfoPacket packet(ContactInfoPacket::PIT_FRIENDING_REQUEST, sessionEncryptionKey);
        packet.ContactID = userID;
        packet.ContactFriendingRequest.nickName = nickName;
        packet.ContactFriendingRequest.userFace = userFace;
        packet.ContactFriendingRequest.message = verificationMessage;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool sendAddContactResultPacket(int peerSocketID, const QString &contactID, quint8 errorCode, const QString &reasonMessage, const QByteArray &sessionEncryptionKey)
    {
        qDebug() << "--sendAddContactResultPacket(...)";
        ContactInfoPacket packet(ContactInfoPacket::PIT_FRIENDING_RESULT, sessionEncryptionKey);
        packet.ContactID = contactID;
        packet.ContactFriendingResult.errorCode = errorCode;
        packet.ContactFriendingResult.message = reasonMessage;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }


    bool sendDeleteContactResultPacket(int peerSocketID, const QString &contactID, quint8 blockForever, quint8 errorCode, const QByteArray &sessionEncryptionKey)
    {
        qDebug() << "--sendDeleteContactResultPacket(...)";
        ContactInfoPacket packet(ContactInfoPacket::PIT_CONTACT_DELETION, sessionEncryptionKey);
        packet.ContactID = contactID;
        packet.ContactDeletionInfo.blockForever = blockForever;
        packet.ContactDeletionInfo.errorCode = errorCode;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendSearchContactResultPacket(int peerSocketID, const QString &result, const QByteArray &sessionEncryptionKey)
    {
        qDebug() << "--sendSearchContactResultPacket(...)";
        SearchInfoPacket packet(SearchInfoPacket::PIT_SEARCH_CONTACT_RESULT, sessionEncryptionKey);
        packet.SearchContactResult.result = result;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendSearchInterestGroupResultPacket(int peerSocketID, const QString &result, const QByteArray &sessionEncryptionKey)
    {
        qDebug() << "--sendSearchInterestGroupResultPacket(...)";
        SearchInfoPacket packet(SearchInfoPacket::PIT_SEARCH_INTEREST_GROUP_RESULT, sessionEncryptionKey);
        packet.SearchInterestGroupResult.result = result;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendContactChatMessagePacket(int peerSocketID, const QString &senderID, const QString &message, const QString &imageNames, const QByteArray &sessionEncryptionKey)
    {
        qDebug() << "--sendContactChatMessagePacket(...)";
        ChatMessagePacket packet(ChatMessagePacket::PIT_CONTACT_CHAT_MESSAGE, sessionEncryptionKey);
        packet.ContactChatMessage.contactID = senderID;
        packet.ContactChatMessage.message = message;
        packet.ContactChatMessage.imageNames = imageNames;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendCachedChatMessagesPacket(int peerSocketID, const QStringList &messages, const QByteArray &sessionEncryptionKey)
    {
        qDebug() << "--sendCachedChatMessagesPacket(...)";
        ChatMessagePacket packet(ChatMessagePacket::PIT_CONTACT_CHAT_MESSAGES_CACHED_ON_SERVER, sessionEncryptionKey);
        packet.ContactChatMessagesCachedOnServer.messages = messages.join(QString(UNIT_SEPARTOR));

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendCachedInterestGroupChatMessagesPacket(int peerSocketID, const QStringList &messages, const QByteArray &sessionEncryptionKey)
    {
        qDebug() << "--sendCachedInterestGroupChatMessagesPacket(...)";
        ChatMessagePacket packet(ChatMessagePacket::PIT_GROUP_CHAT_MESSAGES_CACHED_ON_SERVER, sessionEncryptionKey);
        packet.GroupChatMessagesCachedOnServer.messages = messages.join(QString(UNIT_SEPARTOR));

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendInterestGroupChatMessagesToMemberPacket(int peerSocketID, const QByteArray &sessionEncryptionKey, quint32 interestGroupID, const QString &senderID, const QString &message, const QString &imageNames)
    {
        qDebug() << "--sendInterestGroupChatMessagesToMemberPacket(...)";
        ChatMessagePacket packet(ChatMessagePacket::PIT_GROUP_CHAT_MESSAGE, sessionEncryptionKey);
        packet.GroupChatMessage.groupID = interestGroupID;
        packet.GroupChatMessage.memberID = senderID;
        packet.GroupChatMessage.message = message;
        packet.GroupChatMessage.imageNames = imageNames;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendRequestImagePacket(int peerSocketID, const QString &contactID, const QString &imageName, const QByteArray &sessionEncryptionKey)
    {
        qDebug() << "--sendRequestImagePacket(...)";
        ChatMessagePacket packet(ChatMessagePacket::PIT_CHAT_IMAGE, sessionEncryptionKey);
        packet.ChatImage.isRequest = 1;
        packet.ChatImage.contactID = contactID;
        packet.ChatImage.name = imageName;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendImagePacket(int peerSocketID, const QString &contactID, const QString &imageName, const QByteArray &image, const QByteArray &sessionEncryptionKey)
    {
        qDebug() << "--sendImagePacket(...)";
        ChatMessagePacket packet(ChatMessagePacket::PIT_CHAT_IMAGE, sessionEncryptionKey);
        packet.ChatImage.isRequest = 0;
        packet.ChatImage.contactID = contactID;
        packet.ChatImage.name = imageName;
        packet.ChatImage.image = image;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendSessionEncryptionKeyWithContact(int peerSocketID, const QString &contactID, const QByteArray &sessionEncryptionKeyWithContact, const QByteArray &sessionEncryptionKey)
    {
        qDebug() << "--sendSessionEncryptionKeyWithContact(...)";
        ChatMessagePacket packet(ChatMessagePacket::PIT_SESSION_ENCRYPTION_KEY_WITH_CONTACT, sessionEncryptionKey);
        packet.SessionEncryptionKeyWithContact.contactID = contactID;
        packet.SessionEncryptionKeyWithContact.key = sessionEncryptionKeyWithContact;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool responseFileServerInfo(SOCKETID socketID, const QString &serverAddress, quint16 serverPort, const QByteArray &sessionEncryptionKey)
    {
        FileTransferPacket packet(FileTransferPacket::FT_FILE_SERVER_INFO, sessionEncryptionKey);
        packet.FileServerInfo.address = serverAddress;
        packet.FileServerInfo.port = serverPort;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(socketID, &ba);
    }
















    void processUserOnlineStatusChanged(const  QString &userID, quint8 onlineStateCode, const QString &userHostAddress, quint16 userHostPort)
    {
        //qDebug()<<"processUserOnlineStatusChanged(...)";

        UserInfo *userInfo = getUserInfo(userID);
        return processUserOnlineStatusChanged(userInfo, onlineStateCode, userHostAddress, userHostPort);

    }

    void processUserOnlineStatusChanged(UserInfo *userInfo, quint8 onlineStateCode, const QString &userHostAddress, quint16 userHostPort, const QString &deviceInfo = "")
    {
        //qDebug()<<"processUserOnlineStatusChanged(...)";

        if(!userInfo) {
            return;
        }
        userInfo->setOnlineState(IM::OnlineState(onlineStateCode));
        if(onlineStateCode == quint8(IM::ONLINESTATE_OFFLINE)) {
            userOffline(userInfo);
            saveUserLastLogoutInfo(userInfo);
            //saveUserInfoToDatabase(userInfo);
            m_userSocketsHash.remove(userInfo->getSocketID());

            QStringList images = imageDownloadingRequestHash.keys(userInfo);
            foreach (QString imageName, images) {
                imageDownloadingRequestHash.remove(imageName, userInfo);
            }

        } else {
            userOnline(userInfo);
            saveUserLastLoginInfo(userInfo, userHostAddress, userHostPort, deviceInfo);
        }

        //通知所有在线联系人
        QStringList contacts = userInfo->getAllContacts();
        foreach (QString contact, contacts) {
            UserInfo *contactInfo = getOnlineUserInfo(contact);
            if(contactInfo) {
                sendContactOnlineStatusChangedPacket(contactInfo->getSocketID(), contactInfo->getSessionEncryptionKey(), onlineStateCode, userInfo->getUserID(),  userHostAddress, userHostPort);
            }
        }


    }





    void userExceptionalOffline(const QString &peerAddress, quint16 peerPort);




private slots:


    //TODO:Replace
    int crypto(QByteArray *destination, const QByteArray &source, const QByteArray &key, bool encrypt);

    bool encrypeData(const QString &userID, QByteArray *destination, const QByteArray &source);
    bool encrypeData(UserInfo *info, QByteArray *destination, const QByteArray &source);
    bool decryptData(const QString &userID, QByteArray *destination, const QByteArray &source);
    bool decryptData(UserInfo *info, QByteArray *destination, const QByteArray &source);

    QByteArray getUserSessionEncryptionKey(const QString &userID);

    //    void slotCheckIMUsersOnlineStatus();

    void peerDisconnected(SOCKETID socketID);

    void sendInterestGroupChatMessageToMembers(quint32 interestGroupID, UserInfo *sender, const QString &message, const QString &imageNames);

    void addContactForUser(UserInfo *userInfo, UserInfo *contactInfo, quint32 groupID);

    void processServerDiscoveryPacket(const ServerDiscoveryPacket &packet);
    void processRgeistrationPacket(const RgeistrationPacket &packet);
    void processUpdatePasswordPacket(const UpdatePasswordPacket &packet);
    void processLoginPacket(const LoginPacket &packet);
    void processContactGroupsInfoPacket(const ContactGroupsInfoPacket &packet);
    void processInterestGroupsInfoPacket(const InterestGroupsInfoPacket &packet);
    void processContactInfoPacket(const ContactInfoPacket &packet);
    void processSearchInfoPacket(const SearchInfoPacket &packet);
    void processChatMessagePacket(const ChatMessagePacket &packet);
    void processCaptchaInfoPacket(const CaptchaInfoPacket &packet);
    void processFileTransferPacket(const FileTransferPacket &packet);




signals:

    void signalClientLookForServerPacketReceived(const QHostAddress &clientAddress, quint16 clientTCPListeningPort, const QString &clientName);


private:
    UserInfo *logUserIn(const QString &userID, const QByteArray &encryptedPassword, IM::OnlineState loginState, IM::ErrorType *errorType);



private:

    QString m_serverName;
    quint16 m_localRTPListeningPort;

    QString m_chatImageCacheDir;


    UDPServer *m_ipmcServer;
    RTP *m_rtp;
//    UDTProtocol *m_udtProtocol;
    TCPServer *m_tcpServer;
    ENETProtocol *m_enetProtocol;

    QHash<int /*Socket ID*/, UserInfo * /*UserInfo*/> m_userSocketsHash;
    QMultiHash<QString/*Image Name*/, UserInfo * /*UserInfo*/> imageDownloadingRequestHash;


    Cryptography *cryptography;

    QMutex mutex;

//    QHash<QString/*User ID*/, QByteArray/*Key*/> sessionEncryptionKeysHash;

    QTimer *checkIMUsersOnlineStateTimer;

    int m_serverInstanceID;




};

}

#endif /* SERVERPACKETSPARSER_H_ */
