/*
 * clientnetworkmanager.h
 *
 *  Created on: 2009-9-14
 *      Author: 贺辉
 */

#ifndef CLIENTRESOURCEMANAGER_H_
#define CLIENTRESOURCEMANAGER_H_

#include <QObject>

#include "../../sharedim/resourcesmanager.h"


#include "HHSharedCore/huser.h"
#include "HHSharedCore/hsingleton.h"

namespace HEHUI {


class ClientResourcesManager: public ResourcesManager, public Singleton<ClientResourcesManager>
{
    Q_OBJECT

    friend class Singleton<ClientResourcesManager> ;

public:
    ClientResourcesManager(QObject *parent = 0);
//    ClientResourcesManager(NetworkType type, CommunicationMode mode, QObject *parent);
    virtual ~ClientResourcesManager();



signals:



public slots:



private slots:

private:



};

} //namespace HEHUI

#endif /* CLIENTRESOURCEMANAGER_H_ */
