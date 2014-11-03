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



void ServerPacketsParser::parseIncomingPacketData(Packet *packet){

    //    qDebug()<<"----ServerPacketsParser::parseIncomingPacketData(Packet *packet)";


    QByteArray packetData = packet->getPacketData();
    QDataStream in(&packetData, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    QString peerID = "";
    in >> peerID;

    QHostAddress peerAddress = packet->getPeerHostAddress();
    quint16 peerPort = packet->getPeerHostPort();
    quint8 packetType = packet->getPacketType();
    int socketID = packet->getSocketID();

    qDebug()<<"--ServerPacketsParser::parseIncomingPacketData(...) "<<" socketID:"<<socketID <<" peerID:"<<peerID<<" peerAddress:"<<peerAddress.toString()<<" peerPort:"<<peerPort<<" packetType:"<<packetType;

    PacketHandlerBase::recylePacket(packet);

    switch(packetType){
    //    case quint8(HEHUI::HeartbeatPacket):
    //    {
    //        QString userID = peerID;
    //        //in >> userID;

    //        UserInfo *userInfo = getOnlineUserInfo(userID);
    //        if(userInfo){
    //            userInfo->updateRemainingOnlineStatusCheckingTimes(true);
    //        }

    //        emit signalHeartbeatPacketReceived(packet->getPeerHostAddress().toString(), userID);
    //        qWarning()<<"~~HeartbeatPacket--"<<" userID:"<<userID;
    //    }
    //    break;
    //    case quint8(HEHUI::ConfirmationOfReceiptPacket):
    //    {
    //        quint16 packetSerialNumber1 = 0, packetSerialNumber2 = 0;
    //        in >> packetSerialNumber1 >> packetSerialNumber2;
    //        m_packetHandlerBase->removeWaitingForReplyPacket(packetSerialNumber1, packetSerialNumber2);
    //        emit signalConfirmationOfReceiptPacketReceived(packetSerialNumber1, packetSerialNumber2);
    //        //qDebug()<<"~~ConfirmationOfReceiptPacket--"<<"packetSerialNumber:"<<packetSerialNumber;
    //    }
    //    break;


    case quint8(IM::DataForwardRequestByClient):
    {
        qDebug()<<"~~DataForwardRequestByClient";

        QByteArray encryptedData;
        QString senderID = peerID;

        UserInfo *senderInfo = getOnlineUserInfo(senderID);
        if(!senderInfo){
            qCritical()<<"ERROR! Sender Not Found!";
            return;
        }

        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, senderInfo->getSessionEncryptionKey(), false);
        //TODO

        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);

        QString receiverID = "";
        QByteArray data;
        stream >> receiverID >> data;


        UserInfo *receiverInfo = getUserInfo(receiverID);
        if(!receiverInfo){
            qCritical()<<"ERROR! Receiver Not Found!";
            return;
        }
        if(!receiverInfo->isOnLine()){
            qCritical()<<"ERROR! Receiver Offline!";
            return;
        }

        forwardDataForward(m_userSocketsHash.key(receiverInfo), data, receiverInfo->getSessionEncryptionKey());

    }
        break;

    case quint8(IM::ClientLookForServer):
    {
        qDebug()<<"~~ClientLookForServer";

        QString clientVersion = "";
        in >> clientVersion;
        //TODO:Version

        sendServerDeclarePacket(peerAddress, peerPort);
        //emit signalClientLookForServerPacketReceived(peerAddress, peerPort, peerName);
        //qDebug()<<"~~ClientLookForServer--"<<" peerAddress:"<<peerAddress.toString()<<"   peerPort:"<<peerPort;
    }
        break;

    case quint8(IM::CLIENT_REQUEST_REGISTRATION_SERVER_INFO):
    {
        qDebug()<<"~~CLIENT_REQUEST_REGISTRATION_SERVER_INFO";

        QString clientVersion = "";
        in >> clientVersion;


        //TODO
        IM::ErrorType errorype = IM::ERROR_NoError;
        bool canRegister = true;
        QString extraMessage = "TEST";
        IM::RegistrationMode regMode = IM::RM_UserDefineAll;
        QString regServerAddress = "";
        bool requireActivation = false;

        sendRegistrationServerInfoPacket(socketID, quint8(errorype), extraMessage, canRegister, regMode, regServerAddress, requireActivation);

    }
        break;

    case quint8(IM::CLIENT_REGISTRATION):
    {
        qDebug()<<"~~CLIENT_REGISTRATION";
        
        QString userID = "", password = "";
        in >> userID >> password;

        IM::ErrorType errorype = IM::ERROR_UnKnownError;
        QString message = "";
        quint32 sysID = 0;
        registerNewUser(userID, password, &errorype, &sysID, &message);
        sendClientRegistrationResultPacket(socketID, quint8(errorype), sysID, message);

    }
        break;

    case quint8(IM::CLIENT_REQUEST_UPDATE_PASSWORD):
    {

        qDebug()<<"~~CLIENT_REQUEST_UPDATE_PASSWORD";

        QString userID = peerID;
        QByteArray encryptedNewPassword;
        in >> encryptedNewPassword;

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedNewPassword)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);
        QString newPassword = "";
        stream >> newPassword;

        IM::ErrorType errorype = IM::ERROR_UnKnownError;
        QString message = "";
        updateUserPassword(userID, newPassword, &errorype, &message);
        sendClientUpdatePasswordResultPacket(socketID, quint8(errorype), message);

    }
        break;

    case quint8(IM::CLIENT_REQUEST_LOGIN):
    {
        qDebug()<<"~~CLIENT_REQUEST_LOGIN";

        QString userID = peerID;

        QString clientVersion = "";
        in >> clientVersion;


        IM::ErrorType errorType = IM::ERROR_UnKnownError;
        if(canUserLogin(userID, clientVersion, &errorType)){
            QHostAddress loginServerAddress = QHostAddress::Null;
            quint16 loginServerPort = 0;
            getUserLoginServer(userID, &loginServerAddress, &loginServerPort);
            sendClientCanLoginPacket(socketID, userID, loginServerAddress, loginServerPort);
        }else{
            sendClientCanNotLoginPacket(socketID, userID, quint8(errorType));
        }

        //        emit signalUserRequestLoginPacketReceived(userID, clientVersion);
        //qWarning()<<"~~CLIENT_REQUEST_LOGIN--"<<" peerAddress:"<<clientAddress.toString()<<"   peerPort:"<<clientPort;

    }
        break;

    case quint8(IM::CLIENT_LOGIN_INFO):
    {
        qDebug()<<"--CLIENT_LOGIN_INFO";

        QString userID = peerID;
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

        //        emit signalUserLoginInfoPacketReceived(userID, encryptedPassword, IM::OnlineState(onlineStateCode));
        //        qWarning()<<"~~CLIENT_LOGIN_INFO--"<<" peerAddress:"<<clientAddress.toString()<<"   peerPort:"<<clientPort;

    }
        break;

    case quint8(IM::ONLINE_STATE_CHANGED):
    {
        qDebug()<<"~~ONLINE_STATE_CHANGED";

        QString userID = peerID;
        QByteArray encryptedOnlineState;
        in >> encryptedOnlineState;

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedOnlineState)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);
        quint8 onlineStateCode = quint8(IM::ONLINESTATE_ONLINE);
        stream >> onlineStateCode;

        processUserOnlineStatusChanged(userID, IM::OnlineState(onlineStateCode), peerAddress.toString(), peerPort);

        //        emit signalUserOnlineStateChanged(userID, encryptedOnlineState);

    }
        break;

    case quint8(IM::CONTACT_GROUPS_INFO):
    {
        qDebug()<<"~~CONTACT_GROUPS_INFO";

        QString userID =peerID;

        UserInfo *userInfo = getOnlineUserInfo(userID);
        if(!userInfo){return;}

        sendPersonalContactGroupsInfoPacket(socketID, userInfo->getContactGroupsInfoString(), userInfo->getPersonalContactGroupsVersion(), userInfo->getSessionEncryptionKey());
    }
        break;
    case quint8(IM::CONTACTS_INFO_VERSION):
    {
        qDebug()<<"~~CONTACTS_INFO_VERSION";

        QString userID =peerID;

        UserInfo *userInfo = getOnlineUserInfo(userID);
        if(!userInfo){return;}

        QString versionInfo = "";
        if(!getUserAllContactsInfoVersionFromDatabase(userInfo, &versionInfo)){
            return;
        }

        sendPersonalContactsInfoVersionPacket(socketID, versionInfo, userInfo->getPersonalContactGroupsVersion(), userInfo->getSessionEncryptionKey());
    }
        break;

    case quint8(IM::CLIENT_REQUEST_USER_INFO):
    {
        qDebug()<<"~~CLIENT_REQUEST_USER_INFO";

        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;
        //        qWarning()<<"userID:"<<userID << encryptedUserID.size();
        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        
        //QBuffer buf(&decryptedData);
        //buf.open(QIODevice::ReadOnly);
        //in.setDevice(&buf);
        //in.device()->seek(0);
        //QString contactID = "";
        //in >> contactID;
        
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);
        QString contactID = "";
        quint8 requestSummaryInfo = 1;
        stream >> contactID >> requestSummaryInfo;
        //        qWarning()<<"contactID:"<<contactID;

        QString userInfoString = "";
        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}
        if(userID == contactID){
            userInfoString = userInfo->getPersonalInfoString(requestSummaryInfo);
        }else{
            UserInfo *contactInfo = getUserInfo(contactID);
            if(!contactInfo){return;}
            userInfoString = contactInfo->getPersonalInfoString(requestSummaryInfo);
        }
        sendUserInfoPacket(socketID, contactID, userInfoString, userInfo->getSessionEncryptionKey(), requestSummaryInfo);

        //        emit signalUserRequestUserInfo(userID, encryptedUserID);
    }
        break;

    case quint8(IM::CLIENT_REQUEST_SEARCH_CONTACTS):
    {
        qDebug()<<"~~CLIENT_REQUEST_SEARCH_CONTACTS";

        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;
        
        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);

        QString propertiesString = "";
        bool matchExactly = true, searchOnlineUsersOnly = true, searchWebcamUsersOnly = false;
        int startIndex = 0;
        stream >> propertiesString >> matchExactly >> searchOnlineUsersOnly >> searchWebcamUsersOnly >> startIndex;
        
        QString usersListString = searchContact(propertiesString, matchExactly, searchOnlineUsersOnly, searchWebcamUsersOnly, startIndex);
        if(!usersListString.isEmpty()){
            sendSearchContactResultPacket(socketID, usersListString, userInfo->getSessionEncryptionKey(), peerAddress, peerPort);
        }
        
    }
        break;

    case quint8(IM::CLIENT_REQUEST_SEARCH_INTERESTGROUPS):
    {
        qDebug()<<"~~CLIENT_REQUEST_SEARCH_INTERESTGROUPS";

        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;

        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);

        QString keyword = "";
        int startIndex = 0;
        stream >> keyword >> startIndex;

        QString groupsListString = searchInterestGroup(keyword, startIndex);
        if(!groupsListString.isEmpty()){
            sendSearchInterestGroupResultPacket(socketID, groupsListString, userInfo->getSessionEncryptionKey());
        }

    }
        break;


    case quint8(IM::CLIENT_REQUEST_JOIN_OR_QUIT_INTERESTGROUP):
    {
        qDebug()<<"--CLIENT_REQUEST_JOIN_OR_QUIT_INTERESTGROUP";

        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;

        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);

        quint32 groupID = 0;
        quint8 join = 0;
        QString verificationMessage = "";
        stream >> groupID >> join >> verificationMessage;

        InterestGroup *group = getInterestGroup(groupID);
        if(!group){return;}

        if(join){
            if(userInfo->isMemberOfInterestGroup(groupID)){
                qWarning()<<QString("ERROR! Failed to join group! '%1' is already a member of group '%2' !").arg(userID).arg(groupID);
                return;
                //TODO:Send Error Message
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


        }else{
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






    }
        break;

    case quint8(IM::CLIENT_REQUEST_ADD_CONTACT):
    {
        qDebug()<<"--CLIENT_REQUEST_ADD_CONTACT";

        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;

        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);
        QString contactID = "", verificationMessage = "";
        quint32 groupID = ContactGroupBase::Group_Friends_ID;
        stream >> contactID >> verificationMessage >> groupID;

        if(userID == contactID){return;}

        UserInfo *contactInfo = getUserInfo(contactID);
        if(!contactInfo){return;}

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

//            if(addNewContactForUserInDB(userID, contactID, groupID)){
//                sendAddContactResultPacket(socketID, contactID, contactInfo->getNickName(), contactInfo->getFace(), IM::ERROR_NoError, "", userInfo->getSessionEncryptionKey(), userInfo->getLastLoginExternalHostAddress(), userInfo->getLastLoginExternalHostPort());
//                userInfo->addNewContact(contactID, groupID);
//            }else{
//                sendAddContactResultPacket(socketID, contactID, contactInfo->getNickName(), contactInfo->getFace(), IM::ERROR_ServerError, "", userInfo->getSessionEncryptionKey(), userInfo->getLastLoginExternalHostAddress(), userInfo->getLastLoginExternalHostPort());
//            }

//            contactInfo->addNewContact(userID, contactInfo->defaultFriendContactGroupID());
//            addNewContactForUserInDB(contactID, userID, groupID);
//            if(contactInfo->getOnlineState() != IM::ONLINESTATE_OFFLINE){
//                sendAddContactResultPacket(socketID, userID, userInfo->getNickName(), userInfo->getFace(), IM::ERROR_NoError, "", contactInfo->getSessionEncryptionKey(), contactInfo->getLastLoginExternalHostAddress(), contactInfo->getLastLoginExternalHostPort());
//            }else{
//                //TODO:保存消息到数据库
//                saveFriendshipApplyRequestToDB(userID, contactID, verificationMessage, UserInfo::FAR_ACCEPTED, true);
//            }

        }else{
            if(contactInfo->getOnlineState() == IM::ONLINESTATE_OFFLINE){
                //TODO:保存请求到数据库
                saveFriendshipApplyRequestToDB(userID, contactID, verificationMessage);
            }else{
                sendAddContactRequestFromUserPacket(contactInfo->getSocketID(), userID, userInfo->getNickName(), userInfo->getFace(), verificationMessage, contactInfo->getSessionEncryptionKey(), peerAddress, peerPort);
            }
        }


    }
        break;

    case quint8(IM::CLIENT_REQUEST_MOVE_CONTACT):
    {
        qDebug()<<"~~CLIENT_REQUEST_MOVE_CONTACT";

        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;
        
        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);
        QString contactID = "";
        quint32 oldGroupID = 0, newGroupID = 0;
        stream >> contactID >> oldGroupID >> newGroupID;
        
        if(userID == contactID){return;}
        
        UserInfo *contactInfo = getUserInfo(contactID);
        if(!contactInfo){return;}
        if(!userInfo->hasFriendContact(contactID)){return;}
        
        bool ok = moveContactForUserInDB(userInfo, contactID, newGroupID);
        if(ok){
            userInfo->moveContactToAnotherGroup(contactID, oldGroupID, newGroupID);
        }
//        sendMoveContactToGroupResultPacket(socketID, contactID, oldGroupID, newGroupID, ok, userInfo->getPersonalContactGroupsVersion(), userInfo->getSessionEncryptionKey());
        
    }
        break;

    case quint8(IM::CLIENT_RESPONSE_ADD_CONTACT_REQUEST):
    {
        qDebug()<<"~~CLIENT_RESPONSE_ADD_CONTACT_REQUEST";

        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;
        
        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);
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
        
    }
        break;

    case quint8(IM::CLIENT_REQUEST_DELETE_CONTACT):
    {
        qDebug()<<"~~CLIENT_REQUEST_DELETE_CONTACT";

        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;

        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);
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


        //qDebug()<<"------------contactID:"<<contactID<<" groupName:"<<groupName;

    }
        break;

//    case quint8(IM::CLIENT_REQUEST_ADD_OR_DELETE_BLACKLISTED_CONTACT):
//    {
//        qDebug()<<"~~CLIENT_REQUEST_ADD_OR_DELETE_BLACKLISTED_CONTACT";

//        QString userID = peerID;
//        QByteArray encryptedData;
//        in >> encryptedData;

//        UserInfo *userInfo = getUserInfo(userID);
//        if(!userInfo){return;}

//        //解密数据
//        QByteArray decryptedData;
//        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
//        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
//        stream.setVersion(QDataStream::Qt_4_7);
//        QString contactID = "";
//        bool addToBlacklist = true;
//        stream >> contactID >> addToBlacklist;

//        addOrDeleteBlacklistedContactForUserFromDB(userID, contactID, addToBlacklist);

//        userInfo->addOrDeleteBlacklistedContact(contactID, addToBlacklist);
//        userInfo->clearUpdatedProperties();
//    }
//        break;

    case quint8(IM::CLIENT_REQUEST_INTEREST_GROUPS_LIST):
    {
        qDebug()<<"~~CLIENT_REQUEST_INTEREST_GROUPS_LIST";

        QString userID = peerID;
        //in >> userID ;

        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}
        sendUserInterestGroupsListPacket(socketID, userInfo, peerAddress, peerPort);

    }
        break;

    case quint8(IM::CLIENT_REQUEST_INTEREST_GROUP_INFO):
    {
        qDebug()<<"~~CLIENT_REQUEST_INTEREST_GROUP_INFO";

        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;

        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);
        quint32 groupID = 0;
        stream >> groupID;

        sendUserInterestGroupInfoPacket(socketID, userInfo, groupID, peerAddress, peerPort);

    }
        break;

    case quint8(IM::CLIENT_REQUEST_INTEREST_GROUP_MEMBERS_INFO):
    {

        qDebug()<<"~~CLIENT_REQUEST_INTEREST_GROUP_MEMBERS_INFO";

        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;

        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);
        quint32 groupID = 0;
        stream >> groupID;

        sendUserInterestGroupMembersInfoPacket(socketID, userInfo, groupID, peerAddress, peerPort);

    }
        break;

    case quint8(IM::CLIENT_REQUEST_CREATE_INTEREST_GROUP):
    {

        qDebug()<<"~~CLIENT_REQUEST_CREATE_INTEREST_GROUP";

        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;

        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);

        QString groupName = 0;
        stream >> groupName;

        //TODO
        quint32 groupID = createNewInterestGroup(userInfo, groupName);
        sendCreateInterestGroupResultPacket(socketID, userInfo, groupID, groupName);


    }
        break;

    case quint8(IM::CLIENT_REQUEST_DISBAND_INTEREST_GROUP):
    {

        qDebug()<<"~~CLIENT_REQUEST_DISBAND_INTEREST_GROUP";

        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;

        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);

        quint32 groupID = 0;
        stream >> groupID;

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



    case quint8(IM::CLIENT_REQUEST_PERSONAL_MESSAGE_INFO):
    {
        qDebug()<<"~~CLIENT_REQUEST_PERSONAL_MESSAGE_INFO";

        QString userID = peerID;
        in >> userID ;

        UserInfo *userInfo = getUserInfo(userID);
        if(userInfo){return;}
        sendUserPersonalMessagePacket(socketID, userInfo, peerAddress, peerPort);

    }
        break;

    case quint8(IM::CLIENT_REQUEST_MODIFY_CONTACT_REMARK):
    {
        qDebug()<<"~~CLIENT_REQUEST_MODIFY_CONTACT_REMARK";

        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;

        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);
        QString contactID = "", remarkName = "", extraRemarkInfo = "";
        stream >> contactID >> remarkName >> extraRemarkInfo;

        //TODO:Save To DB!

    }
        break;

    case quint8(IM::CREATE_OR_DELETE_CONTACT_GROUP):
    {
        qDebug()<<"--CREATE_OR_DELETE_CONTACT_GROUP";

        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;

        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);
        quint32 groupID = 0;
        QString groupName = "";
        quint8 create = 1;
        stream >> groupID >> groupName >> create;

        bool ok = createOrDeleteContactGroupInDB(userInfo, &groupID, groupName, create);
        sendCreateOrDeleteContactGroupResultPacket(socketID, groupID, groupName, create, ok, userInfo->getPersonalContactGroupsVersion(), userInfo->getSessionEncryptionKey());

    }
        break;

    case quint8(IM::RENAME_CONTACT_GROUP):
    {
        qDebug()<<"~~RENAME_CONTACT_GROUP";

        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;

        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);
        quint32 groupID = 0;
        QString newGroupName = "";
        stream >> groupID >> newGroupName;

        updateContactGroupNameInDB(userInfo, groupID, newGroupName);

    }
        break;

    case quint8(IM::SESSION_ENCRYPTION_KEY_WITH_CONTACT):
    {
        qDebug()<<"--SESSION_ENCRYPTION_KEY_WITH_CONTACT";


        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;

        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);
        QString contactID = "";
        stream >> contactID;

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

    case quint8(IM::CHAT_MESSAGES_CACHED_ON_SERVER):
    {
        qDebug()<<"--CHAT_MESSAGES_CACHED_ON_SERVER";

        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;

        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);
        QString contactID = "", message = "", imageNames = "";
        stream >> contactID >> message >> imageNames;

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
        UserInfo *contactInfo = getUserInfo(contactID);
        if(!contactInfo){return;}
        if(contactInfo->getOnlineState() != IM::ONLINESTATE_OFFLINE){
            sendContactChatMessagePacket(contactInfo->getSocketID(), userID, message, contactInfo->getSessionEncryptionKey());
        }


    }
        break;

    case quint8(IM::GROUP_CHAT_MESSAGE_TO_SERVER):
    {
        qDebug()<<"--GROUP_CHAT_MESSAGE_TO_SERVER";

        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;

        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);
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


    }
        break;

    case quint8(IM::REQUEST_CHAT_IMAGE):
    {
        qDebug()<<"--REQUEST_CHAT_IMAGE";

        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;

        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);

        QString imageName = "", contactID = "";
        stream >> imageName >> contactID;

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


    }
        break;

    case quint8(IM::CHAT_IMAGE):
    {
        qDebug()<<"--CHAT_IMAGE";


        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;

        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);

        QString  imageName = "";
        QByteArray image;
        stream >> imageName >> image;

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
        break;


        //FILE TX
    case quint8(IM::CLIENT_REQUEST_FILE_SERVER_INFO):
    {
        qDebug()<<"--CLIENT_REQUEST_FILE_SERVER_INFO";


        QString userID = peerID;
        QByteArray encryptedData;
        in >> encryptedData;

        UserInfo *userInfo = getUserInfo(userID);
        if(!userInfo){return;}

        //解密数据
        QByteArray decryptedData;
        if(!decryptData(userID, &decryptedData, encryptedData)){return;}
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_7);

        QString  id = "";
        stream >> id;

        if(id != userID){
            qCritical()<<"ERROR! Invalid Request!";
            return;
        }

        //TODO
        sendFileServerInfoToUser(socketID, userInfo->getSessionEncryptionKey());


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

//void ServerPacketsParser::startCheckIMUsersOnlineStateTimer(){
//    qWarning()<<"--startCheckIMUsersOnlineStateTimer()";

//    if(!checkIMUsersOnlineStateTimer){
//        checkIMUsersOnlineStateTimer = new QTimer(this);
//        checkIMUsersOnlineStateTimer->setInterval(HEARTBEAT_TIMER_INTERVAL); //1 minute
//        checkIMUsersOnlineStateTimer->setSingleShot(false);
//        connect(checkIMUsersOnlineStateTimer, SIGNAL(timeout()), this, SLOT(slotCheckIMUsersOnlineStatus()));
//    }

//    //    if(!checkIMUsersOnlineStateTimer->isActive()){
//    //    }

//    checkIMUsersOnlineStateTimer->start();

//}

//void ServerPacketsParser::slotCheckIMUsersOnlineStatus(){
//    qWarning()<<"--slotCheckIMUsersOnlineStatus()";
//    QList<UserInfo*>  users = onlineUsers();
//    foreach(UserInfo *info, users){
//        if(!info->isOnLine()){
//            processUserOnlineStatusChanged(info, IM::ONLINESTATE_OFFLINE, "", 0);
//        }
//    }

//}


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
