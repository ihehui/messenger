#include "networkmanagerbase.h"

#include "impackets.h"

#include "HHSharedNetwork/hnetworkutilities.h"


namespace HEHUI
{


NetworkManagerBase::NetworkManagerBase(QObject *parent)
    : QObject(parent)
{

    //注册自定义类型，必须重载“<<”和“>>”, 见"packetstreamoperator.h"
    //if(!QMetaType::isRegistered(QMetaType::type("HEHUI::Packet"))){
    //    qRegisterMetaTypeStreamOperators<HEHUI::Packet>("HEHUI::Packet");
    //}




    qRegisterMetaType<QHostAddress>("QHostAddress");

    qRegisterMetaType<SOCKETID>("SOCKETID");

    qRegisterMetaType<PacketBase>("PacketBase");
    qRegisterMetaType<ServerDiscoveryPacket>("ServerDiscoveryPacket");
    qRegisterMetaType<DataForwardPacket>("DataForwardPacket");
    qRegisterMetaType<AnnouncementPacket>("AnnouncementPacket");
    qRegisterMetaType<RgeistrationPacket>("RgeistrationPacket");
    qRegisterMetaType<UpdatePasswordPacket>("UpdatePasswordPacket");
    qRegisterMetaType<LoginPacket>("LoginPacket");
    qRegisterMetaType<OnlineStateChangedPacket>("OnlineStateChangedPacket");
    qRegisterMetaType<OnlineContacts>("OnlineContacts");
    qRegisterMetaType<ContactGroupsInfoPacket>("ContactGroupsInfoPacket");
    qRegisterMetaType<InterestGroupsInfoPacket>("InterestGroupsInfoPacket");
    qRegisterMetaType<ContactInfoPacket>("ContactInfoPacket");
    qRegisterMetaType<SearchInfoPacket>("SearchInfoPacket");
    qRegisterMetaType<ChatMessagePacket>("ChatMessagePacket");
    qRegisterMetaType<CaptchaInfoPacket>("CaptchaInfoPacket");
    qRegisterMetaType<FileTransferPacket>("FileTransferPacket");


    networkType = LAN;
    communicationMode = P2P;

    m_ipmcServer = 0;
    m_udpServer = 0;
//    udtProtocol = new UDTProtocol(true, 0, this);
//    m_tcpServer = new TCPServer(this);

    m_rtp = new RTP(this);

}

NetworkManagerBase::~NetworkManagerBase()
{

    if(m_ipmcServer) {
        m_ipmcServer->close();
        delete m_ipmcServer;
        m_ipmcServer = 0;
    }


    if(m_udpServer) {
        m_udpServer->close();
        delete m_udpServer;
        m_udpServer = 0;
    }

    if(m_rtp) {
        delete m_rtp;
        m_rtp = 0;
    }


}

void NetworkManagerBase::setNetworkType(NetworkType type)
{
    this->networkType = type;

}

NetworkManagerBase::NetworkType NetworkManagerBase::getNetworkType() const
{
    return this->networkType;

}

void NetworkManagerBase::setCommunicationMode(CommunicationMode mode)
{
    this->communicationMode = mode;

}

NetworkManagerBase::CommunicationMode NetworkManagerBase::getCommunicationMode() const
{
    return this->communicationMode;

}

UDPServer * NetworkManagerBase::getIPMCServer()
{
    return this->m_ipmcServer;
}

UDPServer * NetworkManagerBase::startIPMCServer(const QHostAddress &ipmcGroupAddress, quint16 ipmcGroupPort, QString *errorMessage)
{


    if(!m_ipmcServer) {
        m_ipmcServer = new UDPServer(this);
    }

    if (m_ipmcServer->startIPMulticastListening(ipmcGroupAddress, ipmcGroupPort)) {
        return m_ipmcServer;
    } else {
        if(errorMessage) {
            *errorMessage = m_ipmcServer->errorString();
        }

        delete m_ipmcServer;
        m_ipmcServer = 0;

        return 0;
    }

    return m_ipmcServer;

}

UDPServer * NetworkManagerBase::getUDPServer()
{
    return this->m_udpServer;
}

UDPServer * NetworkManagerBase::startUDPServer(const QHostAddress &address, quint16 startPort, bool tryOtherPort, QString *errorMessage)
{

    if(!m_udpServer) {
        m_udpServer = new UDPServer(this);
    }

    if((startPort != 0) && m_udpServer->localPort() == startPort) {
        return m_udpServer;
    }

    if (m_udpServer->startSimpleListening(address, startPort)) {
        return m_udpServer;
    } else {
        if(tryOtherPort) {
            if (m_udpServer->startSimpleListening(address, 0)) {
                return m_udpServer;
            }
        }

        if(errorMessage) {
            *errorMessage = m_udpServer->errorString();
        }
        delete m_udpServer;
        m_udpServer = 0;

        return 0;
    }

    return m_udpServer;

}

RTP * NetworkManagerBase::getRTP()
{
    return m_rtp;
}

RTP * NetworkManagerBase::startRTP(const QHostAddress &localAddress, quint16 localPort, bool tryOtherPort, QString *errorMessage)
{

    if(!m_rtp) {
        m_rtp = new RTP(this);
    }

    m_rtp->startServers(localAddress, localPort, tryOtherPort, errorMessage);
    return m_rtp;

}

//UDTProtocol * ResourcesManager::startUDTProtocol(const QHostAddress &localAddress, quint16 localPort, bool tryOtherPort, QString *errorMessage){

//    if(!udtProtocol){
//        udtProtocol = new UDTProtocol(true, 0, this);
//    }

//    UDTSOCKET socket = udtProtocol->listen(localPort, localAddress);
//    if(socket == UDTProtocolBase::INVALID_UDT_SOCK && tryOtherPort){
//        socket = udtProtocol->listen();
//    }

//    if(socket == UDTProtocolBase::INVALID_UDT_SOCK){
//        if(errorMessage){
//            *errorMessage = udtProtocol->getLastErrorMessage();
//        }
//        delete udtProtocol;
//        udtProtocol = 0;
//        return 0;
//    }

//    return udtProtocol;

//}






} //namespace HEHUI
