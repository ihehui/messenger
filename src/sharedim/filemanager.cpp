

#include "filemanager.h"

#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QTimerEvent>
#include <QCryptographicHash>
#include <QSettings>
#include <QDebug>
#include <QDateTime>
#include <QCoreApplication>


#include "HHSharedCore/hcryptography.h"

#ifndef FILE_PIECE_LENGTH
#define FILE_PIECE_LENGTH 10240 //10 KB
#endif

#define SUFFIX_TEMP_FILE ".tf"
#define SUFFIX_INFO_FILE ".fi"




namespace HEHUI {


FileManager::FileManager(QObject *parent)
    : QThread(parent)
{
    m_quit = false;
    readId = 0;
    //    startVerification = false;
    m_wokeUp = false;

    //qsrand(QDateTime::currentDateTime().toTime_t());

    //    qRegisterMetaType<FileManager::Error>("FileManager::Error");
}

FileManager::~FileManager()
{
    m_quit = true;
    cond.wakeOne();
    wait();

    foreach (FileMetaInfo *metaInfo, fileMetaInfoHash) {
        metaInfo->file->close();
        delete metaInfo->file;
        //fileMetaInfoHash.remove(metaInfo->md5sum);
        delete metaInfo;
    }
    fileMetaInfoHash.clear();

}



int FileManager::readPiece(const QByteArray &fileMD5, int pieceIndex)
{
    ReadRequest request;
    //request.fileID = pieceIndex;
    request.fileMD5 = fileMD5;
    request.pieceIndex = pieceIndex;
    //request.length = length;

    QMutexLocker locker(&mutex);
    request.id = readId++;
    readRequests << request;

    if (!m_wokeUp) {
        m_wokeUp = true;
        QMetaObject::invokeMethod(this, "wakeUp", Qt::QueuedConnection);
    }

    return request.id;
}

void FileManager::writePiece(const QByteArray &fileMD5, int pieceIndex, const QByteArray &data, const QByteArray &dataSHA1SUM)
{
    WriteRequest request;
    //request.fileID = fileID;
    request.fileMD5 = fileMD5;
    request.pieceIndex = pieceIndex;
    request.data = data;
    request.dataSHA1SUM = dataSHA1SUM;

    QMutexLocker locker(&mutex);
    writeRequests << request;

    if (!m_wokeUp) {
        m_wokeUp = true;
        QMetaObject::invokeMethod(this, "wakeUp", Qt::QueuedConnection);
    }
}

//void FileManager::verifyPiece(QByteArray fileMD5, int pieceIndex)
//{
//    QMutexLocker locker(&mutex);

//    FileMetaInfo *info = fileMetaInfoHash.value(fileMD5);
//    if(!info){
//        return;
//    }
//    info->pendingVerificationPieces << pieceIndex;

//    if(!pendingVerificationRequests.contains(fileMD5)){
//        pendingVerificationRequests << fileMD5;
//    }

//    startVerification = true;

//    if (!wokeUp) {
//        wokeUp = true;
//        QMetaObject::invokeMethod(this, "wakeUp", Qt::QueuedConnection);
//    }
//}



//QBitArray FileManager::completedPieces(const QByteArray &fileMD5) const
//{
//    QMutexLocker locker(&mutex);

//    FileMetaInfo * info = fileMetaInfoHash.value(fileMD5);
//    if(!info){
//        return QBitArray();
//    }

//    return info->verifiedPieces;
//}

//void FileManager::setCompletedPieces(const QByteArray &fileMD5, const QBitArray &pieces)
//{
//    QMutexLocker locker(&mutex);

//    FileMetaInfo * info = fileMetaInfoHash.value(fileMD5);
//    if(!info){
//        return ;
//    }
//    info->verifiedPieces = pieces;

//}

QList<int/*Piece Index*/> FileManager::completedPieces(const QByteArray &fileMD5){

    QMutexLocker locker(&mutex);

    QList<int> ret;
    FileMetaInfo * info = fileMetaInfoHash.value(fileMD5);
    if(!info){
        return ret;
    }

    for(int i=0; i< info->verifiedPieces.size(); i++){
        if(info->verifiedPieces.testBit(i)){
            ret << i;
        }
    }

    return ret;

}

QList<int/*Piece Index*/> FileManager::uncompletedPieces(const QByteArray &fileMD5){

    QMutexLocker locker(&mutex);

    QList<int> ret;
    FileMetaInfo * info = fileMetaInfoHash.value(fileMD5);
    if(!info){
        return ret;
    }

    for(int i=0; i< info->verifiedPieces.size(); i++){
        if(!info->verifiedPieces.testBit(i)){
            ret << i;
        }
    }

    return ret;

}

int FileManager::getOneUncompletedPiece(const QByteArray &fileMD5){

    QMutexLocker locker(&mutex);

    FileMetaInfo * info = fileMetaInfoHash.value(fileMD5);
    if(!info){
        return -1;
    }

    for(int i=0; i< info->verifiedPieces.size(); i++){
        if(!info->verifiedPieces.testBit(i)){
            return i;
        }
    }

    return -1;

}



//QString FileManager::errorString() const
//{
//    return errString;
//}

void FileManager::run()
{

    do {
        {
            // Go to sleep if there's nothing to do.
            QMutexLocker locker(&mutex);
            if (!m_quit && readRequests.isEmpty() && writeRequests.isEmpty() )
                cond.wait(&mutex);
        }

        // Read pending read requests
        mutex.lock();
        QList<ReadRequest> newReadRequests = readRequests;
        readRequests.clear();
        mutex.unlock();

        while (!newReadRequests.isEmpty()) {
            ReadRequest request = newReadRequests.takeFirst();
            FileMetaInfo *info = fileMetaInfoHash.value(request.fileMD5) ;
            if(!info){continue;}
            if(request.pieceIndex >= info->verifiedPieces.size()){continue;}
            QByteArray block = readBlock(request.id, info, request.pieceIndex);
            if(block.isEmpty()){continue;}
            if(!info->hashSums.contains(request.pieceIndex)){
                QByteArray hashSUM = QCryptographicHash::hash(block, QCryptographicHash::Md5);
                info->hashSums.insert(request.pieceIndex, hashSUM);
//                                qDebug()<<"-------------pieceIndex:"<<request.pieceIndex<<" MD5:"<<hashSUM.toHex();
            }
            //                Q_ASSERT(dataSHA1SUM == info->sha1Sums.value(request.pieceIndex));

            emit dataRead(request.id, request.fileMD5, request.pieceIndex, block, info->hashSums.value(request.pieceIndex));
        }

        // Write pending write requests
        mutex.lock();
        QList<WriteRequest> newWriteRequests = writeRequests;
        writeRequests.clear();
        while (!m_quit && !newWriteRequests.isEmpty()) {
            WriteRequest request = newWriteRequests.takeFirst();
            FileMetaInfo *info = fileMetaInfoHash.value(request.fileMD5) ;
            if(!info){continue;}
            writeBlock(info, request.pieceIndex, request.data, request.dataSHA1SUM);
        }

        // Process pending verification requests
        //        if (startVerification) {
        //            newPendingVerificationRequests = pendingVerificationRequests;
        //            pendingVerificationRequests.clear();
        ////            verifyFileContents();
        //            VerifyAllPendingFiles();
        //            startVerification = false;
        //        }
        mutex.unlock();
        //        newPendingVerificationRequests.clear();

    } while (!m_quit);

    // Write pending write requests
    mutex.lock();
    QList<WriteRequest> newWriteRequests = writeRequests;
    writeRequests.clear();
    mutex.unlock();
    while (!newWriteRequests.isEmpty()) {
        WriteRequest request = newWriteRequests.takeFirst();
        FileMetaInfo *info = fileMetaInfoHash.value(request.fileMD5) ;
        if(!info){continue;}
        writeBlock(info, request.pieceIndex, request.data, request.dataSHA1SUM);
    }
}

//void FileManager::startDataVerification()
//{
//    QMutexLocker locker(&mutex);
//    startVerification = true;
//    cond.wakeOne();
//}

const FileManager::FileMetaInfo * FileManager::tryToSendFile( const QString &localSavePath, QString *errorString/*, int pieceLength*/){

    Q_ASSERT(errorString);

    {
        QMutexLocker locker(&mutex);
        foreach (FileMetaInfo *metaInfo, fileMetaInfoHash) {
            if(metaInfo->file->fileName() == localSavePath){
                //QString errString = tr("File '%1' is in use!").arg(localSavePath);
                //emit error(FILE_READ_ERROR, errString);
                return metaInfo;
            }
        }
    }

    if(!QFile::exists(localSavePath)){
        *errorString = tr("File '%1' does not exist!").arg(localSavePath);
        //emit error(ERROR_FILE_NOT_EXIST, errString);
        return 0;
    }

    QFile *file = new QFile(localSavePath);
    if (!file->open(QFile::ReadOnly)) {
        *errorString = tr("Failed to open file %1! %2").arg(localSavePath).arg(file->errorString());
        //emit error(FILE_READ_ERROR, errString);
        delete file;
        return 0;
    }

    //QByteArray fileMD5Sum = QCryptographicHash::hash(file->readAll(), QCryptographicHash::Md5);
    QByteArray fileMD5Sum = Cryptography::getFileMD5(localSavePath);

    file->seek(0);
    qint64 fileSize = file->size();
    int pieceCount = fileSize / FILE_PIECE_LENGTH;
    if(fileSize % FILE_PIECE_LENGTH){
        pieceCount++;
    }
    //qDebug()<<"----------------pieceCount:"<<pieceCount;

    FileMetaInfo *info = new FileMetaInfo();
    info->md5sum = fileMD5Sum;
    info->size = fileSize;
    //info->pieceLength = pieceLength;
    //    info->sha1Sums = sha1Sums;
    info->verifiedPieces.resize(pieceCount);
    info->verifiedPieces.fill(true);
    //info->newFile = true;
    info->file = file;
    file->close();

    {
        QMutexLocker locker(&mutex);
        fileMetaInfoHash.insert(fileMD5Sum, info);
        cond.wakeOne();
    }

    return info;

}

const FileManager::FileMetaInfo * FileManager::tryToReceiveFile(QByteArray fileMD5Sum, const QString &localSavePath, quint64 size, QString *errorString/*, int pieceLength*/){

    Q_ASSERT(errorString);

    QString tempFilePath = localSavePath + SUFFIX_TEMP_FILE;
    QString infoFilePath = localSavePath + SUFFIX_INFO_FILE;

    QFileInfo fi(localSavePath);
    if(fi.exists()){
        *errorString = tr("File '%1' already exists!").arg(localSavePath);
        //emit error(ERROR_FILE_EXIST, errString);
        return 0;
    }else{
        if(!fi.dir().mkpath(fi.absolutePath())){
            *errorString = tr("Can not create directory '%1'!").arg(fi.absolutePath());
            return 0;
        }
    }

    {
        QMutexLocker locker(&mutex);
        foreach (FileMetaInfo *metaInfo, fileMetaInfoHash) {
            if(metaInfo->file->fileName() == tempFilePath){
                if(metaInfo->md5sum == fileMD5Sum ){
                    return metaInfo;
                }else{
                    *errorString = tr("File '%1' is in use!").arg(localSavePath);
                    //emit error(ERROR_FILE_EXIST, errString);
                    return 0;
                }
            }
        }
    }

    QFile *file = new QFile(tempFilePath);
    if (!file->open(QFile::ReadWrite)) {
        *errorString = tr("Failed to open/create file %1: %2").arg(file->fileName()).arg(file->errorString());
        //emit error(FILE_READ_ERROR, errString);
        delete file;
        return 0;
    }


    QSettings settings(infoFilePath, QSettings::IniFormat);
    QByteArray oldMD5 = settings.value("MD5", QByteArray()).toByteArray();
    quint64 oldSize = settings.value("size", 0).toULongLong();
    if(oldMD5 != fileMD5Sum || oldSize != size || file->size() == 0){
        file->resize(0);
        if (!file->resize(size)) {
            *errorString = tr("Failed to resize file %1: %2").arg(file->fileName()).arg(file->errorString());
            //emit error(FILE_WRITE_ERROR, errString);
            delete file;
            return 0;
        }
    }
    file->close();


    QBitArray ba = settings.value("Pieces", QBitArray()).toBitArray();
    if(ba.isEmpty()){
        int pieces = size / FILE_PIECE_LENGTH;
        if(size % FILE_PIECE_LENGTH){
            pieces++;
        }
        ba.resize(pieces);
        ba.fill(false);
    }

    settings.setValue("MD5", fileMD5Sum);
    settings.setValue("size", size);
    settings.setValue("Pieces", ba);
    settings.sync();

    FileMetaInfo *info = new FileMetaInfo();

    info->md5sum = fileMD5Sum;
    info->size = size;
    //info->pieceLength = pieceLength;
    info->verifiedPieces = ba;
    //info->newFile = true;
    info->file = file;
    info->infoFileName = infoFilePath;

    {
        QMutexLocker locker(&mutex);
        //        while(fileMetaInfoHash.contains(info->fileID)){
        //            info->fileID = qrand();
        //        }
        fileMetaInfoHash.insert(fileMD5Sum, info);
        cond.wakeOne();
    }

    return info;

}

//bool FileManager::setFileMetaInfo(QByteArray fileMD5Sum, int pieceLength, const QList<QByteArray> &sha1Sums){

//    FileMetaInfo *info = fileMetaInfoHash.value(fileMD5Sum) ;
//    if(!info){return false;}

//    info->pieceLength = pieceLength;
//    info->sha1Sums = sha1Sums;

//}


void FileManager::closeFile(const QString &filePath){

    QMutexLocker locker(&mutex);
    foreach (FileMetaInfo *metaInfo, fileMetaInfoHash) {
        if(metaInfo->file->fileName() == filePath){
            metaInfo->file->close();
            delete metaInfo->file;
            fileMetaInfoHash.remove(metaInfo->md5sum);
            delete metaInfo;
            return;
        }
    }

    //errString = tr("File '%1' does not exist!").arg(filePath);


}

void FileManager::closeFile(const QByteArray &fileMD5){

    QMutexLocker locker(&mutex);
    FileMetaInfo *metaInfo = fileMetaInfoHash.take(fileMD5);
    if(metaInfo){
        metaInfo->file->close();
        delete metaInfo->file;
    }

    delete metaInfo;

}

QString FileManager::getFileLocalSavePath(const QByteArray &fileMD5){

    QMutexLocker locker(&mutex);
    FileMetaInfo *metaInfo = fileMetaInfoHash.value(fileMD5);
    if(metaInfo){
        return metaInfo->file->fileName();
    }

    return "";
}

//int FileManager::regenerateFileID(int oldFileID){

//    QMutexLocker locker(&mutex);
//    FileMetaInfo *info = fileMetaInfoHash.take(oldFileID);
//    if(!info){
//        return 0;
//    }


//    int id = info->fileID;

//    while(fileMetaInfoHash.contains(id) || id == 0){
//        id = qrand();
//    }
//    fileMetaInfoHash.insert(id, info);

//    if(pendingVerificationRequests.contains(info->fileID)){
//        pendingVerificationRequests.removeAll(id);
//        pendingVerificationRequests.append(id);
//    }
//    if(newPendingVerificationRequests.contains(info->fileID)){
//        newPendingVerificationRequests.removeAll(id);
//        newPendingVerificationRequests.append(id);
//    }

//    info->fileID = id;

//}

//QFile * FileManager::generateNewFile(quint64 size, const QString &localSavePath)
//{

//    // Set up the thread local data
//    QMutexLocker locker(&mutex);

//    //        QString prefix;
//    //        if (!destinationPath.isEmpty()) {
//    //            prefix = destinationPath;
//    //            if (!prefix.endsWith("/"))
//    //                prefix += "/";
//    //            QDir dir;
//    //            if (!dir.mkpath(prefix)) {
//    //                errString = tr("Failed to create directory %1").arg(prefix);
//    //                emit error();
//    //                return false;
//    //            }
//    //        }
//    QFile *file = new QFile(localSavePath);
//    if (!file->open(QFile::ReadWrite)) {
//        errString = tr("Failed to open/create file %1: %2").arg(file->fileName()).arg(file->errorString());
//        emit error();
//        return 0;
//    }
//    if (file->size() != size) {
//        newFile = true;
//        if (!file->resize(size)) {
//            errString = tr("Failed to resize file %1: %2").arg(file->fileName()).arg(file->errorString());
//            emit error();
//            return 0;
//        }
//    }
//    file->close();

//    return file;
//}

QByteArray FileManager::readBlock(int requestID, FileMetaInfo *info, int pieceIndex){
    qDebug()<<"--FileManager::readBlock(...) "<<" pieceIndex:"<<pieceIndex;

    QByteArray block;

    if(!info){
        QString errString = tr("Failed to read file info!");
        emit error(requestID, info->md5sum, quint8(ERROR_UNKNOWN), errString);
        return block;
    }


    qint64 startReadIndex = (quint64(pieceIndex) * FILE_PIECE_LENGTH);

    QFile *file = info->file;
    qint64 currentFileSize = info->size;
    if (!file->isOpen()) {
        if (!file->open(QFile::ReadOnly)) {
            QString errString = tr("Failed to open file '%1'! %2").arg(file->fileName()).arg(file->errorString());
            emit error(requestID, info->md5sum, quint8(FILE_READ_ERROR), errString);
            return block;
        }
    }

    file->seek(startReadIndex);
    qint64 sizeToRead = qMin<qint64>(FILE_PIECE_LENGTH, currentFileSize - file->pos());
    block = file->read(sizeToRead);
    file->close();

    if (block.size() != sizeToRead) {
        QString errString = tr("Failed to read data from file '%1'! %2").arg(file->fileName()).arg(file->errorString());
        emit error(requestID, info->md5sum, quint8(FILE_READ_ERROR), errString);
        return QByteArray();
    }

    return block;
}

bool FileManager::writeBlock(FileMetaInfo *info, int pieceIndex, const QByteArray &data, const QByteArray &dataHashSUM){
    qDebug()<<"--FileManager::writeBlock(...) "<<"pieceIndex:"<<pieceIndex<<" data size:"<<data.size()<<" MD5:"<<dataHashSUM.toHex();

    if(!info){
        QString errString = tr("Failed to read file info!");
        emit error(0, info->md5sum, quint8(ERROR_UNKNOWN), errString);
        return false;
    }

    QByteArray hashSUM = QCryptographicHash::hash(data, QCryptographicHash::Md5);
    if(hashSUM != dataHashSUM){
        emit pieceVerified(info->md5sum, pieceIndex, false, 0);
        return false;
    }


    qint64 startWriteIndex = (qint64(pieceIndex) * FILE_PIECE_LENGTH);
    int bytesToWrite = data.size();
    //int bytesToWrite = qMin<qint64>(data.size(), info->size - startWriteIndex);

    QFile *file = info->file;
    //qint64 currentFileSize = info->size;

    if (!file->isOpen()) {
        if (!file->open(QFile::ReadWrite)) {
            QString errString = tr("Failed to open file '%1'! %2").arg(file->fileName()).arg(file->errorString());
            emit error(0, info->md5sum, quint8(FILE_READ_ERROR), errString);
            return false;
        }
    }

    file->seek(startWriteIndex);
    qint64 bytesWritten = file->write(data.constData(), bytesToWrite);
    file->flush();
    file->close();

    if (bytesWritten != bytesToWrite) {
        QString errString = tr("Failed to write data to file '%1'! %2").arg(file->fileName()).arg(file->errorString());
        emit error(0, info->md5sum, quint8(FILE_WRITE_ERROR), errString);
        return false;
    }

    info->hashSums.insert(pieceIndex, dataHashSUM);

    //    QByteArray block = readBlock(0, info, pieceIndex);
    //    QByteArray su = QCryptographicHash::hash(block, QCryptographicHash::Sha1);
    //    qDebug()<<"-------------pieceIndex:"<<pieceIndex<<" MD5:"<<su.toBase64()<<" ==?"<<(sha1Sum==su);

    ////////////////////////////////////////

    info->verifiedPieces.setBit(pieceIndex);

    if((pieceIndex % 25) == 0){
        QSettings settings(info->infoFileName, QSettings::IniFormat);
        settings.setValue("Pieces", info->verifiedPieces);
        settings.sync();
    }

    int verificationProgress = (info->verifiedPieces.count(true) * 100) / info->verifiedPieces.size();
    Q_ASSERT(verificationProgress >= 0 && verificationProgress <= 100);

    if(verificationProgress == 100){
        QString name = info->file->fileName();
        Q_ASSERT(name.endsWith(SUFFIX_TEMP_FILE));
        name.remove(SUFFIX_TEMP_FILE);
        if(!info->file->rename(name)){
            qCritical()<<"ERROR! Failed to rename file!";
        }else{
            QFile::remove(info->infoFileName);
            qWarning()<<"File Received!";

            //            info->file->open(QIODevice::ReadOnly);
            //            info->file->seek(0);
            //            QByteArray fileMD5Sum = QCryptographicHash::hash(info->file->readAll(), QCryptographicHash::Md5);
            //            if(fileMD5Sum != info->md5sum){
            //                qCritical()<<"Damaged! MD5:"<<fileMD5Sum.toBase64()<<" info->md5sum:"<<info->md5sum.toBase64();
            //            }
        }
    }

    emit pieceVerified(info->md5sum, pieceIndex, true, verificationProgress);

    return true;


    //return verifySinglePiece(info, pieceIndex);


}

//void FileManager::verifyFileContents(FileMetaInfo *info)
//{

//    if(!info){
//        errString = tr("Failed to read file info!");
//        emit error();
//        return;
//    }

//    // Verify all pieces the first time
////    if (info->pendingVerificationPieces.isEmpty()) {
//        if (info->verifiedPieces.count(true) == 0) {
//            info->verifiedPieces.resize(info->sha1Sums.size());

//            int oldPercent = 0;
//            int numPieces = info->sha1Sums.size();

//            for (int index = 0; index < numPieces; ++index) {
//                verifySinglePiece(info, index);

//                int percent = ((index + 1) * 100) / numPieces;
//                if (oldPercent != percent) {
//                    emit verificationProgress(info->md5sum, percent);
//                    oldPercent = percent;
//                }
//            }

//        }
//        emit verificationDone(info->md5sum);
//        return;
////    }else{
////        foreach (int pieceIndex, info->pendingVerificationPieces) {
////            verifySinglePiece(info, pieceIndex);
////        }
////        info->pendingVerificationPieces.clear();
////    }

//}

//void FileManager::VerifyAllPendingFiles(){

//    // Verify all pending pieces
//    foreach (QByteArray fileMD5, newPendingVerificationRequests){
//        FileMetaInfo *info = fileMetaInfoHash.value(fileMD5);
//        if(!info){
//            errString = tr("Failed to read file info!");
//            emit error();
//            continue;
//        }
//        verifyFileContents(info);

//    }
//    newPendingVerificationRequests.clear();


//}

bool FileManager::verifySinglePiece(FileMetaInfo *info, int pieceIndex)
{

    if(!info){
        return false;
    }

    //qint64 startReadIndex = (quint64(pieceIndex) * FILE_PIECE_LENGTH);

    QByteArray block = readBlock(0, info, pieceIndex);
    QByteArray hashSum = QCryptographicHash::hash(block, QCryptographicHash::Md5);

    bool verified = false;
    if (hashSum != info->hashSums.value(pieceIndex)){
        verified = false;
    }else{
        info->verifiedPieces.setBit(pieceIndex);
        verified = true;
    }

    if((pieceIndex % 10) == 0){
        QSettings settings(info->infoFileName, QSettings::IniFormat);
        settings.setValue("Pieces", info->verifiedPieces);
        settings.sync();
    }

    int verificationProgress = (info->verifiedPieces.count(true) * 100) / info->verifiedPieces.size();
    Q_ASSERT(verificationProgress >= 0 && verificationProgress <= 100);

    if(verificationProgress == 100){
        QString name = info->file->fileName();
        Q_ASSERT(name.endsWith(SUFFIX_TEMP_FILE));
        name.remove(SUFFIX_TEMP_FILE);
        if(!info->file->rename(name)){
            qCritical()<<"ERROR! Failed to rename file!";
        }else{
            //QString cfgFile = name + SUFFIX_INFO_FILE;
            QFile::remove(info->infoFileName);
            QFile::remove(info->infoFileName);
            qWarning()<<"File Received!";
        }
    }

    emit pieceVerified(info->md5sum, pieceIndex, verified, verificationProgress);

    return verified;
}

void FileManager::wakeUp()
{
    QMutexLocker locker(&mutex);
    m_wokeUp = false;
    cond.wakeOne();
}


} //namespace HEHUI
