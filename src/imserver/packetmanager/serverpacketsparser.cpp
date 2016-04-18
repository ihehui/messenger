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


namespace HEHUI {


ServerPacketsParser::ServerPacketsParser(ResourcesManagerInstance *resourcesManager, QObject *parent)
    :QObject(parent), UsersManager(parent)
{

    Q_ASSERT(resourcesManager);

    m_ipmcServer = resourcesManager->getIPMCServer();
    Q_ASSERT_X(m_ipmcServer, "ClientPacketsParser::ClientPacketsParser(...)", "Invalid IP MC Server!");
    connect(m_ipmcServer, SIGNAL(signalNewUDPPacketReceived(Packet*)), this, SLOT(parseIncomingPacketData(Packet*)), Qt::QueuedConnection);


    m_rtp = resourcesManager->getRTP();
    Q_ASSERT(m_rtp);
    connect(m_rtp, SIGNAL(disconnected(int)), this, SLOT(peerDisconnected(int)));

    m_udtProtocol = m_rtp->getUDTProtocol();
    Q_ASSERT(m_udtProtocol);
    connect(m_udtProtocol, SIGNAL(packetReceived(Packet*)), this, SLOT(parseIncomingPacketData(Packet*)), Qt::QueuedConnection);
    m_udtProtocol->startWaitingForIOInOneThread(1);
    //m_udtProtocol->startWaitingForIOInSeparateThread(100, 1000);

    m_tcpServer = m_rtp->getTCPServer();
    Q_ASSERT(m_tcpServer);
    connect(m_tcpServer, SIGNAL(packetReceived(Packet*)), this, SLOT(parseIncomingPacketData(Packet*)), Qt::QueuedConnection);





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

ServerPacketsParser::~ServerPacketsParser() {
    // TODO Auto-generated destructor stub

    QMutexLocker locker(&mutex);
    
    if(checkIMUsersOnlineStateTimer){
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



void ServerPacketsParser::parseIncomingPacketData(PacketBase *packet){

    //    qDebug()<<"----ServerPacketsParser::parseIncomingPacketData(Packet *packet)";


    QByteArray packetData = packet->getPacketData();
    QDataStream in(&packetData, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    QString peerID = "";
    in >> peerID;

    packet->setPeerID(peerID);

    QHostAddress peerAddress = packet->getPeerHostAddress();
    quint16 peerPort = packet->getPeerHostPort();
    quint8 packetType = packet->getPacketType();
    int socketID = packet->getSocketID();

    QByteArray sessionEncryptionKey = getUserSessionEncryptionKey(peerID);

    qDebug()<<"--ServerPacketsParser::parseIncomingPacketData(...) "<<" socketID:"<<socketID <<" peerID:"<<peerID<<" peerAddress:"<<peerAddress.toString()<<" peerPort:"<<peerPort<<" packetType:"<<packetType;

    PacketHandlerBase::recylePacket(packet);

    switch(packetType){

    case quint8(IM::CMD_ServerDiscovery):
    {
        qDebug()<<"~~ClientLookForServer";
        ServerDiscoveryPacket p(packet, QByteArray());
        sendServerDeclarePacket(peerAddress, peerPort);
        //qDebug()<<"~~ClientLookForServer--"<<" peerAddress:"<<peerAddress.toString()<<"   peerPort:"<<peerPort;
    }
        break;

    case quint8(IM::CMD_DataForward):
    {
        qDebug()<<"~~DataForwardRequestByClient";

        UserInfo *senderInfo = getOnlineUserInfo(senderID);
        if(!senderInfo){
            qCritical()<<"ERROR! Sender Not Found!";
            return;
        }

        DataForwardPacket p(packet, senderInfo->getSessionEncryptionKey());
        if(!p.isValid()){
            qCritical()<<"ERROR! Invalid Packet!";
            return;
        }

        UserInfo *receiverInfo = getUserInfo(p.receiver);
        if(!receiverInfo){
            qCritical()<<"ERROR! Receiver Not Found!";
            return;
        }
        if(!receiverInfo->isOnLine()){
            qCritical()<<"ERROR! Receiver Offline!";
            return;
        }

        forwardData(m_userSocketsHash.key(receiverInfo), p.data, receiverInfo->getSessionEncryptionKey());

    }
        break;


    case quint8(IM::CMD_Announcement):
    {
        AnnouncementPacket p(packet, sessionEncryptionKey);
        qDebug()<<"~~CMD_Announcement";
        //TODO

    }
        break;

    case quint8(IM::CMD_Rgeistration):
    {
        RgeistrationPacket p(packet, sessionEncryptionKey);
        qDebug()<<"~~CMD_Rgeistration";
        //TODO
        processRgeistrationPacket(p);
    }
        break;

    case quint8(IM::CMD_UpdatePassword):
    {
        UpdatePasswordPacket p(packet, sessionEncryptionKey);
        p.AuthInfo.authMode = UsersManager::passwordUpdateAuthMode();

        processUpdatePasswordPacket(p);
        qDebug()<<"~~CMD_UpdatePassword";
    }
        break;

    case quint8(IM::CMD_Login):
    {
        LoginPacket p(packet, sessionEncryptionKey);
        processLoginPacket(p);
        qDebug()<<"~~CMD_Login";
    }
        break;

    case quint8(IM::CMD_OnlineStateChanged):
    {
        OnlineStateChangedPacket p(packet, sessionEncryptionKey);
        quint8 stateCode = p.stateCode;
        processUserOnlineStatusChanged(userID, IM::OnlineState(stateCode), peerAddress.toString(), peerPort);
        qDebug()<<"~~CMD_OnlineStateChanged";
    }
        break;

    case quint8(IM::CMD_ContactGroupsInfo):
    {
        ContactGroupsInfoPacket p(packet, sessionEncryptionKey);
        processContactGroupsInfoPacket(p);
        qDebug()<<"~~CMD_ContactGroupsInfo";
    }
        break;

    case quint8(IM::CMD_InterestGroupsInfo):
    {
        InterestGroupsInfoPacket p(packet, sessionEncryptionKey);
        processInterestGroupsInfoPacket(p);
        qDebug()<<"~~CMD_InterestGroupsInfo";
    }
        break;

    case quint8(IM::CMD_ContactInfo):
    {
        ContactInfoPacket p(packet, sessionEncryptionKey);
        processContactInfoPacket(p);
        qDebug()<<"~~CMD_ContactInfo";
    }
        break;

    case quint8(IM::CMD_SearchInfo):
    {
        SearchInfoPacket p(packet, sessionEncryptionKey);
        processSearchInfoPacket(p);
        qDebug()<<"~~CMD_SearchInfo";
    }
        break;

    case quint8(IM::CMD_Captcha):
    {
        CaptchaInfoPacket p(packet, sessionEncryptionKey);
        processCaptchaInfoPacket(p);
        qDebug()<<"~~CMD_Captcha";
    }
        break;

        //File TX
    case quint8(IM::CMD_FileTransfer):
    {
        FileTransferPacket p(packet, sessionEncryptionKey);
        emit signalFileTransferPacketReceived(p);
        qDebug()<<"~~CMD_FileTransfer";
    }
        break;






    default:
        qWarning()<<"Unknown Packet Type:"<<packetType
                 <<" From:"<<peerAddress.toString()
                <<" Port:"<<peerPort;
        break;

    }



}


void ServerPacketsParser::userExceptionalOffline(const QString &peerAddress, quint16 peerPort){
    qDebug()<<"--userExceptionalOffline(...)"<<" peerAddress:"<<peerAddress<<" peerPort:"<<peerPort;

    QList<UserInfo*>  users = onlineUsers();
    foreach(UserInfo *info, users){
        if(info->getLastLoginExternalHostAddress() == peerAddress && info->getLastLoginExternalHostPort() == peerPort){
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

int ServerPacketsParser::crypto(QByteArray *destination, const QByteArray &source, const QByteArray &key, bool encrypt){
    return cryptography->teaCrypto(destination, source, key, encrypt);
}

bool ServerPacketsParser::encrypeData(const QString &userID, QByteArray *destination, const QByteArray &source){

    UserInfo *info = getUserInfo(userID);
    return encrypeData(info, destination, source);
}

bool ServerPacketsParser::encrypeData(UserInfo *info, QByteArray *destination, const QByteArray &source){

    if(!info){
        return false;
    }
    return cryptography->teaCrypto(destination, source, info->getSessionEncryptionKey(), true);
}

bool ServerPacketsParser::decryptData(const QString &userID, QByteArray *destination, const QByteArray &source){

    UserInfo *info = getUserInfo(userID);

    return decryptData(info, destination, source);

}

bool ServerPacketsParser::decryptData(UserInfo *info, QByteArray *destination, const QByteArray &source){

    if(!info){
        return false;
    }

    //TODO 验证数据是否解密成功  //qCritical()<<"Data Decryption Failed!";
    return cryptography->teaCrypto(destination, source, info->getSessionEncryptionKey(), false);

}

QByteArray ServerPacketsParser::getUserSessionEncryptionKey(const QString &userID){
    UserInfo *info = getUserInfo(userID);
    if(!info){
        return QByteArray();
    }

    return info->getSessionEncryptionKey();
}

void ServerPacketsParser::peerDisconnected(int socketID){
    //qDebug()<<"--ServerPacketsParser::peerDisconnected(...) "<<" socketID:"<<socketID;

    //TODO

    if(m_userSocketsHash.contains(socketID)){

        UserInfo *info = m_userSocketsHash.value(socketID);
        if(!info){return;}

        QString peerAddress = info->getLastLoginExternalHostAddress();
        quint16 peerPort = info->getLastLoginExternalHostPort();
        processUserOnlineStatusChanged(info, IM::ONLINESTATE_OFFLINE, peerAddress, peerPort);

        qCritical()<<QString("ERROR! Peer %1:%2 Closed Unexpectedly!").arg(peerAddress).arg(peerPort);

    }


}

void ServerPacketsParser::sendInterestGroupChatMessageToMembers(quint32 interestGroupID, UserInfo *sender, const QString &message){

    //TODO
    Q_ASSERT(sender);
    if(!sender){return;}

     QList<UserInfo *> onlineMembers = getAllOnlineInterestGroupMembers(interestGroupID);
     onlineMembers.removeAll(sender);
     QString senderID = sender->getUserID();
     foreach (UserInfo *user, onlineMembers) {
         sendInterestGroupChatMessagesToMemberPacket(user->getSocketID(), interestGroupID, senderID, message, user->getSessionEncryptionKey());
     }

}

void ServerPacketsParser::addContactForUser(UserInfo *userInfo, UserInfo *contactInfo, quint32 groupID){

    QString userID = userInfo->getUserID();
    QString contactID = contactInfo->getUserID();

    if(addNewContactForUserInDB(userID, contactID, groupID)){
        sendAddContactResultPacket(userInfo->getSocketID(), contactID, contactInfo->getNickName(), contactInfo->getFace(), groupID, IM::ERROR_NoError, "", contactInfo->getOnlineState(), userInfo->getSessionEncryptionKey(), userInfo->getLastLoginExternalHostAddress(), userInfo->getLastLoginExternalHostPort());
        userInfo->addNewContact(contactID, groupID);
    }else{
        sendAddContactResultPacket(userInfo->getSocketID(), contactID, contactInfo->getNickName(), contactInfo->getFace(), groupID, IM::ERROR_ServerError, "", contactInfo->getOnlineState(), userInfo->getSessionEncryptionKey(), userInfo->getLastLoginExternalHostAddress(), userInfo->getLastLoginExternalHostPort());
    }

    quint32 defaultGroupID = contactInfo->defaultFriendContactGroupID();
    contactInfo->addNewContact(userID, defaultGroupID);
    addNewContactForUserInDB(contactID, userID, defaultGroupID);
    if(contactInfo->getOnlineState() == IM::ONLINESTATE_OFFLINE){
        //TODO:保存请求到数据库
        saveFriendshipApplyRequestToDB(contactID, userID, "", UserInfo::FAR_ACCEPTED, false, true);
        return;
    }else{
        sendAddContactResultPacket(contactInfo->getSocketID(), userID, userInfo->getNickName(), userInfo->getFace(), defaultGroupID, IM::ERROR_NoError, "", userInfo->getOnlineState(), contactInfo->getSessionEncryptionKey(), contactInfo->getLastLoginExternalHostAddress(), contactInfo->getLastLoginExternalHostPort());
    }

}

void ServerPacketsParser::processRgeistrationPacket(const RgeistrationPacket &packet){
    RgeistrationPacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case RgeistrationPacket::REGISTRATION_SERVER_INFO:
    {
        sendRegistrationServerInfoPacket(packet.getSocketID(), packet);
    }
        break;

    case RgeistrationPacket::REGISTRATION_INFO:
    {
        IM::ErrorType errorype = IM::ERROR_UnKnownError;
        quint32 sysID = 0;
        QString userID = packet.RgeistrationInfo.userID;
        QString password = packet.RgeistrationInfo.password;
        registerNewUser(userID, password, &errorype, &sysID);
        sendClientRegistrationResultPacket(packet.getSocketID(), sysID, userID, password, quint8(errorype));
    }
        break;

    case RgeistrationPacket::REGISTRATION_RESULT:
    {
    }
        break;

    default:
        break;
    }



}

void ServerPacketsParser::processUpdatePasswordPacket(const UpdatePasswordPacket &packet){
    UpdatePasswordPacket::PacketInfoType infoType = packet.InfoType;

    //TODO
    switch (infoType) {
    case UpdatePasswordPacket::INFO_TYPE_INIT_REQUEST:
    {
        quint8 authMode = UsersManager::passwordUpdateAuthMode();
        QByteArray captchaImage = QByteArray();
        QString userID = packet.AuthInfo.userID;
        QString securityQuestion = "";
        QString email = "";
        quint32 cellphoneNO = "";
        QString url = "";


        UpdatePasswordPacket::AuthMode mode = UpdatePasswordPacket::AuthMode(authMode);
        switch (mode) {
        case UpdatePasswordPacket::AUTH_OLD_PWD_ONLY:
            break;
        case UpdatePasswordPacket::AUTH_SECURITY_QUESTION:
        {

        }
            break;
        case UpdatePasswordPacket::AUTH_EMAIL:
        {

        }
            break;
        case UpdatePasswordPacket::AUTH_SMS:
        {

        }
            break;
        case UpdatePasswordPacket::AUTH_HTTP:
        {

        }
            break;
        default:
            break;
        }

        sendUpdatePasswordAuthInfoPacket(authMode, captchaImage, userID, securityQuestion, email, cellphoneNO, url);

    }
        break;
    case UpdatePasswordPacket::INFO_TYPE_CAPTCHA_REQUEST:
    {

    }
        break;
    case UpdatePasswordPacket::INFO_TYPE_CAPTCHA_IMAGE:
    {
        //TODO
    }
        break;

    case UpdatePasswordPacket::INFO_TYPE_AUTH_INFO_FROM_SERVER:
    {
    }
        break;

    case UpdatePasswordPacket::INFO_TYPE_AUTH_INFO_FROM_CLIENT:
    {


        UpdatePasswordPacket::AuthMode mode = UpdatePasswordPacket::AuthMode(AuthInfo.authMode);
        switch (mode) {
        case UpdatePasswordPacket::AUTH_OLD_PWD_ONLY:
        {
        }
            break;
        case UpdatePasswordPacket::AUTH_SECURITY_QUESTION:
        {
        }
            break;
        case UpdatePasswordPacket::AUTH_EMAIL:
        {
        }
            break;
        case UpdatePasswordPacket::AUTH_SMS:
        {
        }
            break;
        case UpdatePasswordPacket::AUTH_HTTP:
        {

        }
            break;
        default:
            break;
        }
    }
        break;

    case UpdatePasswordPacket::INFO_TYPE_AUTH_RESULT:
    {
    }
        break;

    case UpdatePasswordPacket::INFO_TYPE_UPDATE_RESULT:
    {
    }
        break;


    default:
        break;
    }


}

void ServerPacketsParser::processLoginPacket(const LoginPacket &packet){
    LoginPacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case LoginPacket::INFO_TYPE_LOGIN_SERVER_INFO:
    {
        QString userID = packet.getPeerID();
        IM::ErrorType errorType = IM::ERROR_UnKnownError;
        if(canUserLogin(userID, clientVersion, &errorType)){
            QHostAddress loginServerAddress = QHostAddress::Null;
            quint16 loginServerPort = 0;
            getUserLoginServer(userID, &loginServerAddress, &loginServerPort);
            sendClientCanLoginPacket(socketID, userID, loginServerAddress, loginServerPort);
        }else{
            sendClientCanNotLoginPacket(socketID, userID, quint8(errorType));
        }
    }
        break;
    case LoginPacket::INFO_TYPE_CAPTCHA:
    {
        //out << CaptchaInfo.type << CaptchaInfo.captchaImage << CaptchaInfo.captcha << CaptchaInfo.authResult;
    }
        break;
    case LoginPacket::INFO_TYPE_AUTH_INFO_FROM_CLIENT:
    {

        QString userID = packet.getPeerID();
        QByteArray encryptedPassword;
        quint8 onlineStateCode = quint8(IM::ONLINESTATE_ONLINE);
        QString deviceInfo = "";
        in >> encryptedPassword >> onlineStateCode >> deviceInfo;
        //        qDebug()<<"----onlineStateCode:"<<onlineStateCode;

        IM::ErrorType errorType = IM::ERROR_UnKnownError;
        UserInfo *userInfo = logUserIn(userID, encryptedPassword, IM::OnlineState(onlineStateCode), &errorType);
        if(userInfo){

            userInfo->setSocketID(socketID);

            QByteArray sessionEncryptionKey = userInfo->getSessionEncryptionKey();
            sendClientLoginSucceededPacket(socketID, userID, userInfo->encryptedPassword(), sessionEncryptionKey,
                                           userInfo->getPersonalSummaryInfoVersion(),
                                           userInfo->getPersonalDetailInfoVersion(),
                                           userInfo->getPersonalContactGroupsVersion(),
                                           userInfo->getInterestGroupInfoVersion(),
                                           userInfo->getPersonalMessageInfoVersion()
                                           );


            //Send all contact groups info to user
            sendPersonalContactGroupsInfoPacket(socketID, userInfo->getContactGroupsInfoString(), userInfo->getPersonalContactGroupsVersion(), userInfo->getSessionEncryptionKey());

            //Send all contacts version info to user
            QString contactsInfoString = "";
            if(getUserAllContactsInfoVersionFromDatabase(userInfo, &contactsInfoString)){
                if(!contactsInfoString.trimmed().isEmpty()){
                    sendPersonalContactsInfoVersionPacket(socketID, contactsInfoString, userInfo->getPersonalContactGroupsVersion(), sessionEncryptionKey);
                }
            }

            //Send last login info
            sendClientLastLoginInfoPacket(socketID, sessionEncryptionKey,
                                          userInfo->getLastLoginExternalHostAddress(),
                                          userInfo->getLastLoginTime().toString("yyyy-MM-dd hh:mm:ss"),
                                          userInfo->getLastLogoutTime().toString("yyyy-MM-dd hh:mm:ss"),
                                          userInfo->getLastLoginDeviceInfo());

            //Send all online contacts list to user
            sendContactsOnlineInfo(socketID, userInfo);

            //Save login or logout info, send info to contacts
            processUserOnlineStatusChanged(userInfo, onlineStateCode, peerAddress.toString(), peerPort, deviceInfo);


            QStringList messagesCachedOnServer = cachedChatMessagesForIMUser(userInfo);
            if(!messagesCachedOnServer.isEmpty()){
                sendCachedChatMessagesPacket(socketID, messagesCachedOnServer, sessionEncryptionKey);
            }

            QList<QStringList> sentApplicationList, receivedApplicationList;
            getFriendshipApplyRequest(userID, &sentApplicationList, &receivedApplicationList);
            if(!sentApplicationList.isEmpty()){
                foreach(QStringList infoList, sentApplicationList){
                    QString receiverID = infoList.at(0);
                    UserInfo *receiver = getUserInfo(receiverID);
                    if(!receiver){continue;}
                    UserInfo::FriendshipApplyResult faResult = UserInfo::FriendshipApplyResult(infoList.at(1).toUInt());
                    QString message = infoList.at(2);
                    if(faResult == UserInfo::FAR_ACCEPTED ){
                        sendAddContactResultPacket(socketID, receiverID, receiver->getNickName(), receiver->getFace(), userInfo->groupIDThatContactBelongsTo(receiverID), IM::ERROR_NoError, message, receiver->getOnlineState(),  sessionEncryptionKey, peerAddress.toString(), userInfo->getLastLoginExternalHostPort() );
                    }else{
                        sendAddContactResultPacket(socketID, receiverID, receiver->getNickName(), receiver->getFace(), -1, IM::ERROR_RequestDenied, message, receiver->getOnlineState(), sessionEncryptionKey, peerAddress.toString(), userInfo->getLastLoginExternalHostPort() );
                    }
                }
            }

            if(!receivedApplicationList.isEmpty()){
                foreach(QStringList infoList, receivedApplicationList){
                    QString senderID = infoList.at(0);
                    UserInfo *sender = getUserInfo(senderID);
                    if(!sender){continue;}
                    UserInfo::FriendshipApplyResult faResult = UserInfo::FriendshipApplyResult(infoList.at(1).toUInt());
                    QString message = infoList.at(2);
                    if(faResult == UserInfo::FAR_UNKNOWN ){
                        sendAddContactRequestFromUserPacket(socketID, senderID, sender->getNickName(), sender->getFace(), message, sessionEncryptionKey, peerAddress, peerPort );
                    }else if(faResult == UserInfo::FAR_ACCEPTED ){
                        sendAddContactResultPacket(socketID, senderID, sender->getNickName(), sender->getFace(), userInfo->groupIDThatContactBelongsTo(senderID), IM::ERROR_NoError, message, sender->getOnlineState(), sessionEncryptionKey, peerAddress.toString(), peerPort);
                    }/*else{
                        sendAddContactResultPacket(senderID, sender->getNickName(), sender->getFace(), IM::ERROR_RequestDenied, message, sessionEncryptionKey, clientAddress.toString(), clientPort );
                    }*/

                }
            }

            QStringList interestgroupChatMessagesCachedOnServer = getCachedInterestGroupChatMessagesForUserFromDB(userInfo);
            if(!interestgroupChatMessagesCachedOnServer.isEmpty()){
                sendCachedInterestGroupChatMessagesPacket(socketID, interestgroupChatMessagesCachedOnServer, sessionEncryptionKey, peerAddress, peerPort);
            }

            m_userSocketsHash.insert(socketID, userInfo);

            qWarning()<<QString("User %1 logged in!").arg(userID);
        }else{
            sendClientLoginFailedPacket(socketID, userID, quint8(errorType));
            qWarning()<<QString("User %1 Login Failed!").arg(userID);

        }
    }
        break;

    case LoginPacket::INFO_TYPE_LOGIN_RESULT:
    {
        //out << AuthResultInfo.authResult << AuthResultInfo.sessionEncryptionKey;
    }
        break;


    default:
        break;
    }



}

void ServerPacketsParser::processContactGroupsInfoPacket(const ContactGroupsInfoPacket &packet){
    UserInfo *userInfo = getUserInfo(packet.getPeerID());
    if(!userInfo){return;}

    ContactGroupsInfoPacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case ContactGroupsInfoPacket::PIT_GROUPS_LIST:
    {
        sendPersonalContactGroupsInfoPacket(socketID, userInfo->getContactGroupsInfoString(), userInfo->getPersonalContactGroupsVersion(), userInfo->getSessionEncryptionKey());

        slotProcessContactGroupsInfo();
        out << GroupsList.groupsInfo << GroupsList.version;
    }
        break;
    case ContactGroupsInfoPacket::PIT_GROUP_CHANGE_PARENT:
    {
        out << GroupParentInfo.groupID << GroupParentInfo.parentID;
    }
        break;
    case ContactGroupsInfoPacket::PIT_GROUP_CREATION:
    {
        out << GroupCreationInfo.name << GroupCreationInfo.parentID << GroupCreationInfo.id;
    }
        break;
    case ContactGroupsInfoPacket::PIT_GROUP_DELETION:
    {
        out << GroupDeletionInfo.id << GroupDeletionInfo.deleted;
    }
        break;
    case ContactGroupsInfoPacket::PIT_GROUP_RENAMING:
    {
        quint32 groupID = 0;
        QString newGroupName = "";

        updateContactGroupNameInDB(userInfo, groupID, newGroupName);

    }
        break;



    default:
        break;
    }

}


void ServerPacketsParser::processInterestGroupsInfoPacket(const InterestGroupsInfoPacket &packet){
    QString userID = packet.getPeerID();
    UserInfo *userInfo = getUserInfo(userID);
    if(!userInfo){return;}


    InterestGroupsInfoPacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case InterestGroupsInfoPacket::PIT_GROUPS_LIST:
    {
        sendUserInterestGroupsListPacket(packet.getSocketID(), userInfo);
    }
        break;
    case InterestGroupsInfoPacket::PIT_GROUP_INFO:
    {
        sendUserInterestGroupInfoPacket(socketID, userInfo, groupID);
    }
        break;
    case InterestGroupsInfoPacket::PIT_GROUP_UPDATE_ANNOUNCEMENT:
    {
        out << GroupAnnouncementInfo.content << GroupAnnouncementInfo.admin;
    }
        break;
    case InterestGroupsInfoPacket::PIT_GROUP_CREATION:
    {
        quint32 groupID = createNewInterestGroup(userInfo, packet.GroupCreationInfo.name, packet.GroupCreationInfo.type);
        sendCreateInterestGroupResultPacket(socketID, userInfo, groupID, groupName);
    }
        break;
    case InterestGroupsInfoPacket::PIT_GROUP_DELETION:
    {
        quint32 groupID = packet.GroupDeletionInfo.id;

        //TODO
        bool ok = disbandInterestGroup(userInfo, groupID);
        sendDisbandInterestGroupResultPacket(socketID, userInfo, groupID, ok);
        if(!ok){return;}

        InterestGroup *group = getInterestGroup(groupID);
        QStringList members = group->members();
        members.removeAll(userID);
        foreach (QString memberID, members) {
            UserInfo *member = getOnlineUserInfo(memberID);
            if(!member){continue;}
            member->joinOrLeaveInterestGroup(groupID, false);
            sendDisbandInterestGroupResultPacket(member->getSocketID(), member, groupID, ok);
        }

    }
        break;
    case InterestGroupsInfoPacket::PIT_GROUP_MEMBER_APPLICATION:
    {
        InterestGroup *group = getInterestGroup(groupID);
        if(!group){return;}

        if(userInfo->isMemberOfInterestGroup(groupID)){
            qWarning()<<QString("ERROR! Failed to join group! '%1' is already a member of group '%2' !").arg(userID).arg(groupID);
            return;
        }

        if(group->getPrivacy() == InterestGroupBase::Allow_Anyone_To_Join){
            bool ok = memberJoinOrQuitInterestGroup(userInfo, group, true);
            if(!ok){return;}
            //TODO:
            QStringList members = group->members();
            foreach (QString memberID, members) {
                UserInfo *member = getOnlineUserInfo(memberID);
                if(!member){continue;}
                sendUserJoinOrQuitInterestGroupResultToUserPacket(member->getSocketID(), groupID, memberID, true, member->getSessionEncryptionKey());
            }

        }else if(group->getPrivacy() == InterestGroupBase::Request_Verfication_To_Join){
            QStringList admins = group->getAdmins(true);
            bool requestSent = false;
            foreach (QString adminID, admins) {
                UserInfo *adminInfo = getOnlineUserInfo(userID);
                if(!adminInfo){continue;}
                sendUserJoinInterestGroupRequestToAdminPacket(adminInfo->getSocketID(), groupID, verificationMessage, userInfo);
                requestSent = true;
            }
            if(!requestSent){
                saveOrDeleteMembershipApplication(userID, groupID, verificationMessage, true);
            }


        }else{
            //TODO

        }

        out << MemberApplicationInfo.userID << MemberApplicationInfo.message << MemberApplicationInfo.approved << MemberApplicationInfo.admin;
    }
        break;

    case InterestGroupsInfoPacket::PIT_GROUP_MEMBER_DELETION:
    {
        if(userID  = packet.MemberDeletionInfo.userID){
        //Quit Group
        if(!userInfo->isMemberOfInterestGroup(groupID)){
            qWarning()<<QString("ERROR! Failed to quit group! '%1' is not a member of group '%2' !").arg(userID).arg(groupID);
            return;
            //TODO:Send Error Message
        }

        bool ok = memberJoinOrQuitInterestGroup(userInfo, group, false);
        if(!ok){return;}
        //TODO:
        QStringList members = group->members();
        foreach (QString memberID, members) {
            UserInfo *member = getOnlineUserInfo(memberID);
            if(!member){continue;}
            sendUserJoinOrQuitInterestGroupResultToUserPacket(member->getSocketID(), groupID, memberID, false, member->getSessionEncryptionKey());
        }
        sendUserJoinOrQuitInterestGroupResultToUserPacket(userInfo->getSocketID(), groupID, userID, false, userInfo->getSessionEncryptionKey());

        }
        //out << MemberDeletionInfo.userID << MemberDeletionInfo.blockForever << MemberDeletionInfo.admin;
    }
        break;

    case InterestGroupsInfoPacket::PIT_GROUP_MEMBER_PROMOTION:
    {
        out << MemberPromotionInfo.userID << MemberPromotionInfo.promoted;
    }
        break;



    default:
        break;
    }

}

void ServerPacketsParser::processContactInfoPacket(const ContactInfoPacket &packet){
    QString userID = packet.getPeerID();
    UserInfo *userInfo = getUserInfo(userID);
    if(!userInfo){return;}

    UserInfo *contactInfo = 0;
    QString contactID = packet.ContactID;
    if(userID == contactID){
        contactInfo = userInfo;
    }else{
        contactInfo = getUserInfo(contactID);
        if(!contactInfo){return;}
    }

    ContactInfoPacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case ContactInfoPacket::PIT_CONTACT_INFO:
    {
        quint8 requestSummaryInfo = packet.info.isSummaryInfo;
        QString infoString = contactInfo->getPersonalInfoString(requestSummaryInfo);

        sendUserInfoPacket(socketID, requestSummaryInfo, contactID, infoString, userInfo->getSessionEncryptionKey());
    }
        break;
    case ContactInfoPacket::PIT_GROUP_CHANGE:
    {
        if(userID == contactID){return;}
        if(!userInfo->hasFriendContact(contactID)){return;}

        quint32 oldGroupID = packet.ContactChangeGroup.oldGroupID;
        quint32 newGroupID = packet.ContactChangeGroup.newGroupID;

        bool ok = moveContactForUserInDB(userInfo, contactID, newGroupID);
        if(ok){
            userInfo->moveContactToAnotherGroup(contactID, oldGroupID, newGroupID);
        }

    }
        break;

    case ContactInfoPacket::PIT_FRIENDING_REQUEST:
    {

        QString verificationMessage = packet.ContactFriendingRequest.message;
        quint32 groupID = packet.ContactFriendingRequest.groupID;
        if(userID == contactID){return;}

        if(userInfo->hasFriendContact(contactID)){
            //sendAddContactResultPacket(contactID, IM::ERROR_RequestDenied, "Contact already exists!", userInfo->getSessionEncryptionKey(), clientAddress, clientPort);
            //TODO:Send Error Message
            qWarning()<<QString("'%1' already has contact '%2' !").arg(userID).arg(contactID);
            return;
        }

        if(!userInfo->hasContactGroup(groupID)){
            groupID = ContactGroupBase::Group_Friends_ID;
        }

        if(contactInfo->getFriendshipApply() == UserInfo::FA_AUTO_ACCEPT){
            addContactForUser(userInfo, contactInfo, groupID);


        }else{
            if(contactInfo->getOnlineState() == IM::ONLINESTATE_OFFLINE){
                //TODO:保存请求到数据库
                saveFriendshipApplyRequestToDB(userID, contactID, verificationMessage);
            }else{
                sendAddContactRequestFromUserPacket(contactInfo->getSocketID(), userID, userInfo->getNickName(), userInfo->getFace(), verificationMessage, contactInfo->getSessionEncryptionKey(), peerAddress, peerPort);
            }
        }

        in >> ContactFriendingRequest.message;
    }
        break;

    case ContactInfoPacket::PIT_FRIENDING_RESULT:
    {
        QString contactID = "", extraMessage = "";
        bool acceptRequest = true;
        quint32 groupID = ContactGroupBase::Group_Friends_ID;
        stream >> contactID >> acceptRequest;

        stream >> groupID >> extraMessage;

        UserInfo *contactInfo = getUserInfo(contactID);
        if(!contactInfo){return;}


        //TODO:
        if(acceptRequest){
            addContactForUser(userInfo, contactInfo, groupID);

//            contactInfo->addNewContact(userID, UserInfo::defaultFriendContactGroupID());

//            if(contactInfo->getOnlineState() == IM::ONLINESTATE_OFFLINE){
//                //TODO:保存请求到数据库
//                saveFriendshipApplyRequestToDB(contactID, userID, extraMessage, UserInfo::FAR_ACCEPTED, false, true);
//                return;
//            }else{
//                sendAddContactResultPacket(socketID, userID, userInfo->getNickName(), userInfo->getFace(), IM::ERROR_NoError, "", contactInfo->getSessionEncryptionKey(), contactInfo->getLastLoginExternalHostAddress(), contactInfo->getLastLoginExternalHostPort());
//            }

        }else{
            if(!extraMessage.trimmed().isEmpty()){
                if(contactInfo->getOnlineState() == IM::ONLINESTATE_OFFLINE){
                    //TODO:保存请求到数据库
                    saveFriendshipApplyRequestToDB(contactID, userID, extraMessage, UserInfo::FAR_DENIED, false, true);
                    return;
                }else{
                    sendAddContactResultPacket(contactInfo->getSocketID(), userID, userInfo->getNickName(), userInfo->getFace(), -1, IM::ERROR_RequestDenied, extraMessage, userInfo->getOnlineState(), contactInfo->getSessionEncryptionKey(), contactInfo->getLastLoginExternalHostAddress(), contactInfo->getLastLoginExternalHostPort());
                }
            }

        }
        //TODO:从数据库删除请求
        deleteFriendshipApplyRequest(contactID, userID);

        in >> ContactFriendingResult.message >> ContactFriendingResult.approved >> ContactFriendingResult.blocked;
    }
        break;

    case ContactInfoPacket::PIT_CONTACT_DELETION:
    {
        QString contactID = "";
        quint8 deleteMeFromOpposition = 0, addToBlacklist = 0;
        stream >> contactID >> deleteMeFromOpposition >> addToBlacklist;

        //TODO:数据库
        //deleteFriendshipApplyRequest(userID, contactID);
        deleteContactForUserFromDB(userID, contactID, deleteMeFromOpposition, addToBlacklist);

        userInfo->deleteContact(contactID);
        if(deleteMeFromOpposition){
            //TODO:
            UserInfo *contactInfo = getOnlineUserInfo(contactID);
            if(!contactInfo){
                contactInfo = getOfflineUserInfo(contactID);;
            }
            if(!contactInfo){return;}
            contactInfo->deleteContact(userID);

            //deleteFriendshipApplyRequest(contactID, userID);
        }
        if(addToBlacklist){
            userInfo->addOrDeleteBlacklistedContact(contactID, true);
            userInfo->clearUpdatedProperties();
        }

        //TODO
        sendDeleteContactResultPacket(socketID, contactID, true, addToBlacklist, userInfo->getSessionEncryptionKey());

    }
        break;

    case ContactInfoPacket::PIT_CONTACT_REMARK:
    {
        in >> ContactRemarkInfo.newRemarkName;
    }
        break;


    default:
        break;
    }

}

void ServerPacketsParser::processSearchInfoPacket(const SearchInfoPacket &packet){
    QString userID = packet.getPeerID();
    UserInfo *userInfo = getUserInfo(userID);
    if(!userInfo){return;}

    SearchInfoPacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case SearchInfoPacket::PIT_SEARCH_CONTACT_CONDITIONS:
    {
        QString usersListString = searchContact(propertiesString, matchExactly, searchOnlineUsersOnly, searchWebcamUsersOnly, startIndex);
        if(!usersListString.isEmpty()){
            sendSearchContactResultPacket(socketID, usersListString, userInfo->getSessionEncryptionKey(), peerAddress, peerPort);
        }


        out << SearchContactConditions.keyword << SearchContactConditions.isOnline
                << SearchContactConditions.hasWebcam << SearchContactConditions.location
                << SearchContactConditions.hometown << SearchContactConditions.gender
                << SearchContactConditions.age << SearchContactConditions.matchExactly
                << SearchContactConditions.startIndex
                ;
    }
        break;
    case SearchInfoPacket::PIT_SEARCH_CONTACT_RESULT:
    {
        out << SearchContactResult.result;
    }
        break;
    case SearchInfoPacket::PIT_SEARCH_INTEREST_GROUP_CONDITIONS:
    {
        QString keyword = packet.SearchInterestGroupConditions.keyword;
        int startIndex = packet.SearchInterestGroupConditions.startIndex;

        QString groupsListString = searchInterestGroup(keyword, startIndex);
        if(!groupsListString.isEmpty()){
            sendSearchInterestGroupResultPacket(socketID, groupsListString, userInfo->getSessionEncryptionKey());
        }

    }
        break;
    case SearchInfoPacket::PIT_SEARCH_INTEREST_GROUP_RESULT:
    {
        out << SearchInterestGroupResult.result;
    }
        break;



    default:
        break;
    }

}

void ServerPacketsParser::processChatMessagePacket(const ChatMessagePacket &packet){
    QString userID = packet.getPeerID();
    UserInfo *userInfo = getUserInfo(userID);
    if(!userInfo){return;}

    SearchInfoPacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case ChatMessagePacket::PIT_CONTACT_CHAT_MESSAGE:
    {

        QString contactID = packet.ContactChatMessage.contactID;
        UserInfo *contactInfo = getUserInfo(contactID);
        if(!contactInfo){return;}

        QString message = packet.ContactChatMessage.message;
        QString imageNames = packet.ContactChatMessage.imageNames;

        saveCachedChatMessageFromIMUser(userID, contactID, message);

        QStringList imageNameList = imageNames.split(",");
        if(imageNameList.isEmpty()){return;}
        foreach (QString imageName, imageNameList) {
            if(!QFile::exists(m_chatImageCacheDir + "/" + imageName)){
                qDebug()<<QString("Image '%1' exists on server!").arg(imageName);
                sendRequestImagePacket(socketID, userID, imageName, userInfo->getSessionEncryptionKey());
            }
        }


        //Send To Contact
        if(contactInfo->getOnlineState() != IM::ONLINESTATE_OFFLINE){
            sendContactChatMessagePacket(contactInfo->getSocketID(), userID, message, contactInfo->getSessionEncryptionKey());
        }


    }
        break;
    case ChatMessagePacket::PIT_CONTACT_CHAT_MESSAGES_CACHED_ON_SERVER:
    {
        out << ContactChatMessageCachedOnServer.messages;
    }
        break;
    case ChatMessagePacket::PIT_CONTACT_CHAT_HISTORY_MESSAGES:
    {
        out << ContactChatHistoryMessages.contactID << ContactChatHistoryMessages.messages << ContactChatHistoryMessages.startime;
    }
        break;
    case ChatMessagePacket::PIT_GROUP_CHAT_MESSAGE:
    {
        quint32 interestGroupID = 0;
        QString  message = "", imageNames = "";
        stream >> interestGroupID >> message >> imageNames;

        if(userInfo->isMemberOfInterestGroup(interestGroupID)){
            saveCachedInterestGroupChatMessageFromIMUser(userID, interestGroupID, message);
        }else{
            qCritical()<<QString("User '%1' is not a member of group '%2'!").arg(userID).arg(interestGroupID);
            return;
        }

        QStringList imageNameList = imageNames.split(",");
        if(imageNameList.isEmpty()){return;}
        foreach (QString imageName, imageNameList) {
            if(!QFile::exists(m_chatImageCacheDir + "/" + imageName)){
                qDebug()<<QString("Image '%1' exists on server!").arg(imageName);
                sendRequestImagePacket(socketID, userID, imageName, userInfo->getSessionEncryptionKey());
            }
        }

        //TODO:Send message to all contacts
        sendInterestGroupChatMessageToMembers(interestGroupID, userInfo, message);


       out << GroupChatMessage.groupID << GroupChatMessage.memberID << GroupChatMessage.time << GroupChatMessage.message;
    }
        break;
    case ChatMessagePacket::PIT_GROUP_CHAT_MESSAGES_CACHED_ON_SERVER:
    {
        out << GroupChatMessagesCachedOnServer.messages;
    }
        break;
    case ChatMessagePacket::PIT_GROUP_CHAT_HISTORY_MESSAGES:
    {
        out << GroupChatHistoryMessages.groupID << GroupChatHistoryMessages.messages << GroupChatHistoryMessages.startime;
    }
        break;
    case ChatMessagePacket::PIT_CHAT_IMAGE:
    {

        bool isRequest = packet.ChatImage.isRequest;
        QString imageName = packet.ChatImage.name;
        QByteArray image = packet.ChatImage.image;
        if(isRequest){
            //TODO:Check cached file
            QFile file(m_chatImageCacheDir + "/" + imageName);
            if(file.exists()){
                qDebug()<<QString("Image '%1' exists on server!").arg(imageName);
                QByteArray ba;
                if (file.open(QFile::ReadOnly)) {
                    ba = file.readAll();
                    file.close();

                    sendImagePacket(socketID, contactID, imageName, ba, userInfo->getSessionEncryptionKey());
                    return ;
                }else{
                    qCritical()<< QString("ERROR! Failed to open image '%1'! %2").arg(imageName).arg(file.errorString());
                }
            }


            UserInfo *contactInfo = getOnlineUserInfo(contactID);
            if(!contactInfo){
                sendImagePacket(socketID, contactID, imageName, QByteArray(), userInfo->getSessionEncryptionKey());
                return;
            }

            sendRequestImagePacket(contactInfo->getSocketID(), userID, imageName, contactInfo->getSessionEncryptionKey());

        }else{

            if(image.isNull()){
                qCritical()<<"ERROR! Invalid image!";
                return;
            }

            //TODO:Save image
            Settings settings;
            QString cacheDir = settings.getChatImageCacheDir();
            QString fileName = cacheDir + "/" + imageName;

            QFile file(fileName);
            if(!file.exists()){
                if(!file.open(QFile::WriteOnly)){
                    qCritical()<< QString("ERROR! Failed to write image '%1'! %2").arg(imageName).arg(file.errorString());
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
    case ChatMessagePacket::PIT_SESSION_ENCRYPTION_KEY_WITH_CONTACT:
    {
        UserInfo *contactInfo = getUserInfo(contactID);
        if(!contactInfo){return;}
        if(contactInfo->isOnLine()){
            QByteArray key = ServerUtilities::generateSessionEncryptionKey();
            sendSessionEncryptionKeyWithContact(socketID, userID, contactInfo->getSessionEncryptionKey(), key, contactInfo->getLastLoginExternalHostAddress(), contactInfo->getLastLoginExternalHostPort());
            sendSessionEncryptionKeyWithContact(socketID, contactID, userInfo->getSessionEncryptionKey(), key, peerAddress.toString(), peerPort);
        }else{
            sendContactOnlineStatusChangedPacket(socketID, contactID, quint8(IM::ONLINESTATE_OFFLINE), userInfo->getSessionEncryptionKey(), contactInfo->getLastLoginExternalHostAddress(), contactInfo->getLastLoginExternalHostPort(), peerAddress.toString(), peerPort);

        }
    }
        break;

    default:
        break;
    }


}

void ServerPacketsParser::processCaptchaInfoPacket(const CaptchaInfoPacket &packet){
    QString userID = packet.getPeerID();
    UserInfo *userInfo = getUserInfo(userID);
    if(!userInfo){return;}

    CaptchaInfoPacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case CaptchaInfoPacket::CAPTCHA_REQUEST:
    {
    }
        break;
    case CaptchaInfoPacket::CAPTCHA_IMAGE:
    {
        out << captchaImage;
    }
        break;
    case CaptchaInfoPacket::CAPTCHA_CODE:
    {
        out << captchaCode;
    }
        break;
    case CaptchaInfoPacket::CAPTCHA_AUTH_RESULT:
    {
        out << approved;
    }
        break;



    default:
        break;
    }


}

void ServerPacketsParser::processFileTransferPacket(const FileTransferPacket &packet){
    QString userID = packet.getPeerID();
    UserInfo *userInfo = getUserInfo(userID);
    if(!userInfo){return;}

    FileTransferPacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case FileTransferPacket::FT_FILE_SERVER_INFO:
    {
        //TODO
        QString address = "";
        quint16 port = 0;
        responseFileServerInfo(packet.getSocketID(), address, port);
    }
        break;

    case FileTransferPacket::FT_FileSystemInfoRequest:
    {
        //in >> FileSystemInfoRequest.parentDirPath;
    }
        break;

    case FileTransferPacket::FT_FileSystemInfoResponse:
    {
        //in >> FileSystemInfoResponse.baseDirPath >> FileSystemInfoResponse.fileSystemInfoData;
    }
        break;

    case FileTransferPacket::FT_FileDeletingRequest:
    {
        //in >> FileDeletingRequest.baseDirPath >> FileDeletingRequest.files;
    }
        break;

    case FileTransferPacket::FT_FileDeletingResponse:
    {
        //in >> FileDeletingResponse.baseDirPath >> FileDeletingResponse.failedFiles;
    }
        break;

    case FileTransferPacket::FT_FileRenamingRequest:
    {
        //in >> FileRenamingRequest.baseDirPath >> FileRenamingRequest.oldFileName >> FileRenamingRequest.newFileName;
    }
        break;

    case FileTransferPacket::FT_FileRenamingResponse:
    {
        //in >> FileRenamingResponse.baseDirPath >> FileRenamingResponse.oldFileName >> FileRenamingResponse.renamed >> FileRenamingResponse.message;
    }
        break;

    case FileTransferPacket::FileTransferPacket::FT_FileDownloadingRequest:
    {
        //in >> FileDownloadingRequest.baseDir >> FileDownloadingRequest.fileName >> FileDownloadingRequest.dirToSaveFile;
    }
        break;

    case FileTransferPacket::FT_FileDownloadingResponse:
    {
        //in >> FileDownloadingResponse.accepted >> FileDownloadingResponse.baseDir >> FileDownloadingResponse.fileName >> FileDownloadingResponse.fileMD5Sum >> FileDownloadingResponse.size >> FileDownloadingResponse.errorCode;
    }
        break;

    case FileTransferPacket::FT_FileUploadingRequest:
    {
        //in >> FileUploadingRequest.fileName >> FileUploadingRequest.fileMD5Sum >> FileUploadingRequest.size >> FileUploadingRequest.fileSaveDir;
    }
        break;

    case FileTransferPacket::FT_FileUploadingResponse:
    {
        //in >> FileUploadingResponse.accepted >> FileUploadingResponse.fileMD5Sum >> FileUploadingResponse.message;
    }
        break;

    case FileTransferPacket::FT_FileDataRequest:
    {
        //in >> FileDataRequest.fileMD5 >> FileDataRequest.startPieceIndex >> FileDataRequest.endPieceIndex;
    }
        break;

    case FileTransferPacket::FT_FileData:
    {
        //in >> FileDataResponse.fileMD5 >> FileDataResponse.pieceIndex >> FileDataResponse.data >> FileDataResponse.pieceMD5;
    }
        break;

    case FileTransferPacket::FT_FileTXStatus:
    {
        //in >> FileTXStatus.fileMD5 >> FileTXStatus.status;
    }
        break;

    case FileTransferPacket::FT_FileTXError:
    {
        //in >> FileTXError.fileName >> FileTXError.fileMD5 >> FileTXError.errorCode >> FileTXError.message;
    }
        break;

    default:
        break;
    }


}







UserInfo* ServerPacketsParser::logUserIn(const QString &userID, const QByteArray &encryptedPassword, IM::OnlineState loginState, IM::ErrorType *errorType){
    //qWarning()<<"logUserIn(...)";

    UserInfo *userInfo = getUserInfo(userID);
    if(!userInfo){
        *errorType = IM::ERROR_IDNotExist;
        return 0;
    }

    //TODO:密码保存方式
    QByteArray decryptedPassword;
    Cryptography cryptography;
    cryptography.teaCrypto(&decryptedPassword, encryptedPassword, userInfo->encryptedPassword(), false);

    if(decryptedPassword != QByteArray::fromBase64(userInfo->getPassword().toUtf8())){
        *errorType = IM::ERROR_PasswordIncorrect;
        userInfo->authenticationFailed();
        return 0;
    }else{

        //Check Duplicate Login
        if(userInfo->getOnlineState() != IM::ONLINESTATE_OFFLINE){
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
