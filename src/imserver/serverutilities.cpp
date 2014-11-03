/*
 * serverutilities.cpp
 *
 *  Created on: 2009-12-29
 *      Author: 贺辉
 */


#include <QDateTime>


#include "serverutilities.h"
#include "settings.h"
#include "app_constants_server.h"

//#include "../../../shared/core/database/databaseutility.h"


namespace HEHUI {

ServerUtilities::ServerUtilities() {
    // TODO Auto-generated constructor stub

}

ServerUtilities::~ServerUtilities() {
    // TODO Auto-generated destructor stub
}

QByteArray ServerUtilities::generateSessionEncryptionKey(){
    QByteArray tempKeyArray;
    qsrand(QDateTime::currentDateTime().time().msec());
    for(int i=0; i<16; i++){
        int k = qrand()%255;
        if(k < 0){
            k += 255;
        }
        tempKeyArray.append(k);
    }

    return tempKeyArray;


}




QSqlQuery ServerUtilities::queryDatabase(const QString & queryString, bool localConfigDatabase) {

    QSqlQuery query;
    DatabaseUtility du;

    Settings settings;
    QString localDatabaseFilePath = settings.getLocalDatabaseFilePath();


    if(localConfigDatabase){
        query = du.queryDatabase(queryString,
                                 LOCAL_SERVER_DB_CONNECTION_NAME,
                                 LOCAL_SERVER_DB_DRIVER,
                                 "",
                                 0,
                                 "",
                                 "",
                                 localDatabaseFilePath,
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


QSqlQuery ServerUtilities::queryDatabase(const QString & queryString, const QString &connectionName, const QString &driver,
                                         const QString &host, int port, const QString &user, const QString &passwd,
                                         const QString &databaseName, HEHUI::DatabaseType databaseType) {


    QSqlQuery query;
    DatabaseUtility du;

    query = du.queryDatabase(queryString, connectionName, driver, host, port, user, passwd, databaseName, databaseType);

    return query;

}

//QString ServerUtilities::databaseColumnName(IM::PropertyID propertyID){
//    QString columnName = "";
//    switch(propertyID){
//    case IM::PI_UserID:
//        columnName = "UserID";
//        break;
//    case IM::PI_Password:
//        columnName = "UserPassword";
//        break;
//    case IM::PI_TrueName:
//        columnName = "TrueName";
//        break;
//    case IM::PI_NickName:
//        columnName = "NickName";
//        break;
//    case IM::PI_Gender:
//        columnName = "Gender";
//        break;
//    case IM::PI_Age:
//        columnName = "Age";
//        break;
//    case IM::PI_Face:
//        columnName = "Face";
//        break;

//    case IM::PI_PersonalContactGroupsInfoString:
//        columnName = "PersonalContactGroupsInfo";
//        break;  
//    case IM::PI_PersonalContactGroupsInfoVersion:
//        columnName = "PersonalContactGroupsInfoVersion";
//        break;         
//    case IM::PI_PersonalInterestGroupsInfoString:
//        columnName = "PersonalInterestGroupsInfo";
//        break;  
//    case IM::PI_PersonalInterestGroupsInfoVersion:
//        columnName = "PersonalInterestGroupsInfoVersion";
//        break; 
//    case IM::PI_SystemGroupsInfoString:
//        columnName = "SystemGroupsInfo";
//        break; 
//    case IM::PI_SystemGroupsInfoVersion:
//        columnName = "SystemGroupsInfoVersion";
//        break; 
//    case IM::PI_PersonalInfoVersion:
//        columnName = "PersonalInfoVersion";
//        break; 


//    case IM::PI_HomeAddress:
//        columnName = "HomeAddress";
//        break;
//    case IM::PI_HomePhoneNumber:
//        columnName = "HomePhoneNumber";
//        break;
//    case IM::PI_HomeZipCode:
//        columnName = "HomeZipCode";
//        break;
//    case IM::PI_PersonalHomepage:
//        columnName = "PersonalHomepage";
//        break;
//    case IM::PI_PersonalEmailAddress:
//        columnName = "PersonalEmailAddress";
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
//    case IM::PI_QuestionForSecurity:
//        columnName = "QuestionForSecurity";
//        break;
//    case IM::PI_AnswerForSecurity:
//        columnName = "AnswerForSecurity";
//        break;

//    case IM::PI_CompanyName:
//        columnName = "CompanyName";
//        break;
//    case IM::PI_JobTitle:
//        columnName = "JobTitle";
//        break;
//    case IM::PI_BusinessAddress:
//        columnName = "BusinessAddress";
//        break;
//    case IM::PI_BusinessPhoneNumber:
//        columnName = "BusinessPhoneNumber";
//        break;
//    case IM::PI_BusinessZipCode:
//        columnName = "BusinessZipCode";
//        break;
//    case IM::PI_BusinessFaxNumber:
//        columnName = "BusinessFaxNumber";
//        break;
//    case IM::PI_BusinessHomepage:
//        columnName = "BusinessHomepage";
//        break;
//    case IM::PI_BusinessEmailAddress:
//        columnName = "BusinessEmailAddress";
//        break;
//    case IM::PI_RegistrationTime:
//        columnName = "RegistrationTime";
//        break;
//    case IM::PI_LoginTimes:
//        columnName = "LoginTimes";
//        break;
//    case IM::PI_FriendshipApply:
//        columnName = "FriendshipApply";
//        break;    
//    case IM::PI_ShortTalk:
//        columnName = "ShortTalk";
//        break;  

//    case IM::PI_Role:
//        columnName = "Role";
//        break;         

//    default:
//        columnName = "";




//    }

//    return columnName;



//}














} //namespace HEHUI
