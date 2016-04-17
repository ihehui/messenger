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



namespace HEHUI {


class ServerPacketsParser : public QObject, public UsersManager{
    Q_OBJECT
public:
    ServerPacketsParser(ResourcesManagerInstance *resourcesManager, QObject *parent = 0);
    virtual ~ServerPacketsParser();








public slots:
    void parseIncomingPacketData(PacketBase *packet);


    bool sendServerDeclarePacket(const QHostAddress peerAddress, quint16 peerPort){

        qsrand(QDateTime::currentDateTime().toTime_t());
        static int serverInstanceID = qrand();
        //qDebug()<<"Server Instance ID:"<<serverInstanceID;

        ServerDiscoveryPacket packet;
        packet.responseFromServer = 1;
        packet.version = QString(APP_VERSION);
        packet.rtpPort = localRTPListeningPort;
        packet.tcpPort = m_localTCPServerListeningPort;
        packet.serverInstanceID = serverInstanceID;

        return m_ipmcServer->sendDatagram(packet.toByteArray(), peerAddress, peerPort);
    }

    bool forwardData(int peerSocketID, const QByteArray &data, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--forwardData(...)";

        DataForwardPacket p(sessionEncryptionKey);
        p.data = data;

        QByteArray ba = p.toByteArray();
        if(ba.isEmpty()){return false;}

        return m_rtp->sendReliableData(peerSocketID, &ba);
    }

    bool sendServerAnnouncementPacket(const QHostAddress peerAddress, quint16 peerPort, const QString &announcement, bool mustRead = true){
        qDebug()<<"----sendServerAnnouncementPacket(...)";


        AnnouncementPacket packet(QByteArray());
        packet.InfoType = AnnouncementPacket::ANNOUNCEMENT_INFO;
        packet.AnnouncementInfo.announcementID = ;
        packet.AnnouncementInfo.content = "";

        QByteArray ba;


        out << m_serverName << announcement << (mustRead?quint8(1):quint8(0));
        packet->setPacketData(ba);

        QByteArray ba = p.toByteArray();
        if(ba.isEmpty()){return false;}

        return m_ipmcServer->sendDatagram(ba, peerAddress, peerPort);

    }


    bool sendRegistrationServerInfoPacket(int peerSocketID, const RgeistrationPacket &peerPacket){
        qDebug()<<"--sendRegistrationServerInfoPacket(...)";

        //TODO
        RgeistrationPacket packet(QByteArray());
        packet.InfoType = RgeistrationPacket::REGISTRATION_SERVER_INFO;
        packet.ServerInfo.requestServerInfo = 0;
        packet.ServerInfo.version = APP_VERSION;
        packet.ServerInfo.regMode = quint8(RgeistrationPacket::REG_MODE_USER_CREATE_ALL);
        packet.ServerInfo.activationMode = quint8(RgeistrationPacket::ACT_MODE_AUTO);;
        packet.ServerInfo.message = "";
        packet.ServerInfo.address = 0;
        packet.ServerInfo.port = 0;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendClientRegistrationResultPacket(int peerSocketID, quint32 sysID, const QString &userID, const QString &password, quint8 errorTypeCode){
        qWarning()<<"--sendClientRegistrationResultPacket(...)";
        
        //TODO
        RgeistrationPacket packet(QByteArray());
        packet.InfoType = RgeistrationPacket::REGISTRATION_RESULT;
        packet.RgeistrationResult.sysID = sysID;
        packet.RgeistrationResult.userID = userID;
        packet.RgeistrationResult.password = password;
        packet.RgeistrationResult.errorCode = errorTypeCode;
        packet.RgeistrationResult.activationMode = UsersManager::activationMode();

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendUpdatePasswordAuthInfoPacket(int peerSocketID, quint8 authMode, const QByteArray &captchaImage, const QString &userID, const QString &securityQuestion, const QString &email, const QString &cellphoneNO, const QString &url){
        qWarning()<<"--sendUpdatePasswordAuthInfoPacket(...)";

        //TODO
        UpdatePasswordPacket packet(QByteArray());
        packet.InfoType = UpdatePasswordPacket::INFO_TYPE_AUTH_INFO_FROM_SERVER;
        packet.AuthInfo.authMode = authMode;
        packet.AuthInfo.captchaImage = captchaImage;
        packet.AuthInfo.userID = userID;
        packet.AuthInfo.securityQuestion = securityQuestion;
        packet.AuthInfo.email = email;
        packet.AuthInfo.cellphoneNO =cellphoneNO;
        packet.AuthInfo.url = url;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendClientUpdatePasswordResultPacket(int peerSocketID, const QString &userID, quint8 updateResult){
        qDebug()<<"--sendClientUpdatePasswordResultPacket(...)";
        
        UpdatePasswordPacket packet(getUserSessionEncryptionKey(userID));
        packet.InfoType = UpdatePasswordPacket::INFO_TYPE_UPDATE_RESULT;
        packet.AuthInfo.updateResult = updateResult;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendClientCanLoginPacket(int peerSocketID, const QString &userID, const QHostAddress &serverAddress, quint16 serverPort){
        qDebug()<<"--sendClientCanLoginPacket(...)";

        LoginPacket packet(getUserSessionEncryptionKey(userID));
        packet.InfoType = LoginPacket::INFO_TYPE_LOGIN_SERVER_INFO;
        packet.LoginServerInfo.serverAddress = serverAddress.toString();
        packet.LoginServerInfo.serverPort = serverPort;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendClientLoginFailedPacket(int peerSocketID, const QString &userID, quint8 errorTypeCode, const QString &errorMessage = ""){
        qDebug()<<"--sendClientLoginFailedPacket(...)";
        
        LoginPacket packet(getUserSessionEncryptionKey(userID));
        packet.InfoType = LoginPacket::INFO_TYPE_LOGIN_RESULT;
        packet.AuthResultInfo.loggedin = 0;
        packet.AuthResultInfo.errorType = errorTypeCode;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendClientLoginSucceededPacket(int peerSocketID, const QString &userID, const QByteArray &encryptedPassword, const QByteArray &sessionEncryptionKey, quint32 personalSummaryInfoVersion,
                                        quint32 personalDetailInfoVersionOnServer, quint32 personalContactGroupsInfoVersionOnServer,
                                        quint32 interestGroupInfoVersionOnServer, quint32 blacklistInfoVersionOnServer){

        qDebug()<<"--sendClientLoginSucceededPacket(...)";
        
        LoginPacket packet(getUserSessionEncryptionKey(userID));
        packet.InfoType = LoginPacket::INFO_TYPE_LOGIN_RESULT;
        packet.AuthResultInfo.loggedin = 0;
        packet.AuthResultInfo.errorType = quint8(IM::ERROR_NoError);

        packet.AuthResultInfo.sessionEncryptionKey = sessionEncryptionKey;
        packet.AuthResultInfo.personalSummaryInfoVersionOnServer = personalSummaryInfoVersion;
        packet.AuthResultInfo.personalDetailInfoVersionOnServer = personalDetailInfoVersionOnServer;
        packet.AuthResultInfo.personalContactGroupsInfoVersionOnServer = personalContactGroupsInfoVersionOnServer;
        packet.AuthResultInfo.interestGroupsInfoVersionOnServer = interestGroupInfoVersionOnServer;
        packet.AuthResultInfo.personalMessageInfoVersionOnServer = blacklistInfoVersionOnServer;

        uint serverTime = QDateTime::currentDateTime().toUTC().toTime_t();
        packet.AuthResultInfo.serverTime = serverTime;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendClientLastLoginInfoPacket(int peerSocketID, const QByteArray &sessionEncryptionKey, const QString &extIPAddress, uint loginTime, uint logoutTime, const QString &clientVersion, const QString &deviceInfo ){
        qDebug()<<"--sendClientLastLoginInfoPacket(...)";

        LoginPacket packet(sessionEncryptionKey);
        packet.InfoType = LoginPacket::INFO_TYPE_PREVIOUS_LOGIN_INFO;
        packet.PreviousLoginInfo.loginIP = extIPAddress;
        packet.PreviousLoginInfo.loginTime = loginTime;
        packet.PreviousLoginInfo.logoutTime = logoutTime;
        packet.PreviousLoginInfo.clientVersion = clientVersion;
        packet.PreviousLoginInfo.deviceInfo = deviceInfo;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendContactOnlineStatusChangedPacket(int peerSocketID, const QByteArray &sessionEncryptionKey, quint8 onlineStateCode, const QString &contactID, const QString &contactHostAddress, quint16 contactHostPort){
        qDebug()<<"--sendContactOnlineStatusChangedPacket(...)";

        OnlineStateChangedPacket packet(sessionEncryptionKey);
        packet.stateCode = onlineStateCode;
        packet.contactID = contactID;
        packet.contactIP = contactHostAddress;
        packet.contactPort = contactHostPort;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendContactsOnlineInfo(int peerSocketID, UserInfo *userInfo){
        if(!userInfo){
            return false;
        }

        QStringList contacts = userInfo->getAllContacts();
        QStringList contactsOnlineInfo;
        foreach (QString contactID, contacts) {
            UserInfo *info = getOnlineUserInfo(contactID);
            if(info){
                IM::OnlineState state = info->getOnlineState();
                if(state != IM::ONLINESTATE_OFFLINE && state != IM::ONLINESTATE_INVISIBLE){
                    QStringList onlineInfo;
                    onlineInfo.append(contactID);
                    onlineInfo.append(QString::number(quint8(state)));
                    onlineInfo.append(info->getLastLoginExternalHostAddress());
                    onlineInfo.append(QString::number(info->getLastLoginExternalHostPort()));
                    contactsOnlineInfo.append(onlineInfo.join(QString(PACKET_DATA_SEPARTOR)));
                }

            }
        }
        if(contactsOnlineInfo.isEmpty()){return false;}

        OnlineContacts packet(userInfo->getSessionEncryptionKey());
        packet.contactsOnlineInfoString = contactsOnlineInfo.join(QString(UNIT_SEPARTOR));

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendUserInterestGroupsListPacket(int peerSocketID, UserInfo *userInfo){
        InterestGroupsInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = InterestGroupsInfoPacket::PIT_GROUPS_LIST;
        packet.GroupsList.groups = getInterestGroupsListForUser(userInfo);
        packet.GroupsList.version = userInfo->getInterestGroupInfoVersion();

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendUserInterestGroupInfoPacket(int peerSocketID, UserInfo *userInfo, quint32 groupID){
        InterestGroupsInfoPacket packet(userInfo->getSessionEncryptionKey());
        packet.InfoType = InterestGroupsInfoPacket::PIT_GROUP_INFO;
        packet.GroupInfo.id = groupID;
        packet.GroupInfo.infoString = getInterestGroupInfoStringForUser(userInfo, groupID);

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendCreateInterestGroupResultPacket(int peerSocketID, UserInfo *userInfo, quint32 groupID, const QString &groupName){
        qDebug()<<"--sendCreateInterestGroupResultPacket() "<<" groupID:"<<groupID<<" peerSocketID:"<<peerSocketID;
        InterestGroupsInfoPacket packet(userInfo->getSessionEncryptionKey());
        packet.InfoType = InterestGroupsInfoPacket::PIT_GROUP_CREATION;
        packet.GroupCreationInfo.id = groupID;
        packet.GroupCreationInfo.name = groupName;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendPersonalContactGroupsInfoPacket(int peerSocketID, const QString &contactGroupsInfo, quint32 personalContactGroupsInfoVersionOnServer, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendPersonalContactGroupsInfoPacket(...)";
        ContactGroupsInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = ContactGroupsInfoPacket::PIT_GROUPS_LIST;
        packet.GroupsList.groupsInfo = contactGroupsInfo;
        packet.GroupsList.version = personalContactGroupsInfoVersionOnServer;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendCreateContactGroupResultPacket(int peerSocketID, quint32 groupID, const QString &groupName, quint8 errorCode, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendCreateContactGroupResultPacket(...)";
        ContactGroupsInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = ContactGroupsInfoPacket::PIT_GROUP_CREATION;
        packet.GroupCreationInfo.name = groupName;
        packet.GroupCreationInfo.groupID = groupID;
        packet.GroupCreationInfo.errorCode = errorCode;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendDeleteContactGroupResultPacket(int peerSocketID, quint32 groupID, const QString &groupName, quint8 errorCode, bool createGroup, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendDeleteContactGroupResultPacket(...)";
        ContactGroupsInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = ContactGroupsInfoPacket::PIT_GROUP_DELETION;
        packet.GroupDeletionInfo.groupID = groupID;
        packet.GroupDeletionInfo.errorCode = errorCode;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendDisbandInterestGroupResultPacket(int peerSocketID, UserInfo *userInfo, quint32 groupID, bool result){
        qDebug()<<"--sendDisbandInterestGroupResultPacket() "<<" groupID:"<<groupID<<" peerSocketID:"<<peerSocketID;
        InterestGroupsInfoPacket packet(userInfo->getSessionEncryptionKey());
        packet.InfoType = InterestGroupsInfoPacket::PIT_GROUP_DELETION;
        packet.GroupDeletionInfo.id = groupID;
        packet.GroupDeletionInfo.deleted = result;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendUserJoinInterestGroupResultToUserPacket(int peerSocketID, quint32 groupID, const QString &memberID, bool approved, const QString &message, const QString &admin, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendUserJoinInterestGroupResultToUserPacket(...)";
        InterestGroupsInfoPacket packet(sessionEncryptionKey);
        packet.GroupID = groupID;
        packet.InfoType = InterestGroupsInfoPacket::PIT_GROUP_MEMBER_APPLICATION;
        packet.MemberApplicationInfo.userID = memberID;
        packet.MemberApplicationInfo.message = message;
        packet.MemberApplicationInfo.approved = approved;
        packet.MemberApplicationInfo.admin = admin;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendUserJoinInterestGroupRequestToAdminPacket(int peerSocketID, quint32 groupID, const QString &userID, const QString &verificationMessage){
        qDebug()<<"--sendUserJoinInterestGroupRequestToAdminPacket(...)";
        InterestGroupsInfoPacket packet(sessionEncryptionKey);
        packet.GroupID = groupID;
        packet.InfoType = InterestGroupsInfoPacket::PIT_GROUP_MEMBER_APPLICATION;
        packet.MemberApplicationInfo.userID = userID;
        packet.MemberApplicationInfo.message = verificationMessage;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendUserInfoPacket(int peerSocketID, const QString &contactID, bool isSummaryInfo, const QString &infoString, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendUserInfoPacket(...)";
        ContactInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = ContactInfoPacket::PIT_CONTACT_INFO;
        packet.ContactID = contactID;
        packet.info.isSummaryInfo = quint8(isSummaryInfo?1:0);
        packet.info.infoString = infoString;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendAddContactRequestFromUserPacket(int peerSocketID, const QString &userID, const QString &verificationMessage, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendAddContactRequestFromUserPacket(...)";

        ContactInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = ContactInfoPacket::PIT_FRIENDING_REQUEST;
        packet.ContactID = userID;
        packet.ContactFriendingRequest.message = verificationMessage;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool sendAddContactResultPacket(int peerSocketID, const QString &contactID, quint8 errorCode, const QString &reasonMessage, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendAddContactResultPacket(...)";
        ContactInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = ContactInfoPacket::PIT_FRIENDING_RESULT;
        packet.ContactID = contactID;
        packet.ContactFriendingResult.errorCode = errorCode;
        packet.ContactFriendingResult.message = reasonMessage;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }


    bool sendDeleteContactResultPacket(int peerSocketID, const QString &contactID, bool blockForever, quint8 errorCode, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendDeleteContactResultPacket(...)";
        ContactInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = ContactInfoPacket::PIT_CONTACT_DELETION;
        packet.ContactID = contactID;
        packet.ContactDeletionInfo.blockForever = blockForever;
        packet.ContactDeletionInfo.errorCode = errorCode;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendSearchContactResultPacket(int peerSocketID, const QString &result, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendSearchContactResultPacket(...)";
        SearchInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = SearchInfoPacket::PIT_SEARCH_CONTACT_RESULT;
        packet.SearchContactResult.result = result;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendSearchInterestGroupResultPacket(int peerSocketID, const QString &result, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendSearchInterestGroupResultPacket(...)";
        SearchInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = SearchInfoPacket::PIT_SEARCH_INTEREST_GROUP_RESULT;
        packet.SearchInterestGroupResult.result = result;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendCachedChatMessagesPacket(int peerSocketID, const QStringList &messages, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendCachedChatMessagesPacket(...)";
        ChatMessagePacket packet(sessionEncryptionKey);
        packet.InfoType = ChatMessagePacket::PIT_CONTACT_CHAT_MESSAGES_CACHED_ON_SERVER;
        packet.ContactChatMessagesCachedOnServer.messages = messages.join(QString(UNIT_SEPARTOR));

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendCachedInterestGroupChatMessagesPacket(int peerSocketID, const QStringList &messages, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendCachedInterestGroupChatMessagesPacket(...)";
        ChatMessagePacket packet(sessionEncryptionKey);
        packet.InfoType = ChatMessagePacket::PIT_GROUP_CHAT_MESSAGES_CACHED_ON_SERVER;
        packet.GroupChatMessagesCachedOnServer.messages = messages.join(QString(UNIT_SEPARTOR));

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendInterestGroupChatMessagesToMemberPacket(int peerSocketID, quint32 interestGroupID, const QString &senderID, const QString &message, const QStringList &imageNameList, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendInterestGroupChatMessagesToMemberPacket(...)";
        ChatMessagePacket packet(sessionEncryptionKey);
        packet.InfoType = ChatMessagePacket::PIT_GROUP_CHAT_MESSAGE;
        packet.GroupChatMessage.groupID = interestGroupID;
        packet.GroupChatMessage.memberID = senderID;
        packet.GroupChatMessage.message = message;
        packet.GroupChatMessage.imageNames = imageNameList.join(",");;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendRequestImagePacket(int peerSocketID, const QString &imageName, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendRequestImagePacket(...)";
        ChatMessagePacket packet(sessionEncryptionKey);
        packet.InfoType = ChatMessagePacket::PIT_CHAT_IMAGE;
        packet.ChatImage.isRequest = 1;
        packet.ChatImage.name = imageName;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendImagePacket(int peerSocketID, const QString &imageName, const QByteArray &image, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendImagePacket(...)";
        ChatMessagePacket packet(sessionEncryptionKey);
        packet.InfoType = ChatMessagePacket::PIT_CHAT_IMAGE;
        packet.ChatImage.isRequest = 0;
        packet.ChatImage.name = imageName;
        packet.ChatImage.image = image;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }




















    void processUserOnlineStatusChanged(const  QString &userID, quint8 onlineStateCode, const QString &userHostAddress, quint16 userHostPort){
        //qDebug()<<"processUserOnlineStatusChanged(...)";
        
        UserInfo *userInfo = getUserInfo(userID);
        return processUserOnlineStatusChanged(userInfo, onlineStateCode, userHostAddress, userHostPort);

    }
    
    void processUserOnlineStatusChanged(UserInfo *userInfo, quint8 onlineStateCode, const QString &userHostAddress, quint16 userHostPort, const QString &deviceInfo = ""){
        //qDebug()<<"processUserOnlineStatusChanged(...)";
        
        
        if(!userInfo){return;}
        userInfo->setOnlineState(IM::OnlineState(onlineStateCode));
        if(onlineStateCode == quint8(IM::ONLINESTATE_OFFLINE)){
            userOffline(userInfo);
            saveUserLastLogoutInfo(userInfo);
            //saveUserInfoToDatabase(userInfo);
            m_userSocketsHash.remove(userInfo->getSocketID());

            QStringList images = imageDownloadingRequestHash.keys(userInfo);
            foreach (QString imageName, images) {
                imageDownloadingRequestHash.remove(imageName, userInfo);
            }

        }else{
            userOnline(userInfo);
            saveUserLastLoginInfo(userInfo, userHostAddress, userHostPort, deviceInfo);
        }

        //通知所有在线联系人
        QStringList contacts = userInfo->getAllContacts();
        foreach (QString contact, contacts) {
            UserInfo *contactInfo = getOnlineUserInfo(contact);
            if(contactInfo){
                sendContactOnlineStatusChangedPacket(contactInfo->getSocketID(), userInfo->getUserID(), onlineStateCode, contactInfo->getSessionEncryptionKey(), userHostAddress, userHostPort, contactInfo->getLastLoginExternalHostAddress(), contactInfo->getLastLoginExternalHostPort());
            }
        }


    }




















    bool sendSessionEncryptionKeyWithContact(int peerSocketID, const QString &contactID, const QByteArray &encryptedPassword, const QByteArray &sessionEncryptionKeyWithContact, const QString &targetHostAddress, quint16 targetHostPort){
        qDebug()<<"--sendSessionEncryptionKeyWithContact(...)";
        
        PacketBase *packet = PacketHandlerBase::getPacket();
        packet->setPacketType(quint8(IM::SESSION_ENCRYPTION_KEY_WITH_CONTACT));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;



        out << contactID << sessionEncryptionKeyWithContact;
        QByteArray encryptedData;
        crypto(&encryptedData, ba, encryptedPassword, true);
        ba.clear();
        out.device()->seek(0);
        out << m_serverName << encryptedData;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;
        return m_rtp->sendReliableData(peerSocketID, &ba);

    }





    bool sendContactChatMessagePacket(int peerSocketID, const QString &senderID, const QString &message, const QString &imageNames, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendContactChatMessagePacket(...)";
        ChatMessagePacket packet(sessionEncryptionKey);
        packet.InfoType = ChatMessagePacket::PIT_CONTACT_CHAT_MESSAGE;
        packet.ContactChatMessage.contactID = senderID;
        packet.ContactChatMessage.message = message;
        packet.ContactChatMessage.imageNames = imageNames;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }









    bool sendFileServerInfoToUser(int peerSocketID, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendFileServerInfoToUser(...)";

        qCritical()<<"-------!!!!-------TODO:Get File Server Info!-------!!!!-------";
        quint32 ipv4Address = 0;
        quint16 port = m_localRTPListeningPort;


        //TODO:缓存消息的格式
        PacketBase *packet = PacketHandlerBase::getPacket();
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_FILE_SERVER_INFO));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;



        out << ipv4Address << port;

        QByteArray encryptedData;
        crypto(&encryptedData, ba, sessionEncryptionKey, true);
        ba.clear();
        out.device()->seek(0);
        out << m_serverName << encryptedData;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;
        return m_rtp->sendReliableData(peerSocketID, &ba);

    }




    void userExceptionalOffline(const QString &peerAddress, quint16 peerPort);




private slots:
    //HeartbeatPacket: PacketType+ComputerName
    //    void startHeartbeat(int interval = HEARTBEAT_TIMER_INTERVAL);
    //    void stopHeartbeat();


    //TODO:Replace
    int crypto(QByteArray *destination, const QByteArray &source, const QByteArray &key, bool encrypt);

    bool encrypeData(const QString &userID, QByteArray *destination, const QByteArray &source);
    bool encrypeData(UserInfo *info, QByteArray *destination, const QByteArray &source);
    bool decryptData(const QString &userID, QByteArray *destination, const QByteArray &source);
    bool decryptData(UserInfo *info, QByteArray *destination, const QByteArray &source);

    QByteArray getUserSessionEncryptionKey(const QString &userID);

    //    void slotCheckIMUsersOnlineStatus();

    void peerDisconnected(int socketID);

    void sendInterestGroupChatMessageToMembers(quint32 interestGroupID, UserInfo *sender, const QString &message);

    void addContactForUser(UserInfo *userInfo, UserInfo *contactInfo, quint32 groupID);


    void processRgeistrationPacket(const RgeistrationPacket &packet);
    void processUpdatePasswordPacket(const UpdatePasswordPacket &packet);
    void processLoginPacket(const LoginPacket &packet);
    void processContactGroupsInfoPacket(const ContactGroupsInfoPacket &packet);
    void processInterestGroupsInfoPacket(const InterestGroupsInfoPacket &packet);
    void processContactInfoPacket(const ContactInfoPacket &packet);
    void processSearchInfoPacket(const SearchInfoPacket &packet);
    void processChatMessagePacket(const ChatMessagePacket &packet);




signals:
    void  signalHeartbeatPacketReceived(const QString &clientAddress, const QString &userID);
    void  signalConfirmationOfReceiptPacketReceived(quint16 packetSerialNumber1, quint16 packetSerialNumber2);

    void signalClientLookForServerPacketReceived(const QHostAddress &clientAddress, quint16 clientTCPListeningPort, const QString &clientName);

    //    void signalUserRequestLoginPacketReceived(const QString &userID, const QString &clientVersion);
    //    void signalUserLoginInfoPacketReceived(const QString &userID, QByteArray encryptedPassword, IM::OnlineState state);
    //    void signalUserOnlineStateChanged(const QString &userID, const QByteArray &encryptedOnlineState);
    //    void signalUserRequestUserInfo(const QString &userID, const QByteArray &encryptedUserID);

    //    void signalUserUpdatePassword(const QString &userID, const QByteArray &encryptedNewPassword);


private:
    UserInfo* logUserIn(const QString &userID, const QByteArray &encryptedPassword, IM::OnlineState loginState, IM::ErrorType *errorType);



private:
    //    QHostAddress serverTCPListeningAddress;
    //    quint16 serverTCPListeningPort;
    QString m_serverName;
    quint16 m_localRTPListeningPort;

    QString m_chatImageCacheDir;


    //    QTimer *heartbeatTimer;
    //        QTimer *processWaitingForReplyPacketsTimer;

    UDPServer *m_ipmcServer;
    RTP *m_rtp;
    UDTProtocol *m_udtProtocol;
    TCPServer *m_tcpServer;
    QHash<int /*Socket ID*/, UserInfo* /*UserInfo*/> m_userSocketsHash;
    QMultiHash<QString/*Image Name*/, UserInfo* /*UserInfo*/> imageDownloadingRequestHash;


    UsersManager usersManager;
    Cryptography *cryptography;

    QMutex mutex;

//    QHash<QString/*User ID*/, QByteArray/*Key*/> sessionEncryptionKeysHash;

    QTimer *checkIMUsersOnlineStateTimer;




};

}

#endif /* SERVERPACKETSPARSER_H_ */
