/*
 * app_constants_server.h
 *
 *  Created on: 2009-4-27
 *      Author: 贺辉
 */

#ifndef APP_CONSTANTS_SERVER_H_
#define APP_CONSTANTS_SERVER_H_

//#include "../../shared/core/global_core.h"
#include "HHSharedCore/hglobal_core.h"


#ifndef SETTINGS_FILE_NAME
#define SETTINGS_FILE_NAME	"Server"
#endif

#ifndef APP_NAME
#define APP_NAME	"Server"
#endif

#ifndef APP_VERSION
#define APP_VERSION	"2011.2.25.1"
#endif

#ifndef APP_ORG_DOMAIN
#define APP_ORG_DOMAIN	"www.gnu.org"
#endif

#ifndef APP_ORG
#define APP_ORG	"HeHui"
#endif

#ifndef APP_AUTHOR
#define APP_AUTHOR	"He Hui"
#endif

#ifndef APP_AUTHOR_EMAIL
#define APP_AUTHOR_EMAIL	"hehui@hehui.org"
#endif

#ifndef APP_LICENSE
#define APP_LICENSE	"LGPL"
#endif





#ifndef LOCAL_SERVER_DATA_DIR_NAME
#define LOCAL_SERVER_DATA_DIR_NAME	"data"
#endif


#ifndef LOCAL_SERVER_DB_CONNECTION_NAME
#define LOCAL_SERVER_DB_CONNECTION_NAME	"LOCAL_SERVER_DB"
#endif

#ifndef LOCAL_SERVER_DB_NAME
#define LOCAL_SERVER_DB_NAME	"data.db"
#endif

#ifndef LOCAL_SERVER_DB_DRIVER
#define LOCAL_SERVER_DB_DRIVER	"QSQLITE"
#endif

#ifndef Server_DB_CONNECTION_NAME
#define Server_DB_CONNECTION_NAME "SERVER_DB_CONNECTION_NAME"
#endif


enum ServerType{ST_IM_SERVER, ST_FILE_SERVER, ST_ALL_IN_ONE};

#endif /* APP_CONSTANTS_SERVER_H_ */
