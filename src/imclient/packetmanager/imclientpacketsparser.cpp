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

    if(cryptography){
        delete cryptography;
        cryptography = 0;
    }

}




void IMClientPacketsParser::parseIncomingPacketData(PacketBase *packet){
    //    qDebug()<<"----IMClientPacketsParser::parseIncomingPacketData(Packet *packet)";

    //QByteArray packetBody = packet.getPacketBody();
    quint8 packetType = packet.getPacketType();
    QString peerID = packet.getPeerID();

    QHostAddress peerAddress = packet.getPeerHostAddress();
    quint16 peerPort = packet.getPeerHostPort();
    SOCKETID socketID = packet.getSocketID();


    switch(packetType){

    case quint8(IM::CMD_ServerDiscovery):
    {

        ServerDiscoveryPacket p(packet, QByteArray());
        emit signalServerDeclarePacketReceived(p);

        qWarning()<<"~~ServerDeclare"<<" serverAddress:"<<peerAddress.toString()<<" servername:"<<peerID <<" serverRTPListeningPort:"<<port;
    }
        break;

    case quint8(IM::CMD_DataForward):
    {
        qDebug()<<"~~ForwardedDataByServer";

        DataForwardPacket p(packet, sessionEncryptionKey);

        PacketBase packet2;
        if(packet2.fromByteArray(p.data)){
            packet2.setSocketID(socketID);
            parseIncomingPacketData(packet2);
        }else{
            qWarning()<<"ERROR! Can not convert data to Packet!";
        }

    }
        break;

    case quint8(IM::CMD_Announcement):
    {
        AnnouncementPacket p(packet, QByteArray());
        emit signalServerAnnouncementPacketReceived(p.AnnouncementInfo.announcementID, (mustRead == quint8(0))?false:true);
        qDebug()<<"~~ServerAnnouncement"<<"groupName:"<<groupName<<"computerName:"<<computerName<<"announcement:"<<announcement<<"mustRead:"<<mustRead;
    }
        break;

    case quint8(IM::CMD_Rgeistration):
    {
        RgeistrationPacket p(packet, sessionEncryptionKey);
        emit signalRegistrationPacketReceived(p);
        qDebug()<<"~~CMD_Rgeistration";
    }
        break;

    case quint8(IM::CMD_UpdatePassword):
    {
        UpdatePasswordPacket p(packet, sessionEncryptionKey);
        emit signalUpdatePasswordResultReceived(p);
        qDebug()<<"--CMD_UpdatePassword";
    }
        break;

    case quint8(IM::CMD_Login):
    {
        LoginPacket p(packet, sessionEncryptionKey);
        processLoginPacket(p);
        qDebug()<<"--CMD_Login";
    }
        break;

    case quint8(IM::CMD_OnlineStateChanged):
    {
        OnlineStateChangedPacket p(packet, sessionEncryptionKey);
        quint8 stateCode = p.stateCode;
        QString contactID = p.contactID;
        QString contactIP = p.contactIP;
        quint16 contactPort = p.contactPort;

        emit signalContactStateChangedPacketReceived(stateCode, contactID, contactIP, contactPort);
        qDebug()<<"--CMD_OnlineStateChanged";
    }
        break;

    case quint8(IM::CMD_OnlineContacts):
    {
        OnlineContacts p(packet, sessionEncryptionKey);
        emit signalContactsOnlineInfoPacketReceived(p.contactsOnlineInfoString);
        qDebug()<<"--CMD_OnlineContacts";
    }
        break;

    case quint8(IM::CMD_ContactGroupsInfo):
    {
        ContactGroupsInfoPacket p(packet, sessionEncryptionKey);
        emit signalContactGroupsInfoPacketReceived(p);

        qDebug()<<"--CMD_ContactGroupsInfo";
    }
        break;

    case quint8(IM::CMD_InterestGroupsInfo):
    {
        InterestGroupsInfoPacket p(packet, sessionEncryptionKey);
        emit signalInterestGroupsInfoPacketReceived(p);
    }
        break;

    case quint8(IM::CMD_ContactInfo):
    {
        ContactInfoPacket p(packet, sessionEncryptionKey);
        emit signalContactInfoPacketReceived(p);
        qDebug()<<"~~CMD_ContactInfo";

    }
        break;

    case quint8(IM::CMD_SearchInfo):
    {
        SearchInfoPacket p(packet, sessionEncryptionKey);
        emit signalSearchContactsResultPacketReceived(p);
        qDebug()<<"~~CMD_SearchInfo";
    }
        break;

    case quint8(IM::CMD_ChatMessage):
    {

//        bool fromContact = myself->hasContact(peerID);
//        if(fromContact){
//            //From contact
//        }else{
//            //From server
//        }


        if(peerID == m_serverName){
            //FROM SERVER
            ChatMessagePacket p(packet, sessionEncryptionKey);
            emit signalChatMessageReceivedFromContact(p);

            qDebug()<<"Chat message from server!";
        }else{
            //FROM CONTACT
            ChatMessagePacket p(packet, sessionEncryptionKeyWithContactHash.value(peerID));
            emit signalChatMessageReceivedFromContact(p);

            qDebug()<<"Chat message from contact!";
        }

        //emit signalChatMessageReceivedFromContact(p);
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

void IMClientPacketsParser::processLoginPacket(const LoginPacket &packet){

    LoginPacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case LoginPacket::INFO_TYPE_LOGIN_SERVER_INFO:
    {
        QString serverAddress = packet.LoginServerInfo.serverAddress;
        quint16 serverPort = packet.LoginServerInfo.serverPort;
        if( serverPort == 0 || ((serverAddress == peerAddress.toString()) && (serverPort == peerPort)) ){
            if(!login(socketID)){
                emit signalLoginResultReceived(quint8(IM::ERROR_UnKnownError));
            }
        }else{
            emit signalLoginServerRedirected(serverAddress, serverPort, peerID);
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
    }
        break;

    case LoginPacket::INFO_TYPE_LOGIN_RESULT:
    {
        quint8 loginResultCode = packet.AuthResultInfo.loggedin;
        quint8 errorTypeCode = packet.AuthResultInfo.errorType;
        QString errorMessage = "";

        if(loginResultCode){

            m_serverName = peerID;
            m_socketConnectedToServer = socketID;

            uint serverTime = packet.AuthResultInfo.serverTime;
            Q_ASSERT(serverTime);
            ServerTime::instance()->startSync(serverTime);

            myself->setSessionEncryptionKey(packet.AuthResultInfo.sessionEncryptionKey);
            //TODO:
            myself->loadMyInfoFromLocalDatabase();


            quint32 personalSummaryInfoVersionOnServer = packet.AuthResultInfo.personalSummaryInfoVersionOnServer;
            quint32 personalDetailInfoVersionOnServer = packet.AuthResultInfo.personalDetailInfoVersionOnServer;
            quint32 personalContactGroupsInfoVersionOnServer = packet.AuthResultInfo.personalContactGroupsInfoVersionOnServer;
            quint32 interestGroupsInfoVersionOnServer = packet.AuthResultInfo.interestGroupsInfoVersionOnServer;
            quint32 personalMessageInfoVersionOnServer = packet.AuthResultInfo.personalMessageInfoVersionOnServer;
            if(personalSummaryInfoVersionOnServer != myself->getPersonalSummaryInfoVersion()){requestContactInfo(socketID, m_myUserID, true);}
            if(personalDetailInfoVersionOnServer != myself->getPersonalDetailInfoVersion()){requestContactInfo(socketID, m_myUserID, false);}
//            if(personalContactGroupsInfoVersionOnServer != user->getPersonalContactGroupsVersion()){requestPersonalContactGroupsInfo(socketID);}
            if(interestGroupsInfoVersionOnServer != myself->getInterestGroupInfoVersion()){requestInterestGroupsList(socketID);}
            if(personalMessageInfoVersionOnServer != myself->getPersonalMessageInfoVersion()){requestPersonalMessage(socketID, m_myUserID);}

        }

        emit signalLoginResultReceived(errorTypeCode, errorMessage);

    }
        break;

    case LoginPacket::INFO_TYPE_PREVIOUS_LOGIN_INFO:
    {
        QString extIPAddress = packet.PreviousLoginInfo.loginIP;
        QDateTime loginTime = QDateTime::fromTime_t(packet.PreviousLoginInfo.loginTime);
        QDateTime LogoutTime = QDateTime::fromTime_t(packet.PreviousLoginInfo.logoutTime);
        QString deviceInfo = packet.PreviousLoginInfo.deviceInfo;

        myself->setLastLoginExternalHostAddress(extIPAddress);
        myself->setLastLoginTime(loginTime);
        myself->setLastLogoutTime(LogoutTime);
        myself->setLastLoginDeviceInfo(deviceInfo);
        //TODO
        emit signalClientLastLoginInfoPacketReceived(extIPAddress, loginTime.toString(), LogoutTime.toString(), deviceInfo);
    }
        break;

    default:
        break;
    }

}














} //namespace HEHUI
