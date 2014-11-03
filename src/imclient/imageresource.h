/*
 * imageresource.h
 *
 *  Created on: 2009-11-9
 *      Author: 贺辉
 */

#ifndef IMAGERESOURCE_H_
#define IMAGERESOURCE_H_


#include <QObject>
#include <QIcon>

#include "app_constants_client.h"
#include "../sharedim/constants_global_shared.h"

//#include "../../shared/gui/imageresourcebase.h"
#include "HHSharedGUI/himageresourcebase.h"


namespace HEHUI{

class ImageResource : public HEHUI::ImageResourceBase{
    Q_OBJECT
public:
    ImageResource(QObject *parent = 0);
    virtual ~ImageResource();

    //        static QIcon createIconForContact(const QString &iconIndex = QString(USER_FACE_DEFAULT_INDEX), bool isOnline = false);
    static QIcon createIconForInterestGroup(bool normal = true);

    static QIcon createIconForContact(const QString &iconFileNane, IM::OnlineState state);
    static QString getIconFileFullPathForContact(const QString &iconFileNane) ;

    static QString getIconFilePathForContact2(const QString &iconIndex = QString(USER_FACE_DEFAULT_INDEX), bool isOnline = false) ;
    static QString getIconFilePathForContact2(const QString &iconIndex = QString(USER_FACE_DEFAULT_INDEX), IM::OnlineState state = IM::ONLINESTATE_OFFLINE) ;




};

} //namespace HEHUI

#endif /* IMAGERESOURCE_H_ */
