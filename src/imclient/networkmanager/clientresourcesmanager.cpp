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


namespace HEHUI {

ClientResourcesManager::ClientResourcesManager(QObject *parent)
    :ResourcesManager(parent)
{
    setNetworkType(LAN);
    setCommunicationMode(P2P);
}

//ClientResourcesManager::ClientResourcesManager(NetworkType type, CommunicationMode mode, QObject *parent)
//    :ResourcesManager(parent)//, mainWindow(mainWindow)
//{

//    setNetworkType(type);
//    setCommunicationMode(mode);

//}

ClientResourcesManager::~ClientResourcesManager() {
    // TODO Auto-generated destructor stub
}







} //namespace HEHUI
