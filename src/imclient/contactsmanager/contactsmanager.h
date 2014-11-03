/*
 * contactsmanager.h
 *
 *  Created on: 2009-9-12
 *      Author: 贺辉
 */

#ifndef CONTACTSMANAGER_H_
#define CONTACTSMANAGER_H_

#include <QObject>
#include <QtSql>
#include <QHash>

#include "contact.h"
#include "imuser.h"
#include "interestgroup.h"
//#include "contactgroup.h"
#include "../../sharedim/contactgroupbase.h"




//#include "../../../shared/core/global_core.h"
//#include "../../../shared/core/singleton.h"
//#include "../../shared/gui/widgetbase/expandlistview/expandlistview.h"
#include "HHSharedCore/hglobal_core.h"
#include "HHSharedCore/hsingleton.h"
#include "HHSharedGUI/ItemBox"


namespace HEHUI {

class ContactsManager : public QObject, public Singleton<ContactsManager>{
    Q_OBJECT
    friend class Singleton<ContactsManager>;

public:
    typedef ItemBoxWidget::Item ObjectItem;
    typedef ItemBoxWidget:: ItemList ObjectItemList;
    typedef ItemBoxWidget::Category Category;
    typedef ItemBoxWidget::CategoryList CategoryList;

    ContactsManager(QObject *parent = 0);
    virtual ~ContactsManager();



    bool isFriendContact(const QString &contactID);
    bool isStranger(const QString &contactID);
    bool hasUserInfo(const QString &userID);
    Contact * getUser(const QString &contactID);
    QHash<QString/*Contact ID*/, Contact*> getAllUsers();


    bool loadInterestGroups();
    InterestGroup * getInterestGroup(quint32 groupID);
    bool addNewInterestGroupToDatabase(InterestGroup *interestGroup);
    bool removeInterestGroupFromLocalDB(quint32 groupID);
    bool saveInterestGroupInfoToDatabase(InterestGroup *interestGroup);
    QList<InterestGroup *> getInterestGroupsList();
    QList<quint32 > getInterestGroupIDsList();

//    bool isMemberOfInterestGroup(quint32 groupID);

    bool saveInterestGroupMemberToDatabase(quint32 groupID, const QString &userID, quint32 memberRole);
    bool saveInterestGroupMembersToDatabase(InterestGroup *interestGroup);
    bool fetchAllInterestGroupMembers(InterestGroup *interestGroup);

    bool saveContactChatMessageToDatabase(const QString &senderID, const QString &receiverID, const QString &message, const QString &time = "");
    bool saveInterestGroupChatMessageToDatabase(const QString &senderID, quint32 interestGroupID, const QString &message, const QString &time = "");


    Contact * createNewContact(const QString &contactID, int groupID = ContactGroupBase::Group_Strangers_ID, const QString &nickname = "", const QString &face = "");
    int onlineContactGroupMembersCount(int contactGroupID);

public slots:
    //void slotUpdateContactsInfo2();
    void slotFetchAllContactsInfo2(ItemBoxWidget *expandListView);
    void slotFetchAllContactsInfoFromDB();
    void slotFetchRecentChatsFromDB(QStringList *contacts, QList<quint32> *interestGroups);


    void slotAddNewContactGroupToUI(ItemBoxWidget *expandListView, int personalContactGroupID, const QString &groupName);
    void slotDeleteContactGroupFromUI(ItemBoxWidget *expandListView, int contactGroupID);

    void addContactToUI(ItemBoxWidget *expandListView, int groupID, const QString &contactID);
    void deleteContactFromUI(ItemBoxWidget *expandListView, quint32 groupID, const QString &contactID);
    void moveContactToUI(ItemBoxWidget *expandListView, quint32 old_groupID, quint32 new_groupID, const QString &contactID);
    void updateContactToUI(ItemBoxWidget *expandListView, int personalContactGroupID, const QString &contactID);

    void renameContactGroupToUI(ItemBoxWidget *expandListView, quint32 groupID, const QString &new_groupName);


//    bool addContact(const QString &contactID, quint32 groupID);
    bool deleteContact(const QString &contactID, bool addToBlacklist = false);
    bool moveContact(const QString &contactID, quint32 oldGroupID, quint32 newGroupID);


    //联系人在线状态改变
    void slotChangeContactOnlineState(const QString &contactID, quint8 onlineStateCode, const QString &peerAddress, quint16 peerPort, const QString &greetingInfo);


//    QString getPersonalContactGroupName(int personalContactGroupID) ;
    bool slotAddNewContactGroupToDatabase(quint32 groupID, const QString &groupName);
    bool renameContactGroupToDatabase(quint32 groupID, const QString &new_groupName);
    bool deleteContactGroupFromDatabase(int groupID);
    bool resetAllContactGroupInDatabase();



    bool getMyInfoFormLocalDatabase();
    bool saveMyInfoToDatabase();

    bool setContactInfoString(const QString &contactID, const QString &contactInfoString, bool summaryInfo);
    bool getContactInfoFormLocalDatabase(const QString &contactID);
    bool saveContactInfoToDatabase(const QString &contactID);

//    bool saveContactGroupsInfoToDatabase();

    bool getContactHistoryMessage(const QString &startTime, const QString &endTime, const QString &content, bool requestBackword, const QString &contactID, QStringList *messages, bool *canFetchMore);
    bool getGrouptHistoryMessage(const QString &startTime, const QString &endTime, const QString &content, bool requestBackword, quint32 groupID, QStringList *messages, bool *canFetchMore);


    bool openDatabase(bool reopen = false);
    bool initLocalDatabase(QString *errorMessage = 0);

    bool slotAddNewContactToDatabase(Contact *contact);
    bool slotdeleteContactFromDatabase(Contact *contact);

    void slotLoadContactGroupToUI(ItemBoxWidget *expandListView, int groupID, const QString groupName, QList<Contact*> contactList);

private slots:
    //void slotSetupContacts(bool loadFromFile, const QString &string);
    //void slotLoadContacts(ItemBoxWidget *widgetBox, bool loadFromFile = true, const QString &string = "");
    void addNewContactGroupToUI(ItemBoxWidget *expandListView, Category category);

    //void slotLoadContacts(ItemBoxWidget *expandListView,ContactGroup *contactGroup);




private:
    QSqlQuery queryDatabase(const QString & queryString, bool localConfigDatabase) ;

    QSqlQuery queryDatabase(const QString & queryString, const QString &connectionName, const QString &driver,
                            const QString &host, int port, const QString &user, const QString &passwd,
                            const QString &databaseName, HEHUI::DatabaseType databaseType) ;

private:


    //QSqlQuery *query;
    QString userPrivateDataFilePath;
    QSqlDatabase localUserDataDB;

    QHash<QString/*Contact ID*/, Contact*> contactHash;

    QHash<quint32/*Grup ID*/, InterestGroup*> interestGroupsHash;

    IMUser *m_myself;




};

} //namespace HEHUI

#endif /* CONTACTSMANAGER_H_ */
