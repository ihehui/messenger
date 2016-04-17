
#include <QDataStream>
#include <QDebug>

#include "udtprotocol.h"


namespace HEHUI {


UDTProtocol::UDTProtocol(bool stream, const SocketOptions *options, QObject *parent) :
    UDTProtocolBase(stream, options, parent)
{


    //注册自定义类型，必须重载“<<”和“>>”, 见"packetstreamoperator.h"
    //qRegisterMetaTypeStreamOperators<HEHUI::Packet>("HEHUI::Packet");
    PacketBase::registerMetaTypeStreamOperators();

}

//bool UDTProtocol::sendData(UDTSOCKET socket, const QByteArray *byteArray){

//    if(isStreamMode()){
//        return sendUDTStreamData(socket, byteArray);
//    }else{
//        return sendUDTMessageData(socket, byteArray);
//    }

//}

void UDTProtocol::streamDataReceived(UDTSOCKET socket, QByteArray *data){
    qDebug()<<"--UDTProtocol::streamDataReceived(...) "<<"socket:"<<socket;

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

//    QDataStream in(data, QIODevice::ReadOnly);
//    in.setVersion(QDataStream::Qt_4_8);
//    QVariant v;
//    in >> v;
//    if (v.canConvert<PacketBase>()){
//        PacketBase *packet = PacketHandlerBase::getPacket();
//        *packet = v.value<PacketBase>();
//        packet->setSocketID(socket);


//        packet->setPeerHostAddress(QHostAddress(ip));
//        packet->setPeerHostPort(port);

//        emit packetReceived(packet);
//    }

    PacketBase packet;
    if(packet.fromByteArray(data)){
        packet.setSocketID(socket);
        packet.setPeerHostAddress(QHostAddress(ip));
        packet.setPeerHostPort(port);
        emit packetReceived(packet);
    }



}














} //namespace HEHUI
