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

#include "HHSharedCore/hcryptography.h"



namespace HEHUI {


class ServerPacketsParser : public QObject, public UsersManager{
    Q_OBJECT
public:
    ServerPacketsParser(ResourcesManagerInstance *resourcesManager, QObject *parent = 0);
    virtual ~ServerPacketsParser();








public slots:
    void parseIncomingPacketData(Packet *packet);


    //    void sendHeartbeatPacket(){
    //        qDebug()<<"----sendHeartbeatPacket()";

    //        if(serverTCPListeningAddress.isNull()){
    //            serverTCPListeningAddress = QHostAddress::Broadcast;
    //        }

    //        Packet *packet = new Packet(QString(IM_SERVER_IPMC_ADDRESS), quint16(IM_SERVER_RUDP_LISTENING_PORT), localIPMCListeningAddress.toString(), localIPMCListeningPort);
    //        packet->setPacketType(quint8(HEHUI::HeartbeatPacket));
    //        QByteArray ba;
    //        QDataStream out(&ba, QIODevice::WriteOnly);
    //        out.setVersion(QDataStream::Qt_4_8);
    //        out << m_serverName;
    //        packet->setPacketData(ba);
    //        m_packetHandlerBase->appendOutgoingPacket(packet);


    //    }


    //    void sendConfirmationOfReceiptPacket(const QHostAddress peerAddress, quint16 peerPort, quint16 packetSerialNumber, const QString &peerID){
    //        qDebug()<<"----sendConfirmationOfReceiptPacket(...)";

    //        //Packet *packet = packetHandlerBase->getPacket(peerAddress, peerPort, localIPMCListeningAddress, localIPMCListeningPort);
    //        Packet *packet = PacketHandlerBase::getPacket(peerAddress, peerPort, localUDPListeningAddress, localUDPListeningPort);

    //        packet->setPacketType(quint8(HEHUI::ConfirmationOfReceiptPacket));
    //        QByteArray ba;
    //        QDataStream out(&ba, QIODevice::WriteOnly);
    //        out.setVersion(QDataStream::Qt_4_8);
    //        out << m_serverName << packetSerialNumber << getLastReceivedPacketSN(peerID);
    //        packet->setPacketData(ba);
    //        m_packetHandlerBase->appendOutgoingPacket(packet);

    //    }

    bool forwardDataForward(int peerSocketID, const QByteArray &data, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--forwardDataForward(...)";

        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);

        packet->setPacketType(quint8(IM::ForwardedDataByServer));
        packet->setTransmissionProtocol(TP_UDT);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << data;

        QByteArray encryptedData;
        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKey, true);
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


    bool sendServerDeclarePacket(const QHostAddress peerAddress, quint16 peerPort){
        qWarning()<<"--sendServerDeclarePacket(...)"<<" Peer Address:"<<peerAddress.toString()<<":"<<peerPort;

        Packet *packet = PacketHandlerBase::getPacket();
        packet->setPacketType(quint8(IM::ServerDeclare));
        packet->setTransmissionProtocol(TP_UDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << m_serverName << m_localRTPListeningPort << QString(APP_VERSION);
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;
        return m_ipmcServer->sendDatagram(ba, peerAddress, peerPort);

    }

    bool sendServerOnlinePacket(const QHostAddress &targetAddress = QHostAddress(IM_SERVER_IPMC_ADDRESS), quint16 targetPort = quint16(IM_SERVER_IPMC_LISTENING_PORT)){
        qDebug()<<"----sendServerOnlinePacket(...)";

        Packet *packet = PacketHandlerBase::getPacket();

        packet->setPacketType(quint8(IM::ServerOnline));
        packet->setTransmissionProtocol(TP_UDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << m_serverName << m_localRTPListeningPort;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;
        return m_ipmcServer->sendDatagram(ba, targetAddress, targetPort);

    }

    bool sendServerOfflinePacket(const QHostAddress &targetAddress = QHostAddress(IM_SERVER_IPMC_ADDRESS), quint16 targetPort = quint16(IM_SERVER_IPMC_LISTENING_PORT)){
        qDebug()<<"----sendServerOfflinePacket(...)";

        Packet *packet = PacketHandlerBase::getPacket();
        packet->setPacketType(quint8(IM::ServerOffline));
        packet->setTransmissionProtocol(TP_UDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << m_serverName << m_localRTPListeningPort;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;
        return m_ipmcServer->sendDatagram(ba, targetAddress, targetPort);

    }

    bool sendServerAnnouncementPacket(const QHostAddress peerAddress, quint16 peerPort, const QString &announcement, bool mustRead = true){
        qDebug()<<"----sendServerAnnouncementPacket(...)";

        Packet *packet = PacketHandlerBase::getPacket();
        packet->setPacketType(quint8(IM::ServerAnnouncement));
        packet->setTransmissionProtocol(TP_UDP);
        //packet->setRemainingRetransmissionTimes(int(PACKET_RETRANSMISSION_TIMES));
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << m_serverName << announcement << (mustRead?quint8(1):quint8(0));
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;
        return m_ipmcServer->sendDatagram(ba, peerAddress, peerPort);

    }






    bool sendRegistrationServerInfoPacket(int peerSocketID, quint8 errorTypeCode, const QString &extraMessage, bool canRegister, IM::RegistrationMode regMode, const QString &regServerAddress, bool requireActivation){
        qDebug()<<"--sendRegistrationServerInfoPacket(...)";

        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_CLIENT_REQUEST_REGISTRATION_SERVER_INFO));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << m_serverName << errorTypeCode << extraMessage << quint8(canRegister?1:0) << quint8(regMode) << regServerAddress << quint8(requireActivation?1:0);
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;
        return m_rtp->sendReliableData(peerSocketID, &ba);

    }

    bool sendClientRegistrationResultPacket(int peerSocketID, quint8 errorTypeCode, quint32 sysID, const QString &message){
        qWarning()<<"--sendClientRegistrationResultPacket(...)";
        
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_CLIENT_REGISTRATION));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << m_serverName << errorTypeCode << sysID << message;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;
        return m_rtp->sendReliableData(peerSocketID, &ba);

    }
    
    bool sendClientUpdatePasswordResultPacket(int peerSocketID, quint8 errorTypeCode, const QString &message){
        qDebug()<<"--sendClientUpdatePasswordResultPacket(...)";
        
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_CLIENT_REQUEST_UPDATE_PASSWORD));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << m_serverName << errorTypeCode << message;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;
        return m_rtp->sendReliableData(peerSocketID, &ba);

    }

    bool sendClientCanNotLoginPacket(int peerSocketID, const QString &userID, quint8 errorTypeCode){
        qDebug()<<"--sendClientCanNotLoginPacket(...)";
        
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_CLIENT_REQUEST_LOGIN));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        //out << userID << quint8(0) << quint8(errorType);
        out << m_serverName << quint8(0) << errorTypeCode;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;
        return m_rtp->sendReliableData(peerSocketID, &ba);

    }

    bool sendClientCanLoginPacket(int peerSocketID, const QString &userID, const QHostAddress &serverAddress, quint16 serverPort){
        qDebug()<<"--sendClientCanLoginPacket(...)";

        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_CLIENT_REQUEST_LOGIN));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        //out << userID << quint8(1) << serverAddress << serverPort;
        out << m_serverName << quint8(1) << serverAddress.toString() << serverPort;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;
        return m_rtp->sendReliableData(peerSocketID, &ba);

    }

    bool sendClientLoginRedirectionPacket(int peerSocketID, const QString &serverAddress, quint16 serverPort, const QString &userHostAddress, quint16 userHostPort){
        qDebug()<<"--sendClientLoginRedirectionPacket(...)";
        
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_CLIENT_LOGIN_REDIRECTION));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << m_serverName << serverAddress << serverPort;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;
        return m_rtp->sendReliableData(peerSocketID, &ba);

    }

    bool sendClientLoginFailedPacket(int peerSocketID, const QString &userID, quint8 errorTypeCode, const QString &errorMessage = ""){
        qDebug()<<"--sendClientLoginFailedPacket(...)";
        
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_CLIENT_LOGIN_RESULT));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        //out << userID << quint8(0) << quint8(errorType);
        out << m_serverName << quint8(0) << errorTypeCode << errorMessage;

        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;
        return m_rtp->sendReliableData(peerSocketID, &ba);

    }

    bool sendClientLoginSucceededPacket(int peerSocketID, const QString &userID, const QByteArray &encryptedPassword, const QByteArray &sessionEncryptionKey, quint32 personalSummaryInfoVersion,
                                        quint32 personalDetailInfoVersionOnServer, quint32 personalContactGroupsInfoVersionOnServer,
                                        quint32 interestGroupInfoVersionOnServer, quint32 blacklistInfoVersionOnServer){

        qDebug()<<"--sendClientLoginSucceededPacket(...)";
        
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_CLIENT_LOGIN_RESULT));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << sessionEncryptionKey << personalSummaryInfoVersion << personalDetailInfoVersionOnServer << personalContactGroupsInfoVersionOnServer << interestGroupInfoVersionOnServer << blacklistInfoVersionOnServer;

        uint serverTime = QDateTime::currentDateTime().toUTC().toTime_t();
        out << serverTime;

        QByteArray encryptedData;
        crypto(&encryptedData, ba, encryptedPassword, true);
        ba.clear();
        out.device()->seek(0);

        out << m_serverName << quint8(1) << encryptedData ;
        //qWarning()<<"---encryptedData.size():"<<encryptedData.size() <<"--encryptedPassword:"<<encryptedPassword.toBase64();
        //qWarning()<<"---sessionEncryptionKey From Server:"<<sessionEncryptionKey.toBase64();

        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;
        return m_rtp->sendReliableData(peerSocketID, &ba);

    }

    bool sendClientLastLoginInfoPacket(int peerSocketID, const QByteArray &sessionEncryptionKey, const QString &extIPAddress, const QString &loginTime, const QString &LogoutTime, const QString &deviceInfo ){
        qDebug()<<"--sendClientLastLoginInfoPacket(...)";


        Packet *packet = PacketHandlerBase::getPacket();
        packet->setPacketType(quint8(IM::CLIENT_LAST_LOGIN_INFO));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << extIPAddress << loginTime << LogoutTime << deviceInfo;
        QByteArray encryptedData;
        crypto(&encryptedData, ba, sessionEncryptionKey, true);
        ba.clear();
        out.device()->seek(0);
        out << m_serverName  << encryptedData;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;
        return m_rtp->sendReliableData(peerSocketID, &ba);

    }

    bool sendUserInfoPacket(int peerSocketID, const QString &userID, const QString &userInfo, const QByteArray &sessionEncryptionKey, bool summaryInfo){
        qDebug()<<"--sendUserInfoPacket(...)";
        
        //TODO:用户信息的格式
        Packet *packet = PacketHandlerBase::getPacket();
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_USER_INFO));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << userInfo << quint8(summaryInfo?1:0);
        QByteArray encryptedData;
        crypto(&encryptedData, ba, sessionEncryptionKey, true);
        ba.clear();
        out.device()->seek(0);
        out << m_serverName << userID << encryptedData;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;
        return m_rtp->sendReliableData(peerSocketID, &ba);

    }

    bool sendPersonalContactGroupsInfoPacket(int peerSocketID, const QString &contactGroupsInfo, quint32 personalContactGroupsInfoVersionOnServer, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendPersonalContactGroupsInfoPacket(...)";
        
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::CONTACT_GROUPS_INFO));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << contactGroupsInfo << personalContactGroupsInfoVersionOnServer;
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

    bool sendCreateOrDeleteContactGroupResultPacket(int peerSocketID, quint32 groupID, const QString &groupName, bool createGroup, bool result, quint32 personalContactGroupsInfoVersionOnServer, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendCreateOrDeleteContactGroupResultPacket(...)";

        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_CREATE_OR_DELETE_CONTACT_GROUP));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << groupID << groupName << quint8(createGroup?1:0) << quint8(result?1:0) << personalContactGroupsInfoVersionOnServer;
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


//    bool sendMoveContactToGroupResultPacket(int peerSocketID, const QString &contactID, quint32 oldGroupID, quint32 newGroupID, bool result, quint32 personalContactGroupsInfoVersionOnServer, const QByteArray &sessionEncryptionKey){
//        qDebug()<<"--sendMoveContactToGroupResultPacket(...)";

//        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
//        packet->setPacketType(quint8(IM::SERVER_RESPONSE_MOVE_CONTACT));
//        packet->setTransmissionProtocol(TP_RUDP);
//        QByteArray ba;
//        QDataStream out(&ba, QIODevice::WriteOnly);
//        out.setVersion(QDataStream::Qt_4_8);

//        out << contactID << oldGroupID << newGroupID << quint8(result?1:0) << personalContactGroupsInfoVersionOnServer;
//        QByteArray encryptedData;
//        crypto(&encryptedData, ba, sessionEncryptionKey, true);
//        ba.clear();
//        out.device()->seek(0);
//        out << m_serverName << encryptedData;

//        packet->setPacketData(ba);

//        ba.clear();
//        out.device()->seek(0);
//        QVariant v;
//        v.setValue(*packet);
//        out << v;
//        return m_rtp->sendReliableData(peerSocketID, &ba);

//    }


    bool sendPersonalContactsInfoVersionPacket(int peerSocketID, const QString &contactsInfoVersion, quint32 contactGroupsInfoVersionOnServer, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendPersonalContactsInfoVersionPacket(...)";

        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::CONTACTS_INFO_VERSION));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << contactsInfoVersion << contactGroupsInfoVersionOnServer;
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

    bool sendContactOnlineStatusChangedPacket(int peerSocketID, const QString &contactID, quint8 onlineStateCode, const QByteArray &sessionEncryptionKey, const QString &contactHostAddress, quint16 contactHostPort, const QString &targetHostAddress, quint16 targetHostPort){
        qDebug()<<"--sendContactOnlineStatusChangedPacket(...)";
        
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::ONLINE_STATE_CHANGED));
        packet->setTransmissionProtocol(TP_RUDP);
        //packet->setRemainingRetransmissionTimes(int(PACKET_RETRANSMISSION_TIMES));
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << contactID << onlineStateCode << contactHostAddress << contactHostPort;
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
        if(contactsOnlineInfo.isEmpty()){
            return false;
        }

        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::CONTACTS_ONLINE_INFO));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << contactsOnlineInfo.join(QString(UNIT_SEPARTOR));
        QByteArray encryptedData;
        crypto(&encryptedData, ba, userInfo->getSessionEncryptionKey(), true);
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

    bool sendSearchContactResultPacket(int peerSocketID, const QString &usersString, const QByteArray &sessionEncryptionKey, const QHostAddress &targetHostAddress, quint16 targetHostPort){
        qDebug()<<"--sendSearchContactResultPacket(...)";
        
        //TODO:搜索结果的格式
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_SEARCH_CONTACTS));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << usersString;
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

    bool sendSearchInterestGroupResultPacket(int peerSocketID, const QString &groupsString, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendSearchInterestGroupResultPacket(...)";

        //TODO:搜索结果的格式
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_SEARCH_INTERESTGROUPS));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << groupsString;
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

    bool sendUserJoinInterestGroupRequestToAdminPacket(int peerSocketID, quint32 groupID, const QString &verificationMessage, UserInfo *userInfo){
        qDebug()<<"--sendJoinInterestGroupRequestFromUserPacket(...)";

        //TODO:搜索结果的格式
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::USER_REQUEST_JOIN_INTERESTGROUP));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << groupID << verificationMessage << userInfo->getUserID() << userInfo->getUserName() << userInfo->getFace();

        QByteArray encryptedData;
        crypto(&encryptedData, ba, userInfo->getSessionEncryptionKey(), true);
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


    bool sendUserJoinOrQuitInterestGroupResultToUserPacket(int peerSocketID, quint32 groupID, const QString &memberID, bool join, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendUserJoinOrQuitInterestGroupResultToUserPacket(...)";

        //TODO:搜索结果的格式
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_JOIN_OR_QUIT_INTERESTGROUP));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << groupID << memberID << quint8(join?1:0);

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



    bool sendAddContactRequestFromUserPacket(int peerSocketID, const QString &userID, const QString &userNickName, const QString &userFace, const QString &verificationMessage, const QByteArray &sessionEncryptionKey, const QHostAddress &targetHostAddress, quint16 targetHostPort){
        qDebug()<<"--sendAddContactRequestFromUserPacket(...)";
        
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::CLIENT_REQUEST_ADD_CONTACT));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << userID << userNickName << userFace << verificationMessage;

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

    bool sendAddContactResultPacket(int peerSocketID, const QString &contactID, const QString &contactNickName, const QString &contactFace, int contactGroupID, quint8 errorTypeCode, const QString &reasonMessage, quint8 onlineStateCode, const QByteArray &sessionEncryptionKey, const QString &targetHostAddress, quint16 targetHostPort){
        qDebug()<<"--sendAddContactResultPacket(...)";
        
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::CLIENT_RESPONSE_ADD_CONTACT_REQUEST));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << contactID << contactNickName << contactFace << contactGroupID << errorTypeCode << reasonMessage << onlineStateCode;

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

    bool sendDeleteContactResultPacket(int peerSocketID, const QString &contactID, bool contactDeleted, bool addToBlacklist, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendDeleteContactResultPacket(...)";

        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_DELETE_CONTACT));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << contactID << quint8(contactDeleted?1:0) << quint8(addToBlacklist?1:0);
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


    bool sendUserInterestGroupsListPacket(int peerSocketID, UserInfo *userInfo, const QHostAddress &targetHostAddress, quint16 targetHostPort){

        //TODO:用户信息的格式
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_INTEREST_GROUPS_LIST));
        packet->setTransmissionProtocol(TP_RUDP);
        //packet->setRemainingRetransmissionTimes(int(PACKET_RETRANSMISSION_TIMES));
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << getInterestGroupsListForUser(userInfo) << userInfo->getInterestGroupInfoVersion();
        QByteArray encryptedData;
        crypto(&encryptedData, ba, userInfo->getSessionEncryptionKey(), true);
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
    
    bool sendUserInterestGroupInfoPacket(int peerSocketID, UserInfo *userInfo, quint32 groupID, const QHostAddress &targetHostAddress, quint16 targetHostPort){

        //TODO:用户信息的格式
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_INTEREST_GROUP_INFO));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << getInterestGroupInfoStringForUser(userInfo, groupID) << groupID;

        QByteArray encryptedData;
        crypto(&encryptedData, ba, userInfo->getSessionEncryptionKey(), true);
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

    bool sendUserInterestGroupMembersInfoPacket(int peerSocketID, UserInfo *userInfo, quint32 groupID, const QHostAddress &targetHostAddress, quint16 targetHostPort){
        //qWarning()<<"----sendUserInterestGroupMembersInfoPacket(...)"<<" groupID"<<groupID;

        InterestGroup *group = getInterestGroup(groupID);
        if(!group){
            return false;
        }

        //TODO:用户信息的格式
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_INTEREST_GROUP_MEMBERS_INFO));
        packet->setTransmissionProtocol(TP_RUDP);
        //packet->setRemainingRetransmissionTimes(int(PACKET_RETRANSMISSION_TIMES));
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << getInterestGroupMembersInfoStringForUser(userInfo, group) << group->getGroupMemberListInfoVersion() << groupID;

        QByteArray encryptedData;
        crypto(&encryptedData, ba, userInfo->getSessionEncryptionKey(), true);
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
    
    bool sendCreateInterestGroupResultPacket(int peerSocketID, UserInfo *userInfo, quint32 groupID, const QString &groupName){
        qDebug()<<"--sendCreateInterestGroupResultPacket() "<<" groupID:"<<groupID<<" peerSocketID:"<<peerSocketID;

        //TODO:用户信息的格式
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_CREATE_INTEREST_GROUP));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << groupID << groupName ;
        QByteArray encryptedData;
        crypto(&encryptedData, ba, userInfo->getSessionEncryptionKey(), true);
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

    bool sendDisbandInterestGroupResultPacket(int peerSocketID, UserInfo *userInfo, quint32 groupID, bool result){
        qDebug()<<"--sendDisbandInterestGroupResultPacket() "<<" groupID:"<<groupID<<" peerSocketID:"<<peerSocketID;

        //TODO:用户信息的格式
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_DISBAND_INTEREST_GROUP));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << groupID << quint8(result?1:0) ;
        QByteArray encryptedData;
        crypto(&encryptedData, ba, userInfo->getSessionEncryptionKey(), true);
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


    bool sendUserPersonalMessagePacket(int peerSocketID, UserInfo *userInfo, const QHostAddress &targetHostAddress, quint16 targetHostPort){
        qDebug()<<"--sendUserPersonalMessagePacket(...)";

        //TODO:用户信息的格式
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_PERSONAL_MESSAGE_INFO));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << userInfo->getUserID() << userInfo->getPersonalMessage();
        QByteArray encryptedData;
        crypto(&encryptedData, ba, userInfo->getSessionEncryptionKey(), true);
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


    bool sendSessionEncryptionKeyWithContact(int peerSocketID, const QString &contactID, const QByteArray &encryptedPassword, const QByteArray &sessionEncryptionKeyWithContact, const QString &targetHostAddress, quint16 targetHostPort){
        qDebug()<<"--sendSessionEncryptionKeyWithContact(...)";
        
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SESSION_ENCRYPTION_KEY_WITH_CONTACT));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

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



    bool sendCachedChatMessagesPacket(int peerSocketID, const QStringList &messages, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendCachedChatMessagesPacket(...)";
        
        //TODO:缓存消息的格式
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::CHAT_MESSAGES_CACHED_ON_SERVER));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << messages.join(QString(UNIT_SEPARTOR));
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

    bool sendContactChatMessagePacket(int peerSocketID, const QString &senderID, const QString &message, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendContactChatMessagePacket(...)";

        //TODO:缓存消息的格式
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::CHAT_MESSAGE_FROM_CONTACT));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << senderID << message;

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


    bool sendRequestImagePacket(int peerSocketID, const QString &contactID, const QString &imageName, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendRequestImagePacket(...)";

        //TODO:缓存消息的格式
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::REQUEST_CHAT_IMAGE));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << imageName << contactID ;

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

    bool sendImagePacket(int peerSocketID, const QString &contactID, const QString &imageName, const QByteArray &image, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendImagePacket(...)";

        //TODO:缓存消息的格式
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::CHAT_IMAGE));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << imageName << image << contactID;

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

    bool sendCachedInterestGroupChatMessagesPacket(int peerSocketID, const QStringList &messages, const QByteArray &sessionEncryptionKey, const QHostAddress &targetHostAddress, quint16 targetHostPort){
        qDebug()<<"--sendCachedInterestGroupChatMessagesPacket(...)";

        //TODO:缓存消息的格式
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::GROUP_CHAT_MESSAGES_CACHED_ON_SERVER));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << messages.join(QString(UNIT_SEPARTOR));
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

    bool sendInterestGroupChatMessagesToMemberPacket(int peerSocketID, quint32 interestGroupID, const QString &senderID, const QString &message, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendInterestGroupChatMessagesToMemberPacket(...)";

        //TODO:缓存消息的格式
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::GROUP_CHAT_MESSAGE_FROM_SERVER));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

        out << interestGroupID << senderID << message;
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

    bool sendFileServerInfoToUser(int peerSocketID, const QByteArray &sessionEncryptionKey){
        qDebug()<<"--sendFileServerInfoToUser(...)";

        qCritical()<<"-------!!!!-------TODO:Get File Server Info!-------!!!!-------";
        quint32 ipv4Address = 0;
        quint16 port = m_localRTPListeningPort;


        //TODO:缓存消息的格式
        Packet *packet = PacketHandlerBase::getPacket(peerSocketID);
        packet->setPacketType(quint8(IM::SERVER_RESPONSE_FILE_SERVER_INFO));
        packet->setTransmissionProtocol(TP_RUDP);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);

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


    //    void slotCheckIMUsersOnlineStatus();

    void peerDisconnected(int socketID);

    void sendInterestGroupChatMessageToMembers(quint32 interestGroupID, UserInfo *sender, const QString &message);

    void addContactForUser(UserInfo *userInfo, UserInfo *contactInfo, quint32 groupID);

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
