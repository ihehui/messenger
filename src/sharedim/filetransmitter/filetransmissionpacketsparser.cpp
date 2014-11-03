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


namespace HEHUI {


FileTransmissionPacketsParserBase::FileTransmissionPacketsParserBase(const QString &myID, QObject *parent)
    :QObject(parent), m_myID(myID)
{

    cryptography = new Cryptography();
    sessionEncryptionKey = QByteArray();


    m_resourcesManager = new ResourcesManager(this);

    QString errorMessage = "";
    m_udpServer = m_resourcesManager->startUDPServer(QHostAddress::Any, 0, true, &errorMessage);
    if(!m_udpServer){
        qCritical()<<tr("ERROR! Can not start UDP listening! %1").arg(errorMessage);
    }else{
        qWarning()<<QString("UDP listening on port %1 for file transmission!").arg(m_udpServer->localPort());
    }

    m_rtp = m_resourcesManager->startRTP(QHostAddress::Any, 0, true, &errorMessage);
    if(!errorMessage.isEmpty()){
        qCritical()<<errorMessage;
    }
    //connect(m_rtp, SIGNAL(connected(int, const QString &, quint16)), this, SLOT(peerConnected(int, const QString &, quint16)));
    connect(m_rtp, SIGNAL(disconnected(int)), this, SLOT(peerDisconnected(int)));



    //m_udpServer = m_resourcesManager->getUDPServer();
    Q_ASSERT_X(m_udpServer, "IMClientPacketsParser::IMClientPacketsParser(...)", "Invalid UDPServer!");
    connect(m_udpServer, SIGNAL(signalNewUDPPacketReceived(Packet*)), this, SLOT(parseIncomingPacketData(Packet*)), Qt::QueuedConnection);

    //m_rtp = m_resourcesManager->getRTP();
    Q_ASSERT(m_rtp);

    m_udtProtocol = m_rtp->getUDTProtocol();
    Q_ASSERT(m_udtProtocol);
    m_udtProtocol->startWaitingForIOInOneThread(20);
    //m_udtProtocol->startWaitingForIOInSeparateThread(100, 1000);
    connect(m_udtProtocol, SIGNAL(packetReceived(Packet*)), this, SLOT(parseIncomingPacketData(Packet*)), Qt::QueuedConnection);

    m_tcpServer = m_rtp->getTCPServer();
    Q_ASSERT(m_tcpServer);
    connect(m_tcpServer, SIGNAL(packetReceived(Packet*)), this, SLOT(parseIncomingPacketData(Packet*)), Qt::QueuedConnection);

//    m_socketConnectedToServer = INVALID_SOCK_ID;


    

}

FileTransmissionPacketsParserBase::~FileTransmissionPacketsParserBase() {
    // TODO Auto-generated destructor stub

    qDebug()<<"IMClientPacketsParser::~IMClientPacketsParser() ";

    QMutexLocker locker(&mutex);


    if(cryptography){
        delete cryptography;
        cryptography = 0;
    }



    if(m_udpServer){
        m_udpServer->close();
    }

    if(m_rtp){
        m_rtp->stopServers();
    }



    delete m_resourcesManager;
    m_resourcesManager = 0;


}




void FileTransmissionPacketsParserBase::parseIncomingPacketData(Packet *packet){
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

    case quint8(DataForwardRequestByClient):
    {
        QByteArray encryptedData;
        QString senderID = peerID;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithPeerHash.value(senderID), false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);

        QString receiverID = "";
        QByteArray data;
        stream >> receiverID >> data;

        forwardData(peerSocketHash.value(receiverID), data, sessionEncryptionKeyWithPeerHash.value(receiverID));

        qDebug()<<"~~DataForwardRequestByClient";
    }
        break;

    case quint8(ForwardedDataByServer):
    {
        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithPeerHash.value(peerID), false);
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

//-------------------------------------------------------------
        //File TX
    case quint8(RequestUploadFile):
    {
        QByteArray encryptedData;
        QString contactID = peerID;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithPeerHash.value(contactID), false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QByteArray fileMD5Sum;
        QString fileName = "";
        quint64 size = 0;
        QString localFileSaveDir = "./";
        stream >> fileMD5Sum >> fileName >> size >> localFileSaveDir ;

        emit signalPeerRequestUploadFile(socketID, contactID, fileMD5Sum, fileName, size, localFileSaveDir);

        qDebug()<<"~~RequestUploadFile";
    }
        break;

    case quint8(CancelUploadFileRequest):
    {
        QByteArray encryptedData;
        QString contactID = peerID;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithPeerHash.value(contactID), false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QByteArray fileMD5Sum;
        stream >> fileMD5Sum;

        emit signalPeerCanceledUploadFileRequest(socketID, contactID, fileMD5Sum);

        qDebug()<<"~~CancelUploadFileRequest";
    }
        break;

    case quint8(RequestDownloadFile):
    {
        QByteArray encryptedData;
        QString contactID = peerID;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithPeerHash.value(contactID), false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QString fileName;
        stream >> fileName;

        emit signalContactRequestDownloadFile(socketID, contactID, fileName);

        qDebug()<<"~~RequestDownloadFile";
    }
        break;

    case quint8(CancelDownloadFileRequest):
    {
        QByteArray encryptedData;
        QString contactID = peerID;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithPeerHash.value(contactID), false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QString fileName;
        stream >> fileName;

        emit signalPeerCanceledDownloadFileRequest(socketID, contactID, fileName);

        qDebug()<<"~~CancelDownloadFileRequest";
    }
        break;

    case quint8(ResponseFileDownloadRequest):
    {
        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithPeerHash.value(peerID), false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);

        QString remoteFileName = "";
        bool accepted = false;
        stream >> remoteFileName >> accepted;
        if(accepted){
            QByteArray fileMD5Sum;
            quint64 size = 0;
            stream >> fileMD5Sum >> size;
            emit signalFileDownloadRequestAccepted(socketID, peerID, remoteFileName, fileMD5Sum, size);
        }else{
            QString message;
            stream >> message;
            emit signalFileDownloadRequestDenied(socketID, peerID, remoteFileName, message);

        }

        qDebug()<<"~~ResponseFileDownloadRequest";
    }
    break;

    case quint8(ResponseFileUploadRequest):
    {
        QByteArray encryptedData;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithPeerHash.value(peerID), false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QByteArray fileMD5Sum;
        QString message = "";
        bool accepted = false;
        stream >> fileMD5Sum >> accepted >> message;
        emit signalFileUploadRequestResponsed(socketID, peerID, fileMD5Sum, accepted, message);

        qDebug()<<"~~ResponseFileUploadRequest";
    }
    break;

    case quint8(RequestFileData):
    {
        QByteArray encryptedData;
        QString contactID = peerID;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithPeerHash.value(contactID), false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QByteArray fileMD5;
        int startPieceIndex = 0, endPieceIndex = 0;
        stream >> fileMD5 >> startPieceIndex >> endPieceIndex;

        emit signalFileDataRequested(socketID, contactID, fileMD5, startPieceIndex, endPieceIndex);

        qDebug()<<"~~RequestFileData";
    }
        break;
    case quint8(FileData):
    {
        QByteArray encryptedData;
        QString contactID = peerID;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithPeerHash.value(contactID), false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QByteArray fileMD5, data, sha1;;
        int pieceIndex = 0;
        stream >> fileMD5 >> pieceIndex >> data >>sha1;

        emit signalFileDataReceived(socketID, contactID, fileMD5, pieceIndex, data, sha1);

        //qDebug()<<"~~FileData";
    }
        break;
    case quint8(FileTXStatusChanged):
    {
        QByteArray encryptedData;
        QString contactID = peerID;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithPeerHash.value(contactID), false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QByteArray fileMD5;
        quint8 status;
        stream >> fileMD5 >> status;

        emit signalFileTXStatusChanged(socketID, contactID, fileMD5, status);

        qDebug()<<"~~FileTXStatusChanged";
    }
        break;
    case quint8(FileTXError):
    {
        QByteArray encryptedData;
        QString contactID = peerID;
        in >> encryptedData;

        QByteArray decryptedData;
        cryptography->teaCrypto(&decryptedData, encryptedData, sessionEncryptionKeyWithPeerHash.value(contactID), false);
        //TODO
        QDataStream stream(&decryptedData, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_4_8);
        QByteArray fileMD5;
        quint8 errorCode;
        QString message;
        stream >> fileMD5 >> errorCode >> message;

        emit signalFileTXError(socketID, contactID, fileMD5, errorCode, message);

        qDebug()<<"~~FileTXStatusChanged";
    }
        break;




    default:
        parseOtherIncomingPacketData(packet);
        qWarning()<<"Unknown Packet Type:"<<packetType
                 <<" From:"<<peerAddress.toString()
                <<" Port:"<<peerPort;
        return;
    }

    PacketHandlerBase::recylePacket(packet);

}

void FileTransmissionPacketsParserBase::parseOtherIncomingPacketData(Packet *packet){
    //TODO
}


QStringList FileTransmissionPacketsParserBase::runningNICAddresses(){

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

QString FileTransmissionPacketsParserBase::lastErrorMessage() const{
    return m_rtp->lastErrorString();
}

void FileTransmissionPacketsParserBase::setPeerSessionEncryptionKey(const QString &peerID, const QByteArray &encryptionKey){
    sessionEncryptionKeyWithPeerHash[peerID] = encryptionKey;
}

void FileTransmissionPacketsParserBase::peerDisconnected(int socketID){
    QString peerID = peerSocketHash.key(socketID);
    peerSocketHash.remove(peerID);

    emit signalPeerDisconnected(peerID);
}












} //namespace HEHUI
