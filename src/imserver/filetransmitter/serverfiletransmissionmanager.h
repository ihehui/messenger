#ifndef SERVERFILETRANSMISSIONMANAGER_H
#define SERVERFILETRANSMISSIONMANAGER_H

#include <QObject>

#include "serverfiletransmissionpacketsparser.h"

#include "../../sharedim/filemanager.h"
#include "../../sharedim/filetransmitter/filetransmissionmanager.h"

#include "HHSharedCore/hsingleton.h"

namespace HEHUI {


class ServerFileTransmissionManager : public FileTransmissionManagerBase
{
    Q_OBJECT

public:
    explicit ServerFileTransmissionManager(const QString &myID, FileTransmissionPacketsParserBase *fileTransmissionPacketsParser, QObject *parent = 0);
    ~ServerFileTransmissionManager();


signals:



public slots:

private slots:

    void processPeerRequestUploadFilePacket(int socketID, const QString &peerID, const QByteArray &fileMD5Sum, const QString &fileName, quint64 size, const QString &localFileSaveDir);



private:






};

} //namespace HEHUI

#endif // SERVERFILETRANSMISSIONMANAGER_H
