/*
 * global_shared.h
 *
 *  Created on: 2009-4-27
 *      Author: 贺辉
 */

#ifndef CONSTANTS_GLOBAL_SHARED_H_
#define CONSTANTS_GLOBAL_SHARED_H_



//#ifndef APP_NAME
//#define APP_NAME	"Messenger"
//#endif
//
//#ifndef APP_VERSION
//#define APP_VERSION	"2011.2.25.1"
//#endif
//
//#ifndef APP_ICON_PATH
//#define APP_ICON_PATH	"/images/app.png"
//#endif
//
//#ifndef APP_AUTHOR
//#define APP_AUTHOR	"HeHui"
//#endif
//
//#ifndef APP_AUTHOR_EMAIL
//#define APP_AUTHOR_EMAIL	"hehui@hehui.org"
//#endif
//
//#ifndef APP_ORG
//#define APP_ORG	"HeHui"
//#endif
//
//#ifndef APP_ORG_DOMAIN
//#define APP_ORG_DOMAIN	"www.hehui.org"
//#endif
//
//#ifndef APP_LICENSE
//#define APP_LICENSE	"LGPL"
//#endif


//MySQL
#ifndef REMOTE_SITOY_COMPUTERS_DB_CONNECTION_NAME
    #define REMOTE_SITOY_COMPUTERS_DB_CONNECTION_NAME	"SITOY_COMPUTERS_DB"
#endif

#ifndef REMOTE_SITOY_COMPUTERS_DB_DRIVER
    #define REMOTE_SITOY_COMPUTERS_DB_DRIVER	"QMYSQL"
#endif

#ifndef REMOTE_SITOY_COMPUTERS_DB_SERVER_HOST
    #define REMOTE_SITOY_COMPUTERS_DB_SERVER_HOST	"200.200.200.40"
#endif

#ifndef REMOTE_SITOY_COMPUTERS_DB_SERVER_PORT
    #define REMOTE_SITOY_COMPUTERS_DB_SERVER_PORT	3306
#endif

#ifndef REMOTE_SITOY_COMPUTERS_DB_USER_NAME
    #define REMOTE_SITOY_COMPUTERS_DB_USER_NAME	"hehui"
#endif

#ifndef REMOTE_SITOY_COMPUTERS_DB_USER_PASSWORD
    #define REMOTE_SITOY_COMPUTERS_DB_USER_PASSWORD	"hehui"
#endif

#ifndef REMOTE_SITOY_COMPUTERS_DB_NAME
    #define REMOTE_SITOY_COMPUTERS_DB_NAME	"im"
#endif

#ifndef IM_SERVER_IPMC_ADDRESS
    #define IM_SERVER_IPMC_ADDRESS	"239.239.239.239"
#endif

#ifndef IM_SERVER_IPMC_LISTENING_PORT
    #define IM_SERVER_IPMC_LISTENING_PORT	54320
#endif

#ifndef IM_SERVER_RTP_LISTENING_PORT
    #define IM_SERVER_RTP_LISTENING_PORT	54321
#endif

//#ifndef IM_SERVER_TCP_LISTENING_PORT
//#define IM_SERVER_TCP_LISTENING_PORT	54319
//#endif

#ifndef INVALID_SOCK_ID
    #define INVALID_SOCK_ID	-1
#endif

#ifndef CRYPTOGRAPHY_KEY
    #define CRYPTOGRAPHY_KEY "HEHUI"
#endif

//#ifndef HEARTBEAT_TIMER_INTERVAL
//#define HEARTBEAT_TIMER_INTERVAL 600000
//#endif

#ifndef MIN_THREAD_COUNT
    #define MIN_THREAD_COUNT 10
#endif


#include "HHSharedNetwork/hglobal_network.h"






#ifndef MAX_AUTHENTICATION_REQUEST_TIMES
    #define MAX_AUTHENTICATION_REQUEST_TIMES 5
#endif

#ifndef TIME_FOR_BAN_ON_AUTHENTICATION_REQUEST
    #define TIME_FOR_BAN_ON_AUTHENTICATION_REQUEST 7200000 //2 hours
#endif

#ifndef MAX_CONTACT_NUMBER
    #define MAX_CONTACT_NUMBER	 512
#endif

#ifndef GROUP_INFO_SEPARATOR
    #define GROUP_INFO_SEPARATOR	 "||"
#endif

#ifndef CONTACT_INFO_SEPARATOR
    #define CONTACT_INFO_SEPARATOR	 ","
#endif
#ifndef CONTACTS_INFO_ROW_SEPARATOR
    #define CONTACTS_INFO_ROW_SEPARATOR	 ";"
#endif



#ifndef SEPARTOR_GROUP
    #define SEPARTOR_GROUP 0X1D
#endif

#ifndef SEPARTOR_RECORD
    #define SEPARTOR_RECORD 0X1E
#endif

#ifndef SEPARTOR_UNIT
    #define SEPARTOR_UNIT 0X1F
#endif


#ifndef MAX_MESSAGE_SIZE
    #define MAX_MESSAGE_SIZE    470
#endif

//#ifndef IM_RUDP_KEEPALIVE_TIMER_INTERVAL
//#define IM_RUDP_KEEPALIVE_TIMER_INTERVAL 30000 //1 minute
//#endif

#ifndef FILE_PIECES_IN_ONE_REQUEST
    #define FILE_PIECES_IN_ONE_REQUEST 20 //20 Pieces
#endif


#ifndef SEARCH_RESULT_PAGE_SIZE
    #define SEARCH_RESULT_PAGE_SIZE 10
#endif

#ifndef HISTORY_MESSAGES_PAGE_SIZE
    #define HISTORY_MESSAGES_PAGE_SIZE 50
#endif


namespace HEHUI
{

namespace IM
{

enum OnlineState {
    ONLINESTATE_ONLINE = 1,
    ONLINESTATE_OFFLINE,
    ONLINESTATE_INVISIBLE,
    ONLINESTATE_BUSY,
    ONLINESTATE_AWAY
};



enum Command {

    CMD_ServerDiscovery,
    CMD_DataForward,
    CMD_Announcement,
    CMD_Rgeistration,
    CMD_UpdatePassword,
    CMD_Login,
    CMD_OnlineStateChanged,
    CMD_OnlineContacts,
    CMD_ContactGroupsInfo,
    CMD_InterestGroupsInfo,
    CMD_ContactInfo,
    CMD_SearchInfo,
    CMD_ChatMessage,
    CMD_Captcha,

    CMD_FileTransfer,





    //File TX
    SERVER_RESPONSE_FILE_SERVER_INFO,

    REQUEST_DOWNLOAD_FILE,
    CANCEL_FILE_DOWNLOAD_REQUEST,
    RESPONSE_FILE_DOWNLOAD_REQUEST,
    //    CLIENT_REQUEST_UPLOADING_OFFLINE_FILE,
    //    SERVER_RESPONSE_UPLOADING_OFFLINE_FILE,

    REQUEST_UPLOAD_FILE,
    CANCEL_FILE_UPLOAD_REQUEST,
    RESPONSE_FILE_UPLOAD_REQUEST,


    CMD_JobProgress,
    CMD_AdminLogin,
    CMD_ClientInfo,


};

enum FileTXStatus {
    File_TX_Preparing,
    File_TX_Receiving,
    File_TX_Sending,
    //File_TX_Transferring,
    File_TX_Progress,
    File_TX_Paused,
    File_TX_Aborted,
    File_TX_Done
};

enum ErrorType {
    ERROR_NoError = 0,

    ERROR_SoftwareVersionExpired = 1,
    ERROR_IPBanned,
    ERROR_IDBanned,
    ERROR_IDBlacklisted,
    ERROR_IDNotExist,
    ERROR_PasswordIncorrect,
    ERROR_Duplicate_Login,

    ERROR_GROUP_ID_NotExist,


    ERROR_Timeout,
    ERROR_ConnectionFailed,
    ERROR_KickedOut,


    ERROR_RequestDenied,

    ERROR_AuthenticationNeeded,
    ERROR_AuthenticationFailed,

    //registration
    ERROR_IDEXISTED,
    ERROR_EMAILEXISTED,


    ERROR_ServerError,

    ERROR_UnKnownError = 255
};

enum PropertyIDOfUser {
    PI_SysID = 1,
    PI_UserID,
    PI_Password,

    PI_TrueName,
    PI_NickName,
    PI_Gender,
    PI_Face,

    //    PI_PersonalContactGroupsInfoString,
    PI_PersonalContactGroupsInfoVersion,
    //    PI_InterestGroupsInfoString,
    PI_InterestGroupsInfoVersion,
    PI_PersonalMessageInfoVersion,
    PI_PersonalMessage,

    //    PI_Blacklist,
    //    PI_PersonalInfoVersion,
    PI_PersonalSummaryInfoVersion,
    PI_PersonalDetailInfoVersion,
    PI_FriendshipApply,
    PI_ShortTalk,
    PI_Role,
    PI_Description,
    PI_AccountState,

    PI_Age,
    PI_HomeAddress,
    PI_HomePhoneNumber,
    PI_HomeZipCode,
    PI_PersonalHomepage,
    PI_PersonalEmailAddress,
    PI_QuestionForSecurity,
    PI_AnswerForSecurity,
    PI_EmailForSecurity,
    PI_CompanyName,
    PI_JobTitle,
    PI_BusinessAddress,
    PI_BusinessPhoneNumber,
    PI_BusinessZipCode,
    PI_BusinessFaxNumber,
    PI_BusinessHomepage,
    PI_BusinessEmailAddress,
    PI_RegistrationTime,
    //    PI_LoginTimes,


    //    PI_LastLoginTime,
    //    PI_LastLoginHostAddress,
    //    PI_LastLoginHostPort,


    //For Contact
    //    PI_InterestGroupID,
    PI_ContactGroupID,
    PI_RemarkName



};

enum PropertyIDOfGroup {
    PIG_GroupID = 1,

    PIG_GroupTypeID,
    PIG_ParentGroupID,
    PIG_GroupName,
    PIG_CreatorID,
    PIG_CreationTime,

    PIG_GroupInfoVersion,
    PIG_MemberListInfoVersion,
    PIG_Description,
    PIG_Announcement,
    PIG_Remark,
    PIG_State,
    PIG_Privacy,


    //For Table InterestGroupMembers
    PIG_MemberSysID,
    PIG_MemberRole

};












} //namespace IM

} //namespace HEHUI

#endif /* CONSTANTS_GLOBAL_SHARED_H_ */
