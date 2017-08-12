#include "updatepasswordwidget.h"
#include "ui_updatepasswordwidget.h"

#include <QMessageBox>

namespace HEHUI
{



UpdatePasswordWidget::UpdatePasswordWidget(IMClientPacketsParser *clientPacketsParser, QWidget *parent)
    :QWidget(parent),
    ui(new Ui::UpdatePasswordWidgetUI),
    m_clientPacketsParser(clientPacketsParser)
{
    ui->setupUi(this);

    ui->stackedWidgetMain->setCurrentWidget(ui->pageInfo);
    ui->frameBottom->hide();

    Q_ASSERT(clientPacketsParser);
    connect(m_clientPacketsParser, SIGNAL(signalUpdatePasswordResultReceived(const UpdatePasswordPacket &)), this, SLOT(slotProcessUpdatePasswordResult(const UpdatePasswordPacket &)), Qt::QueuedConnection);

    m_clientPacketsParser->requestUpdatePassword(m_clientPacketsParser->socketConnectedToServer());

    m_authMode = UpdatePasswordPacket::AUTH_OLD_PWD_ONLY;


    connect(ui->pushButtonOK, SIGNAL(clicked(bool)), this, SLOT(slotOkClicked()));

}

UpdatePasswordWidget::~UpdatePasswordWidget()
{
    delete ui;
}

void UpdatePasswordWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void UpdatePasswordWidget::slotProcessUpdatePasswordResult(const UpdatePasswordPacket &packet)
{
    ui->frameBottom->show();

    UpdatePasswordPacket::PacketInfoType infoType = UpdatePasswordPacket::PacketInfoType(packet.getPacketSubType());
    switch (infoType) {
    case UpdatePasswordPacket::INFO_TYPE_UPDATE_MODE_INFO:
    {
        ui->stackedWidgetMain->setCurrentWidget(ui->pageVerify);
        m_authMode = UpdatePasswordPacket::AuthMode(packet.UpdateModeInfo.authMode);
        switch (m_authMode) {
        case UpdatePasswordPacket::AUTH_OLD_PWD_ONLY:
        {
            ui->labelQuestion->hide();
            ui->labelQuestion1->hide();
            ui->labelAnswer->hide();
            ui->lineEditAnswer->hide();
            ui->labelCode->hide();
            ui->lineEditAuthCode->hide();
            ui->pushButtonRequestAuthCode->hide();
            ui->labelCaptcha->hide();
            ui->lineEditCaptcha->hide();
            ui->labelCaptchaImage->hide();


        }
            break;
        case UpdatePasswordPacket::AUTH_SECURITY_QUESTION: {
            ui->stackedWidgetMain->setCurrentWidget(ui->pageVerify);

        }
        break;
        case UpdatePasswordPacket::AUTH_EMAIL: {

        }
        break;
        case UpdatePasswordPacket::AUTH_SMS: {

        }
        break;
        case UpdatePasswordPacket::AUTH_HTTP: {

        }
        break;
        default:
            break;
        }

    }
        break;

    case UpdatePasswordPacket::INFO_TYPE_AUTH_INFO_FROM_SERVER:
    {
        ui->labelQuestion->setText(packet.AuthInfoFromServer.securityQuestion);

        //out << AuthInfoFromServer.authMode << AuthInfoFromServer.securityQuestion << AuthInfoFromServer.url;
    }
        break;

    case UpdatePasswordPacket::INFO_TYPE_AUTH_INFO_FROM_CLIENT:
    {
    }
        break;

//    case UpdatePasswordPacket::INFO_TYPE_AUTH_RESULT:
//    {
//        if(packet.AuthResult.result){
//            ui->stackedWidgetMain->setCurrentWidget(ui->pageNewPassword);
//            ui->pushButtonOK->setText(tr("Update"));
//        }else{
//            ui->labelInfo->setText(tr("Authentication Failed! Code:%1").arg(packet.AuthResult.errorCode));
//            ui->stackedWidgetMain->setCurrentWidget(ui->pageInfo);

//        }

//    }
//        break;

    case UpdatePasswordPacket::INFO_TYPE_UPDATE_RESULT:
    {
        QString msg;
        if(packet.UpdateResult.result){
            msg = tr("Password has been updated!");
        }else{
            msg = tr("Failed to update password! Code:%1").arg(packet.UpdateResult.errorCode);
        }

        ui->labelInfo->setText(msg);
        ui->pushButtonOK->setEnabled(true);
        ui->stackedWidgetMain->setCurrentWidget(ui->pageInfo);
    }
        break;

    default:
        break;
    }



}

void UpdatePasswordWidget::slotOkClicked()
{

    if(ui->stackedWidgetMain->currentWidget() == ui->pageVerify){

        if(ui->lineEditNewPassword->text().trimmed().isEmpty()){
            QMessageBox::critical(this, tr("Error"), tr("Invalid Password!"));
            ui->lineEditNewPassword->setFocus();
            return;
        }

        if(ui->lineEditNewPassword->text().trimmed() != ui->lineEditNewPasswordAgain->text().trimmed()){
            QMessageBox::critical(this, tr("Error"), tr("The new password and confirmation do not match.\n Please re-enter your new password!"));
            ui->lineEditNewPassword->setFocus();
            return;
        }


        ui->pushButtonOK->setEnabled(false);

        QString oldPwd = User::hashHexString(ui->lineEditCurrentPassword->text().trimmed());
        QString newPwd = User::hashHexString(ui->lineEditNewPassword->text().trimmed());
        m_clientPacketsParser->updatePassword(ui->lineEditCaptcha->text().trimmed(), oldPwd, newPwd, ui->lineEditAnswer->text(), ui->lineEditAuthCode->text().trimmed());
        ui->labelInfo->setText("Waiting for result...");
        ui->stackedWidgetMain->setCurrentWidget(ui->pageInfo);
    }else{
        emit closed();
    }

}






} //namespace HEHUI
