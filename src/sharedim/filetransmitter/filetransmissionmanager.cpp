#include "filetransmissionmanager.h"


#include "HHSharedCore/hutilities.h"




namespace HEHUI
{

class FileTransmissionInfo
{
public:

    FileTransmissionInfo(int socketID, const QString &peerID, const QByteArray &md5, quint64 size, quint64 pieces)
    {
        this->socketID = socketID;
        this->peerID = peerID;
        this->fileMd5 = md5;
        this->fileSize = size;
        this->totalPieces = pieces;
        this->curPieceIndex = 0;
        this->timeBase = 0;
        this->transmittedSize = 0;
    }

    int socketID;
    QString peerID;
    QByteArray fileMd5;
    quint64 fileSize;

    //Progress
    quint64 totalPieces;
    int curPieceIndex;

    //Speed
    quint64 timeBase;
    quint64 transmittedSize;
};


FileTransmissionManagerBase::FileTransmissionManagerBase(const QString &myID, FileTransmissionPacketsParserBase *fileTransmissionPacketsParser, QObject *parent) :
    QObject(parent), m_fileTransmissionPacketsParser(fileTransmissionPacketsParser)
{

    if(!m_fileTransmissionPacketsParser) {
        m_fileTransmissionPacketsParser = new FileTransmissionPacketsParserBase(myID, this);
    }
    connect(m_fileTransmissionPacketsParser, SIGNAL(signalFileTransferPacketReceived(const FileTransferPacket &)), this, SLOT(processFileTransferPacket(const FileTransferPacket &)), Qt::QueuedConnection);


    m_idleTimer = 0;

    m_resourcesManager = 0;
    m_fileManager = 0;

//    //File TX
//    connect(m_fileTransmissionPacketsParser, SIGNAL(signalPeerRequestUploadFile(int, const QString &, const QByteArray &, const QString &, quint64, const QString &)), this, SLOT(processPeerRequestUploadFilePacket(int, const QString &, const QByteArray &, const QString &, quint64, const QString &)), Qt::QueuedConnection);
//    connect(m_fileTransmissionPacketsParser, SIGNAL(signalPeerCanceledUploadFileRequest(int, const QString &, const QByteArray &)), this, SLOT(processPeerCanceledUploadFileRequestPacket(int, const QString &, const QByteArray &)), Qt::QueuedConnection);

//    connect(m_fileTransmissionPacketsParser, SIGNAL(signalContactRequestDownloadFile(int, const QString &, const QString &)), this, SLOT(processPeerRequestDownloadFilePacket(int, const QString &, const QString &)), Qt::QueuedConnection);
//    connect(m_fileTransmissionPacketsParser, SIGNAL(signalPeerCanceledDownloadFileRequest(int, const QString &, const QString &)), this, SLOT(processPeerCanceledDownloadFileRequestPacket(int, const QString &, const QString &)), Qt::QueuedConnection);

//    connect(m_fileTransmissionPacketsParser, SIGNAL(signalFileDownloadRequestAccepted(int, const QString &, const QString &, const QByteArray &, quint64)), this, SLOT(fileDownloadRequestAccepted(int, const QString &, const QString &, const QByteArray &, quint64)), Qt::QueuedConnection);
//    connect(m_fileTransmissionPacketsParser, SIGNAL(signalFileDownloadRequestDenied(int, const QString &, const QString &, const QString &)), this, SLOT(fileDownloadRequestDenied(int, const QString &, const QString &, const QString &)), Qt::QueuedConnection);
//    connect(m_fileTransmissionPacketsParser, SIGNAL(signalFileUploadRequestResponsed(int, const QString &, const QByteArray &, bool, const QString &)), this, SLOT(fileUploadRequestResponsed(int, const QString &, const QByteArray &, bool, const QString &)), Qt::QueuedConnection);

//    connect(m_fileTransmissionPacketsParser, SIGNAL(signalFileDataRequested(int, const QString &, const QByteArray &, int, int )), this, SLOT(processFileDataRequestPacket(int, const QString &, const QByteArray &, int, int )), Qt::QueuedConnection);
//    connect(m_fileTransmissionPacketsParser, SIGNAL(signalFileDataReceived(int, const QString &, const QByteArray &, int, const QByteArray &, const QByteArray &)), this, SLOT(processFileDataReceivedPacket(int, const QString &, const QByteArray &, int, const QByteArray &, const QByteArray &)), Qt::QueuedConnection);
//    connect(m_fileTransmissionPacketsParser, SIGNAL(signalFileTXStatusChanged(int, const QString &, const QByteArray &, quint8)), this, SLOT(processFileTXStatusChangedPacket(int, const QString &, const QByteArray &, quint8)), Qt::QueuedConnection);
//    connect(m_fileTransmissionPacketsParser, SIGNAL(signalFileTXError(int, const QString &, const QByteArray &, quint8 , const QString &)), this, SLOT(processFileTXErrorFromPeer(int, const QString &, const QByteArray &, quint8 , const QString &)), Qt::QueuedConnection);

//    connect(m_fileTransmissionPacketsParser, SIGNAL(signalPeerDisconnected(const QString &)), this, SLOT(peerDisconnected(const QString &)), Qt::QueuedConnection);


}

FileTransmissionManagerBase::~FileTransmissionManagerBase()
{

    if(m_idleTimer){
        m_idleTimer->stop();
        delete m_idleTimer;
        m_idleTimer = 0;
    }

    if(m_resourcesManager){
        delete m_resourcesManager;
        m_resourcesManager = 0;
    }

    if(m_fileManager){
        delete m_fileManager;
        m_fileManager = 0;
    }


    foreach (FileTransmissionInfo *info, fileTXInfoHash.values()) {
        delete info;
        info = 0;
    }
    fileTXInfoHash.clear();
    peerFileTXInfoHash.clear();


}

bool FileTransmissionManagerBase::queryFileTransmissionInfo(const QString &peerID, const QByteArray &fileMD5Sum, float *progress, float *speed)
{

    QList<FileTransmissionInfo *>  infoList = peerFileTXInfoHash.values(peerID);
    foreach (FileTransmissionInfo *info, infoList) {
        if(info->fileMd5 != fileMD5Sum) {
            continue;
        }

        if(progress) {
            *progress = (float)info->curPieceIndex / (info->totalPieces);
        }
        if(speed) {
            quint64 curTime = Utilities::timeGet();
            Q_ASSERT(curTime > info->timeBase);

            *speed = (float)info->transmittedSize / (curTime - info->timeBase);
            info->transmittedSize = 0;
            info->timeBase = curTime;
        }

        return true;
    }

    return false;

}

void FileTransmissionManagerBase::setPeerSessionEncryptionKey(const QString &peerID, const QByteArray &encryptionKey)
{
    m_fileTransmissionPacketsParser->setPeerSessionEncryptionKey(peerID, encryptionKey);
}

void FileTransmissionManagerBase::processFileTransferPacket(const FileTransferPacket &packet)
{
    FileTransferPacket::PacketInfoType infoType = FileTransferPacket::PacketInfoType(packet.getPacketSubType());
    switch (infoType) {
    case FileTransferPacket::FT_FILE_SERVER_INFO: {
        QString address = packet.FileServerInfo.address;
        quint16 port = packet.FileServerInfo.port;
        //TODO

    }
    break;

    case FileTransferPacket::FT_FileSystemInfoRequest: {
        //in >> FileSystemInfoRequest.parentDirPath;
    }
    break;

    case FileTransferPacket::FT_FileSystemInfoResponse: {
        //in >> FileSystemInfoResponse.baseDirPath >> FileSystemInfoResponse.fileSystemInfoData;
    }
    break;

    case FileTransferPacket::FT_FileDeletingRequest: {
        //in >> FileDeletingRequest.baseDirPath >> FileDeletingRequest.files;
    }
    break;

    case FileTransferPacket::FT_FileDeletingResponse: {
        //in >> FileDeletingResponse.baseDirPath >> FileDeletingResponse.failedFiles;
    }
    break;

    case FileTransferPacket::FT_FileRenamingRequest: {
        //in >> FileRenamingRequest.baseDirPath >> FileRenamingRequest.oldFileName >> FileRenamingRequest.newFileName;
    }
    break;

    case FileTransferPacket::FT_FileRenamingResponse: {
        //in >> FileRenamingResponse.baseDirPath >> FileRenamingResponse.oldFileName >> FileRenamingResponse.renamed >> FileRenamingResponse.message;
    }
    break;

    case FileTransferPacket::FileTransferPacket::FT_FileDownloadingRequest: {
        //in >> FileDownloadingRequest.baseDir >> FileDownloadingRequest.fileName >> FileDownloadingRequest.dirToSaveFile;
    }
    break;

    case FileTransferPacket::FT_FileDownloadingResponse: {
        //in >> FileDownloadingResponse.accepted >> FileDownloadingResponse.baseDir >> FileDownloadingResponse.fileName >> FileDownloadingResponse.fileMD5Sum >> FileDownloadingResponse.size >> FileDownloadingResponse.errorCode;
    }
    break;

    case FileTransferPacket::FT_FileUploadingRequest: {
        //in >> FileUploadingRequest.fileName >> FileUploadingRequest.fileMD5Sum >> FileUploadingRequest.size >> FileUploadingRequest.fileSaveDir;
    }
    break;

    case FileTransferPacket::FT_FileUploadingResponse: {
        //in >> FileUploadingResponse.accepted >> FileUploadingResponse.fileMD5Sum >> FileUploadingResponse.message;
    }
    break;

    case FileTransferPacket::FT_FileDataRequest: {
        //in >> FileDataRequest.fileMD5 >> FileDataRequest.startPieceIndex >> FileDataRequest.endPieceIndex;
    }
    break;

    case FileTransferPacket::FT_FileData: {
        //in >> FileDataResponse.fileMD5 >> FileDataResponse.pieceIndex >> FileDataResponse.data >> FileDataResponse.pieceMD5;
    }
    break;

    case FileTransferPacket::FT_FileTXStatus: {
        //in >> FileTXStatus.fileMD5 >> FileTXStatus.status;
    }
    break;

    case FileTransferPacket::FT_FileTXError: {
        //in >> FileTXError.fileName >> FileTXError.fileMD5 >> FileTXError.errorCode >> FileTXError.message;
    }
    break;

    default:
        break;
    }


}

void FileTransmissionManagerBase::requestUploadFilesToPeer(int socketID, const QString &peerID, const QString &localBaseDir, const QStringList &localFiles, const QString &remoteDir)
{

    if(socketID == INVALID_SOCK_ID) {
        //TODO:
        return;
    }


    startFileManager();

    foreach (QString localFileName, localFiles) {
        QFileInfo fi(localBaseDir, localFileName);
        if(fi.isDir()) {
            QStringList newFiles;
            QDir dir(fi.absoluteFilePath());

            QStringList filters;
            filters << "*" << "*.*";
            foreach(QString file, dir.entryList(filters, QDir::Dirs | QDir::Files | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot)) {
                newFiles.append(file);
                //qApp->processEvents();
            }

            if(!newFiles.isEmpty()) {
                QString newRemoteDir = remoteDir + "/" + localFileName;
                if(remoteDir.endsWith("/") || remoteDir.endsWith("\\") ) {
                    newRemoteDir = remoteDir + localFileName;
                }
                requestUploadFilesToPeer(socketID, peerID, fi.absoluteFilePath(), newFiles, newRemoteDir);
            }
            continue;
        }

        QString errorString;
        const FileManager::FileMetaInfo *info = m_fileManager->tryToSendFile(fi.absoluteFilePath(), &errorString);
        if(!info) {
            qCritical() << tr("ERROR! Can not send file request! %1").arg(errorString);
            continue ;
        }

        bool ok = m_fileTransmissionPacketsParser->requestUploadFile(socketID, info->md5sum, fi.fileName(), info->size, remoteDir);
        if(!ok) {
            tryToCloseFile(peerID, info->md5sum);
            qCritical() << tr("ERROR! Can not send file! %1").arg(m_fileTransmissionPacketsParser->lastErrorMessage());
            continue ;
        } else {
            initFileTransmissionInfo(socketID, peerID, info->md5sum, info->size, info->hashSums.size());
            qDebug() << QString("Request uploading file %1").arg(localFileName);
        }

    }

}

void FileTransmissionManagerBase::cancelUploadFileRequest(int socketID, const QString &peerID, const QByteArray &fileMD5Sum)
{

    if(socketID == INVALID_SOCK_ID) {
        //TODO:
        return;
    }

    m_fileTransmissionPacketsParser->stopFileTX(socketID, "", fileMD5Sum);
    tryToCloseFile(peerID, fileMD5Sum);
}

void FileTransmissionManagerBase::requestDownloadFilesFromPeer(int socketID, const QString &peerID, const QString &remoteBaseDir, const QStringList &remoteFiles, const QString &localDir)
{

    if(socketID == INVALID_SOCK_ID) {
        //TODO:
        qCritical() << "ERROR! Invalid socket!";
        return;
    }

    startFileManager();

    foreach (QString remoteFileName, remoteFiles) {
        bool ok = m_fileTransmissionPacketsParser->requestDownloadFile(socketID, peerID, remoteBaseDir + QDir::separator() + remoteFileName, localDir);
        if(!ok) {
            qCritical() << tr("ERROR! Can not send file download request!");
            continue ;
        } else {
            qDebug() << QString("Request downloading file %1").arg(remoteFileName);
        }
    }

}

void FileTransmissionManagerBase::cancelDownloadFileRequest(int socketID, const QString &peerID, const QString &remoteFileName)
{

    if(socketID == INVALID_SOCK_ID) {
        //TODO:
        return;
    }

    m_fileTransmissionPacketsParser->stopFileTX(socketID, remoteFileName, QByteArray());
}

void FileTransmissionManagerBase::acceptFileUploadRequest(int socketID, const QString &peerID, const QByteArray &fileMD5Sum, quint64 size, const QString &localSavePath)
{

    if(socketID == INVALID_SOCK_ID) {
        //TODO:
        qCritical() << "ERROR! Invalid socket!";
        return;
    }


    startFileManager();

    QString errorString;
    const FileManager::FileMetaInfo *info = m_fileManager->tryToReceiveFile(fileMD5Sum, localSavePath, size, &errorString);
    if(!info) {
        //TODO
        m_fileTransmissionPacketsParser->responseFileUploadRequest(socketID, false, fileMD5Sum, errorString);
        qCritical() << QString("ERROR! Can not upload file '%1'! %2").arg(localSavePath).arg(errorString);
        return;
    }

    if(!m_fileTransmissionPacketsParser->requestFileData(socketID, fileMD5Sum, 0, 0)) {
        tryToCloseFile(peerID, fileMD5Sum);
        return;
    }

    initFileTransmissionInfo(socketID, peerID, info->md5sum, info->size, info->hashSums.size());

}

void FileTransmissionManagerBase::declineFileUploadRequest(int socketID, const QString &peerID, const QByteArray &fileMD5Sum)
{
    if(socketID == INVALID_SOCK_ID) {
        //TODO:
        qCritical() << "ERROR! Invalid socket!";
        return;
    }


    m_fileTransmissionPacketsParser->responseFileUploadRequest(socketID, false, fileMD5Sum,  "");
}

void FileTransmissionManagerBase::acceptFileDownloadRequest(int socketID, const QString &peerID, const QString &fileName, const QByteArray &fileMD5Sum, quint64 size)
{

    if(socketID == INVALID_SOCK_ID) {
        qCritical() << "ERROR! Invalid socket!";
        return;
    }

    startFileManager();

    QString errorString;
    const FileManager::FileMetaInfo *info = m_fileManager->tryToSendFile(fileName, &errorString);
    if(!info) {
        qCritical() << tr("ERROR! Can not send file! %1").arg(errorString);
        m_fileTransmissionPacketsParser->responseFileDownloadRequest(socketID, false, fileName);
        return ;
    }

    bool ok = m_fileTransmissionPacketsParser->responseFileDownloadRequest(socketID, true, fileName, info->md5sum, info->size);
    if(!ok) {
        tryToCloseFile(peerID, info->md5sum);
        qCritical() << tr("ERROR! Can not send file! %1").arg(m_fileTransmissionPacketsParser->lastErrorMessage());
        return ;
    } else {
        initFileTransmissionInfo(socketID, peerID, info->md5sum, info->size, info->hashSums.size());
        qDebug() << QString("Uploading file %1").arg(fileName);
    }

}

void FileTransmissionManagerBase::declineFileDownloadRequest(int socketID, const QString &peerID, const QString &fileName)
{

    if(socketID == INVALID_SOCK_ID) {
        //TODO:
        qCritical() << "ERROR! Invalid socket!";
        return;
    }
    bool ok = m_fileTransmissionPacketsParser->responseFileDownloadRequest(socketID, true, fileName);
}

void FileTransmissionManagerBase::abortFileTransmission(int socketID, const QString &peerID, const QByteArray &fileMD5Sum)
{



    m_fileTransmissionPacketsParser->fileTXStatusChanged(socketID, fileMD5Sum, quint8(File_TX_Aborted));

    tryToCloseFile(peerID, fileMD5Sum);

}

void FileTransmissionManagerBase::peerDisconnected(const QString &peerID)
{

    tryToCloseFile(peerID);


    //TODO

}


////////////////////////////////////////

//void FileTransmissionManagerBase::requestFileSystemInfo(const QString &parentDirPath){

//    if(!m_fileTransmissionPacketsParser->requestFileSystemInfo(m_peerSocket, parentDirPath)){
//        ui.textEditLogs->append(tr("Error! Can not send request! %1").arg(m_fileTransmissionPacketsParser->lastErrorMessage()));
//    }

//}

//void FileTransmissionManagerBase::fileSystemInfoReceived(int socketID, const QString &parentDirPath, const QByteArray &fileSystemInfoData){

//    if(socketID != m_peerSocket){
//        return;
//    }

////    FileTransmissionManagerBase *fm = qobject_cast<FileTransmissionManagerBase *>(ui.tabFileTransmissionManagerBase);
////    if(!fm){return;}

//    parseRemoteFilesInfo(parentDirPath, fileSystemInfoData);

//}



void FileTransmissionManagerBase::fileDownloadRequestAccepted(int socketID, const QString &peerID, const QString &remoteFileName, const QByteArray &fileMD5Sum, quint64 size)
{

    if(socketID == INVALID_SOCK_ID) {
        //TODO:
        qCritical() << "ERROR! Invalid socket!";
        return;
    }



    if(isTransmittingFile(peerID, fileMD5Sum)) {
        return;
    }


    startFileManager();

    QString localPath = "./" + remoteFileName;

    QString errorString;
    const FileManager::FileMetaInfo *info = m_fileManager->tryToReceiveFile(fileMD5Sum, localPath, size, &errorString);
    if(!info) {
        //TODO
        qCritical() << tr("ERROR! Failed to download file '%1'!").arg(remoteFileName);
        return;
    }

    if(!m_fileTransmissionPacketsParser->requestFileData(socketID, fileMD5Sum, 0, 0)) {
        tryToCloseFile(peerID, fileMD5Sum);
        qCritical() << tr("ERROR! Failed to download file '%1'! %2 ").arg(remoteFileName).arg(m_fileTransmissionPacketsParser->lastErrorMessage());
        return;
    }

    initFileTransmissionInfo(socketID, peerID, fileMD5Sum, size, info->hashSums.size());

    emit signalFileDownloadRequestAccepted(socketID, peerID, remoteFileName, fileMD5Sum, size);

}

void FileTransmissionManagerBase::fileDownloadRequestDenied(int socketID, const QString &peerID, const QString &remoteFileName, const QString &message)
{
    //TODO

    emit signalFileDownloadRequestDenied(socketID, peerID, remoteFileName, message);
}

void FileTransmissionManagerBase::fileUploadRequestResponsed(int socketID, const QString &peerID, const QByteArray &fileMD5Sum, bool accepted, const QString &message)
{

    Q_ASSERT(m_fileManager);
    if(accepted) {
        //TODO

    } else {
        tryToCloseFile(peerID, fileMD5Sum);
        qCritical() << tr("ERROR! Can not send file! %1").arg(message);
    }

    emit signalFileUploadRequestResponsed(socketID, peerID, fileMD5Sum, accepted, message);

}


void FileTransmissionManagerBase::startFileManager()
{

    if(!m_fileManager) {
        m_resourcesManager = new ResourcesManager(this);
        m_fileManager = m_resourcesManager->getFileManager();
        connect(m_fileManager, SIGNAL(dataRead(int , const QByteArray &, int , const QByteArray &, const QByteArray &)), this, SLOT(fileDataRead(int , const QByteArray &, int , const QByteArray &, const QByteArray &)), Qt::QueuedConnection);
        connect(m_fileManager, SIGNAL(error(int , const QByteArray &, quint8, const QString &)), this, SLOT(fileTXError(int , const QByteArray &, quint8, const QString &)), Qt::QueuedConnection);
        connect(m_fileManager, SIGNAL(pieceVerified(const QByteArray &, int , bool , int )), this, SLOT(pieceVerified(const QByteArray &, int , bool , int )), Qt::QueuedConnection);

    }

    checkFileTransmissionList();

}

void FileTransmissionManagerBase::stopFileManager()
{
    m_resourcesManager->stopFileManager();
}

//void FileTransmissionManagerBase::processPeerRequestUploadFilePacket(int socketID, const QString &peerID, const QByteArray &fileMD5Sum, const QString &fileName, quint64 size, const QString &localFileSaveDir){

//    //Contact *contact = m_contactsManager->getUser(contactID);
//    //if(!contact){return;}

//    //TODO
//    //emit signalPeerRequestUploadFile(socketID, contactID, fileMD5Sum, fileName, size, localFileSaveDir);
//}

void FileTransmissionManagerBase::processPeerCanceledUploadFileRequestPacket(int socketID, const QString &contactID, const QByteArray &fileMD5Sum)
{

    emit signalPeerCanceledUploadFileRequest(socketID, contactID, fileMD5Sum);
}

void FileTransmissionManagerBase::processPeerRequestDownloadFilePacket(int socketID, const QString &contactID, const QString &fileName)
{


    emit signalPeerRequestDownloadFile(socketID, contactID, fileName);

//    startFileManager();

//    QString errorString;

//    QFileInfo fi(localBaseDir, fileName);
//    QString absoluteFilePath = fi.absoluteFilePath();
//    if(fi.isDir()){
//        QDir dir(absoluteFilePath);

//        QStringList filters;
//        filters << "*" << "*.*";

//        foreach(QString file, dir.entryList(filters, QDir::Dirs | QDir::Files | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot))
//        {
//            QString newRemoteDir = remoteFileSaveDir + "/" + fileName;
//            if(remoteFileSaveDir.endsWith('/')){
//                newRemoteDir = remoteFileSaveDir + fileName;
//            }
//            processContactRequestDownloadFilePacket(socketID, contactID, absoluteFilePath, file, newRemoteDir);

//            qApp->processEvents();
//        }

//        return;
//    }

//    const FileManager::FileMetaInfo *info = m_fileManager->tryToSendFile(absoluteFilePath, &errorString);
//    if(!info){
//        m_fileTransmissionPacketsParser->denyFileDownloadRequest(socketID, contactID, fileName, false, errorString);
//    }

//    if(m_fileTransmissionPacketsParser->acceptFileDownloadRequest(socketID, contactID, fileName, true, info->md5sum, info->size, remoteFileSaveDir)){
//        fileTXSocketHash.insertMulti(socketID, info->md5sum);
//    }else{
//        m_fileManager->closeFile(info->md5sum);
//    }

}

void FileTransmissionManagerBase::processFileDataRequestPacket(int socketID, const QString &peerID, const QByteArray &fileMD5, int startPieceIndex, int endPieceIndex)
{

//    if(!isTransmittingFile(socketID, fileMD5)){
//        return;
//    }


    Q_ASSERT(m_fileManager);

    FileTransmissionInfo *info = getFileTransmissionInfo(peerID, fileMD5);
    if(!info) {
        return;
    }


    if( (startPieceIndex == -1) && (endPieceIndex == -1) ) {
        QList<int> completedPieces = m_fileManager->completedPieces(fileMD5);
        foreach (int pieceIndex, completedPieces) {
            fileTXRequestHash.insert(m_fileManager->readPiece(fileMD5, pieceIndex), info);
            //QCoreApplication::processEvents();
        }

    } else {
        Q_ASSERT(endPieceIndex >= startPieceIndex);
        for(int i = startPieceIndex; i <= endPieceIndex; i++) {
            fileTXRequestHash.insert(m_fileManager->readPiece(fileMD5, i), info);
            //QCoreApplication::processEvents();
        }

    }

    //    int id = m_fileManager->readPiece(fileMD5, pieceIndex);
    //    fileTXRequestHash.insert(id, socketID);

}

void FileTransmissionManagerBase::processFileDataReceivedPacket(int socketID, const QString &peerID, const QByteArray &fileMD5, int pieceIndex, const QByteArray &data, const QByteArray &hash)
{

    if(!isTransmittingFile(peerID, fileMD5)) {
        return;
    }


    Q_ASSERT(m_fileManager);
    m_fileManager->writePiece(fileMD5, pieceIndex, data, hash);

    updateFileTransmissionInfo(peerID, fileMD5, pieceIndex, data.size());

}

void FileTransmissionManagerBase::processFileTXStatusChangedPacket(int socketID, const QString &peerID, const QByteArray &fileMD5, quint8 status)
{

    //IM::FileTXStatus status = IM::FileTXStatus(status);
    switch(status) {
    case quint8(IM::File_TX_Preparing): {

    }
    break;
    case quint8(IM::File_TX_Receiving): {

    }
    break;
    case quint8(IM::File_TX_Sending): {

    }
    break;
    case quint8(IM::File_TX_Progress): {

    }
    break;
    case quint8(IM::File_TX_Paused): {

    }
    break;
    case quint8(IM::File_TX_Aborted): {
        emit signalFileTransmissionAborted(socketID, peerID, fileMD5);
        tryToCloseFile(peerID, fileMD5);

    }
    break;
    case quint8(IM::File_TX_Done): {
        emit signalFileTransmissionDone(socketID, peerID, fileMD5);
        tryToCloseFile(peerID, fileMD5);
    }
    break;
    default:
        break;
    }

}

void FileTransmissionManagerBase::processFileTXErrorFromPeer(int socketID, const QString &contactID, const QByteArray &fileMD5, quint8 errorCode, const QString &errorString)
{
    qDebug() << "--MainWindow::processFileTXErrorFromPeer(...) " << " contactID:" << contactID << " fileMD5:" << fileMD5;
    qCritical() << errorString;

}

void FileTransmissionManagerBase::fileDataRead(int requestID, const QByteArray &fileMD5, int pieceIndex, const QByteArray &data, const QByteArray &dataHashSUM)
{
    qDebug() << "--MainWindow::fileDataRead(...) " << " pieceIndex:" << pieceIndex << " size:" << data.size();

    //int socketID = fileTXRequestHash.take(requestID);

    FileTransmissionInfo *info = fileTXRequestHash.take(requestID);
    if(!info) {
        return;
    }
    int socketID = info->socketID;
    if(socketID == INVALID_SOCK_ID) {
        return;
    }

    m_fileTransmissionPacketsParser->sendFileData(socketID, fileMD5, pieceIndex, &data, &dataHashSUM);

//    updateFileTransmissionInfo(socketID, fileMD5, pieceIndex, data.size());
    updateFileTransmissionInfo(info, pieceIndex, data.size());

}

void FileTransmissionManagerBase::fileTXError(int requestID, const QByteArray &fileMD5, quint8 errorCode, const QString &errorString)
{
    qCritical() << errorString;

    FileTransmissionInfo *info = fileTXRequestHash.take(requestID);
    if(!info) {
        return;
    }
    int socketID = info->socketID;
    if(socketID == INVALID_SOCK_ID) {
        return;
    }

    m_fileTransmissionPacketsParser->fileTXError(socketID, fileMD5, errorCode, errorString);

}

void FileTransmissionManagerBase::pieceVerified(const QByteArray &fileMD5, int pieceIndex, bool verified, int verificationProgress)
{
    qDebug() << "--MainWindow::pieceVerified(...) " << " pieceIndex:" << pieceIndex << " verified:" << verified << "verificationProgress:" << verificationProgress;

    FileTransmissionInfo *info = fileTXInfoHash.value(fileMD5);
    if(info) {
        qCritical() << "ERROR!";
        return;
    }

    if(verified) {

        if(verificationProgress == 100) {
            qWarning() << "Done!";
            m_fileTransmissionPacketsParser->fileTXStatusChanged(info->socketID, fileMD5, quint8(IM::File_TX_Done));
        } else {
            //TODO:
            //            int uncompletedPieceIndex = m_fileManager->getOneUncompletedPiece(fileMD5);
            //            qDebug()<<"uncompletedPieceIndex:"<<uncompletedPieceIndex;
            //            if(uncompletedPieceIndex < 0){
            //                return;
            //            }
            //            m_fileTransmissionPacketsParser->requestFileData(sockets.first(), fileMD5, uncompletedPieceIndex);


            //if((pieceIndex % FILE_PIECES_IN_ONE_REQUEST) == 0){
            //    qDebug()<<"----0----pieceIndex:"<<pieceIndex;
            //    m_fileTransmissionPacketsParser->requestFileData(sockets.first(), fileMD5, pieceIndex + 1, pieceIndex + FILE_PIECES_IN_ONE_REQUEST);
            //}

            if((pieceIndex % FILE_PIECES_IN_ONE_REQUEST) == 0) {
                if(pieceIndex == 0 ) {
                    m_fileTransmissionPacketsParser->requestFileData(info->socketID, fileMD5, 1, 2 * FILE_PIECES_IN_ONE_REQUEST);
                } else {
                    m_fileTransmissionPacketsParser->requestFileData(info->socketID, fileMD5, pieceIndex + FILE_PIECES_IN_ONE_REQUEST + 1, pieceIndex + 2 * FILE_PIECES_IN_ONE_REQUEST);
                }
            }

        }

    } else {
        qCritical() << "ERROR! Verification Failed! Piece:" << pieceIndex;
        m_fileTransmissionPacketsParser->requestFileData(info->socketID, fileMD5, pieceIndex, pieceIndex);
    }


}

void FileTransmissionManagerBase::processPeerCanceledDownloadFileRequestPacket(int socketID, const QString &contactID, const QString &fileName)
{

    emit signalPeerCanceledDownloadFileRequest(socketID, contactID, fileName);
}

void FileTransmissionManagerBase::tryToCloseFile(const QString &peerID, const QByteArray &fileMD5)
{

    QList<FileTransmissionInfo *> infoList = peerFileTXInfoHash.values(peerID);
    foreach (FileTransmissionInfo *info, infoList) {
        if(!fileMD5.isEmpty() && (info->fileMd5 != fileMD5)) {
            continue;
        }

        peerFileTXInfoHash.remove(peerID, info);
        fileTXInfoHash.remove(fileMD5, info);

        QList<int> requestList = fileTXRequestHash.keys(info);
        foreach (int request, requestList) {
            fileTXRequestHash.remove(request);
        }

        delete info;
        info = 0;

        if(infoList.size() == 1) {
            m_fileManager->closeFile(fileMD5);
        }
    }

}

void FileTransmissionManagerBase::checkFileTransmissionList()
{
    if(!fileTXInfoHash.isEmpty()) {
        return;
    }

    if(!m_idleTimer) {
        m_idleTimer = new QTimer(this);
        connect(m_idleTimer, SIGNAL(timeout()), this, SLOT(idleTimerTimeout()));
    }

    m_idleTimer->start(300000);

}

void FileTransmissionManagerBase::idleTimerTimeout()
{

    if(!fileTXInfoHash.isEmpty()) {
        return;
    }

    stopFileManager();

    //TODO
    m_idleTimer->stop();
    delete m_idleTimer;
    m_idleTimer = 0;

}

bool FileTransmissionManagerBase::isTransmittingFile(const QString &peerID, const QByteArray &fileMD5)
{
    QList<FileTransmissionInfo *> infoList = peerFileTXInfoHash.values(peerID);
    foreach (FileTransmissionInfo *info, infoList) {
        if(info->fileMd5 == fileMD5) {
            return true;
        }
    }

    return false;
}

void FileTransmissionManagerBase::initFileTransmissionInfo(int socketID, const QString &peerID, const QByteArray &fileMD5, quint64 size, quint64 pieces)
{

    FileTransmissionInfo *ftInfo = new FileTransmissionInfo(socketID, peerID, fileMD5, size, pieces);
    ftInfo->timeBase = Utilities::timeGet();
    fileTXInfoHash.insert(fileMD5, ftInfo);
    peerFileTXInfoHash.insert(peerID, ftInfo);

}

bool FileTransmissionManagerBase::updateFileTransmissionInfo(const QString &peerID, const QByteArray &fileMD5Sum, int pieceIndex, qint64 dataSize)
{


    QList<FileTransmissionInfo *>  infoList = peerFileTXInfoHash.values(peerID);
    foreach (FileTransmissionInfo *info, infoList) {
        if(info->fileMd5 != fileMD5Sum) {
            continue;
        }

        info->curPieceIndex = pieceIndex;
        info->transmittedSize += dataSize;

        return true;
    }

    return false;

}

bool FileTransmissionManagerBase::updateFileTransmissionInfo(FileTransmissionInfo *info, int pieceIndex, qint64 dataSize)
{
    if(!info) {
        return false;
    }

    info->curPieceIndex = pieceIndex;
    info->transmittedSize += dataSize;

    return true;
}

FileTransmissionInfo *FileTransmissionManagerBase::getFileTransmissionInfo(const QString &peerID, const QByteArray &md5)
{

    QList<FileTransmissionInfo *> infoList = peerFileTXInfoHash.values(peerID);
    foreach (FileTransmissionInfo *info, infoList) {
        if(info->fileMd5 == md5) {
            return info;
        }
    }

    return 0;
}



} //namespace HEHUI
