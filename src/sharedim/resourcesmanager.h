/*
 * networkmanager.h
 *
 *  Created on: 2010-7-21
 *      Author: HeHui
 */

#ifndef RESOURCESMANAGER_H_
#define RESOURCESMANAGER_H_

#include <QObject>
#include <QCoreApplication>

#include "constants_global_shared.h"
#include "filemanager.h"
#include "sharedimlib.h"

#include "rtp.h"

#include "networkmanagerbase.h"

#include "./filetransmitter/filetransmissionmanagerbase.h"
#include "./filetransmitter/filetransmissionpacketsparserbase.h"

#include "HHSharedNetwork/hudpsocket.h"
#include "HHSharedUDT/hudtprotocolforfiletransmission.h"




namespace HEHUI
{


class SHAREDIMLIB_API ResourcesManager : public QObject
{
    Q_OBJECT

public:
    ResourcesManager(QObject *parent);
    virtual ~ResourcesManager();

    NetworkManagerBase * getNetworkManager();

    bool initFileTransmission(const QString &myID);
    FileTransmissionManagerBase * getFileTransmissionManager();

    void getFileTransmissionServerPorts(quint16 *udpPort, quint16 *tcpPort, quint16 *rtpPort);


private:


private:
//    QString m_myID;

    NetworkManagerBase *m_networkManager;


    FileTransmissionManagerBase *m_fileTransmissionManager;

};

}

#endif /* NETWORKMANAGER_H_ */
