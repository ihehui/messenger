/*
 ****************************************************************************
 * imclientpacketsparser.cpp
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

#include "imclientpacketsparser.h"


#include <QNetworkInterface>


#include "../servertime/servertime.h"



namespace HEHUI {


IMClientPacketsParser::IMClientPacketsParser(ClientResourcesManager *resourcesManager, QObject *parent)
    :QObject(parent)
{


    Q_ASSERT(resourcesManager);

    m_udpServer = resourcesManager->getUDPServer();
    Q_ASSERT_X(m_udpServer, "IMClientPacketsParser::IMClientPacketsParser(...)", "Invalid UDPServer!");
    connect(m_udpServer, SIGNAL(signalNewUDPPacketReceived(Packet*)), this, SLOT(parseIncomingPacketData(Packet*)), Qt::QueuedConnection);

    m_rtp = resourcesManager->getRTP();
    Q_ASSERT(m_rtp);

    m_udtProtocol = m_rtp->getUDTProtocol();
    Q_ASSERT(m_udtProtocol);
    m_udtProtocol->startWaitingForIOInOneThread(100);
    //m_udtProtocol->startWaitingForIOInSeparateThread(100, 1000);
    connect(m_udtProtocol, SIGNAL(packetReceived(Packet*)), this, SLOT(parseIncomingPacketData(Packet*)), Qt::QueuedConnection);

    m_tcpServer = m_rtp->getTCPServer();
    Q_ASSERT(m_tcpServer);
    connect(m_tcpServer, SIGNAL(packetReceived(Packet*)), this, SLOT(parseIncomingPacketData(Packet*)), Qt::QueuedConnection);



    //    heartbeatTimer = 0;
    //    processWaitingForReplyPacketsTimer = 0;

    //packetHandlerBase = new PacketHandlerBase(this);
    //m_packetHandlerBase = packetHandlerBase;


    //    m_networkManager = ClientNetworkManager::instance();
    //    connect(networkManager, SIGNAL(signalNewPacketReceived(Packet *)), this, SLOT(slotNewPacketReceived(Packet *)));
    //    processingPackets = false;

    //    localIPMCListeningAddress = NetworkManager::instance()->localIPMCListeningAddress();
    //    localIPMCListeningPort = NetworkManager::instance()->localIPMCListeningPort();

    


    //serverLastOnlineTime = QDateTime();

    myself = IMUser::instance();
    m_myUserID = myself->getUserID();
    m_serverName = "";
    m_socketConnectedToServer = INVALID_SOCK_ID;

    cryptography = new Cryptography();
    sessionEncryptionKey = QByteArray();

    

}

IMClientPacketsParser::~IMClientPacketsParser() {
    // TODO Auto-generated destructor stub

    qDebug()<<"IMClientPacketsParser::~IMClientPacketsParser() ";

    QMutexLocker locker(&mutex);

    //    if(heartbeatTimer){
    //        qDebug()<<"----------------------------33333333";
    //        heartbeatTimer->stop();
    //        qDebug()<<"----------------------------44444444";
    //    }
    //    delete heartbeatTimer;
    //    heartbeatTimer = 0;


    //    if(processWaitingForReplyPacketsTimer){
    //        processWaitingForReplyPacketsTimer->stop();
    //    }
    //    delete processWaitingForReplyPacketsTimer;
    //    processWaitingForReplyPacketsTimer = 0;


    if(cryptography){
        delete cryptography;
        cryptography = 0;
    }


    //networkManager = 0;

}




void IMClientPacketsParser::parseIncomingPacketData(Packet *packet){
    //    qDebug()<<"----IMClientPacketsParser::parseIncomingPacketData(Packet *packet)";

    //        if((packet->getTransmissionProtocol() == TP_UDP)
    //            && (networkManager->isLocalAddress(packet->getPeerHostAddress()))
    //            && (packet->getPeerHostPort() == localIPMCListeningPort)){
    //            qDebug()<<"~~Packet is been discarded!";
    //            return;
    //        }else if((packet->getTransmissionProtocol() == TP_TCP)
    //            && (packet->getPeerHostAddress() == networkManager->localTCPListeningAddress())
    //            && (packet->getPeerHostPort() == networkManager->localTCPListeningPort())){
    //            qDebug()<<"~~Packet is been discarded!";
    //            return;
    //        }

    //    qDebug()<<"~~networkManager->localAddress():"<<networkManager->localTCPListeningAddress().toString();
    //    qDebug()<<"~~packet->getPeerHostAddress():"<<packet->getPeerHostAddress();

    QByteArray packetData = packet->getPacketData();
    QDataStream in(&packetData, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_8);

    QString peerID = "";
    in >> peerID;

    QHostAddress peerAddress = packet->getPeerHostAddress();
    quint16 peerPort = packet->getPeerHostPort();

    quint8 packetType = packet->getPacketType();
    int socketID = packet->getSocketID();


    switch(packetType){
    //    case quint8(HEHUI::HeartbeatPacket):
    //    {
    //        QString contactID;
    //        in >> contactID;
    //        emit signalHeartbeatPacketReceived(contactID);
    //        qDebug()<<"~~HeartbeatPacket--"<<" contactID:"<<contactID;
    //    }
    //    break;
    //    case quint8(HEHUI::ConfirmationOfReceiptPacket):
    //    {
    //        quint16 packetSerialNumber1 = 0, packetSerialNumber2 = 0;
    //        in >> packetSerialNumber1 >> packetSerialNumber2;
    //        m_packetHandlerBase->removeWaitingForReplyPacket(packetSerialNumber1, packetSerialNumber2);
    //        emit signalConfirmationOfReceiptPacketReceived(packetSerialNumber1, packetSerialNumber2);
    //        //qDebug()<<"~~ConfirmationOfReceiptPacket:"<<packetSerialNumber;
    //    }
    //    break;

    case quint8(IM::ForwardedDataByServer):
    {
        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QByteArray data;
        stream >> data;

        QDataStream ds(&data, QIODevice::ReadOnly);
        ds.setVersion(QDataStream::Qt_4_8);
        QVariant v;
        ds >> v;
        if (v.canConvert<Packet>()){
            *packet = v.value<Packet>();
            //packet->setTransmissionProtocol(TP_UDT);
            packet->setSocketID(socketID);

            //packet->setPeerHostAddress(QHostAddress(address));
            //packet->setPeerHostPort(port);

            parseIncomingPacketData(packet);
        }


        qDebug()<<"~~ForwardedDataByServer";
    }
        break;



    case quint8(IM::ServerDeclare):
    {

        quint16 port = 0;
        QString version;
        in >> port >> version;

        //serverLastOnlineTime = QDateTime::currentDateTime();

        emit signalServerDeclarePacketReceived(peerAddress.toString(), port, peerID, version);

        qWarning()<<"~~ServerDeclare"<<" serverAddress:"<<peerAddress.toString()<<" servername:"<<peerID <<" serverRTPListeningPort:"<<port;
    }
        break;
    case quint8(IM::ServerOnline):
    {
        quint16 port;
        in >> port;

        //serverLastOnlineTime = QDateTime::currentDateTime();

        emit signalServerOnlinePacketReceived(peerAddress.toString(), port, peerID);
        qDebug()<<"~~ServerOnline";
    }
        break;
    case quint8(IM::ServerOffline):
    {
        quint16 port;
        in >> port;

        //        stopHeartbeat();

        emit signalServerOfflinePacketReceived(peerAddress.toString(), port, peerID);
        qWarning()<<"~~ServerOffline";
    }
        break;
    case quint8(IM::ServerAnnouncement):
    {

        QString groupName, computerName, announcement;
        quint8 mustRead;
        in >> announcement >> mustRead;
        emit signalServerAnnouncementPacketReceived(announcement, (mustRead == quint8(0))?false:true);
        qDebug()<<"~~ServerAnnouncement"<<"groupName:"<<groupName<<"computerName:"<<computerName<<"announcement:"<<announcement<<"mustRead:"<<mustRead;
    }
        break;

    case quint8(IM::SERVER_RESPONSE_CLIENT_REQUEST_REGISTRATION_SERVER_INFO):
    {

        qWarning()<<"--SERVER_RESPONSE_CLIENT_REQUEST_REGISTRATION_SERVER_INFO";

        quint8 errorTypeCode = quint8(IM::ERROR_UnKnownError);
        quint8 canRegister = 1;
        QString extraMessage = "";
        quint8 regMode = quint8(IM::RM_UserDefineAll);
        QString regServerAddress = "";
        quint8 requireActivation = 0;
        in >> errorTypeCode >> extraMessage >> canRegister  >> regMode >> regServerAddress >> requireActivation;
        emit signalRegistrationServerInfoReceived(errorTypeCode, canRegister, extraMessage, regMode, regServerAddress, requireActivation);

    }
        break;

    case quint8(IM::SERVER_RESPONSE_CLIENT_REGISTRATION):
    {
        
        qWarning()<<"--SERVER_RESPONSE_CLIENT_REQUEST_REGISTRATION";
        
        quint8 errorTypeCode = quint8(IM::ERROR_UnKnownError);
        quint32 sysID = 0;
        QString message = "";
        in >> errorTypeCode >> sysID >> message;
        emit signalRegistrationResultReceived(errorTypeCode, sysID, message);
        
    }
        break;

    case quint8(IM::SERVER_RESPONSE_CLIENT_REQUEST_UPDATE_PASSWORD):
    {
        
        qWarning()<<"--SERVER_RESPONSE_CLIENT_REQUEST_UPDATE_PASSWORD";
        

        quint8 errorTypeCode = quint8(IM::ERROR_UnKnownError);
        QString message = "";
        in >> errorTypeCode >> message;
        emit signalUpdatePasswordResultReceived(errorTypeCode, message);
        
    }
        break;

    case quint8(IM::SERVER_RESPONSE_CLIENT_REQUEST_LOGIN):
    {

        quint8 requestLoginResultCode = 0;
        in >> requestLoginResultCode;
        if(requestLoginResultCode){
            QString serverAddress = "";
            quint16 serverPort = 0;
            in >> serverAddress >> serverPort;
            if( serverPort == 0 || ((serverAddress == peerAddress.toString()) && (serverPort == peerPort)) ){
                if(!login(socketID)){
                   emit signalLoginResultReceived(quint8(IM::ERROR_UnKnownError));
                }
            }else{
                emit signalLoginServerRedirected(serverAddress, serverPort, peerID);
            }
        }else{
            quint8 errorTypeCode = quint8(IM::ERROR_UnKnownError);
            in >> errorTypeCode;
            emit signalLoginResultReceived(errorTypeCode);

        }

        qDebug()<<"--SERVER_RESPONSE_CLIENT_REQUEST_LOGIN";
    }
        break;

    case quint8(IM::SERVER_RESPONSE_CLIENT_LOGIN_REDIRECTION):
    {

        QString serverAddress = "";
        quint16 serverPort = 0;
        in >> serverAddress >> serverPort;

        emit signalLoginServerRedirected(serverAddress, serverPort, peerID);

    }
        break;

    case quint8(IM::SERVER_RESPONSE_CLIENT_LOGIN_RESULT):
    {

        quint8 loginResultCode = 10;
        in >> loginResultCode;
        quint8 errorTypeCode = quint8(IM::ERROR_UnKnownError);
        QString errorMessage = "";

        if(loginResultCode){

            m_serverName = peerID;
            m_socketConnectedToServer = socketID;

            QByteArray encryptedData;
            in >> encryptedData ;

            QByteArray decryptedData;
            cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
            //TODO
            QDataStream stream(&decryptedData, QIODevice::ReadOnly);
            stream.setVersion(QDataStream::Qt_4_8);

            quint32 personalSummaryInfoVersionOnServer = 1, personalDetailInfoVersionOnServer = 1, personalContactGroupsInfoVersionOnServer = 1, interestGroupsInfoVersionOnServer = 1, personalMessageInfoVersionOnServer = 1;
            uint serverTime = 0;
            stream >> sessionEncryptionKey >> personalSummaryInfoVersionOnServer >> personalDetailInfoVersionOnServer >> personalContactGroupsInfoVersionOnServer >> interestGroupsInfoVersionOnServer >> personalMessageInfoVersionOnServer >> serverTime;

            Q_ASSERT(serverTime);
            ServerTime::instance()->startSync(serverTime);

            myself->setSessionEncryptionKey(sessionEncryptionKey);

            errorTypeCode = quint8(IM::ERROR_NoError);
            errorMessage = "";

            //TODO:
            myself->loadMyInfoFromLocalDatabase();

            if(personalSummaryInfoVersionOnServer != myself->getPersonalSummaryInfoVersion()){requestContactInfo(socketID, m_myUserID, true);}
            if(personalDetailInfoVersionOnServer != myself->getPersonalDetailInfoVersion()){requestContactInfo(socketID, m_myUserID, false);}
//            if(personalContactGroupsInfoVersionOnServer != user->getPersonalContactGroupsVersion()){requestPersonalContactGroupsInfo(socketID);}
            if(interestGroupsInfoVersionOnServer != myself->getInterestGroupInfoVersion()){requestInterestGroupsList(socketID);}
            if(personalMessageInfoVersionOnServer != myself->getPersonalMessageInfoVersion()){requestPersonalMessage(socketID, m_myUserID);}

        }else{
            in >> errorTypeCode >> errorMessage;
        }

        emit signalLoginResultReceived(errorTypeCode, errorMessage);


        qDebug()<<"--SERVER_RESPONSE_CLIENT_LOGIN_RESULT";

    }
        break;

    case quint8(IM::CLIENT_LAST_LOGIN_INFO):
    {

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);

        QString extIPAddress = "", loginTime = "", LogoutTime = "", deviceInfo = "";
        stream >> extIPAddress >> loginTime >> LogoutTime >> deviceInfo;
        myself->setLastLoginExternalHostAddress(extIPAddress);
        myself->setLastLoginTime(QDateTime::fromString(loginTime));
        myself->setLastLogoutTime(QDateTime::fromString(LogoutTime));
        myself->setLastLoginDeviceInfo(deviceInfo);
        //TODO

        emit signalClientLastLoginInfoPacketReceived(extIPAddress, loginTime, LogoutTime, deviceInfo);

        qDebug()<<"--CLIENT_LAST_LOGIN_INFO";
    }
        break;

    case quint8(IM::ONLINE_STATE_CHANGED):
    {

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QString contactID = "", contactHostAddress = "";
        quint16 contactHostPort = 0;
        quint8 stateCode = quint8(IM::ONLINESTATE_OFFLINE);
        stream >> contactID >> stateCode >> contactHostAddress >> contactHostPort;

        //emit signalContactStateChangedPacketReceived(contactID, IM::OnlineState(stateCode), contactHostAddress, contactHostPort);
        emit signalContactStateChangedPacketReceived(contactID, stateCode, contactHostAddress, contactHostPort);

        qWarning()<<"--ONLINE_STATE_CHANGED";
    }
        break;

    case quint8(IM::CONTACTS_ONLINE_INFO):
    {
        qWarning()<<"--CONTACTS_ONLINE_INFO";


        QByteArray encryptedData;
        in >> encryptedData;


        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QString contactsOnlineInfoString = "";
        stream >> contactsOnlineInfoString;



        emit signalContactsOnlineInfoPacketReceived(contactsOnlineInfoString);

    }
        break;


    case quint8(IM::SERVER_RESPONSE_USER_INFO):
    {
        qDebug()<<"--SERVER_RESPONSE_USER_SUMMARY_INFO";
        

        //TODO:
        QString userID = "";
        QByteArray encryptedData;
        in >> userID >> encryptedData;


        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QString userInfo = "";
        quint8 isSummaryInfo = 1;
        stream >> userInfo >> isSummaryInfo;
        
        if(userID == myself->getUserID()){
            myself->setPersonalInfoString(userInfo, isSummaryInfo);
            myself->saveMyInfoToLocalDatabase();
        }else{
            myself->setContactInfoString(userID, userInfo, isSummaryInfo);
            myself->saveContactInfoToLocalDatabase(userID);
        }

        emit signalUserInfoPacketReceived(userID);

    }
        break;

    case quint8(IM::CONTACT_GROUPS_INFO):
    {

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QString groupsInfo = "";
        quint32 personalContactGroupsInfoVersionOnServer = 1;
        stream >> groupsInfo >> personalContactGroupsInfoVersionOnServer;
        
        //TODO: Save the data

        emit signalContactGroupsInfoPacketReceived(groupsInfo, personalContactGroupsInfoVersionOnServer);
        
        qDebug()<<"--CONTACT_GROUPS_INFO";

    }
        break;

    case quint8(IM::CONTACTS_INFO_VERSION):
    {

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);

        QString contactsInfoVersionString = "";
        quint32 contactGroupsInfoVersionOnServer = 1;
        stream >> contactsInfoVersionString >> contactGroupsInfoVersionOnServer;

        emit signalContactsInfoVersionPacketReceived(contactsInfoVersionString, contactGroupsInfoVersionOnServer);

        qDebug()<<"--CONTACTS_INFO_VERSION";

    }
        break;

    case quint8(IM::SERVER_RESPONSE_CREATE_OR_DELETE_CONTACT_GROUP):
    {

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);

        quint32 groupID = 0;
        QString groupName = "";
        quint8 createGroup = 1, result = 0;
        quint32 contactGroupsVersionOnServer = 1;
        stream >> groupID >> groupName >> createGroup >> result >> contactGroupsVersionOnServer;

        emit signalCreateOrDeleteContactGroupResultPacketReceived(groupID, groupName, createGroup, result);

//        if(contactGroupsVersionOnServer != (user->getPersonalContactGroupsVersion() + 1) ){
//            requestPersonalContactGroupsInfo(socketID);
//        }

        qWarning()<<"--SERVER_RESPONSE_CREATE_OR_DELETE_CONTACT_GROUP";

    }
        break;


    case quint8(IM::SERVER_RESPONSE_SEARCH_CONTACTS):
    {
        //TODO:

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QString result = "";
        stream >> result;

        emit signalSearchContactsResultPacketReceived(result);

        qWarning()<<"--SERVER_RESPONSE_SEARCH_CONTACTS";
    }
        break;

    case quint8(IM::SERVER_RESPONSE_SEARCH_INTERESTGROUPS):
    {
        //TODO:

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QString result = "";
        stream >> result;

        emit signalSearchInterestGroupsResultPacketReceived(result);

        qWarning()<<"--SERVER_RESPONSE_SEARCH_INTERESTGROUPS";
    }
        break;

    case quint8(IM::USER_REQUEST_JOIN_INTERESTGROUP):
    {
        //TODO:

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);

        quint32 groupID = 0;
        QString verificationMessage = "", userID = "", nickName = "", face = "";
        stream >> groupID >> verificationMessage >> userID >> nickName >> face;

        emit signalUserRequestJoinInterestGroupsPacketReceived(groupID, verificationMessage, userID, nickName, face);

        qWarning()<<"--User_REQUEST_JOIN_INTERESTGROUP";
    }
        break;

    case quint8(IM::SERVER_RESPONSE_JOIN_OR_QUIT_INTERESTGROUP):
    {
        //TODO:

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);

        quint32 groupID = 0;
        QString memberID = "";
        quint8 join = 0;
        stream >> groupID >> memberID >> join;

        emit signalUserJoinOrQuitInterestGroupPacketReceived(groupID, memberID, join);

        qWarning()<<"--SERVER_RESPONSE_JOIN_OR_QUIT_INTERESTGROUP";
    }
        break;

    case quint8(IM::CLIENT_REQUEST_ADD_CONTACT):
    {

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QString userID = "", userNickName = "", userFace = "", verificationMessage = "";
        stream >> userID >> userNickName >> userFace >> verificationMessage;

        emit signalAddContactRequestFromUserPacketReceived(userID, userNickName, userFace, verificationMessage);
        
    }
        break;

    case quint8(IM::CLIENT_RESPONSE_ADD_CONTACT_REQUEST):
    {

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QString userID = "", userNickName = "", userFace = "", reasonMessage = "";
        quint8 errorTypeCode = quint8(IM::ERROR_UnKnownError);
        int contactGroupID = ContactGroupBase::Group_Strangers_ID;
        quint8 onlineStateCode = quint8(IM::ONLINESTATE_OFFLINE);

        stream >> userID >> userNickName >> userFace >> contactGroupID >> errorTypeCode >> reasonMessage >> onlineStateCode;

        emit signalAddContactResultPacketReceived(userID, userNickName, userFace, contactGroupID, errorTypeCode, reasonMessage, onlineStateCode);
        
    }
        break;

    case quint8(IM::SERVER_RESPONSE_DELETE_CONTACT):
    {

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QString contactID = "";
        quint8 contactDeleted = 0, addToBlacklist = 0;

        stream >> contactID >> contactDeleted >> addToBlacklist;

        emit signalDeleteContactResultPacketReceived(contactID, contactDeleted, addToBlacklist);

    }
        break;


    case quint8(IM::SERVER_RESPONSE_INTEREST_GROUPS_LIST):
    {
        qDebug()<<"--SERVER_RESPONSE_INTEREST_GROUPS_LIST";


        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QString interestGroupsListOnServerForUser = "";
        quint32 interestGroupsInfoVersionOnServer = 1;

        stream >> interestGroupsListOnServerForUser >> interestGroupsInfoVersionOnServer;

        emit signalInterestGroupsListPacketReceived(interestGroupsListOnServerForUser, interestGroupsInfoVersionOnServer);

    }
        break;

    case quint8(IM::SERVER_RESPONSE_INTEREST_GROUP_INFO):
    {

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        quint32 groupID = 0;
        QString interestGroupInfoStringFromServer = "";

        stream >> interestGroupInfoStringFromServer >> groupID;

        emit signalInterestGroupInfoPacketReceived(interestGroupInfoStringFromServer, groupID);

    }
        break;

    case quint8(IM::SERVER_RESPONSE_INTEREST_GROUP_MEMBERS_INFO):
    {
        qWarning()<<"----SERVER_RESPONSE_INTEREST_GROUP_MEMBERS_INFO";


        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QString interestGroupMembersInfoStringFromServer = "";
        quint32 interestGroupMembersInfoVersionOnServer = 0, groupID = 0;

        stream >> interestGroupMembersInfoStringFromServer >> interestGroupMembersInfoVersionOnServer >> groupID;

        emit signalInterestGroupMembersInfoPacketReceived(interestGroupMembersInfoStringFromServer, interestGroupMembersInfoVersionOnServer, groupID);

    }
        break;

    case quint8(IM::SERVER_RESPONSE_CREATE_INTEREST_GROUP):
    {
        qWarning()<<"----SERVER_RESPONSE_CREATE_INTEREST_GROUP";


        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);

        quint32 groupID = 0;
        QString groupName = "";
        stream >> groupID >> groupName ;

        emit signalCreateInterestGroupResultPacketReceived(groupID, groupName);

    }
        break;

    case quint8(IM::SERVER_RESPONSE_DISBAND_INTEREST_GROUP):
    {
        qWarning()<<"----SERVER_RESPONSE_DISBAND_INTEREST_GROUP";


        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);

        quint32 groupID = 0;
        quint8 result = 0;
        stream >> groupID >> result ;

        emit signalDisbandInterestGroupResultPacketReceived(groupID, result);

    }
        break;


    case quint8(IM::SERVER_RESPONSE_PERSONAL_MESSAGE_INFO):
    {

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QString userID = "", personalMessage = "";

        stream >> userID >> personalMessage;

        emit signalPersonalMessagePacketReceived(userID, personalMessage);

    }
        break;


    case quint8(IM::SESSION_ENCRYPTION_KEY_WITH_CONTACT):
    {

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QString contactID = "";
        QByteArray key;
        stream >> contactID >> key;

        sessionEncryptionKeyWithContactHash[contactID] = key;

    }
        break;

    case quint8(IM::CHAT_MESSAGE_FROM_CONTACT):
    {

        //TODO:

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        if(peerID == m_serverName){
            //FROM SERVER
            cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
            qDebug()<<"Chat message from server!";
        }else{
            //FROM CONTACT
            cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithContactHash.value(peerID), false);
            qDebug()<<"Chat message from contact!";
        }
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);

        QString contactID = "", message = "";
        stream  >> contactID >> message;

        emit signalChatMessageReceivedFromContact(contactID, message, ServerTime::instance()->timeString());

    }
        break;

    case quint8(IM::CHAT_MESSAGES_CACHED_ON_SERVER):
    {
        //TODO:

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QString messagesString = "";
        stream >> messagesString;

        emit signalChatMessageCachedOnServerReceived(messagesString.split(QString(UNIT_SEPARTOR)));

    }
        break;

    case quint8(IM::REQUEST_CHAT_IMAGE):
    {
        //TODO:

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        bool fromContact = myself->hasContact(peerID);
        if(fromContact){
            //From contact
            cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithContactHash.value(peerID), false);
        }else{
            //From server
            cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        }
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);


        QString imageName = "", contactID = "";
        stream >> imageName >> contactID;

        emit signalImageDownloadRequestReceived(contactID, imageName);
    }
        break;

    case quint8(IM::CHAT_IMAGE):
    {
        //TODO:


        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        bool fromContact = myself->hasContact(peerID);
        if(fromContact){
            //From contact
            cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithContactHash.value(peerID), false);
        }else{
            //From server
            cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        }
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);


        QString imageName = "";
        QByteArray image;

        stream >> imageName >> image;

        QString contactID = "";
        if(fromContact){
            contactID = peerID;
        }else{
            stream >> contactID;
        }


        emit signalImageDownloadResultReceived(contactID, imageName, image);
    }
        break;


    case quint8(IM::GROUP_CHAT_MESSAGES_CACHED_ON_SERVER):
    {
        //TODO:

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QString messagesString = "";
        stream >> messagesString;

        emit signalInterestGroupChatMessagesCachedOnServerReceived(messagesString.split(QString(UNIT_SEPARTOR)));

    }
        break;

    case quint8(IM::GROUP_CHAT_MESSAGE_FROM_SERVER):
    {

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        quint32 interestGroupID = 0;
        QString senderID = "", message = "";
        stream  >> interestGroupID >> senderID >> message;

        emit signalInterestGroupChatMessageReceivedFromContact(interestGroupID, senderID, message, ServerTime::instance()->timeString());

    }
        break;


        //File TX
    case quint8(IM::SERVER_RESPONSE_FILE_SERVER_INFO):
    {
        qDebug()<<"~~SERVER_RESPONSE_FILE_SERVER_INFO";

        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKey, false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        quint32 ipv4Address = 0;
        quint16 port = 0;
        stream >> ipv4Address >> port ;

        Q_ASSERT(port != 0);
        if(port == 0){
            qCritical()<<"ERROR! Invalid File Server Address Info!";
            return;
        }

        if(ipv4Address == 0){
            myself->setFileServerAddress(myself->getLoginServerAddress());
        }else{
            myself->setFileServerAddress(QHostAddress(ipv4Address).toString());
        }

        myself->setFileServerPort(port);

    }
        break;

    case quint8(IM::REQUEST_UPLOAD_FILE):
    {
        QByteArray encryptedData;
        QString contactID = peerID;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithContactHash.value(contactID), false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QByteArray fileMD5Sum;
        QString fileName = "";
        quint64 size = 0;
        stream >> fileMD5Sum >> fileName >> size ;

        emit signalContactRequestUploadFile(contactID, fileMD5Sum, fileName, size);

        qDebug()<<"~~REQUEST_UPLOAD_FILE";
    }
        break;

    case quint8(IM::REQUEST_DOWNLOAD_FILE):
    {
        QByteArray encryptedData;
        QString contactID = peerID;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithContactHash.value(contactID), false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QString localBaseDir, fileName;
        stream >> localBaseDir >> fileName;

        emit signalContactRequestDownloadFile(contactID, localBaseDir, fileName);

        qDebug()<<"~~RequestDownloadFile";
    }
        break;

    case quint8(IM::RESPONSE_FILE_DOWNLOAD_REQUEST):
    {
        QByteArray encryptedData;
        QString contactID = peerID;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithContactHash.value(contactID), false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QString fileName = "";
        bool accepted = false;
        stream >> fileName >> accepted;

        if(accepted){
            QByteArray fileMD5Sum;
            quint64 size;
            stream >> fileMD5Sum >> size;
            emit signalFileDownloadRequestAccepted(contactID, fileName, fileMD5Sum, size);
        }else{
            QString message = "";
            stream >> message;
            signalFileDownloadRequestDenied(contactID, fileName, message);
        }
        qDebug()<<"~~RESPONSE_FILE_DOWNLOAD_REQUEST";
    }
        break;
    case quint8(IM::RESPONSE_FILE_UPLOAD_REQUEST):
    {
        QByteArray encryptedData;
        QString contactID = peerID;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithContactHash.value(contactID), false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QByteArray fileMD5;
        bool accepted = false;
        QString message = "";

        in >> fileMD5 >> accepted >> message;

        emit signalFileUploadRequestResponsed(contactID, fileMD5, accepted, message);

        qDebug()<<"~~RESPONSE_FILE_UPLOAD_REQUEST";
    }
        break;



//    case quint8(IM::RequestFileData):
//    {
//        QByteArray encryptedData;
//        QString contactID = peerID;
//        in >> encryptedData;

//        QByteArray decryptedData;
//        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithContactHash.value(contactID), false);
//        //TODO
//        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
//        stream.setVersion(QDataStream::Qt_4_8);
//        QByteArray fileMD5;
//        int startPieceIndex = 0, endPieceIndex = 0;
//        in >> fileMD5 >> startPieceIndex >> endPieceIndex;

//        emit signalFileDataRequested(socketID, contactID, fileMD5, startPieceIndex, endPieceIndex);

//        qDebug()<<"~~RequestFileData";
//    }
//        break;
//    case quint8(IM::FileData):
//    {
//        QByteArray encryptedData;
//        QString contactID = peerID;
//        in >> encryptedData;

//        QByteArray decryptedData;
//        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithContactHash.value(contactID), false);
//        //TODO
//        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
//        stream.setVersion(QDataStream::Qt_4_8);
//        QByteArray fileMD5, data, sha1;;
//        int pieceIndex = 0;
//        stream >> fileMD5 >> pieceIndex >> data >>sha1;

//        //        if(data.size() != size || sha1 != QCryptographicHash::hash(data, QCryptographicHash::Sha1)){
//        //            qCritical()<<"ERROR! Data Verification Failed!";
//        //            requestFileData(socketID, offset, size);
//        //            return;
//        //        }

//        emit signalFileDataReceived(socketID, contactID, fileMD5, pieceIndex, data, sha1);

//        //qDebug()<<"~~FileData";
//    }
//        break;
//    case quint8(IM::FileTXStatusChanged):
//    {
//        QByteArray encryptedData;
//        QString contactID = peerID;
//        in >> encryptedData;

//        QByteArray decryptedData;
//        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithContactHash.value(contactID), false);
//        //TODO
//        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
//        stream.setVersion(QDataStream::Qt_4_8);
//        QByteArray fileMD5;
//        quint8 status;
//        stream >> fileMD5 >> status;

//        emit signalFileTXStatusChanged(socketID, contactID, fileMD5, status);

//        qDebug()<<"~~FileTXStatusChanged";
//    }
//        break;
//    case quint8(IM::FileTXError):
//    {
//        QByteArray encryptedData;
//        QString contactID = peerID;
//        in >> encryptedData;

//        QByteArray decryptedData;
//        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithContactHash.value(contactID), false);
//        //TODO
//        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
//        stream.setVersion(QDataStream::Qt_4_8);
//        QByteArray fileMD5;
//        quint8 errorCode;
//        QString message;
//        stream >> fileMD5 >> errorCode >> message;

//        emit signalFileTXError(socketID, contactID, fileMD5, errorCode, message);

//        qDebug()<<"~~FileTXStatusChanged";
//    }
//        break;




    default:
        qWarning()<<"Unknown Packet Type:"<<packetType
                 <<" From:"<<peerAddress.toString()
                <<" Port:"<<peerPort;
        break;

    }

    PacketHandlerBase::recylePacket(packet);


}


//void IMClientPacketsParser::startHeartbeat(int interval){
//    if(NULL == heartbeatTimer){
//        heartbeatTimer = new QTimer();
//        heartbeatTimer->setSingleShot(false);
//        heartbeatTimer->setInterval(interval);
//        connect(heartbeatTimer, SIGNAL(timeout()), this, SLOT(sendHeartbeatPacket()));

//    }else{
//        heartbeatTimer->stop();
//        heartbeatTimer->setInterval(interval);
//    }

//    heartbeatTimer->start();


//}

//void IMClientPacketsParser::stopHeartbeat(){
//    if(heartbeatTimer){
//        heartbeatTimer->stop();
//    }

//}

//int IMClientPacketsParser::crypto(QByteArray *destination, const QByteArray &source, bool encrypt){

//    return cryptography->teaCrypto(destination, source, sessionEncryptionKey, encrypt);

//}


//quint16 IMClientPacketsParser::getLastReceivedPacketSN(const QString &peerID){
//    quint16 lastpacketSN = 0;

//    QList< QPair<quint16 /*Packet Serial Number*/, QDateTime/*Received Time*/> > list = m_receivedPacketsHash.values(peerID);
//    if(list.isEmpty()){
//        return lastpacketSN;
//    }

//    QDateTime lastpacketTime(QDate(1970, 1, 1));
//    for(int i=0; i<list.size(); i++){
//        QPair<quint16, QDateTime> pair = list.at(i);
//        QDateTime time = pair.second;
//        if(time.addSecs(UDP_PACKET_WAITING_FOR_REPLY_TIMEOUT) < QDateTime::currentDateTime()){
//            m_receivedPacketsHash.remove(peerID, pair);
//        }else{
//            if(time > lastpacketTime){
//                lastpacketTime = time;
//                lastpacketSN = pair.first;
//            }
//        }
//    }
//    //    foreach ( QPair<quint16, QDateTime> pair, list) {
//    //        QDateTime time = pair.second;
//    //        if(time.addSecs(UDP_PACKET_WAITING_FOR_REPLY_TIMEOUT) < QDateTime::currentDateTime()){
//    //            m_receivedPacketsHash.remove(peerID, pair);
//    //            list.removeOne(pair);
//    //        }else{
//    //            if(time > lastpacketTime){
//    //                lastpacketTime = time;
//    //                lastpacketSN = pair.first;
//    //            }
//    //        }
//    //    }

//    //TODO:TX Rate

//    return lastpacketSN;


//}



QStringList IMClientPacketsParser::runningNICAddresses(){

    QStringList addresses;

    foreach (QNetworkInterface nic, QNetworkInterface::allInterfaces()) {
        if ( (nic.flags() & QNetworkInterface::IsRunning) && ( !(nic.flags() & QNetworkInterface::IsLoopBack)) ) {
            foreach (QNetworkAddressEntry entry, nic.addressEntries()) {
                //qDebug()<<"IP:"<<entry.ip()<<" Hardware Address:"<<interface.hardwareAddress()<<" Flags:"<<interface.flags();
                QHostAddress ip = entry.ip();
                if(ip.protocol() == QAbstractSocket::IPv6Protocol){continue;}
                if(ip.isLoopback()){continue;}

                addresses.append(ip.toString());
            }
        }
    }

    return addresses;

}

















} //namespace HEHUI
