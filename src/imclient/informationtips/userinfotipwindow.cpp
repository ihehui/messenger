#include "userinfotipwindow.h"

#include <QDesktopServices>

#include <QDebug>


namespace HEHUI {


UserInfoTipWindow::UserInfoTipWindow(QWidget *parent, Qt::WindowFlags fl)
    : QWidget(parent, fl)
{
    ui.setupUi(this);

    //setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::SplashScreen | Qt::WindowStaysOnTopHint);
    //setWindowFlags(Qt::SplashScreen);

    m_contact = 0;

    timer = new QTimer(this);
    timer->setInterval(3000);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(hideUserInfoTip()));

}

UserInfoTipWindow::~UserInfoTipWindow()
{

}

//bool UserInfoTipWindow::event(QEvent *event){

//    switch(event->type()){
//        case QEvent::WindowDeactivate:
//        {
//            hideUserInfoTip();
//            return true;
//        }
//        break;
//        default:
//            break;
//    }

//    return false;

//}

void UserInfoTipWindow::showUserInfoTip(Contact *contact, const QPoint &global_pos){
    qDebug()<<"--UserInfoTipWindow::showUserInfoTip(...)";


    if(!contact){
        return;
    }
    if(this->m_contact == contact){
        return;
    }

    hideUserInfoTip();

    this->m_contact = contact;


    ui.labelUserIDInfo->setText(contact->getNickName() + "("  + contact->getUserID() + ")");
    ui.labelDescription->setText(contact->getdescription());

    if( (contact->getOnlineState() != IM::ONLINESTATE_INVISIBLE) && (contact->getOnlineState() != IM::ONLINESTATE_OFFLINE)){
        QString info = QString("IP: %1").arg(contact->getLastLoginExternalHostAddress());
        ui.labelExtraInfo->setText(info);
    }

    if(contact->getBusinessEmailAddress().trimmed().isEmpty()){
        ui.toolButtonEmail->setEnabled(false);
    }else{
        ui.toolButtonEmail->setEnabled(true);
    }

    move(global_pos);

    show();
    update();

    timer->start();


}

void UserInfoTipWindow::hideUserInfoTip(){
    qDebug()<<"--UserInfoTipWindow::hideUserInfoTip()";

    timer->stop();

    m_contact = 0;
    ui.labelUserIDInfo->clear();
    ui.labelDescription->clear();
    ui.labelExtraInfo->clear();
    ui.toolButtonEmail->setEnabled(false);

    hide();

}

void UserInfoTipWindow::sendEmail(){
    QString email = m_contact->getBusinessEmailAddress();

    QDesktopServices::openUrl(QUrl(QString("mailto:%1").arg(email)));



}


} //namespace HEHUI
