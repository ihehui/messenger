#ifndef SEARCH_H
#define SEARCH_H

#include <QDialog>
#include <QTimer>


#include "ui_search.h"

#include "userinfomodel.h"
#include "interestgroupinfomodel.h"
#include "../contactsmanager/contact.h"




namespace HEHUI
{

class Search : public QDialog
{
    Q_OBJECT

public:
//    static Search * instance();
    ~Search();

//private:
    Search(QWidget *parent = 0);

signals:
    void signalSearchContact(const QString &keyword, quint8 searchOnlineUsersOnly,
                             quint8 searchWebcamUsersOnly, quint16 location, quint16 hometown,
                             quint8 gender, quint8 age, bool matchExactly, int startIndex);

    void signalAddContact(const QString &userID, const QString &verificationMessage);
    void signalSearchInterestGroup(const QString &keyword, int startIndex = 0);
    void signalJoinInterestGroup(quint32 groupID, const QString &verificationMessage);


public slots:
    void slotSearchContactsResultPacketReceived(const QString &usersString);
    void slotSearchInterestGroupsResultPacketReceived(const QString &groupsString);


private slots:

    void reset();

    void slotUserSelected(const QModelIndex &index);
    void slotAddUserAsAContact(const QModelIndex &index);

    void slotGroupSelected(const QModelIndex &index);
    void slotJoinGroup(const QModelIndex &index);


    void on_tabWidget_currentChanged( int index );

    void on_pushButtonSearchContact_clicked();
    void searchContact(int pageIndex);

    void on_pushButtonUserDetails_clicked();
    void on_pushButtonAddAsContact_clicked();

    void on_toolButtonPreviousUsersResultPage_clicked();
    void on_toolButtonNextUsersResultPage_clicked();


    void on_pushButtonSearchGroup_clicked();

    void on_pushButtonGroupDetails_clicked();
    void on_pushButtonJoinGroup_clicked();

    void on_toolButtonPreviousGroupsResultPage_clicked();
    void on_toolButtonNextGroupsResultPage_clicked();


    void searchContactTimeout();
    void searchGroupTimeout();

private:
    Ui::SearchClass ui;


    bool matchExactly, searchOnlineUsersOnly, searchWebcamUsersOnly;

    QString groupKeyword;


    UserInfoModel *userInfoModel;

    InterestGroupInfoModel *groupInfoModel;


    QTimer searchContactTimer;
    QTimer searchGroupTimer;

    int usersResultPageIndex;
    int groupsResultPageIndex;






};

} //namespace HEHUI

#endif // SEARCH_H
