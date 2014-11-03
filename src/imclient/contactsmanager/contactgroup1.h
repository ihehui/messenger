/*
 * contactgroup.h
 *
 *  Created on: 2009-11-2
 *      Author: 贺辉
 */

#ifndef CONTACTGROUP_H_
#define CONTACTGROUP_H_

#include <QObject>
#include "contact.h"

#include "../../sharedim/contactgroupbase.h"

namespace HEHUI {

class ContactGroup :public ContactGroupBase{
    Q_OBJECT
public:
    ContactGroup(quint32 groupID, const QString &groupName, QList<Contact*> contactList, QObject *parent = 0);
    virtual ~ContactGroup();


    QList<Contact*> getContactList() const
    {
        return contactList;
    }

    void setContactList(QList<Contact*> contactList)
    {
        this->contactList = contactList;
    }


//    int getParentGroupID() const
//    {
//        return parentGroupID;
//    }

//    void setParentGroupID(int parentGroupID)
//    {
//        this->parentGroupID = parentGroupID;
//    }


    void addContact(Contact *contact){
        contactList.append(contact);
    }
    void deleteContact(Contact *contact){
        contactList.removeAll(contact);
    }


private:
//    int parentGroupID;

    QList<Contact*> contactList;

};

} //namespace HEHUI

#endif /* CONTACTGROUP_H_ */
