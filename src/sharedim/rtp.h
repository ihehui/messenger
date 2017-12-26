#ifndef RTP_H
#define RTP_H

#include <QObject>

#include <limits>

#include "sharedimlib.h"
#include "udtprotocol.h"
#include "tcpserver.h"
#include "enetprotocol.h"


namespace HEHUI
{


class SHAREDIMLIB_API RTP : public QObject
{
    Q_OBJECT
public:
    enum Protocol {AUTO = 0, TCP, ENET, UDT};

    explicit RTP(QObject *parent = 0);
    ~RTP();

    void startServers(const QHostAddress &localAddress = QHostAddress::Any, quint16 localPort = 0, bool tryOtherPort = true, QString *errorMessage = 0);
    void stopServers();

//    UDTProtocol * getUDTProtocol(){return m_udtProtocol;}
    UDTProtocol *startUDTProtocol(const QHostAddress &localAddress = QHostAddress::Any, quint16 localPort = 0, bool tryOtherPort = true, QString *errorMessage = 0);
    quint16 getUDTServerPort();

    TCPServer *getTCPServer();
    TCPServer *startTCPServer(const QHostAddress &address = QHostAddress::Any, quint16 port = 0, bool tryOtherPort = true, QString *errorMessage = 0);
    quint16 getTCPServerPort();

    ENETProtocol *getENETProtocol();
    ENETProtocol *startENETProtocol(const QHostAddress &address = QHostAddress::Any, quint16 port = 0, bool tryOtherPort = true, QString *errorMessage = 0);
    quint16 getENETProtocolPort();

    SOCKETID connectToHost( const QHostAddress &hostAddress, quint16 port, int waitMsecs = 0, QString *errorMessage = 0, Protocol protocol = AUTO);
    void closeSocket(SOCKETID socketID);
    bool isSocketConnected(SOCKETID socketID);
    bool getAddressInfoFromSocket(SOCKETID socketID, QString *address, quint16 *port, bool getPeerInfo = true);

    QString socketProtocolString(SOCKETID socketID);

    bool sendReliableData(SOCKETID socketID, const QByteArray *byteArray);
    QString lastErrorString() const;

signals:
//    void connected (int socketID, const QString &peerAddress, quint16 peerPort);
    void disconnected (SOCKETID socketID/*, const QString &peerAddress, quint16 peerPort*/);

public slots:


private slots:
    void tcpPeerConnected(SOCKETID socketID, const QString &address, quint16 port);
    void enetPeerConnected(SOCKETID socketID, const QString &address, quint16 port);
    void udtPeerConnected(SOCKETID socketID, const QString &address, quint16 port);

private:

    QHash<SOCKETID /*socketID*/, Protocol> m_socketInfoHash;

    UDTProtocol *m_udtProtocol;
    TCPServer *m_tcpServer;
    ENETProtocol *m_enetProtocol;

    QString m_lastErrorString;

};

} //namespace HEHUI

#endif // RTP_H
