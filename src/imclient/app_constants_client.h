/*
 * app_constants.h
 *
 *  Created on: 2009-4-27
 *      Author: 贺辉
 */

#ifndef APP_CONSTANTS_CLIENT_H_
#define APP_CONSTANTS_CLIENT_H_

//#include "../../shared/core/global_core.h"
#include "HHSharedCore/hglobal_core.h"


#ifndef APP_NAME
#define APP_NAME	"Messenger"
#endif

#ifndef APP_VERSION
#define APP_VERSION	"2011.2.25.1"
#endif

#ifndef APP_ICON_PATH
#define APP_ICON_PATH	"/images/app.png"
#endif

#ifndef APP_AUTHOR
#define APP_AUTHOR	"HeHui"
#endif

#ifndef APP_AUTHOR_EMAIL
#define APP_AUTHOR_EMAIL	"hehui@hehui.org"
#endif

#ifndef APP_ORG
#define APP_ORG	"HeHui"
#endif

#ifndef APP_ORG_DOMAIN
#define APP_ORG_DOMAIN	"www.hehui.org"
#endif

#ifndef APP_LICENSE
#define APP_LICENSE	"LGPL"
#endif

#ifndef APP_SPLASH_IMAGE_PATH
#define APP_SPLASH_IMAGE_PATH	"/images/splash.png"
#endif


#ifndef LOCAL_USERDATA_DB_CONNECTION_NAME
#define LOCAL_USERDATA_DB_CONNECTION_NAME	"LOCAL_USERDATA_DB"
#endif

#ifndef LOCAL_USERDATA_DB_NAME
#define LOCAL_USERDATA_DB_NAME	"userdata.db"
#endif

#ifndef LOCAL_USERDATA_DB_DRIVER
#define LOCAL_USERDATA_DB_DRIVER	"QSQLITE"
#endif



#ifndef USER_FACE_FILE_PATH_PREFIX
#define USER_FACE_FILE_PATH_PREFIX	":/face"
#endif

#ifndef USER_FACE_DEFAULT_INDEX
#define USER_FACE_DEFAULT_INDEX	 "0"
#endif

#ifndef PNG_FILE_PATH_AWAY
#define PNG_FILE_PATH_AWAY	 "/images/away.png"
#endif

#ifndef PNG_FILE_PATH_BUSY
#define PNG_FILE_PATH_BUSY	 "/images/busy.png"
#endif

#ifndef PNG_FILE_PATH_INVISIBLE
#define PNG_FILE_PATH_INVISIBLE	 "/images/invisible.png"
#endif

#ifndef MAX_GROUP_MEMBERS_COUNT
#define MAX_GROUP_MEMBERS_COUNT	 256
#endif

#ifndef MAX_GROUPS_COUNT
#define MAX_GROUPS_COUNT    20
#endif





enum SystemTrayIconDataType{
    STIDT_Unknown = 0,
    STIDT_SystemMessage,
    STIDT_ServerAnnouncement,
    STIDT_FriendshipApplicationResult,
    STIDT_FriendshipApplicationFromContact,
    STIDT_ContactChatMessage,
    STIDT_InterestGroupChatMessage,
    STIDT_InterestGroupMemberJoinedOrQuitted
    


};











#endif /* APP_CONSTANTS_CLIENT_H_ */
