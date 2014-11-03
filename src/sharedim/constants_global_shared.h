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


namespace HEHUI {

namespace IM {

enum OnlineState {
        ONLINESTATE_ONLINE = 1, ONLINESTATE_OFFLINE, ONLINESTATE_INVISIBLE, ONLINESTATE_BUSY, ONLINESTATE_AWAY
};



enum Command {

    DataForwardRequestByClient, //c
    ForwardedDataByServer, //s

    ClientLookForServer = UserDefinedPacket + 100, //106
    ServerDeclare,
    ServerOnline,
    ServerOffline,
    ServerAnnouncement, //110
    

    CLIENT_REQUEST_REGISTRATION_SERVER_INFO, //c
    SERVER_RESPONSE_CLIENT_REQUEST_REGISTRATION_SERVER_INFO, //s
    CLIENT_REGISTRATION, //c
    SERVER_RESPONSE_CLIENT_REGISTRATION, //s

    CLIENT_REQUEST_UPDATE_PASSWORD,
    SERVER_RESPONSE_CLIENT_REQUEST_UPDATE_PASSWORD,

    CLIENT_REQUEST_LOGIN, //c
    SERVER_RESPONSE_CLIENT_REQUEST_LOGIN, //s
    CLIENT_LOGIN_INFO, //c
    SERVER_RESPONSE_CLIENT_LOGIN_RESULT, //s
    SERVER_RESPONSE_CLIENT_LOGIN_REDIRECTION, //s
    CLIENT_LAST_LOGIN_INFO, //s

    ONLINE_STATE_CHANGED, //c s

    CONTACTS_ONLINE_INFO, //c,s



//    CLIENT_REQUEST_PERSONAL_INFO,
//    SERVER_RESPONSE_CLIENT__PERSONAL_INFO,

    CONTACT_GROUPS_INFO, //c s
    CONTACTS_INFO_VERSION, //c s

    CLIENT_REQUEST_USER_INFO, //c
    SERVER_RESPONSE_USER_INFO, //s

//    CLIENT_REQUEST_CONTACT_GROUPS_LIST,
//    SERVER_RESPONSE__CONTACT_GROUPS_LIST,
//    CLIENT_REQUEST_CONTACT_GROUP_INFO,
//    SERVER_RESPONSE__CONTACT_GROUP_INFO,

    CLIENT_REQUEST_INTEREST_GROUPS_LIST,
    SERVER_RESPONSE_INTEREST_GROUPS_LIST,
    CLIENT_REQUEST_INTEREST_GROUP_INFO,
    SERVER_RESPONSE_INTEREST_GROUP_INFO,
    CLIENT_REQUEST_INTEREST_GROUP_MEMBERS_INFO,
    SERVER_RESPONSE_INTEREST_GROUP_MEMBERS_INFO,
    CLIENT_REQUEST_CREATE_INTEREST_GROUP,
    SERVER_RESPONSE_CREATE_INTEREST_GROUP,
    CLIENT_REQUEST_DISBAND_INTEREST_GROUP,
    SERVER_RESPONSE_DISBAND_INTEREST_GROUP,



    CLIENT_REQUEST_PERSONAL_MESSAGE_INFO, //c
    SERVER_RESPONSE_PERSONAL_MESSAGE_INFO, //s


//    CLIENT_REQUEST_SYSTEM_GROUPS_LIST,
//    SERVER_RESPONSE_SYSTEM_GROUPS_LIST,
//    CLIENT_REQUEST_SYSTEM_GROUP_INFO,
//    SERVER_RESPONSE_SYSTEM_GROUP_INFO,

    CLIENT_REQUEST_SEARCH_CONTACTS, //c
    SERVER_RESPONSE_SEARCH_CONTACTS, //s

    CLIENT_REQUEST_SEARCH_INTERESTGROUPS, //c
    SERVER_RESPONSE_SEARCH_INTERESTGROUPS, //s

    CLIENT_REQUEST_JOIN_OR_QUIT_INTERESTGROUP, //c
    SERVER_RESPONSE_JOIN_OR_QUIT_INTERESTGROUP, //s
    USER_REQUEST_JOIN_INTERESTGROUP,
    ADMIN_RESPONSE_JOIN_INTERESTGROUP,

    CLIENT_REQUEST_ADD_CONTACT, //c
    CLIENT_RESPONSE_ADD_CONTACT_REQUEST, //c, s
    CLIENT_REQUEST_DELETE_CONTACT, //c
    SERVER_RESPONSE_DELETE_CONTACT, //s

//    CLIENT_REQUEST_ADD_OR_DELETE_BLACKLISTED_CONTACT,
//    BLACKLIST_INFO,

    CLIENT_REQUEST_MOVE_CONTACT, //c
//    SERVER_RESPONSE_MOVE_CONTACT, //s
    CREATE_OR_DELETE_CONTACT_GROUP, //c
    SERVER_RESPONSE_CREATE_OR_DELETE_CONTACT_GROUP, //s
    RENAME_CONTACT_GROUP, //c
//    SERVER_RESPONSE_RENAME_CONTACT_GROUP, //s

    CLIENT_REQUEST_MODIFY_CONTACT_REMARK, //c
//    SERVER_RESPONSE_MODIFY_CONTACT_REMARK,

//    SERVER_MESSAGE_CLIENT_SOFTWARE_UPDATE,
//    CLIENT_RESPONSE_SERVER_MESSAGE,
//    SERVER_RESPONSE_CLIENT_STATE_CHANGED,

    SESSION_ENCRYPTION_KEY_WITH_CONTACT, //s
    CHAT_MESSAGE_FROM_CONTACT, //c
    CHAT_MESSAGES_CACHED_ON_SERVER, //s

    GROUP_CHAT_MESSAGE_TO_SERVER, //c
    GROUP_CHAT_MESSAGE_FROM_SERVER, //s
    GROUP_CHAT_MESSAGES_CACHED_ON_SERVER,

    CHAT_IMAGE, //c, s
    REQUEST_CHAT_IMAGE, //c, s


    //File TX
    CLIENT_REQUEST_FILE_SERVER_INFO,
    SERVER_RESPONSE_FILE_SERVER_INFO,

    REQUEST_DOWNLOAD_FILE,
    CANCEL_FILE_DOWNLOAD_REQUEST,
    RESPONSE_FILE_DOWNLOAD_REQUEST,
//    CLIENT_REQUEST_UPLOADING_OFFLINE_FILE,
//    SERVER_RESPONSE_UPLOADING_OFFLINE_FILE,

    REQUEST_UPLOAD_FILE,
    CANCEL_FILE_UPLOAD_REQUEST,
    RESPONSE_FILE_UPLOAD_REQUEST,
//    CancelUploadFileRequest,
//    ResponseFileUploadRequest,

//    RequestFileData,
//    FileData,
//    FileTXStatusChanged,
//    FileTXError,

};

enum FileTXStatus{
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

enum PropertyIDOfUser{
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

enum PropertyIDOfGroup{
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



enum RegistrationMode{
    RM_UserDefineAll,
    RM_ServerCreateAll,
    RM_ServerCreateUserIDOnly,
    RM_ServerCreatePasswordOnly

};









} //namespace IM

} //namespace HEHUI

#endif /* CONSTANTS_GLOBAL_SHARED_H_ */
