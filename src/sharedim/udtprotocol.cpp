
#include <QDataStream>
#include <QDebug>

#include "udtprotocol.h"

#include "HHSharedNetwork/hpackethandlerbase.h"

namespace HEHUI {


UDTProtocol::UDTProtocol(bool stream, const SocketOptions *options, QObject *parent) :
    UDTProtocolBase(stream, options, parent)
{


    //注册自定义类型，必须重载“<<”和“>>”, 见"packetstreamoperator.h"
    //qRegisterMetaTypeStreamOperators<HEHUI::Packet>("HEHUI::Packet");
    Packet::registerMetaTypeStreamOperators();

}

//bool UDTProtocol::sendData(UDTSOCKET socket, const QByteArray *byteArray){

//    if(isStreamMode()){
//        return sendUDTStreamData(socket, byteArray);
//    }else{
//        return sendUDTMessageData(socket, byteArray);
//    }

//}

void UDTProtocol::streamDataReceived(UDTSOCKET socket, QByteArray *data){
    //qDebug()<<"--UDTProtocol::streamDataReceived(...) "<<"socket:"<<socket;

    convertDataToPacket(socket, data);

}

void UDTProtocol::messageDataReceived(UDTSOCKET socket, QByteArray *data){
    qDebug()<<"--UDTProtocol::messageDataReceived(...) "<<"socket:"<<socket;

    convertDataToPacket(socket, data);

}

inline void UDTProtocol::convertDataToPacket(UDTSOCKET socket, QByteArray *data){

    QString ip = "";
    quint16 port = 0;
    getAddressInfoFromSocket(socket, &ip, &port);

    QDataStream in(data, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_4_7);
    QVariant v;
    in >> v;
    if (v.canConvert<Packet>()){
        Packet *packet = PacketHandlerBase::getPacket();
        *packet = v.value<Packet>();
        packet->setTransmissionProtocol(TP_UDT);
        packet->setSocketID(socket);


        packet->setPeerHostAddress(QHostAddress(ip));
        packet->setPeerHostPort(port);
//        packet->setLocalHostAddress(m_udpSocket->localAddress());
//        packet->setLocalHostPort(m_udpSocket->localPort());

//        m_packetHandlerBase->appendIncomingPacket(packet);

        emit packetReceived(packet);

    }else{
        qWarning()<<"ERROR! Can not convert UDT data to Packet!";
    }

}














} //namespace HEHUI
