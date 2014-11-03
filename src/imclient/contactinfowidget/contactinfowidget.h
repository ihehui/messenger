#ifndef CONTACTINFOWIDGET_H
#define CONTACTINFOWIDGET_H

#include <QWidget>
#include "ui_contactinfowidget.h"

#include "../../sharedim/imuserbase.h"


#include "HHSharedGUI/hwidgetbase.h"

namespace HEHUI {

class ContactInfoWidget : public WidgetBase
{
    Q_OBJECT

public:
    ContactInfoWidget(IMUserBase *user, QWidget *parent = 0, Qt::WindowFlags fl = Qt::Dialog);
    ~ContactInfoWidget();


    QSystemTrayIcon *SystemTrayIcon(){return 0;}

    static ContactInfoWidget * getContactInfoWidget(IMUserBase *user);

protected:
    void languageChange();
    void closeEvent(QCloseEvent *);


signals:
    void saveUserInfoRequested(IMUserBase *user);
    void updateUserInfoRequested(IMUserBase *user);

private slots:

    void on_toolButtonUserFace_clicked();
    void on_pushButtonGeneralInformation_clicked();
    void on_pushButtonMoreInformation_clicked();

    void on_pushButtonSettings_clicked();
    void on_pushButtonUpdate_clicked();
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();
    void on_pushButtonApply_clicked();

    void on_lineEditNickName_editingFinished();
    void on_plainTextEditSignature_textChanged();


private:
    void initUI();


private:
    Ui::ContactInfoWidgetClass ui;



    IMUserBase *m_user;

    static QHash<QString, ContactInfoWidget*> m_openWindows;









};

} //namespace HEHUI

#endif // CONTACTINFOWIDGET_H
