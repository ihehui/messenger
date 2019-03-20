/*
 * clientnetworkmanager.h
 *
 *  Created on: 2009-9-14
 *      Author: 贺辉
 */

#ifndef CLIENTRESOURCEMANAGER_H_
#define CLIENTRESOURCEMANAGER_H_

#include <QObject>

#include "../../sharedim/resourcesmanager.h"

#include "filetransmitter/clientfiletransmissionmanager.h"


#include "HHSharedCore/User"
#include "HHSharedCore/Singleton"

namespace HEHUI
{


class ClientResourcesManager: public ResourcesManager, public Singleton<ClientResourcesManager>
{
    Q_OBJECT

    friend class Singleton<ClientResourcesManager> ;

public:
    ClientResourcesManager(QObject *parent = 0);
//    ClientResourcesManager(NetworkType type, CommunicationMode mode, QObject *parent);
    virtual ~ClientResourcesManager();


    void setForwardServerAddressInfo(const QHostAddress &address, quint16 tcpPort, quint16 rtpPort);

signals:



public slots:



private slots:
//    bool initFileTransmission();

private:


    QHostAddress m_forwardServerAddress;
    quint16 m_forwardServerTCPPort;
    quint16 m_forwardServerRTPPort;


};

} //namespace HEHUI

#endif /* CLIENTRESOURCEMANAGER_H_ */
