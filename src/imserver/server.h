/*
 ****************************************************************************
 * server.h
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



#ifndef IMSERVER_H
#define IMSERVER_H

#include <QObject>


#include "serverlib.h"
#include "usersmanager/userinfo.h"
#include "packetmanager/serverpacketsparser.h"
#include "filetransmitter/serverfiletransmissionmanager.h"

#include "../sharedim/resourcesmanagerinstance.h"

#include "HHSharedCore/hdatabaseutility.h"
//#include "HHSharedNetwork/hnetworkmanagerbase.h"



namespace HEHUI {


class SERVERLIB_API IMServer : public QObject
{
    Q_OBJECT
public:
    IMServer(QObject *parent = 0);
    ~IMServer();

    void start();
    void stop();

    void setServerTye(ServerType serverType);

    bool startIMServer();
    bool startFileServer();

signals:


private slots:
    bool startMainService();
//    void sendServerOnlinePacket();
    void saveClientLog(const QString &computerName, const QString &users, const QString &log, const QString &clientAddress);

    void saveFileLog(const QString &sender, const QString &receiver, const QString &fileName, const QString &md5Hex, quint64 size);

    void peerConnected(const QHostAddress &peerAddress, quint16 peerPort);
    void signalConnectToPeerTimeout(const QHostAddress &peerAddress, quint16 peerPort);
    void peerDisconnected(const QHostAddress &peerAddress, quint16 peerPort, bool normalClose);
    void peerDisconnected(int socketID);


private:
    bool openDatabase(bool reopen = false);




private:

    PacketHandlerBase *m_packetHandler;
    ResourcesManagerInstance *resourcesManager;
    ServerPacketsParser *serverPacketsParser;

    ServerFileTransmissionPacketsParser *m_fileTransmissionPacketsParser;
    ServerFileTransmissionManager *m_fileTransmissionManager;

    UDPServer *m_udpServer;
    RTP *m_rtp;



    DatabaseUtility *databaseUtility;
    QSqlQuery *query;

    bool mainServiceStarted;


    QHash<QString, UserInfo *> userInfoHash;

    int onlineAdminsCount;


//    QStringList logs;

    ServerType m_serverType;



};

} //namespace HEHUI

#endif // IMSERVER_H
