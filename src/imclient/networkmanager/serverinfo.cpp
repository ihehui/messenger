/*
 ****************************************************************************
 * serverinfo.cpp
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

#include "serverinfo.h"

namespace HEHUI {

ServerInfo::ServerInfo(const QString &ip, uint port, QObject *parent)
	:QObject(parent)
{

	setIp(ip);
	setPort(port);
	setCurState(NotTested);

}

ServerInfo::~ServerInfo() {
	// TODO Auto-generated destructor stub
}

}

