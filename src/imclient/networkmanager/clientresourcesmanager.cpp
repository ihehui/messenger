/*
 * networkmanager.cpp
 *
 *  Created on: 2009-9-14
 *      Author: 贺辉
 */

#include <QDebug>
#include <QHostInfo>

#include <QNetworkInterface>
#include <QHostAddress>

#include "clientresourcesmanager.h"
#include "../contactsmanager/contactsmanager.h"

//#include "../../shared/core/user.h"
//#include "../../shared/core/utilities.h"
#include "HHSharedCore/huser.h"
#include "HHSharedCore/hutilities.h"


namespace HEHUI
{

ClientResourcesManager::ClientResourcesManager(QObject *parent)
    : ResourcesManager(parent)
{

    getNetworkManager()->setNetworkType(NetworkManagerBase::LAN);
    getNetworkManager()->setCommunicationMode(NetworkManagerBase::P2P);

//    m_fileTransmissionManager = 0;
//    m_fileTransmissionPacketsParser = 0;
//    m_socketConnectedToServer = INVALID_SOCK_ID;

    m_forwardServerAddress = QHostAddress::Null;
    m_forwardServerTCPPort = 0;
    m_forwardServerRTPPort = 0;
}

//ClientResourcesManager::ClientResourcesManager(NetworkType type, CommunicationMode mode, QObject *parent)
//    :ResourcesManager(parent)//, mainWindow(mainWindow)
//{

//    setNetworkType(type);
//    setCommunicationMode(mode);

//}

ClientResourcesManager::~ClientResourcesManager()
{
    // TODO Auto-generated destructor stub

}

void ClientResourcesManager::setForwardServerAddressInfo(const QHostAddress &address, quint16 tcpPort, quint16 rtpPort)
{
    m_forwardServerAddress = address;
    m_forwardServerRTPPort = tcpPort;
    m_forwardServerRTPPort = rtpPort;
}

//bool ClientResourcesManager::initFileTransmission()
//{

//    if(!m_fileTransmissionPacketsParser) {

//        IMUser *myself = IMUser::instance();
//        QString myID = myself->getUserID();

//        //TODO:Get File Server Info
//        emit signalRequestFileServerInfo();

//        int n = 0;
//        while (myself->getFileServerPort() == 0) {
//            Utilities::msleep(500);
//            qApp->processEvents();

//            n++;
//            if(10 == n) {
//                QMessageBox::critical(this, tr("Error"), tr("Timeout! Can not get file server info!"));
//                return false;
//            }
//        }

//        m_fileTransmissionPacketsParser = new ClientFileTransmissionPacketsParser(myID, this);
//        m_fileTransmissionManager = new ClientFileTransmissionManager(myID, m_fileTransmissionPacketsParser, this);

//    }

//    while (m_socketConnectedToServer == INVALID_SOCK_ID) {
//        m_socketConnectedToServer = m_fileTransmissionPacketsParser->connectToServer();

//        if(m_socketConnectedToServer == INVALID_SOCK_ID) {
//            int btn = QMessageBox::critical(this, tr("Connection Failed"), tr("Can not connect to file server!"),
//                                            QMessageBox::Retry | QMessageBox::Cancel,
//                                            QMessageBox::Retry
//                                            );
//            if(btn == QMessageBox::Cancel) {
//                return false;
//            }
//        }
//    }


//    return true;

//}





} //namespace HEHUI
