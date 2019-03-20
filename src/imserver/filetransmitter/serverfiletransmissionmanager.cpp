#include "serverfiletransmissionmanager.h"


#include "HHSharedCore/CoreUtilities"




namespace HEHUI
{


ServerFileTransmissionManager::ServerFileTransmissionManager(const QString &myID, QObject *parent) :
    FileTransmissionManagerBase(myID, parent)
{


}

ServerFileTransmissionManager::~ServerFileTransmissionManager()
{


}

void ServerFileTransmissionManager::processPeerRequestUploadFilePacket(int socketID, const QString &peerID, const QByteArray &fileMD5Sum, const QString &fileName, quint64 size, const QString &localFileSaveDir)
{
    qDebug() << "--ServerFileTransmissionManager::processPeerRequestUploadFilePacket(...) " << " socketID:" << socketID << " peerID:" << peerID << " fileMD5Sum:" << fileMD5Sum << " fileName:" << fileName;

    //TODO

    acceptFileUploadRequest(socketID, peerID, fileMD5Sum, size, "./offline_files");

}



} //namespace HEHUI
