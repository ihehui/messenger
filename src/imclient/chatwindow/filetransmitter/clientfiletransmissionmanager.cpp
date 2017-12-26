#include "clientfiletransmissionmanager.h"


#include "HHSharedCore/hutilities.h"




namespace HEHUI
{


ClientFileTransmissionManager::ClientFileTransmissionManager(const QString &myID, QObject *parent) :
    FileTransmissionManagerBase(myID, parent)
{


}

void ClientFileTransmissionManager::processPeerRequestUploadFilePacket(int socketID, const QString &peerID, const QByteArray &fileMD5Sum, const QString &fileName, quint64 size, const QString &localFileSaveDir)
{
    qDebug() << "--ClientFileTransmissionManager::processPeerRequestUploadFilePacket(...) " << " socketID:" << socketID << " peerID:" << peerID << " fileMD5Sum:" << fileMD5Sum << " fileName:" << fileName;

    //TODO

}






} //namespace HEHUI
