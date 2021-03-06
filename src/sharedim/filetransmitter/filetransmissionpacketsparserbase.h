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
#include "../networkmanagerbase.h"
#include "../imuserbase.h"
#include "../impackets.h"


#include "HHSharedCore/Cryptography"
#include "HHSharedNetwork/NetworkUtilities"



namespace HEHUI
{


class SHAREDIMLIB_API FileTransmissionPacketsParserBase : public QObject
{
    Q_OBJECT
public:
    FileTransmissionPacketsParserBase(const QString &myID, QObject *parent = 0);
    virtual ~FileTransmissionPacketsParserBase();

public slots:
    void parseIncomingPacketData(const PacketBase &packet);

    virtual void parseOtherIncomingPacketData(const PacketBase &packet);

    int connectToPeer(const QHostAddress &peerAddress, quint16 peerPort)
    {
        QString errorMessage;
        int socket = INVALID_SOCK_ID;
        socket = m_rtp->connectToHost(peerAddress, peerPort, 10000, &errorMessage);
        if(socket == INVALID_SOCK_ID) {
            qCritical() << tr("ERROR! Can not connect to host! %1").arg(errorMessage);
        }

        return socket;
    }


    bool requestDataForward(int serverSocketID, const QString &receiverID, const QByteArray &data)
    {

        DataForwardPacket p(getSessionEncryptionKey(serverSocketID));
        p.data = data;
        p.receiver = receiverID;
        p.isRequest = 1;

        QByteArray ba = p.toByteArray();
        if(ba.isEmpty()) {
            return false;
        }

        return m_rtp->sendReliableData(serverSocketID, &ba);
    }

    bool forwardData(int socketID, const QByteArray &data)
    {

        DataForwardPacket p(getSessionEncryptionKey(socketID));
        p.data = data;
        p.isRequest = 0;

        QByteArray ba = p.toByteArray();
        if(ba.isEmpty()) {
            return false;
        }

        return m_rtp->sendReliableData(socketID, &ba);
    }



    ///////////////////////////////////////////////
    bool requestFileServerInfo(RTP * rtp, SOCKETID socketID, const QString &contactID)
    {
        if(!rtp){
            qCritical()<<"Invalid RTP Server!";
            return false;
        }

        FileTransferPacket packet(FileTransferPacket::FT_FILE_SERVER_INFO, getSessionEncryptionKey(socketID));
        packet.ContactID = contactID;
        packet.FileServerInfo.request = true;
        //packet.FileServerInfo.address = "";
        //packet.FileServerInfo.port = 0;

        QByteArray ba = packet.toByteArray();
        return rtp->sendReliableData(socketID, &ba);
    }
    bool responseFileServerInfo(RTP * rtp, SOCKETID socketID, quint32 address, quint16 tcpPort, quint16 rtpPort, quint16 externalPort)
    {
        if(!rtp){
            qCritical()<<"Invalid RTP Server!";
            return false;
        }

        FileTransferPacket packet(FileTransferPacket::FT_FILE_SERVER_INFO, getSessionEncryptionKey(socketID));
        packet.ContactID = m_myID;
        packet.FileServerInfo.request = false;
        packet.FileServerInfo.lanAddress = address;
        packet.FileServerInfo.tcpPort = tcpPort;
        packet.FileServerInfo.rtpPort = rtpPort;
        packet.FileServerInfo.wanPort = externalPort;


        QByteArray ba = packet.toByteArray();
        return rtp->sendReliableData(socketID, &ba);
    }

    bool requestFileSystemInfo(SOCKETID socketID, const QString &parentDirPath)
    {
        FileTransferPacket packet(FileTransferPacket::FT_FileSystemInfoRequest, getSessionEncryptionKey(socketID));
        packet.FileSystemInfoRequest.parentDirPath = parentDirPath;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool responseFileSystemInfo(SOCKETID socketID, const QString &baseDirPath, const QByteArray &fileSystemInfoData)
    {
        FileTransferPacket packet(FileTransferPacket::FT_FileSystemInfoResponse, getSessionEncryptionKey(socketID));
        packet.FileSystemInfoResponse.baseDirPath = baseDirPath;
        packet.FileSystemInfoResponse.fileSystemInfoData = fileSystemInfoData;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool requestDeleteFiles(SOCKETID socketID, const QString &remoteBaseDir, const QStringList &remoteFiles)
    {
        FileTransferPacket packet(FileTransferPacket::FT_FileDeletingRequest, getSessionEncryptionKey(socketID));
        packet.FileDeletingRequest.baseDirPath = remoteBaseDir;
        packet.FileDeletingRequest.files = remoteFiles;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool responseDeletingFiles(SOCKETID socketID, const QString &baseDirPath, const QStringList &failedFiles)
    {
        FileTransferPacket packet(FileTransferPacket::FT_FileDeletingResponse, getSessionEncryptionKey(socketID));
        packet.FileDeletingResponse.baseDirPath = baseDirPath;
        packet.FileDeletingResponse.failedFiles = failedFiles;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool requestRenameFile(SOCKETID socketID, const QString &remoteBaseDir, const QString &oldFileName, const QString &newFileName)
    {
        FileTransferPacket packet(FileTransferPacket::FT_FileRenamingRequest, getSessionEncryptionKey(socketID));
        packet.FileRenamingRequest.baseDirPath = remoteBaseDir;
        packet.FileRenamingRequest.oldFileName = oldFileName;
        packet.FileRenamingRequest.newFileName = newFileName;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool responseRenamingFiles(SOCKETID socketID, const QString &baseDirPath, const QString &fileName, bool renamed, const QString &message)
    {
        FileTransferPacket packet(FileTransferPacket::FT_FileRenamingResponse, getSessionEncryptionKey(socketID));
        packet.FileRenamingResponse.baseDirPath = baseDirPath;
        packet.FileRenamingResponse.oldFileName = fileName;
        packet.FileRenamingResponse.renamed = renamed;
        packet.FileRenamingResponse.message = message;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool requestUploadFile(SOCKETID socketID, const QByteArray &fileMD5Sum, const QString &fileName, quint64 size, const QString &remoteFileSaveDir = "")
    {
        FileTransferPacket packet(FileTransferPacket::FT_FileUploadingRequest, getSessionEncryptionKey(socketID));
        packet.FileUploadingRequest.fileName = fileName;
        packet.FileUploadingRequest.fileMD5Sum = fileMD5Sum;
        packet.FileUploadingRequest.size = size;
        packet.FileUploadingRequest.fileSaveDir = remoteFileSaveDir;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool requestDownloadFile(SOCKETID socketID, const QString &remoteBaseDir, const QString &remoteFileName, const QString &localFileSaveDir)
    {
        FileTransferPacket packet(FileTransferPacket::FT_FileDownloadingRequest, getSessionEncryptionKey(socketID));
        packet.FileDownloadingRequest.baseDir = remoteBaseDir;
        packet.FileDownloadingRequest.fileName = remoteFileName;
        packet.FileDownloadingRequest.dirToSaveFile = localFileSaveDir;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool responseFileDownloadRequest(SOCKETID socketID, bool accepted, const QString &fileName, const QByteArray &fileMD5Sum = QByteArray(), quint64 size = 0)
    {
        FileTransferPacket packet(FileTransferPacket::FT_FileDownloadingResponse, getSessionEncryptionKey(socketID));
        packet.FileDownloadingResponse.accepted = accepted;
        //packet.FileDownloadingResponse.baseDir = baseDir;
        packet.FileDownloadingResponse.fileName = fileName;
        packet.FileDownloadingResponse.fileMD5Sum = fileMD5Sum;
        packet.FileDownloadingResponse.size = size;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool responseFileUploadRequest(SOCKETID socketID, bool accepted, const QByteArray &fileMD5Sum, quint8 errorCode)
    {
        FileTransferPacket packet(FileTransferPacket::FT_FileUploadingResponse, getSessionEncryptionKey(socketID));
        packet.FileUploadingResponse.fileMD5Sum = fileMD5Sum;
        packet.FileUploadingResponse.accepted = accepted;
        packet.FileUploadingResponse.errorCode = errorCode;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool requestFileData(SOCKETID socketID, const QByteArray &fileMD5, int startPieceIndex, int endPieceIndex)
    {
        //qDebug()<<"--requestFileData(...) "<<" startPieceIndex:"<<startPieceIndex<<" endPieceIndex:"<<endPieceIndex;

        FileTransferPacket packet(FileTransferPacket::FT_FileDataRequest, getSessionEncryptionKey(socketID));
        packet.FileDataRequest.fileMD5 = fileMD5;
        packet.FileDataRequest.startPieceIndex = startPieceIndex;
        packet.FileDataRequest.endPieceIndex = endPieceIndex;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool sendFileData(SOCKETID socketID, const QByteArray &fileMD5, int pieceIndex, const QByteArray *data, const QByteArray *pieceMD5)
    {
        FileTransferPacket packet(FileTransferPacket::FT_FileData, getSessionEncryptionKey(socketID));
        packet.FileDataResponse.fileMD5 = fileMD5;
        packet.FileDataResponse.pieceIndex = pieceIndex;
        packet.FileDataResponse.data = *data;
        packet.FileDataResponse.pieceMD5 = *pieceMD5;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool fileTXStatusChanged(SOCKETID socketID, const QByteArray &fileMD5, quint8 status)
    {
        FileTransferPacket packet(FileTransferPacket::FT_FileTXStatus, getSessionEncryptionKey(socketID));
        packet.FileTXStatus.fileMD5 = fileMD5;
        packet.FileTXStatus.status = status;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool fileTXError(SOCKETID socketID, const QByteArray &fileMD5, quint8 errorCode, const QString &errorString)
    {
        FileTransferPacket packet(FileTransferPacket::FT_FileTXError, getSessionEncryptionKey(socketID));
        packet.FileTXError.fileMD5 = fileMD5;
        packet.FileTXError.errorCode = errorCode;
        packet.FileTXError.message = errorString;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool stopFileTX(SOCKETID socketID, const QString &remoteFileName, const QByteArray &fileMD5)
    {
        FileTransferPacket packet(FileTransferPacket::FT_FileTXStop, getSessionEncryptionKey(socketID));
        packet.FileTXError.fileName = remoteFileName;
        packet.FileTXError.fileMD5 = fileMD5;

        QByteArray ba = packet.toByteArray();
        return m_rtp->sendReliableData(socketID, &ba);
    }



signals:
    void signalPeerDisconnected(const QString &peerID);

    //File TX
    void signalFileTransferPacketReceived(const FileTransferPacket &packet);


public:
    QStringList runningNICAddresses();
    QString lastErrorMessage() const;

    void setPeerSessionEncryptionKey(const QString &peerID, const QByteArray &encryptionKey);

    void getServerPorts(quint16 *udpPort, quint16 *tcpPort, quint16 *rtpPort);

private slots:
    void peerDisconnected(SOCKETID socketID);

    QByteArray getSessionEncryptionKey(SOCKETID socketID);

private:

    QMutex mutex;

    QString m_myID;

    Cryptography *cryptography;

    QHash<QString/*Contact's ID*/, QByteArray/*Session Encryption Key*/> sessionEncryptionKeyWithPeerHash;
    QHash<QString/*Contact's ID*/, int/*Socket ID*/> peerSocketHash;

    NetworkManagerBase *m_networkManager;


    UDPServer *m_udpServer;
    RTP *m_rtp;
//    UDTProtocol *m_udtProtocol;
    TCPServer *m_tcpServer;

    ENETProtocol *m_enetProtocol;



//    int m_socketConnectedToServer;


};

}

#endif /* FILETXPACKETSPARSERBASE_H_ */
