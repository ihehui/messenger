/*
 ****************************************************************************
 * FileTransmissionPacketsParserBase.h
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

#ifndef FILETXPACKETSPARSERBASE_H_
#define FILETXPACKETSPARSERBASE_H_



#include <QHostAddress>
#include <QHostInfo>
#include <QDebug>
#include <QFile>


#include "../constants_global_shared.h"
#include "../resourcesmanager.h"
#include "../imuserbase.h"

#include "HHSharedCore/hcryptography.h"
#include "HHSharedNetwork/hnetworkutilities.h"



namespace HEHUI {


class SHAREDIMLIB_API FileTransmissionPacketsParserBase : public QObject{
    Q_OBJECT
public:
    FileTransmissionPacketsParserBase(const QString &myID, QObject *parent = 0);
    virtual ~FileTransmissionPacketsParserBase();

public slots:
    void parseIncomingPacketData(Packet *packet);

    virtual void parseOtherIncomingPacketData(Packet *packet);

    int connectToPeer(const QHostAddress &peerAddress, quint16 peerPort){
        QString errorMessage;
        int socket = INVALID_SOCK_ID;
        socket = m_rtp->connectToHost(peerAddress, peerPort, 10000, &errorMessage);
        if(socket == INVALID_SOCK_ID){
            qCritical()<<tr("ERROR! Can not connect to host! %1").arg(errorMessage);
        }

        return socket;
    }


    bool requestDataForward(int socketID, const QString &receiverID, const QByteArray &data, const QByteArray &sessionEncryptionKey){
        Packet *packet = PacketHandlerBase::getPacket(socketID);

        packet->setPacketType(quint8(DataForwardRequestByClient));
        packet->setTransmissionProtocol(TP_UDT);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << receiverID << data;

        QByteArray encryptedData;
        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKey, true);
        ba.clear();
        out.device()->seek(0);
        out << m_myID << encryptedData;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;

        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool forwardData(int socketID, const QByteArray &data, const QByteArray &sessionEncryptionKey){
        Packet *packet = PacketHandlerBase::getPacket(socketID);

        packet->setPacketType(quint8(ForwardedDataByServer));
        packet->setTransmissionProtocol(TP_UDT);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << data;

        QByteArray encryptedData;
        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKey, true);
        ba.clear();
        out.device()->seek(0);
        out << m_myID << encryptedData;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;

        return m_rtp->sendReliableData(socketID, &ba);
    }



    ///////////////////////////////////////////////
    bool requestUploadFile(int socketID, const QString &contactID, const QByteArray &fileMD5Sum, const QString &fileName, quint64 size, const QString &remoteFileSaveDir = ""){
        Packet *packet = PacketHandlerBase::getPacket(socketID);

        packet->setPacketType(quint8(RequestUploadFile));
        packet->setTransmissionProtocol(TP_UDT);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << fileMD5Sum << fileName << size << remoteFileSaveDir;

        QByteArray encryptedData;
        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKeyWithPeerHash.value(contactID), true);
        ba.clear();
        out.device()->seek(0);
        out << m_myID << encryptedData;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;

        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool cancelUploadFileRequest(int socketID, const QString &contactID, const QByteArray &fileMD5Sum){
        Packet *packet = PacketHandlerBase::getPacket(socketID);

        packet->setPacketType(quint8(CancelUploadFileRequest));
        packet->setTransmissionProtocol(TP_UDT);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << fileMD5Sum;

        QByteArray encryptedData;
        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKeyWithPeerHash.value(contactID), true);
        ba.clear();
        out.device()->seek(0);
        out << m_myID << encryptedData;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;

        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool requestDownloadFile(int socketID, const QString &contactID, const QString &remoteFileName){
        Packet *packet = PacketHandlerBase::getPacket(socketID);

        packet->setPacketType(quint8(RequestDownloadFile));
        packet->setTransmissionProtocol(TP_UDT);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << remoteFileName;

        QByteArray encryptedData;
        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKeyWithPeerHash.value(contactID), true);
        ba.clear();
        out.device()->seek(0);
        out << m_myID << encryptedData;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;

        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool cancelDownloadFileRequest(int socketID, const QString &contactID, const QString &remoteFileName){
        Packet *packet = PacketHandlerBase::getPacket(socketID);

        packet->setPacketType(quint8(CancelDownloadFileRequest));
        packet->setTransmissionProtocol(TP_UDT);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << remoteFileName;

        QByteArray encryptedData;
        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKeyWithPeerHash.value(contactID), true);
        ba.clear();
        out.device()->seek(0);
        out << m_myID << encryptedData;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;

        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool acceptFileDownloadRequest(int socketID, const QString &contactID, const QString &fileName, const QByteArray &fileMD5Sum, quint64 size){
        Packet *packet = PacketHandlerBase::getPacket(socketID);

        packet->setPacketType(quint8(ResponseFileDownloadRequest));
        packet->setTransmissionProtocol(TP_UDT);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << fileName << true << fileMD5Sum << size;

        QByteArray encryptedData;
        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKeyWithPeerHash.value(contactID), true);
        ba.clear();
        out.device()->seek(0);
        out << m_myID << encryptedData;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;

        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool denyFileDownloadRequest(int socketID, const QString &contactID, const QString &fileName, const QString &message){
        Packet *packet = PacketHandlerBase::getPacket(socketID);

        packet->setPacketType(quint8(ResponseFileDownloadRequest));
        packet->setTransmissionProtocol(TP_UDT);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << fileName << false << message;

        QByteArray encryptedData;
        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKeyWithPeerHash.value(contactID), true);
        ba.clear();
        out.device()->seek(0);
        out << m_myID << encryptedData;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;

        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool responseFileUploadRequest(int socketID, const QString &contactID, const QByteArray &fileMD5Sum, bool accepted, const QString &message){
        Packet *packet = PacketHandlerBase::getPacket(socketID);

        packet->setPacketType(quint8(ResponseFileUploadRequest));
        packet->setTransmissionProtocol(TP_UDT);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << fileMD5Sum << accepted << message;

        QByteArray encryptedData;
        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKeyWithPeerHash.value(contactID), true);
        ba.clear();
        out.device()->seek(0);
        out << m_myID << encryptedData;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;

        return m_rtp->sendReliableData(socketID, &ba);
    }


    bool requestFileData(int socketID, const QString &contactID, const QByteArray &fileMD5, int startPieceIndex, int endPieceIndex){
        qDebug()<<"--requestFileData(...) "<<" startPieceIndex:"<<startPieceIndex<<" endPieceIndex:"<<endPieceIndex;

        Packet *packet = PacketHandlerBase::getPacket(socketID);

        packet->setPacketType(quint8(RequestFileData));
        packet->setTransmissionProtocol(TP_UDT);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << fileMD5 << startPieceIndex << endPieceIndex;

        QByteArray encryptedData;
        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKeyWithPeerHash.value(contactID), true);
        ba.clear();
        out.device()->seek(0);
        out << m_myID << encryptedData;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;

        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool sendFileData(int socketID, const QString &contactID, const QByteArray &fileMD5, int pieceIndex, const QByteArray *data, const QByteArray *sha1){
        Packet *packet = PacketHandlerBase::getPacket(socketID);

        packet->setPacketType(quint8(FileData));
        packet->setTransmissionProtocol(TP_UDT);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << fileMD5 << pieceIndex << *data << *sha1 ;

        QByteArray encryptedData;
        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKeyWithPeerHash.value(contactID), true);
        ba.clear();
        out.device()->seek(0);
        out << m_myID << encryptedData;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;

        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool fileTXStatusChanged(int socketID, const QString &contactID, const QByteArray &fileMD5, quint8 status){
        Packet *packet = PacketHandlerBase::getPacket(socketID);

        packet->setPacketType(quint8(FileTXStatusChanged));
        packet->setTransmissionProtocol(TP_UDT);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << fileMD5 << status ;

        QByteArray encryptedData;
        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKeyWithPeerHash.value(contactID), true);
        ba.clear();
        out.device()->seek(0);
        out << m_myID << encryptedData;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;

        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool fileTXError(int socketID, const QString &contactID, const QByteArray &fileMD5, quint8 errorCode, const QString &errorString){
        Packet *packet = PacketHandlerBase::getPacket(socketID);

        packet->setPacketType(quint8(FileTXError));
        packet->setTransmissionProtocol(TP_UDT);
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_8);
        out << fileMD5 << errorCode << errorString ;

        QByteArray encryptedData;
        cryptography->teaCrypto(&encryptedData, ba, sessionEncryptionKeyWithPeerHash.value(contactID), true);
        ba.clear();
        out.device()->seek(0);
        out << m_myID << encryptedData;
        packet->setPacketData(ba);

        ba.clear();
        out.device()->seek(0);
        QVariant v;
        v.setValue(*packet);
        out << v;

        return m_rtp->sendReliableData(socketID, &ba);
    }






signals:

    ///////////////////////////
    //File TX
    void signalPeerRequestUploadFile(int socketID, const QString &peerID, const QByteArray &fileMD5Sum, const QString &fileName, quint64 size, const QString &localFileSaveDir);
    void signalPeerCanceledUploadFileRequest(int socketID, const QString &contactID, const QByteArray &fileMD5Sum);

    void signalContactRequestDownloadFile(int socketID, const QString &contactID, const QString &fileName);
    void signalPeerCanceledDownloadFileRequest(int socketID, const QString &contactID, const QString &fileName);

    void signalFileDownloadRequestAccepted(int socketID, const QString &contactID, const QString &remoteFileName, const QByteArray &fileMD5Sum, quint64 size);
    void signalFileDownloadRequestDenied(int socketID, const QString &contactID, const QString &remoteFileName, const QString &message);
    void signalFileUploadRequestResponsed(int socketID, const QString &contactID, const QByteArray &fileMD5Sum, bool accepted, const QString &message);

    void signalFileDataRequested(int socketID, const QString &contactID, const QByteArray &fileMD5, int startPieceIndex, int endPieceIndex);
    void signalFileDataReceived(int socketID, const QString &contactID, const QByteArray &fileMD5, int pieceIndex, const QByteArray &data, const QByteArray &sha1);
    void signalFileTXStatusChanged(int socketID, const QString &contactID, const QByteArray &fileMD5, quint8 status);
    void signalFileTXError(int socketID, const QString &contactID, const QByteArray &fileMD5, quint8 errorCode, const QString &errorString);

    void signalPeerDisconnected(const QString &peerID);


    ///////////////////////////
    //void signalFileSystemInfoReceived(int socketID, const QString &parentDirPath, const QByteArray &fileSystemInfoData);


public:
    QStringList runningNICAddresses();
    QString lastErrorMessage() const;

    void setPeerSessionEncryptionKey(const QString &peerID, const QByteArray &encryptionKey);

private slots:
    void peerDisconnected(int socketID);


private:

    enum Command {
        DataForwardRequestByClient,
        ForwardedDataByServer,

        //File TX
        RequestDownloadFile,
        CancelDownloadFileRequest,

        RequestUploadFile,
        CancelUploadFileRequest,

        ResponseFileDownloadRequest,
        ResponseFileUploadRequest,

        RequestFileData,
        FileData,

        FileTXStatusChanged,
        FileTXError,

    };



    QMutex mutex;

    QString m_myID;

    Cryptography *cryptography;
    QByteArray sessionEncryptionKey;

    QHash<QString/*Contact's ID*/, QByteArray/*Session Encryption Key*/> sessionEncryptionKeyWithPeerHash;
    QHash<QString/*Contact's ID*/, int/*Socket ID*/> peerSocketHash;


    ResourcesManager *m_resourcesManager;


    UDPServer *m_udpServer;
    RTP *m_rtp;
    UDTProtocol *m_udtProtocol;
    TCPServer *m_tcpServer;

//    int m_socketConnectedToServer;


};

}

#endif /* FILETXPACKETSPARSERBASE_H_ */
