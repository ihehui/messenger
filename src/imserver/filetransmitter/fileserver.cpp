#include "fileserver.h"

namespace HEHUI {


FileServer::FileServer(const QString &hostName, quint16 port, const QHostAddress &address, QObject *parent) :
    QObject(parent), m_hostName(hostName), m_port(port), m_hostAddress(address)
{

}

void FileServer::setHostName(const QString &hostName){
    m_hostName = hostName;
}

QString FileServer::getHostName(){
    return m_hostName;
}

void FileServer::setPort(quint16 port){
    m_port = port;
}

quint16 FileServer::getPort(){
    return m_port;
}

void FileServer::setHostAddress(const QHostAddress &address){
    m_hostAddress = address;
}

QHostAddress FileServer::getHostAddress(){
    return m_hostAddress;
}



} //namespace
