
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QBitArray>
#include <QHash>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>



#include "sharedimlib.h"


QT_BEGIN_NAMESPACE
class QByteArray;
class QFile;
class QTimerEvent;
QT_END_NAMESPACE


namespace HEHUI {

class SHAREDIMLIB_API FileManager : public QThread
{
    Q_OBJECT

public:
    enum Error{ERROR_NO_ERROR = 0, ERROR_UNKNOWN, ERROR_FILE_EXIST, ERROR_FILE_NOT_EXIST, ERROR_FILE_IN_USE, FILE_READ_ERROR, FILE_WRITE_ERROR};
    struct FileMetaInfo
    {
        FileMetaInfo(){
            //fileID = qrand();
            md5sum.resize(0);
            size = 0;
            //name = "";
            //newFile = false;
            //pieceLength = FILE_PIECE_LENGTH;
            hashSums.clear();
            //verifiedPieces.fill(false);
            file = 0;
            infoFileName = "";
            //pendingVerificationPieces.clear();
        }

        //int fileID;
        QByteArray md5sum;
        quint64 size;
        //QString name;
        //bool newFile;
        //int pieceLength;
        QHash<int/*Piece Index*/, QByteArray/*SHA1 Hash*/> hashSums;
        QBitArray verifiedPieces;

        QFile *file;
        QString infoFileName;
        //QString destinationPath;
        //QList<int/*Piece index*/> pendingVerificationPieces;
    };

    FileManager(QObject *parent = 0);
    virtual ~FileManager();

    const FileMetaInfo * tryToSendFile(const QString &localSavePath, QString *errorString/*, int pieceLength = FILE_PIECE_LENGTH*/);
    const FileMetaInfo * tryToReceiveFile(QByteArray fileMD5Sum, const QString &localSavePath, quint64 size, QString *errorString/*, int pieceLength = FILE_PIECE_LENGTH*/);

    int readPiece(const QByteArray &fileMD5, int pieceIndex);
    void writePiece(const QByteArray &fileMD5, int pieceIndex, const QByteArray &data, const QByteArray &dataSHA1SUM);
    //    void verifyPiece(QByteArray fileMD5, int pieceIndex);

    //    QBitArray completedPieces(const QByteArray &fileMD5) const;
    //    void setCompletedPieces(const QByteArray &fileMD5, const QBitArray &pieces);

    QList<int/*Piece Index*/> completedPieces(const QByteArray &fileMD5);
    QList<int/*Piece Index*/> uncompletedPieces(const QByteArray &fileMD5);
    int getOneUncompletedPiece(const QByteArray &fileMD5);

    //    QByteArray calcFileMD5(const QString &filePath);

    void closeFile(const QString &filePath);
    void closeFile(const QByteArray &fileMD5);

    QString getFileLocalSavePath(const QByteArray &fileMD5);

    //    int regenerateFileID(int oldFileID);
    //    QFile *generateNewFile(quint64 size, const QString &localSavePath);

    //    QString errorString() const;

public slots:
    //    void startDataVerification();

signals:
    void dataRead(int requestID, const QByteArray &fileMD5, int pieceIndex, const QByteArray &data, const QByteArray &dataSHA1SUM);
    void error(int requestID, const QByteArray &fileMD5, quint8 errorCode, const QString &errorString);
    //    void verificationProgress(QByteArray fileMD5, int percent);
    //    void verificationDone(QByteArray fileMD5);
    void pieceVerified(const QByteArray &fileMD5, int pieceIndex, bool verified, int verificationProgress);

protected:
    void run();

private slots:
    void wakeUp();

private:
    typedef QByteArray MD5 ; //File MD5 Hash

    QHash<MD5/*File MD5*/ , FileMetaInfo *> fileMetaInfoHash;


    QByteArray readBlock(int requestID, FileMetaInfo *info, int pieceIndex);
    bool writeBlock(FileMetaInfo *info, int pieceIndex, const QByteArray &data, const QByteArray &dataHashSUM);
    bool verifySinglePiece(FileMetaInfo *info, int pieceIndex);
    //    void verifyFileContents(FileMetaInfo *info);
    //    void VerifyAllPendingFiles();

    struct WriteRequest {
        //int fileID;
        QByteArray fileMD5;
        int pieceIndex;
        QByteArray data;
        QByteArray dataSHA1SUM;
    };
    struct ReadRequest {
        //int fileID;
        QByteArray fileMD5;
        int pieceIndex;
        //int length;
        int id;
    };


    //    QString errString;



    int readId;
    //    bool startVerification;
    bool m_quit;
    bool m_wokeUp;

    QList<WriteRequest> writeRequests;
    QList<ReadRequest> readRequests;
    //    QList<MD5/*File MD5*/> pendingVerificationRequests;
    //    QList<MD5/*File MD5*/> newPendingVerificationRequests;

    mutable QMutex mutex;
    mutable QWaitCondition cond;

};

} //namespace HEHUI

#endif
