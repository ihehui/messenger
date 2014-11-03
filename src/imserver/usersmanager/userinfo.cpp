/*
 ****************************************************************************
 * userinfo.cpp
 *
 * Created on: 2010-5-27
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
 * Last Modified on: 2010-5-31
 * Last Modified by: 贺辉
 ***************************************************************************
 */





#include "userinfo.h"

#include "../../sharedim/constants_global_shared.h"
#include "../serverutilities.h"


namespace HEHUI {


UserInfo::UserInfo(const QString &userID, QObject *parent)
    :IMUserBase(userID, parent)
{

    init();
    setLastLoginExternalHostAddress("");
    setLastLoginExternalHostPort(0);

}

UserInfo::UserInfo(const QString &ipAddress, quint16 port, QObject *parent)
    :IMUserBase(parent)
{

    init();

    setUserID("");
    setLastLoginExternalHostAddress(ipAddress);
    setLastLoginExternalHostPort(port);



}

UserInfo::~UserInfo(){


}

void UserInfo::init(){
    setPassword("");
    setVerified(false);

    //    remainingOnlineStatusCheckingTimes = ONLINE_STATUS_CHECKING_TIMES;

    remainingAuthenticationRequestTimes = MAX_AUTHENTICATION_REQUEST_TIMES;
    bannedFromAuthenticationRequest = false;
    timeToLiftTheBan = QDateTime();
    //    timeLastHeartbeatPacketReceived = QDateTime();

    m_sysID = 0;

    interestGroups.clear();


}

void UserInfo::joinOrLeaveInterestGroup(quint32 interestGroupID, bool join){

    if(join){
        if(interestGroups.contains(interestGroupID)){
            return;
        }else{
            interestGroups.append(interestGroupID);
        }
    }else{
        interestGroups.removeAll(interestGroupID);
    }

    updateInterestGroupInfoVersion();

}

bool UserInfo::isMemberOfInterestGroup(quint32 interestGroupID){
    return interestGroups.contains(interestGroupID);
}

bool UserInfo::isOnLine(){
    //    if(QDateTime::currentDateTime() < timeLastHeartbeatPacketReceived.addMSecs(ONLINE_STATUS_CHECKING_TIMES*HEARTBEAT_TIMER_INTERVAL)){
    //        return true;
    //    }
    //    return false;

    if(getOnlineState() == IM::ONLINESTATE_OFFLINE){
        return false;
    }

    return true;

}

//void UserInfo::updateRemainingOnlineStatusCheckingTimes(bool heartbeatPacketReceived){
//    if(heartbeatPacketReceived){
//        timeLastHeartbeatPacketReceived = QDateTime::currentDateTime();
//        remainingOnlineStatusCheckingTimes ++;
//        if(remainingOnlineStatusCheckingTimes > ONLINE_STATUS_CHECKING_TIMES){
//            remainingOnlineStatusCheckingTimes = ONLINE_STATUS_CHECKING_TIMES;
//        }
//    }else{
//        remainingOnlineStatusCheckingTimes --;
//    }

//}

bool UserInfo::isBanned(){
    if(bannedFromAuthenticationRequest){
        if(timeToLiftTheBan > QDateTime::currentDateTime()){
            return true;
        }else{
            bannedFromAuthenticationRequest = false;
            remainingAuthenticationRequestTimes = MAX_AUTHENTICATION_REQUEST_TIMES;
            return false;
        }
    }

    return bannedFromAuthenticationRequest;
}

void UserInfo::setBanned(bool banned){
    this->bannedFromAuthenticationRequest = banned;
}

//void UserInfo::ban(){
//    bannedFromAuthenticationRequest = true;
//    timeToLiftTheBan = QDateTime::currentDateTime().addMSecs(TIME_FOR_BAN_ON_AUTHENTICATION_REQUEST);
//}
//
//void UserInfo::liftTheBan(){
//    bannedFromAuthenticationRequest = false;
//    timeToLiftTheBan = QDateTime();
//}

void UserInfo::authenticationFailed(){
    if(bannedFromAuthenticationRequest){
        return;
    }

    remainingAuthenticationRequestTimes --;

    if(remainingAuthenticationRequestTimes <= 0){
        bannedFromAuthenticationRequest = true;
        timeToLiftTheBan = QDateTime::currentDateTime().addMSecs(TIME_FOR_BAN_ON_AUTHENTICATION_REQUEST);
    }

}

void UserInfo::setOnline(){

    //    remainingOnlineStatusCheckingTimes = ONLINE_STATUS_CHECKING_TIMES;

    remainingAuthenticationRequestTimes = MAX_AUTHENTICATION_REQUEST_TIMES;
    bannedFromAuthenticationRequest = false;
    timeToLiftTheBan = QDateTime();

    //    timeLastHeartbeatPacketReceived = QDateTime::currentDateTime();
    //    setLastLoginTime(timeLastHeartbeatPacketReceived);
}

void UserInfo::setOffline(){

    //QDateTime time = QDateTime::currentDateTime();
    //setLastLoginTime(time);
    //    setLastLoginTime(timeLastHeartbeatPacketReceived);

    //    addUpdatedProperty(IM::PI_LastLoginTime, "'"+timeLastHeartbeatPacketReceived.toString("yyyy-MM-dd hh:mm:ss")+"'");

    //TODO:Save to database


    setLastLoginExternalHostPort(0);

}

QString UserInfo::databaseColumnName(IM::PropertyIDOfUser propertyID) const{
    QString columnName = "";
    switch(propertyID){
    case IM::PI_SysID:
        columnName = "SysID";
        break;
    case IM::PI_UserID:
        columnName = "UserID";
        break;
    case IM::PI_Password:
        columnName = "UserPassword";
        break;
    case IM::PI_TrueName:
        columnName = "TrueName";
        break;
    case IM::PI_NickName:
        columnName = "NickName";
        break;
    case IM::PI_Gender:
        columnName = "Gender";
        break;
    case IM::PI_Age:
        columnName = "Age";
        break;
    case IM::PI_Face:
        columnName = "Face";
        break;
//    case IM::PI_PersonalContactGroupsInfoString:
//        columnName = "PersonalContactGroupsInfo";
//        break;
    case IM::PI_PersonalContactGroupsInfoVersion:
        columnName = "PersonalContactGroupsInfoVersion";
        break;
//    case IM::PI_InterestGroupsInfoString:
//        columnName = "InterestGroupsInfo";
//        break;
    case IM::PI_InterestGroupsInfoVersion:
        columnName = "InterestGroupsInfoVersion";
        break;
    case IM::PI_PersonalMessageInfoVersion:
        columnName = "PersonalMessageInfoVersion";
        break;
    case IM::PI_PersonalMessage:
        columnName = "PersonalMessage";
        break;
//    case IM::PI_Blacklist:
//        columnName = "Blacklist";
//        break;
    case IM::PI_PersonalDetailInfoVersion:
        columnName = "PersonalDetailInfoVersion";
        break;
    case IM::PI_PersonalSummaryInfoVersion:
        columnName = "PersonalSummaryInfoVersion";
        break;
    case IM::PI_FriendshipApply:
        columnName = "FriendshipApply";
        break;
    case IM::PI_ShortTalk:
        columnName = "ShortTalk";
        break;
    case IM::PI_Role:
        columnName = "Role";
        break;
    case IM::PI_Description:
        columnName = "Description";
        break;
    case IM::PI_AccountState:
        columnName = "AccountState";
        break;


    case IM::PI_HomeAddress:
        columnName = "HomeAddress";
        break;
    case IM::PI_HomePhoneNumber:
        columnName = "HomePhoneNumber";
        break;
    case IM::PI_HomeZipCode:
        columnName = "HomeZipCode";
        break;
    case IM::PI_PersonalHomepage:
        columnName = "PersonalHomepage";
        break;
    case IM::PI_PersonalEmailAddress:
        columnName = "PersonalEmailAddress";
        break;

    case IM::PI_QuestionForSecurity:
        columnName = "QuestionForSecurity";
        break;
    case IM::PI_AnswerForSecurity:
        columnName = "AnswerForSecurity";
        break;
    case IM::PI_EmailForSecurity:
        columnName = "EmailForSecurity";
        break;
    case IM::PI_CompanyName:
        columnName = "CompanyName";
        break;
    case IM::PI_JobTitle:
        columnName = "JobTitle";
        break;
    case IM::PI_BusinessAddress:
        columnName = "BusinessAddress";
        break;
    case IM::PI_BusinessPhoneNumber:
        columnName = "BusinessPhoneNumber";
        break;
    case IM::PI_BusinessZipCode:
        columnName = "BusinessZipCode";
        break;
    case IM::PI_BusinessFaxNumber:
        columnName = "BusinessFaxNumber";
        break;
    case IM::PI_BusinessHomepage:
        columnName = "BusinessHomepage";
        break;
    case IM::PI_BusinessEmailAddress:
        columnName = "BusinessEmailAddress";
        break;
    case IM::PI_RegistrationTime:
        columnName = "RegistrationTime";
        break;
//    case IM::PI_LoginTimes:
//        columnName = "LoginTimes";
//        break;


//    case IM::PI_LastLoginTime:
//        columnName = "LastLoginTime";
//        break;
//    case IM::PI_LastLoginHostAddress:
//        columnName = "LastLoginHostAddress";
//        break;
//    case IM::PI_LastLoginHostPort:
//        columnName = "LastLoginHostPort";
//        break;

        
    default:
        columnName = "";




    }

    return columnName;



}







//QString UserInfo::getUpdateSQLStatement() const{
//    QMutexLocker locker(updatedPropertiesMutex);

//    QStringList sqlstatements;
//    QList<IM::PropertyID> keys = updatedProperties.keys();
//    foreach (IM::PropertyID propertyID, keys) {
//        QString propertyName = ServerUtilities::databaseColumnName(propertyID);
//        if(propertyName.isEmpty()){
//            qCritical()<<QString("Unknown Property ID '%1'!").arg(propertyID);
//            continue;
//        }
//        sqlstatements.append(QString(propertyName + "=" + updatedProperties.value(propertyID)));
//    }

//    return sqlstatements.join(" , ");



//}

//void UserInfo::clearUpdatedProperties(){
//    QMutexLocker locker(updatedPropertiesMutex);

//    updatedProperties.clear();


//}






} //namespace HEHUI

