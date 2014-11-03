/*
 ****************************************************************************
 * applyforregistrationwidget.cpp
 *
 * Created on: 2010-5-29
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
 * Last Modified on: 2010-5-29
 * Last Modified by: 贺辉
 ***************************************************************************
 */


#include <QCryptographicHash>
#include <QMessageBox>
#include <QTimer>
#include <QDesktopServices>

#include <QDebug>

#include "applyforregistrationwidget.h"





namespace HEHUI {


ApplyForRegistrationWidget::ApplyForRegistrationWidget(QWidget *parent) :
    QWidget(parent)//, ui(new Ui::ApplyForRegistrationWidget)
{
    ui.setupUi(this);
    
//    validator = new QRegExpValidator(this);
//    QRegExp rx("\\w{6,16}");
//    validator->setRegExp(rx);
//    ui.lineEditUserID->setValidator(validator);

    ui.pushButtonPrevious->hide();
    ui.pushButtonNext->hide();
    ui.pushButtonRegister->setText(tr("&Cancel"));

    user = IMUser::instance();
        
    m_registrationModeInfoResponseReceived = false;
    m_registrationResultReceived = false;

    m_registrationMode = IM::RM_ServerCreateUserIDOnly;
    m_requireActivation = false;

    
    //emit requestRegistrationServerInfo();
    QTimer::singleShot(5000, this, SLOT(requestRegistrationServerInfoTimeout()));

}

ApplyForRegistrationWidget::~ApplyForRegistrationWidget()
{
    qDebug()<<"--ApplyForRegistrationWidget::~ApplyForRegistrationWidget()";

}

void ApplyForRegistrationWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui.retranslateUi(this);
        break;
    default:
        break;
    }
}

void ApplyForRegistrationWidget::slotProcessRegistrationServerInfo(quint8 errorTypeCode, bool canRegister, const QString &extraMessage, quint8 regMode, const QString &regServerAddress, bool requireActivation){
    qDebug()<<"--ApplyForRegistrationWidget::slotProcessRegistrationServerInfo(...)";

    m_registrationModeInfoResponseReceived = true;

    if(!canRegister){
        QString msg = tr("<p><font color='red'>Request Denied By Server!</font></p>%1").arg(extraMessage);
        ui.labelInfo->setText(msg);
        return;
    }


    if(regServerAddress.startsWith("http://", Qt::CaseInsensitive) || regServerAddress.startsWith("https://", Qt::CaseInsensitive)){
        QDesktopServices::openUrl(regServerAddress);
        emit canceled();
        return;
    }


    m_registrationMode = IM::RegistrationMode(regMode);
    user->setRegistrationServerAddressInfo(regServerAddress);
    m_requireActivation = requireActivation;

    switch(m_registrationMode){
    case IM::RM_UserDefineAll:
        break;
    case IM::RM_ServerCreateAll:
        ui.labelUserID->hide();
        ui.lineEditUserID->hide();
        ui.labelUserIDInfo->hide();
        ui.labelPassword->hide();
        ui.lineEditPassword->hide();
        ui.labelPasswordInfo->hide();
        ui.labelRetypePassword->hide();
        ui.lineEditRetypePassword->hide();
        ui.labelRetypedPasswordInfo->hide();
        break;
    case IM::RM_ServerCreateUserIDOnly:
        ui.labelUserID->hide();
        ui.lineEditUserID->hide();
        ui.labelUserIDInfo->hide();
        break;
    case IM::RM_ServerCreatePasswordOnly:
        ui.labelPassword->hide();
        ui.lineEditPassword->hide();
        ui.labelPasswordInfo->hide();
        ui.labelRetypePassword->hide();
        ui.lineEditRetypePassword->hide();
        ui.labelRetypedPasswordInfo->hide();
        break;
    default:
        break;

    }

    ui.stackedWidget->setCurrentWidget(ui.pageRequiredInfo);
    ui.pushButtonNext->show();
    ui.pushButtonRegister->setText(tr("&Register"));

}

void ApplyForRegistrationWidget::slotProcessRegistrationResult(quint8 errorTypeCode, quint32 sysID, const QString &message){
    qDebug()<<"--ApplyForRegistrationWidget::slotProcessRegistrationResult(...)";

    m_registrationResultReceived = true;
    
    IM::ErrorType errorType = IM::ErrorType(errorTypeCode);
    QString errorMessage = "";
    
    if(errorType == IM::ERROR_NoError && sysID != 0){
        QString msg = tr("<p><font color='red'>Registration Successful!</font></p>%1<p>System ID:%2</p><p>User ID:%3</p>").arg(message).arg(sysID).arg(user->getUserID());
        ui.labelInfo->setText(msg);

        //errorMessage = tr("Registration Successful!");
        //ui.labelInfo->setText(errorMessage);
        //QMessageBox::information(this, tr("Registration Successful"), errorMessage + QString("\n%1").arg(message));
    }else{

        switch(errorType){
        case IM::ERROR_SoftwareVersionExpired:
        {
            errorMessage = tr("Application has expired!");
        }
            break;
        case IM::ERROR_IPBanned:
        {
            errorMessage = tr("Your IP address has been banned!");
        }
            break;
        case IM::ERROR_PasswordIncorrect:
        {
            errorMessage = tr("Incorrect Password!");
        }
            break;
        case IM::ERROR_Timeout:
        {
            errorMessage = tr("Request Timeout!");
        }
            break;
        case IM::ERROR_RequestDenied:
        {
            errorMessage = tr("Request Denied!");
        }
            break;
            
        case IM::ERROR_IDEXISTED:
        {
            errorMessage = tr("ID Already Exists!");
        }
            break;
        case IM::ERROR_EMAILEXISTED:
        {
            errorMessage = tr("Email Address Already Exists!");
        }
            break;
            
        default:
        {
            errorMessage = tr("UnKnown Error!");
        }
            break;

        }

        QMessageBox::critical(this, tr("Registration Failed"), errorMessage + QString("\n%1").arg(message));

        ui.pushButtonRegister->setEnabled(true);

    }
    
    ui.labelInfo->show();
    ui.labelInfo->setText(errorMessage);

}

void ApplyForRegistrationWidget::on_pushButtonRegister_clicked(){

    if(!m_registrationModeInfoResponseReceived){
        emit canceled();
        return;
    }

    if(!isUserIDValid()){
        QMessageBox::critical(this, tr("Error"), tr("Invalid User ID!"));
        return;
    }
    
    if(!isPasswordValid()){
        QMessageBox::critical(this, tr("Error"), tr("Invalid Password!"));
        return;
    }
    
    if(ui.lineEditPassword->text() != ui.lineEditRetypePassword->text()){
        QMessageBox::critical(this, tr("Error"), tr("The password and confirmation do not match.\n Please re-enter your password!"));
        ui.lineEditRetypePassword->setFocus();
        ui.lineEditRetypePassword->end(false);
        return;
    }
    
    QByteArray password(ui.lineEditPassword->text().toUtf8());
    password = QCryptographicHash::hash (password, QCryptographicHash::Sha1);

    user->setUserID(ui.lineEditUserID->text());
    user->setPassword(password.toBase64());
    
    //emit registration(ui.lineEditUserID->text(), password.toBase64(), ui.lineEditEmail->text().trimmed());
    emit registration();

    m_registrationResultReceived = false;
    QTimer::singleShot(5000, this, SLOT(registrationTimeout()));
    
    ui.labelInfo->show();
    ui.labelInfo->setText(tr("Registering..."));

    ui.pushButtonRegister->setEnabled(false);

    
} 

bool ApplyForRegistrationWidget::isUserIDValid(){

    QString userID = ui.lineEditUserID->text();
    int pos = 0;
    QRegExpValidator rxValidator(this);
    QRegExp rx("^\\w{5,16}$");
    rxValidator.setRegExp(rx);
    if(rxValidator.validate(userID, pos) != QValidator::Acceptable){
        ui.lineEditUserID->setFocus();
        ui.lineEditUserID->end(false);
        return false;
    } 

    return true;

}


bool ApplyForRegistrationWidget::isPasswordValid(){
    
    QString password = ui.lineEditPassword->text();
//    if(password.trimmed().isEmpty()){
//        return false;
//    }
    
    int pos = 0;
    QRegExpValidator rxValidator(this);
    QRegExp rx("^\\S{8,32}$");
    rxValidator.setRegExp(rx);
    if(rxValidator.validate(password, pos) != QValidator::Acceptable){
        ui.lineEditUserID->setFocus();
        ui.lineEditUserID->end(false);
        return false;  
    }

    return true;

}

void ApplyForRegistrationWidget::requestRegistrationServerInfoTimeout(){

    if(!m_registrationModeInfoResponseReceived){
        //QMessageBox::critical(this, tr("Error"), tr("Timeout!"));
        ui.labelInfo->setText(tr("Timeout! Can't connect to server!"));
    }

}

void ApplyForRegistrationWidget::registrationTimeout(){
 
    if(!m_registrationResultReceived){
        QMessageBox::critical(this, tr("Error"), tr("Timeout!"));
    }
 
}

















} //namespace HEHUI

