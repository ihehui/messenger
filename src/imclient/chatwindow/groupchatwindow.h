#ifndef GROUPCHATWINDOW_H
#define GROUPCHATWINDOW_H

#include <QWidget>
#include "ui_groupchatwindow.h"

#include "messagehistoryview.h"

namespace HEHUI {

class GroupChatWindow : public QWidget
{
    Q_OBJECT

public:
    GroupChatWindow(InterestGroup *interestGroup, QWidget *parent = 0);
    ~GroupChatWindow();

    InterestGroup * interestGroup();

    bool isDownloadingImage(const QString &imageName);
    void processImageDownloadResult(const QString &imageName, bool downloaded);

    void contactOnlineStateChanged(Contact *contact);


    void memberJoinedOrQuitted(const QString &memberID, bool join);

//    QSize sizeHint();


protected:
    void closeEvent(QCloseEvent * event);

signals:
    void sendMsgButtonClicked(InterestGroup *interestGroup, const QString &message, const QStringList &imageList);
    void toBeDstroyed();
    void signalRequestDownloadImage(const QString &contactID, const QString &imageName);
    void signalCloseWindow();

    void signalRequestGrouptHistoryMessage(const QString &startTime, const QString &endTime, const QString &content, bool requestBackword, quint32 groupID);


public slots:
    void appendMessageReceivedFromContact(const QString &message, Contact *contact, const QString &datetime);

    void processGrouptHistoryMessage(const QStringList &messages, bool canFetchMore);

private slots:
    void memberItemActivated(QListWidgetItem *memberItem);
    void showMessageHistory(bool show);
    void requestGroupHistoryMessage(const QString &startTime, const QString &endTime, const QString &content, bool requestBackword);


    void setPreferedSize();

private:
    QListWidgetItem * memberItem(const QString &memberID);


private:
    Ui::GroupChatWindowClass ui;

    InterestGroup *m_interestGroup;

    MessageHistoryView *m_messageHistoryView;

    QSize m_preferedSize;


};

} //namespace HEHUI

#endif // GROUPCHATWINDOW_H
