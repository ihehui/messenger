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
    quint8 packetType = packet->getPacketType();
    QString peerID = packet->getPeerID();

    QHostAddress peerAddress = packet->getPeerHostAddress();
    quint16 peerPort = packet->getPeerHostPort();
    SOCKETID socketID = packet->getSocketID();


    switch(packetType){

    case quint8(IM::CMD_ServerDiscovery):
    {
        qDebug()<<"~~CMD_ServerDiscovery";

        ServerDiscoveryPacket p(*packet, QByteArray());
        emit signalServerDeclarePacketReceived(p);

        qWarning()<<"***Server Found***:"<<" serverAddress:"<<peerAddress.toString()<<" servername:"<<peerID <<" serverRTPListeningPort:"<<peerPort;
    }
        break;

    case quint8(IM::CMD_DataForward):
    {
        qDebug()<<"~~CMD_DataForward";

        DataForwardPacket p(*packet, sessionEncryptionKey);

        PacketBase packet2;
        if(packet2.fromByteArray(&p.data)){
            packet2.setSocketID(socketID);
            parseIncomingPacketData(&packet2);
        }else{
            qWarning()<<"ERROR! Can not convert data to Packet!";
        }

    }
        break;

    case quint8(IM::CMD_Announcement):
    {
        AnnouncementPacket p(*packet, QByteArray());
        emit signalServerAnnouncementPacketReceived(p);
        qDebug()<<"~~CMD_Announcement";
    }
        break;

    case quint8(IM::CMD_Rgeistration):
    {
        RgeistrationPacket p(*packet, sessionEncryptionKey);
        emit signalRegistrationPacketReceived(p);
        qDebug()<<"~~CMD_Rgeistration";
    }
        break;

    case quint8(IM::CMD_UpdatePassword):
    {
        UpdatePasswordPacket p(*packet, sessionEncryptionKey);
        emit signalUpdatePasswordResultReceived(p);
        qDebug()<<"--CMD_UpdatePassword";
    }
        break;

    case quint8(IM::CMD_Login):
    {
        LoginPacket p(*packet, sessionEncryptionKey);
        processLoginPacket(p);
        qDebug()<<"--CMD_Login";
    }
        break;

    case quint8(IM::CMD_OnlineStateChanged):
    {
        OnlineStateChangedPacket p(*packet, sessionEncryptionKey);
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
        OnlineContacts p(*packet, sessionEncryptionKey);
        emit signalContactsOnlineInfoPacketReceived(p.contactsOnlineInfoString);
        qDebug()<<"--CMD_OnlineContacts";
    }
        break;

    case quint8(IM::CMD_ContactGroupsInfo):
    {
        ContactGroupsInfoPacket p(*packet, sessionEncryptionKey);
        emit signalContactGroupsInfoPacketReceived(p);

        qDebug()<<"--CMD_ContactGroupsInfo";
    }
        break;

    case quint8(IM::CMD_InterestGroupsInfo):
    {
        InterestGroupsInfoPacket p(*packet, sessionEncryptionKey);
        emit signalInterestGroupsInfoPacketReceived(p);
    }
        break;

    case quint8(IM::CMD_ContactInfo):
    {
        ContactInfoPacket p(*packet, sessionEncryptionKey);
        emit signalContactInfoPacketReceived(p);
        qDebug()<<"~~CMD_ContactInfo";

    }
        break;

    case quint8(IM::CMD_SearchInfo):
    {
        SearchInfoPacket p(*packet, sessionEncryptionKey);
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
            ChatMessagePacket p(*packet, sessionEncryptionKey);
            emit signalChatMessageReceivedFromContact(p);
            qDebug()<<"Chat message from server!";
        }else{
            //FROM CONTACT
            ChatMessagePacket p(*packet, sessionEncryptionKeyWithContactHash.value(peerID));
            emit signalChatMessageReceivedFromContact(p);
            qDebug()<<"Chat message from contact!";
        }
    }
        break;

    case quint8(IM::CMD_Captcha):
    {
        CaptchaInfoPacket p(*packet, sessionEncryptionKey);
        emit signalCaptchaInfoPacketReceived(p);
        qDebug()<<"~~CMD_Captcha";
    }
        break;

        //File TX
    case quint8(IM::CMD_FileTransfer):
    {
        FileTransferPacket p(*packet, sessionEncryptionKey);
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

void IMClientPacketsParser::setSessionEncryptionKeyWithContact(const QString &contactID, const QByteArray &key){
    sessionEncryptionKeyWithContactHash[contactID] = key;
}


void IMClientPacketsParser::processLoginPacket(const LoginPacket &packet){

    LoginPacket::PacketInfoType infoType = packet.InfoType;
    switch (infoType) {
    case LoginPacket::INFO_TYPE_LOGIN_SERVER_INFO:
    {
        QString serverAddress = packet.LoginServerInfo.serverAddress;
        quint16 serverPort = packet.LoginServerInfo.serverPort;
        if( serverPort == 0 || ((serverAddress == packet.getPeerHostAddress().toString()) && (serverPort == packet.getPeerHostPort())) ){
            if(!login(packet.getSocketID())){
                emit signalLoginResultReceived(quint8(IM::ERROR_UnKnownError));
            }
        }else{
            emit signalLoginServerRedirected(serverAddress, serverPort, packet.getPeerID());
        }
    }
        break;

    case LoginPacket::INFO_TYPE_AUTH_INFO_FROM_CLIENT:
    {
    }
        break;

    case LoginPacket::INFO_TYPE_LOGIN_RESULT:
    {
        quint8 loggedin = packet.AuthResultInfo.loggedin;
        quint8 errorTypeCode = packet.AuthResultInfo.errorType;
        QString errorMessage = "";

        if(loggedin){
            m_serverName = packet.getPeerID();
            m_socketConnectedToServer = packet.getSocketID();

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

            if(personalSummaryInfoVersionOnServer != myself->getPersonalSummaryInfoVersion()){requestContactInfo(m_socketConnectedToServer, m_myUserID, true);}
            if(personalDetailInfoVersionOnServer != myself->getPersonalDetailInfoVersion()){requestContactInfo(m_socketConnectedToServer, m_myUserID, false);}
//            if(personalContactGroupsInfoVersionOnServer != user->getPersonalContactGroupsVersion()){requestPersonalContactGroupsInfo(socketID);}
            if(interestGroupsInfoVersionOnServer != myself->getInterestGroupInfoVersion()){requestInterestGroupsList(m_socketConnectedToServer);}
//            if(personalMessageInfoVersionOnServer != myself->getPersonalMessageInfoVersion()){requestPersonalMessage(m_socketConnectedToServer, m_myUserID);}

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
