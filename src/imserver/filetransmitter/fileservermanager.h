#ifndef FILESERVERMANAGER_H
#define FILESERVERMANAGER_H

#include <QObject>
#include <QHostAddress>

namespace HEHUI {


class FileServer;

//////////////////////////////////////
class FileServerManager : public QObject
{
    Q_OBJECT
public:
    explicit FileServerManager(QObject *parent = 0);
    ~FileServerManager();

    void addFileServer(const QString &hostName, quint16 port, const QHostAddress &address);
    void removeFileServer(const QString &hostName);
    bool selectFileServer(QString *hostName, quint16 *port, QHostAddress *address);


signals:


public slots:


private:
    QHash<QString /*Host Name*/, FileServer*> m_servers;


};

}

#endif // FILESERVERMANAGER_H
