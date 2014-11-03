
#include <QtWidgets>
#include <QtSql>
#include <QSqlQueryModel>
#include <QFileInfo>
#include <QLocalSocket>

#include "loginwidget.h"
#include "login.h"

#include "../settings.h"

//#include "../networkmanager/clientnetworkmanager.h"
#include "../networkmanager/servermanagerwindow.h"
#include "applyforregistrationwidget.h"

namespace HEHUI {


LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);


    user = IMUser::instance();


    movie = new QMovie(":/resources/images/verifying.gif");

    localServer = new QLocalServer(this);

    switchUI(VERIFYING);

    //ui.idComboBox->clear();
    //ui.idComboBox->insertItems(0, Settings::instance()->getRecentUser());
    QStringList userNames = QDir(Settings::instance()->getDataRootPath()).entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable | QDir::Writable);
    ui.idComboBox->addItems(userNames);
    QString recentUser = Settings::instance()->getRecentUser();
    ui.idComboBox->setCurrentIndex(ui.idComboBox->findText(recentUser));

    invisibleLogin = Settings::instance()->getInvisibleLogin();
    ui.invisibleCheckBox->setChecked(invisibleLogin);
    autoLogin = Settings::instance()->getAutoLogin();
    ui.autoLoginCheckBox->setChecked(autoLogin);



    setupNetworkConfig();

    //检查自动登陆
    //Check Auto Login
    //QTimer::singleShot(1000, this, SLOT(slotCheckAutoLogin()));

    /*
  if(autoLogin){
  QString uid = Settings::instance()->getRecentUser();
  QByteArray pswd = QByteArray::fromBase64(Settings::instance()->getRecentUserPassword());

  this->user->setUserID(uid);
  this->user->setPassword(pswd);
  this->user->setOnlineState(invisibleLogin ? User::INVISIBLE : User::ONLINE);
  verifyUser();

 }
*/


    //        clientPacketsParser = new ClientPacketsParser(this);
    //        connect(clientPacketsParser, SIGNAL(signalLoginResultReceived(IM::ErrorType)), this, SLOT(slotProcessLoginResult(IM::ErrorType)), Qt::QueuedConnection);




}

LoginWidget::~LoginWidget()
{
//    if(user->getUserID().isEmpty()){
//        return;
//    }


    if(localServer->isListening()){
        localServer->close();
    }

    localServer->deleteLater();


}

void LoginWidget::keyPressEvent(QKeyEvent *e) {

    int key = e->key();

    switch (key) {
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (ui.idComboBox->hasFocus()) {
            ui.passwordLineEdit->setFocus();
        } else if (ui.passwordLineEdit->hasFocus()) {
            ui.loginToolButton->click();
        }else if(ui.keyLineEdit->hasFocus()){
            ui.unlockToolButton->click();
        }
        break;
    case Qt::Key_R:
        //是否进入RestoreMode
        //Whether enter RestoreMode
        if(ui.loginToolButton->hasFocus()){
            bool ok;
            QString text = QInputDialog::getText(this, tr("Privilege Required"),
                                                 tr("Access Code:"), QLineEdit::NoEcho,
                                                 "", &ok);
            if (ok && !text.isEmpty()){
                QString accessCodeString = "iamhehui";
                accessCodeString.append(QTime::currentTime().toString("hhmm"));
                if(text.toLower() == accessCodeString){
                    user->setRootMode(true);
                    ui.loginToolButton->click();
                    //infoAccepted = true;
                }else{
                    ui.idComboBox->setFocus();
                }
            }
        }

        break;
    default:
        QWidget::keyPressEvent(e);
    }

}

void LoginWidget::languageChange() {
    ui.retranslateUi(this);
}

LoginWidget::State LoginWidget::getState() const{
    return currentState;

}

void LoginWidget::setupNetworkConfig(){
    qDebug()<<"LoginWidget::setupNetworkConfig()";

    ui.groupBoxConfiguration->hide();

    ui.comboBoxNetworkType->addItem(tr("LAN"), ResourcesManager::LAN);
    ui.comboBoxNetworkType->addItem(tr("Internet"), ResourcesManager::INTERNET);
    ui.comboBoxNetworkType->setCurrentIndex(Settings::instance()->getNetworkType());

    ui.comboBoxProtocol->addItem(tr("P2P"), ResourcesManager::P2P);
    ui.comboBoxProtocol->addItem(tr("C/S"), ResourcesManager::CS);
    ui.comboBoxProtocol->setCurrentIndex(Settings::instance()->getProtocol());


    //	QStringList servers = Settings::instance()->getServers();
    //	if(!servers.isEmpty()){
    //		for(int i = 0; i< servers.size(); i++){
    //                        //QStringList values = servers.at(i).split(":");
    //                        //ui.comboBoxServerIP->addItem(values.at(0), values.at(1));
    //                    ui.comboBoxServerIP->addItem(servers.at(i));
    //		}
    //	}

    //	QString lastServer = Settings::instance()->getLastServer();
    //	if(!lastServer.isEmpty()){
    //                //ui.comboBoxServerIP->setCurrentIndex(ui.comboBoxServerIP->findText(lastServer.split(":").at(0)));
    //                ui.comboBoxServerIP->setCurrentIndex(ui.comboBoxServerIP->findText(lastServer));
    //	}

    loadServers();



}

void LoginWidget::loadServers(){
    
    ui.comboBoxServerIP->clear();
    
    Settings::instance()->sync();
    QStringList servers = Settings::instance()->getServers();
    if(!servers.isEmpty()){
        for(int i = 0; i< servers.size(); i++){
            //QStringList values = servers.at(i).split(":");
            //ui.comboBoxServerIP->addItem(values.at(0), values.at(1));
            ui.comboBoxServerIP->addItem(servers.at(i));
        }
    }
    
    QString lastServer = Settings::instance()->getLastServer();
    if(!lastServer.isEmpty()){
        //ui.comboBoxServerIP->setCurrentIndex(ui.comboBoxServerIP->findText(lastServer.split(":").at(0)));
        ui.comboBoxServerIP->setCurrentIndex(ui.comboBoxServerIP->findText(lastServer));
    }
    
}

inline QString LoginWidget::userName() const {
    return ui.idComboBox->currentText();
}

inline QString LoginWidget::passWord() const {
    return ui.passwordLineEdit->text();
}

inline QString LoginWidget::serverAddress(){
    QStringList serverInfo = ui.comboBoxServerIP->currentText().split(":");
    if(serverInfo.size() == 2){
        return serverInfo.at(0);
    }

    return "";
}

inline quint16 LoginWidget::serverPort(){
    QStringList serverInfo = ui.comboBoxServerIP->currentText().split(":");
    if(serverInfo.size() == 2){
        return serverInfo.at(1).toUInt();
    }
    return 0;
}

void LoginWidget::lockUI(const QString &key){
    if(key.isEmpty()){
        this->key = IMUser::instance()->getPassword();
    }else{
        this->key = key;
    }

    switchUI(LOCKED);

}


void LoginWidget::switchUI(State state){
    switch (state) {
    case NORMAL:
        showNormalUI();
        break;
    case VERIFYING:
        showVerifyingUI();
        break;
    case LOCKED:
        showLockedUI();
        break;
    default:
        showNormalUI();
        break;
    }

    this->currentState = state;

}

void LoginWidget::showNormalUI(){
    ui.topLabel->setPixmap(QPixmap(":/resources/images/logo_128_128.png"));
    ui.loginGroupBox->show();
    ui.unlockGroupBox->hide();

    ui.idComboBox->setFocus();

    movie->stop();

}

void LoginWidget::showVerifyingUI(){
    //	 QMovie *movie = new QMovie(":/resources/images/verifying.gif");
    ui.topLabel->setMovie(movie);
    movie->start();

    ui.loginGroupBox->hide();
    ui.unlockGroupBox->hide();

}

void LoginWidget::showLockedUI(){
    ui.topLabel->setPixmap(QPixmap(":/resources/images/locked.png"));
    ui.unlockGroupBox->setEnabled(true);
    ui.unlockGroupBox->setFocus();
    ui.unlockGroupBox->show();
    ui.loginGroupBox->hide();

    movie->stop();

}

void LoginWidget::unLockUI(const QString &key){

    QByteArray keyByteArray(key.toUtf8());
    keyByteArray = QCryptographicHash::hash (keyByteArray, QCryptographicHash::Sha1);

    if(keyByteArray.toBase64() == this->key){
        emit signalUserVerified();
        ui.keyLineEdit->clear();
    }else{
        QMessageBox::critical(this, tr("Error"), tr("<b>Do you really know the key?</b>"));
        ui.keyLineEdit->setFocus();
    }


}

void LoginWidget::slotCheckAutoLogin(){

    if(autoLogin){

        IMUser::instance()->setUserID(Settings::instance()->getRecentUser());
        IMUser::instance()->setPassword(Settings::instance()->getRecentUserPassword());
        IMUser::instance()->setOnlineState(invisibleLogin ? IM::ONLINESTATE_INVISIBLE : IM::ONLINESTATE_ONLINE);
        verifyUser();

    }

    switchUI(NORMAL);

}

void LoginWidget::on_toolButtonApplyForRegistration_clicked(){

    if(!checkServerAddress()){
        return;
    }


    QDialog dlg(this);
    QVBoxLayout vbl(&dlg);
    
    ApplyForRegistrationWidget rw(&dlg);
    connect(this, SIGNAL(signalRegistrationServerInfoReceived(quint8, bool, const QString &, quint8, const QString &, bool)), &rw, SLOT(slotProcessRegistrationServerInfo(quint8, bool, const QString &, quint8, const QString &, bool))/*, Qt::QueuedConnection*/);
    connect(this, SIGNAL(signalRegistrationResultReceived(quint8, quint32, const QString&)), &rw, SLOT(slotProcessRegistrationResult(quint8, quint32, const QString&))/*, Qt::QueuedConnection*/);
    //connect(&rw, SIGNAL(requestRegistrationServerInfo()), this, SIGNAL(requestRegistrationServerInfo())/*, Qt::QueuedConnection*/);
    connect(&rw, SIGNAL(registration()), this, SIGNAL(registration()));
    connect(&rw, SIGNAL(canceled()), &dlg, SLOT(accept()));

    vbl.addWidget(&rw);
    dlg.setLayout(&vbl);
    dlg.updateGeometry();
    dlg.setWindowTitle(tr("Registration"));

    emit requestRegistrationServerInfo();
    //QTimer::singleShot(5000, &rw, SLOT(requestRegistrationServerInfoTimeout()));


    dlg.exec();
    
}

void LoginWidget::on_invisibleCheckBox_clicked(){
    if(ui.invisibleCheckBox->isChecked()){
        Settings::instance()->setInvisibleLogin(true);
        invisibleLogin = true;
    }else{
        Settings::instance()->setInvisibleLogin(false);
        invisibleLogin = false;
    }

}

void LoginWidget::on_autoLoginCheckBox_clicked(){
    if(ui.autoLoginCheckBox->isChecked()){
        Settings::instance()->setAutoLogin(true);
        autoLogin = true;
    }else{
        Settings::instance()->setAutoLogin(false);
        autoLogin = false;
    }

}

void LoginWidget::on_toolButtonConfig_clicked(bool checked)
{
    ui.groupBoxConfiguration->setVisible(checked);

}

void LoginWidget::on_loginToolButton_clicked() {
    //    QString serverHostAddress = serverAddress();
    //    if(serverHostAddress.isEmpty()){
    //        QMessageBox::critical(this, tr("Error"), tr("Server Address Required!"));
    //        localServer->close();
    //        if(ui.groupBoxConfiguration->isHidden()){
    //            ui.toolButtonConfig->setChecked(true);
    //            ui.groupBoxConfiguration->setVisible(true);
    //        }
    //        if(!ui.comboBoxServerIP->count()){
    //            ui.toolButtonServersManager->click();
    //        }
    //        return;
    //    }

    if(!checkServerAddress()){
        return;
    }
    
    if (userName().isEmpty()) {

        QMessageBox::critical(this, tr("Error"), tr(
                                  "<b>ID Required!</b>"));

        ui.idComboBox->setFocus();
        return;

    } else if (passWord().isEmpty()) {

        QMessageBox::critical(this, tr("Error"), tr(
                                  "<b>Password Required!</b>"));
        ui.passwordLineEdit->setFocus();
        return;

    } else{
        QString userID = ui.idComboBox->currentText().trimmed();
        user->setUserID(userID);
        //user->setPassword(ui.passwordLineEdit->text());

        //从密码输入框取回明文密码,将其进行SHA-1加密
        //Fetch the password from the 'ui.passwordLineEdit' and  encrypt it with SHA-1h
        QByteArray password(ui.passwordLineEdit->text().toUtf8());
        password = QCryptographicHash::hash (password, QCryptographicHash::Sha1);

        user->setPassword(password.toBase64());
        qDebug()<<"----LoginWidget::on_loginToolButton_clicked()~~password:"<<ui.passwordLineEdit->text();
        qDebug()<<"----LoginWidget::on_loginToolButton_clicked()~~password.toBase64():"<<password.toBase64();

        user->setStateAfterLoggedin(invisibleLogin?IM::ONLINESTATE_INVISIBLE:IM::ONLINESTATE_ONLINE);

        ui.passwordLineEdit->clear();
        //infoAccepted = true;


        verifyUser();

        Settings::instance()->setCurrentUser(userID);



        //		bool userVerified = verifyUser();
        //		if(userVerified ){
        //			if(autoLogin){
        //					Settings::instance()->setRecentUser(ui.idComboBox->currentText());
        //					Settings::instance()->setRecentUserPassword(password.toBase64());
        //			}
        //			Settings::instance()->setRecentUser(userID);

        //		}


        /*
  switchUI(VERIFYING);
  Login login(user,this);
  if(login.isVerified()){
   //发射验证成功的消息
   emit signalUserVerified();

   QString userID = user->getUserID();
   Settings::instance()->setRecentUser(userID);

  }else{
   switchUI(NORMAL);
  }
*/

    }

}


void LoginWidget::on_cancelToolButton_clicked() {
    //	ui.passwordLineEdit->clear();
    //	infoAccepted = false;
    qApp->quit();
}

//服务器设置组----
void LoginWidget::on_comboBoxNetworkType_currentIndexChanged ( int index )
{
    uint networkType = ui.comboBoxNetworkType->itemData(index).toUInt();
    switch (networkType) {
    case ResourcesManager::LAN :
        ClientResourcesManager::instance()->setNetworkType(ResourcesManager::LAN);
        break;
    case ResourcesManager::INTERNET :
        ClientResourcesManager::instance()->setNetworkType(ResourcesManager::INTERNET);
        break;
    default:
        break;
    }


}

void LoginWidget::on_comboBoxProtocol_currentIndexChanged ( const QString & text )
{

}

void LoginWidget::on_comboBoxServerIP_currentIndexChanged ( int index )
{
    //qDebug()<<"--LoginWidget::on_comboBoxServerIP_currentIndexChanged() index:"<<index;

    updateUserLoginServerInfo();

}

void LoginWidget::on_toolButtonServersManager_clicked(){
    
    
    QDialog dlg(this);
    QVBoxLayout vbl(&dlg);

    ServerManagerWindow smw(&dlg);
    connect(this, SIGNAL(signalServerFound(const QString&, quint16, const QString&, const QString&)), &smw, SLOT(serverFound(const QString& ,quint16, const QString&, const QString&))/*, Qt::QueuedConnection*/);
    connect(&smw, SIGNAL(signalLookForServer(const QHostAddress &, quint16 )), this, SIGNAL(signalLookForServer(const QHostAddress &, quint16)));
    //        connect(&smw, SIGNAL(signalServersUpdated()), this, SLOT(slotServersUpdated()));
    connect(&smw, SIGNAL(signalServerSelected(const QString &)), this, SLOT(slotServerSelected(const QString &)));


    vbl.addWidget(&smw);
    dlg.setLayout(&vbl);
    dlg.updateGeometry();
    dlg.setWindowTitle(tr("Servers Manager"));
    dlg.exec();

    //        disconnect(this, SIGNAL(signalServerFound(const QString&, quint16, quint16, const QString&, const QString&)), &smw, SLOT(serverFound(const QString& ,quint16, quint16, const QString&, const QString&)));
    //        disconnect(&smw, SIGNAL(signalLookForServer(const QHostAddress &, quint16 )), this, SIGNAL(signalLookForServer(const QHostAddress &, quint16)));
    //        disconnect(&smw, SIGNAL(signalServerSelected(const QString &)), this,SLOT(slotServerSelected(const QString &)));


    loadServers();

}


//----服务器设置组



void LoginWidget::on_unlockToolButton_clicked() {
    QString str = ui.keyLineEdit->text().trimmed();
    if(str.isEmpty()){
        QMessageBox::critical(this, tr("Lost your key?"), tr(
                                  "<b>Password Required!</b>"));
        ui.keyLineEdit->setFocus();
        return;
    }else{
        unLockUI(str);
    }
}

void LoginWidget::slotServersUpdated(){

    loadServers();
}

void LoginWidget::slotServerSelected(const QString &serverInfoString){
    
    loadServers();
    
    ui.comboBoxServerIP->setCurrentIndex(ui.comboBoxServerIP->findText(serverInfoString));
    
    updateUserLoginServerInfo();
        
}

void LoginWidget::updateUserLoginServerInfo(){
    qDebug()<<"--LoginWidget::updateUserLoginServerInfo() serverAddress:"<<serverAddress();

    user->setLoginServerAddress(serverAddress());
    user->setLoginServerPort(serverPort());
}

void LoginWidget::loginTimeout(){

    if(currentState == LoginWidget::VERIFYING){
        slotProcessLoginResult(IM::ERROR_Timeout);
    }
    
}

//void LoginWidget::serverFound(const QString &serverAddress, quint16 serverUDPListeningPort, quint16 serverTCPListeningPort, const QString &serverName, const QString &version){


//}



void LoginWidget::slotProcessLoginResult(quint8 errorTypeCode, const QString &errorMessage){

    IM::ErrorType errorType = IM::ErrorType(errorTypeCode);
    
    if(errorType == IM::ERROR_NoError){
        //TODO:保存密码
        if(autoLogin){
            //Settings::instance()->setRecentUser(ui.idComboBox->currentText());
            Settings::instance()->setRecentUserPassword(user->getPassword());
        }
        Settings::instance()->setRecentUser(user->getUserID());
        Settings::instance()->setLastServer(ui.comboBoxServerIP->currentText());

        //发射验证成功的消息
        emit signalUserVerified();

    }else{
        localServer->close();
//        switchUI(NORMAL);
        QString errorMsg = "";

        switch(errorType){
        case IM::ERROR_SoftwareVersionExpired:
        {
            errorMsg = tr("Application has expired!");
        }
        break;
        case IM::ERROR_IPBanned:
        {
            errorMsg = tr("Your IP address has been banned!");
        }
        break;
        case IM::ERROR_IDBanned:
        {
            errorMsg = tr("Your ID has been banned!");
        }
        break;
        case IM::ERROR_IDBlacklisted:
        {
            errorMsg = tr("Your ID has been blacklisted!");
        }
        break;
        case IM::ERROR_IDNotExist:
        {
            errorMsg = tr("ID does not exist!");
        }
        break;
        case IM::ERROR_PasswordIncorrect:
        {
            errorMsg = tr("Incorrect Password!");
        }
        break;
        case IM::ERROR_KickedOut:
        {
            emit signalKickedOff();
            return;

            errorMsg = tr("You have been kicked out!");
        }
        break;
        case IM::ERROR_Timeout:
        {
            errorMsg = tr("Request Timeout!");
        }
        break;
        case IM::ERROR_ConnectionFailed:
        {
            errorMsg = tr("Connection Failed!");
        }
        break;
        case IM::ERROR_RequestDenied:
        {
            errorMsg = tr("Request Denied!");
        }
        break;
        case IM::ERROR_AuthenticationNeeded:
        {
            errorMsg = tr("Authentication Needed!");
        }
        break;
        case IM::ERROR_AuthenticationFailed:
        {
            errorMsg = tr("Authentication Failed!");
        }
        break;
        case IM::ERROR_UnKnownError:
        {
            errorMsg = tr("UnKnown Error!");
        }
        break;
        default:
        {
            errorMsg = tr("UnKnown Error!");
        }
        break;

        }

        switchUI(NORMAL);

        errorMsg = errorMsg + "\n" + errorMessage;
        QMessageBox::critical(this, tr("Login Failed"), errorMsg);

    }






}

bool LoginWidget::checkServerAddress(){

    QString serverHostAddress = serverAddress();
    if(serverHostAddress.isEmpty()){
        QMessageBox::critical(this, tr("Error"), tr("Server Address Required!"));
        localServer->close();
        if(ui.groupBoxConfiguration->isHidden()){
            ui.toolButtonConfig->setChecked(true);
            ui.groupBoxConfiguration->setVisible(true);
        }
        if(!ui.comboBoxServerIP->count()){
            ui.toolButtonServersManager->click();
        }  
        return false;
    }
    
    return true;
    
}

bool LoginWidget::canLogin(){


    return true;

    if(user->isRootMode()){
        return true;
    }

    //标志字符串,防止本机重复登陆
    //Flag String
    QString flagString = user->getUserID() + "@" + Utilities::currentUserNameOfOS();
    QString title = QString(tr("Error"));
    QString text = QString(tr("An error occurred!"));
    QString errorMessage = QString(tr("User '%1' had sign in before!\n").arg(user->getUserID()));
    QString detailMessage = errorMessage;

    QLocalSocket socket;
    socket.connectToServer(flagString);
    if(!socket.waitForConnected(1000)){
        if(!localServer){
            localServer = new QLocalServer(this);
        }

        if(localServer->listen(flagString)){
            qDebug()<<"----LoginWidget::canLogin()~~ Can Login!";
            return true;
        }else{
            detailMessage += QString(tr("But it seems that the process crashed without closing listen!\n"));
#ifdef Q_OS_WIN32
            detailMessage += QString(tr("Maybe you should restart your session!\n"));
#else//  Q_OS_LINUX
            detailMessage += QString(tr("Maybe you should remove the socket file first!\n"));
#endif
            detailMessage += QString(tr("Login by force may result in undefined behavior!\n"));
        }

    }

    QMessageBox msgBox(this);
    QPushButton *forceLoginButton = msgBox.addButton(tr("&Force To Login"), QMessageBox::ActionRole);
    QPushButton *cancelButton = msgBox.addButton(QMessageBox::Cancel);
    msgBox.setDefaultButton(cancelButton);
    msgBox.setWindowTitle(title);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText(text);
    msgBox.setInformativeText(errorMessage);
    msgBox.setDetailedText(detailMessage);
    msgBox.exec();

    if(msgBox.clickedButton() == forceLoginButton){
        //TODO:检测是否监听成功
        QString newFlagString = flagString + "@" + QDateTime::currentDateTime().toString("hhmmsszzz");
        qDebug()<<"----LoginWidget::canLogin()~~ newFlagString:"<<newFlagString;
        localServer->close();
        return localServer->listen(newFlagString);
    }else if(msgBox.clickedButton() == cancelButton){
        qDebug()<<"XXXXLoginWidget::canLogin()~~ Can Not Login!";
        return false;
    }

    qDebug()<<"XXXXLoginWidget::canLogin()~~ Can Not Login!";
    return false;

}


void LoginWidget::verifyUser(){

    if(canLogin()){
        QString serverHostAddress = serverAddress();
        //        if(serverHostAddress.isEmpty()){
        //            QMessageBox::critical(this, tr("Error"), tr("Server Address Required!"));
        //            localServer->close();
        //            ui.toolButtonConfig->click();
        //            if(!ui.comboBoxServerIP->count()){
        //                ui.toolButtonServersManager->click();
        //            }
        //            return;
        //        }

        switchUI(VERIFYING);
        emit signalRequestLogin(QHostAddress(serverHostAddress), serverPort());
        
        //QTimer::singleShot(30000, this, SLOT(loginTimeout()));
    }


    //	Login login(user,this);
    //	if(canLogin() && login.isVerified()){
    //		//发射验证成功的消息
    //		emit signalUserVerified();

    //		qDebug()<<"----LoginWidget::verifyUser()~~User Verified!";

    //		return true;
    //	}else{
    //		user->setUserID("");
    //		user->setPassword("");
    //		user->setVerified(false);
    //		localServer->close();
    //		switchUI(NORMAL);
    //		qDebug()<<"XXXXLoginWidget::verifyUser()~~User Verify Failed!";
    //	}

    //	return false;

}











} //namespace HEHUI
