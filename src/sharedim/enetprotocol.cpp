
#include <QDebug>

#include "enetprotocol.h"


namespace HEHUI
{


ENETProtocol::ENETProtocol(QObject *parent) :
    ENETProtocolBase(parent)
{
    //注册自定义类型，必须重载“<<”和“>>”
    //qRegisterMetaTypeStreamOperators<HEHUI::Packet>("HEHUI::Packet");
    PacketBase::registerMetaTypeStreamOperators();
}

void ENETProtocol::processReceivedData(quint32 peerID, QByteArray data)
{
    //qDebug()<<"--ENETProtocolTest::processReceivedData(...) "<<"peerID:"<<peerID;
    convertDataToPacket(peerID, &data);
}

inline void ENETProtocol::convertDataToPacket(quint32 peerID, QByteArray *data)
{
    QString ip = "";
    quint16 port = 0;
    getPeerAddressInfo(peerID, &ip, &port);

//    QDataStream in(data, QIODevice::ReadOnly);
//    in.setVersion(QDataStream::Qt_4_8);
//    QVariant v;
//    in >> v;
//    if (v.canConvert<PacketBase>()){
//        PacketBase *packet = PacketHandlerBase::getPacket();
//        *packet = v.value<PacketBase>();
//        packet->setTransmissionProtocol(TP_UDT);
//        packet->setSocketID(peerID);

//        packet->setPeerHostAddress(QHostAddress(ip));
//        packet->setPeerHostPort(port);

//        emit packetReceived(packet);
//    }

    PacketBase packet;
    if(packet.fromByteArray(data)) {
        packet.setSocketID(peerID);
        packet.setPeerHostAddress(QHostAddress(ip));
        packet.setPeerHostPort(port);
        emit packetReceived(packet);
    }

}














} //namespace HEHUI
