#include "fileservermanager.h"
#include "fileserver.h"

namespace HEHUI {


FileServerManager::FileServerManager(QObject *parent) :
    QObject(parent)
{

}

FileServerManager::~FileServerManager(){
    foreach (FileServer *server, m_servers) {
        delete server;
        server = 0;
    }
    m_servers.clear();
}

void FileServerManager::addFileServer(const QString &hostName, quint16 port, const QHostAddress &address){
    FileServer *server = m_servers.value(hostName);
    if(server){
        server->setPort(port);
        server->setHostAddress(address);
    }else{
        server = new FileServer(hostName, port, address);
        m_servers.insert(hostName, server);
    }

}

void FileServerManager::removeFileServer(const QString &hostName){
    FileServer *server = m_servers.take(hostName);
    delete server;
}

bool FileServerManager::selectFileServer(QString *hostName, quint16 *port, QHostAddress *address){
    if(m_servers.isEmpty() || hostName == 0 || port == 0){
        return false;
    }

    FileServer *server = m_servers.value(m_servers.keys().first());
    *hostName = server->getHostName();
    *port = server->getPort();
    if(address){
        *address = server->getHostAddress();
    }

    return true;

}





}
