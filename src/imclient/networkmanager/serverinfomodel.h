/*
 ****************************************************************************
 * serverinfomodel.h
 *
 * Created On: 2010-5-24
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
 * Last Modified On: 2010-5-24
 * Last Modified By: 贺辉
 ***************************************************************************
 */

#ifndef SERVERINFOMODEL_H_
#define SERVERINFOMODEL_H_


#include <QObject>
#include <QAbstractTableModel>

#include "serverinfo.h"



namespace HEHUI {

class ServerInfoModel : public QAbstractTableModel {
	Q_OBJECT

public:
	ServerInfoModel(QObject *parent = 0);
	virtual ~ServerInfoModel();

    void setServersList(QList<ServerInfo*> serversList);

    int rowCount ( const QModelIndex & parent = QModelIndex() ) const ;
    int	columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const ;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;


private:
	QList<ServerInfo *> serversList;


};

}

#endif /* SERVERINFOMODEL_H_ */
