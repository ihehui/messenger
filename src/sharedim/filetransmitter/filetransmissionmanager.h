#ifndef FILETXMANAGERBASE_H
#define FILETXMANAGERBASE_H

#include <QObject>

#include "filetransmissionpacketsparser.h"

#include "../filemanager.h"

//#include "HHSharedCore/hsingleton.h"

namespace HEHUI {

class FileTransmissionInfo;

class SHAREDIMLIB_API FileTransmissionManagerBase : public QObject//, public Singleton<FileTransmissionManagerBase>
{
    Q_OBJECT
    //friend class Singleton<FileTransmissionManagerBase> ;

public:
    explicit FileTransmissionManagerBase(const QString &myID, FileTransmissionPacketsParserBase *fileTransmissionPacketsParser, QObject *parent = 0);
    ~FileTransmissionManagerBase();

    bool queryFileTransmissionInfo(const QString &peerID, const QByteArray &fileMD5Sum, float *progress, float *speed);
    void setPeerSessionEncryptionKey(const QString &peerID, const QByteArray &encryptionKey);


signals:

    //void signalPeerRequestUploadFile(int socketID, const QString &contactID, const QByteArray &fileMD5Sum, const QString &fileName, quint64 size, const QString &localFileSaveDir);
    void signalPeerCanceledUploadFileRequest(int socketID, const QString &contactID, const QByteArray &fileMD5Sum);

    void signalPeerRequestDownloadFile(int socketID, const QString &contactID, const QString &filePath);
    void signalPeerCanceledDownloadFileRequest(int socketID, const QString &contactID, const QString &fileName);


    void signalFileDownloadRequestAccepted(int socketID, const QString &peerID, const QString &remoteFileName, const QByteArray &fileMD5Sum, quint64 size);
    void signalFileDownloadRequestDenied(int socketID, const QString &contactID, const QString &remoteFileName, const QString &message);
    void signalFileUploadRequestResponsed(int socketID, const QString &peerID, const QByteArray &fileMD5Sum, bool accepted, const QString &message);

    void signalFileTransmissionAborted(int socketID, const QString &peerID, const QByteArray &fileMD5Sum);
    void signalFileTransmissionDone(int socketID, const QString &peerID, const QByteArray &fileMD5Sum);



public slots:

    void requestUploadFilesToPeer(int socketID, const QString &peerID, const QString &localBaseDir, const QStringList &localFiles, const QString &remoteDir);
    void cancelUploadFileRequest(int socketID, const QString &peerID, const QByteArray &fileMD5Sum);

    void requestDownloadFilesFromPeer(int socketID, const QString &peerID, const QString &remoteBaseDir, const QStringList &remoteFiles, const QString &localDir);
    void cancelDownloadFileRequest(int socketID, const QString &peerID, const QString &remoteFileName);

    void acceptFileUploadRequest(int socketID, const QString &peerID, const QByteArray &fileMD5Sum, quint64 size, const QString &localSavePath);
    void declineFileUploadRequest(int socketID, const QString &peerID, const QByteArray &fileMD5Sum);

    void acceptFileDownloadRequest(int socketID, const QString &peerID, const QString &fileName, const QByteArray &fileMD5Sum, quint64 size);
    void declineFileDownloadRequest(int socketID, const QString &peerID, const QString &fileName, const QString &message);

    void abortFileTransmission(int socketID, const QString &peerID, const QByteArray &fileMD5Sum);


private slots:
    void peerDisconnected(const QString &peerID);


    ///////////////////
//    void requestFileSystemInfo(const QString &parentDirPath);
//    void fileSystemInfoReceived(int socketID, const QString &parentDirPath, const QByteArray &fileSystemInfoData);


    void fileDownloadRequestAccepted(int socketID, const QString &peerID, const QString &remoteFileName, const QByteArray &fileMD5Sum, quint64 size);
    void fileDownloadRequestDenied(int socketID, const QString &peerID, const QString &remoteFileName, const QString &message);
    void fileUploadRequestResponsed(int socketID, const QString &peerID, const QByteArray &fileMD5Sum, bool accepted, const QString &message);



    //////////////////////////////
    //File TX
    void startFileManager();
    void stopFileManager();

    virtual void processPeerRequestUploadFilePacket(int socketID, const QString &peerID, const QByteArray &fileMD5Sum, const QString &fileName, quint64 size, const QString &localFileSaveDir) = 0;
    void processPeerCanceledUploadFileRequestPacket(int socketID, const QString &contactID, const QByteArray &fileMD5Sum);

    void processPeerRequestDownloadFilePacket(int socketID, const QString &contactID, const QString &fileName);
    void processPeerCanceledDownloadFileRequestPacket(int socketID, const QString &contactID, const QString &fileName);

    void processFileDataRequestPacket(int socketID, const QString &peerID, const QByteArray &fileMD5, int startPieceIndex, int endPieceIndex);
    void processFileDataReceivedPacket(int socketID, const QString &peerID, const QByteArray &fileMD5, int pieceIndex, const QByteArray &data, const QByteArray &hash);
    void processFileTXStatusChangedPacket(int socketID, const QString &peerID, const QByteArray &fileMD5, quint8 status);
    void processFileTXErrorFromPeer(int socketID, const QString &contactID, const QByteArray &fileMD5, quint8 errorCode, const QString &errorString);

    void fileDataRead(int requestID, const QByteArray &fileMD5, int pieceIndex, const QByteArray &data, const QByteArray &dataHashSUM);
    void fileTXError(int requestID, const QByteArray &fileMD5, quint8 errorCode, const QString &errorString);
    void pieceVerified(const QByteArray &fileMD5, int pieceIndex, bool verified, int verificationProgress);

    void tryToCloseFile(const QString &peerID, const QByteArray &fileMD5 = QByteArray());

    void checkFileTransmissionList();
    void idleTimerTimeout();

    bool isTransmittingFile(const QString &peerID, const QByteArray &fileMD5);
    void initFileTransmissionInfo(int socketID, const QString &peerID, const QByteArray &fileMD5, quint64 size, quint64 pieces);
    bool updateFileTransmissionInfo(const QString &peerID, const QByteArray &fileMD5Sum, int pieceIndex, qint64 dataSize);
    bool updateFileTransmissionInfo(FileTransmissionInfo *info, int pieceIndex, qint64 dataSize);

    FileTransmissionInfo* getFileTransmissionInfo(const QString &peerID, const QByteArray &md5);

private:

    enum FileTXStatus{
        File_TX_Preparing,
        File_TX_Receiving,
        File_TX_Sending,
        //File_TX_Transferring,
        File_TX_Progress,
        File_TX_Paused,
        File_TX_Aborted,
        File_TX_Done
    };


    FileTransmissionPacketsParserBase *m_fileTransmissionPacketsParser;
    QTimer *m_idleTimer;

    ResourcesManager *m_resourcesManager;
    FileManager *m_fileManager;

//    QHash<int/*File TX Request ID*/, int/*Socket ID*/> fileTXRequestHash;
    QHash<int/*File TX Request ID*/, FileTransmissionInfo*> fileTXRequestHash;

//    QMultiHash<int/*Socket ID*/, QByteArray/*File MD5*/> fileTXSocketHash;

    QMultiHash<QByteArray/*File MD5*/, FileTransmissionInfo*> fileTXInfoHash;

    QMultiHash<QString/*Peer ID*/, FileTransmissionInfo*> peerFileTXInfoHash;


};

} //namespace HEHUI

#endif // FILETXMANAGERBASE_H
