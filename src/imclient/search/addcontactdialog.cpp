


#include "addcontactdialog.h"

#include "../imageresource.h"


namespace HEHUI{

AddContactDialog::AddContactDialog(Contact *user, bool requestToAddNewContact, QWidget *parent) :
    QDialog(parent), m_contact(user)
{

    initUI();
    
    m_requestToAddContact = true;
            
    if(requestToAddNewContact){
        ui.groupBoxUserInfo->setTitle(tr("The following user will be added."));
        ui.pushButtonReject->setText(tr("&Cancel"));
        if(m_contact->getFriendshipApply() == Contact::FA_AUTO_ACCEPT){
            ui.groupBoxSettings->show();
        }else{
            ui.groupBoxMessage->setTitle(tr("Your Request"));
            ui.groupBoxMessage->show();
            ui.lineEditMessage->setReadOnly(false);
            ui.lineEditMessage->setFocus();
        }
    
    }else{
        ui.groupBoxUserInfo->setTitle(tr("The following user has been added you as a contact."));
        ui.pushButtonReject->setEnabled(false);
        ui.pushButtonReject->hide();
        ui.pushButtonAddAsAContact->setText(tr("&OK"));


        ui.groupBoxSettings->show();
        ui.lineEditRename->setText(user->getRemarkName());
        ui.lineEditRename->setFocus();
    }

    
}

AddContactDialog::AddContactDialog(Contact *user, const QString &requestMessage, QWidget *parent)
    :QDialog(parent), m_contact(user)
{
    
    initUI();
    
    m_requestToAddContact = false;
    
    ui.groupBoxUserInfo->setTitle(tr("The following user requested to add you as a contact."));

    if(!requestMessage.isEmpty()){
        ui.groupBoxMessage->setTitle(tr("Request Message"));
        ui.groupBoxMessage->show();
        ui.lineEditMessage->setText(requestMessage);
        ui.lineEditMessage->setReadOnly(true);
    }


    ui.pushButtonReject->setText(tr("&Reject"));
    ui.pushButtonReject->setEnabled(true);
    ui.pushButtonReject->show();

    ui.pushButtonAddAsAContact->setText(tr("&Accept"));
//    ui.pushButtonAddAsAContact->setFocus();

    initContactGroupCombox();
    //ui.lineEditRename->setFocus();

}

void AddContactDialog::closeEvent(QCloseEvent *event){

    ui.lineEditMessage->clear();


}

void AddContactDialog::changeEvent(QEvent *e)
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


QString AddContactDialog::getMessage() const{
    return ui.lineEditMessage->text();
}

QString AddContactDialog::getNewName() const{
    return ui.lineEditRename->text();
}

//QString AddContactDialog::getGroupname(){
//    QString groupName = ui.comboBoxGroup->currentText();
////    if(groupName.trimmed().isEmpty()){
////        groupName = m_user->getDefaultGroupName();
////    }
//    return groupName;
//}

quint32 AddContactDialog::getGroupID(){
    return ui.comboBoxGroup->itemData(ui.comboBoxGroup->currentIndex()).toUInt();
}

bool AddContactDialog::requestRejected(){
    return m_rejectRequest;
}

void AddContactDialog::on_pushButtonAddAsAContact_clicked(){
    m_rejectRequest = false;

    accept();
    return;

    if(ui.groupBoxSettings->isVisible() || m_requestToAddContact){
        accept();
    }else{
        ui.groupBoxMessage->hide();
        ui.groupBoxSettings->show();
        ui.pushButtonReject->setEnabled(false);
        ui.pushButtonReject->hide();
    }

}

void AddContactDialog::on_pushButtonReject_clicked(){
    m_rejectRequest = true;


    if(m_requestToAddContact || (!ui.lineEditMessage->isReadOnly()) ){
        accept();
    }else{
        ui.groupBoxMessage->setTitle(tr("Reply Message"));
        ui.groupBoxMessage->show();
        ui.lineEditMessage->setReadOnly(false);
        ui.lineEditMessage->clear();
        ui.lineEditMessage->setFocus();

        ui.pushButtonAddAsAContact->setEnabled(false);
        ui.pushButtonAddAsAContact->hide();

    }

}

inline void AddContactDialog::initUI(){


    ui.setupUi(this);


    m_imUser = IMUser::instance();

    m_requestToAddContact = false;
    m_rejectRequest = false;

    Q_ASSERT_X(m_contact, "AddContactDialog", "Invalid user object");

    ui.toolButtonUserFace->setIcon(ImageResource::createIconForContact(m_contact->getFace(), m_contact->getOnlineState()));
    ui.labelUserNickName->setText(m_contact->getNickName());
    ui.labelUserID->setText(m_contact->getUserID());

    ui.groupBoxMessage->hide();
    ui.lineEditMessage->setReadOnly(true);
    ui.groupBoxSettings->hide();

    initContactGroupCombox();


    validator = new QRegExpValidator(this);
    QRegExp rx("\\b\\w{0,16}\\b");
    validator->setRegExp(rx);
    ui.lineEditMessage->setValidator(validator);
    ui.lineEditRename->setValidator(validator);
    ui.comboBoxGroup->setValidator(validator);

}

inline void AddContactDialog::initContactGroupCombox(){

    ui.comboBoxGroup->clear();

    QList<ContactGroupBase *> groups = m_imUser->getContactGroups();
    //groups.removeAll(m_imUser->getContactGroup(ContactGroupBase::Group_Friends_ID));
    foreach (ContactGroupBase *group, groups) {
        ui.comboBoxGroup->addItem(group->getGroupName(), QVariant(group->getGroupID()));
    }
    //ui.comboBoxGroup->insertItem(0, ContactGroupBase::Group_Friends_Name, QVariant(ContactGroupBase::Group_Friends_ID));
    ui.comboBoxGroup->setCurrentIndex(0);


}

















} //namespace HEHUI

