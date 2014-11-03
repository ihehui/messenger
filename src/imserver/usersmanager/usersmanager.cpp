/*
 ****************************************************************************
 * usersmanager.cpp
 *
 * Created on: 2009-9-12
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
  * Last Modified on: 2010-05-22
  * Last Modified by: 贺辉
  ***************************************************************************
*/




#include <QDebug>
#include <QSqlError>

#include "usersmanager.h"

#include "../app_constants_server.h"
#include "../settings.h"
#include "../serverutilities.h"


//#include "../../shared/global_shared.h"

//#include "../../../shared/core/database/databaseutility.h"
//#include "../../../shared/gui/databaseconnecter/databaseconnecter.h"
//#include "../../../shared/core/user.h"

#include "HHSharedCore/hdatabaseutility.h"



namespace HEHUI {



//QHash<QString, IMUserBase*> UsersManager::imUsersHash = QHash<QString, IMUserBase*>();
//QTimer * UsersManager::timer = new QTimer();

QMutex * UsersManager::onlineUserMutex = new QMutex();
QHash<QString/*User ID*/, UserInfo*> * UsersManager::onlineUserInfoHash = new QHash<QString, UserInfo*> ();
QMutex * UsersManager::offlineUserMutex = new QMutex();
QHash<QString/*User ID*/, UserInfo*> * UsersManager::offlineUserInfoHash = new QHash<QString, UserInfo*> ();


QMutex * UsersManager::groupMutex = new QMutex();
QHash<quint32/*Group ID*/, InterestGroup*> * UsersManager::m_interestGroupHash = new QHash<quint32, InterestGroup*> ();



UsersManager::UsersManager(QObject *parent)
//:QObject(parent)
{
    openDatabase();
}

UsersManager::~UsersManager() {
    // TODO Auto-generated destructor stub
    
}



UserInfo* UsersManager::getUserInfo(const QString &imUserID){
    
    if(imUserID.trimmed().isEmpty()){
        return 0;
    }
    
    UserInfo * userInfo = getOnlineUserInfo(imUserID);
    //    if(!userInfo){
    //        userInfo = getOnlineUserInfo(imUserID);
    //    }
    if(!userInfo){
        userInfo = getOfflineUserInfo(imUserID);
    }
    if(!userInfo){
        userInfo = queryUserInfo(imUserID);
    }
    return userInfo;

}

UserInfo* UsersManager::getCachedUserInfo(const QString &imUserID){

    if(imUserID.trimmed().isEmpty()){
        return 0;
    }

    UserInfo * userInfo = getOnlineUserInfo(imUserID);
    if(!userInfo){
        userInfo = getOfflineUserInfo(imUserID);
    }

    return userInfo;

}

UserInfo* UsersManager::getOnlineUserInfo(const QString &imUserID){
    QMutexLocker locker(onlineUserMutex);
    UserInfo * userInfo = 0;
    if(onlineUserInfoHash->contains(imUserID)){
        userInfo = onlineUserInfoHash->value(imUserID);
    }
    return userInfo;
}

UserInfo* UsersManager::getOfflineUserInfo(const QString &imUserID){
    QMutexLocker locker(offlineUserMutex);
    UserInfo * userInfo = 0;
    if(offlineUserInfoHash->contains(imUserID)){
        userInfo = offlineUserInfoHash->value(imUserID);
    }
    return userInfo;
}

QList<UserInfo*> UsersManager::onlineUsers(){
    QMutexLocker locker(onlineUserMutex);
    return onlineUserInfoHash->values();
}

void UsersManager::userOnline(UserInfo* userInfo){

    if(userInfo){
        QString userID = userInfo->getUserID();
        userInfo->setOnline();

        QMutexLocker onlineLocker(onlineUserMutex);
        onlineUserInfoHash->insert(userID, userInfo);

        QMutexLocker offlineLocker(offlineUserMutex);
        offlineUserInfoHash->remove(userID);

    }

}

void UsersManager::userOffline(UserInfo* userInfo){

    if(userInfo){
        QString userID = userInfo->getUserID();
        userInfo->setOffline();

        QMutexLocker onlineLocker(onlineUserMutex);
        onlineUserInfoHash->remove(userID);

        QMutexLocker offlineLocker(offlineUserMutex);
        offlineUserInfoHash->insert(userID, userInfo);

        saveUserInfoToDatabase(userInfo);

        qWarning()<<QString("User %1 Offline!").arg(userID);
    }


}

bool UsersManager::canUserLogin(const QString &userID, const QString &clientVersion, IM::ErrorType *errorType){
    //    qWarning()<<"--canUserLogin(...)";
    
    UserInfo *userInfo = getUserInfo(userID);
    //TODO:检查用户是否可以登陆及可登陆的服务器
    if(userInfo){
        if(userInfo->isBanned()){
            *errorType = IM::ERROR_IDBanned;
            return false;
        }

    }else{
        *errorType = IM::ERROR_IDNotExist;
        return false;
    }

    *errorType = IM::ERROR_NoError;
    return true;

}

void UsersManager::getUserLoginServer(const QString &userID, QHostAddress *serverAddress, quint16 *serverPort){
    //    qWarning()<<"--getUserLoginServer(...)";
    
    //TODO:处理登陆服务器
    if(serverAddress){
        //        serverAddress = QHostAddress::Null;
    }
    if(serverPort){
        //        serverPort = 0;
    }

}

//UserInfo* UsersManager::logUserIn(const QString &userID, const QByteArray &encryptedPassword, IM::OnlineState loginState, IM::ErrorType *errorType){
//    //qWarning()<<"logUserIn(...)";

//    UserInfo *userInfo = getUserInfo(userID);
//    if(!userInfo){
//        *errorType = IM::ERROR_IDNotExist;
//        return 0;
//    }

//    //TODO:密码保存方式
//    QByteArray decryptedPassword;
//    Cryptography cryptography;
//    cryptography.teaCrypto(&decryptedPassword, encryptedPassword, userInfo->encryptedPassword(), false);

//    if(decryptedPassword != QByteArray::fromBase64(userInfo->getPassword().toUtf8())){
//        *errorType = IM::ERROR_PasswordIncorrect;
//        userInfo->authenticationFailed();
//        return 0;
//    }else{

//        //Check Duplicate Login
//        if(userInfo->getOnlineState() != IM::ONLINESTATE_OFFLINE){
//            sendClientLoginFailedPacket(userInfo->getSocketID(), userID, quint8(IM::ERROR_KickedOut));
//            processUserOnlineStatusChanged();
//        }



//        *errorType = IM::ERROR_NoError;
//        userInfo->setOnlineState(loginState);
//        userInfo->setSessionEncryptionKey(ServerUtilities::generateSessionEncryptionKey());
//        userOnline(userInfo);


//        //Get contact groups info
//        getUserAllContactGroupsInfoFromDatabase(userInfo);

//        //Load interest groups
//        getUserInterestGroupsFromDB(userInfo);

//        //Load Last login info
//        getUserLastLoginInfo(userInfo);

//        //qWarning()<<"getSessionEncryptionKey:"<<userInfo->getSessionEncryptionKey().toBase64();
//    }


//    return userInfo;
//}

bool UsersManager::registerNewUser(const QString &userID, const QString &password, IM::ErrorType *errorType, quint32 *sysID, QString *message){
    
    
    UserInfo *userInfo = getUserInfo(userID);
    if(userInfo){
        *errorType = IM::ERROR_IDEXISTED;
        return false;
    }
    
    if(!db.isValid()){
        if(!openDatabase()){
            *errorType = IM::ERROR_UnKnownError;
            return false;
        }
    }
    QSqlQuery query(db);
    //QString statement = QString("insert into users_detailed_info(UserID, UserPassword) values('%1', '%2') ").arg(userID).arg(password);
    QString statement = QString("call sp_CreateNewUser('%1', '%2', @sysID);").arg(userID).arg(password);
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not add new user info to database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;

        //TODO:数据库重启，重新连接
        //MySQL数据库重启，重新连接
        if(error.number() == 2006){
            query.clear();
            openDatabase(true);
        }
        
        *errorType = IM::ERROR_UnKnownError;
        return false;
    }

    statement = "select @sysID;";
    query.exec(statement);
    if(!query.first()){
        qCritical()<<QString("Can not query user SysID! Invalid record! User ID:%1").arg(userID);
        return false;
    }

    if(sysID){
        *sysID = query.value(0).toUInt();
        qDebug()<<"---------------------------------sysID:"<<*sysID;
        if(*sysID == 0){
            *errorType = IM::ERROR_UnKnownError;
            return false;
        }
    }


    
    userInfo = getUserInfo(userID);
    if(!userInfo){
        *errorType = IM::ERROR_UnKnownError;
        return false;
    }
    
    
    *errorType = IM::ERROR_NoError;
    return true;
    
}

void UsersManager::updateUserPassword(const QString &userID, const QString &newPassword, IM::ErrorType *errorType, QString *message){
    UserInfo *userInfo = getUserInfo(userID);
    if(!userInfo){
        return ;
    }

    userInfo->setPassword(newPassword);
    userInfo->addUpdatedPersonalInfoProperty(IM::PI_Password, "'"+newPassword+"'", true);
    
    if(saveUserInfoToDatabase(userInfo)){
        *errorType = IM::ERROR_NoError;
    }else{
        *errorType = IM::ERROR_UnKnownError;
    }

}

QString UsersManager::searchContact(const QString &propertiesString, bool matchExactly, bool searchOnlineUsersOnly, bool searchWebcamUsersOnly, int startIndex){
    qDebug()<<"propertiesString:"<<propertiesString;
    
    if(propertiesString.trimmed().isEmpty()){
        return QString();
    }
    
    QStringList propertiesList = propertiesString.split(QString(CONTACT_INFO_SEPARATOR));
    if(propertiesList.size() != 6){
        qWarning()<<"ERROR! Invalid propertiesString!";
        return QString();
    }

    QString userID = propertiesList.at(0).trimmed();
    QString nickName = propertiesList.at(1);
    QString ageSectionString = propertiesList.at(2);
    QString genderString = propertiesList.at(3);
    QString hometown = propertiesList.at(4);
    QString businessAddress = propertiesList.at(5);

    IMUserBase::AgeSection as = IMUserBase::AgeSection(ageSectionString.toUInt());
    quint8 startAge = 0, endAge = 120;
    switch(as){
    case IMUserBase::Age_Any:
        break;
    case IMUserBase::Age_1_18:
        startAge = 1;
        endAge = 18;
        break;
    case IMUserBase::Age_19_30:
        startAge = 19;
        endAge = 30;
        break;
    case IMUserBase::Age_31_40:
        startAge = 31;
        endAge = 40;
        break;
    case IMUserBase::Age_40_:
        startAge = 41;
        endAge = 120;
        break;
    default:
        break;
    }

    //    User::Gender gd = User::Gender(genderString.toInt());

    quint32 pageSize = SEARCH_RESULT_PAGE_SIZE;

    QString queryString ;

    if(matchExactly){
        if(userID.isEmpty() && nickName.trimmed().isEmpty()){
            return QString();
        }
        queryString = QString("call sp_Contact_Search_MatchExactly('%1', '%2');").arg(userID).arg(nickName);
    }else{
        queryString = QString("call sp_Contact_Search_MatchWildcard(%1, %2, %3, '%4', '%5', %6, %7);").arg(startAge).arg(endAge).arg(genderString.toUInt()).arg(hometown).arg(businessAddress).arg(startIndex).arg(pageSize);
    }
    qDebug()<<"----queryString:"<<queryString;

    if(!db.isValid()){
        if(!openDatabase()){
            return QString();
        }
    }
    QSqlQuery query(db);
    
    if(!query.exec(queryString)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not search contact for user from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;

        return QString();
    }
    
    QStringList usersInfoList;
    while(query.next()){
        //TODO:query安全性
        QString userID = query.value(0).toString();
        IM::OnlineState onlineState = IM::ONLINESTATE_OFFLINE;
        if(searchOnlineUsersOnly){
            UserInfo *info = getOnlineUserInfo(userID);
            if(!info || (info->getOnlineState() != IM::ONLINESTATE_ONLINE)){
                continue;
            }else{
                onlineState = IM::ONLINESTATE_ONLINE;
            }
        }
        
        //TODO
        //FORMAT:UserID,NickName,Gender,Age,Face,FriendshipApply,BusinessAddress,OnlineState
        QStringList userInfoList;
        userInfoList.append(userID);
        for(int i=1; i<7; i++){
            userInfoList.append(query.value(i).toString());
            //qWarning()<<i<<":"<<query.value(i).toString();
        }
        userInfoList.append(QString::number(onlineState));
        
        usersInfoList.append(userInfoList.join(QChar(SEPARTOR_RECORD)));
    }
    
    return usersInfoList.join(QChar(SEPARTOR_GROUP));
    
}

bool UsersManager::saveCachedChatMessageFromIMUser(const QString &senderID, const QString &receiverID, const QString &message){

    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);

    QString msg = message;
    msg.replace("'", "\\'");
    msg.replace("\"", "\\\"");
    QString statement = QString("call sp_CacheChatMessage_FromContact_Save('%1', '%2', '%3' )  ").arg(senderID).arg(receiverID).arg(msg);

    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not save contact chat message to database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }


    //    if((query.lastError().type() != QSqlError::NoError)){
    //        qCritical()<<QString("Can not save data to database! Error: %1").arg(query.lastError().text());
    //        return false;
    //    }

    return true;

}

QStringList UsersManager::cachedChatMessagesForIMUser(UserInfo* userInfo){
    //TODO:

    if(!userInfo){
        return QStringList();
    }

    if(!db.isValid()){
        if(!openDatabase()){
            return QStringList();
        }
    }
    QSqlQuery query(db);

    QString imUserID = userInfo->getUserID();

    //QString statement = QString("select SenderID, Message, TransmittingTime from cachedchatmessages where RecieverID='%1' and TransmittingTime>'%2'  ").arg(imUserID).arg(userInfo->getLastLoginTime().toString("yyyy-MM-dd hh:mm:ss"));
    //QString statement = QString("select SenderID, Message, TransmittingTime from cachedchatmessages where RecieverID='%1' ").arg(imUserID);
    QString statement = QString("call sp_CacheChatMessage_FromContact_Get('%1', '%2'); ").arg(imUserID).arg(userInfo->getLastLogoutTime().toString("yyyy-MM-dd hh:mm:ss"));

    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not query chat messages from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;

        return QStringList();
    }

    if((query.lastError().type() != QSqlError::NoError)){
        qCritical()<<QString("Can not query chat messages from database! Error: %1").arg(query.lastError().text());
        return QStringList();
    }

    QStringList messages;
    while(query.next()){
        QStringList message;
        message.append(QVariant(query.value(0)).toString());
        message.append(QVariant(query.value(1)).toString());
        message.append(QVariant(query.value(2)).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

        //MESSAGE FORMAT:Sender 0X1E Message 0X1E Time
        messages.append(message.join(QString(PACKET_DATA_SEPARTOR)));
    }

    //    statement = QString("delete from cachedchatmessages where RecieverID='%1' ").arg(imUserID);
    //    query.exec(statement);


    qWarning()<<"messages cached on server:"<<messages.size();

    return messages;

}

bool UsersManager::saveUserLastLoginInfo(UserInfo* userInfo, const QString &userHostAddress,quint16 userHostPort, const QString &deviceInfo){
    qDebug()<<"--UsersManager::saveUserLoginInfo(...)";

    if(!userInfo){
        return false;
    }

    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);

    QString imUserID = userInfo->getUserID();

    userInfo->setLastLoginExternalHostAddress(userHostAddress);
    userInfo->setLastLoginExternalHostPort(userHostPort);
    userInfo->setLastLoginDeviceInfo(deviceInfo);


    //statement = QString("insert into loginhistories(UserID, IPAddress, LoginTime) values('%1', '%2', '%3') ").arg(imUserID).arg(userHostAddress).arg(curTime);
    QString statement = QString(" call sp_LogUserIn('%1', '%2', %3, '%4', @loginTme);  ").arg(imUserID).arg(userHostAddress).arg(userHostPort).arg(deviceInfo);
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not save user login info into database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }

    statement = QString("select @loginTme;");
    query.exec(statement);
    if(!query.first()){
        qCritical()<<QString("Can not get user login time from database! Error: %1").arg(query.lastError().text());
        return false;
    }

    userInfo->setLastLoginTime(query.value(0).toDateTime());
    qDebug()<<"------------Login Time:"<<query.value(0).toString();

    return true;

}

bool UsersManager::saveUserLastLogoutInfo(UserInfo* userInfo){
    qDebug()<<"--UsersManager::saveUserLastLogoutInfo(...)";

    if(!userInfo){
        return false;
    }

    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);

    QString imUserID = userInfo->getUserID();
    QString loginTime = userInfo->getLastLoginTime().toString("yyyy-MM-dd hh:mm:ss");
    qDebug()<<"-------------loginTime:"<<loginTime;

    QString statement = QString(" call sp_LogUserOut('%1', '%2', @LogoutTime) ; ").arg(imUserID).arg(loginTime);
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not save user logout info into database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }

    statement = QString("select @LogoutTime; ");
    query.exec(statement);
    if(!query.first()){
        qCritical()<<QString("Can not get user logout time from database! Error: %1").arg(query.lastError().text());
        return false;
    }

    userInfo->setLastLogoutTime(query.value(0).toDateTime());
    qDebug()<<"------------logout Time:"<<query.value(0).toString();


    return true;

}

bool UsersManager::getUserLastLoginInfo(UserInfo *userInfo){

    if(!userInfo){
        return false;
    }

    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);

    QString imUserID = userInfo->getUserID();
    //QString statement = QString("SELECT IPAddress, LoginTime FROM loginhistories where ID = (select max(ID) from loginhistories where UserID = '%1')  ").arg(imUserID);
    QString statement = QString("call sp_GetUserLastLoginInfo('%1'); ").arg(imUserID);

    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not query user login info from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }

    if(!query.first()){
        qCritical()<<QString("No user login info returned from database! Error: %1").arg(query.lastError().text());
        return false;
    }


    userInfo->setLastLoginExternalHostAddress(query.value(0).toString());
    userInfo->setLastLoginTime(query.value(1).toDateTime());
    userInfo->setLastLogoutTime(query.value(2).toDateTime());
    userInfo->setLastLoginDeviceInfo(query.value(3).toString());

    return true;

}

bool UsersManager::addNewContactForUserInDB(const QString &userID, const QString &contactID, quint32 contactGroupID){


    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);

    QString statement = QString("call sp_Contact_Add('%1', '%2', %3);  ").arg(userID).arg(contactID).arg(contactGroupID);
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not add contact for user from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }


    return true;

}


bool UsersManager::deleteContactForUserFromDB(const QString &userID, const QString &contactID, bool deleteMeFromOpposition, bool addToBlacklist){


    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);

    QString statement = QString("call sp_Contact_Delete('%1', '%2', %3, %4);  ").arg(userID).arg(contactID).arg(deleteMeFromOpposition?1:0).arg(addToBlacklist?1:0);
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not delete contact for user from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }


    return true;

}

bool UsersManager::moveContactForUserInDB(UserInfo *userInfo, const QString &contactID, quint32 newGroupID){

    if(!userInfo){
        return false;
    }
    int oldGroupID = userInfo->groupIDThatContactBelongsTo(contactID);
    if(oldGroupID == ContactGroupBase::Group_Strangers_ID){
        return false;
    }

    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);

    QString statement = QString("call sp_Contact_MoveToAnotherGroup('%1', '%2', %3);  ").arg(userInfo->getUserID()).arg(contactID).arg(newGroupID);
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not move contact for user in database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }

    userInfo->moveContactToAnotherGroup(contactID, oldGroupID, newGroupID);

    return true;

}


bool UsersManager::addOrDeleteBlacklistedContactForUserFromDB(const QString &userID, const QString &contactID, bool addToBlacklist){


    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);

    QString statement = QString("call sp_Contact_Blacklisted_AddOrDelete('%1', '%2', %3);  ").arg(userID).arg(contactID).arg(addToBlacklist?1:0);
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not add or delete blacklisted contact for user from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }


    return true;

}


//void UsersManager::slotFetchIMUsersInfo(){

////	QString groupQueryString = QString("select * from contactgroups");

////		QSqlQueryModel *model = new QSqlQueryModel();
////		QSqlQueryModel *contactsModel = new QSqlQueryModel();

////		model->setQuery(queryDatabase(groupQueryString, true));
////		if(model->lastError().type() != QSqlError::NoError){
////			//QMessageBox::critical(0, "Query Error", model->lastError().text());
////			qCritical()<<"Query Error!"<<model->lastError().text();
////			return;
////		}

////		for (int i=0;i< model->rowCount();i++) {
////						int groupID = QVariant(model->record(i).value("ID")).toInt();
////						QString groupName = QVariant(model->record(i).value("GroupName")).toString();
////						QList<IMUserBase*> list;

////						QString contactsQueryString = QString("select * from contacts where ContactsGroup=%1") .arg(groupID);

////						contactsModel->setQuery(queryDatabase(contactsQueryString, true));
////						if(contactsModel->lastError().type() != QSqlError::NoError){
////							//QMessageBox::critical(0, "Query Error", contactsModel->lastError().text());
////							qCritical()<<"Query Error!"<<contactsModel->lastError().text();
////							return;
////						}

////						for (int i=0;i< contactsModel->rowCount();i++) {
////							QString contactUID = QVariant(contactsModel->record(i).value("UID")).toString();
////							int contactsGroupID = QVariant(contactsModel->record(i).value("ContactsGroup")).toInt();
////							QString contactDept = QVariant(contactsModel->record(i).value("Dept")).toString();
////							QString contactTrueName = QVariant(contactsModel->record(i).value("TrueName")).toString();
////							QString contactNickName = QVariant(contactsModel->record(i).value("NickName")).toString();
////							QString contactHeadPortrait = QVariant(contactsModel->record(i).value("HeadPortrait")).toString();
////							QString contactLastLoginIP = QVariant(contactsModel->record(i).value("LastLoginIP")).toString();

////							IMUserBase *contact = new IMUserBase();
////                                                        contact->setUserID(contactUID);
////							contact->setUserID(contactUID);
////							contact->setContactsGroupID(contactsGroupID);
////							contact->setDept(contactDept);
////							contact->setHeadPortrait(contactHeadPortrait);
////							contact->setTrueName(contactTrueName);
////							contact->setNickName(contactNickName);
////							contact->setLastLoginIP(contactLastLoginIP);

////                                                        imUsersHash.insert(contactUID, contact);
////							list.append(contact);
////							qApp->processEvents();
////						}

//////						UserGroup *group = new UserGroup(groupID, groupName, list, this);
//////						contactGroupHash.insert(groupID, group);



////					}


////		contactsModel->deleteLater();
////		model->deleteLater();


//}

////添加新的联系人
////Add new contact
//void UsersManager::slotAddNewIMUser(UserInfo *imUser){

//    //TODO:保存数据

////        QString contactUID = imUser->getUserID();
////        QString contactTrueName = imUser->getTrueName();
////        QString contactNickName = imUser->getNickName();
////        QString contactHeadPortrait = imUser->getFace();
////        QString contactLastLoginIP = imUser->getLastLoginHostAddress();

////        QString queryString = QString("Insert  Into [contacts] ([UID],[NickName],[TrueName],[LastLoginIP],[HeadPortrait]) Values('%1', '%2', '%3', '%4', '%5', '%6')")
////                                                                                                .arg(contactUID).arg(contactNickName).arg(contactTrueName).arg(contactLastLoginIP).arg(contactHeadPortrait) ;

////	QSqlQuery query = queryDatabase(queryString, true);
////	if(!query.exec()){
////		qDebug()<<"XXXXContactsManager::slotAddNewContact(...)~~Error:"<<query.lastError().text();
////	}


//}








/*

void UsersManager::slotUpdateContactsInfo2(){



    QSqlQuery query;
    QSqlRecord record;


    //Local
    QString queryString = QString("select ContactsListVersion from contacts where UID = '%1'") .arg(User::instance()->getUserID());

    int localContactsListVersion = -1;

    query = queryDatabase(queryString, true);
    if(query.lastError().type() == QSqlError::NoError && query.isValid()){
        query.first();
        localContactsListVersion = query.value(0).toInt();
        qDebug()<<"Local Contacts List Version:"<<localContactsListVersion;
    }else{
        //QMessageBox::critical(this, QObject::tr("Fatal Error"), tr("Can not query data from local database! <br> %1").arg(model.lastError().text()));
        qCritical()<<QString("Can not query data from local database!");
        qCritical()<<QString("Error: %1").arg(query.lastError().text());
        //statusBar()->showMessage(tr("Error! Can not query data from local database!"));
    }


    if(User::instance()->getRemoteServerContactsListVersion() != localContactsListVersion){
        qDebug()<<"Remote Server Contacts List Version:"<<User::instance()->getRemoteServerContactsListVersion();

        if(User::instance()->getRemoteServerContactsListVersion() > localContactsListVersion){
            queryString = QString("update contacts set ContactsList = '%1'  where UID = '%2'") .arg(User::instance()->getContactsList(), User::instance()->getUserID());
            query = queryDatabase(queryString, true);
            qDebug()<<"Update Local Contacts List Version";
        }else{
            queryString = QString("select ContactsList from contacts where UID = '%1'") .arg(User::instance()->getUserID());
            QString contactsList;
            query = queryDatabase(queryString, true);
            if(query.lastError().type() == QSqlError::NoError && query.isValid()){
                query.first();
                contactsList = query.value(0).toString();
            }else{
                //QMessageBox::critical(this, QObject::tr("Fatal Error"), tr("Can not query data from database server! <br> %1").arg(model.lastError().text()));
                qCritical()<<QString("Can not query data from database server!");
                qCritical()<<QString("Error: %1").arg(query.lastError().text());
                //statusBar()->showMessage(tr("Error! Can not query data from database server!"));
            }
            queryString = QString("update contacts set ContactsList = '%1' , ContactsListVersion = %2 where UID = '%3'") .arg(contactsList).arg(localContactsListVersion).arg(User::instance()->getUserID());
            query = queryDatabase(queryString, false);
            if(query.lastError().type() != QSqlError::NoError){qDebug()<<"Query Error:"<<query.lastError().text();}
            qDebug()<<"Update Remote Server Contacts List Version";
            User::instance()->setContactsList(contactsList);
            User::instance()->setRemoteServerContactsListVersion(localContactsListVersion);
            //qDebug()<<"contactsList~~~~"<<contactsList;
        }


    }


    qDebug()<<"-------------contactsList:"<<User::instance()->getContactsList();
    //queryString = QString("select UID, Role, Dept, TrueName, NickName, HeadPortrait, LastLoginIP from contacts where UID in (%1) ") .arg(User::instance()->getContactsList());
    queryString = QString("select UID, Role, Dept, TrueName, NickName, Icon, LastLoginIP from contacts where UID in (%1) ") .arg(User::instance()->getContactsList());
query.clear();
    query = queryDatabase(queryString, true);
    query.first();
    qDebug()<<"-------------queryString:"<<queryString;
    qDebug()<<"-------------query.size:"<<query.size();
    record = query.record();

    while (query.next()){
        QString contactUID = QVariant(query.value(record.indexOf("UID"))).toString();
        int contactsRoleID = QVariant(query.value(record.indexOf("Role"))).toInt();
        QString contactDept = QVariant(query.value(record.indexOf("Dept"))).toString();
        QString contactTrueName = QVariant(query.value(record.indexOf("TrueName"))).toString();
        QString contactNickName = QVariant(query.value(record.indexOf("NickName"))).toString();
        QString contactHeadPortrait = QVariant(query.value(record.indexOf("HeadPortrait"))).toString();
        QString contactLastLoginIP = QVariant(query.value(record.indexOf("LastLoginIP"))).toString();

        ContactBase *contact = new ContactBase();
        contact->setUID(contactUID);
        contact->setRoleID(contactsRoleID);
        contact->setDept(contactDept);
        contact->setHeadPortrait(contactHeadPortrait);
        contact->setTrueName(contactTrueName);
        contact->setNickName(contactNickName);
        contact->setLastLoginIP(contactLastLoginIP);

        contactHash.insert(contactUID, contact);

    }

    record.clearValues();
    queryString = QString("select * from group");
    query = queryDatabase(queryString, true);
    record = query.record();
    while (query.next()){
        int contactGroupID = QVariant(query.value(record.indexOf("ID"))).toInt();
        QString contactGroupName = QVariant(query.value(record.indexOf("Name"))).toString();
        int contactGroupParentID = QVariant(query.value(record.indexOf("ParentCategory"))).toInt();
        QString contactGroupRemark = QVariant(query.value(record.indexOf("Remark"))).toString();

        UserGroup *group = new UserGroup();
        group->setGroupID(contactGroupID);
        group->setGroupName(contactGroupName);
        group->setParentGroupID(contactGroupParentID);

        contactGroupHash.insert(contactGroupID, group);

    }



    qDebug()<<"UsersManager::slotUpdateContactsInfo()~~~~";

}

*/


/*
 void UsersManager::slotLoadContacts(ExpandListView *widgetBox, bool loadFromFile, const QString &string){

    QString file; // = QApplication::applicationDirPath()+QDir::separator()+"contacts.xml";

        if(string.isEmpty() || !QFile(string).exists()){
            file = ":/text/resources/text/default_contacts.xml";
        }


        if(loadFromFile){
            widgetBox->setFileName(file);
            widgetBox->load();
        }else{
            widgetBox->loadContents(string);
        }


        qDebug()<<"~~~~~~~~~~~~~ Load Contacts ~~~~~~~~~~~~~~";


}
*/

UserInfo * UsersManager::queryUserInfo(const QString &imUserID){
    //    qWarning()<<"queryUserInfo(const QString &imUserID)";

    //TODO:查询错误处理
    UserInfo *info = new UserInfo(imUserID/*, this*/);
    if(!queryUserInfo(info)){
        delete info;
        info = 0;
    }else{
        QMutexLocker locker(offlineUserMutex);
        offlineUserInfoHash->insert(imUserID, info);
    }
    return info;

    //    QSqlQuery query;
    //    QSqlRecord record;
    //    QString queryString = QString("select * from users where %1='%2' ").arg(UserInfo::databaseColumnName(IM::PI_UserID).arg(imUserID);

    //    query = queryDatabase(queryString, false);
    //    if((query.lastError().type() != QSqlError::NoError) || (!query.isValid())){
    //        qCritical()<<QString("Can not query data from local database! Error: %1").arg(query.lastError().text());
    //        return 0;
    //    }

    //    record = query.record();
    //    query.first();

    //    UserInfo *info = new UserInfo(imUserID, this);
    //    info->setPassword(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_Password)))).toString());
    //    info->setTrueName(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_TrueName)))).toString());
    //    info->setNickName(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_NickName)))).toString());
    //    info->setGender(IMUserBase::Gender(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_Gender)))).toUInt()));

    //    info->setHomeAddress(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_HomeAddress)))).toString());
    //    info->setHomePhoneNumber(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_HomePhoneNumber)))).toString());
    //    info->setHomeZipCode(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_HomeZipCode)))).toString());
    //    info->setPersonalHomepage(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_PersonalHomepage)))).toString());
    //    info->setPersonalEmailAddress(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_PersonalEmailAddress)))).toString());

    //    info->setLastLoginTime(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_LastLoginTime)))).toDateTime());
    //    info->setLastLoginHostAddress(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_LastLoginHostAddress)))).toString());
    //    info->setLastLoginHostPort(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_LastLoginHostAddress)))).toUInt());

    //    info->setQuestionForSecurity(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_QuestionForSecurity)))).toString());
    //    info->setAnswerForSecurity(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_AnswerForSecurity)))).toString());
    //    info->setFace(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_Face)))).toString());
    //    info->setCompanyName(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_CompanyName)))).toString());
    //    info->setJobTitle(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_JobTitle)))).toString());

    //    info->setBusinessAddress(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_BusinessAddress)))).toString());
    //    info->setBusinessPhoneNumber(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_BusinessPhoneNumber)))).toString());
    //    info->setBusinessZipCode(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_BusinessZipCode)))).toString());
    //    info->setBusinessFaxNumber(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_BusinessFaxNumber)))).toString());
    //    info->setBusinessHomepage(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_BusinessHomepage)))).toString());
    //    info->setBusinessEmailAddress(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_BusinessEmailAddress)))).toString());

    //    info->setRegistrationTime(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_RegistrationTime)))).toDateTime());
    //    info->setLoginTimes(QVariant(query.value(record.indexOf(ServerUtilities::databaseColumnName(IM::PI_LoginTimes)))).toInt());


    //    return info;



}

bool UsersManager::queryUserInfo(UserInfo *info){
    //qWarning()<<"queryUserInfo(UserInfo *info)";

    if(!info){
        return false;
    }
    
    //    if(!query){
    //        if(!openDatabase()){
    //            return false;
    //        }
    //    }else{
    //        query.clear();
    //    }

    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);
    
    QSqlRecord record;
    //    QString statement = QString("SELECT * FROM UsersSummaryInfo usi left join UsersDetailedInfo udi on usi.%1=udi.%1 where %2='%3' ").arg(info->databaseColumnName(IM::PI_SysID)).arg(info->databaseColumnName(IM::PI_UserID)).arg(info->getUserID());
    QString statement = QString("call sp_GetUserInfo('%1') ").arg(info->getUserID());

    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not query user info from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        //        logMessage(msg, QtServiceBase::Error);
        qCritical()<<msg;

        //TODO:数据库重启，重新连接
        //MySQL数据库重启，重新连接
        if(error.number() == 2006){
            query.clear();
            openDatabase(true);
        }

        return false;
    }
    
    
    //    query = queryDatabase(statement, false);
    
    if((query.lastError().type() != QSqlError::NoError)){
        qCritical()<<QString("Can not query data from local database! Error: %1").arg(query.lastError().text());
        return false;
    }
    
    
    //    if(!query.isValid()){
    //        qCritical()<<QString("Can not query data from local database! Error: %1").arg(query.lastError().text());
    //        return false;
    //    }
    
    if(!query.first()){
        qCritical()<<QString("Can not query user info! Invalid record! User ID:%1").arg(info->getUserID());
        return false;
    }

    record = query.record();
    

    //UserInfo *info = new UserInfo(imUserID, this);
    info->setSysID(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_SysID)))).toUInt());
    info->setPassword(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_Password)))).toString());
    info->setTrueName(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_TrueName)))).toString());
    info->setNickName(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_NickName)))).toString());
    info->setGender(IMUserBase::Gender(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_Gender)))).toUInt()));
    info->setAge(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_Age)))).toInt());
    info->setFace(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_Face)))).toString());

    //    info->setContactGroupsInfoString(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_PersonalContactGroupsInfoString)))).toString());
    info->setPersonalContactGroupsVersion(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_PersonalContactGroupsInfoVersion)))).toInt());
    //    info->setInterestGroupsStringFromDatabase(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_InterestGroupsInfoString)))).toString());
    info->setInterestGroupInfoVersion(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_InterestGroupsInfoVersion)))).toUInt());
    //    info->setBlacklistInfoString(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_Blacklist)))).toString());
    info->setPersonalSummaryInfoVersion(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_PersonalSummaryInfoVersion)))).toInt());
    info->setPersonalDetailInfoVersion(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_PersonalDetailInfoVersion)))).toInt());
    info->setPersonalMessageInfoVersion(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_PersonalMessageInfoVersion)))).toInt());
    info->setFriendshipApply(IMUserBase::FriendshipApply(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_FriendshipApply)))).toUInt()));
    info->setShortTalk(IMUserBase::ShortTalk(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_ShortTalk)))).toUInt()));
    info->setUserRole(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_Role)))).toInt());
    info->setDescription(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_Description)))).toString());
    info->setAccountState(IMUserBase::AccountState(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_AccountState)))).toUInt()));
    info->setPersonalMessage(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_PersonalMessage)))).toString());

    info->setHomeAddress(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_HomeAddress)))).toString());
    info->setHomePhoneNumber(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_HomePhoneNumber)))).toString());
    info->setHomeZipCode(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_HomeZipCode)))).toString());
    info->setPersonalHomepage(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_PersonalHomepage)))).toString());
    info->setPersonalEmailAddress(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_PersonalEmailAddress)))).toString());

    //    info->setLastLoginTime(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_LastLoginTime)))).toDateTime());
    //    info->setLastLoginHostAddress(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_LastLoginHostAddress)))).toString());
    //    info->setLastLoginHostPort(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_LastLoginHostAddress)))).toUInt());

    info->setQuestionForSecurity(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_QuestionForSecurity)))).toString());
    info->setAnswerForSecurity(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_AnswerForSecurity)))).toString());
    info->setEmailForSecurity(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_EmailForSecurity)))).toString());
    
    info->setCompanyName(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_CompanyName)))).toString());
    info->setJobTitle(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_JobTitle)))).toString());
    info->setBusinessAddress(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_BusinessAddress)))).toString());
    info->setBusinessPhoneNumber(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_BusinessPhoneNumber)))).toString());
    info->setBusinessZipCode(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_BusinessZipCode)))).toString());
    info->setBusinessFaxNumber(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_BusinessFaxNumber)))).toString());
    info->setBusinessHomepage(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_BusinessHomepage)))).toString());
    info->setBusinessEmailAddress(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_BusinessEmailAddress)))).toString());

    info->setRegistrationTime(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_RegistrationTime)))).toDateTime());
    //    info->setLoginTimes(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PI_LoginTimes)))).toInt());


    //    getUserInterestGroupsFromDatabase(info);
    //    getUserLastLoginInfo(info);

    info->clearUpdatedProperties();


    
    return true;
    

}

bool UsersManager::getUserAllContactGroupsInfoFromDatabase(UserInfo* info){


    if(!info){
        return false;
    }

    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);
    QString statement = QString("call sp_GetAllContactGroupsInfoForUserAsString('%1'); ").arg(info->getUserID());
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not query user contact groups info from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;

        return false;
    }

    QStringList contactGroups;
    while(query.next()){
        quint32 groupID  = query.value(0).toUInt();
        QString groupName = query.value(1).toString();

        //CONTACTS FORMATE: UserID,UserID,...
        //e.g. user1,user2,user3
        QString contacts = query.value(2).toString();

        contactGroups.append(QString::number(groupID)+","+groupName+","+contacts);
    }


    //STRING FORMATE: GroupID,GroupName,UserID,,UserID,...||GroupID,...
    //e.g. 100,Group100,user1,user2,user3||101,Group101,user4
    QString infoString = contactGroups.join(GROUP_INFO_SEPARATOR);
    info->setContactGroupsInfoString(infoString);

    return true;

}



bool UsersManager::getUserInterestGroupsFromDB(UserInfo* info){

    if(!info){
        return false;
    }

    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);
    //QString statement = QString("select %1 from InterestGroupMembers where %2='%3' ").arg(InterestGroup::databaseColumnName(IM::PIG_GroupID).arg(InterestGroup::databaseColumnName(IM::PIG_MemberSysID)).arg(info->getUserID());
    QString statement = QString("call sp_GetUserInterestGroups('%1') ").arg(info->getUserID());

    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not query user interest groups info from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;

        return false;
    }
    if(!query.first()){
        return false;
    }

    info->setInterestGroups(query.value(0).toString().split(","));


    //    QStringList groups;
    //     while(query.next()){
    //        groups.append(query.value(0).toString());
    //     }
    //     info->setInterestGroups(groups);

    return true;

}


bool UsersManager::getUserAllContactsInfoFromDatabase(UserInfo* info, QString *infoString){

    Q_ASSERT(infoString);

    //FORMATE: GroupID,GroupName;UserID,PersonalSummaryInfoVersion,PersonalDetailInfoVersion,PersonalMessageInfoVersion;UserID,...||GroupID,GroupName;UserID,...
    //e.g. 100,Group-100;user1,10,10,2;user2,5,6,15||101,Group-101;user3,11,10,20

    if(!info){
        return false;
    }
    if(!infoString){
        return false;
    }

    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);
    QString statement = QString("call sp_GetAllContactsInfoForUser('%1'); ").arg(info->getUserID());

    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not query user contacts info from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;

        return false;
    }

    QStringList contactGroups;
    while(query.next()){
        quint32 groupID  = query.value(0).toUInt();
        QString groupName = query.value(1).toString();

        //FORMATE: UserID,PersonalSummaryInfoVersion,PersonalDetailInfoVersion,PersonalMessageInfoVersion;UserID,...
        //e.g. user1,10,10,2;user2,5,6,15;user3,11,10,20
        QString contactsInfo = query.value(2).toString();

        if(contactsInfo.trimmed().isEmpty()){
            contactGroups.append(QString::number(groupID)+","+groupName);
        }else{
            contactGroups.append(QString::number(groupID)+","+groupName+";"+contactsInfo);
        }
    }
    //FORMATE: GroupID,GroupName;UserID,PersonalSummaryInfoVersion,PersonalDetailInfoVersion,PersonalMessageInfoVersion;UserID,...||GroupID,GroupName;UserID,...
    //e.g. 100,Group-100;user1,10,10,2;user2,5,6,15||101,Group-101;user3,11,10,20
    *infoString = contactGroups.join(GROUP_INFO_SEPARATOR);

    return true;

}

bool UsersManager::getUserAllContactsInfoVersionFromDatabase(UserInfo* info, QString *infoString){

    //FORMATE: UserID,PersonalSummaryInfoVersion,PersonalDetailInfoVersion,PersonalMessageInfoVersion;UserID,...
    //e.g. user1,10,10,2;user2,5,6,15;user3,11,10,20

    Q_ASSERT(infoString);

    if(!info){
        return false;
    }
    if(!infoString){
        return false;
    }

    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);
    QString statement = QString("call sp_GetAllContactsInfoForUserAsString('%1'); ").arg(info->getUserID());

    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not query user contacts info from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;

        return false;
    }

    query.first();
    *infoString = query.value(0).toString();

    return true;

}

bool UsersManager::createOrDeleteContactGroupInDB(UserInfo* info, quint32 *groupID, const QString &groupName, bool createGroup){
    qDebug()<<"--UsersManager::createOrDeleteContactGroupInDB(...)  UserID:"<<info->getUserID()<<" groupName:"<<groupName<<" createGroup:"<<createGroup;

    Q_ASSERT(groupID);

    if(!info){
        return false;
    }

    quint32 gID = *groupID;

    if(createGroup){
        int newGroupID = info->getUnusedContactGroupID();
        if(newGroupID < 0){
            qCritical()<<"ERROR! Invalid unused contact group ID!"<<" Name:"<<groupName;
            return false;
        }else{
            gID = newGroupID;
        }
    }else if(!info->hasContactGroup(gID)){
        qCritical()<<"ERROR! Contact group does not exist!"<<" ID:"<<groupID;
        return false;
    }



    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);
    QString statement = QString("call sp_ContactGroup_CreateOrDelete('%1', %2, '%3', %4); ").arg(info->getUserID()).arg(gID).arg(groupName).arg(createGroup?1:0);

    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not %1 contact group '%2' for user '%3'! %4 Error Type:%5 Error NO.:%6").arg(createGroup?"create":"delete").arg(groupName).arg(info->getUserID()).arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;

        return false;
    }

    //    statement = QString(" select @GroupInfoVersion; ");
    //    if(!query.exec(statement)){
    //        QSqlError error = query.lastError();
    //        QString msg = QString("Can not query contact groups info version for user '%1'! %2 Error Type:%3 Error NO.:%4").arg(info->getUserID()).arg(error.text()).arg(error.type()).arg(error.number());
    //        qCritical()<<msg;

    //        return false;
    //    }
    //    if(query.first()){
    //        info->setPersonalContactGroupsVersion(query.value(0).toUInt());
    //        info->clearUpdatedProperties();
    //    }

    if(createGroup){
        info->addContactGroup(gID, groupName);
        *groupID = gID;
    }else{
        info->deleteContactGroup(gID);
    }
    //    qDebug()<<"Contact Groups:"<<info->getContactGroupsInfoString();


    return true;

}

bool UsersManager::updateContactGroupNameInDB(UserInfo* info, quint32 groupID, const QString &newGroupName){

    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);
    QString statement = QString("call sp_ContactGroup_UpdateName('%1', %2, '%3'); ").arg(info->getUserID()).arg(groupID).arg(newGroupName);
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not update user contact group name ! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;

        return false;
    }

    info->renameContactGroup(groupID, newGroupName);

    return true;

}


bool UsersManager::saveUserInfoToDatabase(UserInfo *info){
    //TODO:
    if(!info){
        return false;
    }
    
    QString summaryInfoUpdateSQLStatement = info->getUpdateSQLStatement(true);
    QString detailInfoStatement = info->getUpdateSQLStatement(false);
    QString statement = "" ;
    if(!summaryInfoUpdateSQLStatement.trimmed().isEmpty()){
        statement = QString("update UsersSummaryInfo set %1 where %2='%3'; ").arg(summaryInfoUpdateSQLStatement).arg(info->databaseColumnName(IM::PI_SysID)).arg(info->getSysID());
    }
    if(!detailInfoStatement.trimmed().isEmpty()){
        statement += QString("update UsersDetailedInfo set %1 where %2='%3'; ").arg(summaryInfoUpdateSQLStatement).arg(info->databaseColumnName(IM::PI_SysID)).arg(info->getSysID());
    }

    if(statement.isEmpty()){
        return false;
    }

    
    //    if(!query){
    //        if(!openDatabase()){
    //            return false;
    //        }
    //    }else{
    //        query.clear();
    //    }

    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);
    //    QString statement = QString("update users_detailed_info set %1 where %2='%3' ").arg(updateSQLStatement).arg(info->databaseColumnName(IM::PI_UserID)).arg(info->getUserID());
    
    //    query = queryDatabase(statement, false);
    //    if((query.lastError().type() != QSqlError::NoError)){
    //        qCritical()<<QString("Can not query data from local database! Error: %1").arg(query.lastError().text());
    //        return false;
    //    }
    
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not write user info to database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        //        logMessage(msg, QtServiceBase::Error);
        qCritical()<<msg;

        //TODO:数据库重启，重新连接
        //MySQL数据库重启，重新连接
        if(error.number() == 2006){
            query.clear();
            openDatabase(true);
        }

        return false;
    }else{
        info->clearUpdatedProperties();
    }
    
    
    return true;
    
    
}

bool UsersManager::saveFriendshipApplyRequestToDB(const QString &applicantID, const QString &contactID, const QString &message, quint8 resultCode, bool senderRead, bool receiverRead){


    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);
    //QString statement = QString("insert into friendshipapply(SenderID, ReceiverID, ExtraMessage, Result, SenderRead, ReceiverRead) values('%1',  '%2', '%3', '%4', '%5', '%6' ) ")
    QString statement = QString("call sp_FriendshipApplyRequest_Add('%1',  '%2', '%3', %4, %5, %6 ) ")

            .arg(applicantID).arg(contactID).arg(message).arg(resultCode)
            .arg(senderRead?1:0).arg(receiverRead?1:0)
            ;

    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("ERROR! Can not write friendship application info to database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }


    return true;

}

bool UsersManager::deleteFriendshipApplyRequest(const QString &applicantID, const QString &contactID){


    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);
    //QString statement = QString("delete from friendshipapply where Sender='%1' and Receiver='%2' ").arg(applicantID).arg(contactID);
    QString statement = QString("call sp_FriendshipApplyRequest_Delete('%1', '%2'); ").arg(applicantID).arg(contactID);

    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not delete friendship application info from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return false;
    }

    return true;
}

bool UsersManager::getFriendshipApplyRequest(const QString &userID, QList<QStringList> *sentApplicationList, QList<QStringList> *receivedApplicationList){
    qDebug()<<"--UsersManager::getFriendshipApplyRequest(...)";

    if(!sentApplicationList || !receivedApplicationList){
        return false;
    }

    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);

    //QString statement = QString("select ReceiverID, Result, ExtraMessage from friendshipapply where SenderID='%1' and SenderRead = '0' ").arg(userID);
    QString statement = QString("call sp_FriendshipApplyRequest_SentByUser_Get('%1'); ").arg(userID);
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not get friendship application info from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;

        return false;
    }
    while(query.next()){

        UserInfo::FriendshipApplyResult faResult = UserInfo::FriendshipApplyResult(query.value(1).toUInt());
        if(faResult == UserInfo::FAR_UNKNOWN){continue;}

        QStringList list;
        list << query.value(0).toString()
             <<QString::number(query.value(1).toUInt())
            <<query.value(2).toString()
              ;
        sentApplicationList->append(list);

    }

    //statement = QString("select SenderID, Result, ExtraMessage from friendshipapply where ReceiverID='%1' and ReceiverRead = '0' ").arg(userID);
    statement = QString("call sp_FriendshipApplyRequest_SentToUser_Get('%1'); ").arg(userID);
    query.clear();
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not get friendship application info from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;

        return false;
    }
    while(query.next()){
        QStringList list;
        list << query.value(0).toString()
             <<QString::number(query.value(1).toUInt())
            <<query.value(2).toString()
              ;
        receivedApplicationList->append(list);

    }

    //statement = QString("delete from friendshipapply where SenderID='%1' or ReceiverID='%1' ").arg(userID);
    //    statement = QString("update friendshipapply set SenderRead = '1' where SenderID='%1' and SenderRead = '0'  ").arg(userID);
    //    query.exec(statement);
    //    statement = QString("update friendshipapply set ReceiverRead = '1' where ReceiverID='%1' and ReceiverRead = '0' ").arg(userID);

    statement = QString("call sp_FriendshipApplyRequest_ForUser_Update('%1'); ").arg(userID);
    query.exec(statement);


    return true;

}


QString UsersManager::getInterestGroupsListForUser(UserInfo* userInfo){
    qDebug()<<"--UsersManager::getInterestGroupsListForUser(...)";


    //Interest Groups List Format: GroupID,GroupInfoVersion,MemberListInfoVersion;GroupID,GroupInfoVersion,MemberListInfoVersion


    if(!userInfo){
        qDebug()<<"invalid userInfo";
        return "";
    }

    QStringList infoList;
    QList<quint32> groups = userInfo->getInterestGroups();
    foreach (quint32 groupID, groups) {
        InterestGroup *group = getInterestGroup(groupID);
        if(!group){
            qDebug()<<"invalid group";
            return "";
        }
        infoList.append(QString::number(groupID) + "," + QString::number(group->getGroupInfoVersion()) + "," + QString::number(group->getGroupMemberListInfoVersion()));
    }
    
    return infoList.join(";");

}

//Format: ?
QString UsersManager::getInterestGroupInfoStringForUser(UserInfo* userInfo, quint32 groupID){

    QString infoString = "";
    
    if(!userInfo){
        return infoString;
    }
    
    InterestGroup *group = getInterestGroup(groupID);
    if(!group){
        return infoString;
    }
    
    return group->getGroupInfoString();
    

}

//Format: ?
QString UsersManager::getInterestGroupMembersInfoStringForUser(UserInfo* userInfo, InterestGroup *group){

    QString infoString = "";
    
    if(!userInfo){return infoString;}
    if(!group){return infoString;}
    
    QStringList infoList;
    QHash <QString/*Member's ID*/, InterestGroup::MemberRole/*Member's Role*/> membersHash = group->getMembersHash();
    QStringList members = membersHash.keys();
    
    foreach (QString memberUserID, members) {
        UserInfo *memberInfo = getUserInfo(memberUserID);
        if(!memberInfo){continue;}
        QStringList memberInfoList;
        memberInfoList << memberInfo->getUserID() << QString::number(memberInfo->getPersonalSummaryInfoVersion()) << QString::number(membersHash.value(memberUserID));
        infoList.append(memberInfoList.join(QString(CONTACT_INFO_SEPARATOR)));
    }
    
    return infoList.join(QString(PACKET_DATA_SEPARTOR));

}


bool UsersManager::openDatabase(bool reopen){

    if(reopen){
        //        if(query){
        //            query.clear();
        //            delete query;
        //            query = 0;
        //        }
        DatabaseUtility::closeDBConnection(Server_DB_CONNECTION_NAME);
    }


    //QSqlDatabase db = QSqlDatabase::database(Server_DB_CONNECTION_NAME);
    db = QSqlDatabase::database(Server_DB_CONNECTION_NAME);
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
        HEHUI::Settings settings(APP_NAME, APP_VERSION, SETTINGS_FILE_NAME, "./");
        DatabaseUtility databaseUtility;
        err = databaseUtility.openDatabase(Server_DB_CONNECTION_NAME,
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

    //    if(!query){
    //        query = new QSqlQuery(db);
    //    }


    return true;



}



QSqlQuery UsersManager::queryDatabase(const QString & queryString, bool localConfigDatabase) {

    QSqlQuery query;
    DatabaseUtility du;

    Settings settings;
    QString userPrivateDataFilePath = settings.getDataRootPath();

    if(localConfigDatabase){
        query = du.queryDatabase(queryString,
                                 LOCAL_SERVER_DB_CONNECTION_NAME,
                                 LOCAL_SERVER_DB_DRIVER,
                                 "",
                                 0,
                                 "",
                                 "",
                                 userPrivateDataFilePath,
                                 HEHUI::SQLITE);
    }else{
        query = du.queryDatabase(queryString,
                                 REMOTE_SITOY_COMPUTERS_DB_CONNECTION_NAME,
                                 REMOTE_SITOY_COMPUTERS_DB_DRIVER,
                                 REMOTE_SITOY_COMPUTERS_DB_SERVER_HOST,
                                 REMOTE_SITOY_COMPUTERS_DB_SERVER_PORT,
                                 REMOTE_SITOY_COMPUTERS_DB_USER_NAME,
                                 REMOTE_SITOY_COMPUTERS_DB_USER_PASSWORD,
                                 REMOTE_SITOY_COMPUTERS_DB_NAME,
                                 HEHUI::MYSQL);
    }

    return query;


}


QSqlQuery UsersManager::queryDatabase(const QString & queryString, const QString &connectionName, const QString &driver,
                                      const QString &host, int port, const QString &user, const QString &passwd,
                                      const QString &databaseName, HEHUI::DatabaseType databaseType) {


    QSqlQuery query;
    DatabaseUtility du;

    query = du.queryDatabase(queryString, connectionName, driver, host, port, user, passwd, databaseName, databaseType);

    return query;

}









/////////////////////////// Group Manager ////////////////////////////////////////    

InterestGroup* UsersManager::getInterestGroup(quint32 groupID){
    
    if(groupID < 1){
        return 0;
    }
    
    InterestGroup * group = 0;
    if(m_interestGroupHash->contains(groupID)){
        group = m_interestGroupHash->value(groupID);
    }else{
        group = queryInterestGroup(groupID);
    }
    

    return group;

}

quint32 UsersManager::createNewInterestGroup(UserInfo *creatorInfo, const QString &groupName){
    qDebug()<<"UsersManager::createNewInterestGroup(...) "<<" creatorID:"<<creatorInfo->getUserID()<<" groupName:"<<groupName;

    if(!creatorInfo){return 0;}
    if(groupName.trimmed().isEmpty()){return 0;}

    if(!db.isValid()){
        if(!openDatabase()){
            //*errorType = IM::ERROR_UnKnownError;
            return 0;
        }
    }

    QSqlQuery query(db);
    QString statement = QString("call sp_InterestGroup_Create('%1', '%2', @GroupID); ").arg(creatorInfo->getUserID()).arg(groupName);
    
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not create new group! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        
        return 0;
    }
    
    
    statement = QString("select @GroupID; ");
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not query group id from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        
        return 0;
    }
    
    if(!query.first()){
        return 0;
    }
    
    quint32 groupID = query.value(0).toUInt();
    qDebug()<<"-----------groupID:"<<groupID;

    InterestGroup *group = queryInterestGroup(groupID);
    if(!group){
        return 0;
    }

    creatorInfo->joinOrLeaveInterestGroup(groupID, true);
    
    return groupID;
    
}

bool UsersManager::disbandInterestGroup(UserInfo *creatorInfo, quint32 groupID){
    qDebug()<<"UsersManager::disbandInterestGroup(...) "<<" creatorID:"<<creatorInfo->getUserID()<<" groupID:"<<groupID;

    if(!creatorInfo){return false;}
    InterestGroup *group = getInterestGroup(groupID);
    if(group->memberRole(creatorInfo->getUserID()) != InterestGroup::Role_Creator){
        return false;
    }

    if(!db.isValid()){
        if(!openDatabase()){
            //*errorType = IM::ERROR_UnKnownError;
            return false;
        }
    }

    QSqlQuery query(db);
    QString statement = QString("call sp_InterestGroup_Disband(%1); ").arg(groupID);

    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not disband group! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;

        return false;
    }




    return true;

}

bool UsersManager::memberJoinOrQuitInterestGroup(UserInfo *userInfo, InterestGroup *group, bool join){
    qDebug()<<"UsersManager::memberJoinOrQuitInterestGroup(...) "<<" memberID:"<<userInfo->getUserID()<<" groupID:"<<group->getGroupID();

    if(!userInfo){return false;}
    if(!group){return false;}

    if(!db.isValid()){
        if(!openDatabase()){
            //*errorType = IM::ERROR_UnKnownError;
            return false;
        }
    }
    QSqlQuery query(db);

    quint32 groupID = group->getGroupID();
    QString memberID = userInfo->getUserID();
    QString statement = QString("call sp_InterestGroup_MemberJoinOrQuit('%1', %2, %3, @MemberListVersion); ").arg(memberID).arg(groupID).arg(join?1:0);
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Member '%1' failed to %2 group ! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;

        return false;
    }

    statement = QString("select @MemberListVersion; ");
    query.exec(statement);
    if(!query.first()){
        QString msg = QString("Can not query group member list version! %1 ").arg(query.lastError().text());
        qCritical()<<msg;
        return false;
    }

    if(join){
        group->addMember(memberID, InterestGroupBase::Role_Member);
    }else{
        group->deleteMember(memberID);
    }
    group->setGroupMemberListInfoVersion(query.value(0).toUInt());

    userInfo->joinOrLeaveInterestGroup(groupID, join);


    return true;

}


QList<UserInfo *> UsersManager::getAllOnlineInterestGroupMembers(quint32 groupID){

    QList<UserInfo *> onlineMembers;

    InterestGroup * group = getInterestGroup(groupID);
    if(!group){return onlineMembers;}

    foreach (QString memberID, group->members()) {
        UserInfo *user = getOnlineUserInfo(memberID);
        if(user){onlineMembers.append(user);}
    }

    return onlineMembers;

}

QString UsersManager::searchInterestGroup(const QString &keyword, int startIndex){

    if(keyword.trimmed().isEmpty()){
        return QString();
    }

    if(!db.isValid()){
        if(!openDatabase()){
            return QString();
        }
    }
    QSqlQuery query(db);
    
    quint32 groupID = keyword.toUInt();
    quint32 pageSize = SEARCH_RESULT_PAGE_SIZE;
    QString queryString;
    if(groupID){
        queryString = QString("call sp_InterestGroup_Search(%1, '%2', %3, %4) ;").arg(groupID).arg("").arg(startIndex).arg(pageSize);

    }else{
        queryString = QString("call sp_InterestGroup_Search(%1, '%2', %3, %4) ;").arg("null").arg("%"+keyword+"%").arg(startIndex).arg(pageSize);
    }
    if(!query.exec(queryString)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not query interest group info from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;

        return QString();
    }
    
    QStringList groupsInfoList;
    while(query.next()){
        
        QStringList infoList;
        for(int i=0; i<5; i++){
            infoList.append(query.value(i).toString());
            //            qWarning()<<i<<":"<<query.value(i).toString();
        }
        
        groupsInfoList.append(infoList.join(QChar(SEPARTOR_RECORD)));
    }
    
    return groupsInfoList.join(QChar(SEPARTOR_GROUP));
    
}

bool UsersManager::saveInterestGroupToDatabase(InterestGroup *groupInfo){
    //TODO:
    if(!groupInfo){
        return false;
    }
    
    QString updateSQLStatement = groupInfo->getUpdateSQLStatement();
    if(updateSQLStatement.trimmed().isEmpty()){
        return false;
    }
    

    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);
    QString statement = QString("update groups set %1 where %2='%3' ").arg(updateSQLStatement).arg(groupInfo->databaseColumnName(IM::PIG_GroupID)).arg(groupInfo->getGroupID());

    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not write group info to database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        //        logMessage(msg, QtServiceBase::Error);
        qCritical()<<msg;

        //TODO:数据库重启，重新连接
        //MySQL数据库重启，重新连接
        if(error.number() == 2006){
            query.clear();
            openDatabase(true);
        }

        return false;
    }else{
        groupInfo->clearUpdatedProperties();
    }
    
    
    return true;
    
    
}

bool UsersManager::saveOrDeleteMembershipApplication(const QString &applicantID, quint32 groupID, const QString &message, bool save){


    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);
    QString statement = QString("call sp_InterestGroup_SaveOrDeleteMembershipApplication('%1',  %2, '%3' ); ")
            .arg(applicantID).arg(groupID).arg(message).arg(save?1:0);

    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not write membership application info to database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;

        return false;
    }

    return true;

}

bool UsersManager::getMembershipApplyRequest(quint32 groupID, QList<QStringList> *receivedApplicationList){

    if(!receivedApplicationList){
        return false;
    }

    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);

    QString statement = QString("select ApplicantSystemID, ExtraMessage from membershipapply where GroupID='%1' ").arg(groupID);
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not get membership application info from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;

        //TODO:数据库重启，重新连接
        //MySQL数据库重启，重新连接
        if(error.number() == 2006){
            query.clear();
            openDatabase(true);
        }

        return false;
    }
    while(query.next()){
        QStringList list;
        list <<QString::number(query.value(0).toUInt())
            <<query.value(1).toString()
              ;
        receivedApplicationList->append(list);

    }

    statement = QString("delete from membership where GroupID='%1' ").arg(groupID);
    query.exec(statement);


    return true;

}

bool UsersManager::saveCachedInterestGroupChatMessageFromIMUser(const QString &senderID, quint32 interestGroupID, const QString &message){

    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);

    QString msg = message;
    msg.replace("'", "\\'");
    msg.replace("\"", "\\\"");
    //QString statement = QString("insert into CachedInterestGroupChatMessages(SenderID, InterestGroupID, Message) values('%1', '%2', '%3' )  ").arg(senderID).arg(interestGroupID).arg(msg);
    QString statement = QString("call sp_CachedChatMessages_FromInterestGroup_Save('%1', %2, '%3');  ").arg(senderID).arg(interestGroupID).arg(msg);
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not save group chat message to database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        //        logMessage(msg, QtServiceBase::Error);
        qCritical()<<msg;

        //TODO:数据库重启，重新连接
        //MySQL数据库重启，重新连接
        if(error.number() == 2006){
            query.clear();
            openDatabase(true);
        }

        return false;
    }


    //    if((query.lastError().type() != QSqlError::NoError)){
    //        qCritical()<<QString("Can not save data to database! Error: %1").arg(query.lastError().text());
    //        return false;
    //    }

    return true;

}

QStringList UsersManager::getCachedInterestGroupChatMessagesForUserFromDB(UserInfo* userInfo){
    //TODO:

    if(!userInfo){
        return QStringList();
    }

    if(userInfo->getInterestGroups().isEmpty()){
        return QStringList();
    }

    if(!db.isValid()){
        if(!openDatabase()){
            return QStringList();
        }
    }
    QSqlQuery query(db);

    //    QString imUserID = userInfo->getUserID();

    //QString statement = QString("select InterestGroupID, SenderID,  Message, TransmittingTime from cachedinterestgroupchatmessages where InterestGroupID in (%1) and TransmittingTime>'%2'  ").arg(interestgroups.join(",")).arg(userInfo->getLastLoginTime().toString("yyyy-MM-dd hh:mm:ss"));
    QString statement = QString("call sp_CachedChatMessages_FromInterestGroup_Get('%1', '%2');  ").arg(userInfo->getUserID()).arg(userInfo->getLastLogoutTime().toString("yyyy-MM-dd hh:mm:ss"));

    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not query cached InterestGroup chat messages from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;
        return QStringList();
    }

    if((query.lastError().type() != QSqlError::NoError)){
        qCritical()<<QString("Can not query cached InterestGroup chat messages from database! Error: %1").arg(query.lastError().text());
        return QStringList();
    }

    QStringList messages;
    while(query.next()){
        QStringList message;
        message.append(QVariant(query.value(0)).toString());
        message.append(QVariant(query.value(1)).toString());
        message.append(QVariant(query.value(2)).toString());
        message.append(QVariant(query.value(3)).toDateTime().toString("yyyy.MM.dd hh:mm:ss"));

        messages.append(message.join(QString(PACKET_DATA_SEPARTOR)));

    }

    //    qWarning()<<"messages cached on server:"<<messages.size();

    return messages;

}





InterestGroup * UsersManager::queryInterestGroup(quint32 groupID){
    
    InterestGroup *group = new InterestGroup(groupID);
    if(!queryInterestGroup(group)){
        delete group;
        group = 0;
    }
    
    return group;
    
}

bool UsersManager::queryInterestGroup(InterestGroup *info){

    if(!info){
        return false;
    }

    if(!db.isValid()){
        if(!openDatabase()){
            return false;
        }
    }
    QSqlQuery query(db);
    
    quint32 groupID = info->getGroupID();
    //QString statement = QString("select * from groups where %1='%2' ").arg(info->databaseColumnName(IM::PIG_GroupID)).arg(info->getGroupID());
    QString statement = QString("call sp_GetInterestGroupInfo(%1); ").arg(groupID);
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not query group info from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;

        return false;
    }
    

    if((query.lastError().type() != QSqlError::NoError)){
        qCritical()<<QString("Can not query data from database! Error: %1").arg(query.lastError().text());
        return false;
    }

    if(!query.first()){
        qCritical()<<QString("Can not query group info! Invalid record! Group ID:%1").arg(groupID);
        return false;
    }

    QSqlRecord record = query.record();

    info->setGroupTypeID(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PIG_GroupTypeID)))).toUInt());
    info->setParentGroupID(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PIG_ParentGroupID)))).toUInt());
    info->setCreatorID(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PIG_CreatorID)))).toString());
    info->setGroupName(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PIG_GroupName)))).toString());
    info->setCreationTime(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PIG_CreationTime)))).toDateTime());

    info->setGroupInfoVersion(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PIG_GroupInfoVersion)))).toUInt());
    info->setGroupMemberListInfoVersion(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PIG_MemberListInfoVersion)))).toUInt());
    info->setDescription(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PIG_Description)))).toString());
    info->setAnnouncement(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PIG_Announcement)))).toString());
    info->setRemark(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PIG_Remark)))).toString());
    info->setState(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PIG_State)))).toUInt());
    info->setPrivacy(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PIG_Privacy)))).toUInt());


    //    info->setLastUpdateTime(QVariant(query.value(record.indexOf(info->databaseColumnName(IM::PIG_LastUpdateTime)))).toDateTime());

    
    info->clearUpdatedProperties();
    
    
    
    //TODO:Members

    //statement = QString("select MemberUserID, MemberRole from groupmembers where GroupID=%1 ").arg(info->getGroupID());
    statement = QString("call sp_GetInterestGroupMembers(%1);").arg(groupID);
    query.clear();
    if(!query.exec(statement)){
        QSqlError error = query.lastError();
        QString msg = QString("Can not query group members info from database! %1 Error Type:%2 Error NO.:%3").arg(error.text()).arg(error.type()).arg(error.number());
        qCritical()<<msg;

        return false;
    }

    if((query.lastError().type() != QSqlError::NoError)){
        qCritical()<<QString("Can not query data from database! Error: %1").arg(query.lastError().text());
        return false;
    }

    QHash <QString/*Member's ID*/, InterestGroup::MemberRole/*Member's Role*/> membersHash;
    
    while (query.next()) {
        membersHash.insert(query.value(0).toString(), InterestGroup::MemberRole(query.value(1).toUInt()) );
    }
    
    info->setMembersHash(membersHash);
    
    m_interestGroupHash->insert(groupID, info);

    return true;
    
    
}











/////////////////////////// ---Group Manager--- ////////////////////////////////////////    













} //namespace HEHUI
