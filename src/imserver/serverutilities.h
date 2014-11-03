/*
 * serverutilities.h
 *
 *  Created on: 2009-12-29
 *      Author: 贺辉
 */

#ifndef SERVERUTILITIES_H_
#define SERVERUTILITIES_H_


#include <QSqlQuery>



#include "../sharedim/constants_global_shared.h"

//#include "../../../shared/core/global_core.h"

#include "HHSharedCore/hutilities.h"
#include "HHSharedNetwork/hnetworkutilities.h"
#include "HHSharedCore/hdatabaseutility.h"


namespace HEHUI {

class ServerUtilities: public Utilities, public HEHUI::NetworkUtilities {
public:
    ServerUtilities();
    virtual ~ServerUtilities();

    static QByteArray generateSessionEncryptionKey();

    static QSqlQuery queryDatabase(const QString & queryString, bool localConfigDatabase) ;

    static QSqlQuery queryDatabase(const QString & queryString, const QString &connectionName, const QString &driver,
                                   const QString &host, int port, const QString &user, const QString &passwd,
                                   const QString &databaseName, HEHUI::DatabaseType databaseType) ;

    //        static QString databaseColumnName(IM::PropertyID propertyID);



};

}

#endif /* SERVERUTILITIES_H_ */
