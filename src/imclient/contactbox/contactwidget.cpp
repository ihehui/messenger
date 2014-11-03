

#include <QHoverEvent>



#include "contactwidget.h"

#include "../imageresource.h"




namespace HEHUI{


ContactWidget::ContactWidget(Contact *contact, QWidget *parent)
    : QWidget(parent), m_contact(contact)
{
	ui.setupUi(this);

    Q_ASSERT(m_contact);

    timer = 0;
    iconIndex = 0;



    installEventFilter(this);
    setAutoFillBackground(true);

    updateContactToUI();
    setExpanded(false);


}

ContactWidget::~ContactWidget()
{

    if(timer){
        timer->stop();
        delete timer;
    }


}

void ContactWidget::updateContactToUI(){

    ui.pushButtonFace->setIcon(ImageResource::createIconForContact(m_contact->getFace(), m_contact->getOnlineState()));
    ui.labelDisplayName->setText(m_contact->displayName());
}

void ContactWidget::setExpanded(bool expand){

    if(expand){
        ui.frameTools->show();
        ui.pushButtonFace->setIconSize(QSize(36, 36));
        setBackgroundRole(QPalette::Highlight);

    }else{
        ui.frameTools->hide();
        ui.pushButtonFace->setIconSize(QSize(22, 22));
        setBackgroundRole(QPalette::Base);
    }

    updateGeometry();

}

bool ContactWidget::isExpanded(){
    return ui.frameTools->isVisible();
}

bool ContactWidget::isMouseUnderFace(){
    QPoint mousePoint = mapFromGlobal(QCursor::pos());
    return ui.pushButtonFace->rect().contains(mousePoint);
}

void ContactWidget::flashFace(bool flash){

    if(flash){
        if(!timer){
            timer = new QTimer(this);
            timer->setInterval(500);
            connect(timer, SIGNAL(timeout()), this, SLOT(slotFlashFace()));
        }

        timer->start();
    }else{
        if(timer){
            //disconnect(timer, SIGNAL(timeout()), 0, 0);
            timer->stop();
            delete timer;
            timer = 0;

            ui.pushButtonFace->setIcon(ImageResource::createIconForContact(m_contact->getFace(), m_contact->getOnlineState()));
        }
    }

}


bool ContactWidget::eventFilter(QObject *obj, QEvent *event)
{
    //qDebug()<<"ContactWidget::eventFilter:"<<event->type();


    switch (event->type()) {
    case QEvent::Enter:
    {
        //qDebug()<<"----QEvent::Enter"<<" "<<this;

        if(!isExpanded()){
            setBackgroundRole(QPalette::Midlight);
        }

        return true;
    }
        break;
    case QEvent::Leave:
    {
        //qDebug()<<"----QEvent::Leave"<<" "<<this;

        if(!isExpanded()){
            setBackgroundRole(QPalette::Base);
        }


        return true;
    }
        break;

//    case QEvent::MouseButtonPress:
//    {
//        setBackgroundRole(QPalette::Highlight);
//        qDebug()<<"----QEvent::MouseButtonPress"<<" "<<this;
//        return false;
//    }
//        break;

    default:
        break;
    }

    return QObject::eventFilter(obj, event);

}

//bool ContactWidget::event(QEvent *event){

//    switch(event->type()){

//    case QEvent::Enter:
//    {
//        QHoverEvent *e = static_cast<QHoverEvent *>(event);
//        qDebug()<<"----QEvent::HoverEnter";
//    }
//        break;

//    default:
//        break;

//    }

//    qDebug()<<"-----1-----:"<<event->type();

//    return QWidget::event(event);

//}


void ContactWidget::slotFlashFace(){
    iconIndex++;
    if(iconIndex > 1){
        iconIndex = 0;
    }

    if(iconIndex){
        ui.pushButtonFace->setIcon(ImageResource::createIconForContact(m_contact->getFace(), m_contact->getOnlineState()));
    }else{
        ui.pushButtonFace->setIcon(ImageResource::emptyIcon());
    }


}



} //namespace HEHUI
