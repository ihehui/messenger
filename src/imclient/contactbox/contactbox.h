/*
 ****************************************************************************
 * filename
 *
 * Created on: 2013-10-28
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
 * Last Modified on: 2013-10-28
 * Last Modified by: 贺辉
 ***************************************************************************
 */





#ifndef CONTACTBOX_H
#define CONTACTBOX_H

#include <QTreeWidget>
#include <QContextMenuEvent>




#include "contactwidget.h"
#include "../contactsmanager/contact.h"
#include "../contactsmanager/contactsmanager.h"
#include "../informationtips/userinfotipwindow.h"
#include "../packetmanager/imclientpacketsparser.h"


namespace HEHUI {



class ContactBox : public QTreeWidget
{
    Q_OBJECT
public:
    explicit ContactBox(QWidget *parent = 0);
    ~ContactBox();

    void loadAllContacts();

    void addOrRemoveContactItem(Contact *contact, bool add);
    void updateContactItemInfo(Contact *contact);

    void addOrRemoveContactGroupItem(ContactGroupBase *contactGroup, bool add);
    void insertContactGroupItem(ContactGroupBase *contactGroup);
    void updateContactGroupItemInfo(ContactGroupBase *contactGroup);
    void updateContactGroupItemInfo(QTreeWidgetItem *contactGroupItem);

    void moveContact(Contact *contact, ContactGroupBase *oldContactGroup, ContactGroupBase *newContactGroup);
    void moveContact2(Contact *contact, ContactGroupBase *oldContactGroup, ContactGroupBase *newContactGroup);

    void setContactGroupItemExpanded(ContactGroupBase *contactGroup, bool expanded);
    void setContactGroupItemHidden(ContactGroupBase *contactGroup, bool hide);
    void setContactItemHidden(Contact *contact, bool hide);


    void chatMessageReceivedFromContact(Contact *contact);
    void chatMessageFromContactRead(Contact *contact);


protected:
    bool event(QEvent *event);
//    void contextMenuEvent(QContextMenuEvent *e);
//    void resizeEvent(QResizeEvent *e);


signals:
    void signalContextMenuEventOnContactGroup(ContactGroupBase *group, const QPoint &global_mouse_pos);
    void signalContextMenuEventOnContact(Contact *contact, const QPoint &global_mouse_pos);
    void signalContactItemActivated(Contact *contact);
    void signalTooltipEventOnContact(Contact *contact, const QPoint &global_item_topLeft_pos, const QPoint &global_mouse_pos);


public slots:



private slots:
    void handleMousePress(QTreeWidgetItem* item);
    void handleMouseDoubleClick(QTreeWidgetItem* item);
    void handleCurrentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous);


    void handleContextMenuEventOnContactGroup(QTreeWidgetItem* item, const QPoint &global_mouse_pos);
    void handleContextMenuEventOnContact(QTreeWidgetItem* item, const QPoint &global_mouse_pos);

//    void handleTooltipEventOnContact(QTreeWidgetItem* item, const QPoint &global_item_topLeft_pos, const QPoint &global_mouse_pos);

    void startFlashTimer();
    void flash();

private:

    ContactsManager *m_contactsManager;
    IMUser *m_myself;

    UserInfoTipWindow *m_userInfoTipWindow;

    QHash<Contact*, QTreeWidgetItem*> contactsHash;
    QHash<ContactGroupBase*, QTreeWidgetItem*> contactGroupsHash;

    QTimer *flashTimer;
    int flashIndex;
    QHash<Contact *, QTreeWidgetItem*/*ContactGroup Item*/> flashContactItems;

};

} //namespace HEHUI

#endif // CONTACTBOX_H
