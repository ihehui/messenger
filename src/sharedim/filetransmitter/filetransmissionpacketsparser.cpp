/*
 ****************************************************************************
 * filetransmissionpacketsparser.cpp
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

#include "filetransmissionpacketsparser.h"


#include <QNetworkInterface>


namespace HEHUI
{


FileTransmissionPacketsParserBase::FileTransmissionPacketsParserBase(const QString &myID, QObject *parent)
    : QObject(parent), m_myID(myID)
{

    cryptography = new Cryptography();

    m_resourcesManager = new ResourcesManager(this);

    QString errorMessage = "";
    m_udpServer = m_resourcesManager->startUDPServer(QHostAddress::Any, 0, true, &errorMessage);
    if(!m_udpServer) {
        qCritical() << tr("ERROR! Can not start UDP listening! %1").arg(errorMessage);
    } else {
        qWarning() << QString("UDP listening on port %1 for file transmission!").arg(m_udpServer->localPort());
    }

    m_rtp = m_resourcesManager->startRTP(QHostAddress::Any, 0, true, &errorMessage);
    if(!errorMessage.isEmpty()) {
        qCritical() << errorMessage;
    }
    //connect(m_rtp, SIGNAL(connected(int, const QString &, quint16)), this, SLOT(peerConnected(int, const QString &, quint16)));
    connect(m_rtp, SIGNAL(disconnected(int)), this, SLOT(peerDisconnected(int)));



    //m_udpServer = m_resourcesManager->getUDPServer();
    Q_ASSERT_X(m_udpServer, "IMClientPacketsParser::IMClientPacketsParser(...)", "Invalid UDPServer!");
    connect(m_udpServer, SIGNAL(signalNewUDPPacketReceived(Packet *)), this, SLOT(parseIncomingPacketData(Packet *)), Qt::QueuedConnection);

    //m_rtp = m_resourcesManager->getRTP();
    Q_ASSERT(m_rtp);

    m_udtProtocol = m_rtp->getUDTProtocol();
    Q_ASSERT(m_udtProtocol);
    m_udtProtocol->startWaitingForIOInOneThread(20);
    //m_udtProtocol->startWaitingForIOInSeparateThread(100, 1000);
    connect(m_udtProtocol, SIGNAL(packetReceived(Packet *)), this, SLOT(parseIncomingPacketData(Packet *)), Qt::QueuedConnection);

    m_tcpServer = m_rtp->getTCPServer();
    Q_ASSERT(m_tcpServer);
    connect(m_tcpServer, SIGNAL(packetReceived(Packet *)), this, SLOT(parseIncomingPacketData(Packet *)), Qt::QueuedConnection);

//    m_socketConnectedToServer = INVALID_SOCK_ID;




}

FileTransmissionPacketsParserBase::~FileTransmissionPacketsParserBase()
{
    // TODO Auto-generated destructor stub

    qDebug() << "IMClientPacketsParser::~IMClientPacketsParser() ";

    QMutexLocker locker(&mutex);


    if(cryptography) {
        delete cryptography;
        cryptography = 0;
    }



    if(m_udpServer) {
        m_udpServer->close();
    }

    if(m_rtp) {
        m_rtp->stopServers();
    }



    delete m_resourcesManager;
    m_resourcesManager = 0;


}




void FileTransmissionPacketsParserBase::parseIncomingPacketData(PacketBase *packet)
{
    //    qDebug()<<"----IMClientPacketsParser::parseIncomingPacketData(Packet *packet)";

    quint8 packetType = packet->getPacketType();
    QString peerID = packet->getPeerID();

    QHostAddress peerAddress = packet->getPeerHostAddress();
    quint16 peerPort = packet->getPeerHostPort();
    SOCKETID socketID = packet->getSocketID();



    switch(packetType) {
    case quint8(IM::CMD_DataForward): {
        DataForwardPacket p(*packet, sessionEncryptionKeyWithPeerHash.value(peerID));
        QByteArray data = p.data;

        if(p.isRequest) {
            QString receiverID = p.receiver;
            forwardData(peerSocketHash.value(receiverID), data);
        } else {
            PacketBase packet2;
            if(packet2.fromByteArray(&data)) {
                packet2.setSocketID(socketID);
                parseIncomingPacketData(&packet2);
            } else {
                qWarning() << "ERROR! Can not convert data to Packet!";
            }
        }

        //qDebug()<<"~~CMD_DataForward";
    }
    break;

    ////////////////////////////////////////////
    case quint8(IM::CMD_FileTransfer): {
        FileTransferPacket p(*packet, sessionEncryptionKeyWithPeerHash.value(peerID));
        emit signalFileTransferPacketReceived(p);
        //qDebug()<<"~~CMD_FileTransfer";
    }
    break;



    default:
        parseOtherIncomingPacketData(packet);
        qWarning() << "Unknown Packet Type:" << packetType
                   << " From:" << peerAddress.toString()
                   << " Port:" << peerPort;
        return;
    }


}

void FileTransmissionPacketsParserBase::parseOtherIncomingPacketData(PacketBase *packet)
{
    //TODO
}


QStringList FileTransmissionPacketsParserBase::runningNICAddresses()
{

    QStringList addresses;

    foreach (QNetworkInterface nic, QNetworkInterface::allInterfaces()) {
        if ( (nic.flags() & QNetworkInterface::IsRunning) && ( !(nic.flags() & QNetworkInterface::IsLoopBack)) ) {
            foreach (QNetworkAddressEntry entry, nic.addressEntries()) {
                //qDebug()<<"IP:"<<entry.ip()<<" Hardware Address:"<<interface.hardwareAddress()<<" Flags:"<<interface.flags();
                QHostAddress ip = entry.ip();
                if(ip.protocol() == QAbstractSocket::IPv6Protocol) {
                    continue;
                }
                if(ip.isLoopback()) {
                    continue;
                }

                addresses.append(ip.toString());
            }
        }
    }

    return addresses;

}

QString FileTransmissionPacketsParserBase::lastErrorMessage() const
{
    return m_rtp->lastErrorString();
}

void FileTransmissionPacketsParserBase::setPeerSessionEncryptionKey(const QString &peerID, const QByteArray &encryptionKey)
{
    sessionEncryptionKeyWithPeerHash[peerID] = encryptionKey;
}

void FileTransmissionPacketsParserBase::peerDisconnected(int socketID)
{
    QString peerID = peerSocketHash.key(socketID);
    peerSocketHash.remove(peerID);

    emit signalPeerDisconnected(peerID);
}

QByteArray FileTransmissionPacketsParserBase::getSessionEncryptionKey(SOCKETID socketID)
{
    QString peerID = peerSocketHash.key(socketID);
    return sessionEncryptionKeyWithPeerHash.value(peerID);
}












} //namespace HEHUI
