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


#include "HHSharedNetwork/hudpsocket.h"
#include "HHSharedUDT/hudtprotocolforfiletransmission.h"




namespace HEHUI {


class SHAREDIMLIB_API ResourcesManager : public QObject {
    Q_OBJECT

public:
    enum NetworkType {LAN = 0, INTERNET = 1, NWTYPE_MIXED};
    enum CommunicationMode {P2P = 0, CS = 1};

    ResourcesManager(QObject *parent);
    virtual ~ResourcesManager();

    void setNetworkType(NetworkType type);
    NetworkType getNetworkType() const;

    void setCommunicationMode(CommunicationMode mode);
    CommunicationMode getCommunicationMode() const;


    UDPServer * getIPMCServer(){return this->m_ipmcServer;}
    UDPServer * startIPMCServer(const QHostAddress &ipmcGroupAddress = QHostAddress(IM_SERVER_IPMC_ADDRESS), quint16 ipmcGroupPort = quint16(IM_SERVER_IPMC_LISTENING_PORT), QString *errorMessage = 0);

    UDPServer * getUDPServer(){return this->m_udpServer;}
    UDPServer * startUDPServer(const QHostAddress &address = QHostAddress::Any, quint16 port = 0, bool tryOtherPort = true, QString *errorMessage = 0);

    RTP * getRTP();
    RTP * startRTP(const QHostAddress &localAddress = QHostAddress::Any, quint16 localPort = 0, bool tryOtherPort = true, QString *errorMessage = 0);

    FileManager *getFileManager();
    void stopFileManager();

private:


private:

    NetworkType networkType;
    CommunicationMode communicationMode;

    UDPServer *m_ipmcServer;
    UDPServer *m_udpServer;

//    UDTProtocol *udtProtocol;
//    TCPServer *m_tcpServer;
    RTP *m_rtp;

    FileManager *m_fileManager;

    

};

}

#endif /* NETWORKMANAGER_H_ */
