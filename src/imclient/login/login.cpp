/*
 * login.cpp
 *
 *  Created on: 2009-9-11
 *      Author: 贺辉
 */

#include <QtSql>
#include <QtGui>
#include <QString>
#include <QMessageBox>
//#include <QLocalSocket>

#include "login.h"
#include "app_constants_client.h"
#include "../settings.h"

//#include "../../shared/gui/databaseconnecter/databaseconnecter.h"
//#include "../../shared/core/utilities.h"

#include "HHSharedGUI/hdatabaseconnecter.h"

namespace HEHUI {

Login::Login(IMUser *user, QObject *parent)
    :LoginBase(user, parent), user(user)
{

    //localServer = new QLocalServer(this);



}

Login::~Login() {

    /*
 if(localServer->isListening()){
        localServer->close();
    }

    localServer->deleteLater();

*/

}

//bool Login::getUserInfo(){
//	return true;
//}


/*
bool Login::canLogin(){
    if(!getUser()->isSingleUserMode() || getUser()->isRestoreMode()){
        return true;
    }

    //标志字符串,防止本机重复登陆
    //Flag String
    QString flagString = getUser()->getUserID() + "@" + Utilities::currentUserNameOfOS();
    QString title = QString(tr("Error"));
    QString text = QString(tr("An error occurred!"));
    QString errorMessage = QString(tr("User '%1' had sign in before!\n").arg(getUser()->getUserID()));
    QString detailMessage = errorMessage;

    QLocalSocket socket;
    socket.connectToServer(flagString);
    if(!socket.waitForConnected(1000)){
            if(!localServer){
                localServer = new QLocalServer(this);
            }

            if(localServer->listen(flagString)){
                    qDebug()<<"----Login::canLogin()~~ Can Login!";
                    return true;
            }else{
                detailMessage += QString(tr("But it seems that the process crashed without closing listen!\n"));
#ifdef Q_OS_WIN32
                detailMessage += QString(tr("Maybe you should restart your session!\n"));
#else//  Q_OS_LINUX
                detailMessage += QString(tr("Maybe you should remove the socket file first!\n"));
#endif

            }

    }

    QMessageBox msgBox(getParentWidget());
     QPushButton *forceLoginButton = msgBox.addButton(tr("&Force Login"), QMessageBox::ActionRole);
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
         qDebug()<<"----Login::canLogin()~~ newFlagString:"<<newFlagString;
         localServer->close();
         return localServer->listen(newFlagString);
     }else if(msgBox.clickedButton() == cancelButton){
         qDebug()<<"XXXXLogin::canLogin()~~ Can Not Login!";
         return false;
     }

     qDebug()<<"XXXXLogin::canLogin()~~ Can Not Login!";
     return false;

}
*/

bool Login::verifyUser() {


    //	DatabaseConnecter dc(this);
    //	if(!dc.isDatabaseOpened(REMOTE_SITOY_COMPUTERS_DB_CONNECTION_NAME,
    //						REMOTE_SITOY_COMPUTERS_DB_DRIVER,
    //						REMOTE_SITOY_COMPUTERS_DB_SERVER_HOST,
    //						REMOTE_SITOY_COMPUTERS_DB_SERVER_PORT,
    //						REMOTE_SITOY_COMPUTERS_DB_USER_NAME,
    //						REMOTE_SITOY_COMPUTERS_DB_USER_PASSWORD,
    //						REMOTE_SITOY_COMPUTERS_DB_NAME,
    //						HEHUI::MYSQL
    //						)){
    //
    //		QMessageBox::critical(getParentWidget(), tr("Fatal Error"), tr("Database Connection Failed! Query Failed!"));
    //		qCritical() << QString("Error: Database Connection Failed! Query Failed!");
    //		return false;
    //	}

    DatabaseConnecter dc(this);
    QString userPrivateDataFilePath = Settings::instance()->getCurrentUserPrivateDataFilePath();
    if(!dc.isDatabaseOpened(LOCAL_USERDATA_DB_CONNECTION_NAME,
                            LOCAL_USERDATA_DB_DRIVER,
                            "",
                            0,
                            "",
                            "",
                            userPrivateDataFilePath,
                            HEHUI::SQLITE
                            )){

        QMessageBox::critical(getParentWidget(), tr("Fatal Error"), tr("Database Connection Failed! Query Failed!"));
        qCritical() << QString("Error: Database Connection Failed! Query Failed!");
        return false;
    }

    QSqlDatabase db;
    db = QSqlDatabase::database(LOCAL_USERDATA_DB_CONNECTION_NAME);
    //QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQueryModel model(this);



    QString queryString =
            QString("select * from userinfo where UID = '%1'") .arg(user->getUserID());

    model.setQuery(QSqlQuery(queryString, db));

    if (model.lastError().type() != QSqlError::NoError) {
        qCritical() << QString("An error occurred when querying the database: %1").arg(model.lastError().text());
        QMessageBox::critical(getParentWidget(), tr("Fatal Error"), tr("An error occurred when querying the database!<br> %1") .arg(model.lastError().text()));
        return false;
    }

    QString userID = QVariant(model.record(0).value("UID")).toString();

    //从数据库取回经Base64编码后的SHA-1加密过的密码,将其还原
    //Fetch the Base64 encoded password which is already encrypted by SHA-1, then decode it and convert it to QCryptographicHash
    QByteArray password = QVariant(model.record(0).value("PassWD")).toByteArray();
    password = QByteArray::fromBase64(password);

    QString role = QVariant(model.record(0).value("UserRole")).toString();

    if (userID.isEmpty()) {
        QMessageBox::critical(getParentWidget(), tr("Authentication Failed"), tr(
                                  "<font color=red>Sorry,I can't find your name!<br>  "
                                  "Please check out your name.</font>"));
        return false;

    } else if (password != user->getPassword()) {
        QMessageBox::critical(getParentWidget(), tr("Authentication Failed"), tr(
                                  "<font color=red>Sorry,Password incorrect!<br>  "
                                  "Please check out your password.</font>"));
        return false;
    } else if(role == ""){
        QMessageBox::critical(getParentWidget(), tr("Authentication Failed"), tr("Cannot access the specified server: <font color=red>Permission Denied!</font>"));
    }else{


        int userRole = QVariant(model.record(0).value("UserRole")).toInt();
        QString userTrueName = QVariant(model.record(0).value("TrueName")).toString();
        QString userNickName = QVariant(model.record(0).value("NickName")).toString();
        QString email = QVariant(model.record(0).value("Email")).toString();
        QDateTime lastLoginTime = QVariant(model.record(0).value("LastLoginTime")).toDateTime();
        QString phone = QVariant(model.record(0).value("Phone")).toString();
        QString lastLoginIP = QVariant(model.record(0).value("LastLoginIP")).toString();
        QString face = QVariant(model.record(0).value("Face")).toString();


        //getUser()->setUserID(userID);
        user->setUserRole(userRole);
        user->setNickName(userNickName);
        user->setTrueName(userTrueName);
        user->setBusinessEmailAddress(email);
//        user->setLastLsetLastLoginExternalHostAddressinIP);
        user->setLastLoginTime(lastLoginTime);
        user->setFace(face);
        user->setBusinessPhoneNumber(phone);

        user->setVerified(true);

        int id = QVariant(model.record(0).value("ID")).toInt();
        //model.setQuery(QSqlQuery(QString("update systemusers set LastLoginTime =(now()) where id=%1").arg(id), db));
        QString currentDateTime = QDateTime::currentDateTime().toString(Qt::ISODate);
        model.setQuery(QSqlQuery(QString("update contacts set LastLoginTime =%1 where ID=%2").arg(currentDateTime).arg(id), db));

        return true;

    }


    return false;
}


/*
 * 检查用户是否验证成功
 * verify the user
 *
 */
bool Login::isVerified() {


    //检查用户是否验证成功
    //verify the user
    bool isSuccesseful = false;


    //RestoreMode 无需服务器密码验证,直接返回真
    //RestoreMode returns true, no need server authentication
    if(user->isRootMode()){
        isSuccesseful = true;
    }else{
        //服务器认证
        //Server Authentication
        isSuccesseful = verifyUser();
    }


    if (!isSuccesseful) {
        //localServer->close();
        qCritical() << QString("Error: Authentication Failed!");
    }

    return isSuccesseful;
}













} //namespace HEHUI
