
#include "deletecontactdialog.h"

#include "../imageresource.h"



namespace HEHUI {

DeleteContactDialog::DeleteContactDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);

    m_contact = 0;
    m_blacklistMode = false;

    m_resultReceivedFromServer = false;


}

void DeleteContactDialog::setContact(Contact *contact, bool blacklistMode){


    Q_ASSERT(contact);
    this->m_contact = contact;
    m_blacklistMode = blacklistMode;

    if(m_blacklistMode){
        ui.groupBoxUserInfo->setTitle(tr("Are you sure you want to move this contact to your blacklist?"));
        ui.checkBox->hide();
    }else{
        ui.groupBoxUserInfo->setTitle(tr("Are you sure you want to delete the contact below?"));
        ui.checkBox->show();
        ui.checkBox->setChecked(false);
    }

    ui.toolButtonUserFace->setIcon(ImageResource::createIconForContact(m_contact->getFace(), m_contact->getOnlineState()));
    ui.labelUserNickName->setText(m_contact->getNickName());
    ui.labelUserID->setText(m_contact->getUserID());

    ui.pushButtonOK->setEnabled(true);
    ui.pushButtonCancel->setEnabled(true);

}

void DeleteContactDialog::deleteContactResultReceivedFromServer(const QString &contactID, bool contactDeleted){
    if(contactID != m_contact->getUserID()){return;}

    m_resultReceivedFromServer = true;
    if(contactDeleted){
        accept();
    }else{
        ui.pushButtonOK->setEnabled(true);
        ui.pushButtonCancel->setEnabled(true);
        ui.groupBoxUserInfo->setTitle(tr("Delete contact failed!"));
    }
}

void DeleteContactDialog::on_toolButtonUserFace_clicked(){
    emit signalShowContactDetailInfo(m_contact->getUserID());
}

void DeleteContactDialog::on_pushButtonOK_clicked(){
    ui.pushButtonOK->setEnabled(false);
    ui.pushButtonCancel->setEnabled(false);
    emit signalRequestDeleteContact(m_contact->getUserID(), ui.checkBox->isChecked(), m_blacklistMode);

    QTimer::singleShot(5000, this, SLOT(timeout()));
}

void DeleteContactDialog::on_pushButtonCancel_clicked(){
    m_resultReceivedFromServer = true;
    accept();
}

void DeleteContactDialog::timeout(){
    if(m_resultReceivedFromServer){
       return;
    }

    ui.pushButtonOK->setEnabled(true);
    ui.pushButtonCancel->setEnabled(true);
    ui.groupBoxUserInfo->setTitle(tr("Delete contact timeout!"));


}



void DeleteContactDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui.retranslateUi(this);
        break;
    default:
        break;
    }
}








} //namespace HEHUI

