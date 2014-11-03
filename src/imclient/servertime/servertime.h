/*
 ****************************************************************************
 * filename
 *
 * Created on: 2013-10-10
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
 * Last Modified on: 2013-10-10
 * Last Modified by: 贺辉
 ***************************************************************************
 */





#ifndef SERVERTIME_H
#define SERVERTIME_H

#include <QObject>
#include <QDateTime>
#include <QTimer>


namespace HEHUI {


class ServerTime : public QObject
{
    Q_OBJECT

public:

    static ServerTime * instance();
    static void destoryInstance();

    void startSync(uint time_t);
    QDateTime time();
    QString timeString(const QString &format = "yyyy-MM-dd hh:mm:ss") const;


private:
    ServerTime();
    ~ServerTime();


private slots:
    void timeout();

private:

    static ServerTime *m_instance;

    QDateTime m_startTime;

    QTimer *m_syncTimer;
    quint64 count;



};

} //namespace HEHUI

#endif // SERVERTIME_H
