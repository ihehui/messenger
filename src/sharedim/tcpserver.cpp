
#include <QDataStream>

#include "tcpserver.h"

#include "HHSharedNetwork/hpackethandlerbase.h"


namespace HEHUI {


TCPServer::TCPServer(QObject *parent) :
    TCPBase(parent)
{

}

quint16 TCPServer::getTCPServerListeningPort(){
    quint16 port = 0;
    serverAddressInfo(0, &port);
    return port;
}

void TCPServer::processData(int socketID, QByteArray *data){

    QString address;
    quint16 port;
    getAddressInfoFromSocket(socketID, &address, &port, true);


    QDataStream in(data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_7);
    QVariant v;
    in >> v;
    if (v.canConvert<Packet>()){
        Packet *packet = PacketHandlerBase::getPacket();
        *packet = v.value<Packet>();
        packet->setTransmissionProtocol(TP_UDT);
        packet->setSocketID(socketID);


        packet->setPeerHostAddress(QHostAddress(address));
        packet->setPeerHostPort(port);
//        packet->setLocalHostAddress(m_udpSocket->localAddress());
//        packet->setLocalHostPort(m_udpSocket->localPort());

//        m_packetHandlerBase->appendIncomingPacket(packet);

        emit packetReceived(packet);

    }else{
        qWarning()<<"ERROR! Can not convert TCP data to Packet!";
    }


}













}
