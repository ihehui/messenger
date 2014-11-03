 /****************************************************************************
 * servermanagerwindow.cpp
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



#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>


#include "servermanagerwindow.h"

#include "../settings.h"


namespace HEHUI {

ServerManagerWindow::ServerManagerWindow(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    model = new ServerInfoModel(this);
    ui.tableViewServers->setModel(model);
    ui.tableViewServers->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    connect(ui.tableViewServers, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotServerSelected(const QModelIndex &)));


    clientNetworkManager = ClientResourcesManager::instance();
    //connect(clientNetworkManager, SIGNAL(signalServerDeclarePacketReceived(const QString&, quint16, quint16, const QString&, const QString&)), this, SLOT(serverFound(const QString& , quint16, quint16, const QString&, const QString&)), Qt::QueuedConnection);


    if(clientNetworkManager->getNetworkType() == ClientResourcesManager::LAN){
        ui.lineEditIP->setText(QString(IM_SERVER_IPMC_ADDRESS));
        ui.spinBoxPort->setValue(IM_SERVER_IPMC_LISTENING_PORT);
        ui.toolButtonSearchServer->show();
        ui.toolButtonSearchServer->setEnabled(true);
        ui.toolButtonSearchServer->setFocus();

        ui.toolButtonAddServer->hide();
    }else{
        ui.toolButtonSearchServer->hide();
        ui.toolButtonAddServer->show();
    }


    serversHash.clear();
    slotLoadServers();

    ui.lineEditIP->setFocus();

    //        clientPacketsParser = 0;


}

ServerManagerWindow::~ServerManagerWindow()
{
    slotSaveServers();
    
    delete model;
    model = 0;

    qDeleteAll(serversHash);
    serversHash.clear();
    
    this->disconnect();

}

void ServerManagerWindow::closeEvent(QCloseEvent *event){
    event->accept();

}

void ServerManagerWindow::updateModel(){
    
    slotSaveServers();
    
    model->setServersList(serversHash.values());
    
    ui.tableViewServers->selectRow(0);
    slotUpdateUI();
    
    //emit signalServersUpdated();

}

bool ServerManagerWindow::isIPAddressValid(){
    QRegExpValidator validator(this);
    // Regexp for IP: ^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$
    QRegExp rx("^(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])$");
    validator.setRegExp(rx);

    int pos = 0;
    QString ip = ui.lineEditIP->text();

    if(validator.validate(ip, pos) == QValidator::Acceptable){
        quint8 number = ip.section(".", 0, 0).toUShort();
        if((number >= 224 && number <= 239) || (number == 255)){
            ui.toolButtonSearchServer->setEnabled(true);
            ui.toolButtonSearchServer->show();
            ui.toolButtonAddServer->setEnabled(false);
            ui.toolButtonAddServer->hide();
            ui.spinBoxPort->setValue(IM_SERVER_IPMC_LISTENING_PORT);
        }else{
            ui.toolButtonAddServer->setEnabled(true);
            ui.toolButtonAddServer->show();
            ui.toolButtonSearchServer->setEnabled(false);
            ui.toolButtonSearchServer->hide();
            ui.spinBoxPort->setValue(IM_SERVER_RTP_LISTENING_PORT);
        }
        return true;
    }else{
        ui.toolButtonAddServer->setEnabled(false);
        ui.toolButtonSearchServer->setEnabled(false);
        QMessageBox::critical(this, tr("Error"), tr("Invalid IP address!"));
        ui.lineEditIP->setFocus();
        ui.lineEditIP->end(false);
        return false;
    }

}

void ServerManagerWindow::serverFound(const QString &serverAddress, quint16 serverRTPListeningPort, const QString &serverName, const QString &version){

    qWarning();
    qWarning()<<"Server Found!"<<" Address:"<<serverAddress<<" RTP Port:"<<serverRTPListeningPort<<" Name:"<<serverName<<" Version:"<<version;
    qWarning();

    ServerInfo *info;
    if(serversHash.contains(serverAddress)){
        info = serversHash.value(serverAddress);
    }else{
        info = new ServerInfo(serverAddress, serverRTPListeningPort, this);
    }

    info->setCurState(ServerInfo::TestOK);
    serversHash[serverAddress] = info;


    updateModel();


}

void ServerManagerWindow::slotRequestForLANServer(const QString &ip, quint16 port){
    ui.toolButtonSearchServer->setEnabled(false);
    ui.toolButtonTestServers->setEnabled(false);
    //	serversHash.clear();
    //	updateModel();

    //        if(!clientPacketsParser){
    //            clientPacketsParser = new ClientPacketsParser(this);
    //        }
    //        clientPacketsParser->sendClientLookForServerPacket(QHostAddress(ip), port);
    emit signalLookForServer(QHostAddress(ip), port);
}

void ServerManagerWindow::slotTestServers(){

    //    if(!clientPacketsParser){
    //        clientPacketsParser = new ClientPacketsParser(this);
    //        connect(clientPacketsParser, SIGNAL(signalServerDeclarePacketReceived(const QString&, quint16, quint16, const QString&, const QString&)), this, SLOT(serverFound(const QString& , quint16, quint16, const QString&, const QString&)), Qt::QueuedConnection);
    //    }

    ui.toolButtonTestServers->setEnabled(false);
    foreach(ServerInfo *info, serversHash.values()){
        //        clientPacketsParser->sendClientLookForServerPacket(QHostAddress(info->getIp()), info->getPort());
        info->setCurState(ServerInfo::Testing);
        emit signalLookForServer(QHostAddress(info->getIp()), info->getPort());
    }

    updateModel();

    QTimer::singleShot(5000, this, SLOT(slotTimeout()));

}

void ServerManagerWindow::slotTimeout(){
    foreach(ServerInfo *info, serversHash.values()){
        if(info->getCurState() != ServerInfo::TestOK){
            info->setCurState(ServerInfo::TestFailed);
        }
    }

    updateModel();

    //slotSaveServers();

    ui.toolButtonTestServers->setEnabled(true);

}


void ServerManagerWindow::slotSaveServers(){
    QStringList serverList;
    foreach(ServerInfo *info, serversHash.values()){
        //if(info->getCurState() == ServerInfo::TestOK){
        serverList<<info->getIp() + ":" + QString::number(info->getPort());
        //}
    }

    Settings::instance()->setServers(serverList);
    Settings::instance()->sync();

}

void ServerManagerWindow::slotLoadServers(){
    QStringList serverList = Settings::instance()->getServers();

    foreach(QString server, serverList){
        QStringList values = server.split(":");
        ServerInfo *info = new ServerInfo(values.at(0), values.at(1).toUInt(), this);
        serversHash.insert(values.at(0), info);

    }

    updateModel();


}

void ServerManagerWindow::slotUpdateUI(){
    if(ui.tableViewServers->currentIndex().isValid()){
        ui.toolButtonDeleteServer->setEnabled(true);
    }else{
        ui.toolButtonDeleteServer->setEnabled(false);
    }

    if(ui.tableViewServers->model()->rowCount()){
        ui.toolButtonShowAddServer->setChecked(false);
        ui.toolButtonTestServers->setEnabled(true);
    }else{
        ui.toolButtonShowAddServer->setChecked(true);
        ui.toolButtonTestServers->setEnabled(false);
    }

}

void ServerManagerWindow::on_toolButtonSearchServer_clicked(){
    if(!isIPAddressValid()){
        return;
    }

    slotRequestForLANServer(ui.lineEditIP->text(), ui.spinBoxPort->value());

}

void ServerManagerWindow::on_toolButtonAddServer_clicked(){
    if(!isIPAddressValid()){
        return;
    }

    QString ip = ui.lineEditIP->text();
    quint16 port = ui.spinBoxPort->value();
    ServerInfo *info;
    if(serversHash.contains(ip)){
        info = serversHash.value(ip);
    }else{
        info = new ServerInfo(ip, ui.spinBoxPort->value(), this);
    }

    info->setCurState(ServerInfo::NotTested);
    serversHash[ip] = info;

    updateModel();
    
    emit signalLookForServer(QHostAddress(ip), port);
    
    


}

void ServerManagerWindow::on_toolButtonShowAddServer_toggled(bool checked){
    ui.groupBoxIP->setVisible(checked);

}

void ServerManagerWindow::on_lineEditIP_editingFinished (){

    isIPAddressValid();


}

void ServerManagerWindow::on_toolButtonDeleteServer_clicked(){

    QModelIndex index = ui.tableViewServers->currentIndex();
    if(!index.isValid()){
        return;
    }

    int row = index.row();
    QString ip = index.sibling(row, 0).data().toString();
    ServerInfo *info = serversHash.value(ip);
    serversHash.remove(ip);
    delete info;
    info = 0;
    updateModel();

}

void ServerManagerWindow::on_toolButtonTestServers_clicked(){
    slotTestServers();

}

void ServerManagerWindow::slotServerSelected(const QModelIndex &index){    
    
    if(!index.isValid()){
        return;
    }
    
    slotSaveServers();
    
    QStringList server;

    int row = index.row();
    for(int i=0; i<2; i++){
        QModelIndex idx = index.sibling(row, i);
        server << idx.data().toString();
    }
    
    emit signalServerSelected(server.join(":"));
    
    QDialog *dlg = qobject_cast<QDialog *> (parentWidget());
    if(dlg){
        dlg->accept();
    }
    
    
}

















} //namespace HEHUI


