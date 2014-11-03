
#include <QDebug>
#include "rtp.h"

#include "constants_global_shared.h"

namespace HEHUI {


RTP::RTP(QObject *parent) :
    QObject(parent)
{

    m_udtProtocol = new UDTProtocol(true, 0, this);
    connect(m_udtProtocol, SIGNAL(disconnected(int)), this, SIGNAL(disconnected(int)));

    m_tcpServer = new TCPServer(this);
    connect(m_tcpServer, SIGNAL(disconnected(int)), this, SIGNAL(disconnected(int)));


}

RTP::~RTP(){

    if(m_udtProtocol){
        m_udtProtocol->close();
        delete m_udtProtocol;
        m_udtProtocol = 0;
    }

    if(m_tcpServer){
        m_tcpServer->closeServer();
        delete m_tcpServer;
        m_tcpServer = 0;
    }

}

void RTP::startServers(const QHostAddress &localAddress, quint16 localPort, bool tryOtherPort, QString *errorMessage){

    QString err;

    UDTSOCKET socket = m_udtProtocol->listen(localPort, localAddress);
    if(socket == INVALID_SOCK_ID && tryOtherPort){
        socket = m_udtProtocol->listen();
    }
    if(socket == INVALID_SOCK_ID){
        err = m_udtProtocol->getLastErrorMessage();
    }

    bool ok = m_tcpServer->listen(localAddress, localPort);
    if(!ok){
        ok = m_tcpServer->listen();
    }
    if(!ok){
        err += " " + m_tcpServer->serverErrorString();
    }

    if(errorMessage){
        *errorMessage = err;
    }


}

void RTP::stopServers(){
    if(m_udtProtocol){
        m_udtProtocol->close();
    }

    if(m_tcpServer){
        m_tcpServer->closeServer();
    }

}

UDTProtocol * RTP::startUDTProtocol(const QHostAddress &localAddress, quint16 localPort, bool tryOtherPort, QString *errorMessage){

    if(!m_udtProtocol){
        m_udtProtocol = new UDTProtocol(true, 0, this);
    }

    UDTSOCKET socket = m_udtProtocol->listen(localPort, localAddress);
    if(socket == UDTProtocolBase::INVALID_UDT_SOCK && tryOtherPort){
        socket = m_udtProtocol->listen();
    }

    if(socket == UDTProtocolBase::INVALID_UDT_SOCK){
        if(errorMessage){
            *errorMessage = m_udtProtocol->getLastErrorMessage();
        }
        delete m_udtProtocol;
        m_udtProtocol = 0;
        return 0;
    }

    return m_udtProtocol;

}

quint16 RTP::getUDTServerPort(){
    return m_udtProtocol->getUDTListeningPort();
}

TCPServer * RTP::startTCPServer(const QHostAddress &address, quint16 port, bool tryOtherPort, QString *errorMessage){

    if(!m_tcpServer){
        m_tcpServer = new TCPServer(this);
    }

    if( (!m_tcpServer->listen(address, port)) && tryOtherPort){
        m_tcpServer->listen();
    }

    if(!m_tcpServer->isListening()){
        if(errorMessage){
            *errorMessage = m_tcpServer->serverErrorString();
        }
        delete m_tcpServer;
        m_tcpServer = 0;
    }

    return m_tcpServer;

}

quint16 RTP::getTCPServerPort(){
    quint16 port;
    m_tcpServer->serverAddressInfo(0, &port);
    return port;
}

int RTP::connectToHost( const QHostAddress & hostAddress, quint16 port, int waitMsecs, QString *errorMessage, Protocol protocol){

    int socketID = -1;
    QString err;

    if(protocol != TCP){
        socketID = m_udtProtocol->connectToHost(hostAddress, port, 0, true, waitMsecs);
        if( (socketID == INVALID_SOCK_ID) || (!m_udtProtocol->isSocketConnected(socketID)) ){
            err += tr("Can not connect to host %1:%2 via UDT! %3").arg(hostAddress.toString()).arg(port).arg(m_udtProtocol->getLastErrorMessage());
            qCritical()<<err;
            if(protocol == UDT){
                if(errorMessage){
                    *errorMessage = err;
                }
                return socketID;
            }
        }else{
            qDebug()<<QString("Peer %1:%2 connected via UDT! ").arg(hostAddress.toString()).arg(port);
            return socketID;
        }
    }

//    if( (socketID == INVALID_SOCK_ID) || (!m_udtProtocol->isSocketConnected(socketID)) ){
//        err = tr("Can not connect to host %1:%2 via UDT! %3").arg(hostAddress.toString()).arg(port).arg(m_udtProtocol->getLastErrorMessage());

        socketID = m_tcpServer->connectToHost(hostAddress, port, waitMsecs);
        if(!m_tcpServer->isConnected(socketID) ){
            err += tr("\nCan not connect to host %1:%2 via TCP! %3").arg(hostAddress.toString()).arg(port).arg(m_tcpServer->socketErrorString(socketID));
            qCritical()<<err;
            m_tcpServer->abort(socketID);
            socketID = INVALID_SOCK_ID;
        }
//    }

    if(errorMessage){
        *errorMessage = err;
    }

    return socketID;

}

void RTP::closeSocket(int socketID){
    qDebug()<<"--RTP::closeSocket(...)";

    m_udtProtocol->closeSocket(socketID);
    m_tcpServer->disconnectFromHost(socketID);

}

bool RTP::isSocketConnected(int socketID){

    bool connected = false;
    if(m_udtProtocol->isSocketExist(socketID)){
        connected = m_udtProtocol->isSocketConnected(socketID);
    }else{
        connected = m_tcpServer->isConnected(socketID);
    }

    return connected;

}

bool RTP::getAddressInfoFromSocket(int socketID, QString *address, quint16 *port, bool getPeerInfo){

    if(m_udtProtocol->isSocketExist(socketID)){
        return m_udtProtocol->getAddressInfoFromSocket(socketID, address, port, getPeerInfo);
    }else{
        return m_tcpServer->getAddressInfoFromSocket(socketID, address, port, getPeerInfo);
    }

}

bool RTP::isUDTSocket(int socketID){
    return m_udtProtocol->isSocketExist(socketID);
}


bool RTP::sendReliableData(int socketID, const QByteArray *byteArray){

    bool ok = false;
    if(m_udtProtocol->isSocketExist(socketID)){
        ok = m_udtProtocol->sendData(socketID, byteArray);
        m_lastErrorString = m_udtProtocol->getLastErrorMessage();
        //qDebug()<<"Sending UDT data! "<<m_lastErrorString;
    }else{
        ok = m_tcpServer->sendData(socketID, byteArray);
        m_lastErrorString = m_tcpServer->socketErrorString(socketID);
        //qDebug()<<"Sending TCP data! "<<m_lastErrorString;
    }

    return ok;

}




} //namespace HEHUI
