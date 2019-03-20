#ifndef NETWORKMANAGERBASE_H
#define NETWORKMANAGERBASE_H

#include <QObject>

#include "constants_global_shared.h"
#include "sharedimlib.h"

#include "rtp.h"

#include "HHSharedNetwork/UDPSocket"
#include "HHSharedUDT/hudtprotocolforfiletransmission.h"


namespace HEHUI
{


class SHAREDIMLIB_API NetworkManagerBase : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManagerBase(QObject *parent = nullptr);
    virtual ~NetworkManagerBase();

    enum NetworkType {LAN = 0, INTERNET = 1, NWTYPE_MIXED};
    enum CommunicationMode {P2P = 0, CS = 1};

    void setNetworkType(NetworkType type);
    NetworkType getNetworkType() const;

    void setCommunicationMode(CommunicationMode mode);
    CommunicationMode getCommunicationMode() const;


    UDPServer *getIPMCServer();
    UDPServer *startIPMCServer(const QHostAddress &ipmcGroupAddress = QHostAddress(IM_SERVER_IPMC_ADDRESS), quint16 ipmcGroupPort = quint16(IM_SERVER_IPMC_LISTENING_PORT), QString *errorMessage = 0);

    UDPServer *getUDPServer();
    UDPServer *startUDPServer(const QHostAddress &address = QHostAddress::Any, quint16 port = 0, bool tryOtherPort = true, QString *errorMessage = 0);

    RTP *getRTP();
    RTP *startRTP(const QHostAddress &localAddress = QHostAddress::Any, quint16 localPort = 0, bool tryOtherPort = true, QString *errorMessage = 0);


signals:

public slots:

private:
    NetworkType networkType;
    CommunicationMode communicationMode;

    UDPServer *m_ipmcServer;
    UDPServer *m_udpServer;

    RTP *m_rtp;


};

} //namespace HEHUI

#endif // NETWORKMANAGERBASE_H
