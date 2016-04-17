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
#include "../impackets.h"

#include "HHSharedCore/hcryptography.h"
#include "HHSharedNetwork/hnetworkutilities.h"



namespace HEHUI {


class SHAREDIMLIB_API FileTransmissionPacketsParserBase : public QObject{
    Q_OBJECT
public:
    FileTransmissionPacketsParserBase(const QString &myID, QObject *parent = 0);
    virtual ~FileTransmissionPacketsParserBase();

public slots:
    void parseIncomingPacketData(PacketBase *packet);

    virtual void parseOtherIncomingPacketData(PacketBase *packet);

    int connectToPeer(const QHostAddress &peerAddress, quint16 peerPort){
        QString errorMessage;
        int socket = INVALID_SOCK_ID;
        socket = m_rtp->connectToHost(peerAddress, peerPort, 10000, &errorMessage);
        if(socket == INVALID_SOCK_ID){
            qCritical()<<tr("ERROR! Can not connect to host! %1").arg(errorMessage);
        }

        return socket;
    }


    bool requestDataForward(int socketID, const QString &receiverID, const QByteArray &data){

        DataForwardPacket p(sessionEncryptionKey);
        p.data = data;
        p.receiver = receiverID;

        QByteArray ba = p.toByteArray();
        if(ba.isEmpty()){return false;}

        return m_rtp->sendReliableData(socketID, &ba);
    }

    bool forwardData(int socketID, const QByteArray &data){

        DataForwardPacket p(sessionEncryptionKey);
        p.data = data;

        QByteArray ba = p.toByteArray();
        if(ba.isEmpty()){return false;}

        return m_rtp->sendReliableData(socketID, &ba);
    }



    ///////////////////////////////////////////////
    bool requestFileSystemInfo(SOCKETID socketID, const QString &parentDirPath){
        FileTransferPacket packet(sessionEncryptionKey);
        packet.InfoType = FileTransferPacket::FT_FileSystemInfoRequest;
        packet.FileSystemInfoRequest.parentDirPath = parentDirPath;

        return m_rtp->sendReliableData(socketID, &packet.toByteArray());
    }

    bool responseFileSystemInfo(SOCKETID socketID, const QString &baseDirPath, const QByteArray &fileSystemInfoData){

        FileTransferPacket packet(sessionEncryptionKey);
        packet.InfoType = FileTransferPacket::FT_FileSystemInfoResponse;
        packet.FileSystemInfoResponse.baseDirPath = baseDirPath;
        packet.FileSystemInfoResponse.fileSystemInfoData = fileSystemInfoData;

        return m_rtp->sendReliableData(socketID, &packet.toByteArray());
    }

    bool requestDeleteFiles(SOCKETID socketID, const QString &remoteBaseDir, const QStringList &remoteFiles){

        FileTransferPacket packet(sessionEncryptionKey);
        packet.InfoType = FileTransferPacket::FT_FileDeletingRequest;
        packet.FileDeletingRequest.baseDirPath = remoteBaseDir;
        packet.FileDeletingRequest.files = remoteFiles;

        return m_rtp->sendReliableData(socketID, &packet.toByteArray());
    }

    bool responseDeletingFiles(SOCKETID socketID, const QString &baseDirPath, const QStringList &failedFiles){

        FileTransferPacket packet(sessionEncryptionKey);
        packet.InfoType = FileTransferPacket::FT_FileDeletingResponse;
        packet.FileDeletingResponse.baseDirPath = baseDirPath;
        packet.FileDeletingResponse.failedFiles = failedFiles;

        return m_rtp->sendReliableData(socketID, &packet.toByteArray());
    }

    bool requestRenameFile(SOCKETID socketID, const QString &remoteBaseDir, const QString &oldFileName, const QString &newFileName){

        FileTransferPacket packet(sessionEncryptionKey);
        packet.InfoType = FileTransferPacket::FT_FileRenamingRequest;
        packet.FileRenamingRequest.baseDirPath = remoteBaseDir;
        packet.FileRenamingRequest.oldFileName = oldFileName;
        packet.FileRenamingRequest.newFileName = newFileName;

        return m_rtp->sendReliableData(socketID, &packet.toByteArray());
    }

    bool responseRenamingFiles(SOCKETID socketID, const QString &baseDirPath, const QString &fileName, bool renamed, const QString &message){

        FileTransferPacket packet(sessionEncryptionKey);
        packet.InfoType = FileTransferPacket::FT_FileRenamingResponse;
        packet.FileRenamingResponse.baseDirPath = baseDirPath;
        packet.FileRenamingResponse.oldFileName = fileName;
        packet.FileRenamingResponse.renamed = renamed;
        packet.FileRenamingResponse.message = message;

        return m_rtp->sendReliableData(socketID, &packet.toByteArray());
    }

    bool requestUploadFile(SOCKETID socketID, const QByteArray &fileMD5Sum, const QString &fileName, quint64 size, const QString &remoteFileSaveDir = ""){

        FileTransferPacket packet(sessionEncryptionKey);
        packet.InfoType = FileTransferPacket::FT_FileUploadingRequest;
        packet.FileUploadingRequest.fileName = fileName;
        packet.FileUploadingRequest.fileMD5Sum = fileMD5Sum;
        packet.FileUploadingRequest.size = size;
        packet.FileUploadingRequest.fileSaveDir = remoteFileSaveDir;

        return m_rtp->sendReliableData(socketID, &packet.toByteArray());
    }

    bool requestDownloadFile(SOCKETID socketID, const QString &remoteBaseDir, const QString &remoteFileName, const QString &localFileSaveDir){

        FileTransferPacket packet(sessionEncryptionKey);
        packet.InfoType = FileTransferPacket::FT_FileDownloadingRequest;
        packet.FileDownloadingRequest.baseDir = remoteBaseDir;
        packet.FileDownloadingRequest.fileName = remoteFileName;
        packet.FileDownloadingRequest.dirToSaveFile = localFileSaveDir;

        return m_rtp->sendReliableData(socketID, &packet.toByteArray());
    }

    bool responseFileDownloadRequest(SOCKETID socketID, bool accepted, const QString &baseDir, const QString &fileName, const QByteArray &fileMD5Sum, quint64 size, const QString &pathToSaveFile){

        FileTransferPacket packet(sessionEncryptionKey);
        packet.InfoType = FileTransferPacket::FT_FileDownloadingResponse;
        packet.FileDownloadingResponse.accepted = accepted;
        packet.FileDownloadingResponse.baseDir = baseDir;
        packet.FileDownloadingResponse.fileName = fileName;
        packet.FileDownloadingResponse.fileMD5Sum = fileMD5Sum;
        packet.FileDownloadingResponse.size = size;
        packet.FileDownloadingResponse.pathToSaveFile = pathToSaveFile;

        return m_rtp->sendReliableData(socketID, &packet.toByteArray());
    }

    bool responseFileUploadRequest(SOCKETID socketID, bool accepted, const QByteArray &fileMD5Sum, const QString &message){

        FileTransferPacket packet(sessionEncryptionKey);
        packet.InfoType = FileTransferPacket::FT_FileUploadingResponse;
        packet.FileUploadingResponse.fileMD5Sum = fileMD5Sum;
        packet.FileUploadingResponse.accepted = accepted;
        packet.FileUploadingResponse.message = message;

        return m_rtp->sendReliableData(socketID, &packet.toByteArray());
    }

    bool requestFileData(SOCKETID socketID, const QByteArray &fileMD5, int startPieceIndex, int endPieceIndex){
        //qDebug()<<"--requestFileData(...) "<<" startPieceIndex:"<<startPieceIndex<<" endPieceIndex:"<<endPieceIndex;

        FileTransferPacket packet(sessionEncryptionKey);
        packet.InfoType = FileTransferPacket::FT_FileDataRequest;
        packet.FileDataRequest.fileMD5 = fileMD5;
        packet.FileDataRequest.startPieceIndex = startPieceIndex;
        packet.FileDataRequest.endPieceIndex = endPieceIndex;

        return m_rtp->sendReliableData(socketID, &packet.toByteArray());
    }

    bool sendFileData(SOCKETID socketID, const QByteArray &fileMD5, int pieceIndex, const QByteArray *data, const QByteArray *pieceMD5){

        FileTransferPacket packet(sessionEncryptionKey);
        packet.InfoType = FileTransferPacket::FT_FileData;
        packet.FileDataResponse.fileMD5 = fileMD5;
        packet.FileDataResponse.pieceIndex = pieceIndex;
        packet.FileDataResponse.data = *data;
        packet.FileDataResponse.pieceMD5 = *pieceMD5;

        return m_rtp->sendReliableData(socketID, &packet.toByteArray());
    }

    bool fileTXStatusChanged(SOCKETID socketID, const QByteArray &fileMD5, quint8 status){

        FileTransferPacket packet(sessionEncryptionKey);
        packet.InfoType = FileTransferPacket::FT_FileTXStatus;
        packet.FileTXStatus.fileMD5 = fileMD5;
        packet.FileTXStatus.status = status;

        return m_rtp->sendReliableData(socketID, &packet.toByteArray());
    }

    bool fileTXError(SOCKETID socketID, const QByteArray &fileMD5, quint8 errorCode, const QString &errorString){

        FileTransferPacket packet(sessionEncryptionKey);
        packet.InfoType = FileTransferPacket::FT_FileTXError;
        packet.FileTXError.fileMD5 = fileMD5;
        packet.FileTXError.errorCode = errorCode;
        packet.FileTXError.message = errorString;

        return m_rtp->sendReliableData(socketID, &packet.toByteArray());
    }

    bool stopFileTX(SOCKETID socketID, const QString &remoteFileName, const QByteArray &fileMD5){
        FileTransferPacket packet(sessionEncryptionKey);
        packet.InfoType = FileTransferPacket::FT_FileTXStop;
        packet.FileTXError.fileName = remoteFileName;
        packet.FileTXError.fileMD5 = fileMD5;

        return m_rtp->sendReliableData(socketID, &packet.toByteArray());
    }



signals:
    void signalPeerDisconnected(const QString &peerID);

    ///////////////////////////
    //File TX
    void signalFileTransferPacketReceived(const FileTransferPacket &packet);

//    void signalPeerRequestUploadFile(int socketID, const QString &peerID, const QByteArray &fileMD5Sum, const QString &fileName, quint64 size, const QString &localFileSaveDir);
//    void signalPeerCanceledUploadFileRequest(int socketID, const QString &contactID, const QByteArray &fileMD5Sum);

//    void signalContactRequestDownloadFile(int socketID, const QString &contactID, const QString &fileName);
//    void signalPeerCanceledDownloadFileRequest(int socketID, const QString &contactID, const QString &fileName);

//    void signalFileDownloadRequestAccepted(int socketID, const QString &contactID, const QString &remoteFileName, const QByteArray &fileMD5Sum, quint64 size);
//    void signalFileDownloadRequestDenied(int socketID, const QString &contactID, const QString &remoteFileName, const QString &message);
//    void signalFileUploadRequestResponsed(int socketID, const QString &contactID, const QByteArray &fileMD5Sum, bool accepted, const QString &message);

//    void signalFileDataRequested(int socketID, const QString &contactID, const QByteArray &fileMD5, int startPieceIndex, int endPieceIndex);
//    void signalFileDataReceived(int socketID, const QString &contactID, const QByteArray &fileMD5, int pieceIndex, const QByteArray &data, const QByteArray &sha1);
//    void signalFileTXStatusChanged(int socketID, const QString &contactID, const QByteArray &fileMD5, quint8 status);
//    void signalFileTXError(int socketID, const QString &contactID, const QByteArray &fileMD5, quint8 errorCode, const QString &errorString);

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
