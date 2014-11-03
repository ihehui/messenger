/*
 * contactgroup.h
 *
 *  Created on: 2009-11-2
 *      Author: 贺辉
 */

#ifndef CONTACTGROUP_SERVER_H_
#define CONTACTGROUP_SERVER_H_

#include <QObject>
#include "userinfo.h"

#include "../../sharedim/contactgroupbase.h"

namespace HEHUI {

class ContactGroup :public ContactGroupBase{
    Q_OBJECT
public:
    ContactGroup(quint32 groupID, const QString &groupName, QObject *parent = 0);
    virtual ~ContactGroup();





private:


};

} //namespace HEHUI

#endif /* CONTACTGROUP_SERVER_H_ */
