/*
 ****************************************************************************
 * serverpacketsparser.cpp
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

#include <QDebug>

#include "serverpacketsparser.h"
#include "../serverutilities.h"

#include "../settings.h"


namespace HEHUI
{


ServerPacketsParser::ServerPacketsParser(ResourcesManagerInstance *resourcesManager, QObject *parent)
    : QObject(parent), UsersManager(parent)
{

    Q_ASSERT(resourcesManager);

    m_ipmcServer = resourcesManager->getIPMCServer();
    Q_ASSERT_X(m_ipmcServer, "ClientPacketsParser::ClientPacketsParser(...)", "Invalid IP MC Server!");
    connect(m_ipmcServer, SIGNAL(packetReceived(const PacketBase &)), this, SLOT(parseIncomingPacketData(const PacketBase &)), Qt::QueuedConnection);


    m_rtp = resourcesManager->getRTP();
    Q_ASSERT(m_rtp);
    connect(m_rtp, SIGNAL(disconnected(SOCKETID)), this, SLOT(peerDisconnected(SOCKETID)));

//    m_udtProtocol = m_rtp->getUDTProtocol();
//    Q_ASSERT(m_udtProtocol);
//    connect(m_udtProtocol, SIGNAL(packetReceived(const PacketBase &)), this, SLOT(parseIncomingPacketData(const PacketBase &)), Qt::QueuedConnection);
//    m_udtProtocol->startWaitingForIOInOneThread(1);
//    //m_udtProtocol->startWaitingForIOInSeparateThread(100, 1000);

    m_tcpServer = m_rtp->getTCPServer();
    Q_ASSERT(m_tcpServer);
    connect(m_tcpServer, SIGNAL(packetReceived(const PacketBase &)), this, SLOT(parseIncomingPacketData(const PacketBase &)), Qt::QueuedConnection);

    m_enetProtocol = m_rtp->getENETProtocol();
    Q_ASSERT(m_enetProtocol);
    connect(m_enetProtocol, SIGNAL(packetReceived(const PacketBase &)), this, SLOT(parseIncomingPacketData(const PacketBase &)), Qt::QueuedConnection);





    //    heartbeatTimer = 0;


    //m_serverName = networkManager->hostName();
    m_serverName = QHostInfo::localHostName().toLower();
    m_localRTPListeningPort = m_rtp->getTCPServerPort();

    Settings settings;
    m_chatImageCacheDir = settings.getChatImageCacheDir();

    //    usersManager = UsersManager::instance();
    cryptography = new Cryptography();

    checkIMUsersOnlineStateTimer = 0;




}

ServerPacketsParser::~ServerPacketsParser()
{
    // TODO Auto-generated destructor stub

    QMutexLocker locker(&mutex);

    if(checkIMUsersOnlineStateTimer) {
        checkIMUsersOnlineStateTimer->stop();
    }
    delete checkIMUsersOnlineStateTimer;
    checkIMUsersOnlineStateTimer = 0;

    //    if(heartbeatTimer){
    //        heartbeatTimer->stop();
    //    }
    //    delete heartbeatTimer;
    //    heartbeatTimer = 0;


    delete cryptography;
    cryptography = 0;

}



void ServerPacketsParser::parseIncomingPacketData(const PacketBase &packet)
{

    //    qDebug()<<"----ServerPacketsParser::parseIncomingPacketData(Packet packet)";


    QString peerID = packet.getPeerID();

    QHostAddress peerAddress = packet.getPeerHostAddress();
    quint16 peerPort = packet.getPeerHostPort();
    quint8 packetType = packet.getPacketType();
    int socketID = packet.getSocketID();

    QByteArray sessionEncryptionKey = getUserSessionEncryptionKey(peerID);

    qDebug() << "--ServerPacketsParser::parseIncomingPacketData(...) " << " socketID:" << socketID << " peerID:" << peerID << " peerAddress:" << peerAddress.toString() << " peerPort:" << peerPort << " packetType:" << packetType;


    switch(packetType) {

    case quint8(IM::CMD_ServerDiscovery): {
        qDebug() << "~~ClientLookForServer";
        ServerDiscoveryPacket p(packet);
        sendServerDeclarePacket(peerAddress, peerPort);
        //qDebug()<<"~~ClientLookForServer--"<<" peerAddress:"<<peerAddress.toString()<<"   peerPort:"<<peerPort;
    }
    break;

    case quint8(IM::CMD_DataForward): {
        qDebug() << "~~DataForwardRequestByClient";

        UserInfo *senderInfo = getOnlineUserInfo(peerID);
        if(!senderInfo) {
            qCritical() << "ERROR! Sender Not Found!";
            return;
        }

        DataForwardPacket p(packet, senderInfo->getSessionEncryptionKey());
        if(!p.isValid()) {
            qCritical() << "ERROR! Invalid Packet!";
            return;
        }

        UserInfo *receiverInfo = getUserInfo(p.receiver);
        if(!receiverInfo) {
            qCritical() << "ERROR! Receiver Not Found!";
            return;
        }
        if(!receiverInfo->isOnLine()) {
            qCritical() << "ERROR! Receiver Offline!";
            return;
        }

        forwardData(m_userSocketsHash.key(receiverInfo), p.data, receiverInfo->getSessionEncryptionKey());

    }
    break;


    case quint8(IM::CMD_Announcement): {
        AnnouncementPacket p(packet, sessionEncryptionKey);
        qDebug() << "~~CMD_Announcement";
        //TODO

    }
    break;

    case quint8(IM::CMD_Rgeistration): {
        RgeistrationPacket p(packet, sessionEncryptionKey);
        qDebug() << "~~CMD_Rgeistration";
        //TODO
        processRgeistrationPacket(p);
    }
    break;

    case quint8(IM::CMD_UpdatePassword): {
        UpdatePasswordPacket p(packet, sessionEncryptionKey);
        p.AuthInfo.authMode = UsersManager::passwordUpdateAuthMode();

        processUpdatePasswordPacket(p);
        qDebug() << "~~CMD_UpdatePassword";
    }
    break;

    case quint8(IM::CMD_Login): {
        LoginPacket p(packet, sessionEncryptionKey);
        processLoginPacket(p);
        qDebug() << "~~CMD_Login";
    }
    break;

    case quint8(IM::CMD_OnlineStateChanged): {
        OnlineStateChangedPacket p(packet, sessionEncryptionKey);
        quint8 stateCode = p.stateCode;
        processUserOnlineStatusChanged(peerID, IM::OnlineState(stateCode), peerAddress.toString(), peerPort);
        qDebug() << "~~CMD_OnlineStateChanged";
    }
    break;

    case quint8(IM::CMD_ContactGroupsInfo): {
        ContactGroupsInfoPacket p(packet, sessionEncryptionKey);
        processContactGroupsInfoPacket(p);
        qDebug() << "~~CMD_ContactGroupsInfo";
    }
    break;

    case quint8(IM::CMD_InterestGroupsInfo): {
        InterestGroupsInfoPacket p(packet, sessionEncryptionKey);
        processInterestGroupsInfoPacket(p);
        qDebug() << "~~CMD_InterestGroupsInfo";
    }
    break;

    case quint8(IM::CMD_ContactInfo): {
        ContactInfoPacket p(packet, sessionEncryptionKey);
        processContactInfoPacket(p);
        qDebug() << "~~CMD_ContactInfo";
    }
    break;

    case quint8(IM::CMD_SearchInfo): {
        SearchInfoPacket p(packet, sessionEncryptionKey);
        processSearchInfoPacket(p);
        qDebug() << "~~CMD_SearchInfo";
    }
    break;

    case quint8(IM::CMD_Captcha): {
        CaptchaInfoPacket p(packet, sessionEncryptionKey);
        processCaptchaInfoPacket(p);
        qDebug() << "~~CMD_Captcha";
    }
    break;

    //File TX
    case quint8(IM::CMD_FileTransfer): {
        FileTransferPacket p(packet, sessionEncryptionKey);
        processFileTransferPacket(p);
        qDebug() << "~~CMD_FileTransfer";
    }
    break;






    default:
        qWarning() << "Unknown Packet Type:" << packetType
                   << " From:" << peerAddress.toString()
                   << " Port:" << peerPort;
        break;

    }



}


void ServerPacketsParser::userExceptionalOffline(const QString &peerAddress, quint16 peerPort)
{
    qDebug() << "--userExceptionalOffline(...)" << " peerAddress:" << peerAddress << " peerPort:" << peerPort;

    QList<UserInfo *>  users = onlineUsers();
    foreach(UserInfo *info, users) {
        if(info->getLastLoginExternalHostAddress() == peerAddress && info->getLastLoginExternalHostPort() == peerPort) {
            processUserOnlineStatusChanged(info, IM::ONLINESTATE_OFFLINE, peerAddress, peerPort);
        }
    }

}

//void ServerPacketsParser::startHeartbeat(int interval){
//    if(NULL == heartbeatTimer){
//        heartbeatTimer = new QTimer();
//        heartbeatTimer->setSingleShot(false);
//        heartbeatTimer->setInterval(interval);
//        connect(heartbeatTimer, SIGNAL(timeout()), this, SLOT(heartbeat()));

//    }else{
//        heartbeatTimer->stop();
//        heartbeatTimer->setInterval(interval);

//    }

//    heartbeatTimer->start();


//}

//void ServerPacketsParser::stopHeartbeat(){

//    heartbeatTimer->stop();
//}

int ServerPacketsParser::crypto(QByteArray *destination, const QByteArray &source, const QByteArray &key, bool encrypt)
{
    return cryptography->teaCrypto(destination, source, key, encrypt);
}

bool ServerPacketsParser::encrypeData(const QString &userID, QByteArray *destination, const QByteArray &source)
{

    UserInfo *info = getUserInfo(userID);
    return encrypeData(info, destination, source);
}

bool ServerPacketsParser::encrypeData(UserInfo *info, QByteArray *destination, const QByteArray &source)
{

    if(!info) {
        return false;
    }
    return cryptography->teaCrypto(destination, source, info->getSessionEncryptionKey(), true);
}

bool ServerPacketsParser::decryptData(const QString &userID, QByteArray *destination, const QByteArray &source)
{

    UserInfo *info = getUserInfo(userID);

    return decryptData(info, destination, source);

}

bool ServerPacketsParser::decryptData(UserInfo *info, QByteArray *destination, const QByteArray &source)
{

    if(!info) {
        return false;
    }

    //TODO 验证数据是否解密成功  //qCritical()<<"Data Decryption Failed!";
    return cryptography->teaCrypto(destination, source, info->getSessionEncryptionKey(), false);

}

QByteArray ServerPacketsParser::getUserSessionEncryptionKey(const QString &userID)
{
    UserInfo *info = getUserInfo(userID);
    if(!info) {
        return QByteArray();
    }

    return info->getSessionEncryptionKey();
}

void ServerPacketsParser::peerDisconnected(SOCKETID socketID)
{
    //qDebug()<<"--ServerPacketsParser::peerDisconnected(...) "<<" socketID:"<<socketID;

    //TODO

    if(m_userSocketsHash.contains(socketID)) {

        UserInfo *info = m_userSocketsHash.value(socketID);
        if(!info) {
            return;
        }

        QString peerAddress = info->getLastLoginExternalHostAddress();
        quint16 peerPort = info->getLastLoginExternalHostPort();
        processUserOnlineStatusChanged(info, IM::ONLINESTATE_OFFLINE, peerAddress, peerPort);

        qCritical() << QString("ERROR! Peer %1:%2 Closed Unexpectedly!").arg(peerAddress).arg(peerPort);

    }


}

void ServerPacketsParser::sendInterestGroupChatMessageToMembers(quint32 interestGroupID, UserInfo *sender, const QString &message, const QString &imageNames)
{

    //TODO
    Q_ASSERT(sender);
    if(!sender) {
        return;
    }

    QList<UserInfo *> onlineMembers = getAllOnlineInterestGroupMembers(interestGroupID);
    onlineMembers.removeAll(sender);
    QString senderID = sender->getUserID();
    foreach (UserInfo *user, onlineMembers) {
        sendInterestGroupChatMessagesToMemberPacket(user->getSocketID(), user->getSessionEncryptionKey(), interestGroupID, senderID, message, imageNames);
    }

}

void ServerPacketsParser::addContactForUser(UserInfo *userInfo, UserInfo *contactInfo, quint32 groupID)
{

    QString userID = userInfo->getUserID();
    QString contactID = contactInfo->getUserID();

    if(addNewContactForUserInDB(userID, contactID, groupID)) {
        sendAddContactResultPacket(userInfo->getSocketID(), contactID,  quint8(IM::ERROR_NoError), "", userInfo->getSessionEncryptionKey());
        userInfo->addNewContact(contactID, groupID);
    } else {
        sendAddContactResultPacket(userInfo->getSocketID(), contactID,  quint8(IM::ERROR_ServerError), "", userInfo->getSessionEncryptionKey());
    }

    quint32 defaultGroupID = contactInfo->defaultFriendContactGroupID();
    contactInfo->addNewContact(userID, defaultGroupID);
    addNewContactForUserInDB(contactID, userID, defaultGroupID);
    if(contactInfo->getOnlineState() == IM::ONLINESTATE_OFFLINE) {
        //TODO:保存请求到数据库
        saveFriendshipApplyRequestToDB(contactID, userID, "", UserInfo::FAR_ACCEPTED, false, true);
        return;
    } else {
        sendAddContactResultPacket(contactInfo->getSocketID(), userID, quint8(IM::ERROR_NoError), "", contactInfo->getSessionEncryptionKey());
    }

}

void ServerPacketsParser::processRgeistrationPacket(const RgeistrationPacket &packet)
{
    RgeistrationPacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case RgeistrationPacket::REGISTRATION_SERVER_INFO: {
        //TODO
        sendRegistrationServerInfoPacket(packet.getSocketID(), quint8(RgeistrationPacket::REG_MODE_USER_CREATE_ALL), "", "");
    }
    break;

    case RgeistrationPacket::REGISTRATION_INFO: {
        IM::ErrorType errorype = IM::ERROR_UnKnownError;
        quint32 sysID = 0;
        QString userID = packet.RgeistrationInfo.userID;
        QString password = packet.RgeistrationInfo.password;
        registerNewUser(userID, password, &errorype, &sysID);
        sendClientRegistrationResultPacket(packet.getSocketID(), sysID, userID, password, quint8(errorype));
    }
    break;

    case RgeistrationPacket::REGISTRATION_RESULT: {
    }
    break;

    default:
        break;
    }



}

void ServerPacketsParser::processUpdatePasswordPacket(const UpdatePasswordPacket &packet)
{
    UpdatePasswordPacket::PacketInfoType infoType = packet.InfoType;

    //TODO
    switch (infoType) {
    case UpdatePasswordPacket::INFO_TYPE_INIT_REQUEST: {
        quint8 authMode = UsersManager::passwordUpdateAuthMode();
        QByteArray captchaImage = QByteArray();
        QString userID = packet.AuthInfo.userID;
        QString securityQuestion = "";
        QString email = "";
        quint32 cellphoneNO = 0;
        QString url = "";


        UpdatePasswordPacket::AuthMode mode = UpdatePasswordPacket::AuthMode(authMode);
        switch (mode) {
        case UpdatePasswordPacket::AUTH_OLD_PWD_ONLY:
            break;
        case UpdatePasswordPacket::AUTH_SECURITY_QUESTION: {

        }
        break;
        case UpdatePasswordPacket::AUTH_EMAIL: {

        }
        break;
        case UpdatePasswordPacket::AUTH_SMS: {

        }
        break;
        case UpdatePasswordPacket::AUTH_HTTP: {

        }
        break;
        default:
            break;
        }

        sendUpdatePasswordAuthInfoPacket(packet.getSocketID(), userID, authMode, captchaImage, securityQuestion, email, cellphoneNO, url);
    }
    break;
    case UpdatePasswordPacket::INFO_TYPE_CAPTCHA_REQUEST: {

    }
    break;
    case UpdatePasswordPacket::INFO_TYPE_CAPTCHA_IMAGE: {
        //TODO
    }
    break;

    case UpdatePasswordPacket::INFO_TYPE_AUTH_INFO_FROM_SERVER: {
    }
    break;

    case UpdatePasswordPacket::INFO_TYPE_AUTH_INFO_FROM_CLIENT: {
        UpdatePasswordPacket::AuthMode mode = UpdatePasswordPacket::AuthMode(packet.AuthInfo.authMode);
        switch (mode) {
        case UpdatePasswordPacket::AUTH_OLD_PWD_ONLY: {
        }
        break;
        case UpdatePasswordPacket::AUTH_SECURITY_QUESTION: {
        }
        break;
        case UpdatePasswordPacket::AUTH_EMAIL: {
        }
        break;
        case UpdatePasswordPacket::AUTH_SMS: {
        }
        break;
        case UpdatePasswordPacket::AUTH_HTTP: {

        }
        break;
        default:
            break;
        }
    }
    break;

    case UpdatePasswordPacket::INFO_TYPE_AUTH_RESULT: {
    }
    break;

    case UpdatePasswordPacket::INFO_TYPE_UPDATE_RESULT: {
    }
    break;


    default:
        break;
    }


}

void ServerPacketsParser::processLoginPacket(const LoginPacket &packet)
{
    int socketID = packet.getSocketID();
    QString userID = packet.getPeerID();
    QHostAddress peerAddress = packet.getPeerHostAddress();
    quint16 peerPort = packet.getPeerHostPort();

    LoginPacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case LoginPacket::INFO_TYPE_LOGIN_SERVER_INFO: {
        IM::ErrorType errorType = IM::ERROR_UnKnownError;
        if(canUserLogin(userID, packet.LoginServerInfo.version, &errorType)) {
            QHostAddress loginServerAddress = QHostAddress::Null;
            quint16 loginServerPort = 0;
            getUserLoginServer(userID, &loginServerAddress, &loginServerPort);
            sendClientCanLoginPacket(socketID, userID, loginServerAddress, loginServerPort);
        } else {
            sendClientLoginFailedPacket(socketID, userID, quint8(errorType));
        }
    }
    break;
    case LoginPacket::INFO_TYPE_AUTH_INFO_FROM_CLIENT: {

        QByteArray encryptedPassword = packet.AuthInfo.password;
        quint8 onlineStateCode = packet.AuthInfo.stateAfterLoggedin;
        QString deviceInfo = packet.AuthInfo.deviceInfo;

        IM::ErrorType errorType = IM::ERROR_UnKnownError;
        UserInfo *userInfo = logUserIn(userID, encryptedPassword, IM::OnlineState(onlineStateCode), &errorType);
        if(userInfo) {

            userInfo->setSocketID(socketID);

            QByteArray sessionEncryptionKey = userInfo->getSessionEncryptionKey();
            sendClientLoginSucceededPacket(socketID, userID, userInfo->encryptedPassword(), sessionEncryptionKey,
                                           userInfo->getPersonalSummaryInfoVersion(),
                                           userInfo->getPersonalDetailInfoVersion(),
                                           userInfo->getPersonalContactGroupsVersion(),
                                           userInfo->getInterestGroupInfoVersion(),
                                           userInfo->getPersonalMessageInfoVersion()
                                          );



            //Send all contacts version info to user
            QString contactInfoVersionListString = "";
            getUserAllContactsInfoVersionFromDatabase(userInfo, &contactInfoVersionListString);

            //Send all contact groups info to user
            sendPersonalContactGroupsInfoPacket(socketID, userInfo->getContactGroupsInfoString(), userInfo->getPersonalContactGroupsVersion(), contactInfoVersionListString, userInfo->getSessionEncryptionKey());


            //Send last login info
            sendClientLastLoginInfoPacket(socketID, sessionEncryptionKey,
                                          userInfo->getLastLoginExternalHostAddress(),
                                          userInfo->getLastLoginTime().toTime_t(),
                                          userInfo->getLastLogoutTime().toTime_t(),
                                          userInfo->getLastLoginDeviceInfo());

            //Send all online contacts list to user
            sendContactsOnlineInfo(socketID, userInfo);

            //Save login or logout info, send info to contacts
            processUserOnlineStatusChanged(userInfo, onlineStateCode, peerAddress.toString(), peerPort, deviceInfo);


            QStringList messagesCachedOnServer = cachedChatMessagesForIMUser(userInfo);
            if(!messagesCachedOnServer.isEmpty()) {
                sendCachedChatMessagesPacket(socketID, messagesCachedOnServer, sessionEncryptionKey);
            }

            QList<QStringList> sentApplicationList, receivedApplicationList;
            getFriendshipApplyRequest(userID, &sentApplicationList, &receivedApplicationList);
            if(!sentApplicationList.isEmpty()) {
                foreach(QStringList infoList, sentApplicationList) {
                    QString receiverID = infoList.at(0);
                    UserInfo *receiver = getUserInfo(receiverID);
                    if(!receiver) {
                        continue;
                    }
                    UserInfo::FriendshipApplyResult faResult = UserInfo::FriendshipApplyResult(infoList.at(1).toUInt());
                    QString message = infoList.at(2);
                    if(faResult == UserInfo::FAR_ACCEPTED ) {
                        sendAddContactResultPacket(socketID, receiverID, IM::ERROR_NoError, message,  sessionEncryptionKey);
                    } else {
                        sendAddContactResultPacket(socketID, receiverID, IM::ERROR_RequestDenied, message, sessionEncryptionKey);
                    }
                }
            }

            if(!receivedApplicationList.isEmpty()) {
                foreach(QStringList infoList, receivedApplicationList) {
                    QString senderID = infoList.at(0);
                    UserInfo *sender = getUserInfo(senderID);
                    if(!sender) {
                        continue;
                    }
                    UserInfo::FriendshipApplyResult faResult = UserInfo::FriendshipApplyResult(infoList.at(1).toUInt());
                    QString message = infoList.at(2);
                    if(faResult == UserInfo::FAR_UNKNOWN ) {
                        sendAddContactRequestFromUserPacket(socketID, senderID, sender->getNickName(), sender->getFace(), message, sessionEncryptionKey);

                    } else if(faResult == UserInfo::FAR_ACCEPTED ) {
                        sendAddContactResultPacket(socketID, senderID, IM::ERROR_NoError, message, sessionEncryptionKey);
                    }/*else{
                    sendAddContactResultPacket(senderID, sender->getNickName(), sender->getFace(), IM::ERROR_RequestDenied, message, sessionEncryptionKey, clientAddress.toString(), clientPort );
                }*/

                }
            }

            QStringList interestgroupChatMessagesCachedOnServer = getCachedInterestGroupChatMessagesForUserFromDB(userInfo);
            if(!interestgroupChatMessagesCachedOnServer.isEmpty()) {
                sendCachedInterestGroupChatMessagesPacket(socketID, interestgroupChatMessagesCachedOnServer, sessionEncryptionKey);
            }

            m_userSocketsHash.insert(socketID, userInfo);

            qWarning() << QString("User %1 logged in!").arg(userID);
        } else {
            sendClientLoginFailedPacket(socketID, userID, quint8(errorType));
            qWarning() << QString("User %1 Login Failed!").arg(userID);

        }
    }
    break;

    case LoginPacket::INFO_TYPE_LOGIN_RESULT: {
        //out << AuthResultInfo.authResult << AuthResultInfo.sessionEncryptionKey;
    }
    break;


    default:
        break;
    }



}

void ServerPacketsParser::processContactGroupsInfoPacket(const ContactGroupsInfoPacket &packet)
{
    UserInfo *userInfo = getUserInfo(packet.getPeerID());
    if(!userInfo) {
        return;
    }

    int socketID = packet.getSocketID();
    QByteArray sessionEncryptionKey = userInfo->getSessionEncryptionKey();

    ContactGroupsInfoPacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case ContactGroupsInfoPacket::PIT_GROUPS_LIST: {
        //Send all contacts version info to user
        QString contactInfoVersionListString = "";
        getUserAllContactsInfoVersionFromDatabase(userInfo, &contactInfoVersionListString);

        //Send all contact groups info to user
        sendPersonalContactGroupsInfoPacket(socketID, userInfo->getContactGroupsInfoString(), userInfo->getPersonalContactGroupsVersion(), contactInfoVersionListString, sessionEncryptionKey);
    }
    break;
    case ContactGroupsInfoPacket::PIT_GROUP_CHANGE_PARENT: {
        //out << GroupParentInfo.groupID << GroupParentInfo.parentID;
    }
    break;
    case ContactGroupsInfoPacket::PIT_GROUP_CREATION: {
        quint32 groupID = 0;
        QString groupName = packet.GroupCreationInfo.name;
        IM::ErrorType errorCode = createOrDeleteContactGroupInDB(userInfo, &groupID, groupName, true, packet.GroupCreationInfo.parentID);
        sendCreateContactGroupResultPacket(socketID, groupID, groupName, quint8(errorCode), sessionEncryptionKey);
    }
    break;
    case ContactGroupsInfoPacket::PIT_GROUP_DELETION: {
        quint32 groupID = packet.GroupDeletionInfo.groupID;
        IM::ErrorType errorCode = createOrDeleteContactGroupInDB(userInfo, &groupID, "", false);
        sendDeleteContactGroupResultPacket(socketID, groupID, quint8(errorCode), sessionEncryptionKey);
    }
    break;
    case ContactGroupsInfoPacket::PIT_GROUP_RENAMING: {
        quint32 groupID = 0;
        QString newGroupName = "";
        updateContactGroupNameInDB(userInfo, groupID, newGroupName);
    }
    break;



    default:
        break;
    }

}


void ServerPacketsParser::processInterestGroupsInfoPacket(const InterestGroupsInfoPacket &packet)
{
    QString userID = packet.getPeerID();
    UserInfo *userInfo = getUserInfo(userID);
    if(!userInfo) {
        return;
    }

    int socketID = packet.getSocketID();


    InterestGroupsInfoPacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case InterestGroupsInfoPacket::PIT_GROUPS_LIST: {
        sendUserInterestGroupsListPacket(packet.getSocketID(), userInfo);
    }
    break;
    case InterestGroupsInfoPacket::PIT_GROUP_INFO: {
        sendUserInterestGroupInfoPacket(socketID, userInfo, packet.GroupID);
    }
    break;
    case InterestGroupsInfoPacket::PIT_GROUP_UPDATE_ANNOUNCEMENT: {
        //TODO
        //out << GroupAnnouncementInfo.content << GroupAnnouncementInfo.admin;
    }
    break;
    case InterestGroupsInfoPacket::PIT_GROUP_CREATION: {
        QString groupName = packet.GroupCreationInfo.name;
        quint32 groupID = createNewInterestGroup(userInfo, groupName, packet.GroupCreationInfo.type);
        sendCreateInterestGroupResultPacket(socketID, userInfo, groupID, groupName);
    }
    break;
    case InterestGroupsInfoPacket::PIT_GROUP_DELETION: {
        quint32 groupID = packet.GroupID;

        //TODO
        bool ok = disbandInterestGroup(userInfo, groupID);
        sendDisbandInterestGroupResultPacket(socketID, userInfo, groupID, ok);
        if(!ok) {
            return;
        }

        InterestGroup *group = getInterestGroup(groupID);
        QStringList members = group->members();
        members.removeAll(userID);
        foreach (QString memberID, members) {
            UserInfo *member = getOnlineUserInfo(memberID);
            if(!member) {
                continue;
            }
            member->joinOrLeaveInterestGroup(groupID, false);
            sendDisbandInterestGroupResultPacket(member->getSocketID(), member, groupID, ok);
        }

    }
    break;
    case InterestGroupsInfoPacket::PIT_GROUP_MEMBER_APPLICATION: {
        quint32 groupID = packet.GroupID;
        QString verificationMessage = packet.MemberApplicationInfo.message;
        InterestGroup *group = getInterestGroup(groupID);
        if(!group) {
            return;
        }

        if(userInfo->isMemberOfInterestGroup(groupID)) {
            qWarning() << QString("ERROR! Failed to join group! '%1' is already a member of group '%2' !").arg(userID).arg(groupID);
            return;
        }

        if(group->getPrivacy() == InterestGroupBase::Allow_Anyone_To_Join) {
            bool ok = memberJoinOrQuitInterestGroup(userInfo, group, true);
            if(!ok) {
                return;
            }
            //TODO:
            QStringList members = group->members();
            foreach (QString memberID, members) {
                UserInfo *member = getOnlineUserInfo(memberID);
                if(!member) {
                    continue;
                }
                sendUserJoinInterestGroupResultToUserPacket(member->getSocketID(), groupID, memberID, true, "", "", member->getSessionEncryptionKey());
            }

        } else if(group->getPrivacy() == InterestGroupBase::Request_Verfication_To_Join) {
            QStringList admins = group->getAdmins(true);
            bool requestSent = false;
            foreach (QString adminID, admins) {
                UserInfo *adminInfo = getOnlineUserInfo(userID);
                if(!adminInfo) {
                    continue;
                }
                sendUserJoinInterestGroupRequestToAdminPacket(adminInfo->getSocketID(), groupID, userInfo->getUserID(), verificationMessage, adminInfo->getSessionEncryptionKey());
                requestSent = true;
            }
            if(!requestSent) {
                saveOrDeleteMembershipApplication(userID, groupID, verificationMessage, true);
            }


        } else {
            //TODO

        }

    }
    break;

    case InterestGroupsInfoPacket::PIT_GROUP_MEMBER_DELETION: {
        quint32 groupID = packet.GroupID;
        QString targeID = packet.MemberDeletionInfo.userID;
        QString admin = packet.MemberDeletionInfo.admin;



        //if(userID  == targeID){
        //Quit Group
        if(!userInfo->isMemberOfInterestGroup(groupID)) {
            qWarning() << QString("ERROR! Failed to quit group! '%1' is not a member of group '%2' !").arg(userID).arg(groupID);
            return;
            //TODO:Send Error Message
        }

        InterestGroup *group = getInterestGroup(groupID);
        if(!group) {
            return;
        }
        if((userID != targeID) && (!group->isAdmin(userID))) {
            return;
        }

        bool ok = memberJoinOrQuitInterestGroup(userInfo, group, false);
        if(!ok) {
            return;
        }
        //TODO:
        QStringList members = group->members();
        foreach (QString memberID, members) {
            UserInfo *member = getOnlineUserInfo(memberID);
            if(!member) {
                continue;
            }
            sendUserQuitInterestGroupResultToUserPacket(member->getSocketID(), groupID, targeID, admin, member->getSessionEncryptionKey());
        }
        //sendUserQuitInterestGroupResultToUserPacket(userInfo->getSocketID(), groupID, targeID, false, userInfo->getSessionEncryptionKey());
        //}
        //out << MemberDeletionInfo.userID << MemberDeletionInfo.blockForever << MemberDeletionInfo.admin;
    }
    break;

    case InterestGroupsInfoPacket::PIT_GROUP_MEMBER_PROMOTION: {
        //TODO
        //out << MemberPromotionInfo.userID << MemberPromotionInfo.promoted;
    }
    break;



    default:
        break;
    }

}

void ServerPacketsParser::processContactInfoPacket(const ContactInfoPacket &packet)
{
    QString userID = packet.getPeerID();
    UserInfo *userInfo = getUserInfo(userID);
    if(!userInfo) {
        return;
    }

    UserInfo *contactInfo = 0;
    QString contactID = packet.ContactID;
    if(userID == contactID) {
        contactInfo = userInfo;
    } else {
        contactInfo = getUserInfo(contactID);
        if(!contactInfo) {
            return;
        }
    }

    ContactInfoPacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case ContactInfoPacket::PIT_CONTACT_INFO: {
        quint8 requestSummaryInfo = packet.info.isSummaryInfo;
        QString infoString = contactInfo->getPersonalInfoString(requestSummaryInfo);

        sendUserInfoPacket(packet.getSocketID(), contactID, requestSummaryInfo, infoString, userInfo->getSessionEncryptionKey());
    }
    break;
    case ContactInfoPacket::PIT_GROUP_CHANGE: {
        if(userID == contactID) {
            return;
        }
        if(!userInfo->hasFriendContact(contactID)) {
            return;
        }

        quint32 oldGroupID = packet.ContactChangeGroup.oldGroupID;
        quint32 newGroupID = packet.ContactChangeGroup.newGroupID;

        bool ok = moveContactForUserInDB(userInfo, contactID, newGroupID);
        if(ok) {
            userInfo->moveContactToAnotherGroup(contactID, oldGroupID, newGroupID);
        }

    }
    break;

    case ContactInfoPacket::PIT_FRIENDING_REQUEST: {

        QString verificationMessage = packet.ContactFriendingRequest.message;
        quint32 groupID = packet.ContactFriendingRequest.groupID;
        if(userID == contactID) {
            return;
        }

        if(userInfo->hasFriendContact(contactID)) {
            //sendAddContactResultPacket(contactID, IM::ERROR_RequestDenied, "Contact already exists!", userInfo->getSessionEncryptionKey(), clientAddress, clientPort);
            //TODO:Send Error Message
            qWarning() << QString("'%1' already has contact '%2' !").arg(userID).arg(contactID);
            return;
        }

        if(!userInfo->hasContactGroup(groupID)) {
            groupID = ContactGroupBase::Group_Friends_ID;
        }

        if(contactInfo->getFriendshipApply() == UserInfo::FA_AUTO_ACCEPT) {
            addContactForUser(userInfo, contactInfo, groupID);

        } else {
            if(contactInfo->getOnlineState() == IM::ONLINESTATE_OFFLINE) {
                //TODO:保存请求到数据库
                saveFriendshipApplyRequestToDB(userID, contactID, verificationMessage);
            } else {
                sendAddContactRequestFromUserPacket(contactInfo->getSocketID(), userID, userInfo->getNickName(), userInfo->getFace(), verificationMessage, contactInfo->getSessionEncryptionKey());
            }
        }

    }
    break;

    case ContactInfoPacket::PIT_FRIENDING_RESULT: {
        QString contactID = packet.ContactID;
        QString extraMessage = packet.ContactFriendingResult.message;
        quint32 groupID = ContactGroupBase::Group_Friends_ID;

        UserInfo *contactInfo = getUserInfo(contactID);
        if(!contactInfo) {
            return;
        }


        //TODO:
        if(packet.ContactFriendingResult.errorCode == quint8(IM::ERROR_NoError)) {
            addContactForUser(userInfo, contactInfo, groupID);

//            contactInfo->addNewContact(userID, UserInfo::defaultFriendContactGroupID());

//            if(contactInfo->getOnlineState() == IM::ONLINESTATE_OFFLINE){
//                //TODO:保存请求到数据库
//                saveFriendshipApplyRequestToDB(contactID, userID, extraMessage, UserInfo::FAR_ACCEPTED, false, true);
//                return;
//            }else{
//                sendAddContactResultPacket(socketID, userID, userInfo->getNickName(), userInfo->getFace(), IM::ERROR_NoError, "", contactInfo->getSessionEncryptionKey(), contactInfo->getLastLoginExternalHostAddress(), contactInfo->getLastLoginExternalHostPort());
//            }

        } else {
            if(!extraMessage.trimmed().isEmpty()) {
                if(contactInfo->isOnLine()) {
                    sendAddContactResultPacket(contactInfo->getSocketID(), userID, quint8(IM::ERROR_RequestDenied), extraMessage, contactInfo->getSessionEncryptionKey());
                } else {
                    //TODO:保存请求到数据库
                    saveFriendshipApplyRequestToDB(contactID, userID, extraMessage, UserInfo::FAR_DENIED, false, true);
                    return;
                }
            }

        }
        //TODO:从数据库删除请求
        deleteFriendshipApplyRequest(contactID, userID);

        //in >> ContactFriendingResult.message >> ContactFriendingResult.approved >> ContactFriendingResult.blocked;
    }
    break;

    case ContactInfoPacket::PIT_CONTACT_DELETION: {
        quint8 deleteMeFromOpposition = packet.ContactDeletionInfo.deleteMeFromOpposition;
        quint8 addToBlacklist = packet.ContactDeletionInfo.blockForever;

        //TODO:数据库
        //deleteFriendshipApplyRequest(userID, contactID);
        deleteContactForUserFromDB(userID, contactID, deleteMeFromOpposition, addToBlacklist);

        userInfo->deleteContact(contactID);
        //TODO
        sendDeleteContactResultPacket(packet.getSocketID(), contactID, addToBlacklist, quint8(IM::ERROR_NoError), userInfo->getSessionEncryptionKey());

        if(addToBlacklist) {
            userInfo->addOrDeleteBlacklistedContact(contactID, true);
            userInfo->clearUpdatedProperties();
        }

        if(deleteMeFromOpposition) {
            //TODO:
            UserInfo *contactInfo = getOnlineUserInfo(contactID);
            if(!contactInfo) {
                contactInfo = getOfflineUserInfo(contactID);;
            }
            if(!contactInfo) {
                return;
            }
            contactInfo->deleteContact(userID);
            if(contactInfo->isOnLine()) {
                sendDeleteContactResultPacket(contactInfo->getSocketID(), userID, 0, quint8(IM::ERROR_NoError), contactInfo->getSessionEncryptionKey());
            }

            //deleteFriendshipApplyRequest(contactID, userID);
        }

    }
    break;

    case ContactInfoPacket::PIT_CONTACT_REMARK: {
        //TODO
        //in >> ContactRemarkInfo.newRemarkName;
    }
    break;


    default:
        break;
    }

}

void ServerPacketsParser::processSearchInfoPacket(const SearchInfoPacket &packet)
{
    QString userID = packet.getPeerID();
    UserInfo *userInfo = getUserInfo(userID);
    if(!userInfo) {
        return;
    }

    int socketID = packet.getSocketID();

    SearchInfoPacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case SearchInfoPacket::PIT_SEARCH_CONTACT_CONDITIONS: {

        QString usersListString = searchContact(packet.SearchContactConditions.keyword,
                                                packet.SearchContactConditions.isOnline,
                                                packet.SearchContactConditions.hasWebcam,
                                                packet.SearchContactConditions.location,
                                                packet.SearchContactConditions.hometown,
                                                packet.SearchContactConditions.gender,
                                                packet.SearchContactConditions.age,
                                                packet.SearchContactConditions.matchExactly,
                                                packet.SearchContactConditions.startIndex
                                               );


        //QString usersListString = searchContact(propertiesString, matchExactly, searchOnlineUsersOnly, searchWebcamUsersOnly, startIndex);
        if(!usersListString.isEmpty()) {
            sendSearchContactResultPacket(socketID, usersListString, userInfo->getSessionEncryptionKey());
        }

    }
    break;
    case SearchInfoPacket::PIT_SEARCH_CONTACT_RESULT: {
    }
    break;
    case SearchInfoPacket::PIT_SEARCH_INTEREST_GROUP_CONDITIONS: {
        QString keyword = packet.SearchInterestGroupConditions.keyword;
        int startIndex = packet.SearchInterestGroupConditions.startIndex;

        QString groupsListString = searchInterestGroup(keyword, startIndex);
        if(!groupsListString.isEmpty()) {
            sendSearchInterestGroupResultPacket(socketID, groupsListString, userInfo->getSessionEncryptionKey());
        }

    }
    break;
    case SearchInfoPacket::PIT_SEARCH_INTEREST_GROUP_RESULT: {
    }
    break;



    default:
        break;
    }

}

void ServerPacketsParser::processChatMessagePacket(const ChatMessagePacket &packet)
{
    QString userID = packet.getPeerID();
    UserInfo *userInfo = getUserInfo(userID);
    if(!userInfo) {
        return;
    }

    int socketID = packet.getSocketID();

    ChatMessagePacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case ChatMessagePacket::PIT_CONTACT_CHAT_MESSAGE: {

        QString contactID = packet.ContactChatMessage.contactID;
        UserInfo *contactInfo = getUserInfo(contactID);
        if(!contactInfo) {
            return;
        }

        QString message = packet.ContactChatMessage.message;
        QString imageNames = packet.ContactChatMessage.imageNames;

        saveCachedChatMessageFromIMUser(userID, contactID, message);

        QStringList imageNameList = imageNames.split(",");
        if(imageNameList.isEmpty()) {
            return;
        }
        foreach (QString imageName, imageNameList) {
            if(!QFile::exists(m_chatImageCacheDir + "/" + imageName)) {
                qDebug() << QString("Image '%1' exists on server!").arg(imageName);
                sendRequestImagePacket(socketID, userID, imageName, userInfo->getSessionEncryptionKey());
            }
        }


        //Send To Contact
        if(contactInfo->getOnlineState() != IM::ONLINESTATE_OFFLINE) {
            sendContactChatMessagePacket(contactInfo->getSocketID(), userID, message, imageNames, contactInfo->getSessionEncryptionKey());
        }


    }
    break;
    case ChatMessagePacket::PIT_CONTACT_CHAT_MESSAGES_CACHED_ON_SERVER: {
    }
    break;
    case ChatMessagePacket::PIT_CONTACT_CHAT_HISTORY_MESSAGES: {
    }
    break;
    case ChatMessagePacket::PIT_GROUP_CHAT_MESSAGE: {
        quint32 interestGroupID = packet.GroupChatMessage.groupID;
        QString memberID = packet.GroupChatMessage.memberID;
        uint time = packet.GroupChatMessage.time;
        QString  message = packet.GroupChatMessage.message;
        QString imageNames = packet.GroupChatMessage.imageNames;

        if(userInfo->isMemberOfInterestGroup(interestGroupID)) {
            saveCachedInterestGroupChatMessageFromIMUser(userID, interestGroupID, message);
        } else {
            qCritical() << QString("User '%1' is not a member of group '%2'!").arg(userID).arg(interestGroupID);
            return;
        }

        QStringList imageNameList = imageNames.split(",");
        if(imageNameList.isEmpty()) {
            return;
        }
        foreach (QString imageName, imageNameList) {
            if(!QFile::exists(m_chatImageCacheDir + "/" + imageName)) {
                qDebug() << QString("Image '%1' exists on server!").arg(imageName);
                sendRequestImagePacket(socketID, userID, imageName, userInfo->getSessionEncryptionKey());
            }
        }

        //TODO:Send message to all contacts
        sendInterestGroupChatMessageToMembers(interestGroupID, userInfo, message, imageNames);

    }
    break;
    case ChatMessagePacket::PIT_GROUP_CHAT_MESSAGES_CACHED_ON_SERVER: {
        //out << GroupChatMessagesCachedOnServer.messages;
    }
    break;
    case ChatMessagePacket::PIT_GROUP_CHAT_HISTORY_MESSAGES: {
        //out << GroupChatHistoryMessages.groupID << GroupChatHistoryMessages.messages << GroupChatHistoryMessages.startime;
    }
    break;
    case ChatMessagePacket::PIT_CHAT_IMAGE: {

        bool isRequest = packet.ChatImage.isRequest;
        QString contactID = packet.ChatImage.contactID;
        QString imageName = packet.ChatImage.name;
        QByteArray image = packet.ChatImage.image;
        if(isRequest) {
            //TODO:Check cached file
            QFile file(m_chatImageCacheDir + "/" + imageName);
            if(file.exists()) {
                qDebug() << QString("Image '%1' exists on server!").arg(imageName);
                QByteArray ba;
                if (file.open(QFile::ReadOnly)) {
                    ba = file.readAll();
                    file.close();

                    sendImagePacket(socketID, contactID, imageName, ba, userInfo->getSessionEncryptionKey());
                    return ;
                } else {
                    qCritical() << QString("ERROR! Failed to open image '%1'! %2").arg(imageName).arg(file.errorString());
                }
            }


            UserInfo *contactInfo = getOnlineUserInfo(contactID);
            if(!contactInfo) {
                sendImagePacket(socketID, contactID, imageName, QByteArray(), userInfo->getSessionEncryptionKey());
                return;
            }

            sendRequestImagePacket(contactInfo->getSocketID(), userID, imageName, contactInfo->getSessionEncryptionKey());

        } else {

            if(image.isNull()) {
                qCritical() << "ERROR! Invalid image!";
                return;
            }

            //TODO:Save image
            Settings settings;
            QString cacheDir = settings.getChatImageCacheDir();
            QString fileName = cacheDir + "/" + imageName;

            QFile file(fileName);
            if(!file.exists()) {
                if(!file.open(QFile::WriteOnly)) {
                    qCritical() << QString("ERROR! Failed to write image '%1'! %2").arg(imageName).arg(file.errorString());
                    return;
                }
                file.write(image);
                file.flush();
                file.close();
            }
            //QString md5String = QCryptographicHash::hash(image, QCryptographicHash::Md5).toHex();
            QList<UserInfo *> users = imageDownloadingRequestHash.values(imageName);
            foreach (UserInfo *user, users) {
                sendImagePacket(user->getSocketID(), userID, imageName, image, user->getSessionEncryptionKey());
            }
        }


    }
    break;
    case ChatMessagePacket::PIT_SESSION_ENCRYPTION_KEY_WITH_CONTACT: {
        QString contactID = packet.SessionEncryptionKeyWithContact.contactID;
        UserInfo *contactInfo = getUserInfo(contactID);
        if(!contactInfo) {
            return;
        }
        if(contactInfo->isOnLine()) {
            QByteArray key = ServerUtilities::generateSessionEncryptionKey();
            sendSessionEncryptionKeyWithContact(socketID, userID, key, contactInfo->getSessionEncryptionKey());
            sendSessionEncryptionKeyWithContact(socketID, contactID, key, userInfo->getSessionEncryptionKey());
        } else {
            sendContactOnlineStatusChangedPacket(socketID, userInfo->getSessionEncryptionKey(), quint8(IM::ONLINESTATE_OFFLINE), contactID,  contactInfo->getLastLoginExternalHostAddress(), contactInfo->getLastLoginExternalHostPort());
        }
    }
    break;

    default:
        break;
    }


}

void ServerPacketsParser::processCaptchaInfoPacket(const CaptchaInfoPacket &packet)
{
    QString userID = packet.getPeerID();
    UserInfo *userInfo = getUserInfo(userID);
    if(!userInfo) {
        return;
    }

    CaptchaInfoPacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case CaptchaInfoPacket::CAPTCHA_REQUEST: {
    }
    break;
    case CaptchaInfoPacket::CAPTCHA_IMAGE: {
        //out << captchaImage;
    }
    break;
    case CaptchaInfoPacket::CAPTCHA_CODE: {
        //out << captchaCode;
    }
    break;
    case CaptchaInfoPacket::CAPTCHA_AUTH_RESULT: {
        //out << approved;
    }
    break;



    default:
        break;
    }


}

void ServerPacketsParser::processFileTransferPacket(const FileTransferPacket &packet)
{
    QString userID = packet.getPeerID();
    UserInfo *userInfo = getUserInfo(userID);
    if(!userInfo) {
        return;
    }

    FileTransferPacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case FileTransferPacket::FT_FILE_SERVER_INFO: {
        //TODO
        QString address = "";
        quint16 port = 0;
        responseFileServerInfo(packet.getSocketID(), address, port, userInfo->getSessionEncryptionKey());
    }
    break;

    case FileTransferPacket::FT_FileSystemInfoRequest: {
        //in >> FileSystemInfoRequest.parentDirPath;
    }
    break;

    case FileTransferPacket::FT_FileSystemInfoResponse: {
        //in >> FileSystemInfoResponse.baseDirPath >> FileSystemInfoResponse.fileSystemInfoData;
    }
    break;

    case FileTransferPacket::FT_FileDeletingRequest: {
        //in >> FileDeletingRequest.baseDirPath >> FileDeletingRequest.files;
    }
    break;

    case FileTransferPacket::FT_FileDeletingResponse: {
        //in >> FileDeletingResponse.baseDirPath >> FileDeletingResponse.failedFiles;
    }
    break;

    case FileTransferPacket::FT_FileRenamingRequest: {
        //in >> FileRenamingRequest.baseDirPath >> FileRenamingRequest.oldFileName >> FileRenamingRequest.newFileName;
    }
    break;

    case FileTransferPacket::FT_FileRenamingResponse: {
        //in >> FileRenamingResponse.baseDirPath >> FileRenamingResponse.oldFileName >> FileRenamingResponse.renamed >> FileRenamingResponse.message;
    }
    break;

    case FileTransferPacket::FileTransferPacket::FT_FileDownloadingRequest: {
        //in >> FileDownloadingRequest.baseDir >> FileDownloadingRequest.fileName >> FileDownloadingRequest.dirToSaveFile;
    }
    break;

    case FileTransferPacket::FT_FileDownloadingResponse: {
        //in >> FileDownloadingResponse.accepted >> FileDownloadingResponse.baseDir >> FileDownloadingResponse.fileName >> FileDownloadingResponse.fileMD5Sum >> FileDownloadingResponse.size >> FileDownloadingResponse.errorCode;
    }
    break;

    case FileTransferPacket::FT_FileUploadingRequest: {
        //in >> FileUploadingRequest.fileName >> FileUploadingRequest.fileMD5Sum >> FileUploadingRequest.size >> FileUploadingRequest.fileSaveDir;
    }
    break;

    case FileTransferPacket::FT_FileUploadingResponse: {
        //in >> FileUploadingResponse.accepted >> FileUploadingResponse.fileMD5Sum >> FileUploadingResponse.message;
    }
    break;

    case FileTransferPacket::FT_FileDataRequest: {
        //in >> FileDataRequest.fileMD5 >> FileDataRequest.startPieceIndex >> FileDataRequest.endPieceIndex;
    }
    break;

    case FileTransferPacket::FT_FileData: {
        //in >> FileDataResponse.fileMD5 >> FileDataResponse.pieceIndex >> FileDataResponse.data >> FileDataResponse.pieceMD5;
    }
    break;

    case FileTransferPacket::FT_FileTXStatus: {
        //in >> FileTXStatus.fileMD5 >> FileTXStatus.status;
    }
    break;

    case FileTransferPacket::FT_FileTXError: {
        //in >> FileTXError.fileName >> FileTXError.fileMD5 >> FileTXError.errorCode >> FileTXError.message;
    }
    break;

    default:
        break;
    }


}







UserInfo *ServerPacketsParser::logUserIn(const QString &userID, const QByteArray &encryptedPassword, IM::OnlineState loginState, IM::ErrorType *errorType)
{
    //qWarning()<<"logUserIn(...)";

    UserInfo *userInfo = getUserInfo(userID);
    if(!userInfo) {
        *errorType = IM::ERROR_IDNotExist;
        return 0;
    }

    //TODO:密码保存方式
    QByteArray decryptedPassword;
    Cryptography cryptography;
    cryptography.teaCrypto(&decryptedPassword, encryptedPassword, userInfo->encryptedPassword(), false);

    if(decryptedPassword != QByteArray::fromBase64(userInfo->getPassword().toUtf8())) {
        *errorType = IM::ERROR_PasswordIncorrect;
        userInfo->authenticationFailed();
        return 0;
    } else {

        //Check Duplicate Login
        if(userInfo->getOnlineState() != IM::ONLINESTATE_OFFLINE) {
            //Kick user off
            QString msg = tr("Your account has been logged in from other place!");
            sendClientLoginFailedPacket(userInfo->getSocketID(), userID, quint8(IM::ERROR_KickedOut), msg);
            m_rtp->closeSocket(userInfo->getSocketID());
            processUserOnlineStatusChanged(userInfo, IM::ONLINESTATE_OFFLINE, userInfo->getLastLoginExternalHostAddress(), userInfo->getLastLoginExternalHostPort());
        }


        *errorType = IM::ERROR_NoError;
        userInfo->setOnlineState(loginState);
        userInfo->setSessionEncryptionKey(ServerUtilities::generateSessionEncryptionKey());
        userOnline(userInfo);


        //Get contact groups info
        getUserAllContactGroupsInfoFromDatabase(userInfo);

        //Load interest groups
        getUserInterestGroupsFromDB(userInfo);

        //Load Last login info
        getUserLastLoginInfo(userInfo);

        //qWarning()<<"getSessionEncryptionKey:"<<userInfo->getSessionEncryptionKey().toBase64();
    }


    return userInfo;
}




















} //namespace HEHUI
