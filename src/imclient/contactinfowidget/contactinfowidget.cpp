

#include <QDesktopWidget>
#include <QMessageBox>

#include "contactinfowidget.h"

#include "../imuser.h"

#include "../imageresource.h"




namespace HEHUI {

QHash<QString, ContactInfoWidget*> ContactInfoWidget::m_openWindows = QHash<QString, ContactInfoWidget*>();

ContactInfoWidget::ContactInfoWidget(IMUserBase *user, QWidget *parent, Qt::WindowFlags fl)
    : WidgetBase(parent, fl), m_user(user)
{
    ui.setupUi(this);

    Q_ASSERT_X(m_user, "ContactInfoWidget::ContactInfoWidget(...)", "Invalid IMUserBase!");

    m_openWindows.insert(m_user->getUserID(), this);


    initUI();

    on_pushButtonGeneralInformation_clicked();


    moveWindow(HEHUI::Center);

}

ContactInfoWidget::~ContactInfoWidget()
{

    //QMessageBox::information(this, "~ContactInfoWidget()", "~ContactInfoWidget()");

    m_openWindows.remove(m_user->getUserID());

}

void ContactInfoWidget::languageChange(){

    ui.retranslateUi(this);

    setWindowTitle(tr("%1's Information").arg(m_user->getNickName()));

}

void ContactInfoWidget::closeEvent(QCloseEvent *){
    //QMessageBox::information(this, "closeEvent()", "closeEvent()");



    deleteLater();

}

ContactInfoWidget * ContactInfoWidget::getContactInfoWidget(IMUserBase *user){
    Q_ASSERT_X(user, "ContactInfoWidget::getContactInfoWidget(...)", "Invalid IMUserBase!");

    return m_openWindows.value(user->getUserID());
}

void ContactInfoWidget::initUI(){
    QString userID = m_user->getUserID();

    ui.lineEditUserID->setText(userID);
    ui.lineEditNickName->setText(m_user->getNickName());
    ui.plainTextEditSignature->appendPlainText("");

    ui.comboBoxGender->setCurrentIndex(m_user->getGender());
    ui.spinBoxAge->setValue(m_user->getAge());

    QDate birthday = m_user->getBirthday();
    ui.spinBoxYear->setValue(birthday.year());
    ui.spinBoxMonth->setValue(birthday.month());
    ui.spinBoxDay->setValue(birthday.day());


    //TODO

    ui.pushButtonApply->setEnabled(false);


    if(userID == IMUser::instance()->getUserID()){
        setWindowTitle(tr("My Information"));


    }else{
        setWindowTitle(tr("%1's Information").arg(m_user->getNickName()));

        ui.lineEditNickName->setReadOnly(true);
        ui.plainTextEditSignature->setReadOnly(true);
        ui.comboBoxGender->setEnabled(false);
        ui.spinBoxAge->setReadOnly(true);
        ui.spinBoxYear->setReadOnly(true);
        ui.spinBoxMonth->setReadOnly(true);
        ui.spinBoxDay->setReadOnly(true);


        ui.toolButtonChangeFace->setEnabled(false);
        ui.pushButtonSettings->hide();
        ui.pushButtonApply->hide();


    }

    setWindowIcon(ImageResource::createIconForContact(m_user->getFace(), IM::ONLINESTATE_ONLINE));



}

void ContactInfoWidget::on_toolButtonUserFace_clicked(){
    //TODO

}

void ContactInfoWidget::on_pushButtonGeneralInformation_clicked(){
    ui.pushButtonGeneralInformation->setChecked(true);
    ui.pushButtonMoreInformation->setChecked(false);

    ui.stackedWidget->setCurrentWidget(ui.pageGeneralInfo);

}

void ContactInfoWidget::on_pushButtonMoreInformation_clicked(){
    ui.pushButtonGeneralInformation->setChecked(false);
    ui.pushButtonMoreInformation->setChecked(true);

    ui.stackedWidget->setCurrentWidget(ui.pageMoreInfo);

}

void ContactInfoWidget::on_pushButtonSettings_clicked(){
    //TODO

}

void ContactInfoWidget::on_pushButtonUpdate_clicked(){
    //TODO

    emit updateUserInfoRequested(m_user);

    initUI();

}

void ContactInfoWidget::on_pushButtonOK_clicked(){
    //TODO

    emit saveUserInfoRequested(m_user);

    close();

}

void ContactInfoWidget::on_pushButtonCancel_clicked(){
    close();
}

void ContactInfoWidget::on_pushButtonApply_clicked(){
    //TODO

    emit saveUserInfoRequested(m_user);

    ui.pushButtonApply->setEnabled(false);



}

void ContactInfoWidget::on_lineEditNickName_editingFinished(){
    QString newNickName = ui.lineEditNickName->text();
    if(newNickName != m_user->getNickName()){
        m_user->setNickName(newNickName);
        m_user->addUpdatedPersonalInfoProperty(IM::PI_NickName, newNickName, true);
    }

}

void ContactInfoWidget::on_plainTextEditSignature_textChanged(){

}
























}
