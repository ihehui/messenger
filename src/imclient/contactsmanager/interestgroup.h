/*
 * groupfromserver.h
 *
 *  Created on: 2011-2-28
 *      Author: hehui
 */

#ifndef INTERESTGROUP_H_
#define INTERESTGROUP_H_


#include <QObject>
#include <QDateTime>
#include <QStringList>
#include <QMap>

#include "../../sharedim/interestgroupbase.h"
#include "contact.h"


namespace HEHUI {

class InterestGroup : public InterestGroupBase {
    Q_OBJECT
public:
    InterestGroup(quint32 groupID = 0, const QString &groupName = "", QObject *parent = 0);
    virtual ~InterestGroup();


    QString databaseColumnName(IM::PropertyIDOfGroup propertyID) const;


    struct GroupChatMessage{
        Contact *contact;
        QString message;
        QString time;
    };
    void appandUnreadMessage(Contact *contact, const QString &message, const QString &time);
    QList<GroupChatMessage> takeUnreadMessages();
    int unreadMessagesCount();


protected:

private:

    QList<GroupChatMessage> unreadMessages;


};

}

#endif /* INTERESTGROUP_H_ */
