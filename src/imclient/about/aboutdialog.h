#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
//#include "../ui/ui_aboutdialog.h"
#include "ui_aboutdialog.h"

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

private:
	void retranslateUi();

protected:
	void languageChange();


private:
    Ui::AboutDialogUI ui;


};

#endif // ABOUTDIALOG_H
