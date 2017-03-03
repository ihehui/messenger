#include <QDataStream>

#include "tcpserver.h"



namespace HEHUI
{


TCPServer::TCPServer(QObject *parent) :
    TCPBase(parent)
{

}

TCPServer::~TCPServer()
{
    qDebug() << "--TCPServer::~TCPServer()";
}

quint16 TCPServer::getTCPServerListeningPort()
{
    quint16 port = 0;
    serverAddressInfo(0, &port);
    return port;
}

void TCPServer::processData(SOCKETID socketID, QByteArray *data)
{

    QString address;
    quint16 port;
    getAddressInfoFromSocket(socketID, &address, &port, true);


//    QDataStream in(data, QIODevice::ReadOnly);
//    in.setVersion(QDataStream::Qt_4_8);
//    QVariant v;
//    in >> v;
//    if (v.canConvert<PacketBase>()){
//        PacketBase *packet = PacketHandlerBase::getPacket();
//        *packet = v.value<PacketBase>();
//        packet->setTransmissionProtocol(TP_UDT);
//        packet->setSocketID(socketID);

//        packet->setPeerHostAddress(QHostAddress(address));
//        packet->setPeerHostPort(port);

//        emit packetReceived(packet);
//    }

    PacketBase packet;
    if(packet.fromByteArray(data)) {
        packet.setSocketID(socketID);
        packet.setPeerHostAddress(QHostAddress(address));
        packet.setPeerHostPort(port);
        emit packetReceived(packet);
    }


}













}
