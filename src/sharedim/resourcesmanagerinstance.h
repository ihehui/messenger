/*
 * resourcesmanagerinstance.h
 *
 *  Created on: 2011-7-23
 *      Author: 贺辉
 */

#ifndef SERVERRESOURCEMANAGERINSTANCE_H_
#define SERVERRESOURCEMANAGERINSTANCE_H_

#include <QObject>

#include "resourcesmanager.h"

#include "HHSharedCore/hsingleton.h"

namespace HEHUI {


class SHAREDIMLIB_API ResourcesManagerInstance: public ResourcesManager, public Singleton<ResourcesManagerInstance>
{
    Q_OBJECT

    friend class Singleton<ResourcesManagerInstance> ;

public:

    ResourcesManagerInstance(QObject *parent = 0);
    virtual ~ResourcesManagerInstance();



signals:



public slots:



private slots:


private:



};

} //namespace HEHUI

#endif /* SERVERRESOURCEMANAGERINSTANCE_H_ */
