#include "aboutdialog.h"


AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	retranslateUi();

}

AboutDialog::~AboutDialog()
{

}

void AboutDialog::languageChange(){

	ui.retranslateUi(this);
	retranslateUi();

}

void AboutDialog::retranslateUi(){

        QString appInfo = QString("<center><h3>%1</h3><p>Build %2 </p></center>")
                                                        .arg(APP_NAME).arg(APP_VERSION);
	appInfo += QString(tr("<p>Author: %1</p><p>License: <a href=\"http://www.gnu.org/copyleft/lesser.html\">LGPL</a></p><p>Email: %2</p>"))
							.arg(APP_AUTHOR).arg(APP_AUTHOR_EMAIL);
	appInfo += QString(tr("<p><center>The program is provided AS IS with NO WARRANTY OF ANY KIND.</center></p>"));
	//appInfo += QString(tr("<p><center></center></p>"));

	ui.generalInfoLabel->setText(appInfo);

}


