/*
 ****************************************************************************
 * applyforregistrationwidget.h
 *
 * Created on: 2010-5-29
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
 * Last Modified on: 2010-5-29
 * Last Modified by: 贺辉
 ***************************************************************************
 */





#ifndef APPLYFORREGISTRATIONWIDGET_H
#define APPLYFORREGISTRATIONWIDGET_H

#include <QWidget>
#include "ui_applyforregistrationwidget.h"
#include "../imuser.h"

#include "../../sharedim/constants_global_shared.h"
#include "../../sharedim/impackets.h"

namespace HEHUI{

class ApplyForRegistrationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ApplyForRegistrationWidget(QWidget *parent = 0);
    ~ApplyForRegistrationWidget();
    
    

protected:
    void changeEvent(QEvent *e);
    
signals:
    void requestRegistrationServerInfo();
    void registration();
    void canceled();
        
public slots:
    void slotProcessRegistrationPacket(const RgeistrationPacket &packet);
    void slotProcessRegistrationServerInfo(quint8 regMode, const QString &extraMessage, const QString &regServerAddress);
    void slotProcessRegistrationResult(quint8 errorTypeCode, quint32 sysID, const QString &message);

private slots:
    void on_pushButtonRegister_clicked();
    bool isUserIDValid();
    bool isPasswordValid();
    
    void requestRegistrationServerInfoTimeout();
    void registrationTimeout();

    
private:
    Ui::ApplyForRegistrationWidgetUI ui;

    IMUser *user;

//    QRegExpValidator *validator;
    
    bool m_registrationModeInfoResponseReceived;
    bool m_registrationResultReceived;


};
} //namespace HEHUI

#endif // APPLYFORREGISTRATIONWIDGET_H
