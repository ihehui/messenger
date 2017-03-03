#ifndef UDTPROTOCOL_H
#define UDTPROTOCOL_H

#include "HHSharedUDT/hudtprotocolbase.h"
#include "HHSharedNetwork/PacketBase"

#include "sharedimlib.h"


namespace HEHUI
{


class SHAREDIMLIB_API UDTProtocol : public UDTProtocolBase
{
    Q_OBJECT
public:
    explicit UDTProtocol(bool stream = false, const SocketOptions *options = 0, QObject *parent = 0);

signals:
    void packetReceived(const PacketBase &packet);

public slots:
//    bool sendData(UDTSOCKET socket, const QByteArray *byteArray);


private slots:
    void streamDataReceived(UDTSOCKET socket, QByteArray *data) ;
    void messageDataReceived(UDTSOCKET socket, QByteArray *data) ;



private:
    inline void convertDataToPacket(UDTSOCKET socket, QByteArray *data);



};

} //namespace HEHUI

#endif // UDTPROTOCOL_H
