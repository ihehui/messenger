/*
 * networkmanager.cpp
 *
 *  Created on: 2010-7-21
 *      Author: HeHui
 */



#include <QHostInfo>
#include <QDebug>

#include "resourcesmanager.h"

#include "impackets.h"

#include "HHSharedNetwork/hnetworkutilities.h"
//#include "HHSharedNetwork/hpacketstreamoperator.h"






namespace HEHUI
{


ResourcesManager::ResourcesManager(QObject *parent)
    : QObject(parent)
{

//    m_myID = "";

    m_networkManager = new NetworkManagerBase(this);

    m_fileTransmissionManager = 0;

}

ResourcesManager::~ResourcesManager()
{

    qDebug() << "ResourcesManager::~ResourcesManager()";

    if(m_networkManager){
        delete m_networkManager;
    }


    if(m_fileTransmissionManager){
        delete m_fileTransmissionManager;
    }

}

NetworkManagerBase * ResourcesManager::getNetworkManager()
{
    return m_networkManager;
}


//TCPServer * ResourcesManager::startTCPServer(const QHostAddress &address, quint16 port, bool tryOtherPort, QString *errorMessage){

//    if(!m_tcpServer){
//        m_tcpServer = new TCPServer(this);
//    }

//    if( (!m_tcpServer->listen(address, port)) && tryOtherPort){
//        m_tcpServer->listen();
//    }

//    if(!m_tcpServer->isListening()){
//        if(errorMessage){
//            *errorMessage = m_tcpServer->serverErrorString();
//        }
//        delete m_tcpServer;
//        m_tcpServer = 0;
//    }

//    return m_tcpServer;

//}

//bool ResourcesManager::sendReliableData(int socketID, const QByteArray *byteArray){

//    if(!m_tcpServer->sendData(socketID, byteArray)){
//        return udtProtocol->sendData(socketID, byteArray);
//    }
//    return true;

//}




bool ResourcesManager::initFileTransmission(const QString &myID)
{

    if(!m_fileTransmissionManager) {
        m_fileTransmissionManager = new FileTransmissionManagerBase(myID, this);
    }

    return true;
}

FileTransmissionManagerBase * ResourcesManager::getFileTransmissionManager()
{
    return m_fileTransmissionManager;
}

void ResourcesManager::getFileTransmissionServerPorts(quint16 *udpPort, quint16 *tcpPort, quint16 *rtpPort)
{
    if(m_fileTransmissionManager){
        m_fileTransmissionManager->getServerPorts(udpPort, tcpPort, rtpPort);
    }
}

















}
