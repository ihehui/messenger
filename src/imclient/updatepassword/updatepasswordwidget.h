#ifndef UPDATEPASSWORDWIDGET_H
#define UPDATEPASSWORDWIDGET_H

#include <QWidget>

#include "packetmanager/imclientpacketsparser.h"

#include "../../sharedim/constants_global_shared.h"
#include "../../sharedim/impackets.h"





namespace Ui {
class UpdatePasswordWidgetUI;
}

namespace HEHUI
{

class UpdatePasswordWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UpdatePasswordWidget(IMClientPacketsParser *clientPacketsParser, QWidget *parent = 0);
    ~UpdatePasswordWidget();

protected:
    void changeEvent(QEvent *e);

signals:
    void closed();

public slots:
    void slotProcessUpdatePasswordResult(const UpdatePasswordPacket &packet);
    void slotOkClicked();


private:
    Ui::UpdatePasswordWidgetUI *ui;

    IMClientPacketsParser *m_clientPacketsParser;

    UpdatePasswordPacket::AuthMode m_authMode;

};

} //namespace HEHUI

#endif // UPDATEPASSWORDWIDGET_H
