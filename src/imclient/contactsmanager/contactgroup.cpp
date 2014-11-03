/*
 * contactgroup.cpp
 *
 *  Created on: 2009-11-2
 *      Author: 贺辉
 */

#include "contactgroup1.h"

namespace HEHUI {

ContactGroup::ContactGroup(quint32 groupID, const QString &groupName, QList<Contact*> contactList, QObject *parent)
    :ContactGroupBase(groupID, groupName), contactList(contactList)
{
    // TODO Auto-generated constructor stub

}

ContactGroup::~ContactGroup() {
    // TODO Auto-generated destructor stub
}



}
