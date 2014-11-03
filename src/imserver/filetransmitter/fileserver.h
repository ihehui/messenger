#ifndef FILESERVER_H
#define FILESERVER_H

#include <QObject>
#include <QHostAddress>


namespace HEHUI {


class FileServer : public QObject
{
    Q_OBJECT
public:
    FileServer(const QString &hostName, quint16 port, const QHostAddress &address, QObject *parent = 0);

    void setHostName(const QString &hostName);
    QString getHostName();
    void setPort(quint16 port);
    quint16 getPort();
    void setHostAddress(const QHostAddress &address);
    QHostAddress getHostAddress();

private:
    QString m_hostName;
    quint16 m_port;
    QHostAddress m_hostAddress;

};

} //namespace

#endif // FILESERVER_H
