/*
 ****************************************************************************
 * servermanagerwindow.h
 *
 * Created On: 2010-5-24
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
 * Last Modified On: 2010-5-24
 * Last Modified By: 贺辉
 ***************************************************************************
 */





#ifndef SERVERMANAGERWINDOW_H
#define SERVERMANAGERWINDOW_H

#include <QWidget>
#include "ui_servermanagerwindow.h"

#include "clientresourcesmanager.h"
#include "serverinfo.h"
#include "serverinfomodel.h"
//#include "../packetmanager/clientpacketsparser.h"



namespace HEHUI {

class ServerManagerWindow : public QWidget
{
    Q_OBJECT

public:
    ServerManagerWindow(QWidget *parent = 0);
    ~ServerManagerWindow();


protected:
    void closeEvent(QCloseEvent *event);

private:
    void updateModel();
    bool isIPAddressValid();
    
signals:
    void signalLookForServer(const QHostAddress &targetAddress, quint16 targetPort);
    //    void signalServersUpdated();
    void signalServerSelected(const QString &serverInfoString);
    

public slots:
    void serverFound(const QString &serverAddress, quint16 serverRTPListeningPort, const QString &serverName, const QString &version);
    
private slots:
    void slotRequestForLANServer(const QString &ip = IM_SERVER_IPMC_ADDRESS, quint16 port = quint16(IM_SERVER_IPMC_LISTENING_PORT));
    void slotTestServers();
    void slotTimeout();

    void slotSaveServers();
    void slotLoadServers();

    void slotUpdateUI();

    void on_toolButtonSearchServer_clicked();
    void on_toolButtonAddServer_clicked();
    void on_toolButtonShowAddServer_toggled(bool checked);
    void on_lineEditIP_editingFinished ();
    void on_toolButtonDeleteServer_clicked();
    void on_toolButtonTestServers_clicked();

    void slotServerSelected(const QModelIndex &index);



private:
    Ui::ServerManagerWindowClass ui;


    ClientResourcesManager *clientNetworkManager;
    ServerInfoModel *model;

    QHash<QString/*Server Address*/, ServerInfo *> serversHash;

    //    ClientPacketsParser *clientPacketsParser;


};

} //namespace HEHUI

#endif // SERVERMANAGERWINDOW_H
