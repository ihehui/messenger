#ifndef ENETPROTOCOL_H
#define ENETPROTOCOL_H

#include "HHSharedENET/henetprotocolbase.h"
#include "HHSharedNetwork/PacketBase"

namespace HEHUI
{


class ENETProtocol : public ENETProtocolBase
{
    Q_OBJECT
public:
    explicit ENETProtocol(QObject *parent = 0);

signals:
    void packetReceived(const PacketBase &packet);
    //void dataReceived(const QString &peerAddress, quint16 peerPort, const QByteArray &data);

public slots:


private:
    void processReceivedData(quint32 peerID, QByteArray data);

    void convertDataToPacket(quint32 peerID, QByteArray *data);


private:




};

} //namespace HEHUI

#endif // ENETPROTOCOL_H
