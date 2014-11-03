#ifndef CONTACTGROUPBASE_H
#define CONTACTGROUPBASE_H


#include <QString>

#include "sharedimlib.h"
#include "imgroupbase.h"


namespace HEHUI {



class SHAREDIMLIB_API ContactGroupBase : public IMGroupBase
{
public:
    enum DefaultContactGroupID{
        Group_Strangers_ID = -1,
        Group_Blacklist_ID = 0,
        Group_Friends_ID = 1
    };

    static QString Group_Blacklist_Name;
    static QString Group_Friends_Name;
    static QString Group_Strangers_Name;

    ContactGroupBase(quint32 groupID = 0, const QString &groupName = "", QObject *parent = 0);

    static bool isUserCreatedGroup(int groupID);


//    bool setGroupInfoFromString(const QString &infoString, const QString &fieldSepartor);
//    QString getGroupInfoAsString(const QString &fieldSepartor);

    void setMembersFromString(const QString &infoString, const QString &fieldSepartor);
    QString getMembersAsString(const QString &fieldSepartor);
//    QStringList getMembers();
    void addMember(const QString &memberuserID);
    void deleteMember(const QString &memberuserID);
    bool hasMember(const QString &memberUserID);

    QStringList members() const;
    void setMembers(const QStringList &members);
    int countOfMembers();





private:


    QStringList /*Member's ID*/ membersList;

    QHash<IM::PropertyIDOfGroup/*Property ID*/, QString/*SQL Update Statement*/> updatedProperties;
//    QMutex *updatedPropertiesMutex;

};

} //namespace HEHUI

#endif // CONTACTGROUPBASE_H
