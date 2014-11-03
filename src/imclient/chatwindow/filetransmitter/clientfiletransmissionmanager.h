#ifndef CLIENTFILETRANSMISSIONMANAGER_H
#define CLIENTFILETRANSMISSIONMANAGER_H

#include <QObject>

#include "clientfiletransmissionpacketsparser.h"

#include "../../../sharedim/filemanager.h"
#include "../../../sharedim/filetransmitter/filetransmissionmanager.h"

#include "HHSharedCore/hsingleton.h"

namespace HEHUI {


class ClientFileTransmissionManager : public FileTransmissionManagerBase
{
    Q_OBJECT

public:
    explicit ClientFileTransmissionManager(const QString &myID, FileTransmissionPacketsParserBase *fileTransmissionPacketsParser, QObject *parent = 0);


private slots:
    void processPeerRequestUploadFilePacket(int socketID, const QString &peerID, const QByteArray &fileMD5Sum, const QString &fileName, quint64 size, const QString &localFileSaveDir);


private:


};

} //namespace HEHUI

#endif // CLIENTFILETRANSMISSIONMANAGER_H
