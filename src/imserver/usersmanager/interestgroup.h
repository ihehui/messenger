/*
 * Group.h
 *
 *  Created on: 2011-2-28
 *      Author: hehui
 */

#ifndef INTERESTGROUP_H_
#define INTERESTGROUP_H_


#include <QObject>
#include <QDateTime>
#include <QStringList>
#include <QHash>

#include "../../sharedim/interestgroupbase.h"


namespace HEHUI {

class InterestGroup : public InterestGroupBase {
    Q_OBJECT
public:
    InterestGroup(quint32 groupID = 0, QObject *parent = 0);
    virtual ~InterestGroup();


    QString databaseColumnName(IM::PropertyIDOfGroup propertyID) const;


protected:

private:
    










};

}

#endif /* INTERESTGROUP_H_ */
