#ifndef DELETECONTACTDIALOG_H
#define DELETECONTACTDIALOG_H

#include "ui_deletecontactdialog.h"

#include "../contactsmanager/contact.h"

namespace HEHUI {


class DeleteContactDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DeleteContactDialog(QWidget *parent = 0);

    void setContact(Contact *contact, bool blacklistMode = false);

signals:
    void signalRequestDeleteContact(const QString &contactID, bool deleteMeFromOpposition = false, bool addToBlacklist = false);
    void signalShowContactDetailInfo(const QString &contactID);

public slots:
    void deleteContactResultReceivedFromServer(const QString &contactID, bool contactDeleted);
    

private slots:
    void on_toolButtonUserFace_clicked();
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();

    void timeout();

protected:
    void changeEvent(QEvent *e);


private:
    Ui::DeleteContactDialog ui;

    Contact *m_contact;
    bool m_blacklistMode;

    bool m_resultReceivedFromServer;





};

} //namespace HEHUI

#endif // DELETECONTACTDIALOG_H
