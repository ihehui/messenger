/*
 ****************************************************************************
 * server.cpp
 *
 * Created on: 2010-7-12
 *     Author: 贺辉
 *    License: LGPL
 *    Comment:
 *
 *
 *    =============================  Usage  =============================
 *|
 *|
 *    ===================================================================
 *
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 ****************************************************************************
 */

/*
 ***************************************************************************
 * Last Modified on: 2010-9-3
 * Last Modified by: 贺辉
 ***************************************************************************
 */




#include <QtCore>
#include "server.h"

#include "app_constants_server.h"
#include "settings.h"
#include "../sharedim/constants_global_shared.h"


namespace HEHUI {


IMServer::IMServer(QObject *parent)
    :QObject(parent)
{

    m_packetHandler = 0;
    resourcesManager = 0;

    serverPacketsParser = 0;

    m_fileTransmissionPacketsParser = 0;
    m_fileTransmissionManager = 0;

    m_udpServer = 0;
    m_rtp = 0;

    databaseUtility = 0;
    query = 0;

    mainServiceStarted = false;

    userInfoHash.clear();

    onlineAdminsCount = 0;

    m_serverType = ST_ALL_IN_ONE;


}

IMServer::~IMServer(){
    qDebug()<<"Server::~Server()";


    delete serverPacketsParser;
    serverPacketsParser = 0;

    //TODO:释放资源
    ResourcesManagerInstance::cleanInstance();
    delete resourcesManager;
    resourcesManager = 0;

    if(m_packetHandler){
        m_packetHandler->clean();
        delete m_packetHandler;
        m_packetHandler = 0;
    }
    PacketHandlerBase::clean();


    QList<UserInfo*> clientInfoList = userInfoHash.values();
    userInfoHash.clear();
    foreach(UserInfo *info, clientInfoList){
        delete info;
        info = 0;
    }

//TODO
    delete m_fileTransmissionManager;
    delete m_fileTransmissionPacketsParser;

    delete databaseUtility;
    databaseUtility = 0;
    delete query;
    query = 0;

        
    mainServiceStarted = false;


}


bool IMServer::startMainService(){

    if(mainServiceStarted){
        qWarning()<<"Main service has already started!";
        return true;
    }


    //TODO
    if(openDatabase()){
        qApp->exit();
    }


    if(m_serverType != ST_FILE_SERVER){
        startIMServer();
    }


    if(m_serverType != ST_IM_SERVER){
        startFileServer();
    }


    mainServiceStarted = true;

    return true;
}

void IMServer::saveClientLog(const QString &computerName, const QString &users, const QString &log, const QString &clientAddress){
    //    qWarning()<<"Server::saveClientLog(...)";

    if(!query){
        if(!openDatabase()){
            return;
        }
    }else{
        query->clear();
    }

    query->prepare("INSERT INTO logs (ComputerName, Users, IPAddress, Content) "
                   "VALUES (:ComputerName, :Users, :IPAddress, :Content)");

    query->bindValue(":ComputerName", computerName);
    query->bindValue(":Users", users);
    query->bindValue(":IPAddress", clientAddress);
    query->bindValue(":Content", log);


    if(!query->exec()){
        QSqlError error = query->lastError();
        QString msg = QString("Can not write log to database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<< msg;

        //MySQL数据库重启，重新连接
        if(error.number() == 2006){
            query->clear();
            openDatabase(true);
        }

    }

    query->clear();

}


void IMServer::saveFileLog(const QString &sender, const QString &receiver, const QString &fileName, const QString &md5Hex, quint64 size){
    //    qWarning()<<"Server::saveFileLog(...)";

    if(!query){
        if(!openDatabase()){
            return;
        }
    }else{
        query->clear();
    }

    query->prepare("INSERT INTO filelogs (Sender, Receiver, FileName, MD5Hex, Size) "
                   "VALUES (:Sender, :Receiver, :FileName, :MD5Hex, :Size)");

    query->bindValue(":Sender", sender);
    query->bindValue(":Receiver", receiver);
    query->bindValue(":FileName", fileName);
    query->bindValue(":MD5Hex", md5Hex);
    query->bindValue(":Size", size);


    if(!query->exec()){
        QSqlError error = query->lastError();
        QString msg = QString("Can not write log to database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<< msg;

        //MySQL数据库重启，重新连接
        if(error.number() == 2006){
            query->clear();
            openDatabase(true);
        }

    }

    query->clear();

}

void IMServer::peerConnected(const QHostAddress &peerAddress, quint16 peerPort){
    qWarning()<<QString("Connected! "+peerAddress.toString()+":"+QString::number(peerPort));

}

void IMServer::signalConnectToPeerTimeout(const QHostAddress &peerAddress, quint16 peerPort){
    qCritical()<<QString("Connecting Timeout! "+peerAddress.toString()+":"+QString::number(peerPort));

}

void IMServer::peerDisconnected(const QHostAddress &peerAddress, quint16 peerPort, bool normalClose){
    qDebug()<<QString("Client Disconnected! "+peerAddress.toString()+":"+QString::number(peerPort));

    if(!normalClose){
        serverPacketsParser->userExceptionalOffline(peerAddress.toString(), peerPort);
        qCritical()<<QString("ERROR! Peer %1:%2 Closed Unexpectedly!").arg(peerAddress.toString()).arg(peerPort);

    }


}

void IMServer::peerDisconnected(int socketID){
    //TODO



}









bool IMServer::openDatabase(bool reopen){

    if(reopen){       
        if(query){
            query->clear();
            delete query;
            query = 0;
        }
        databaseUtility->closeDBConnection(Server_DB_CONNECTION_NAME);
    }

    //    Settings settings(APP_NAME, APP_VERSION, SERVICE_NAME);
    //    qWarning()<<settings.getDBType();
    //    qWarning()<<settings.getDBDriver();
    //    qWarning()<<settings.getDBServerHost();
    //    qWarning()<<settings.getDBServerPort();
    //    qWarning()<<settings.getDBServerUserName();
    //    qWarning()<<settings.getDBServerUserPassword();
    //    qWarning()<<settings.getDBName();



    QSqlDatabase db = QSqlDatabase::database(Server_DB_CONNECTION_NAME);
    if(!db.isValid()){
        QSqlError err;
        //        err = databaseUtility->openDatabase(MYSQL_DB_CONNECTION_NAME,
        //                                            REMOTE_SITOY_COMPUTERS_DB_DRIVER,
        //                                            REMOTE_SITOY_COMPUTERS_DB_SERVER_HOST,
        //                                            REMOTE_SITOY_COMPUTERS_DB_SERVER_PORT,
        //                                            REMOTE_SITOY_COMPUTERS_DB_USER_NAME,
        //                                            REMOTE_SITOY_COMPUTERS_DB_USER_PASSWORD,
        //                                            REMOTE_SITOY_COMPUTERS_DB_NAME,
        //                                            HEHUI::MYSQL);
        Settings settings(APP_NAME, APP_VERSION, SETTINGS_FILE_NAME, "./");
        err = databaseUtility->openDatabase(Server_DB_CONNECTION_NAME,
                                            settings.getDBDriver(),
                                            settings.getDBServerHost(),
                                            settings.getDBServerPort(),
                                            settings.getDBServerUserName(),
                                            settings.getDBServerUserPassword(),
                                            settings.getDBName(),
                                            settings.getDBType());
        if (err.type() != QSqlError::NoError) {
            qCritical() << QString("An error occurred when opening the database on '%1'! %2").arg(REMOTE_SITOY_COMPUTERS_DB_SERVER_HOST).arg(err.text());
            return false;
        }

    }

    db = QSqlDatabase::database(Server_DB_CONNECTION_NAME);
    if(!db.isOpen()){
        qCritical()<<QString("Database is not open! %1").arg(db.lastError().text());
        return false;
    }

    if(!query){
        query = new QSqlQuery(db);

        //        recordsInDatabase.clear();
        //        if(query->exec("select ComputerName from clientinfo")){
        //            while(query->next()){
        //                recordsInDatabase.append(query->value(0).toString());
        //            }
        //            qWarning()<<"Records In Database:"<<recordsInDatabase.size();
        //        }
        //        query->clear();

    }


    return true;



}



void IMServer::start()
{

    qDebug()<<"----Server::start()";


    m_packetHandler = 0;
    resourcesManager = ResourcesManagerInstance::instance();
    serverPacketsParser = 0;

    databaseUtility = new DatabaseUtility(this);

    mainServiceStarted = false;


    startMainService();






}

void IMServer::stop()
{
//    if(sendServerOnlinePacketTimer){
//        sendServerOnlinePacketTimer->stop();
//    }
    
    
    if(serverPacketsParser){
        serverPacketsParser->sendServerOfflinePacket();
    }

    databaseUtility->closeAllDBConnections();
    DatabaseUtility::closeAllDBConnections();


    if(m_udpServer){
        m_udpServer->close();
    }
    if(m_rtp){
        m_rtp->stopServers();
    }

    


}

void IMServer::setServerTye(ServerType serverType){
    this->m_serverType = serverType;
}

bool IMServer::startIMServer(){

    if(!serverPacketsParser){

        QString errorMessage = "";
        m_udpServer = resourcesManager->startIPMCServer(QHostAddress(IM_SERVER_IPMC_ADDRESS), quint16(IM_SERVER_IPMC_LISTENING_PORT), &errorMessage);
        if(!m_udpServer){
            qCritical()<<QString("Can not start IP Multicast listening on address '%1', port %2! %3").arg(IM_SERVER_IPMC_ADDRESS).arg(IM_SERVER_IPMC_LISTENING_PORT).arg(errorMessage);
            m_udpServer = resourcesManager->startUDPServer(QHostAddress::AnyIPv4, quint16(IM_SERVER_IPMC_LISTENING_PORT), true, &errorMessage);
        }else{
            qWarning()<<QString("IP Multicast listening on address '%1', port %2!").arg(IM_SERVER_IPMC_ADDRESS).arg(IM_SERVER_IPMC_LISTENING_PORT);
        }

        m_rtp = resourcesManager->startRTP(QHostAddress::Any, IM_SERVER_RTP_LISTENING_PORT, true, &errorMessage);
        //connect(m_rtp, SIGNAL(disconnected(int)), this, SLOT(peerDisconnected(int)));

        serverPacketsParser = new ServerPacketsParser(resourcesManager, this);


        //Single Process Thread
        //QtConcurrent::run(serverPacketsParser, &ServerPacketsParser::run);
        //IMPORTANT For Multi-thread
    //    QThreadPool::globalInstance()->setMaxThreadCount(MIN_THREAD_COUNT);
    //    QtConcurrent::run(serverPacketsParser, &ServerPacketsParser::startparseIncomingPackets);
    //    QtConcurrent::run(serverPacketsParser, &ServerPacketsParser::startparseIncomingPackets);
    //    QtConcurrent::run(serverPacketsParser, &ServerPacketsParser::startprocessOutgoingPackets);
    //    serverPacketsParser->startCheckIMUsersOnlineStateTimer();


        serverPacketsParser->sendServerDeclarePacket(QHostAddress(IM_SERVER_IPMC_ADDRESS), quint16(IM_SERVER_IPMC_LISTENING_PORT));
        //serverPacketsParser->sendServerDeclarePacket(QHostAddress::Broadcast, quint16(IP_MULTICAST_GROUP_PORT), networkManager->localTCPListeningAddress(), networkManager->localTCPListeningPort(), networkManager->hostName());


    }

    return true;

}

bool IMServer::startFileServer(){

    if(!m_fileTransmissionPacketsParser){
        QString m_serverName = QHostInfo::localHostName().toLower();
        m_fileTransmissionPacketsParser = new ServerFileTransmissionPacketsParser(m_serverName, this);
        m_fileTransmissionManager = new ServerFileTransmissionManager(m_serverName, m_fileTransmissionPacketsParser, this);
    }

    return true;

}
































} //namespace HEHUI


