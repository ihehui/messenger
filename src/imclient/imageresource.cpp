/*
 * imageresource.cpp
 *
 *  Created on: 2009-11-9
 *      Author: 贺辉
 */


#include <QPainter>
#include <QFile>
#include <QDebug>



#include "imageresource.h"

#include "settings.h"


namespace HEHUI{


ImageResource::ImageResource(QObject *parent)
    :HEHUI::ImageResourceBase(parent)

{
    // TODO Auto-generated constructor stub

}

ImageResource::~ImageResource() {
    // TODO Auto-generated destructor stub
}

//QIcon ImageResource::createIconForContact(const QString &iconIndex, bool isOnline){
//	QString fileName;
//	QString idx = iconIndex;

//	if(idx.isEmpty()){
//                idx = QString(USER_FACE_DEFAULT_INDEX);
//	}

//	if(isOnline){
//                fileName = QString(USER_FACE_FILE_PATH_PREFIX) + idx + ".png";
//	}else{
//                fileName = QString(USER_FACE_FILE_PATH_PREFIX) +idx + "off.png";
//	}


//	return QIcon(fileName);

//}


QIcon ImageResource::createIconForInterestGroup(bool normal){

    static const QString iconPath = ":/resources/images/group.png";
    static const QIcon groupIcon(iconPath);

    if(!normal){
        QIcon icon;
        QSize size = QImage(iconPath).size();
        QPixmap pixmap = groupIcon.pixmap(size, QIcon::Disabled);
        icon.addPixmap(pixmap);
        return icon;
    }

    return  groupIcon;
}

QIcon ImageResource::createIconForContact(const QString &iconFileNane, IM::OnlineState state){


    QString normalIconPath = getIconFileFullPathForContact(iconFileNane);

    QString pngFilePath = "";
    switch(state){
    case IM::ONLINESTATE_ONLINE:
    {
        return QIcon(normalIconPath);
    }
        break;
    case IM::ONLINESTATE_OFFLINE:
    {
        QIcon icon;
        QSize size = QImage(normalIconPath).size();
        QPixmap pixmap = QIcon(normalIconPath).pixmap(size, QIcon::Disabled);
        icon.addPixmap(pixmap);
        return icon;
    }
        break;
    case IM::ONLINESTATE_INVISIBLE:
    {
        pngFilePath = QString(RESOURCE_PATH) + QString(PNG_FILE_PATH_INVISIBLE);
    }
        break;
    case IM::ONLINESTATE_BUSY:
    {
        pngFilePath = QString(RESOURCE_PATH) + QString(PNG_FILE_PATH_BUSY);
    }
        break;
    case IM::ONLINESTATE_AWAY:
    {
        pngFilePath = QString(RESOURCE_PATH) + QString(PNG_FILE_PATH_AWAY);
    }
        break;
    default:
        return QIcon(normalIconPath);
    }

    //    qWarning()<<"---------fileName:"<<QFileInfo(pngFilePath).fileName();
    //    qWarning()<<"---------exists:"<<QFile(pngFilePath).exists();

    QImage image(normalIconPath), pngImage(pngFilePath);
    QPainter painter(&image);
    painter.drawImage(0, 0, pngImage);
    //image.save("icon.png");

    QPixmap pixmap;
    pixmap.convertFromImage(image);
    QIcon icon;
    icon.addPixmap(pixmap);
    return icon;


}

QString ImageResource::getIconFileFullPathForContact(const QString &iconFileNane ){

    QString fullIconPath;
    if(iconFileNane.size() < 3){
        fullIconPath = QString(USER_FACE_FILE_PATH_PREFIX) + "/" + iconFileNane + ".png";
    }else{
        fullIconPath = Settings::instance()->getCustomFaceDir() + "/" + iconFileNane;
    }

    if(!QFile::exists(fullIconPath)){
        fullIconPath = QString(USER_FACE_FILE_PATH_PREFIX) + "/" + USER_FACE_DEFAULT_INDEX + ".png";
    }

    return fullIconPath;

}


QString ImageResource::getIconFilePathForContact2(const QString &iconIndex, bool isOnline) {
    QString filePath;
    QString idx = iconIndex;

    if(idx.isEmpty()){
        idx = QString(USER_FACE_DEFAULT_INDEX);
    }

    if(isOnline){
        filePath = QString(USER_FACE_FILE_PATH_PREFIX) + idx + ".png";
    }else{
        filePath = QString(USER_FACE_FILE_PATH_PREFIX) +idx + "off.png";
    }

    return filePath;

}

QString ImageResource::getIconFilePathForContact2(const QString &iconIndex, IM::OnlineState state){

    QString filePath;
    QString idx = iconIndex;

    if(idx.isEmpty()){
        idx = QString(USER_FACE_DEFAULT_INDEX);
    }


    switch(state){
    case IM::ONLINESTATE_ONLINE:
    {
        filePath = QString(USER_FACE_FILE_PATH_PREFIX) + idx + ".png";
    }
        break;
    case IM::ONLINESTATE_OFFLINE:
    case IM::ONLINESTATE_INVISIBLE:
    {
        filePath = QString(USER_FACE_FILE_PATH_PREFIX) +idx + "off.png";
    }
        break;
        //    case IM::ONLINESTATE_INVISIBLE:
        //    {

        //    }
        //        break;
    case IM::ONLINESTATE_BUSY:
    {
        filePath = QString(USER_FACE_FILE_PATH_PREFIX) +idx + "busy.png";
    }
        break;
    case IM::ONLINESTATE_AWAY:
    {
        filePath = QString(USER_FACE_FILE_PATH_PREFIX) +idx + "away.png";
    }
        break;
    default:
        filePath = QString(USER_FACE_FILE_PATH_PREFIX) +idx + "off.png";
    }

    return filePath;

}

















} //namespace HEHUI

