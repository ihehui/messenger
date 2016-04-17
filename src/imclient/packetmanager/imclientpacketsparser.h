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


#include "HHSharedCore/hcryptography.h"
#include "HHSharedNetwork/hnetworkutilities.h"



namespace HEHUI {


class IMClientPacketsParser : public QObject{
    Q_OBJECT
public:
    IMClientPacketsParser(ClientResourcesManager *resourcesManager, QObject *parent = 0);
    virtual ~IMClientPacketsParser();



    //    void startHeartbeat(int interval = HEARTBEAT_TIMER_INTERVAL);
    //    void stopHeartbeat();

public slots:
    void parseIncomingPacketData(PacketBase *packet);


    bool sendClientLookForServerPacket(const QString &targetAddress = QString(IM_SERVER_IPMC_ADDRESS), quint16 targetPort = IM_SERVER_IPMC_LISTENING_PORT){
        qDebug()<<"----sendClientLookForServerPacket(...) targetAddress:"<<targetAddress;

        QHostAddress address = QHostAddress(targetAddress);
        if(address.isNull()){
            address = QHostAddress(QString(IM_SERVER_IPMC_ADDRESS));
        }
        quint16 port = targetPort;
        if(!port){
            port = IM_SERVER_IPMC_LISTENING_PORT;
        }

        ServerDiscoveryPacket p;
        p.responseFromServer = 0;
        p.version = QString(APP_VERSION);
        //packet.udpPort = m_udpServer->localPort();

        QByteArray ba = p.toByteArray();
        if(ba.isEmpty()){return false;}

        return m_udpServer->sendDatagram(ba, address, port );
    }

    bool requestDataForward(int socketID, const QString &receiverID, const QByteArray &data){
        qDebug()<<"--requestDataForward(...) receiverID:"<<receiverID;

        DataForwardPacket p(sessionEncryptionKey);
        p.data = data;
        p.receiver = receiverID;

        QByteArray ba = p.toByteArray();
        if(ba.isEmpty()){return false;}

        return m_rtp->sendReliableData(socketID, &ba);
    }


    bool sendClientLookForServerPacket(const QHostAddress &targetAddress, quint16 targetPort){
        //qDebug()<<"----sendClientLookForServerPacket(...)";

        packet->setPacketType(quint8(IM::CMD_ServerDiscovery));
        packet->setTransmissionProtocol(TP_UDP);
        //packet->setRemainingRetransmissionTimes(int(PACKET_RETRANSMISSION_TIMES*3));
        QByteArray ba;


        out << m_myUserID << QString(APP_VERSION);


        return m_udpServer->sendDatagram(ba, targetAddress, targetPort);

    }


    bool requestRegistrationServerInfo(int serverSocketID, const QString &clientVersion = APP_VERSION){
        qWarning()<<"--requestRegistrationServerInfo(...)";

        RgeistrationPacket packet;
        packet.InfoType = RgeistrationPacket::REGISTRATION_SERVER_INFO;
        packet.ServerInfo.requestServerInfo = 1;
        packet.ServerInfo.version = clientVersion;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool registration(int serverSocketID, const QString &userID, const QString &password){
        qWarning()<<"--registration(...)";

        RgeistrationPacket packet;
        packet.InfoType = RgeistrationPacket::REGISTRATION_INFO;
        packet.RgeistrationInfo.userID = userID;
        packet.RgeistrationInfo.password = password;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool requestUpdatePassword(int serverSocketID){
        qDebug()<<"--requestUpdatePassword(...)";

        UpdatePasswordPacket packet;
        packet.InfoType = UpdatePasswordPacket::INFO_TYPE_INIT_REQUEST;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool requestCaptcha(int serverSocketID){
        qDebug()<<"--requestCaptcha(...)";

        UpdatePasswordPacket packet;
        packet.InfoType = UpdatePasswordPacket::INFO_TYPE_CAPTCHA_REQUEST;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool updatePassword(int serverSocketID, const QString &captcha, const QString &userID, const QString &oldPassword, const QString &newPassword, const QString &securityAnswer, const QString &email, const QString &smsCaptcha){
        qDebug()<<"--updatePassword(...)";
        
        UpdatePasswordPacket packet;
        packet.InfoType = UpdatePasswordPacket::INFO_TYPE_AUTH_INFO_FROM_CLIENT;
        packet.AuthInfo.captcha = captcha;
        packet.AuthInfo.userID = userID;
        packet.AuthInfo.oldPassword = oldPassword;
        packet.AuthInfo.newPassword = newPassword;
        packet.AuthInfo.securityAnswer = securityAnswer;
        packet.AuthInfo.email = email;
        packet.AuthInfo.smsCaptcha = smsCaptcha;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool requestLogin(int serverSocketID, const QString &clientVersion = APP_VERSION){
        qDebug()<<"--requestLogin(...)";
        
        m_myUserID = myself->getUserID();
        sessionEncryptionKey = cryptography->SHA1(myself->getPassword().toUtf8());
        Q_ASSERT_X(!m_myUserID.isEmpty(), "requestLogin", "Invalid User ID!");
        if(m_myUserID.isEmpty()){
            qCritical()<<"Invalid User ID!";
            return false;
        }
        qWarning()<<"My User ID:"<<m_myUserID;


        LoginPacket packet;
        packet.InfoType = LoginPacket::INFO_TYPE_LOGIN_SERVER_INFO;
        packet.LoginServerInfo.version = APP_VERSION;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool changeMyOnlineState(int serverSocketID, quint8 onlineStateCode){
        qDebug()<<"--changeMyOnlineState(...)";

        OnlineStateChangedPacket packet(sessionEncryptionKey);
        packet.stateCode = onlineStateCode;
        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool logout(int serverSocketID){
        qDebug()<<"--logout(...)";
        return changeMyOnlineState(serverSocketID, IM::ONLINESTATE_OFFLINE);
    }

    bool requestInterestGroupsList(int serverSocketID){
        qDebug()<<"------------------requestInterestGroupsList(...)";

        InterestGroupsInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = InterestGroupsInfoPacket::PIT_GROUPS_LIST;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool requestInterestGroupInfo(int serverSocketID, quint32 groupID){

        InterestGroupsInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = InterestGroupsInfoPacket::PIT_GROUP_INFO;
        packet.GroupInfo.id = groupID;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool requestCreateInterestGroup(int serverSocketID, const QString &groupName, quint8 groupType){
        qDebug()<<"----requestCreateInterestGroup(...)";

        InterestGroupsInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = InterestGroupsInfoPacket::PIT_GROUP_CREATION;
        packet.GroupCreationInfo.name = groupName;
        packet.GroupCreationInfo.type = groupType;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool requestDisbandInterestGroup(int serverSocketID, quint32 groupID){
        qDebug()<<"----requestDisbandInterestGroup(...)";
        InterestGroupsInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = InterestGroupsInfoPacket::PIT_GROUP_DELETION;
        packet.GroupDeletionInfo.id = groupID;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool joinOrQuitInterestGroup(int serverSocketID, quint32 groupID, bool join, const QString &verificationMessage = ""){
        qWarning()<<"--joinOrQuitInterestGroup(...)";
        InterestGroupsInfoPacket packet(sessionEncryptionKey);
        packet.GroupID = groupID;

        if(join){
            packet.InfoType = InterestGroupsInfoPacket::PIT_GROUP_MEMBER_APPLICATION;
            packet.MemberApplicationInfo.userID = m_myUserID;
            packet.MemberApplicationInfo.message = verificationMessage;
        }else{
            packet.InfoType = InterestGroupsInfoPacket::PIT_GROUP_MEMBER_DELETION;
            packet.MemberDeletionInfo.userID = m_myUserID;
        }

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool requestPersonalContactGroupsInfo(int serverSocketID){
        //qWarning()<<"--requestPersonalContactGroupsInfo(...)";
        ContactGroupsInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = ContactGroupsInfoPacket::PIT_GROUPS_LIST;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool requestContactInfo(int serverSocketID, const QString &contactID, bool summaryInfo = true){
        qDebug()<<"--requestContactInfo(...)";
        ContactInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = ContactInfoPacket::PIT_CONTACT_INFO;
        packet.ContactID = contactID;
        packet.info.isSummaryInfo = quint8(summaryInfo?1:0);

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool addContact(int serverSocketID, const QString &contactID, const QString &verificationMessage = "", quint32 groupID = ContactGroupBase::Group_Friends_ID){
        qWarning()<<"--addContact(...)";
        ContactInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = ContactInfoPacket::PIT_FRIENDING_REQUEST;
        packet.ContactID = contactID;
        packet.ContactFriendingRequest.message = verificationMessage;
        packet.ContactFriendingRequest.groupID = groupID;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool responseAddContactRequestFromUser(int serverSocketID, const QString &userID, quint8 errorCode, const QString &extraMessage = ""){
        qDebug()<<"--responseAddContactRequestFromUser(...)";
        ContactInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = ContactInfoPacket::PIT_FRIENDING_RESULT;
        packet.ContactID = userID;
        packet.ContactFriendingResult.errorCode = errorCode;
        packet.ContactFriendingResult.message = extraMessage;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool deleteContact(int serverSocketID, const QString &contactID, bool deleteMeFromOpposition = false, bool blockForever = false){
        qDebug()<<"--deleteContact(...)";
        ContactInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = ContactInfoPacket::PIT_CONTACT_DELETION;
        packet.ContactID = contactID;
        packet.ContactDeletionInfo.deleteMeFromOpposition = deleteMeFromOpposition;
        packet.ContactDeletionInfo.blockForever = blockForever;
        //packet.ContactDeletionInfo.errorCode = errorCode;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool moveContactToGroup(int serverSocketID, const QString &contactID, quint32 oldGroupID, quint32 newGroupID){
        qDebug()<<"--moveContactToGroup(...)";
        ContactInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = ContactInfoPacket::PIT_GROUP_CHANGE;
        packet.ContactID = contactID;
        packet.ContactChangeGroup.oldGroupID = oldGroupID;
        packet.ContactChangeGroup.newGroupID = newGroupID;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool updateContactRemarkInfo(int serverSocketID, quint32 contactID, const QString &remarkName, const QString &extraRemarkInfo){
        qDebug()<<"--updateContactRemarkInfo(...)";
        ContactInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = ContactInfoPacket::PIT_CONTACT_REMARK;
        packet.ContactID = contactID;
        packet.ContactRemarkInfo.newRemarkName = remarkName;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool createContactGroup(int serverSocketID, quint32 parentGroupID, const QString &groupName, bool create = true){
        qDebug()<<"--createContactGroup(...)";
        ContactGroupsInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = ContactGroupsInfoPacket::PIT_GROUP_CREATION;
        packet.GroupCreationInfo.name = groupName;
        packet.GroupCreationInfo.parentID = parentGroupID;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool deleteContactGroup(int serverSocketID, quint32 groupID){
        qDebug()<<"--deleteContactGroup(...)";
        ContactGroupsInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = ContactGroupsInfoPacket::PIT_GROUP_DELETION;
        packet.GroupDeletionInfo.groupID = groupID;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool renameContactGroup(int serverSocketID, quint32 groupID, const QString &newGroupName){
        qDebug()<<"--renameContactGroup(...)";
        ContactGroupsInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = ContactGroupsInfoPacket::PIT_GROUP_RENAMING;
        packet.GroupRenamingInfo.groupID = groupID;
        packet.GroupRenamingInfo.newName = newGroupName;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool searchContact(int serverSocketID, const QString &keyword, quint8 searchOnlineUsersOnly,
                       quint8 searchWebcamUsersOnly, quint16 location, quint16 hometown,
                       quint8 gender, quint8 age, bool matchExactly, int startIndex
                       ){
        qDebug()<<"--searchContact(...)";
        SearchInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = SearchInfoPacket::PIT_SEARCH_CONTACT_CONDITIONS;
        packet.SearchContactConditions.keyword = keyword;
        packet.SearchContactConditions.isOnline = searchOnlineUsersOnly;
        packet.SearchContactConditions.hasWebcam = searchWebcamUsersOnly;
        packet.SearchContactConditions.location = location;
        packet.SearchContactConditions.hometown = hometown;
        packet.SearchContactConditions.gender = gender;
        packet.SearchContactConditions.age = age;
        packet.SearchContactConditions.matchExactly = matchExactly;
        packet.SearchContactConditions.startIndex = startIndex;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool searchInterestGroup(int serverSocketID, const QString &keyword, int startIndex){
        qDebug()<<"--searchInterestGroup(...)";
        SearchInfoPacket packet(sessionEncryptionKey);
        packet.InfoType = SearchInfoPacket::PIT_SEARCH_INTEREST_GROUP_CONDITIONS;
        packet.SearchInterestGroupConditions.keyword = keyword;
        packet.SearchInterestGroupConditions.startIndex = startIndex;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool sendChatMessageToContact(int peerSocketID, const QString &contactID, const QString &message, const QStringList &imageNameList){
        qDebug()<<"--sendChatMessageToContact(...)";
        ChatMessagePacket packet(sessionEncryptionKey);
        packet.InfoType = ChatMessagePacket::PIT_CONTACT_CHAT_MESSAGE;
        packet.ContactChatMessage.contactID = contactID;
        packet.ContactChatMessage.message = message;
        packet.ContactChatMessage.imageNames = imageNameList.join(",");;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool sendInterestGroupChatMessageToServer(int serverSocketID, quint32 interestGroupID ,const QString &message, const QStringList &imageNameList){
        qDebug()<<"--sendInterestGroupChatMessageToServer(...)";
        ChatMessagePacket packet(sessionEncryptionKey);
        packet.InfoType = ChatMessagePacket::PIT_GROUP_CHAT_MESSAGE;
        packet.GroupChatMessage.groupID = interestGroupID;
        packet.GroupChatMessage.message = message;
        packet.GroupChatMessage.imageNames = imageNameList.join(",");;

        return m_rtp->sendReliableData(serverSocketID, &packet.toByteArray());
    }

    bool sendImageToContact(int peerSocketID, const QString &imageName, const QByteArray &image, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendImageToContact(...)";
        ChatMessagePacket packet(sessionEncryptionKey);
        packet.InfoType = ChatMessagePacket::PIT_CHAT_IMAGE;
        packet.ChatImage.isRequest = 0;
        packet.ChatImage.name = imageName;
        packet.ChatImage.image = image;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }

    bool requestDownloadImageFromContact(int peerSocketID, const QString &imageName, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--requestImageFromContact(...)";
        ChatMessagePacket packet(sessionEncryptionKey);
        packet.InfoType = ChatMessagePacket::PIT_CHAT_IMAGE;
        packet.ChatImage.isRequest = 1;
        packet.ChatImage.name = imageName;

        return m_rtp->sendReliableData(peerSocketID, &packet.toByteArray());
    }









    bool requestPersonalInfo(int serverSocketID){
        qDebug()<<"--requestPersonalInfo(...)";

        return requestContactInfo(serverSocketID, m_myUserID);

    }




    bool requestSessionEncryptionKeyWithContact(int serverSocketID, quint32 contactID){
        qDebug()<<"--requestSessionEncryptionKeyWithContact(...)";
        

        packet->setPacketType(quint8(IM::SESSION_ENCRYPTION_KEY_WITH_CONTACT));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;


        out << contactID;

        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKey, true);


        out << m_myUserID << encryptedData;

        return m_rtp->sendReliableData(serverSocketID, &ba);

    }













    bool requestSendFileToContact(int peerSocketID, const QString &contactID, const QStringList &filePathList){
        //TODO:
        return true;
    }

    bool sendChatMessageToContacts(const QStringList &contacts, const QString &message, const QStringList &imagePathList){
        //TODO:
        return true;
    }


    
    //FILE TX
    ///////////////////////////////////////////////
    bool requestFileServerInfo(int serverSocketID){
        qDebug()<<"--requestFileServerInfo(...)";



        packet->setPacketType(quint8(IM::CLIENT_REQUEST_FILE_SERVER_INFO));

        QByteArray ba;


        out << m_myUserID;


        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKey, true);


        out << m_myUserID << encryptedData;








        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool requestUploadFile(IMUserBase *contact, const QByteArray &fileMD5Sum, const QString &fileName, quint64 size){
        qDebug()<<"--requestUploadFile(...) Contact ID"<<contact->getUserID()<<" fileName:"<<fileName;
        QString contactID = contact->getUserID();



        packet->setPacketType(quint8(IM::REQUEST_UPLOAD_FILE));

        QByteArray ba;


        out << fileMD5Sum << fileName << size;


        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKeyWithContactHash.value(contactID), true);


        out << m_myUserID << encryptedData;








        if(!contact->isConnected()){
            return requestDataForward(m_socketConnectedToServer, contactID, ba, sessionEncryptionKey);
        }

        return m_rtp->sendReliableData(contact->getSocketID(), &ba);
    }


    bool cancelUploadFileRequest(IMUserBase *contact, const QByteArray &fileMD5Sum){
        QString contactID = contact->getUserID();



        packet->setPacketType(quint8(IM::CANCEL_FILE_UPLOAD_REQUEST));

        QByteArray ba;


        out << fileMD5Sum;


        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKeyWithContactHash.value(contactID), true);


        out << m_myUserID << encryptedData;








        if(!contact->isConnected()){
            return requestDataForward(m_socketConnectedToServer, contactID, ba, sessionEncryptionKey);
        }

        return m_rtp->sendReliableData(contact->getSocketID(), &ba);
    }


    bool requestDownloadFile(IMUserBase *contact, const QString &remoteBaseDir, const QString &remoteFileName){
        QString contactID = contact->getUserID();



        packet->setPacketType(quint8(IM::REQUEST_DOWNLOAD_FILE));

        QByteArray ba;


        out << remoteBaseDir << remoteFileName;


        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKeyWithContactHash.value(contactID), true);


        out << m_myUserID << encryptedData;


        if(!contact->isConnected()){
            return requestDataForward(m_socketConnectedToServer, contactID, ba, sessionEncryptionKey);
        }

        return m_rtp->sendReliableData(contact->getSocketID(), &ba);
    }

    bool cancelDownloadFileRequest(IMUserBase *contact, const QString &remoteFileName){
        QString contactID = contact->getUserID();



        packet->setPacketType(quint8(IM::CANCEL_FILE_DOWNLOAD_REQUEST));

        QByteArray ba;


        out << remoteFileName;


        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKeyWithContactHash.value(contactID), true);


        out << m_myUserID << encryptedData;


        if(!contact->isConnected()){
            return requestDataForward(m_socketConnectedToServer, contactID, ba, sessionEncryptionKey);
        }

        return m_rtp->sendReliableData(contact->getSocketID(), &ba);
    }


    bool acceptFileDownloadRequest(IMUserBase *contact, const QString &fileName, const QByteArray &fileMD5Sum, quint64 size){
        QString contactID = contact->getUserID();



        packet->setPacketType(quint8(IM::RESPONSE_FILE_DOWNLOAD_REQUEST));

        QByteArray ba;


        out << fileName << true << fileMD5Sum << size;


        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKeyWithContactHash.value(contactID), true);


        out << m_myUserID << encryptedData;

        if(!contact->isConnected()){
            return requestDataForward(m_socketConnectedToServer, contactID, ba, sessionEncryptionKey);
        }

        return m_rtp->sendReliableData(contact->getSocketID(), &ba);
    }

    bool denyFileDownloadRequest(IMUserBase *contact, const QString &fileName, const QString &message){
        QString contactID = contact->getUserID();



        packet->setPacketType(quint8(IM::RESPONSE_FILE_DOWNLOAD_REQUEST));

        QByteArray ba;


        out << fileName << false << message;


        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKeyWithContactHash.value(contactID), true);


        out << m_myUserID << encryptedData;

        if(!contact->isConnected()){
            return requestDataForward(m_socketConnectedToServer, contactID, ba, sessionEncryptionKey);
        }

        return m_rtp->sendReliableData(contact->getSocketID(), &ba);
    }

    bool responseFileUploadRequest(IMUserBase *contact, const QByteArray &fileMD5Sum, bool accepted, const QString &message){
        QString contactID = contact->getUserID();



        packet->setPacketType(quint8(IM::RESPONSE_FILE_UPLOAD_REQUEST));

        QByteArray ba;


        out << fileMD5Sum << accepted << message;


        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKeyWithContactHash.value(contactID), true);


        out << m_myUserID << encryptedData;


        if(!contact->isConnected()){
            return requestDataForward(m_socketConnectedToServer, contactID, ba, sessionEncryptionKey);
        }

        return m_rtp->sendReliableData(contact->getSocketID(), &ba);
    }




private slots:


    //    int crypto(QByteArray *destination, const QByteArray &source, bool encrypt);

    ///////////////////////////////////////////////
    //quint16 login(const QString &serverAddress, quint16 serverPort){
    bool login(int serverSocketID){
        //qWarning()<<"--login() "<<m_serverAddress.toString()<<":"<<m_serverUDPListeningPort;

        //TODO:Device Info
        QString deviceInfo = "PC";

        //TODO:密码保存方式
        sessionEncryptionKey = myself->encryptedPassword();
        QByteArray encryptedPassword;
        cryptography->teaCrypto(&encryptedPassword, QByteArray::fromBase64(myself->getPassword().toUtf8()), sessionEncryptionKey, true);
        
        LoginPacket packet(sessionEncryptionKey);
        packet.InfoType = LoginPacket::INFO_TYPE_AUTH_INFO_FROM_CLIENT;
        packet.AuthInfo.password = encryptedPassword;
        packet.AuthInfo.stateAfterLoggedin = quint8(myself->getStateAfterLoggedin());
        packet.AuthInfo.deviceInfo = deviceInfo;

        return m_rtp->sendReliableData(serverSocketID, &ba);
    }





signals:
    void  signalHeartbeatPacketReceived(const QString &contactID);
    void  signalConfirmationOfReceiptPacketReceived(quint16 packetSerialNumber1, quint16 packetSerialNumber2);

    void signalRegistrationPacketReceived(const RgeistrationPacket &packet);
    void signalUpdatePasswordResultReceived(const UpdatePasswordPacket &packet);
    void signalContactGroupsInfoPacketReceived(const ContactGroupsInfoPacket &packet);
    void signalInterestGroupsInfoPacketReceived(const InterestGroupsInfoPacket &packet);
    void signalContactInfoPacketReceived(const ContactInfoPacket &packet);
    void signalSearchContactsResultPacketReceived(const SearchInfoPacket &packet);
    void signalChatMessageReceivedFromContact(const ChatMessagePacket &packet);







    void signalLoginServerRedirected(const QString &serverAddress, quint16 serverPort, const QString &serverName);
    void signalLoginResultReceived(quint8 errorType, const QString &errorMessage = "");
    void signalClientLastLoginInfoPacketReceived(const QString &extIPAddress, const QString &loginTime, const QString &LogoutTime, const QString &deviceInfo);
    //void signalContactStateChangedPacketReceived(const QString &contactID, IM::OnlineState onlineState, const QString &contactHostAddress, quint16 contactHostPort);
    void signalContactStateChangedPacketReceived(quint8 onlineState, const QString &contactID, const QString &contactHostAddress, quint16 contactHostPort);
    void signalContactsOnlineInfoPacketReceived(const QString &contactsOnlineInfoString);


    void signalCreateOrDeleteContactGroupResultPacketReceived(quint32 groupID, const QString &groupName, bool createGroup, bool result);


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

    void signalServerDeclarePacketReceived(const ServerDiscoveryPacket &packet);
    void signalServerAnnouncementPacketReceived(const QString &announcement, bool mustRead = true);


    ///////////////////////////
    //File TX
    void signalContactRequestUploadFile(const QString &contactID, const QByteArray &fileMD5Sum, const QString &fileName, quint64 size);
    void signalContactRequestDownloadFile(const QString &contactID, const QString &localBaseDir, const QString &fileName);

    void signalFileDownloadRequestAccepted(const QString &contactID, const QString &remoteFileName, const QByteArray &fileMD5Sum, quint64 size);
    void signalFileDownloadRequestDenied(const QString &contactID, const QString &remoteFileName, const QString &message);
    void signalFileUploadRequestResponsed(const QString &contactID, const QByteArray &fileMD5Sum, bool accepted, const QString &message);



public:
    QStringList runningNICAddresses();

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
    UDTProtocol *m_udtProtocol;
    TCPServer *m_tcpServer;



};

}

#endif /* IMCLIENTPACKETSPARSER_H_ */
